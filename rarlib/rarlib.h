#ifndef RARLIB_H
#define RARLIB_H

#define _DO_CRC32_CHECK                     // perform cyclical redundancy check (CRC32) - disable this for a little speed-up

#define _WIN_32                             /* Win32 with VisualC           */
#define _DEBUG_LOG_FILE "C:\\temp\\debug_unrar.txt" /* log file path        */

/*
#define _UNIX      
#define _DEBUG_LOG_FILE "/tmp/debug_unrar.txt" 
#define NON_INTEL_BYTE_ORDER
*/      

extern int rarlib_get(void  *output, unsigned long *size, char *filename, char *libfilename, char *libpassword);


#endif

