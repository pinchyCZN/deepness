// a little stream class to handle file-io functions from disc or from rar-file
// by hellfire.haujobb
// additional features&changes by oidberg

#ifndef HJBSTREAM_H
#define HJBSTREAM_H

#include <stdio.h>
#include "string.h"

#define STREAM_NODEF 0
#define STREAM_FILE  1
#define STREAM_RAR   2
class HjbStream
{
  public:
    HjbStream();
    HjbStream(String filename, char *mode);
    ~HjbStream();

    void            open(String filename, char *mode);  // open a file (same as fopen)
    void            close();                            // close a file (same as fclose)
    int             getError();                         // check whether an error occured while opening or reading
    bool            eof();                              // end of file?
    int             getSize();                          // size of file
    int             getPos();                           // position in file
    void            setPos(int pos);                    // jump to position in file

// read stuff
    char            readChar();                         
    unsigned char   readByte();
    unsigned short  readWord();
    int             readInt();
    float           readFloat();
    String          readString();
    String          readLine();
    int             read(void *buffer, int size);       // general read (like fread)

// write stuff
    void            writeChar(char c);                  
    void            writeByte(unsigned char c);
    void            writeWord(unsigned short w);
    void            writeInt(int i);
    void            writeFloat(float f);
    void            writeString(String s);
	void			write(void *buffer, int size);
  
// other stuff
  private:
    void            mread(void *buf, int num, int esize, unsigned char *src); // some faked fread for memory-access

  private:
    unsigned int    m_mode;                 // do we read from file, rar, ..., ?
    FILE            *m_f;                   // pointer to file (if any)
    unsigned char   *m_buf;                 // mem pointer to unpacked file from rar
    unsigned int    m_pos;                  // position in mem (faked filepointer)
    unsigned int    m_size;                 // size of memblock
    int             m_error;                // error flag
};


#endif // stream_h
