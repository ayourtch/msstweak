all:
	@echo Use make build to compile the library, make help for help.

build: msstweak.c
	gcc -o msstweak.so -ldl -shared -fPIC msstweak.c

clean:
	rm -f msstweak.so

help:
	@echo Examples:
	@echo '  Linux: TCPMSS=1000 LD_PRELOAD=./msstweak.so curl http://google.com/'
	@echo '  MacOS X (does not work?):'
	@echo '    TCPMSS=1000 DYLD_INSERT_LIBRARIES=./msstweak.so DYLD_FORCE_FLAT_NAMESPACE=1 curl http://google.com/'
	
