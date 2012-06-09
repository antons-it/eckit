/*
 * (C) Copyright 1996-2012 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#include "eclib/Application.h"
#include "eclib/Log.h"
#include "eclib/Logger.h"
#include "eclib/LogBuffer.h"
#include "eclib/Monitor.h"

LogStream::LogStream(Logger* logger):
    ostream( new LogBuffer( logger ) )
{
    setformat(*this,Log::compactFormat);
}

LogStream::~LogStream()
{
    delete (LogBuffer*)rdbuf();
}

LogBuffer::LogBuffer( Logger* logger ) :
    streambuf(),
    logger_(logger),
    start_(true),
    line_(0),
    len_(0)
{
    setp(buffer_, buffer_ + sizeof(buffer_));
    setg(0, 0, 0);
    //	unbuffered(1);
}

void LogBuffer::source(const CodeLocation &where)
{
    logger_->location(where);
}

LogBuffer::~LogBuffer()
{
    dumpBuffer();
    logger_->out() << flush;
}

int	LogBuffer::overflow(int c)
{
    if (c == EOF) {
        sync();
        return 0;
    }

    dumpBuffer();
    sputc(c);

    return 0;
}

int	LogBuffer::sync()
{
    dumpBuffer();
    start_ = true;
    logger_->out() << flush;
    return 0;
}

static void _debug(const char*,const char*);

void LogBuffer::dumpBuffer(void)
{
    Monitor::out(pbase(),pptr());
    
    const char *p = pbase();
    
    while(p != pptr())
    {
        if(start_)
        {
            logger_->beginLine();
            start_ = false;
        }

        if(*p == '\n')
        {
            logger_->endLine();
            start_ = true;
        }

        logger_->out() << *p;
        p++;
    }

    setp(pbase(), epptr());

    logger_->out() << flush;
}