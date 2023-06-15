// Settings for libelf and liblib libraries

#ifndef __ELF_SET
#define __ELF_SET

    // ����� ����������� ������� ������ � ����� �� ������ ������.
    // intel - little-endian.
    // _X86 and/or __i386__ - ��������������� � gcc.
    // _M_IX86 - ��������������� � MSVC.
#if defined(_X86) || defined(__i386__) || defined(_M_IX86) || defined(_M_AMD64) || defined(__x86_64) || \
        ( defined(__BYTE_ORDER__) && ( __BYTE_ORDER__==__ORDER_LITTLE_ENDIAN__ ) )
    // !!! native Intel coding !!!
    // little-endian
static const unsigned char _HOST_ENCODING = ELFDATA2LSB;

#else  // defined(_X86) || defined(__i386__) || defined(_M_IX86)

    // E90 - big endian
    // _E90 - defined in makefile
#if defined(_E90) || ( defined(__BYTE_ORDER__) && ( __BYTE_ORDER__==__ORDER_BIG_ENDIAN__ ) )
static const unsigned char _HOST_ENCODING = ELFDATA2MSB;

#else
#error "Needs change data encoding according with native host machine data encoding"
#endif  // defined(_E90)
#endif  // defined(_X86) || defined(__i386__) || defined(_M_IX86)

    // target machine data encoding
#if defined (NM6403) || defined (NM6404) || defined (DSM) || defined (NM6405)
static const unsigned char _TARG_ENCODING = ELFDATA2LSB;

#else
#error "Needs change data encoding according with target machine data encoding"
#endif  // defined (NM6403) || defined (NM6404) || defined (DSM) || defined (NM6405)


#endif  // __ELF_SET
