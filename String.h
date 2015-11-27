// a little string class to handle set & compare functionality
// since VC's CString is not compatible with other platforms (mac, dreamcast) this file is for compatibility issues
// by hellfire.haujobb

// might be buggy ;)


#ifndef STRING_H
#define STRING_H

#include <windows.h>

struct refString
{
    short _size;
    short _length;
    short _refs;
    unsigned short _flags;

    enum rsflags { ICASE=1 };

    refString (short length, short size, unsigned short flgs =0)
        : _length(length), _size(size), _refs(1), _flags(flgs)
        {}
    ~refString (void) {}

    unsigned short flags() const
    {
        return _flags;
    }

    void setf(unsigned short f)
    {
        _flags = (unsigned short)(_flags | f);
    }

    void resetf(unsigned short f)
    {
        _flags = (unsigned short)(_flags & ~f);
    }

    char * ptr (void)
    {
        return ((char *)this) + sizeof(refString);
    }

    void* operator new(unsigned sz, short allocsz);
//    void operator delete();

};




class String
{
  protected:

    static unsigned short default_flags;
    refString * Stringdata;

    // Check to see if big enough for size
    int _chksize (short sz =0);
    int _compare (String const s) const;
    int _concat (char const * s, short len =-1);
	int _concat (String const & s);

    void _Stringinit (char const * s =0, short slen =0, short siz =-1, unsigned short flgs = 0);


  public:
        // conStringuctors/deStringuctors
    String (void);
    String (char const * s, short len =-1);
    String (String const & s);
    virtual ~String (void);

        // assignment

    String & operator= (String const & s);
    String & operator= (char const * s);
    String & operator= (char c);

    String & operator= (unsigned char const * s)
    {
        return operator= ((char const *)s);
    }
    String & operator= (signed char const * s)
    {
        return operator= ((char const *)s);
    }

        // primitive members

    short length (void) const
    {
        return Stringdata->_length;
    }

    short size (void) const
    {
        return Stringdata->_size;
    }

        // operators

    String & operator+ (char const * s);              // concatenate
    String & operator+ (unsigned char const * s);
    String & operator+ (signed char const * s);
    String & operator+ (String const & s);
    String & operator+ (int val);
    String & operator+ (unsigned int val);
    String & operator+ (short val);
    String & operator+ (unsigned short val);
    String & operator+ (long val);
    String & operator+ (unsigned long val);
    String & operator+ (char c);
    String & operator+ (unsigned char c);
    String & operator+ (signed char c);

    char const & operator[] (short pos) const;
    char & operator[] (short pos);

    char * c_ptr (void) const            // not necessarily NUL terminated!
    {                                                // Use with caution...
        return Stringdata->ptr();
    }
    char const * c_String (void) const    // return char*
    {
        char * buf = c_ptr();
        buf[Stringdata->_length] = 0;
        return buf;
    }
    unsigned char const * u_String (void) const
    {
        return (unsigned char const *)c_String();
    }

    signed char const * s_String (void) const
    {
        return (signed char const *)c_String();
    }

    int copy(char * dest, short maxlen =-1) const;

        // manipulators

    short insert (short pos, char const * s, short len =-1);
    short insert (short pos, String const & s)
    {
        return insert (pos, s.c_ptr(), s.length());
    }
    short insert (short pos, unsigned char const * s, short len =-1)
    {
        return insert (pos, (char const *)s, len);
    }

    short insert (short pos, signed char const * s,
                    short len =-1)
    {
        return insert (pos, (char const *)s, len);
    }

    short insert (short pos, char c)
    {
        return insert (pos, &c, 1);
    }
    short insert (short pos, unsigned char c)
    {
        return insert (pos, (char const *)&c, 1);
    }

    short insert (short pos, signed char c)
    {
        return insert (pos, (char const *)&c, 1);
    }

    short remove (short pos =0, short len =-1);
    short replace (short pos, char const * s, short clen =-1, short len =-1);
    short replace (short pos, String & s, short clen =-1)
    {
        return replace (pos, s.c_ptr(), clen, s.length());
    }
    short replace (short pos, unsigned char const * s, short clen =-1, short len =-1)
    {
        return replace (pos, (char const *)s, clen, len);
    }

    short replace (short pos, signed char const * s, short clen =-1, short len =-1)
    {
        return replace (pos, (char const *)s, clen, len);
    }

    short replace (short pos, char c, short clen =-1)
    {
        return replace (pos, &c, clen, 1);
    }

    short replace (short pos, unsigned char c, short clen =-1)
    {
        return replace (pos, (char const *)&c, clen, 1);
    }

    short replace (short pos, signed char c, short clen =-1)
    {
        return replace (pos, (char const *)&c, clen, 1);
    }

    String & left (short len, char padch =' ');
    String & right (short len, char padch =' ');
    String & mid (short pos, short len, char padch =' ');

    String subString(short start, short len =-1) const;

    short removech (char const * clist ="\r\n");
    short countch (char const * clist);

    int compare (String const & s) const
    {
        return _compare(s);
    }

    int compare (char const * s) const
    {
        return _compare(String(s));
    }

    short Find(char* s);
    
    int ReverseFind(char f);

    String Right(int index);

    void setflags (unsigned short flags);
    void resetflags (unsigned short flags);
    char* getData();
    void SetAt(int index, char c);

    void setcase (bool s =TRUE)
    {
        if (s)
            setflags(refString::ICASE);
        else
            resetflags(refString::ICASE);
    }

    static void setdefaultcase (bool s = TRUE)
    {
        default_flags = (s == FALSE)
                        ? (unsigned short)(default_flags | refString::ICASE)
                        : (unsigned short)(default_flags & ~refString::ICASE);
    }
    bool operator== (String const & s) const
    {
        return (_compare(s) == 0) ? TRUE : FALSE;
    }
    bool operator== (char const * s) const
    {
        return (_compare(s) == 0) ? TRUE : FALSE;
    }

    bool operator!= (String const & s) const
    {
        return (_compare(s) != 0) ? TRUE : FALSE;
    }

    bool operator!= (char const * s) const
    {
        return (_compare(s) != 0) ? TRUE : FALSE;
    }

    String left (String s, short len);
    String right (String s, short len);
    String mid (String s, short pos, short len);
    int compare(String s, String b);
    int compare(String s, char const * b);
    int comparePartial(String const s) const;


};


#endif // String_H
