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

#include <string.h>
#include <ctype.h>
#include <limits.h>

#include <strutils.h>

void strtolower( char *p, const int max_len )
{
  int i = 0;

  while( (*p!='\0') && (i++ < max_len) )
  {
    *p = tolower( *p );
    ++p;
  }
}

void strtoupper( char *p, const int max_len )
{
  int i = 0;

  while( (*p!='\0') && (i++ < max_len) )
  {
    *p = toupper( *p );
    ++p;
  }
}

void condition_string( char *p, const int max_len )
{
  int i = 0, j = 0;

  /* remove intial tabs, spaces and CR, LF characters */
  while(  (p[i]==' ' || p[i]=='\x09' || p[i]=='\n' || p[i]=='\r') &&
          (p[i]!='\0') && (i < max_len-1) )
    ++i;

  if( i )
  {
    while( p[i]!= '\0' && (i < max_len-1) )
      p[j++] = p[i++];

    p[j++]='\0';
    i = 0;
  }


  /* replace inner spaces tabs and CR, LF characters by spaces */
  while( *p!='\0' )
  {
    switch( (int)*p )
    {
    case 10:
    case 13:
    case 9:
      *p = ' ';
      break;

    default:
      break;
    }

    ++i;
    ++p;
  }

  /* remove trailing tabs and spaces */
  --p;
  while( (*p==' ' || *p == '\x09' ) && (*p!='\0') && (i > 0) )
  {
    --i;
    --p;
  }

  if( i > 0)
    *++p = '\0';
}

int str_replace_plus_by_double_zero( char* dest, const char* source, int max_len )
{
  int is, id;
  int error = 0;

  /* replacement loop */
  for( is=0, id=0; is < max_len && source[is]!='\0'; ++is )
  {
    if( source[is] == '+' )
    {
      if( id < (max_len-1) )
      {
        dest[id++] = '0';
        dest[id++] = '0';
      }
      else
      {
        error = -1;
      }
    }
    else
    {
      if( id < max_len )
      {
        dest[id++] = source[is];
      }
      else
      {
        error = -1;
      }
    }
  }

  /* null termination */
  if( id < max_len )
  {
    dest[id++] = '\0';
  }
  else
  {
    error = -1;
    dest[max_len-1] = '\0';
  }


  return error;
}

int cstring2int( const char* s, int* p_i, const int minval, const int maxval )
{
  char* end = 0;
  long l;
  int retcode = 0;

  l = strtol( s, &end, 10 );
  if( l == LONG_MIN || l == LONG_MAX ) {
    retcode = -1;
  } else  {
    if( l >= minval && l <=maxval ) {
      *p_i = (int)l;
    } else {
      retcode = -2;
    }
  }

  return retcode;
}

int string2int( const string_t* s, int* p_i, const int minval, const int maxval )
{
  char tmp_cstring[30];
  char* end = 0;
  long l;
  int retcode = 0;

  string_tmp_cstring_from( s, tmp_cstring, sizeof( tmp_cstring ) );
  return cstring2int( tmp_cstring, p_i, minval, maxval );
}


int string2bool( const string_t* s, int* p_b )
{
  char tmp_cstring[10];
  int retcode = 0;

  string_tmp_cstring_from( s, tmp_cstring, sizeof( tmp_cstring ) );

  if( !strncmp( tmp_cstring, "true", 4) || !strncmp( tmp_cstring, "TRUE", 4) ||
      !strncmp( tmp_cstring, "True", 4) ) {
    *p_b = 1;
  } else if ( !strncmp( tmp_cstring, "false", 5) || !strncmp( tmp_cstring, "FALSE", 5) ||
              !strncmp( tmp_cstring, "False", 5) ) {
    *p_b = 0;
  } else {
    *p_b = 0;
    retcode = -1;
  }

  return retcode;
}
