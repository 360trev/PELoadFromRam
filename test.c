/* Example of loading a DLL from alternative source (with in ram relocation!)
 
   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   "Software"), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
   OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include "modl_ldr.h"
#include "load_file.h"

// prototype of the method exported from our dll
typedef int (*addNumberProc)(int, int);

int main(int argc, char* argv[])
{
	unsigned char *data;
	size_t filelen;
	HMEMORYMODULE testBase;			// dll base handle
	addNumberProc addNumber;		// function ptr

	if(argc < 3){
		printf("Usage: %s <dll_name> <dll_func_name>\n",argv[0]);
		return -1;
	}
	
	/* load the dll file into memory 
	 *
	 * Note:(This step could have been from an encrypted, compressed or signed file or 
	 * even dynamically downloaded from the web (!), etc.).
	 */	
	data = load_file(argv[1], &filelen);
	if(data == 0) { printf("failed to load %d, exiting.\n",argv[1]); return 0; }
		
	printf("Opening lib ..\n");
	/* load shared library from buffer, relocate it, etc. */
	if((testBase = MemoryLoadLibrary(data)) == NULL) {
			printf("Can't load library.\n");
	  	free(data); 
	  	return -1;
	}

	printf("Finding api call ..\n");
	/* now we can lookup any named vtable function(s) we want.. */
	addNumber = (addNumberProc)MemoryGetProcAddress(testBase, argv[2]);	
	if(addNumber != 0) {	
		printf("Got it! %d = %s(1,2);\n", addNumber(1, 2),argv[2]);		/* example use of dll exported function */
	} else {
		printf("Sorry %s() api not found in this dll.\n",argv[2]);
	}

	printf("Closing lib ..\n");
	/* close the library */ 
	MemoryFreeLibrary(testBase);
	
	/* free loaded dll buffer memory */
	free(data);

	return 0;
}

