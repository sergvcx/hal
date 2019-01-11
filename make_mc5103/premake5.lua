#!lua

-- A solution contains projects, and defines the available configurations
solution "hal-mc5103"
   	configurations { "Debug", "Release" }

	-- x86  library with printf support  ---------------------------------	 
	project "hal-mc5103-x86"
      		kind "StaticLib"
		includedirs { "../include","$(MC5103)/libload","../src_host_io"}
 		targetdir ("../lib")	  
      		files { 	"../src_mc5103_host/*.*",
				"../src_x86/*.*",
				"../src_ringbuffer/*.*",
				"../include/*.h",
				"../src_host_io/*.*"}
	  
		configuration "Debug"
			targetsuffix ("d")
			defines { "DEBUG","NM6405"}
			symbols  "On" 
		
		configuration "Release"
			
			defines { "NDEBUG","NM6405"}
         	symbols  "Off" 
		
	-- x86  library without printf support  ---------------------------------	 
	project "hal-mc5103silent-x86"
      		kind "StaticLib"
		includedirs { "../include","$(MC5103)/libload","../src_host"}
		targetdir ("../lib")	  
	
	      	files { 	"../src_mc5103_host/*.*",
				"../src_x86/*.*",
				"../src_ringbuffer/*.*",
				"../include/*.h"}
		
		configuration "Debug"
			targetsuffix ("d")
			defines { "DEBUG","SILENT","NM6405"}
			symbols  "On" 
      
		configuration "Release"
			defines { "NDEBUG","SILENT","NM6405"}
			symbols  "Off" 
		 
	-- NeuroMatrix project ---------------------------------		
	project "hal-mc5103"
     		kind "Makefile"
		includedirs { "../include","$(MC5103)/libload"}
      		files { "../src_1879vm5ya/*.*",
		
				"../src_ringbuffer/*.*",
				"../include/*.h", 
				"Makefile" }
	 
		configuration "Debug"
			buildcommands 	{"make DEBUG=y -f 	 Makefile 2>.make & iconv -f cp1251 -t cp866 .make"}
			rebuildcommands {"make -B DEBUG=y -f Makefile 2>.make & iconv -f cp1251 -t cp866 .make"}
			cleancommands 	{"make clean"}
		   
		configuration "Release"
			buildcommands 	{"make -f Makefile		2>.make & iconv -f cp1251 -t cp866 .make"}
			rebuildcommands {"make -B -f Makefile 	2>.make & iconv -f cp1251 -t cp866 .make"}
			cleancommands   {"make clean"}		   
		
		
		
