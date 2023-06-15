// Loader for MC121.01

#include <iostream>

#include "mc12101load.h"
#include "hal/hal.h"
#include "hal/hal-options.h"

#include "nmrun.h"

#ifdef NM_IO_ServiceX
//#include "io_host_win.h"
#include "io_host.h"
#endif

using namespace std;

unsigned int Board_number = 0;
unsigned int Core_number = 0;

unsigned long Serial_number;

// PL_Board *Board;
HalBoard *Board;
HalAccess *Access;

BOOL f_Open_by_sn = FALSE;
BOOL f_Dump_start = FALSE;
BOOL f_Dump_finish = FALSE;
BOOL f_Run_program = TRUE;
BOOL f_Perform_reset = FALSE;
BOOL f_Run_program_special = FALSE;
BOOL f_Return_NM_ret_val = FALSE;
BOOL Quiet = FALSE; // Suppress output (except errors)

#ifdef NM_IO_ServiceX
BOOL f_Use_print = FALSE;
#endif

char *ABS_file_name = NULL;
char *ABS_file_args = NULL;

#ifdef NM_RPC
char *RPC_server_IP = NULL;
int RPC_server_port = 5557;
#endif

DWORD Timeout_value = 0; // (mS)

PL_Addr Dump_memory_begin = Dump_memory_begin_default;
PL_Addr Dump_memory_size = Dump_memory_size_default;
unsigned int Dump_field_bits = 32; // Dump words length (must be 2 in power and not greater 64)

BYTE recv_flags = 0x0; // (disp, sect, size, addr)
char *recv_file_name = NULL;
PL_Addr recv_addr = Dump_memory_begin_default;
unsigned long recv_size = Dump_memory_size_default;
char *recv_sect_name;
PL_Addr recv_disp = 0x0;

BYTE send_flags = 0x0; // (disp, sect, size, addr)
char *send_file_name = NULL;
PL_Addr send_addr = Dump_memory_begin_default;
unsigned long send_size = 0; // 0 - Read entire file
char *send_sect_name;
PL_Addr send_disp = 0x0;

BOOL f_Sync = FALSE;
int Sync_send, Sync_recv;

int main(int argc, char **argv)
{
	Display_version(argc, argv);
	Parse_command_line(argc, argv);

	HalBoardOptions *board_options = halCreateBoardOptions();
	halSetBoardOption(board_options, HAL_BOARD_TYPE, MC12101);

	try
	{
#ifdef NM_RPC
		if (RPC_server_IP)
		{
			//PL_ConnectToHost(RPC_server_IP, RPC_server_port);
			halSetBoardOption(board_options, HAL_SERVER_IP, RPC_server_IP);
			halSetBoardOption(board_options, HAL_SERVER_PORT, RPC_server_port);
			halSetBoardOption(board_options, HAL_SERVER_ENABLED, 1);
			//board_options
		}
#endif

		unsigned int Boards_count, Firmware_version_major, Firmware_version_minor;

		//Boards_count = halGetBoardCount(board_options);
		// if (PL_GetBoardCount(&Boards_count) != PL_OK)
		// {
		// 	cout << "ERROR: Failed get count of boards!\n";
		// 	exit(1);
		// }

		// if (!Boards_count)
		// {
		// 	cout << "ERROR: No boards found!\n";
		// 	exit(1);
		// }

		if (!f_Open_by_sn)
		{
			// if (Board_number >= Boards_count)
			// {
			// 	cout << "ERROR: Invalid board number!\n";
			// 	exit(1);
			// }

			halSetBoardOption(board_options, HAL_BOARD_NUMBER, Board_number);

			if( (Board = halGetBoardOpt(board_options) ) == 0){
				cout << "ERROR: Failed open board " << Board_number << "!\n";
				exit(1);
			}

			// if (PL_GetBoardDesc(Board_number, &Board) != PL_OK)
			// {
			// 	cout << "ERROR: Failed open board " << Board_number << "!\n";
			// 	exit(1);
			// }
		}
		else
		{
			bool Board_found = false;

			cout << "WARNING: get board from serial numbder is disabled" << "!\n";

			// for (unsigned int i = 0; i < Boards_count; i++)
			// {
			// 	unsigned long sn;

			// 	if (PL_GetBoardDesc(i, &Board) != PL_OK)
			// 	{
			// 		cout << "WARNING: Failed open board " << i << "!\n";
			// 		continue;
			// 	}

			// 	if (PL_GetSerialNumber(Board, &sn) == PL_OK)
			// 	{
			// 		if (sn == Serial_number)
			// 		{
			// 			Board_found = true;
			// 			break;
			// 		}
			// 	}
			// 	else
			// 	{
			// 		cout << "WARNING: Failed get serial number of board " << i << "!\n";
			// 	}

			// 	if (PL_CloseBoardDesc(Board) != PL_OK)
			// 	{
			// 		cout << "WARNING: Failed close board descriptor of board " << i << "!\n";
			// 	}
			// }

			if (!Board_found)
			{
				cout << "ERROR: Board with serial number " << Serial_number << " not found!\n";
				exit(1);
			}
		}

		halDestroyBoardOptions(board_options);

		if (f_Perform_reset)
		{
			cout << "Performing reset...\n";

			if (halResetBoard(Board) == HAL_OK)
			{
				cout << "Done.\n";

				if (halLoadInitCode(Board) != PL_OK)
				{
					cout << "ERROR: Failed load init code!\n";
					halCloseBoard(Board);
					exit(1);
				}
			}
			else
				cout << "WARNING: Failed reset board!\n";
		}

		if (!Quiet)
			if (halGetFirmwareVersion(Board, &Firmware_version_major, &Firmware_version_minor) == HAL_OK)
				cout << "Firmware v" << Firmware_version_major << "." << Firmware_version_minor << "\n";
			else
				cout << "WARNING: Failed get firmware version!\n";

		
		HalAccessOptions *access_options = halCreateAccessOptions();
		halSetAccessOption(access_options, HAL_CORE, Core_number);
		if ( (Access = halGetAccessOpt(Board, access_options) ) == 0)
		{
			cout << "ERROR: Failed access core " << Core_number << "!\n";
			halCloseBoard(Board);
			exit(1);
		}
		halDestroyAccessOptions(access_options);

		if (f_Dump_start)
		{
			if (Dump_shared_memory(".dms"))
				cout << "ERROR: Failed create dump of memory!\n";
		}

		if (send_file_name)
		{
			if (Send_binary_file_to_board())
			{
				cout << "ERROR: Failed copy binary file to board!\n";
				halCloseAccess(Access);
				halCloseBoard(Board);
				exit(1);
			}
		}

		PL_Word Status, ABS_return_value;

		if (f_Run_program)
		{
			int ret = 1;

			if (!f_Run_program_special)
				ret = halLoadProgramFileArgs(Access, ABS_file_name, ABS_file_args);
			// else
			// 	ret = PL_LoadProgramFileArgs_Int(Access, ABS_file_name, ABS_file_args);

			if (ret != PL_OK)
			{
				if (ret == PL_FILE)
					cout << ABS_file_name << " :: ERROR: Failed find user program file!\n";
				else if (ret == PL_BADADDRESS)
					cout << ABS_file_name << " :: ERROR: Invalid user program configuration!\n";
				else
					cout << ABS_file_name << " :: ERROR: Failed load program into core " << Core_number << "!\n";
				halCloseAccess(Access);
				halCloseBoard(Board);
				exit(1);
			}

			if (!Quiet)
				cout << "Start user program on core " << Core_number << "...\n" << flush;

			if (f_Run_program_special)
			{
				halCloseAccess(Access);
				halCloseBoard(Board);
				exit(0);
			}

#ifdef NM_IO_ServiceX
			HalAccess *servAccess;
			HalAccessOptions *serv_access_options = halCreateAccessOptions();
			halSetAccessOption(serv_access_options, HAL_CORE, Core_number);			
			HalIO *nms = NULL;

			if (f_Use_print)
			{
				if ( (servAccess = halGetAccessOpt(Board, serv_access_options)) == 0)
				{
					cout << "ERROR: Failed access processor for printf service!\n";
					halCloseAccess(Access);
					halCloseBoard(Board);
					exit(1);
				}

				try
				{
					FILE *F = NULL;
					nms = halStartIO(servAccess, ABS_file_name, stdout);
					//nms = IO_ServiceStart(ABS_file_name, servAccess);
					//nms = new IO_Service(ABS_file_name, servAccess);
				}
				catch (...)
				{
					cout << "WARNING: IO_Service ERROR!\n";
					halCloseAccess(servAccess);
					nms = NULL;
				}
				
			}
			halDestroyAccessOptions(serv_access_options);
#endif

			if (f_Sync)
			{
				if (!Quiet)
					cout << "Call barrier synchronization...\n" << flush;

				int error;
				Sync_recv = halSync(Access, Sync_send, &error);
				if (error == PL_OK)
				{
					cout << ABS_file_name << " :: Barrier synchronization value: ";
					cout << dec << Sync_recv;
					cout << " = 0x" << hex << uppercase << Sync_recv << ".\n";
				}
				else
					cout << "ERROR: Failed call barrier synchronization!\n";
			}

			DWORD Start_time = GetTickCount();

			// Waiting end program
			while (1)
			{
				int error;
				Status = halGetStatus(Access, &error);
				if (error != PL_OK)
				{
					cout << "ERROR: Failed get program status of core " << Core_number << "!\n";
#ifdef NM_IO_ServiceX
					if (nms)
					{
						delete nms;
						halCloseAccess(servAccess);
					}
#endif
					halCloseAccess(Access);
					halCloseBoard(Board);
					exit(1);
				}

				if (Status & PROGRAM_FINISHED)
					break;

				if (Timeout_value)
					if ((GetTickCount() - Start_time) > Timeout_value)
						break;

				Sleep(1);
			}

			if (Status & PROGRAM_FINISHED)
			{
				int error;
				ABS_return_value = halGetResult(Access, &error);
				if (error != PL_OK)
				{
					cout << "ERROR: Failed get program return value of core " << Core_number << "!\n";
#ifdef NM_IO_ServiceX
					if (nms)
					{
						halStopIO(nms);
						halCloseAccess(servAccess);
					}
#endif
					halCloseAccess(Access);
					halCloseBoard(Board);
					exit(1);
				}
			}
			else
				cout << ABS_file_name << " :: ERROR: Timeout for program on core " << Core_number << "!\n";

#ifdef NM_IO_ServiceX
			//if (nms)
			if (nms)
			{
				halStopIO(nms);

				if (halCloseAccess(servAccess) != PL_OK)
				{
					cout << "WARNING: Failed close processor descriptor for printf service!\n";
				}
			}
#endif
		} // f_Run_program

		if (f_Dump_finish)
		{
			if (Dump_shared_memory(".dme"))
				cout << "ERROR: Failed create dump of memory!\n";
		}

		if (recv_file_name)
		{
			if (Receive_binary_dump_from_board())
				cout << "ERROR: Failed create binary dump of memory!\n";
		}

		if (halCloseAccess(Access) != PL_OK)
		{
			cout << "WARNING: Failed close processor descriptor!\n";
		}

		if (halCloseBoard(Board) != PL_OK)
		{
			cout << "WARNING: Failed close board descriptor!\n";
		}

		if (f_Run_program && !Quiet && Status & PROGRAM_FINISHED)
		{
			cout << ABS_file_name << " :: Core " << Core_number << " return ";
			cout << dec << ABS_return_value;
			cout << " = 0x" << hex << uppercase << ABS_return_value << ".\n";
		}

		if (f_Return_NM_ret_val)
			return ABS_return_value;
	}
	catch (...)
	{
		cout << "ERROR: Unhandled exception!\n";
		exit(1);
	}

	return 0;
}
