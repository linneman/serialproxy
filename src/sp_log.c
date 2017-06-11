/*
 * serialproxy - a proxy daemon for serial line communication
 * Copyright 2017 Otto Linnemann
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <sp_log.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/syslog.h>


#define LOG_SETTING  ( LOG_NOWAIT | LOG_PID )

void sp_message( const char* fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  openlog( LOG_TAG, LOG_SETTING, LOG_SYSLOG );
  vsyslog( LOG_INFO, fmt, args );
  closelog();
  va_end( args );
}

void sp_error( const char* fmt, ... )
{
  va_list args;

  va_start( args, fmt );
  openlog( LOG_TAG, LOG_SETTING, LOG_SYSLOG );
  vsyslog( LOG_ERR, fmt, args );
  closelog();
  va_end( args );
}
