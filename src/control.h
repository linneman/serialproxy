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

#ifndef CONTROL_H
#define CONTROL_H

#include <pthread.h>
#include <intercom/server.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \file control.h
    \brief control interface via TCP
    @{
 */

/* forward declaration */
struct s_serialproxy;

/*! event type declaration */
typedef struct s_control {
  struct s_serialproxy*       p_serialproxy;                /*!< back reference to serialproxy instance */
  t_icom_server_state*        server;                       /*!< point to tcp/unix domain socket server instance */
} t_control;


/*!
 * initialize and start background control server
 *
 * \return p instance data
 */
t_control* control_init( struct s_serialproxy* p_serialproxy );


/*!
 * stop control service and release its data
 *
 * \param p instance data to be released
 */
void control_kill( t_control* p );


/*! @} */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CONTROL_H */
