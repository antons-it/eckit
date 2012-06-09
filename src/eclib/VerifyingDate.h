/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// Baudouin Raoult - ECMWF Sep 96

#ifndef VerifyingDate_H
#define VerifyingDate_H

#include "eclib/DateTime.h"

class VerifyingDate : public DateTime {
public:

// -- Contructors

	VerifyingDate(time_t = ::time(0));
	VerifyingDate(const Date&, const Time&);
	VerifyingDate(const string&);
	VerifyingDate(const DateTime&);

#include "eclib/VerifyingDate.b"
    
// -- Destructor

	~VerifyingDate() {}

// -- Operators

	operator string() const;

// -- Methods

	// -- Class methods
	
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
	
// -- Methods

	void print(ostream&) const;

// -- Class methods

// -- Friends

	friend ostream& operator<<(ostream& s,const VerifyingDate& p)
		{ p.print(s); return s; }

};


#endif