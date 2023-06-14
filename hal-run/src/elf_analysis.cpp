// Loader for MC121.01
// Analyzing of ELF-files

#include <windows.h>

#include "libelf.h"

#include "nmrun.h"

using namespace std;

extern char *ABS_file_name;

int Get_sect_addr(const char *Sect_name, PL_Addr *Sect_addr)
{
	ELF *ElfFile;

	try
	{
		ElfFile = new ELF(ABS_file_name, ELF_C_READ);
	}
	catch (...)
	{
		return 1;
	}

	// If is not ELF-file
	if (ElfFile->Kind() != ELF_K_ELF)
	{
		delete ElfFile;
		return 1;
	}

	Elf32_Ehdr *Ehdr = ElfFile->GetEhdr();

	// If is not ABS-file
	if (Ehdr->e_type != ET_EXEC)
	{
		delete ElfFile;
		return 1;
	}

	// If has not program sections
	if (!Ehdr->e_shnum)
	{
		delete ElfFile;
		return 1;
	}

	for (DWORD i = 0; i < Ehdr->e_shnum; i++) // Cycle by sections
	{
		// Section
		ELF_SCN *Scn = ElfFile->GetScn(i);

		// Header of section
		Elf32_Shdr *Shdr = Scn->GetShdr();

		char *sn = Scn->GetName();

		if (!(strcmp(sn, Sect_name)))
		{
			// Section found

			*Sect_addr = Shdr->sh_addr;

			delete ElfFile;

			return 0;
		}
	}

	// Section not found

	delete ElfFile;

	return 1;
}
