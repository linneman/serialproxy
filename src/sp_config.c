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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>
#include <pwd.h>
#include <sp_config.h>
#include <strutils.h>
#include <netinet/ip.h>
#include <sp_log.h>

#include <olcutils/alloc.h>
#include <olcutils/cfg_string.h>

#include <unistd.h>
#include <sys/types.h>
#include <strutils.h>

#include <config.h>

char g_proxy_crtl_address[SP_MAX_ADDR_LEN] = { "127.0.0.1" };
int g_proxy_crtl_port = 6000;


static void* free_string_val( void* p )
{
  string_release( (string_t*)p );
  return NULL;
}

int sp_init_config(void)
{
  struct passwd* pw = getpwuid(getuid());
  char local_conf_path[SP_MAX_PATH];
  char global_conf_path1[SP_MAX_PATH], global_conf_path2[SP_MAX_PATH];
  char* pUsedConfFileName;
  char* p_conf_data;
  int conf_data_len;
  FILE* fp;

  int retcode = 0;

  p_conf_data = (char *) cul_malloc( SP_MAX_CONF_SIZE );
  if( ! p_conf_data ) {
    sp_error("%s: out of memory error!\n", __func__ );
    return -1;
  }
  memset( p_conf_data, 0, SP_MAX_CONF_SIZE );

  /* intialize path to local and global conf files */
  snprintf( local_conf_path, sizeof(local_conf_path), "%s/.serialproxy.rc", pw->pw_dir );
  snprintf( global_conf_path1, sizeof(global_conf_path1), "/etc/serialproxy.rc" );
  snprintf( global_conf_path2, sizeof(global_conf_path2), "/usr/local/etc/serialproxy.rc" );

  fp = fopen(local_conf_path, "r" );
  pUsedConfFileName = local_conf_path;
  if( fp ==  NULL )
  {
    fp = fopen(global_conf_path1, "r" );
    pUsedConfFileName = global_conf_path1;

    if( fp == NULL )
    {
      fp = fopen(global_conf_path2, "r" );
      pUsedConfFileName = global_conf_path2;
    }
  }

  if( fp == NULL )
  {
    sp_error( "%s: configuration data neither at %s nor at %s or %s given!\n",
               __func__, local_conf_path, global_conf_path1, global_conf_path2 );
    cul_free( p_conf_data );
    return -1;
  }

  conf_data_len = fread( p_conf_data, 1, SP_MAX_CONF_SIZE-1 /*-1 for null term. ensurance! */, fp );
  if( conf_data_len > 0 ) {
    string_t* s = string_new_from( p_conf_data );
    hm_t* params;
    hm_leaf_node_t* ln;

    params = cfgstring_parse( s );

    ln = hm_find( params, cstring_hash( "proxy-crtl-address" ) );
    if( ln ) {
      string_tmp_cstring_from( ln->val, g_proxy_crtl_address, sizeof( g_proxy_crtl_address ) );
      sp_message("%s: overwrite default proxy control IP address with %s\n",
                 __func__, g_proxy_crtl_address );
    }

    ln = hm_find( params, cstring_hash( "proxy-crtl-port" ) );
    if( ln ) {
      if( ! string2int( ln->val, & g_proxy_crtl_port, 0, 65535 ) ) {
        sp_message("%s: overwrite default proxy control IP port with %d\n", __func__, g_proxy_crtl_port );
      } else {
        sp_error("%s: could not parse IP proxy control port argument error!\n", __func__ );
      }
    }

    string_release( s );
    hm_free_deep( params, 0, free_string_val );

  } else {
    sp_error( "%s: could not read configuration data error!\n" );
  }

  cul_free( p_conf_data );
  fclose( fp );
  return retcode;
}
