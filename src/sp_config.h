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

#ifndef SP_CONFIG_H
#define SP_CONFIG_H

#include <intercom/server.h>

#ifdef __cplusplus
extern "C" {
#endif


/*! \file sp_config.h
    \brief server configuration

    \addtogroup config config
    @{
 */

/*! maximum length for server addresses according to posix standards */
#define SP_MAX_ADDR_LEN      108


/*! maximum size of configuration file */
#define SP_MAX_CONF_SIZE     4096


/*! maximum file path length */
#define SP_MAX_PATH          256


/*!
 * TCP/UDS address of serialproxy serving  control interface for external handler
 * to public switched telephony network (modem).
 *
 * ATTENTION: Later on the client/server rules might be exchanged!
 */
extern char g_proxy_crtl_address[SP_MAX_ADDR_LEN];


/*!
 * TCP port of serialproxy serving control interface for external
 * handler to public switched telephony network (modem). Specify
 * negative value to use unix domain socket e.g. /tmp/serialproxy.uds.
 */
extern int g_proxy_crtl_port;


/*!
 * maximum allowed connections via TCP/IP or
 * zero for disabling TCP/IP communication
 */
extern int  g_proxy_crtl_ip_max_connections;


/*!
 * served socket file or empty string when no socket file shall be served
 */
extern char g_proxy_crtl_socket_filename[SP_MAX_ADDR_LEN];


/*!
 * maximum allowed connections via Unix Domain Sockets
 * or zero for disabling UDS communication
 */
extern int  g_proxy_crtl_socket_max_connections;


/*!
 * physical (real) character device (proxy source)
 */
extern char g_phy_filename[SP_MAX_ADDR_LEN];


/*!
 * symlink name to pseudo terminal (proxy dest)
 */
extern char g_pty_symlink_name[SP_MAX_ADDR_LEN];



/*!
 * retrieve pointers to server address specification table and table len
 */
void sp_get_server_decl_table( t_icom_server_decl** h_decl_table, int* p_len );


/*!
 * read configuration data from ~/.serialproxy.rc, /etc/serialproxy.rc
 * or /usr/local/etc/serialproxy.rc
 * return 0 in case of success, otherwise error code
 */
int sp_init_config(void);


/*! @} */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef SP_CONFIG_H */
