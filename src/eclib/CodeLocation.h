/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef eclib_CodeLocation_h
#define eclib_CodeLocation_h

#include "eclib/machine.h"

class CodeLocation {

public:
    
    friend ostream& operator<<( ostream& s, const CodeLocation& loc ) { loc.print(s); return s; }
	
    // -- Contructors
    
    CodeLocation( const char * file, int line, const char * func );

    // -- Class methods
    
    /// @return as std::string
    std::string asString() const;
    
    /// conversion operator
    operator std::string() const { return asString(); }
    
    const char * file() const { return file_; }
    const char * func() const { return func_; }
    int line() const { return line_; }
    
private:
    
    // Members    

    int          line_;
    const char * file_; 
    const char * func_;

    // Methods
    
     void print(ostream&) const;
};

// Macros

#define Here() CodeLocation( __FILE__ , __LINE__ , __FUNCTION__ )

#endif // eclib_CodeLocation_h