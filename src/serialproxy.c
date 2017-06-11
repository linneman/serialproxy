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

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <getopt.h>
#include <netinet/in.h>
#include <signal.h>

#include <serialproxy.h>
#include <sp_config.h>
#include <sp_log.h>
#include <revision.h>
#include <olcutils/alloc.h>
#include <olcutils/memtrace.h>
#include <olcutils/revision.h>
#include <intercom/log.h>
#include <intercom/revision.h>
#include <strutils.h>

#include <config.h>

/*!
 * print help string
 *
 * \param app_name application's name
 */
static void help( const char* app_name )
{
  printf("Invocation: %s [ options ]\n\n", app_name );
  printf("Options:\n");
  printf("--version\n-v\n");
  printf("\tPrints version information.\n\n");
  printf("--help\n-h\n");
  printf("\tThis help screen. For more informaiton refer also to the man page.\n\n");
}

/*!
 * command line argument parsing
 *
 * Pay attention to the fact that most of the configuration is
 * done via a configuration (.rc) file
 *
 * \param argc number of program invocation arguments
 * \param argv vector of string pointers refering to prog name and args
 *
 * \return 0 in case of success, otherwise negative error code
 */
static int parse_args( int argc, char* argv[] )
{
  int optindex, optchar, error = 0;
  const struct option long_options[] =
  {
    { "help", no_argument, NULL, 'h' },
    { "version", no_argument, NULL, 'v' },
  };

  printf("%s - serialproxy - a proxy daemon for serial line communication\n", argv[0] );
  printf("written by Otto Linnemann\n");
  printf("Copyright 2017 GNU General Public Licence. All rights reserved.\n\n");

  while( ( optchar = getopt_long( argc, argv, "hv", long_options, &optindex ) ) != -1 )
  {
    switch( optchar )
    {
    case 'h':
      help( argv[0] );
      error = -1;
      break;

    case 'v':
      sp_message( "Version: %s\n", g_sp_revision );
      error = -1;
      break;

    default:
      sp_error( "input argument error!\n");
      return -1;
    }
  }

  return error;
}


/*!
 * helper function to print the current stack frame
 */
static void print_trace( void )
{
  void *array[10];
  size_t size;
  char **strings;
  size_t i;

  size = backtrace (array, 10);
  strings = backtrace_symbols (array, size);

  sp_error ("Obtained %zd stack frames.\n", size);

  for (i = 0; i < size; i++)
    sp_error ("%s\n", strings[i]);

  free (strings);
}

/*!
 * callback for debugging purposes to print stack frame
 * case of exception (segfault)
 *
 * \param sig_num exception signal
 */
static void segfaulthandler( int sig_num )
{
  sp_error( "!!!!! RECEIVED SEGMENTATION FAULT !!!!!\n" );
  sp_error( "=======================================\n" );
  print_trace();
  exit( -1 );
}



/*!
 * stop threads and release instance data
 *
 * \param pointer to instance data to be released
 */
static void serialproxy_kill( t_serialproxy* p)
{
  if( p ) {
    if( p->proxy )
      proxy_kill( p->proxy );

    cul_free( p );
  }
}


/*!
 * initialize instance data and start threads
 *
 * \param return pointer to instance data or NULL in case of error
 */
static t_serialproxy* serialproxy_init()
{
  t_serialproxy* p = (t_serialproxy *) cul_malloc( sizeof( t_serialproxy ) );

  if( p ) {
    /* initialize event handler */
    memset( p, 0, sizeof(t_serialproxy) );
    p->proxy = proxy_init();
    if( p->proxy == NULL ) {
      serialproxy_kill( p );
      p = NULL;
    }
  } else {
    sp_error( "%s: out of memory error!\n", __func__ );
  }

  return p;
}


static int serialproxy( int argc, char* argv[] )
{
  t_serialproxy*   p;
  cul_allocstat_t allocstat;
  FILE*           free_log_fp = NULL;
  long            run_loop_cnt = 1;

  int             retcode = 0;

  memtrace_enable();
  icom_log_init();
  sp_message("Serialproxy Daemon started\n" );
  sp_message("serialproxy revision: %s\n", g_sp_revision );
  sp_message("libcutils revision: %s\n", g_cutillib_revision );
  sp_message("libintercom revision: %s\n", g_icomlib_revision );

  /* print version info string and parse command line args */
  retcode = parse_args( argc, argv );
  if( retcode )
    return retcode;

  /* read configuration data */
  if( sp_init_config() ) {
    sp_error( "configuration file missing, use default setup!\n");
  }

  /* initialize instance data */
  p = serialproxy_init();
  if( p == NULL ) {
    sp_error( "%s: instance initialization failed!\n", __func__ );
    return -1;
  }

  /* main process loop waits just for termination */
  while( ! p->termination_request ) {
    sleep( 1 );
    if( ! (run_loop_cnt % 10) )
      sp_message("alive\n");
    ++run_loop_cnt;
  }


  /* shut down, stop processing threads and release all memory */
  serialproxy_kill( p );
  memtrace_disable();


  /* memory statistics, leak analysis */
  allocstat = get_allocstat();
  printf("\n\n");
  printf("Memory Allocation Statistics in cutillib functions\n");
  printf("--------------------------------------------------\n");
  printf("       number of open allocations: %ld\n", allocstat.nr_allocs );
  printf("     number of failed allocations: %ld\n", allocstat.nr_allocs_failed );
  printf("  still allocated memory in bytes: %ld\n", allocstat.mem_allocated );
  printf("maximum allocated memory in bytes: %ld\n", allocstat.max_allocated );
  printf("\n");

  if( allocstat.nr_allocs != 0 || allocstat.mem_allocated != 0 )
  {
    fprintf( stderr, "ATTENTION: Some memory is leaking out, configure with memcheck option and check for root cause!\n\n");
    retcode = -1;
  }

  memtrace_print_log( stdout );

  free_log_fp = fopen("/tmp/freelog.txt", "w" );
  if( free_log_fp != NULL )
  {
    memtrace_print_free_log( free_log_fp );
    fclose( free_log_fp );
  }

  icom_log_release();

  return 0;
}


/*!
 * daemon's main function
 *
 * \param argc number of variable argument incl. app name
 * \param argv argument list
 *
 * \return result code
 */
int main( int argc, char* argv[] )
{
  if( signal( SIGSEGV, segfaulthandler ) == SIG_ERR )
    sp_error( "Could not register SIGSEGV error!\n");

  if( signal( SIGFPE, segfaulthandler ) == SIG_ERR )
    sp_error( "Could not register SIGFPE error!\n");

  if( signal( SIGPIPE, segfaulthandler ) == SIG_ERR )
    sp_error( "Could not register SIGPIPE error!\n");

  return serialproxy( argc, argv );
}
