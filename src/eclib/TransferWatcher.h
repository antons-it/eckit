/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File TransferWatcher.h
// Baudouin Raoult - ECMWF Jun 98

#ifndef TransferWatcher_H
#define TransferWatcher_H

class TransferWatcher {
public:

// -- Methods

	virtual void watch(const void*,long) = 0;
	virtual ~TransferWatcher() {};

// -- Class methods

	static TransferWatcher& dummy();

};


#endif