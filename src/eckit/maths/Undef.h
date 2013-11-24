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

#ifndef eckit_maths_Undef_h
#define eckit_maths_Undef_h

#include "eckit/maths/Expression.h"


namespace eckit {
namespace maths {


class Undef : public Expression {

    virtual ExpPtr resolve(Scope & ctx);

public: //  methods

    static std::string className() { return "Undef"; }
    static std::string sig() { return "?"; }
    static bool is ( const ExpPtr& e );

    Undef();

    virtual ~Undef();

private:
    virtual ExpPtr optimise() { return shared_from_this(); }

    virtual ValPtr evaluate( Scope& ctx );

    virtual ExpPtr clone();

    virtual std::string typeName() const { return Undef::className(); }
    virtual std::string signature() const { return Undef::sig(); }
    virtual std::string returnSignature() const { return Undef::sig(); }

    virtual void print( std::ostream& o ) const;
    virtual void asCode( std::ostream& ) const;


};

ExpPtr undef();

//--------------------------------------------------------------------------------------------

} // namespace maths
} // namespace eckit

#endif