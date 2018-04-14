#!lua

-- A solution contains projects, and defines the available configurations
solution "hal-mb7707"
   configurations { "Debug", "Release" }

	-- x86  library with printf support  ---------------------------------	 
	project "hal-mb7707-x86"
		kind "StaticLib"
		includedirs { "../include","$(MB7707)/libload","../src_host_io"}
		targetdir ("../lib")
		files {	"../src_mb7707_host/*.*",
				"../src_x86/*.*",
				"../include/*.h",
				"../src_host_io/*.*"}
	  
		configuration "Debug"
			defines { "DEBUG","NM6405"}
			symbols  "On" 
		
		configuration "Release"
			defines { "NDEBUG","NM6405"}
			symbols  "Off" 
		
	-- x86  library without printf support  ---------------------------------	 
	project "hal-mb7707silent-x86"
		kind "StaticLib"
		includedirs { "../include","$(MB7707)/libload","../src_host"}
		targetdir ("../lib")
		files {		"../src_mb7707_host/*.*",
				"../src_x86/*.*",
				"../include/*.h"}
		
		configuration "Debug"
			defines { "DEBUG","SILENT","NM6405"}
			symbols  "On" 
      
		configuration "Release"
			defines { "NDEBUG","SILENT","NM6405"}
			symbols  "Off" 
		 
	-- NeuroMatrix project ---------------------------------		
	project "hal-mb7707"
		kind "Makefile"
		files {	"../src_1879hb1ya/*.cpp",
				"../src_1879hb1ya/*.asm",
				"../include/*.h", 
				"Makefile" }
	 
		configuration "Debug"
			buildcommands {"make DEBUG=y -f Makefile"}
			rebuildcommands {"make -B DEBUG=y -f Makefile"}
			cleancommands {"make clean"}
		   
		configuration "Release"
			buildcommands {"make -f Makefile"}
			rebuildcommands {"make -B -f Makefile"}
			cleancommands {"make clean"}		   
		
		
		
