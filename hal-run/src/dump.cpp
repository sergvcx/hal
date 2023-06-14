// Loader for MC121.01
// Output memory to dump file

#include <iostream>
#include <iomanip>
#include <fstream>
#include "hal/hal.h"

#include "mc12101load.h"

#include "nmrun.h"

using namespace std;

extern HalAccess *Access;

extern BOOL Quiet;

extern char *ABS_file_name;

extern PL_Addr Dump_memory_begin;
extern PL_Addr Dump_memory_size;
extern unsigned int Dump_field_bits;

extern char *recv_file_name;
extern PL_Addr recv_addr;
extern unsigned long recv_size;

extern char *send_file_name;
extern PL_Addr send_addr;
extern unsigned long send_size;

int Dump_shared_memory(const char *ext)
{
	if (!Quiet)
		cout << "Start dump of memory...\n" << flush;

	PL_Word *Board_memory;
	try
	{
		Board_memory = new PL_Word[Dump_memory_size];
	}
	catch (...)
	{
		cout << "ERROR: Insufficient memory!\n";
		return 1;
	}

	if (halReadMemBlock(Access, Board_memory, Dump_memory_begin, Dump_memory_size) != PL_OK)
	{
		cout << "ERROR: Failed read memory range [0x";
		cout << hex << uppercase << setw(8) << setfill('0') << Dump_memory_begin << " - 0x";
		cout << hex << uppercase << setw(8) << setfill('0') << (Dump_memory_begin + Dump_memory_size - 1) << "]!\n";
		delete[] Board_memory;
		return 1;
	}

	size_t Dump_file_name_length = strlen(ABS_file_name) + strlen(ext) + 1;

	char *Dump_file_name;
	try
	{
		Dump_file_name = new char[Dump_file_name_length];
	}
	catch (...)
	{
		cout << "ERROR: Insufficient memory!\n";
		delete[] Board_memory;
		return 1;
	}

	strcpy_s(Dump_file_name, Dump_file_name_length, ABS_file_name);
	char *ptr = strrchr(Dump_file_name, '.');
	if (ptr)
		*ptr = '\0';
	strcat_s(Dump_file_name, Dump_file_name_length, ext);

	ofstream Dump_file(Dump_file_name);
	if (!Dump_file)
	{
		cout << "ERROR: Failed open '" << Dump_file_name << "' for output!\n";
		delete[] Dump_file_name;
		delete[] Board_memory;
		return 1;
	}

	Dump_file << "================================= Shared memory =================================\n";

	Dump_file << hex << uppercase;

	for (PL_Addr i = 0; i < Dump_memory_size;)
	{
		if (!(i % 8))
		{
			// Output address
			Dump_file << "\n";
			Dump_file << setw(8) << setfill('0');
			Dump_file << (Dump_memory_begin + i) << ":";
		}

		// Directly output memory
		if (Dump_field_bits == 64)
		{
			// x64 output
			unsigned long long *w64 = (unsigned long long *)(Board_memory + i);
			Dump_file << "  " << setw(16) << setfill('0') << *w64;
			i += 2;
		}
		else if (Dump_field_bits == 32)
		{
			// x32 output
			Dump_file << " " << setw(8) << setfill('0') << Board_memory[i++];
		}
		else
		{
			// Other output (1 - 16)

			PL_Word val = Board_memory[i++];
			PL_Word mask = (1 << Dump_field_bits) - 1;

			for (unsigned int j = 0; j < 32 / Dump_field_bits; j++)
			{
				Dump_file << " " << setw((Dump_field_bits + 3) / 4) << setfill('0') << (val & mask);
				val >>= Dump_field_bits;
			}
		}
	}

	Dump_file << endl << flush;

	Dump_file.close();
	delete[] Dump_file_name;
	delete[] Board_memory;

	return 0;
}

int Receive_binary_dump_from_board()
{
	if (!Quiet)
		cout << "Start binary dump of memory...\n" << flush;

	PL_Word *Board_memory;
	try
	{
		Board_memory = new PL_Word[recv_size];
	}
	catch (...)
	{
		cout << "ERROR: Insufficient memory!\n";
		return 1;
	}

	if (halReadMemBlock(Access, Board_memory, recv_addr, recv_size) != PL_OK)
	{
		cout << "ERROR: Failed read range of memory [0x";
		cout << hex << uppercase << setw(8) << setfill('0') << recv_addr << " - 0x";
		cout << hex << uppercase << setw(8) << setfill('0') << (recv_addr + recv_size - 1) << "]!\n";
		delete[] Board_memory;
		return 1;
	}

	ofstream Dump_file(recv_file_name, ios::binary | ios::out);
	if (!Dump_file)
	{
		cout << "ERROR: Failed open '" << recv_file_name << "' for writing!\n";
		delete[] Board_memory;
		return 1;
	}

	for (unsigned long i = 0; i < recv_size; i++)
		Dump_file.write((char *)(Board_memory + i), sizeof(PL_Word));

	Dump_file.close();
	delete[] Board_memory;

	return 0;
}

int Send_binary_file_to_board()
{
	if (!Quiet)
		cout << "Start loading binary file to memory...\n" << flush;

	ifstream Dump_file(send_file_name, ios::binary | ios::in);
	if (!Dump_file)
	{
		cout << "ERROR: Failed open '" << send_file_name << "' for reading!\n";
		return 1;
	}

	// Getting file size
	Dump_file.seekg(0, ios::end);
	unsigned long Dump_file_size = (unsigned long)Dump_file.tellg();
	Dump_file.seekg(0, ios::beg);

	unsigned long Dump_size = send_size;

	if (!Dump_size)
		Dump_size = (Dump_file_size + 3) >> 2;

	if (Dump_size > ((Dump_file_size + 3) >> 2))
	{
		cout << "ERROR: Specified file size is greater than real file size!\n";
		Dump_file.close();
		return 1;
	}

	PL_Word *Board_memory;
	try
	{
		Board_memory = new PL_Word[Dump_size];
	}
	catch (...)
	{
		cout << "ERROR: Insufficient memory!\n";
		Dump_file.close();
		return 1;
	}

	if ((Dump_size << 2) > Dump_file_size)
	{
		cout << "WARNING: File size is not multiple of 4 bytes. Added by zeroes.\n";
		Board_memory[Dump_size - 1] = 0x0;
	}

	for (unsigned long i = 0; (i < (Dump_size << 2)) && (i < Dump_file_size); i++)
		Dump_file.read(((char *)Board_memory) + i, 1);

	Dump_file.close();

	if (halWriteMemBlock(Access, Board_memory, send_addr, Dump_size) != PL_OK)
	{
		cout << "ERROR: Failed write range of memory [0x";
		cout << hex << uppercase << setw(8) << setfill('0') << send_addr << " - 0x";
		cout << hex << uppercase << setw(8) << setfill('0') << (send_addr + Dump_size - 1) << "]!\n";
		delete[] Board_memory;
		return 1;
	}

	delete[] Board_memory;

	return 0;
}
