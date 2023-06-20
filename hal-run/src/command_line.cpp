// Loader for MC121.01
// Processing command line parameters

#include <iostream>
#include <iomanip>
#include <sstream>
#include <assert.h>

#include "mc12101load.h"
#include "hal/hal.h"

#include "nmrun.h"

using namespace std;

extern unsigned int Board_number;
extern unsigned int Core_number;

extern unsigned long Serial_number;

extern BOOL f_Open_by_sn;
extern BOOL f_Dump_start;
extern BOOL f_Dump_finish;
extern BOOL f_Run_program;
extern BOOL f_Perform_reset;
extern BOOL f_Run_program_special;
extern BOOL f_Return_NM_ret_val;
extern BOOL Quiet;
extern int Board_type;

#ifdef NM_IO_ServiceX
extern BOOL f_Use_print;
#endif

extern char *ABS_file_name;
extern char *ABS_file_args;

#ifdef NM_RPC
extern char *RPC_server_IP;
extern int RPC_server_port;
bool RPC_server_port_specified = false;
#endif

extern DWORD Timeout_value;

extern PL_Addr Dump_memory_begin;
extern PL_Addr Dump_memory_size;
extern unsigned int Dump_field_bits;

extern BYTE recv_flags;
extern char *recv_file_name;
extern PL_Addr recv_addr;
extern unsigned long recv_size;
extern char *recv_sect_name;
extern PL_Addr recv_disp;

extern BYTE send_flags;
extern char *send_file_name;
extern PL_Addr send_addr;
extern unsigned long send_size;
extern char *send_sect_name;
extern PL_Addr send_disp;

extern BOOL f_Sync;
extern int Sync_send;

void Display_version(int argc, char **argv)
{
	for (int i = 1; i < argc; i++)
		if ((argv[i][0] == '-') && (argv[i][1] == 'q'))
			return;

	cout << "Batch loader for " << DEVICE_NAME_STR;
#ifdef NM_RPC
	cout << " via RPC server";
#endif
	cout << " v" << RUN_VERSION_MAJOR << "." << RUN_VERSION_MINOR;
	cout << ". " COPYRIGHT_STR "\n";
}

void Usage(void)
{
	cout << "Usage: " DEVICE_NAME "run";
#ifdef NM_RPC
	cout << "_rpc";
#endif
	cout << " <FILE.abs> [OPTION(S)]\n";

	cout << "Options:\n";
	cout << "-s\tDump shared memory before program start\n";
	cout << "-e\tDump shared memory after program finish\n";
	cout << "-m<num>\tSpecify dump begin address (hex)";
	cout << " (default: 0x" << hex << uppercase << setw(8) << setfill('0') << Dump_memory_begin_default << ")\n";
	cout << "-l<num>\tSpecify dump size (hex)";
	cout << " (default: 0x" << hex << uppercase << setw(8) << setfill('0') << Dump_memory_size_default << ")\n";
	cout << "-d<num>\tSpecify length of dump words in bits (1; 2; 4; ...; 64) (default: 32)\n";
	cout << "-r\tDo not run program\n";
	cout << "-t<num>\tTimeout in milliseconds (0 - " << dec << MAX_TIMEOUT << ") (default: 0 - no timeout)\n";
	cout << "-b<num>\tWhich board to use (default: 0)\n";
	cout << "-a<num>\tWhich core to use (default: 0)\n";
	cout << "-n<num>\tOpen board by serial number\n";
	cout << "-R\tPerform soft reset (Firmware version must be 6.0 or higher; jumper must be set on X10 on pins 5-6.)\n";
#ifdef NM_IO_ServiceX
	cout << "-p\tOutput print from NM\n";
#endif
	cout << "-g\tSpecial load for GDB stub\n";
	cout << "-v\tReturn NM return value (if no error occured)\n";
	cout << "-q\tQuiet mode\n";

	cout << "--args=\"List of args\"\n\tSpecify arguments forwarded to main of user program\n";

	cout << "--board=<mc12101|mb7707>\n\tSpecify kind of board\n";
#ifdef NM_RPC
	cout << "--server_ip=<Server_IP_address>\n\tSpecify server IP address\n";
	cout << "--server_port=<num>\n\tSpecify server port (default: " << dec << RPC_server_port << ")\n";
#endif

	cout << "--recv_file_name=<File_name>\n\tMake binary dump after program finish\n";
	cout << "--recv_addr=<num>\n\tSpecify binary dump begin address (hex)";
	cout << " (default: 0x" << hex << uppercase << setw(8) << setfill('0') << Dump_memory_begin_default << ")\n";
	cout << "--recv_size=<num>\n\tSpecify binary dump size (hex)";
	cout << " (default: 0x" << hex << uppercase << setw(8) << setfill('0') << Dump_memory_size_default << ")\n";
	cout << "--recv_sect=<Section_name>\n\tSpecify binary dump begin address via begin of section\n";
	cout << "--recv_disp=<num>\n\tSpecify dump begin address displacement relative begin of section (hex)\n";
	cout << "--send_file_name=<File_name>\n\tCopy binary file to shared memory before program start\n";
	cout << "--send_addr=<num>\n\tSpecify begin address for copying file (hex)";
	cout << " (default: 0x" << hex << uppercase << setw(8) << setfill('0') << Dump_memory_begin_default << ")\n";
	cout << "--send_size=<num>\n\tSpecify size of copying file (hex)";
	cout << " (default: 0 - copying entire file)\n";
	cout << "--send_sect=<Section_name>\n\tSpecify begin address for copying file via begin of section\n";
	cout << "--send_disp=<num>\n\tSpecify begin addr. disp. for copying file rel. begin of sect. (hex)\n";
	cout << "--sync=<num>\n\tCall barrier synchronization function (dec | hex)\n";
}

#define If_this_option(option) \
	if (!(strncmp(ex_opt, option, sizeof(option) - 1)))

int Parse_extended_option(char *ex_opt)
{
	If_this_option("args=")
	{
		if (ABS_file_args)
		{
			cout << "ERROR: Duplicate option 'args'!\n";
			return 1;
		}

		if (strlen(ex_opt) < sizeof("args="))
		{
			cout << "ERROR: No args list specified!\n";
			return 1;
		}

		ABS_file_args = ex_opt + sizeof("args=") - 1;
	}
#ifdef NM_RPC
	else If_this_option("server_ip=")
	{
		if (RPC_server_IP)
		{
			cout << "ERROR: Duplicate option 'server_ip'!\n";
			return 1;
		}

		if (strlen(ex_opt) < sizeof("server_ip="))
		{
			cout << "ERROR: No server IP address specified!\n";
			return 1;
		}

		RPC_server_IP = ex_opt + sizeof("server_ip=") - 1;
	}
	else If_this_option("server_port=")
	{
		if (RPC_server_port_specified)
		{
			cout << "ERROR: Duplicate option 'server_port'!\n";
			return 1;
		}

		istringstream sin(ex_opt + sizeof("server_port=") - 1);
		sin >> dec >> RPC_server_port;
		if ((sin.fail()) || (!(sin.eof())))
		{
			cout << "ERROR: Invalid option '" << ex_opt << "'!\n";
			return 1;
		}

		RPC_server_port_specified = true;
	}
#endif
	else If_this_option("board="){
		istringstream sin(ex_opt + sizeof("board=") - 1);
		string kind;
		sin >> kind;

		if ((sin.fail()) || (!(sin.eof())))
		{
			cout << "ERROR: Invalid option '" << ex_opt << "'!\n";
			return 1;
		}

		if(strcmp(kind.c_str(), "mc12101") == 0){
			Board_type = HAL_MC12101;
		} else if(strcmp(kind.c_str(), "mb7707") == 0){
			Board_type = HAL_MB7707;
		}else {
			cout << "Error: Wrong kind of board";
			return 1;
		}
	}
	else If_this_option("recv_file_name=")
	{
		if (strlen(ex_opt) < sizeof("recv_file_name="))
		{
			cout << "ERROR: No receive file name specified!\n";
			return 1;
		}

		// If receive file name is already setted
		if (recv_file_name)
		{
			cout << "ERROR: Redefinition receive file name!\n";
			return 1;
		}

		// Setting receive file name
		recv_file_name = ex_opt + sizeof("recv_file_name=") - 1;
	}
	else If_this_option("recv_addr=")
	{
		if (recv_flags & 0x1) // 0001b
		{
			cout << "ERROR: Duplicate option 'recv_addr'!\n";
			return 1;
		}

		istringstream sin(ex_opt + sizeof("recv_addr=") - 1);
		sin >> hex >> recv_addr;
		if ((sin.fail()) || (!(sin.eof())))
		{
			cout << "ERROR: Invalid option '" << ex_opt << "'!\n";
			return 1;
		}

		recv_flags |= 0x1; // 0001b
	}
	else If_this_option("recv_size=")
	{
		if (recv_flags & 0x2) // 0010b
		{
			cout << "ERROR: Duplicate option 'recv_size'!\n";
			return 1;
		}

		istringstream sin(ex_opt + sizeof("recv_size=") - 1);
		sin >> hex >> recv_size;
		if ((sin.fail()) || (!(sin.eof())))
		{
			cout << "ERROR: Invalid option '" << ex_opt << "'!\n";
			return 1;
		}

		recv_flags |= 0x2; // 0010b
	}
	else If_this_option("recv_sect=")
	{
		if (recv_flags & 0x4) // 0100b
		{
			cout << "ERROR: Duplicate option 'recv_sect'!\n";
			return 1;
		}

		recv_sect_name = ex_opt + sizeof("recv_sect=") - 1;

		recv_flags |= 0x4; // 0100b
	}
	else If_this_option("recv_disp=")
	{
		if (recv_flags & 0x8) // 1000b
		{
			cout << "ERROR: Duplicate option 'recv_disp'!\n";
			return 1;
		}

		istringstream sin(ex_opt + sizeof("recv_disp=") - 1);
		sin >> hex >> recv_disp;
		if ((sin.fail()) || (!(sin.eof())))
		{
			cout << "ERROR: Invalid option '" << ex_opt << "'!\n";
			return 1;
		}

		recv_flags |= 0x8; // 1000b
	}
	else If_this_option("send_file_name=")
	{
		if (strlen(ex_opt) < sizeof("send_file_name="))
		{
			cout << "ERROR: No send file name specified!\n";
			return 1;
		}

		// If send file name is already setted
		if (send_file_name)
		{
			cout << "ERROR: Redefinition send file name!\n";
			return 1;
		}

		// Setting send file name
		send_file_name = ex_opt + sizeof("send_file_name=") - 1;
	}
	else If_this_option("send_addr=")
	{
		if (send_flags & 0x1) // 0001b
		{
			cout << "ERROR: Duplicate option 'send_addr'!\n";
			return 1;
		}

		istringstream sin(ex_opt + sizeof("send_addr=") - 1);
		sin >> hex >> send_addr;
		if ((sin.fail()) || (!(sin.eof())))
		{
			cout << "ERROR: Invalid option '" << ex_opt << "'!\n";
			return 1;
		}

		send_flags |= 0x1; // 0001b
	}
	else If_this_option("send_size=")
	{
		if (send_flags & 0x2) // 0010b
		{
			cout << "ERROR: Duplicate option 'send_size'!\n";
			return 1;
		}

		istringstream sin(ex_opt + sizeof("send_size=") - 1);
		sin >> hex >> send_size;
		if ((sin.fail()) || (!(sin.eof())))
		{
			cout << "ERROR: Invalid option '" << ex_opt << "'!\n";
			return 1;
		}

		send_flags |= 0x2; // 0010b
	}
	else If_this_option("send_sect=")
	{
		if (send_flags & 0x4) // 0100b
		{
			cout << "ERROR: Duplicate option 'send_sect'!\n";
			return 1;
		}

		send_sect_name = ex_opt + sizeof("send_sect=") - 1;

		send_flags |= 0x4; // 0100b
	}
	else If_this_option("send_disp=")
	{
		if (send_flags & 0x8) // 1000b
		{
			cout << "ERROR: Duplicate option 'send_disp'!\n";
			return 1;
		}

		istringstream sin(ex_opt + sizeof("send_disp=") - 1);
		sin >> hex >> send_disp;
		if ((sin.fail()) || (!(sin.eof())))
		{
			cout << "ERROR: Invalid option '" << ex_opt << "'!\n";
			return 1;
		}

		send_flags |= 0x8; // 1000b
	}
	else If_this_option("sync=")
	{
		if (f_Sync)
		{
			cout << "ERROR: Duplicate option 'sync'!\n";
			return 1;
		}

		istringstream sin(ex_opt + sizeof("sync=") - 1);

		if ((ex_opt[sizeof("sync=") - 1] == '0') && (ex_opt[sizeof("sync=")] == 'x')) // hex input
		{
			sin >> hex >> Sync_send;
			if ((sin.fail()) || (!(sin.eof())))
			{
				cout << "ERROR: Invalid option '" << ex_opt << "'!\n";
				return 1;
			}
		}
		else // dec input
		{
			sin >> dec >> Sync_send;
			if ((sin.fail()) || (!(sin.eof())))
			{
				cout << "ERROR: Invalid option '" << ex_opt << "'!\n";
				return 1;
			}
		}

		f_Sync = TRUE;
	}
	else
	{
		cout << "ERROR: Invalid option '" << ex_opt << "'!\n";
		return 1;
	}

	return 0;
}

#define Check_option_no_param \
	if (argv[i][2] != '\0') \
	{ \
		cout << "ERROR: Invalid option '" << &argv[i][1] << "'!\n"; \
		exit(1); \
	}

void Parse_command_line(int argc, char **argv)
{
	BOOL f_Open_by_bn = FALSE;

	// If no input parameters
	if (argc == 1)
	{
		Usage();
		exit(0);
	}

	// Input parameters enumeration cycle
	for (int i = 1; i < argc; i++)
	{
		// If input parameter is option (begining with '-')
		if (argv[i][0] == '-')
		{
			// If input parameter is extended option (begining with '--')
			if (argv[i][1] == '-')
			{
				if (Parse_extended_option(&argv[i][2]))
					exit(1);

				continue;
			}

			// Checking, option not repeated
			for (int j = 1; j < i; j++)
				if ((argv[j][0] == '-') && (argv[j][1] == argv[i][1]))
				{
					cout << "ERROR: Duplicate option '" << argv[i][1] << "'!\n";
					exit(1);
				}

			switch (argv[i][1])
			{
			case 's':
				Check_option_no_param
				f_Dump_start = TRUE;
				break;
			case 'e':
				Check_option_no_param
				f_Dump_finish = TRUE;
				break;
			case 'm':
			{
				istringstream sin(&(argv[i][2]));
				sin >> hex >> Dump_memory_begin;
				if ((sin.fail()) || (!(sin.eof())))
				{
					cout << "ERROR: Invalid option '" << &argv[i][1] << "'!\n";
					exit(1);
				}
				break;
			}
			case 'l':
			{
				istringstream sin(&(argv[i][2]));
				sin >> hex >> Dump_memory_size;
				if ((sin.fail()) || (!(sin.eof())))
				{
					cout << "ERROR: Invalid option '" << &argv[i][1] << "'!\n";
					exit(1);
				}
				break;
			}
			case 'd':
			{
				istringstream sin(&(argv[i][2]));
				sin >> dec >> Dump_field_bits;
				if ((sin.fail()) || (!(sin.eof())))
				{
					cout << "ERROR: Invalid option '" << &argv[i][1] << "'!\n";
					exit(1);
				}

				if ((!Dump_field_bits) || (Dump_field_bits > 64))
				{
					cout << "ERROR: Invalid length of dump words!\n";
					exit(1);
				}

				// Checking for accordance 2^n
				unsigned int n_2 = Dump_field_bits;
				while (n_2 > 2)
				{
					if (n_2 % 2)
					{
						cout << "ERROR: Invalid length of dump words!\n";
						exit(1);
					}

					n_2 /= 2;
				}

				break;
			}
			case 'r':
				Check_option_no_param
				f_Run_program = FALSE;
				break;
			case 't':
			{
				istringstream sin(&(argv[i][2]));
				sin >> Timeout_value;
				if ((sin.fail()) || (!(sin.eof())))
				{
					cout << "ERROR: Invalid option '" << &argv[i][1] << "'!\n";
					exit(1);
				}
				if (Timeout_value > MAX_TIMEOUT)
				{
					cout << "ERROR: Invalid timeout value!\n";
					exit(1);
				}
				break;
			}
			case 'b':
			{
				istringstream sin(&(argv[i][2]));
				sin >> Board_number;
				if ((sin.fail()) || (!(sin.eof())))
				{
					cout << "ERROR: Invalid option '" << &argv[i][1] << "'!\n";
					exit(1);
				}
				f_Open_by_bn = TRUE;
				break;
			}
			case 'a':
			{
				istringstream sin(&(argv[i][2]));
				sin >> Core_number;
				if ((sin.fail()) || (!(sin.eof())))
				{
					cout << "ERROR: Invalid option '" << &argv[i][1] << "'!\n";
					exit(1);
				}
				break;
			}
			case 'n':
			{
				istringstream sin(&(argv[i][2]));
				sin >> Serial_number;
				if ((sin.fail()) || (!(sin.eof())))
				{
					cout << "ERROR: Invalid option '" << &argv[i][1] << "'!\n";
					exit(1);
				}
				f_Open_by_sn = TRUE;
				break;
			}
			case 'R':
				Check_option_no_param
				f_Perform_reset = TRUE;
				break;
#ifdef NM_IO_ServiceX
			case 'p':
				Check_option_no_param
				f_Use_print = TRUE;
				break;
#endif
			case 'g':
				Check_option_no_param
				f_Run_program_special = TRUE;
				break;
			case 'v':
				Check_option_no_param
				f_Return_NM_ret_val = TRUE;
				break;
			case 'q':
				Check_option_no_param
				Quiet = TRUE;
				break;
			default:
				cout << "ERROR: Invalid option '" << &argv[i][1] << "'!\n";
				exit(1);
			} // switch
		} // From checking condition first key's character to accordance with '-'
		else // If input parameter is not a option
		{
			// If executable file name is already setted
			if (ABS_file_name)
			{
				cout << "ERROR: More than one executable file is specified!\n";
				exit(1);
			}

			// Setting executable file name
			ABS_file_name = argv[i];
		}
	} // From input parameters enumeration cycle

#ifdef NM_RPC
	if (!RPC_server_IP && RPC_server_port_specified)
		cout << "WARNING: Option 'server_port' has no effect without option 'server_ip'!\n";
#endif

	if (f_Open_by_bn && f_Open_by_sn)
	{
		cout << "ERROR: Options '-b' and '-n' are incompatible!\n";
		exit(1);
	}

	// If executable file name still not setted
	if (!ABS_file_name)
	{
		cout << "ERROR: No input executable file!\n";
		exit(1);
	}

	if (f_Run_program_special &&
		(f_Dump_start || f_Dump_finish || (!f_Run_program) || f_Return_NM_ret_val || recv_file_name || send_file_name || f_Sync))
	{
		cout << "ERROR: '-g' option is incompatible with '-s', '-e', '-r', '-v' and extended options!\n";
		exit(1);
	}

	if (f_Return_NM_ret_val && (!f_Run_program))
	{
		cout << "ERROR: Return NM return value is not possible without normal run the program!\n";
		exit(1);
	}

	// If x64 dump and begin address and size not multiple of 2
	if ((f_Dump_start || f_Dump_finish) &&
		(Dump_field_bits == 64) &&
		((Dump_memory_begin % 2) || (Dump_memory_size % 2)))
	{
		cout << "ERROR: With 64-words dump begin address and dump size must be multiple of 2!\n";
		exit(1);
	}

	// Checking acceptability of extended options

	// recv

	if ((recv_flags) && (!recv_file_name))
	{
		cout << "ERROR: No receive file name specified!\n";
		exit(1);
	}

	if ((recv_flags & 0x1) && (recv_flags & 0x4))
	{
		cout << "ERROR: Options 'recv_addr' and 'recv_sect' are incompatible!\n";
		exit(1);
	}

	if ((recv_flags & 0x8) && (!(recv_flags & 0x4)))
	{
		cout << "ERROR: With option 'recv_disp' required option 'recv_sect'!\n";
		exit(1);
	}

	if (recv_flags & 0x4) // 0100b
	{
		assert(recv_addr == Dump_memory_begin_default);

		if (Get_sect_addr(recv_sect_name, &recv_addr))
		{
			cout << "ERROR: Failed get address of section '" << recv_sect_name << "'!\n";
			exit(1);
		}

		recv_addr += recv_disp;
	}

	// send

	if ((send_flags) && (!send_file_name))
	{
		cout << "ERROR: No send file name specified!\n";
		exit(1);
	}

	if ((send_flags & 0x1) && (send_flags & 0x4))
	{
		cout << "ERROR: Options 'send_addr' and 'send_sect' are incompatible!\n";
		exit(1);
	}

	if ((send_flags & 0x8) && (!(send_flags & 0x4)))
	{
		cout << "ERROR: With option 'send_disp' required option 'send_sect'!\n";
		exit(1);
	}

	if (send_flags & 0x4) // 0100b
	{
		assert(send_addr == Dump_memory_begin_default);

		if (Get_sect_addr(send_sect_name, &send_addr))
		{
			cout << "ERROR: Failed get address of section '" << send_sect_name << "'!\n";
			exit(1);
		}

		send_addr += send_disp;
	}
}
