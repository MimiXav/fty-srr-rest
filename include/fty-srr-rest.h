/*  =========================================================================
    fty-srr-rest - Save, restore and reset REST API

    Copyright (C) 2014 - 2018 Eaton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    =========================================================================
*/

#ifndef FTY_SRR_REST_H_H_INCLUDED
#define FTY_SRR_REST_H_H_INCLUDED

//  Include the project library file
#include "fty_srr_rest_library.h"

//  Add your own public definitions here, if you need them

#define END_POINT                       "ipc://@/malamute"
#define AGENT_NAME                      "fty-srr-rest"
#define AGENT_NAME_REQUEST_DESTINATION  "fty-srr"
#define MSG_QUEUE_NAME                  "ETN.Q.IPMCORE.SRR"
#define DEFAULT_TIME_OUT                5

// Action definition
#define GET_ACTION                      "getFeatureList"
#define SAVE_ACTION                     "save"
#define RESTORE_ACTION                  "restore"
#define RESET_ACTION                    "reset"

// Properties definition
#define SRR_VERSION                     "version"
#define ACTIVE_VERSION                  "1.0"
#define FEATURE_LIST_NAME               "featuresList"
#define FEATURE_NAME                    "name"
#define SAVE_STATUS                     "status"
#define SAVE_STATUS_LIST                "statusList"
#define CONFIGURATION                   "configuration"
#define SAVE_ERROR                      "error"

// Status definition
#define STATUS_SUCCESS                  "success"
#define STATUS_FAILED                   "failed"

#endif
