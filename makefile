#
# Simple top level make file
#

ECHO    =@echo
MAKE    =@make

default: 
	$(ECHO) "make <target>"
	$(ECHO) "No <Target> Specified.  Should be one of:" 
	$(ECHO) "-------------------------------------------------" 
	$(ECHO) "win32      : Compile for Win32" 
	$(ECHO) "linux      : Compile for Linux" 
 	
clean:
	$(MAKE) -s -f makefile.windows clean
	$(MAKE) -s -f makefile.linux clean

win32:
	$(MAKE) -s -f makefile.windows
	$(ECHO) Done.
linux:
	$(MAKE) -s -f makefile.linux
	$(ECHO) Done.
