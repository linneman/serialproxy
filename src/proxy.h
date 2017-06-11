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

#ifndef PROXY_H
#define PROXY_H

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \file proxy.h
    \brief proxy between physical and virtual serial device
    @{
 */

/* forward declaration */
struct s_serialproxy;

/*! event type declaration */
typedef struct s_proxy {
  struct s_serialproxy*       p_serialproxy;                /*!< back reference to serialproxy instance */
  int                         pty_fdm;                      /*!< file descriptor of master side */
  int                         pty_fds;                      /*!< file descriptor of slave side */
  int                         phy_fd;                       /*!< physical device (UART) */
  pthread_t                   handler;                      /*!< device I/O handler */
  int                         running;                      /*!< 1 when thread is running */
} t_proxy;


/*!
 * initialize and start background proxy service
 *
 * \param p_serialproxy pointer to serialproxy main state structure
 * \param phy_filename physical (real) character device (proxy source)
 * \param phy_simlink_name symlink name to pseudo terminal (proxy dest)
 * \return p instance data
 */
t_proxy* proxy_init( struct s_serialproxy* p_serialproxy,
                     const char* phy_filename,
                     const char* pty_symlink_name );

/*!
 * stop background proxy service and release its data
 *
 * \param p instance data to be released
 */
void proxy_kill( t_proxy* p );


/*! @} */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef PROXY_H */
