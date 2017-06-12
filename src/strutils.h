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

#ifndef STRUTILS_H
#define STRUTILS_H

#include <olcutils/cfg_string.h>


#ifdef __cplusplus
extern "C" {
#endif

/*! \file strutils.h
    \brief utility functions

    \addtogroup utils
    @{
 */

/*!
 * transforms buffer to lower case letters
 */
void strtolower( char *p, const int max_len );


/*!
 * transforms buffer to upper case letters
 */
void strtoupper( char *p, const int max_len );


 /*!
  * helper function for removing carriage return / linefeed charaters,
  * replace tabulators by space, etc.
 */
void condition_string( char *p, const int max_len );


/*!
 * helper function for replacing the initial symbol + by 00
 * used to for phone number conditioning for AT interface
 */
int str_replace_plus_by_double_zero( char* dest, const char* source, int max_len );


/*!
 * helper function for conversion of '\0' delimited string to integer
 *
 * \param s pointer input string
 * \param p_i pointer to delivered integer result
 * \param minval minimum allowed value
 * \param maxval maximum allowed value
 * \return 0 in case of success, otherwise negative error code
 */
int cstring2int( const char* s, int* p_i, const int minval, const int maxval );

/*!
 * helper function for conversion of length delimited string to integer
 *
 * \param s pointer input string
 * \param p_i pointer to delivered integer result
 * \param minval minimum allowed value
 * \param maxval maximum allowed value
 * \return 0 in case of success, otherwise negative error code
 */
int string2int( const string_t* s, int* p_i, const int minval, const int maxval );


/*!
 * helper function for conversion of length delimited string to boolean value represented as integer
 *
 * \param s pointer input string
 * \param p_b pointer to delivered integer result
 * \return 0 in case of success, otherwise negative error code
 */
int string2bool( const string_t* s, int* p_b );


/*! @} */

#ifdef __cplusplus
}
#endif


#endif /* #ifndef STRUTILS_H */
