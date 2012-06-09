/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File DayOfYear.h
// Baudouin Raoult - ECMWF Sep 96

#ifndef DayOfYear_H
#define DayOfYear_H

#include "eclib/Date.h"

// Forwarded declarations
class DayOfYear {
public:

// -- Contructors

	DayOfYear(const Date& = Date(0));
	DayOfYear(long,long);
	DayOfYear(const string&);
    
#include "eclib/DayOfYear.b"

// -- Destructor

	~DayOfYear() {}

// -- Convertors
	
	operator string() const;

// -- Operators

	bool operator==(const DayOfYear& other) const
		{ return value_ == other.value_ ;}

	bool operator!=(const DayOfYear& other) const
		{ return value_ != other.value_ ;}

	bool operator<(const DayOfYear& other)  const
		{ return value_ < other.value_; }

	bool operator>(const DayOfYear& other)  const
		{ return value_ > other.value_; }

	bool operator<=(const DayOfYear& other)  const
		{ return value_ <= other.value_; }

	bool operator>=(const DayOfYear& other)  const
		{ return value_ >= other.value_; }


// -- Methods

	void dump(DumpLoad&) const;
	void load(DumpLoad&);

// -- Class methods


// -- Friends

	friend ostream& operator<< (ostream& s, const DayOfYear& month)
		{ month.print(s); return s; }

protected:


// -- Members
	// None

// -- Methods
	// None

// -- Overridden methods
	// None

// -- Class members
	// None

// -- Class methods
	// None

private:

// -- Members
	
	long value_;

// -- Methods

	void print(ostream&) const;

// -- Class methods

};

#endif