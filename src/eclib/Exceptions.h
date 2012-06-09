/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef Exceptions_H
#define Exceptions_H

#include "eclib/machine.h"

#include "eclib/CodeLocation.h"
#include "eclib/Log.h"
#include "eclib/StrStream.h"

void Panic(const char*);
void Panic(const char *msg,int line,const char *file,const char *proc);

// General purpose exception
// Other exceptions must be defined in the class that throw them

// Misc. errors

class Exception : public exception {


public:
    virtual const char *what() const throw() {  return what_.c_str(); }
    Exception(const string&);
    ~Exception() throw();
    virtual bool retryOnServer() const        { return false; }
    virtual bool retryOnClient() const        { return false; }
    virtual bool terminateApplication() const { return false; }

    static bool throwing();
    static void exceptionStack(ostream&);

protected:
    void reason(const string&);
    Exception();
private:
    string what_;
    SaveStatus save_;
    Exception* next_;
};

class SeriousBug : public Exception {
public:
    SeriousBug(const string& w) : Exception(string("Serious Bug:") + w) {}
    SeriousBug(const string&,const string&);
    SeriousBug(const string&,int);
};

class TooManyRetries : public Exception {
public:
    TooManyRetries(const int);
};

class TimeOut : public Exception {
public:
    TimeOut(const string&, const unsigned long);
};

class FailedSystemCall : public Exception {
public:
    FailedSystemCall(const string&);
    FailedSystemCall(const char*,int,const char*,const char*,int);
    FailedSystemCall(const string&,const char*,int,const char*,const char*,int);
};

class AssertionFailed : public Exception {
public:
    AssertionFailed(const string&);
    AssertionFailed(const char*, const CodeLocation& loc );
};

class BadParameter : public Exception {
public:
    BadParameter(const string& s);
};

class NotImplemented : public Exception {
public:
    NotImplemented( const CodeLocation& );
};

class Stop : public Exception {
public:
    Stop(const string&);
};

class Abort : public Exception {
public:
    Abort(const string&);
};

class Cancel : public Exception {
public:
    Cancel(const string&);
};

class Retry : public Exception {
public:
    Retry(const string&);
};

class UserError : public Exception {
public:
    UserError(const string&);
    UserError(const string&,const string&);
    UserError(const string&,int);
};

class OutOfRange : public Exception {
public:
    OutOfRange(unsigned long long, unsigned long long);
};

// File errors

class FileError : public Exception {
protected:
    FileError(const string&);
    FileError()					{  }
};

class CantOpenFile : public FileError {
    bool retry_;
    virtual bool retryOnServer() const { return retry_; }
public:
    CantOpenFile(const string&,bool retry = false);
};

class WriteError : public FileError {
public:
    WriteError(const string&);
};

class ReadError : public FileError {
public:
    ReadError(const string&);
};

class ShortFile : public ReadError {
public:
    ShortFile(const string&);
};


class RemoteException : public Exception {
public:
    RemoteException(const string& msg, const string& from);
};

// =======================================

inline void SysCall(long long code,const char *msg,int line,const char *file,
    const char *proc)
{
    if(code<0)
        throw FailedSystemCall(msg,line,file,proc,errno);
}
// =======================================
template<class T>
inline void SysCall(long long code,const char *msg,const T& ctx,int line,const char *file,
    const char *proc)
{
    if(code<0)
    {
        StrStream os;
        os << ctx << StrStream::ends;
        throw FailedSystemCall(string(os),msg,line,file,proc,errno);
    }
}
// =======================================

inline void ThrCall(int code,const char *msg,int line,const char *file,
    const char *proc)
{
    if(code != 0) // Threads return errno in return code
        Panic(msg,line,file,proc);
    /*
    {
        throw FailedSystemCall(msg,line,file,proc,code);
    }
    */
}

inline void Assert(int code,const char *msg, const CodeLocation& loc )
{
    if(code != 0)
        throw AssertionFailed(msg,loc);
}

inline void Panic(int code,const char *msg,int line,const char *file,
    const char *proc)
{
    if(code != 0)
        Panic(msg,line,file,proc);
}

//--------------------------------------------------------------
// For compatibility
//--------------------------------------------------------------
class OutOfMemory : public Exception {
    virtual bool terminateApplication() const { return true; }
    virtual const char *what() const throw() {  return "OutOfMemory"; }
public:
    OutOfMemory();

};

#define THRCALL(a) ThrCall(a,#a,__LINE__,__FILE__,__FUNCTION__)
#define SYSCALL(a)   SysCall(a,#a,__LINE__,__FILE__,__FUNCTION__)
#define SYSCALL2(a,b) SysCall(a,#a,b,__LINE__,__FILE__,__FUNCTION__)
#define ASSERT(a)  Assert(!(a),#a, Here() )
#define PANIC(a)   Panic((a),#a,__LINE__,__FILE__,__FUNCTION__)
#define NOTIMP     throw NotImplemented( Here() )


#endif