/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eclib/machine.h"

#include "eclib/DataHandle.h"
#include "eclib/Exporter.h"

const int MAX_STRING_LEN = 10240;

enum {
    TAG_EOF          = 'X',

    TAG_START_CLASS  = 'C',
    TAG_END_CLASS    = 'c',
    TAG_START_OBJECT = 'O',
    TAG_END_OBJECT   = 'o',
    TAG_START_MEMBER = 'M',
    TAG_END_MEMBER   = 'm',

    TAG_START_SUBOBJECT = 'L',
    TAG_END_SUBOBJECT   = 'l',

    TAG_UNSIGNED     = 'u',
    TAG_SIGNED       = 's',
    TAG_STRING       = 'S',
    TAG_DOUBLE       = 'D',

    TAG_START_SCHEMAS   = '{',
    TAG_END_SCHEMAS   = '}',

    TAG_START_DATABASE  = '[',
    TAG_END_DATABASE    = ']',

};

template<class T> struct Swap {

    static const int half = sizeof(T) >> 1;
    static const int last = sizeof(T) - 1;
    T operator()(T v)
    {
        unsigned char *p = (unsigned char*)&v;
        for(int i = 0; i < half ; i++) std::swap(p[i],p[last-i]);
        return v;
    }
};

// xlc needs this,
// others will complain about double initialization
#if defined( __xlC__ )
template<class T> const int Swap<T>::half = sizeof(T) >> 1;
template<class T> const int Swap<T>::last = sizeof(T) - 1;
#endif

class Endian {
    public:

#ifdef EC_LITTLE_ENDIAN
        template<class T> static T transform(T x)  { return Swap<T>()(x); }
#else
        template<class T> static T transform(T x)  { return x; }
#endif

};

Exporter::Exporter(DataHandle& handle):
    handle_(handle),
    objectCount_(0),
    subCount_(0),
    type_(0),
    location_(0),
    objectId_(0),
    inObject_(false)
{
}

Exporter::~Exporter()
{
}

void Exporter::writeTag(char tag)
{
    ASSERT(sizeof(tag) == 1);
    ASSERT(handle_.write(&tag,1) == 1);
}

char Exporter::readTag()
{
    char tag;
    ASSERT(sizeof(tag) == 1);
    ASSERT(handle_.read(&tag,1) == 1);
    return tag;
}


void Exporter::writeString(const string &s)
{
    size_t len = s.length();
    char buffer[MAX_STRING_LEN];

    if(len) {
        ASSERT(sizeof(s[0]) == 1);
        ASSERT(len <= sizeof(buffer));
    }

    writeTag(TAG_STRING);
    writeUnsigned(len);
    for(int i = 0;  i < len; i++) 
        buffer[i] = s[i];

    ASSERT(handle_.write(buffer,len) == len);
}

void Exporter::writeString(const char* s)
{
    size_t len = strlen(s);

    writeTag(TAG_STRING);
    writeUnsigned(len);
	ASSERT(sizeof(s[0]) == 1);

    ASSERT(handle_.write(s,len) == len);
}

string Exporter::_readString()
{
    try {
        string s;

        size_t len = readUnsigned();
        for(int i = 0;  i < len; i++) {
            char c;
            ASSERT(handle_.read(&c,1) == 1);
            s += c;
        }
        return s;
    }
    catch(...)
    {
        cout << "Error reading string " << *this << endl;
        throw;
    }
}

string Exporter::readString()
{
    ASSERT(readTag() == TAG_STRING);
    return _readString();
}

void Exporter::writeDouble(double d) {
    writeTag(TAG_DOUBLE);
    ASSERT(sizeof(double) == 8);
    d = Endian::transform(d);
    ASSERT(handle_.write(&d,sizeof(d)) == sizeof(d));
}

double Exporter::_readDouble() {
    double d;
    ASSERT(sizeof(double) == 8);
    ASSERT(handle_.read(&d,sizeof(d)) == sizeof(d));
    return Endian::transform(d);
}

double Exporter::readDouble() {
    ASSERT(readTag() == TAG_DOUBLE);
    return _readDouble();
}

void Exporter::writeSigned(long long d) {
    writeTag(TAG_SIGNED);
    ASSERT(sizeof(long long) == 8);
    d = Endian::transform(d);
    ASSERT(handle_.write(&d,sizeof(d)) == sizeof(d));
}

long long Exporter::_readSigned() {
    long long d;
    ASSERT(sizeof(long long) == 8);
    ASSERT(handle_.read(&d,sizeof(d)) == sizeof(d));
    return Endian::transform(d);
}

long long Exporter::readSigned() {
    ASSERT(readTag() == TAG_SIGNED);
    return _readSigned();
}

void Exporter::writeUnsigned(unsigned long long d) {
    writeTag(TAG_UNSIGNED);
    ASSERT(sizeof(unsigned long long) == 8);
    d = Endian::transform(d);
    ASSERT(handle_.write(&d,sizeof(d)) == sizeof(d));
}

unsigned long long Exporter::_readUnsigned() {
    unsigned long long d;
    ASSERT(sizeof(unsigned long long) == 8);
    ASSERT(handle_.read(&d,sizeof(d)) == sizeof(d));
    return Endian::transform(d);
}

unsigned long long Exporter::readUnsigned() {
    ASSERT(readTag() == TAG_UNSIGNED);
    return _readUnsigned();
}



void _export(Exporter& out, unsigned char what) { 
    out.writeUnsigned(what);
} 

void _export(Exporter& out, char what) { 
    out.writeSigned(what);
}

void _export(Exporter& out, unsigned int what) { 
    out.writeUnsigned(what);
} 
void _export(Exporter& out, int what) { 
    out.writeSigned(what);
}

void _export(Exporter& out, unsigned long what) { 
    out.writeUnsigned(what);
} 

void _export(Exporter& out, long what) { 
    out.writeSigned(what);
}

void _export(Exporter& out, unsigned short what) { 
    out.writeUnsigned(what);
} 

void _export(Exporter& out, short what) { 
    out.writeSigned(what);
}

void _export(Exporter& out, unsigned long long what) { 
    out.writeUnsigned(what);
} 

void _export(Exporter& out, long long what) { 
    out.writeSigned(what);
}

void _export(Exporter& out, double what) { 
    out.writeDouble(what);
}

void _startClass(Exporter& out, const string& name) {
    out.writeTag(TAG_START_CLASS);
    out.writeString(name);
}

void _endClass(Exporter& out, const string& name) {
    out.writeTag(TAG_END_CLASS);
    //out.writeString(name);
}

void _startClass(Exporter& out, const char* name) {
    out.writeTag(TAG_START_CLASS);
    out.writeString(name);
}

void _endClass(Exporter& out, const char* name) {
    out.writeTag(TAG_END_CLASS);
    //out.writeString(name);
}

void _startMember(Exporter& out, const char* name) {
    out.writeTag(TAG_START_MEMBER);
    out.writeString(name);
}

void _endMember(Exporter& out, const char* name) {
    out.writeTag(TAG_END_MEMBER);
    //out.writeString(name);
}

void Exporter::startObject(unsigned long long type, unsigned long long location, unsigned long long id, size_t count) {
    writeTag(TAG_START_OBJECT);
    writeUnsigned(type);
    writeUnsigned(location);
    writeUnsigned(id);
    writeUnsigned(count);
    objectCount_++;
    ASSERT(!inObject_);
    inObject_ = true;
    subCount_ = 0;
}

void _startObject(Exporter& e, unsigned long long type, unsigned long long location, unsigned long long id, size_t count) 
{
    e.startObject(type, location, id, count);
}

void Exporter::endObject(unsigned long long type, unsigned long long location, unsigned long long id, size_t count) {
    writeTag(TAG_END_OBJECT);
    ASSERT(inObject_);
    inObject_ = false;
    ASSERT(subCount_);


    /*
       writeUnsigned(type);
       writeUnsigned(location);
       writeUnsigned(id);
       writeUnsigned(count);
       */
}


void Exporter::startSubObject()
{
    writeTag(TAG_START_SUBOBJECT);
    subCount_++;
}

void _startSubObject(Exporter& e)
{
    e.startSubObject();
}

void Exporter::endSubObject()
{
    writeTag(TAG_END_SUBOBJECT);
}

void _endSubObject(Exporter& e)
{
    e.endSubObject();
}

void _endObject(Exporter& e, unsigned long long type, unsigned long long location, unsigned long long id, size_t count) 
{
    e.endObject(type, location, id, count);
}

void Exporter::endObject() {
    ASSERT(readTag() == TAG_END_OBJECT);
    ASSERT(subCount_);
    for(map<string,Datatype>::iterator j = members_.begin(); j != members_.end(); ++j)
        if(!(*j).second.used())
            cout << "WARNING NOT USED [" << (*j).first << "]" << endl;

    members_.clear();
    stack_.clear();
}

bool Exporter::nextDatabase(string& name,unsigned long long& id, unsigned long long& count)
{
    char tag = readTag();
    if(tag == TAG_EOF) {
        return false;
    }

    if(tag != TAG_START_DATABASE) {
        cout << "tag " << int(tag) << endl;
        cout << "tag " << tag << endl;
    }

    ASSERT(tag == TAG_START_DATABASE);

    if(tag == TAG_START_DATABASE) {
        name  = readString();
        id    = readUnsigned();
        count = readUnsigned();
    }
    return true; // should only exit with (tag == TAG_START_DATABASE)
}

size_t Exporter::nextObject() {
    char tag = readTag();

    if(tag == TAG_END_DATABASE) {
        unsigned long long objectCount = readUnsigned();
        cout << "objectCount " << objectCount << " " << objectCount_ << endl;
        ASSERT(objectCount == objectCount_);
        return 0;
    }

	if(tag != TAG_START_OBJECT) {
		cout << tag << endl;

	}

    ASSERT(tag == TAG_START_OBJECT);
    objectCount_++;
    subCount_ = 0;
    stack_.clear();

    try {

        type_     = readUnsigned();
        location_ = readUnsigned();
        objectId_ = readUnsigned();
        return     readUnsigned(); // count

    }
    catch(...) {
        cout << "ERROR reading start object " <<  *this << endl; 
        throw;
    }   

}

void _nextSubObject(Exporter& e)
{
    e.nextSubObject();
}

string Exporter::path() const {
    string s;
    for(vector<string>::const_iterator j = stack_.begin(); j != stack_.end(); ++j) {
        if(s.length()) s += ".";
        s += (*j);
    }
    return s;
}

void Exporter::nextSubObject() {
    ASSERT(readTag() == TAG_START_SUBOBJECT);
    subCount_ ++;

    for(map<string,Datatype>::iterator j = members_.begin(); j != members_.end(); ++j)
        if(!(*j).second.used())
            cout << "WARNING NOT USED [" << (*j).first << "]" << endl;

    members_.clear();

    string s;
    for(;;) {
        char tag = readTag();
        switch(tag) {

            case TAG_START_CLASS:
                s = readString();
                //cout << s << endl;
                //stack_.push_back(s.substr(0,s.find('<')));
				stack_.push_back(s);
                //stack_.push_back(s);
                break;

            case TAG_START_MEMBER:
                stack_.push_back(readString());
                break;

            case TAG_END_MEMBER:
                stack_.pop_back();
                break;

            case TAG_END_CLASS:
                stack_.pop_back();
                break;

            case TAG_UNSIGNED:
                {
                    string p = path();
                    Datatype& x = members_[p];
                    ASSERT(x.empty());
                    x = Datatype(_readUnsigned());
                    //cout << "read [" << p << "] = " << x << endl;
                }
                break;

            case TAG_SIGNED:
                {
                    string p = path();
                    Datatype& x = members_[p];
                    ASSERT(x.empty());
                    x = Datatype(_readSigned());
                    //cout << "read [" << p << "] = " << x << endl;
                }
                break;

            case TAG_DOUBLE:
                {
                    string p = path();
                    Datatype& x = members_[p];
                    ASSERT(x.empty());
                    x = Datatype(_readDouble());
                    //cout << "read [" << p << "] = " << x << endl;
                }
                break;

            case TAG_END_SUBOBJECT:
                return;

            default:
                cout << tag << endl;
                ASSERT(1 == 0);
                break;
        }
    }
}

double Exporter::getDoubleMember(const string& name)
{
    map<string,Datatype>::iterator j = members_.find(name);
    if(j != members_.end())
    {
        //cout << "consume [" << name << "] = " << (*j).second << endl;
        return (*j).second;
    }
    cout << name << " not found" << endl;
    return 0;
}

long long Exporter::getSignedMember(const string& name)
{
    map<string,Datatype>::iterator j = members_.find(name);
    if(j != members_.end())
    {
        //cout << "consume [" << name << "] = " << (*j).second << endl;
        return (*j).second;
    }
    cout << name << " not found" << endl;
    return 0;
}

unsigned long long Exporter::getUnsignedMember(const string& name)
{
    map<string,Datatype>::iterator j = members_.find(name);
    if(j != members_.end())
    {
        //cout << "consume [" << name << "] = " << (*j).second << endl;
        return (*j).second;
    }
    cout << name << " not found" << endl;
    return 0;
}

Evolve::Evolve(Exporter& e):
    e_(e)
{
}

Evolve::Evolve(Evolve* e, char const* klass, char const* name):
    e_(e->e_),
    parent_(e),
    path_(e->path())
{
    if(path_.length()) path_ += ".";
    path_ += klass;

    if(name) {
        path_ += ".";
        path_ += name;
    }
}


Evolve Evolve::operator()(char const* klass, char const* name) {
    return Evolve(this,klass,name);
}

Evolve::operator unsigned char()
{
    return e_.getUnsignedMember(path_);
}

Evolve::operator unsigned int()
{
    return e_.getUnsignedMember(path_);
}

Evolve::operator int()
{
    return e_.getSignedMember(path_);
}

Evolve::operator char()
{
    return e_.getSignedMember(path_);
}

Evolve::operator unsigned short()
{
    return e_.getUnsignedMember(path_);
}

Evolve::operator unsigned long()
{
    return e_.getUnsignedMember(path_);
}

Evolve::operator long long()
{
    return e_.getSignedMember(path_);
}

Evolve::operator unsigned long long()
{
    return e_.getUnsignedMember(path_);
}

Evolve::operator double()
{
    return e_.getDoubleMember(path_);
}

Evolve::operator long()
{
    return e_.getSignedMember(path_);
}

void *operator new(size_t, void* addr, Evolve&) {
    return addr;
}

Exporter::Datatype::Datatype():
    type_(0),
    used_(false)
{
}

Exporter::Datatype::Datatype(double d):
    type_(TAG_DOUBLE),
    double_(d),
    used_(false)
{
}

Exporter::Datatype::Datatype(long long d):
    type_(TAG_SIGNED),
    signed_(d),
    used_(false)
{
}

Exporter::Datatype::Datatype(unsigned long long d):
    type_(TAG_UNSIGNED),
    unsigned_(d),
    used_(false)
{
}

Exporter::Datatype::operator long long() {
    ASSERT(type_ == TAG_SIGNED);
    ASSERT(!used_);
    used_ = true;
    return signed_;
}

Exporter::Datatype::operator unsigned long long() {
    ASSERT(type_ == TAG_UNSIGNED);
    ASSERT(!used_);
    used_ = true;
    return unsigned_;
}

Exporter::Datatype::operator double() {
    ASSERT(type_ == TAG_DOUBLE);
    ASSERT(!used_);
    used_ = true;
    return double_;
}


void Exporter::Datatype::print(ostream& out) const
{
    switch(type_) {
        case TAG_SIGNED:   out << "S(" << signed_; break; 
        case TAG_UNSIGNED: out << "U(" << unsigned_; break; 
        case TAG_DOUBLE:   out << "D(" << double_; break; 
        default: out << "X("; break;
    }

    out <<"," << (used_?"used":"new") << ")";
}

#define X(a) out << " " << #a << "=" << a;

void Exporter::print(ostream& out) const
{ 
    out << "Exporter[";

    X(objectCount_);
    X(subCount_);
    X(type_);
    X(location_);
    X(objectId_);
    X(inObject_);
    X(path());

    out << "]";
}

void Exporter::openForWrite()
{
    handle().openForWrite(0);
}

void Exporter::close()
{
    writeTag(TAG_EOF);
    handle().close();
}

void Exporter::startSchemas()
{
    writeTag(TAG_START_SCHEMAS);
}

void Exporter::endSchemas()
{
    writeTag(TAG_END_SCHEMAS);
}

void Exporter::startDatabase(const string& path, unsigned long id, unsigned long  long count)
{
    PathName home("~");
    string p = path;

    if(p.find(home) == 0) {
         p = string("~/") + p.substr(string(home).length());
    }

    writeTag(TAG_START_DATABASE);
    writeString(p);
    writeUnsigned(id);
    writeUnsigned(count);

}

void Exporter::endDatabase(const string&, unsigned long id)
{
    writeTag(TAG_END_DATABASE);
    writeUnsigned(objectCount_);
    objectCount_ = 0;
}