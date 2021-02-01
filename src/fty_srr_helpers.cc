/*  =========================================================================
    fty_srr_helpers - class description

    Copyright (C) 2014 - 2020 Eaton

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

/*
@header
    fty_srr_helpers -
@discuss
@end
*/

#include "fty_srr_rest_classes.h"

#include <cxxtools/serializationinfo.h>
#include <cxxtools/jsonserializer.h>
#include <cxxtools/jsondeserializer.h>

#include <fty_common_json.h>

/**
 * Send a request and wait reply in synchronous mode.
 * @param subject
 * @param userData
 * @return The Reply or MessageBusException when a time out occurs.
 */
dto::UserData sendRequest(const std::string& action, const dto::UserData& userData)
{
    // Client id
    std::string clientId = messagebus::getClientId(AGENT_NAME);
    std::unique_ptr<messagebus::MessageBus> requester(messagebus::MlmMessageBus(END_POINT, clientId));
    requester->connect();

    // Build message
    messagebus::Message msg;
    msg.userData() = userData;
    msg.metaData().emplace(messagebus::Message::SUBJECT, action);
    msg.metaData().emplace(messagebus::Message::FROM, clientId);
    msg.metaData().emplace(messagebus::Message::TO, AGENT_NAME_REQUEST_DESTINATION + std::string("-ui"));
    msg.metaData().emplace(messagebus::Message::CORRELATION_ID, messagebus::generateUuid());
    // Send request
    messagebus::Message resp = requester->request(MSG_QUEUE_NAME + std::string(".UI"), msg, DEFAULT_TIME_OUT);
    // Return the data response
    return resp.userData();
}

/**
 * Utility to split a string with a delimiter into a string vector.
 * @param input string
 * @param delimiter
 * @return A list of string splited.
 */
std::vector<std::string> splitString(const std::string input, const char delimiter)
{
    std::vector<std::string> resultList;
    std::stringstream ss(input);

    std::string token;
    while (std::getline(ss, token, delimiter))
    {
        resultList.push_back (token);
    }
    return resultList;
}

/**
 * Utility to adding the session token from an existing payload.
 * @param input string
 * @param sessionToken string
 * @return A json payload with the session token.
 */
const std::string addSessionToken(const std::string input, const std::string sessionToken)
{
  cxxtools::SerializationInfo si;
  JSON::readFromString(input, si);
  // TODO replacing "SESSION_TOKEN" by SESSION_TOKEN
  if (si.findMember("SESSION_TOKEN") == NULL)
  {
    si.addMember("SESSION_TOKEN") <<= sessionToken;
  }
  return JSON::writeToString (si, false);
}
