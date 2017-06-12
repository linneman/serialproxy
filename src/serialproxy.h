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

#ifndef SERIALPROXY_H
#define SERIALPROXY_H

#include <proxy.h>
#include <control.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \file serialproxy.h
    \brief serialproxy main data context

    \addtogroup main
    @{
 */

/*! event type declaration */
typedef struct s_serialproxy {
  t_proxy*                    proxy;                        /*!< pointer to proxy service */
  t_control*                  control;                      /*!< pointer to control server instance */
  int                         termination_request;          /*!< terminate process when set to 1 */
} t_serialproxy;


/*! @} */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef SERIALPROXY_H */
