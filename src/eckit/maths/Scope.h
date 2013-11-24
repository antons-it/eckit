/*
 * (C) Copyright 1996-2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @file Exp.h
/// @author Baudouin Raoult
/// @author Tiago Quintino
/// @date November 2013

#ifndef eckit_maths_Scope_h
#define eckit_maths_Scope_h

#include "eckit/maths/Expression.h"

//--------------------------------------------------------------------------------------------

namespace eckit {
namespace maths {

//--------------------------------------------------------------------------------------------


class Scope : private boost::noncopyable {
public:

    Scope(const char* name, Scope* = 0);
    Scope(const char* name, ExpPtr, Scope* = 0);
    Scope(const char* name, ExpPtr, ExpPtr, Scope* = 0);

    ~Scope();


    ExpPtr nextArg();
    void pushArg(ExpPtr);

    ExpPtr param(const string&) const;
    void param(const string&, ExpPtr);

private:

    Scope* parent_;
    string name_;

    std::map< string, ExpPtr > params_;
    std::deque< ExpPtr > args_;

    void print( std::ostream& ) const;


    friend std::ostream& operator<<( std::ostream& os, const Scope& v)
    {
        v.print(os);
        return os;
    }

};

//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------

} // namespace maths
} // namespace eckit

#endif