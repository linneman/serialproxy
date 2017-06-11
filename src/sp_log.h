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

#ifndef SP_LOG_H_
#define SP_LOG_H_


#ifdef __cplusplus
extern "C" {
#endif

/*! \file mc_log.h
    \brief logging

    \addtogroup utils utils
    @{
 */

#ifndef LOG_TAG
#define LOG_TAG "SP"          /*!< used application log tag in syslog */
#endif


/*!
 * for message log output, currently to stdout
 */
void sp_message( const char* fmt, ... );


/*!
 * for error log output, currently to stderr
 */
void sp_error( const char* fmt, ... );


/*! @} */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef SP_LOG_H_ */
