#include <stdexcept>

#include "network/network.h"

namespace splyt
{
    HttpInterface* Network::httpint;

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

        if (!resp.IsSuccessful()) {
            throw std::runtime_error("Failed to initialize Splyt: " + resp.GetErrorMessage());
        }
    }

    NetworkResponse Network::Call(std::string sub_path, Json::Value content, NetworkCallback callback)
    {
        if(!Network::httpint) {
            throw std::runtime_error("No HTTP implementation available. Did you call splyt::Init()?");
        }

        std::string path = "/" + Config::kSsfApp + "/ws/interface/" + sub_path;

        //Build query string.
        std::string query = "?";
        query.append("&ssf_output=");
        query.append(Config::kSsfOutput);
        query.append("&ssf_cust_id=");
        query.append(splyt::customer_id);
        query.append("&ssf_ws_version=");
        query.append(Config::kSsfVersion);
        query.append("&ssf_sdk=");
        query.append(Config::kNetworkSdk);
        query.append("&ssf_sdk_version=");
        query.append(Config::kNetworkVersion);
        query.append("&ssf_sdk_contextname=testContext");

        std::string headers[] = {
            "ssf-use-positional-post-params: true",
            "ssf-contents-not-url-encoded: true"
        };

        Json::FastWriter fast_writer;
        std::string str_response = Network::httpint->Post(Config::kNetworkHost, path + query, headers, 2, fast_writer.write(content));

        Json::Value root;
        Json::Reader reader;
        bool parsingSuccessful = reader.parse(str_response, root);

        if (parsingSuccessful) {
            Log::Info(root.toStyledString());

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
            case Error_NotFound:
                return "Path not found.";
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