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

#ifndef SP_COMMON_H
#define SP_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif


/*!
    \file common.h
    \brief common definitions

    \addtogroup config
    @{
 */


/*! maximum number of characters for fully qualified file names in daemon */
#define SP_MAX_PATH          256
#define SP_MAX_MSG_LEN       256                           /*!< maximum IPC message length */
#define SP_MAX_BUF_MSGS      10                            /*!< maximum number of IPC messages in buffer */
#define SP_MAX_ID_LEN        30                            /*!< maximum length of action (verb) id name */


/*! @} */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef SP_COMMON_H */
