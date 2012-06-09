/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef eclib_CallbackLogger_H
#define eclib_CallbackLogger_H

#include "eclib/machine.h"

#include "eclib/Logger.h"

class CallbackLogger : public Logger {
public:
    
    // -- Constructor

    CallbackLogger( std::ostream& out );

    // -- Destructor

    virtual ~CallbackLogger();
    
    // -- Methods
    
    virtual void beginLine();
    
    virtual void endLine();
    
    virtual std::ostream& out() = 0;
    
private:
    
    std::ostream* out_;
  
};

#endif // eclib_CallbackLogger_h