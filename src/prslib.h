/*
 * prslib.h - main header file, links all headers into one place.
 *
 * Author: Philip R. Simonson
 * Date  : 2019/05/14
 ********************************************************************
 */

#define PRS_CFILE "file.h"
#define PRS_CLOGGER "clogger.h"

#ifdef PRS_CFILE_H
#include PRS_CFILE
#endif

#ifdef PRS_CLOGGER_H
#ifndef PRS_CFILE_H
#error "You must include cfile, clogger depends on this."
#endif
#endif