/*  =========================================================================
    fty_srr_helpers - class description

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

#ifndef FTY_SRR_HELPERS_H_INCLUDED
#define FTY_SRR_HELPERS_H_INCLUDED

#include <fty_common_messagebus.h>
#include <fty_srr_dto.h>
#include <srr_pb.h>

#include <string>

/**
 * Send a request and wait reply in synchronous mode.
 * @param subject Request subject
 * @param userData User data to send
 * @return The Reply or MessageBusException when a time out occurs.
 */
dto::UserData sendRequest(const dto::srr::Action action, const dto::UserData& userData);

/**
 * Utility to split a string with a delimiter into a string vector.
 * @param input string
 * @param delimiter 
 * @return A list of string splited.
 */
std::vector<std::string> splitString(const std::string input, const char delimiter);

//helpers with SRR
dto::srr::Query saveQueryFromUiJson(const std::string & json);
dto::srr::Query restoreQueryFromUiJson(const std::string & json);
dto::srr::Query resetQueryFromUiJson(const std::string & json);

void operator>>= (const cxxtools::SerializationInfo& si, dto::srr::SaveQuery & query);
void operator>>= (const cxxtools::SerializationInfo& si, dto::srr::RestoreQuery & query);
void operator>>= (const cxxtools::SerializationInfo& si, dto::srr::ResetQuery & query);

std::string responseToUiJson(const dto::srr::Response & response);
void operator<<= (cxxtools::SerializationInfo& si, const dto::srr::Response & response);
void operator<<= (cxxtools::SerializationInfo& si, const dto::srr::SaveResponse & response);
void operator<<= (cxxtools::SerializationInfo& si, const dto::srr::RestoreResponse & response);
void operator<<= (cxxtools::SerializationInfo& si, const dto::srr::ResetResponse & response);
void operator<<= (cxxtools::SerializationInfo& si, const dto::srr::ListFeatureResponse & response);
 

#endif
