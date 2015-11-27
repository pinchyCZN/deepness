#include "String.h"

#define STDLEN 32

void* refString::operator new (unsigned sz, short allocsz)
{
    return malloc (sz + allocsz);
}

// void refString::operator delete () { }

// constructors -------------------------------------------------------------------------------------------------------

String::String(void)
{
    _Stringinit();
}

String::String(char const * s, short len)
{
    _Stringinit(s, len, -1);
}

String::String (String const & s) : Stringdata(s.Stringdata)
{
    ++Stringdata->_refs;
}

String::~String (void)
{
    if (!--Stringdata->_refs)
        delete Stringdata;
}


// internal functions --------------------------------------------------------------------------------------------------

bool strcompare(const char *t1, const char *t2)
{
  int dif=0;
  while (*t1 && *t2)
  {
    if (*t1!=*t2) dif++;
    t1++;
    t2++;
  }
  if (!dif) return TRUE; else return FALSE;
}


int String::_compare(String const s) const
{
    return strcmp(c_String(), s.c_String());
}


int String::comparePartial(String const s) const
{
    return strcompare(c_String(), s.c_String());
}


int String::_chksize (short sz)
{
    refString * old = 0;
    if (Stringdata->_refs > 1) // Need to dup memory
        --Stringdata->_refs;   // Dec existing Stringing reference
    else if (sz >= size())
        old = Stringdata;
    else
        return 0;
    _Stringinit (c_ptr(), length(), sz);
    delete old;
    return 1;
}

int String::_concat (char const * s, short len)
{
    if (len < 0) len = (short) strlen(s);
    if (len > 0)
    {
        // Special case - are we concatenating ourselves??
        if (Stringdata->_refs == 1 &&  // No danger if we'll be reallocated anyway
            s >= c_ptr() &&         // Refers to us, or subStringing of us
            s <= (c_ptr() + length()))
        {   // This is handled separately, since we do not wish
            // to pass this heinous overhead onto all cases,
            // especially when this particular case is so rare...
            String tmpString(s, len);                 // Copy this Stringing first
            _chksize(short(len + length() + 1));
            memcpy(c_ptr() + length(), tmpString.c_ptr(), len);
        }
        else
        {
            _chksize (short(len + length() + 1));
            memcpy (c_ptr() + length(), s, len);
        }
        Stringdata->_length += len;
    }
    return length();
}

int String::_concat (String const & s)
{
  return _concat (s.c_ptr(), s.length());
}

void String::_Stringinit (char const * s, short len, short siz, unsigned short flgs)
{
    if (len < 0)
        len = (short) ((s) ? strlen (s) : 0);
    if (siz < 0)
        siz = STDLEN;
    if (siz < short(len + 1))
        siz = short(len + 1);
    Stringdata = new(siz) refString(len, siz, flgs);
    if (s && len)
        memcpy (c_ptr(), s, len);
}

// public functions ----------------------------------------------------------------------------------------------------------------

void String::SetAt(int index, char c)
{
  char *s;
  s= c_ptr();
  s[index]= c;
}

char* String::getData()
{
  int l= length();
  char *str=c_ptr();
  str[l]= 0;
  return str;
}

short String::Find(char *s2)
{
    char *s1,*p;
    s1= getData();
    p = strstr(s1, s2);
    if (!p) return -1;
    return p - s1;
}

int String::ReverseFind(char f)
{
  int l= length();
  char *p= getData();//c_ptr();

  while (l>0 && p[l]!=f) l--;

  return l;
}

int String::copy(char * dst, short maxlen) const
{
    if (maxlen == -1)
        maxlen = short(length() + 1);
    short len = short(maxlen - 1);
    if (len > length())
        len = length();
    if (len > 0)
        memcpy(dst, c_ptr(), len);
    if (len >= 0)
        dst[len] = '\0';
    return len;
}


String String::Right(int index)
{
  return subString(index);
}


String String::subString(short start, short len) const
{
    if (start < 0)
        start = short(length() + start);
    if (start < 0 || start >= Stringdata->_length)
        return String();   // Empty
    if (len < 0 || (short(start + len) > Stringdata->_length))
        len = short(Stringdata->_length - start);
    return String(c_ptr() + start, len);
}


// operators ------------------------------------------------------------------------------------------------------------

String& String::operator= (String const & s)
{
    if (&s != this)
    {
        if (!--Stringdata->_refs)
            delete Stringdata;
        Stringdata = s.Stringdata;
        ++Stringdata->_refs;
    }
    return *this;
}

String& String::operator= (char const * s)
{
    if (s != c_ptr())
    {
        short len = (short) strlen (s);
        _chksize (short(len + 1));
        memcpy (c_ptr(), s, len);
        Stringdata->_length = len;
    }
    return *this;
}

String& String::operator= (char c)
{
    _chksize (2);
    *c_ptr() = c;
    Stringdata->_length = 1;
    return *this;
}


String& String::operator+ (char const * s)    // concatenate
{
    _concat (s);
    return *this;
}

String& String::operator+ (String const & s)
{
    _concat (s);
    return *this;
}

String& String::operator+ (int val)    // concatenate
{
	char s[16];
	itoa(val,s,10);
    _concat (s);
    return *this;
}

char const &String::operator[] (short pos) const
{
    if (pos < 0)            // Negative index addresses from eos
        pos = short(Stringdata->_length + pos);
    if (pos >= Stringdata->_length)
    {
        char * buf = c_ptr() + length();
        *buf = 0;
        return *buf;
    }
    return c_ptr()[pos];
}

char &String::operator[] (short pos)
{
    if (pos < 0)                       // Negative index addresses from eos
        pos = short(Stringdata->_length + pos);
    if (pos < 0)                     // Any cleaner way without exceptions?
        pos = Stringdata->_length;
    if (pos < Stringdata->_length)
        _chksize(0);
    else
    {
        _chksize(short(pos + 2));
        ::memset(c_ptr() + length(), ' ', pos - Stringdata->_length + 1);
        Stringdata->_length = short(pos+1);
    }
    return c_ptr()[pos];
}


String String::left (String s, short len)
{
    return s;//.left(len, ' ');
}

String String::right (String s, short len)
{
    return s;//.right(len, ' ');
}

String String::mid (String s, short pos, short len)
{
    return s;//.mid(pos, len, ' ');
}

int String::compare(String s, String b)
{
    return s.compare(b);
}

int String::compare(String s, char const * b)
{
    return s.compare(b);
}
