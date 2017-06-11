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
#include <sp_log.h>
#include <olcutils/alloc.h>

#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <sys/select.h>
#include <ctype.h>

#include <serialproxy.h>

#define MAX(a,b) ((a) > (b) ? a : b)


/*!
 * daemon (threaded) process for proxying communication
 * between physical device and pseudo terminal
 */
static void* proxy_handler( void* p_ctx )
{
  t_proxy* p = (t_proxy *)p_ctx;
  fd_set readset;
  int maxfd;
  char buf[256];
  ssize_t size;
  int result, i;

  maxfd = MAX( p->pty_fdm, p->phy_fd );

  while( 1 )
  {
    // size = read( p->pty_fdm, buf, sizeof( buf ) );
    do {
      FD_ZERO( &readset );
      FD_SET( p->pty_fdm, &readset );
      FD_SET( p->phy_fd, &readset );
      result = select( maxfd+1, &readset, NULL, NULL, NULL );
    } while( result == -1 && errno == EINTR );

    if( result > 0 )
    {
      if( FD_ISSET( p->phy_fd, &readset ) )
      {
        /* data from physical device availalbe */
        size = read( p->phy_fd, buf, sizeof( buf ) );
        sp_message("%s, %d: got %d bytes form phy: %.10s\n",
                   __func__, __LINE__, size, buf );
        for( i=0; i < size  &&  buf[i] != 0; ++i )
          buf[i] = toupper( buf[i] );
        write( p->pty_fdm, buf, size );
      }
      if( FD_ISSET( p->pty_fdm, &readset ) )
      {
        /* data from pseudo terminal (proxies destination) available */
        size = read( p->pty_fdm, buf, sizeof( buf ) );
        sp_message("%s, %d: got %d bytes form pty: %.10s\n",
                   __func__, __LINE__, size, buf );
        for( i=0; i < size  &&  buf[i] != 0; ++i )
          buf[i] = toupper( buf[i] );
        write( p->phy_fd, buf, size );
      }
    }
    else
    {
      sp_error( "%s, %d: error on select(): %s\n",
                __func__, __LINE__, strerror( errno ) );
    }
  }
}


void proxy_kill( t_proxy* p )
{
  if( p ) {
    if( p->running )
      pthread_cancel( p->handler );

    if( p->pty_fdm > 0 )
      close( p->pty_fdm );

    if( p->pty_fds > 0 )
      close( p->pty_fds );

    if( p->phy_fd > 0 )
      close( p->phy_fd );
    cul_free( p );
  }
}


t_proxy* proxy_init( struct s_serialproxy* p_serialproxy,
                     const char* phy_filename,
                     const char* pty_symlink_name )
{
  t_proxy* p = cul_malloc( sizeof( t_proxy ) );
  struct termios slave_orig_term_settings; /* saved terminal settings */
  struct termios new_term_settings; /* current terminal settings */
  int retcode;

  if( p == NULL )
    return p;

  memset( p, 0, sizeof(t_proxy) );
  p->p_serialproxy = p_serialproxy;

  p->phy_fd = open( phy_filename, O_RDWR );
  if( p->phy_fd < 0 ) {
    sp_error("%s, %d: error %d when opening physical device()!\n",
             __func__, __LINE__, errno);
    proxy_kill( p );
    return NULL;
  }

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

  /* recreate simlink to destination pseudo terminal */
  remove( pty_symlink_name );
  retcode = symlink( ptsname( p->pty_fdm ), pty_symlink_name );
  if( retcode < 0 ) {
    sp_error("%s, %d: error %d when creating destination symlink!\n",
             __func__, __LINE__, retcode);
    return NULL;
  }

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


  /* create and start background handler */
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
