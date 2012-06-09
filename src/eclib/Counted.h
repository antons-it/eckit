/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File Counted.h
// Baudouin Raoult - ECMWF Jun 96

#ifndef Counted_H
#define Counted_H

#include "eclib/Mutex.h"

// Subclass from this class if you want reference counting
// object. Remember to use 'virtual' inheritance in case of
// multiple inheritance

class Counted {
public:

// -- Contructors

	Counted() : count_(0) {}

// -- Methods

	void attach();
	void detach();
	unsigned long count() const { return count_; }

// -- Operators

//	void *operator new(size_t s)  { return MemoryPool::fastAllocate(s);}
//	void operator delete(void* p) { MemoryPool::fastDeallocate(p);     } 

protected:

	virtual ~Counted();

private:

// No copy allowed

	Counted(const Counted&);
	Counted& operator=(const Counted&);

// -- Members

	unsigned long count_;
	Mutex mutex_;
};

#endif