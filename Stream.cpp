#include <math.h>
#include <stdlib.h>
#include "rarlib/rarlib.h"
#include "string.h"
#include "stream.h"


#define ARCHIVE_NAME     "blow.job"
#define ARCHIVE_PASSWORD "password"

void HjbStream::mread(void *buf, int num, int esize, unsigned char *src)
{
  int i;
  int size= esize * num;
  unsigned char *dst;

  dst= (unsigned char*)buf;
  src+= m_pos;
  for (i=0;i<size;i++) *dst++=*src++;
  m_pos+=size;
}

HjbStream::HjbStream()
{
  m_error=0;
  m_f =   NULL;
  m_mode= STREAM_NODEF;
  m_buf=  NULL;
  m_pos=  0;
}


HjbStream::~HjbStream()
{
  if (m_f) fclose(m_f);
  if (m_buf) free(m_buf);
  m_pos= 0;
  m_mode= STREAM_NODEF;
}


HjbStream::HjbStream(String filename, char *mode)
{
  unsigned long size= 0;

  m_error=0;
  m_f =   NULL;
  m_mode= STREAM_NODEF;
  m_buf=  NULL;
  m_pos=  0;

  open(filename, mode);
}


void HjbStream::open(String filename, char *mode)
{
  unsigned long size= 0;

  m_error=0;
  
  // try opening file from disk
  m_f = fopen(filename.getData(), mode);
  if (!m_f)
  {
    // if file loading failed, try reading file from rar-archive

    String fname;
    int idx = filename.ReverseFind('\\');
    if (idx>=0) fname= filename.Right(idx+1);
           else fname= filename;
    if (!rarlib_get(&m_buf, &size, fname.getData(), ARCHIVE_NAME, ARCHIVE_PASSWORD))
    {
      m_error= 1;
      m_mode=  STREAM_NODEF;
      return;
    }
    m_pos = 0;
    m_size= size;
    m_mode= STREAM_RAR;
    return;
  }
  m_mode= STREAM_FILE;
}


void HjbStream::close()
{
  if (m_f) fclose(m_f);
  if (m_buf) free(m_buf);
}

bool HjbStream::eof()
{
  if (m_f) return (bool) feof(m_f);
  if (m_buf)
  {
    if (m_pos < m_size) return FALSE; else return TRUE;
  }
  return FALSE;
}

int HjbStream::getError()
{
  return m_error;
}

int HjbStream::getSize()
{
  if (m_mode == STREAM_FILE)
  {
    int actpos= ftell(m_f);
    fseek(m_f, 0, SEEK_END);
    int pos= ftell(m_f);
    fseek(m_f, actpos, SEEK_SET);
    return pos;
  }

  if (m_mode == STREAM_RAR)  return m_size;

  return 0;
}

int HjbStream::getPos()
{
  if (m_mode == STREAM_FILE) return ftell(m_f);
  if (m_mode == STREAM_RAR)  return m_pos;
  return 0;
}

void HjbStream::setPos(int pos)
{
  if (m_mode == STREAM_FILE) fseek(m_f, pos, SEEK_SET);
  if (m_mode == STREAM_RAR)  m_pos= pos;
}

char HjbStream:: readChar()
{
  char c;
  if (m_mode == STREAM_FILE) fread(&c, 1,sizeof(char), m_f);
  if (m_mode == STREAM_RAR)  mread(&c, 1,sizeof(char), m_buf);
  return c;
}

unsigned char HjbStream:: readByte()
{
  unsigned char c;
  if (m_mode == STREAM_FILE) fread(&c, 1,sizeof(char), m_f);
  if (m_mode == STREAM_RAR)  mread(&c, 1,sizeof(char), m_buf);
  return c;
}

unsigned short HjbStream:: readWord()
{
  unsigned short i;  
  if (m_mode == STREAM_FILE) fread(&i, 1,sizeof(short), m_f);
  if (m_mode == STREAM_RAR)  mread(&i, 1,sizeof(short), m_buf);
  return i;
}

int HjbStream:: readInt()
{
  int i;
  if (m_mode == STREAM_FILE) fread(&i, 1,sizeof(int), m_f);
  if (m_mode == STREAM_RAR)  mread(&i, 1,sizeof(int), m_buf);
  return i;
}

float HjbStream:: readFloat()
{
  float fl;
  if (m_mode == STREAM_FILE) fread(&fl, 1,sizeof(float), m_f);
  if (m_mode == STREAM_RAR)  mread(&fl, 1,sizeof(float), m_buf);
  return fl;
}

int HjbStream::read(void *buffer, int size)
{
  int n=0;
  unsigned char *dst= (unsigned char*)buffer;
  
  while (n<size && !eof())
  {
    dst[n]= readByte();
    n++;
  }

  return n;
}

String HjbStream::readString()
{
  int    i=0;
  char   c=1;
  String s="                                                                        ";
  
  while (c)
  {
    c= readChar();
    s.SetAt(i++,c);
  }

  return s;
}

String HjbStream::readLine()
{
  int    i=0;
  char   c=1;
  char   buffer[2048];
  strcpy(buffer,"");
//  String s="                                                                                                                                                                          ";
  
  while (c!=10&&!eof())
  {
    c= readChar();
    if (c!=13&&c!=10) 
	{
		buffer[i]=c;
		i++;
		buffer[i]=NULL;
	}// s._concat(&c);//s.SetAt(i++,c);
  }

  String s=buffer;

  return s;
}


void HjbStream:: writeChar(char c)
{
  if (m_mode == STREAM_FILE) fwrite(&c, 1,sizeof(char), m_f);
}

void HjbStream:: writeByte(unsigned char c)
{
  if (m_mode == STREAM_FILE) fwrite(&c, 1,sizeof(char), m_f);
}

void HjbStream:: writeWord(unsigned short w)
{
  if (m_mode == STREAM_FILE) fwrite(&w, 1,sizeof(short), m_f);
}

void HjbStream:: writeInt(int i)
{
  if (m_mode == STREAM_FILE) fwrite(&i, 1,sizeof(int), m_f);
}

void HjbStream:: writeFloat(float fl)
{
  if (m_mode == STREAM_FILE) fwrite(&fl, 1,sizeof(float), m_f);
}

void HjbStream:: writeString(String s)
{
  int    i=0;
  char   c=1;
  char   *buf= s.getData();

  while (c)
  {
    c= *buf++;
    writeChar(c);
  }
}
   
void HjbStream:: write(void *buffer, int size)
{
  int    i=0;
  char   c=1;
  char   *buf= (char *)buffer;

  for(i=0;i<size;i++)
  {
    c= *buf++;
    writeChar(c);
  }
}
   
