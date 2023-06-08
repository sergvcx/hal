
file(GLOB x86_public_headers 
	include/*.h
	include/hal/*.h
	include/*.mlb)
file(GLOB x86_sources 
	${public_headers}
	src/*.*
	src/x86/*.*
	)