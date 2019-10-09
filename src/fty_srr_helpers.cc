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

/*
@header
    fty_srr_helpers -
@discuss
@end
*/

#include "fty_srr_rest_classes.h"

/**
 * Send a request and wait reply in synchronous mode.
 * @param subject
 * @param userData
 * @return The Reply or MessageBusException when a time out occurs.
 */
messagebus::UserData sendRequest(const std::string& subject, const messagebus::UserData& userData)
{
    messagebus::UserData response;
    try
    {
        // Client id
        std::string clientId = messagebus::getClientId(AGENT_NAME);
        std::unique_ptr<messagebus::MessageBus> requester(messagebus::MlmMessageBus(END_POINT, clientId));
        requester->connect();
        
        // Build message
        messagebus::Message msg;
        msg.userData() = userData;
        msg.metaData().emplace(messagebus::Message::SUBJECT, subject);
        msg.metaData().emplace(messagebus::Message::FROM, clientId);
        msg.metaData().emplace(messagebus::Message::TO, AGENT_NAME_REQUEST_DESTINATION);
        msg.metaData().emplace(messagebus::Message::COORELATION_ID, messagebus::generateUuid());
        // Send request
        messagebus::Message resp = requester->request(MSG_QUEUE_NAME, msg, DEFAULT_TIME_OUT);
        response = resp.userData();
    }
    catch (messagebus::MessageBusException& ex)
    {
        log_error("Message bus exception %s", ex.what());
    }
    catch (...)
    {
        log_error("Unknown exception to get feature list managed");
    }
    return response;
}

