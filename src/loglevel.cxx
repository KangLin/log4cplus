// Module:  Log4CPLUS
// File:    loglevel.cxx
// Created: 6/2001
// Author:  Tad E. Smith
//
//
// Copyright 2001-2009 Tad E. Smith
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <log4cplus/loglevel.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/internal/internal.h>
#include <algorithm>


namespace log4cplus
{


namespace
{

static tstring const ALL_STRING (LOG4CPLUS_TEXT("ALL"));
static tstring const TRACE_STRING (LOG4CPLUS_TEXT("TRACE"));
static tstring const DEBUG_STRING (LOG4CPLUS_TEXT("DEBUG"));
static tstring const INFO_STRING (LOG4CPLUS_TEXT("INFO"));
static tstring const WARN_STRING (LOG4CPLUS_TEXT("WARN"));
static tstring const ERROR_STRING (LOG4CPLUS_TEXT("ERROR"));
static tstring const FATAL_STRING (LOG4CPLUS_TEXT("FATAL"));
static tstring const OFF_STRING (LOG4CPLUS_TEXT("OFF"));
static tstring const NOTSET_STRING (LOG4CPLUS_TEXT("NOTSET"));
static tstring const UNKNOWN_STRING (LOG4CPLUS_TEXT("UNKNOWN"));


struct log_levels_table_rec
{
    LogLevel const ll;
    tstring const * const str;
};


#define DEF_LLTAB_REC(x) { x ## _LOG_LEVEL, &(x ## _STRING) }

static log_levels_table_rec const log_levels_table[8] = {
    DEF_LLTAB_REC (OFF),
    DEF_LLTAB_REC (FATAL),
    DEF_LLTAB_REC (ERROR),
    DEF_LLTAB_REC (WARN),
    DEF_LLTAB_REC (INFO),
    DEF_LLTAB_REC (DEBUG),
    DEF_LLTAB_REC (TRACE),
    DEF_LLTAB_REC (ALL),
};

#undef DEF_LLTAB_REC


static
tstring const &
defaultLogLevelToStringMethod(LogLevel ll)
{
    switch(ll) {
        case OFF_LOG_LEVEL:     return OFF_STRING;
        case FATAL_LOG_LEVEL:   return FATAL_STRING;
        case ERROR_LOG_LEVEL:   return ERROR_STRING;
        case WARN_LOG_LEVEL:    return WARN_STRING;
        case INFO_LOG_LEVEL:    return INFO_STRING;
        case DEBUG_LOG_LEVEL:   return DEBUG_STRING;
        case TRACE_LOG_LEVEL:   return TRACE_STRING;
        //case ALL_LOG_LEVEL:     return ALL_STRING;
        case NOT_SET_LOG_LEVEL: return NOTSET_STRING;
    };
    
    return internal::empty_str;
}


static
LogLevel
defaultStringToLogLevelMethod(const tstring& arg)
{
    tstring s = helpers::toUpper(arg);

    size_t const tbl_size
        = sizeof (log_levels_table) / sizeof (log_levels_table[0]);

    for (log_levels_table_rec const * it = log_levels_table;
        it != log_levels_table + tbl_size; ++it)
    {
        if (*it->str == arg)
            return it->ll;
    }
    
    return NOT_SET_LOG_LEVEL;
}

} // namespace



//////////////////////////////////////////////////////////////////////////////
// public static methods
//////////////////////////////////////////////////////////////////////////////

LogLevelManager&
getLogLevelManager() 
{
    static LogLevelManager singleton;
    return singleton;
}



//////////////////////////////////////////////////////////////////////////////
// LogLevelManager ctors and dtor
//////////////////////////////////////////////////////////////////////////////

LogLevelManager::LogLevelManager() 
{
    toStringMethods.push_back (defaultLogLevelToStringMethod);
    fromStringMethods.push_back (defaultStringToLogLevelMethod);
}


LogLevelManager::~LogLevelManager() 
{ }



//////////////////////////////////////////////////////////////////////////////
// LogLevelManager public methods
//////////////////////////////////////////////////////////////////////////////

tstring const &
LogLevelManager::toString(LogLevel ll) const
{
    for (LogLevelToStringMethodList::const_iterator it
        = toStringMethods.begin (); it != toStringMethods.end (); ++it)
    {
        tstring const & ret = (*it) (ll);
        if (! ret.empty ())
            return ret;
    }

    return UNKNOWN_STRING;
}


LogLevel 
LogLevelManager::fromString(const tstring& s) const
{
    for (StringToLogLevelMethodList::const_iterator it
        = fromStringMethods.begin (); it != fromStringMethods.end (); ++it)
    {
        LogLevel ret = (*it) (s);
        if (ret != NOT_SET_LOG_LEVEL)
            return ret;
    }
    
    return NOT_SET_LOG_LEVEL;
}


void 
LogLevelManager::pushToStringMethod(LogLevelToStringMethod newToString)
{
    toStringMethods.push_back (newToString);
}


void 
LogLevelManager::pushFromStringMethod(StringToLogLevelMethod newFromString)
{
    fromStringMethods.push_back (newFromString);
}

        
} // namespace log4cplus