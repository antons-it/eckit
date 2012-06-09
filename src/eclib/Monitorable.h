/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File Monitorable.h
// Baudouin Raoult - ECMWF Mar 97

#ifndef Monitorable_H
#define Monitorable_H

#include "eclib/ClassExtent.h"

class JSON;

class Monitorable : public ClassExtent<Monitorable> {
public:

	Monitorable();

// -- Destructor

	virtual ~Monitorable();

// -- Methods

	virtual void json(JSON&) const = 0;
	virtual void status(ostream&) const;

// -- Class methods

	static void allStatuses(ostream&);
	static void allJSON(JSON&);

protected:

// -- Methods
	
	virtual void print(ostream&) const;

private:

// No copy allowed

	Monitorable(const Monitorable&);
	Monitorable& operator=(const Monitorable&);

	friend ostream& operator<<(ostream& s,const Monitorable& p)
		{ p.print(s); return s; }

};


#endif