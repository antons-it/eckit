/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef mars_linux_h
#define mars_linux_h

#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

// GCC 3.3 is confused about offsetof
static double _offset;
#define member_offset(Z,z)  size_t( reinterpret_cast<char*>(&reinterpret_cast<Z*>(&_offset)->z) - reinterpret_cast<char*>(&_offset))
#define member_size(Z,z)    size_t( sizeof(reinterpret_cast<Z*>(&_offset)->z))

static double keep_gcc_quiet_about_offset_2(double d);
static double keep_gcc_quiet_about_offset_1(double d)
{
	return d*keep_gcc_quiet_about_offset_2(_offset);
}

static double keep_gcc_quiet_about_offset_2(double d)
{
	return d*keep_gcc_quiet_about_offset_1(_offset);
}

#endif /* mars_linux_h */