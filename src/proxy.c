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

#define _XOPEN_SOURCE 600
#include <proxy.h>
#include <config.h>
#include <sp_config.h>
#include <olcutils/alloc.h>

#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>

#include <ctype.h>

/*!
 * daemon (threaded) process for proxying communication
 * between physical device and pseudo terminal
 */
static void* proxy_handler( void* p_ctx )
{
  t_proxy* p = (t_proxy *)p_ctx;
  char buf[256];
  ssize_t size;
  int i;

  while( 1 )
  {
    size = read( p->pty_fdm, buf, sizeof( buf ) );
    sp_message("%s, %d: got %d bytes: %.10s\n", __func__, __LINE__, size, buf );

    for( i=0; i < size  &&  buf[i] != 0; ++i )
      buf[i] = toupper( buf[i] );

    write( p->pty_fdm, buf, size );
  }
}


void proxy_kill( t_proxy* p )
{
  if( p ) {
    if( p->running )
      pthread_cancel( p->handler );

    cul_free( p );
  }
}


t_proxy* proxy_init( void )
{
  t_proxy* p = cul_malloc( sizeof( t_proxy ) );
  struct termios slave_orig_term_settings; /* saved terminal settings */
  struct termios new_term_settings; /* current terminal settings */
  int retcode;

  if( p == NULL )
    return p;

  memset( p, 0, sizeof(t_proxy) );

  p->pty_fdm = posix_openpt( O_RDWR );
  if( p->pty_fdm < 0 ) {
    sp_error("%s, %d: error %d on posix_openpt()!\n",
             __func__, __LINE__, errno);
    proxy_kill( p );
    return NULL;
  }

  retcode = grantpt( p->pty_fdm );
  if( retcode != 0 ) {
    sp_error("%s, %d: error %d on grantpt()\n",
             __func__, __LINE__, errno);
    proxy_kill( p );
    return NULL;
  }

  retcode = unlockpt( p->pty_fdm );
  if( retcode != 0 ) {
    sp_error("%s, %d: error %d on unlockpt()\n",
             __func__, __LINE__, errno);
    return NULL;
  }

  /* Open the slave PTY */
  p->pty_fds = open( ptsname( p->pty_fdm ), O_RDWR );
  if( p->pty_fds < 0 ) {
    sp_error("%s, %d: error %d on opening slave pty device!\n",
             __func__, __LINE__, errno);
    return NULL;
  }

  sp_message("got pty descriptor: %s\n", ptsname(p->pty_fdm) );

  /* Save the default parameters of the slave side of the PTY */
  retcode = tcgetattr(p->pty_fds, &slave_orig_term_settings);
  if( retcode ) {
    sp_error("%s, %d: could not save slave PTY params error %d!\n",
             __func__, __LINE__, retcode );
  }

  /* Set raw mode on the slave side of the PTY */
  new_term_settings = slave_orig_term_settings;
  cfmakeraw (&new_term_settings);
  tcsetattr (p->pty_fds, TCSANOW, &new_term_settings);


  retcode = pthread_create( &p->handler, NULL, proxy_handler, p );
  if( ! p->handler )
    retcode = pthread_detach( p->handler );
  if( retcode ) {
    sp_error( "%s: creation of proxy service failed with error %d\n", __func__, retcode );
    proxy_kill( p );
    return NULL;
  }

  p->running = 1;
  return p;
}
