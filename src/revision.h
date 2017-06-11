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

#ifndef MC_REVISION_H
#define MC_REVISION_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \file revision.h
    \brief release string

    \addtogroup utils utils
    @{
 */

/*!
 * provides the revision string which  includes version number and commit
 * id of the daemon application */
extern char g_sp_revision[];


/*! @} */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef MC_REVISION_H */
