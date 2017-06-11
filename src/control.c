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

#include <config.h>
#include <sp_config.h>
#include <olcutils/alloc.h>
#include <sp_log.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <control.h>
#include <common.h>
#include <serialproxy.h>


static void sp_print_reply_to_sender( t_icom_evt* p_evt, const char* fmt, ... )
{
  va_list args;
  char reply[SP_MAX_MSG_LEN];

  /* ensure null termination */
  reply[sizeof(reply)-1] = '\0';

  va_start( args, fmt );
  vsnprintf( reply, sizeof(reply) - 1, fmt, args );
  va_end( args );
  icom_reply_to_sender( p_evt, reply, strlen(reply) );
}


static int crtl_server_event_handler( t_icom_evt* p_evt )
{
  t_control* p = (t_control *) p_evt->p_user_ctx;
  char data[128];
  int errors = 0;

  if( p_evt->type == ICOM_EVT_SERVER_CON ) {
    sp_print_reply_to_sender( p_evt, "Serial Proxy Control Interface, enter command or help!\n" );
  }
  else if( p_evt->type == ICOM_EVT_SERVER_DATA ) {
    p_evt->p_data[p_evt->max_data_size-1] = '\0'; /* ensure NULL termination ! */

    if( strstr( p_evt->p_data, "quit" ) != NULL )
    {
      sp_message( "%s, %d: got termination request, exit appliction!\n", __func__, __LINE__ );
      sp_print_reply_to_sender( p_evt, "got termination request, exit appliction!\n" );
      p->p_serialproxy->termination_request = 1;
    }
    else
    {
      sp_print_reply_to_sender( p_evt, "command not recognized error!\n" );
    }
  }

  return errors;
}


void control_kill( t_control* p )
{
  if( p ) {
    if( p->server )
      icom_kill_server_handlers( p->server );

    cul_free( p );
  }
}


t_control* control_init( struct s_serialproxy* p_serialproxy )
{
  t_control* p;
  t_icom_server_decl* p_decl_table;
  int decl_table_len;

  p = cul_malloc( sizeof( t_control ) );
  if( p == NULL ) {
    sp_error( "%s, %d: out of memory error!\n", __func__, __LINE__ );
    return NULL;
  }
  memset( p, 0, sizeof( t_control ) );
  p->p_serialproxy = p_serialproxy;

  sp_get_server_decl_table( & p_decl_table, & decl_table_len );

  p->server = icom_create_server_handlers(
    p_decl_table, decl_table_len, SP_MAX_MSG_LEN, SP_MAX_BUF_MSGS, crtl_server_event_handler, p );

  if( p->server == NULL ) {
    sp_error( "%s, %d: could not create server handlers error!\n", __func__, __LINE__ );
    control_kill( p );
    return NULL;
  }

  return p;
}
