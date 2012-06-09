/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File BasePathNameT.h
// Baudouin Raoult - (c) ECMWF Jun 11

#ifndef BasePathNameT_H
#define BasePathNameT_H

#include "eclib/BasePathName.h"

template<class T>
class BasePathNameT : public BasePathName {
public:

// -- Exceptions
	// None

// -- Contructors

	BasePathNameT(const T& path):
		path_(path) {}

	BasePathNameT(const char* path):
		path_(path) {}

	BasePathNameT(const string& path):
		path_(path) {}

// -- Destructor

	//~BasePathNameT(); // Change to virtual if base class

// -- Convertors
	// None

// -- Operators
	// None

// -- Methods
	// None

// -- Overridden methods
	// None

// -- Class members
	// None

// -- Class methods
	// None

protected:

// -- Members
	// None

// -- Methods
	
	void print(ostream&) const; // Change to virtual if base class

// -- Overridden methods
	// None

// -- Class members
	// None

// -- Class methods
	// None

private:

// No copy allowed

	BasePathNameT(const BasePathNameT&);
	BasePathNameT& operator=(const BasePathNameT&);

// -- Members

	T path_;

// -- Methods
	// None

// -- Overridden methods

    virtual BasePathName* clone() const;
    virtual const char* localPath() const;
    virtual Length size() const;
    virtual time_t lastAccess() const;
    virtual time_t lastModified() const;
    virtual time_t created() const;

    virtual void rename(const BasePathName&) const;
    virtual void link(const BasePathName&) const;
    virtual bool sameAs(const BasePathName&) const;
    virtual BasePathName* mountPoint() const;
    virtual bool exists() const;
    virtual bool available() const;
    virtual void mkdir(short) const;
    virtual void unlink() const;
    virtual void rmdir() const;
    virtual void touch() const;
    virtual void children(vector<BasePathName*>&,vector<BasePathName*>&) const;
    virtual void match(vector<BasePathName*>&,bool) const;
    virtual void reserve(const Length&) const;

    virtual BasePathName* unique() const;
    virtual BasePathName* dirName() const;
    virtual BasePathName* fullName() const;
    virtual BasePathName* orphanName() const;
    virtual BasePathName* checkClusterNode() const;
    virtual BasePathName* baseName(bool) const;

    virtual string clusterName() const;

    virtual string asString() const;
    virtual void fileSystemSize(FileSystemSize&) const;

    virtual DataHandle* fileHandle(bool overwrite) const;
    virtual DataHandle* partHandle(const OffsetList&, const LengthList&) const;
    virtual DataHandle* partHandle(const Offset&, const Length&) const;

    virtual const string& node() const;
    virtual const string& path() const;

// -- Class members
	// None

// -- Class methods
	// None

// -- Friends

	friend ostream& operator<<(ostream& s,const BasePathNameT& p)
		{ p.print(s); return s; }

};

#include "eclib/BasePathNameT.cc"

#endif