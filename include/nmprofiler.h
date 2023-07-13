#ifndef PRIFILER_NMCOUT_INCLUDED
#define PRIFILER_NMCOUT_INCLUDED

//! ���������� ���� ������ ������ ����� ������� � ���� ProfilerData.funcname
//#include "nmprofiler_def.h"
#include "string.h"
#include "time.h"
#include <stdio.h>
//#include "profiler.h"
//#define MAX_FUNCNAME_LENGTH 64
#define MAX_FUNCNAME_LENGTH 16

/*! \page intro �������� 
	
	
������ ���������� �������������� ��� �������������� ��������, ������������� ��� ����������� ����������� ��������� NeuroMatrix. 
��������� � ������� ����� �������� ���������� �� ���-�� ������� ������� � ������� �� ���������� � ������������ ������.
��������� ����� ������� � ��������� ����, � xml-�������, � ����� ��� ������������� ��������������� �������� ������ � ���������� �������������� � ������. 
���������� ������������ ������ � ������� ��5101, ��5103, ��6901, MB7707
*/

/*! \page Principle ������� ������ ���������� 

\par �������������
		��� ������ ���������� ������� ������������ ������ ������� ���������� ��������������.  \n
		���� ������ ����������� ������������� � ������� ����������� �������� � ������� ����������� ������� � ���� ������� <_nmprofiler_list>. 
		
		
\code 
		#include "nmprofiler_def.h"
		import from profiler.mlb;

		begin ".text_profiler"
		nmprofiler_LIB(MAX_FUNCNAME_LENGTH);

		<_nmprofiler_list>
			...
			PROFILE_FUNC(_myfunc,"myfunc");
			PROFILE_FUNC(_sin,"sin");
			PROFILE_FUNC(_cos,"cos");
			PROFILE_FUNC(_bool._.8.8fullBuffer.1.2,"fullBuffer");
			...
		<_nmprofiler_last>
		return;

		end ".text_profiler";
\endcode

\par 
		��� ������� ��������� ������������ ������������� ���������� ����� ������ ������� _nmprofiler_list . 
		������ ������  PROFILE_FUNC() �� ������ ���������� ������� ���� � ������ ��������������� ������� � ��������� ������� �� ����������� ��� � ������� �������.
		������ ������ �������� ��������� ������  ProfilerData � �����  ������� ������������� ���������� � ���������� ������� � ��������� ����������� ������� ������ �������. 
		
\par ����� �����������
  �� ��������� ������ ��������� ���������� �������������� ����������� � ���� �������� ������ �������� ProfilerData , �������  ����� ������� ����� stdout  (���� �������� printf) ���� � ��������� ����, ���� � xml-������� � ������� �������� 
  nmprofiler_PRINT() � nmprofiler_PRINT2XML(). \n
  ����� ������ � ������ ������������� ����� �������� � ���� �������� ������ ���������������� �� ������ ��� ��� ��������. \n
  ������ ����������� �������������� ������� ����� ����� �������� ����� �� �������� ���� � ������� �������� C_Stopwatch. ��� ����������� ������� ���������� ������������ � ������ � ����� ����� ���� ���������� � ������� ��������
  STOPWATCH_PRINT() � STOPWATCH_PRINT2XML(). \n
  
*/


/*! \page manual ����������� �� �������������
	
	
 \par 1. �������� ���������������� ����� 
   \li ������� ���� , �������� � ������ - proflist.asm.\n
   \li ������ � ������� MAX_FUNCNAME_LENGTH (proiler_def.h) ������ ������, ���������� �������� ������� ��� ������ �� �����. ��� ������ ������ ������, ��� ������� ����� ����� ������ ����� �������������� ��� ������. ����������� ������ 8.
   ���� ��� ������� ������ ������� MAX_FUNCNAME_LENGTH , �� ��� ������ ��� ����� ��������. �� ������ ������ ���������� �������� MAX_FUNCNAME_LENGTH �� ������.\n
   \li ���������� ��������������� nmprofiler_06.mlb (��� ����������� NM6405,NM6406) ���� nmprofiler_soc.mlb (��� ������� �� ��������� - ���������� �1879��1� ) \n
   \li �������� ������ ".text_profiler". \n
    \e ����������. C����� .text_profiler ��� ������� �������� ������ ������� ������������� ����������� � ������� ���������� ������. ������� ������, ��� ��� ���� ��� ������ ����� ����� ���� ����������. 
	� ����� ������ ��� �������� ���������������� ������ ������� ����� ����������� � ����������� ������ � ������� ������� nmprofiler_list2table().\n
   \li ����� ������� <_nmprofiler_list> � <_nmprofiler_last> ����������� ����� ������������� ������� ���������� � ������ PROFILE_FUNC() \n
    \e ����������. ��� ��������� ��������� ������� � ������ ����������� � ������ ����� ���: FAdd Mul32 FMul IDiv32 UDiv32 FDiv IMod32 UMod32 LShift32 RShift32 ARShift32 ConvI64toF ConvU64toF
���������� ������������ ������ PROFILE_NONSTD_FUNC()\n
    \e ����������. ����� ������������� ������� ����� ����� �� map-����� , �������������� ������������� ��������� � ������ ������� �������. \n
    \e ����������. �� ��������� ������ ������� map-���� � ����������� �������� �++ ������� �������. ����� ����� ����������  ������� ������� ���������� ������ ���� -full_names \n
	 
	 
	������ ���� profiler.asm:

\code 
const MAX_FUNCNAME_LEN=MAX_FUNCNAME_LENGTH;
import from profiler.mlb;
//import from nmprofiler_soc.mlb; // use nmprofiler_soc.mlb for mc6901 board
begin ".text_profiler"
global _nmprofiler_list:label;
global _nmprofiler_last:label;
<_nmprofiler_list>
	// place here macros with function name for each function you wish to profile like:
	// PROFILE_FUNC(_myfunc,"_myfunc");
	
	PROFILE_FUNC(_mysin,"mysin");
	PROFILE_FUNC(_mycos,"mycos");
	
<_nmprofiler_last>
return;
end ".text_profiler";
\endcode 

\e ����������. ��������������� ���� ����� ������������� ������������� �� ���������� map-�����. 
��������� �������������� �������� \htmlonly <A HREF="http://www.activestate.com/activetcl/downloads">  TCL </A>    \endhtmlonly ������� generate_proflist.tcl �� ����� scripts , ��� � �������� ��������� �����������  ������� ��� map-����� . 
��������:
\verbatim 
tclsh generate_proflist.tcl main.map
\endverbatim
� ���������� ������ ������� ����� ������������ ���� proflist_generated.asm. 
����� ���� ������ ������� ����� ������ ������� �� ����� � ������������� ��� � profile.asm. \n
������� ������, ��� ������ generate_proflist.tcl  �� �������� ����� �� ������ � ������� � � ��������� �������� ����� ������� ����� �� ������.
� ���� ������ ������ ������ ������ ��������� : 
\verbatim 
"DEBUG\DEQUANTIZER.ELF": ERROR LNK415: Symbol "_unsigned._int._const._.8.8Qs.5.7" has different types in different files.
\endverbatim
��������������� ������� ���� ������ ������� 


 
\par 2. ����������� ���������� ����� 
  - ���������� ���������� profiler06.lib , profiler05.lib ��� nmprofiler_soc.lib \n
  - ���������� ���������� nm_io.lib ���� ����� ����������� �������������� �������������� �� stdout  \n
  - �������� ��������������� ���� lib � include ���������� ���������� \n
  - �������� ������ ���������������� ����� proflist.asm \n
  - ������� ���� -fullnames � ���������� ������� 
  
��������:
\verbatim 
nmcc main.cpp -6405 -g          -o.\Release\main.elf     -I..\..\profiler\libprofiler\include -I%NEURO%\include
nmcc proflist.asm -Sc -g -6405  -o.\Release\proflist.elf -I..\..\profiler\libprofiler\include -I%NEURO%\include
linker  -full_names -cmc5101brd.cfg -m -d0 -heap=11000 -o.\example.abs .\Release\main.elf  .\Release\proflist.elf nm_io.lib mc5103lib.lib libc05.lib profiler06.lib cppnew05.lib -l%PROFILER%\lib  -l%NEURO%\lib
\endverbatim


 \par 3. ����������� ����������������� ����� 
 ���������� �������������� ������ ����, ����� ���������� ������ printf  � ����������� ������ .text_profile �� ����������� �� ���������� (������� ������), 
 ��� ����� ������������� ���������� ��������������. \n
 �������� :
 \verbatim
MEMORY
{
	local0:  at 0x00000200, len = 0x00007E00;
	global0: at 0x80000000, len = 0x00008000;
	local1:  at 0x10004000, len = 0x00FFC000;
	global1: at 0xA0000000, len = 0x01000000;
}
SEGMENTS
{
	...
	usercode 	: in local0;
	shared		: in local1;
}

SECTIONS
{
	...
	.rpc_services 		: in shared;		// ���������� ��� printf
	.rpc_services.bss 	: in shared;		// ���������� ��� printf
	.text_profile 		: in usercode;

}
\endverbatim	
	
	
	
 \par 4. ������  ���������� 
 � �������� ����� � �������� main() ���������� ������������ ���� profiler.h � ��������� ������������� ���������� � ������� ����� � ������� ������� nmprofiler_init().


 \code 
#include "profiler.h"

extern "C"{
	int mysin(){
		...
	}
	void mycos(){
		...
	}
};


int main()
{
	...
	nmprofiler_PRINT();
	...
	return 1;
}
 \endcode
	
	
 \par 5. ����� ����������� �� ����� ��� � ����

	����� �� �����  (����� stdout) ����������� �������������� �������������� ��������� :
	nmprofiler_PRINT() - ����� � ��������� ���� \n
	nmprofiler_PRINT2XML() ������������ ����� � ������� xml \n \n
	
	��� ������ ���������� ����� �������� ������� printf ��� ������ �����. \n
	������ ���� ������� �������������� ������������ ���������� nm_io.lib.  \n
	\e ����������. ���������� nm_io.lib ������ ���� ������ � ������ ������������ ���������.
	

	������ ������� � ������ ������� ���������� �� �����: \n
	\verbatim 
	mc5103run example.abs 
	\endverbatim 

	������ ������� � ��������������� ������ � board_stdout.txt : \n
	\verbatim 
	mc5103run example.abs -p 
	\endverbatim 

	
 \par 6. ���������������� ������ � ����������� �������������� 
   ���� printf �� ��������, ������ � ������ �������������� ����� �������� ��������������� �� ������ ��� ��� ��������.  \n\n
   
  \b ������� \b 1. - ������������ ������ ����� ������� ���c��. \n
  ��������������� ������ �������� � ���� �������� ������ �������� ProfilerData.
  
  ������ � ������� �������� ����� �������� � ������� ������ ������� nmprofiler_head():
  \code 
  ProfilerData* profile_item=nmprofiler_head(); 
  \endcode 
  
  ������ � ��������� ��������� �� ������ - �������������� ����� ����  ProfilerData::next . \n
  \e ����������. ������ ������ ������������� � ����������� ������
  
  \b ������� \b 2. - ������ ����� ������� . \n
  �������������� ��������������� ������ ���������� �������� � ���� �������� ������, ������� �� ������ ������  ����������� ��������� � host-������. ����� ���� 
  ������ .text_profiler ����� ����������� � ����������� �����  ���������� ������. � ���� ����� ������������� �������������� ������� ����������� ������ �������� ProfilerData �� ������� ����������� ������ � ������� ������� nmprofiler_list2table(). 
  ������������� ������ �� �������� �������� ��������� �������� ������ , �� ��� ���� ����� ������������� ������� � ���� ����������� �������. ���� ������ ����� ��������� 
  � host-������ �� ��� � ������� �� ����� � ������� ����������� �������:
  
  \e nm-���:
  \code
	...
	int count=nmprofiler_count(nmprofiler_head());  // ��������� ���-�� ��������������� ������� � ������
	ProfilerData* tbl=(ProfilerData*)malloc1(count*sizeof(ProfilerData)); // �������� ������� ��� ������� � ����������� ������
	if (tbl==0) return -1;
	nmprofiler_list2table(nmprofiler_head(),tbl);   // �������� ������ �� �������� � ������� � ����������� ������
	ncl_hostSync((int)tbl);                     // �������� ����� �������
	ncl_hostSync((int)nmprofiler_count(tbl));     // �������� ������ �������
	...
  \endcode
  
  \e pc-���:
  \code 
	...
	int addr, count;
	PL_Sync(access,123,&addr);                      // ������ ����� ������ ��������� ProfilerData � ����������� ������
	PL_Sync(access,123,&count);                     // ������ ������ �������

	ProfilerData* head=new ProfilerData[count]; // �������� ������ ��� �������
	PL_ReadMemBlock(access, (PL_Word*) head, addr, count*sizeof(ProfilerData)/4);	// �������� �������
	
	nmprofiler_table2list(head,count);                // ��������� ����� � �������
	nmprofiler_char8bit(head);                        // ����������� ������ � 8-������ char ������
	nmprofiler_print(head);                           // �������� ������� ��������������� �������
	...
  \endcode
  
  
  
 \par ���������� 1 :  
� ����� scripts ����������� xsl �������������� -profile.xsl , ����������� ���������� ���������� xml-����� � �������� � ��������������� ��������� ���� . 
���� profile.xsl ��� ���� ������ ��������� ����� � ������ profile.xml.

 \par ���������� 2 :
�������� ������� �������� � ������� ���� int ���� ProfilerData::funcname. ��� ����������� ����������� ������ �� host-������ ���������� ������������ ��������� �������������� � ���� char: nmprofiler_char8bit() ��� �������� ������ ������ ProfilerData::funcname2char().

 \par ���������� 3 :
��� ���������� � ������ ����������� -O1 ��� -O2 ��������� ����� ����� ������������ ��� inline ������� � � ���� ������ �� �������������� �� ����������.
	
*/

//#pragma  code_section ".text_profiler"
//#pragma  data_section ".data_profiler"


#ifdef DUMMY_PRINTF
//extern "C"{
//	int printf( const char * _Format, ...){
//		return 1;
//	}	
//};
#endif

#ifdef __cplusplus
	extern "C" {
#endif
	
//!  ��������� �������� ����������� �������������� ��� ����� ������� 
/*!
 �� ������ �������� ��������� ������� ������ �� ������������� ��������. ��������� �� ������ ��������� ��������� ������� profiler_head() 
*/
	void profiler_start();
	void profiler_stop();
	void profiler_reset();
	void profiler_dummy();
	
	void nmprofiler_init();
	
	struct TraceData{
		unsigned 	func;
		unsigned 	depth;
		unsigned 	sp;
		unsigned 	time;
	};
	
	struct ProfilerData {
		unsigned 	next;					///< �������� �� ��������� ���������
		unsigned	funcaddr;				///< ����� �������
		unsigned	calls;					///< ���-�� ������� 
		unsigned	summary;				///< ��������� ����������� ����� (� ������)
		unsigned	funcname[MAX_FUNCNAME_LENGTH];///< ��� ������� ��� ����������� � ���������� (����� �� �������� � �������� ������)
		unsigned	stopwatch;				///< ��������� ����� ������� (�������� ������ t1)
		unsigned long long 	timer_ret_pswr;	///<  (�������� ������ t1)
		
		unsigned	size_summary;			///< ����������� ������������ ������ ������ � ��������
		unsigned	dummy;					///< ��������� ����� ������� (�������� ������ t1)
		
	};
	
	

 //! \defgroup funcs ������� ������ � ����������� 
 //! \{

	//! ������� ������� ��������� �������������� ����� ������� � stdout � ��������������� ����
	//! \param prof ��������� ��������� �������������� 
	//! \param format ������ ������ ����������� printf 
	void nmprofiler_printf(ProfilerData* prof, char* format);

	//! ������� ������������ � ��������� �������������� ����� ������� � dststr
	//! \param prof ��������� ��������� �������������� 
	//! \param format ������ ������ ����������� sprintf 
	void nmprofiler_sprintf(char* dststr, ProfilerData* prof, char* format, char* full_func_name=0);

#ifdef __NM__
//	void nmprofiler_list();
//	void nmprofiler_last();

	extern unsigned nmprofiler_size_current;
	#define PROFILER_SIZE(size) nmprofiler_size_current=size;
	
	//! ������� ���������� ��������� �� ������ ��������� �������������� � ������� ������ 
	#define halProfilerHead nmprofiler_head
	//ProfilerData* halProfilerHead();
	ProfilerData* halProfilerNext();
	ProfilerData* halProfilerCount();
	ProfilerData* nmprofiler_head();
	ProfilerData* nmprofiler_next(ProfilerData* p);


	/*
	{
		int dummy=reinterpret_cast<int> (nmprofiler_list);
		int *p=reinterpret_cast<int*> (dummy);
		return (ProfilerData*)(p+4);
	}
	*/
	//! ������� ���������� ��������� �� ��������� ��������� �������������� � ������� ������ 
	//! \param head ��������� �� ������ ��������� � ������.
	ProfilerData* nmprofiler_tail(ProfilerData* head);

	//! ������� ������� ���������� �������������� � stdout � ���������-������� �� ������ ������� � head. ��� ���������������� ����������� xml-����� ����������� xsl-��������������: profile.xsl.
	//! \param head ��������� �� ������ ��������� � ������.
	void  nmprofiler_print2tbl(ProfilerData* head=0);

	//! ������� ������� ���������� �������������� � stdout � xml-������� �� ������ ������� � head. ��� ���������������� ����������� xml-����� ����������� xsl-��������������: profile.xsl.
	//! \param head ��������� �� ������ ��������� � ������.
	void  nmprofiler_print2xml(ProfilerData* head=0);

	void halProfilerEnable();
	void nmprofiler_enable();
	void nmprofiler_disable();
	int  nmprofiler_count();
	void nmprofiler_copy(ProfilerData* dst);
	void nmprofiler_set_depth(int depth);
#endif	
	
#define NMPROFILER_TBL "%-12u| %-12u| %-12u| %08X| %-20s\n"
//#define NMPROFILER_TBL "%-12u| %-12u| %-12u| %-4f| %08X| %-20s\n"
//#define NMPROFILER_XML "  <prof summary=\"%-12u\"	calls=\"%-12u\"	average=\"%-12u\"	addr=\"%08X\"	name=\"%s\"/>\n"
//#define NMPROFILER_XML "  <prof summary=\"%-12u\"	calls=\"%-12u\"	average=\"%-12u\"	perelement=\"%f\" addr=\"%08X\"	name=\"%s\"/>\n"
#define NMPROFILER_XML "  <prof summary=\"%-12u\"	calls=\"%-12u\"	average=\"%-12u\"	addr=\"%08X\"	name=\"%s\"/>\n"

//! \}

#ifdef __cplusplus
};
#endif


	// ��������� ��� ������� ��������
	//inline void stopwatch_print2tbl()	{ nmprofiler_print2tbl(stopwatch_head()); }
	//inline void stopwatch_print2xml()	{ nmprofiler_print2xml(stopwatch_head()); }
#ifndef DISABLE_PROFILER
	
	//! ������ ������������� ����������
	#define PROFILER_START()		nmprofiler_init();
	
	//! ������ ������������� ����������
	#define PROFILER_RESUME()			nmprofiler_enable();
	
	//! ������ ������������� ����������
	#define PROFILER_STOP()			nmprofiler_disable();
	
	//! ������ ������ �������� �������� ����� stdout � ���� ������� 
	#define PROFILER_PRINT2TBL()	nmprofiler_print2tbl();
	
	//! ������ ������ �������� �������� ����� stdout � xml-������� 
	#define PROFILER_PRINT2XML()	nmprofiler_print2xml();
	
#else 
	#define PROFILER_START()
	#define PROFILER_PRINT2TBL()
	#define STOPWATCH_PRINT2XML()
#endif 

#include "hal/ringbuffert.h"

//INSECTION(".text_nmprofiler") 

#define PROFILER_TRACE(size) extern "C" {	HalRingBufferData<TraceData,size> nmprofiler_trace;}


void  nmprofiler_trace2tbl(int max_depth);


#endif 
