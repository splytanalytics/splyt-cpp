#include <stdexcept>

#include "network/network.h"
#include "util/log.h"

namespace splyt
{
    HttpInterface* Network::httpint;

    /** Initialize the networking class used to make calls to the Splyt API and
        send the initial application_init request.

        @param httpint - HttpInterface used for HTTP requests.
    */
    void Network::Init(HttpInterface& httpint)
    {
        Network::httpint = &httpint;
        Log::Info("Network init.");

        Json::Value json;
        json.append(GetTimestampStr());
        json.append(GetTimestampStr());
        json.append(splyt::user_id);
        json.append(splyt::device_id);
        json.append("");
        json.append("");

        NetworkResponse resp = Network::Call("application_init", json);

        if (resp.IsSuccessful()) {
            Log::Info("Success:\n" + resp.GetContent().toStyledString());
        } else {
            throw std::runtime_error("Failed to initialize Splyt: " + resp.GetErrorMessage());
        }
    }

    /** Make a call to the Splyt API.

        @param std::string sub_path - The path for the API call.
        @param Json::Value content - JSON content sent to Splyt.
        @param NetworkCallback callback - Optional callback for async calls.
    */
    NetworkResponse Network::Call(std::string sub_path, Json::Value content, NetworkCallback callback)
    {
        if(!Network::httpint) {
            throw std::runtime_error("No HTTP implementation available. Did you call splyt::Init()?");
        }

        std::string path = "/isos-personalization/ws/interface/" + sub_path;

        std::string query = "?";
        query.append(SPLYT_QPARM_WSVERSION);
        query.append("&ssf_cust_id=");
        query.append(splyt::customer_id);
        query.append("&");
        query.append(SPLYT_QPARM_OUTPUT);
        query.append("&ssf_sdk=");
        query.append(kNetworkSdk);
        query.append("&ssf_sdk_version=");
        query.append(kNetworkVersion);
        query.append("&ssf_sdk_contextname=testContext");

        std::string headers[] = {
            "ssf-use-positional-post-params: true",
            "ssf-contents-not-url-encoded: true"
        };

        std::string str_response = Network::httpint->Post(kNetworkHost, path + query, headers, 2, content.toStyledString());

        //Log::Info(str_response);

        Json::Value root;
        Json::Reader reader;
        bool parsingSuccessful = reader.parse(str_response, root);

        if (parsingSuccessful) {
            int error_code = root.get("error", -1).asInt();
            bool success = (error_code == Error_Success);

            NetworkResponse response(success);

            if (!success) {
                std::string err = Network::InterpretError(error_code);
                Log::Error(err);
                response.SetErrorMessage(err);
            } else {
                response.SetContent(root["data"]);
            }

            if (callback != NULL) {
                callback(response);
            } else {
                return response;
            }

            //Log::Info(root.get("error", -1).asString());
            //Log::Info(root["data"]["datacollector_beginTransaction"].get("description", "").asString());
        } else {
            NetworkResponse response(false);
            response.SetErrorMessage("Failed to parse JSON response.");
            return response;
            Log::Error("Failed to parse JSON response.");
        }

        NetworkResponse response(false);
        response.SetErrorMessage("Unknown error.");

        if (callback != NULL) {
            callback(response);
        } else {
            return response;
        }
    }

    std::string Network::InterpretError(int code)
    {
        switch (code) {
            case Error_Success:
                return "Success.";
            case Error_Generic:
                return "Generic error.";
            case Error_NotInitialized:
                return "Splyt not initialized.";
            case Error_AlreadyInitialized:
                return "Splyt already initialized.";
            case Error_InvalidArgs:
                return "Invalid arguments or invalid JSON format.";
            case Error_MissingId:
                return "Missing ID.";
            case Error_RequestTimedOut:
                return "Request timed out.";
        }

        return "Unknown error.";
    }
}