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

#include <cxxtools/serializationinfo.h>
#include <cxxtools/jsonserializer.h>
#include <cxxtools/jsondeserializer.h>

/**
 * Send a request and wait reply in synchronous mode.
 * @param subject
 * @param userData
 * @return The Reply or MessageBusException when a time out occurs.
 */
dto::UserData sendRequest(dto::srr::Action action, const dto::UserData& userData)
{
    // Client id
    std::string clientId = messagebus::getClientId(AGENT_NAME);
    std::unique_ptr<messagebus::MessageBus> requester(messagebus::MlmMessageBus(END_POINT, clientId));
    requester->connect();

    // Build message
    messagebus::Message msg;
    msg.userData() = userData;
    msg.metaData().emplace(messagebus::Message::SUBJECT, actionToString(action));
    msg.metaData().emplace(messagebus::Message::FROM, clientId);
    msg.metaData().emplace(messagebus::Message::TO, AGENT_NAME_REQUEST_DESTINATION);
    msg.metaData().emplace(messagebus::Message::COORELATION_ID, messagebus::generateUuid());
    // Send request
    messagebus::Message resp = requester->request(MSG_QUEUE_NAME, msg, DEFAULT_TIME_OUT);
    // Return the data response
    return resp.userData();
}

<<<<<<< Updated upstream
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

static cxxtools::SerializationInfo deserializeJson(const std::string & json);
static std::string serializeJson(const cxxtools::SerializationInfo & si, bool beautify = false);


//deserilizer from UI Json
using namespace dto::srr;
static std::string statusToString(Status status);

static const std::map<Status, std::string> statusInString =
{
    {Status::SUCCESS,           "success"},
    {Status::PARTIAL_SUCCESS,   "partialSuccess"},
    {Status::FAILED,            "failed"},
    {Status::UNKNOWN,           "unknown"}
};

Query saveQueryFromUiJson(const std::string & json)
{
    Query query;
    const cxxtools::SerializationInfo si = deserializeJson(json);

    si >>= *(query.mutable_save());

    return query;
}

Query restoreQueryFromUiJson(const std::string & json)
{
    Query query;
    const cxxtools::SerializationInfo si = deserializeJson(json);

    si >>= *(query.mutable_reset());

    return query;
}

Query resetQueryFromUiJson(const std::string & json)
{
    Query query;
    const cxxtools::SerializationInfo si = deserializeJson(json);

    si >>= *(query.mutable_reset());

    return query;
}
        
void operator>>= (const cxxtools::SerializationInfo& si, SaveQuery & query)
{
    si.getMember("passphrase") >>= *(query.mutable_passpharse());

    const cxxtools::SerializationInfo & featuresSi = si.getMember("featuresList");

    for(size_t index = 0; index < featuresSi.memberCount(); index++ )
    {
        const cxxtools::SerializationInfo & featureSi = featuresSi.getMember(index);
        featureSi.getMember("name") >>= *(query.add_features());
    }           
}

void operator>>= (const cxxtools::SerializationInfo& si, RestoreQuery & query)
{
    si.getMember("passphrase") >>= *(query.mutable_passpharse());

    google::protobuf::Map<std::string, Feature > & mapFeaturesData = *(query.mutable_map_features_data());

    const cxxtools::SerializationInfo & featuresSi = si.getMember("features");

    for(size_t index = 0; index < featuresSi.memberCount(); index++ )
    {
        const cxxtools::SerializationInfo & featureSi = featuresSi.getMember(index);

        std::string featureName = featureSi.name();

        Feature f;

        featureSi.getMember("version") >>= *(f.mutable_version());

        const cxxtools::SerializationInfo & dataSi = featureSi.getMember("data");
        std::string data;
        try
        {
            //try to serialize if it's Json format
            data = serializeJson(dataSi);
        }
        catch(const std::exception& /*e*/)
        {
            dataSi >>= data;
        }
        
        f.set_data(data);
        
        mapFeaturesData[featureName] = std::move(f);
    }  
}

void operator>>= (const cxxtools::SerializationInfo& si, ResetQuery & query)        
{
    const cxxtools::SerializationInfo & featuresSi = si.getMember("featuresList");

    for(size_t index = 0; index < featuresSi.memberCount(); index++ )
    {
        const cxxtools::SerializationInfo & featureSi = featuresSi.getMember(index);
        featureSi.getMember("name") >>= *(query.add_features());
    }        
}

std::string responseToUiJson(const Response & response)
{
    cxxtools::SerializationInfo si; 
    si <<= response;
    return serializeJson(si);
}

void operator<<= (cxxtools::SerializationInfo& si, const Response & response)
{
    switch (response.parameters_case())
    {
    case Response::ParametersCase::kSave :
        si <<= response.save();
        break;

    case Response::ParametersCase::kRestore :
        si <<= response.restore();
        break;
    
    case Response::ParametersCase::kReset :
        si <<= response.reset();
        break;

    case Response::ParametersCase::kListFeature :
        si <<= response.list_feature();
        break;
    
    default:
        break;
    }
}

void operator<<= (cxxtools::SerializationInfo& si, const SaveResponse & response)
{
    si.addMember("status") <<= statusToString(getGlobalStatus(response));
    cxxtools::SerializationInfo & featuresSi = si.addMember("features");
    
    for( const auto & item : response.map_features_data())
    {
        cxxtools::SerializationInfo & featureSi = featuresSi.addMember(item.first);
        const auto & featureAndStatus = item.second;

        featureSi.addMember("status") <<= statusToString(featureAndStatus.status().status());
        featureSi.addMember("error") <<= featureAndStatus.status().error();
        featureSi.addMember("version") <<= featureAndStatus.feature().version();

        
        cxxtools::SerializationInfo & data = featureSi.addMember("data");
        
        try
        {
            //try to unserialize the data if they are on Json format
            data = deserializeJson(featureAndStatus.feature().data());
        }
        catch(const std::exception& /* e */)
        {
            //put the data as a string if they are not in Json
            data <<= featureAndStatus.feature().data();
        }
        
    }
}

void operator<<= (cxxtools::SerializationInfo& si, const RestoreResponse & response)
{
    si.addMember("status") <<= statusToString(getGlobalStatus(response));
    cxxtools::SerializationInfo & featuresSi = si.addMember("statusList");
    
    for( const auto & item : response.map_features_status())
    {
        cxxtools::SerializationInfo & featureSi = featuresSi.addMember("");

        featureSi.addMember("name") <<= item.first;
        featureSi.addMember("status") <<= statusToString(item.second.status());
        featureSi.addMember("error") <<= item.second.error();
    }

    featuresSi.setCategory(cxxtools::SerializationInfo::Category::Array);
}

void operator<<= (cxxtools::SerializationInfo& si, const ResetResponse & response)
{
    si.addMember("status") <<= statusToString(getGlobalStatus(response));
    cxxtools::SerializationInfo & featuresSi = si.addMember("statusList");
    
    for( const auto & item : response.map_features_status())
    {
        cxxtools::SerializationInfo & featureSi = featuresSi.addMember("");

        featureSi.addMember("name") <<= item.first;
        featureSi.addMember("status") <<= statusToString(item.second.status());
        featureSi.addMember("error") <<= item.second.error();
    }

    featuresSi.setCategory(cxxtools::SerializationInfo::Category::Array);
}

void operator<<= (cxxtools::SerializationInfo& si, const ListFeatureResponse & response)
{
    si.addMember("status") <<= statusToString(getGlobalStatus(response));
    cxxtools::SerializationInfo & featuresSi = si.addMember("featuresList");
    
    for( const auto & item : response.map_features_dependencies())
    {
        cxxtools::SerializationInfo & featureSi = featuresSi.addMember("");

        featureSi.addMember("name") <<= item.first;
        const FeatureDependencies & dep = item.second;

        std::set<FeatureName> dependencies(dep.dependencies().begin(), dep.dependencies().end());

        featureSi.addMember("dependencies") <<= dependencies;
    }

    featuresSi.setCategory(cxxtools::SerializationInfo::Category::Array);
}

std::string statusToString(Status status)
{
    const auto it = statusInString.find(status);

    if(it != statusInString.end())
    {
        return it->second;
    }
    else
    {
        return "unknown";
    }
}

static cxxtools::SerializationInfo deserializeJson(const std::string & json)
{
    cxxtools::SerializationInfo si;

    try
    {
        std::stringstream input;
        input << json;
        cxxtools::JsonDeserializer deserializer(input);
        deserializer.deserialize(si);
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("Error in the json from server: "+std::string(e.what()));
    }

    return si;

}

static std::string serializeJson(const cxxtools::SerializationInfo & si, bool beautify)
{
    std::string returnData("");

    try
    {
        std::stringstream output;
        cxxtools::JsonSerializer serializer(output);
        serializer.beautify(beautify);
        serializer.serialize(si);

        returnData = output.str();
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("Error while creating json "+std::string(e.what()));
    }

    return returnData;
}
