#include <thread>

#include "network/network.h"

namespace splyt
{
    HttpInterface* Network::httpint;

    void Network::Init(HttpInterface* httpint)
    {
        Network::httpint = httpint;
        Log::Info("Network init.");

        Json::Value json;
        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(splyt::user_id);
        json.append(splyt::device_id);
        json.append("");
        json.append("");

        SplytResponse resp = Network::Call("application_init", json);

        if (!resp.IsSuccessful()) {
            throw std::runtime_error("Failed to initialize Splyt: " + resp.GetErrorMessage());
        }
    }

    SplytResponse Network::Call(std::string sub_path, Json::Value content, NetworkCallback callback)
    {
        if(!Network::httpint) {
            throw std::runtime_error("No HTTP implementation available. Did you call splyt::Init()?");
        }

        //If a callback has been passed, create a new thread to run it in and return a dummy response.
        if(callback != NULL) {
            std::thread http_thread (Network::PerformCall, sub_path, content, callback);
            http_thread.detach();

            SplytResponse response(false);
            response.SetErrorMessage("A callback was passed to this function.");
            return response;
        }

        return Network::PerformCall(sub_path, content);
    }

    SplytResponse Network::PerformCall(std::string sub_path, Json::Value content, NetworkCallback callback)
    {
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
        SplytResponse nresp = Network::ParseResponse(str_response);

        if(callback != NULL) {
            //callback(nresp.GetContent()[sub_path]["data"]);
            callback(nresp);
        }

        return nresp;
    }

    SplytResponse Network::ParseResponse(std::string str_response)
    {
        Json::Value root;
        Json::Reader reader;
        bool parsingSuccessful = reader.parse(str_response, root);

        if (parsingSuccessful) {
            Log::Info("RESPONSE: " + root.toStyledString());

            int error_code = root.get("error", -1).asInt();
            bool success = (error_code == kErrorSuccess);

            SplytResponse response(success);

            if (!success) {
                std::string err = Network::InterpretError(error_code);
                Log::Error(err);
                response.SetErrorMessage(err);
            } else {
                response.SetContent(root["data"]);
            }

            return response;
        } else {
            SplytResponse response(false);
            response.SetErrorMessage("Failed to parse JSON response.");
            return response;
            Log::Error("Failed to parse JSON response.");
        }

        SplytResponse response(false);
        response.SetErrorMessage("Unknown error.");

        return response;
    }

    std::string Network::InterpretError(int code)
    {
        switch (code) {
            case kErrorSuccess:
                return "Success.";
            case kErrorGeneric:
                return "Generic error.";
            case kErrorNotInitialized:
                return "Splyt not initialized.";
            case kErrorNotFound:
                return "Path not found.";
            case kErrorInvalidArgs:
                return "Invalid arguments or invalid JSON format.";
            case kErrorMissingId:
                return "Missing ID.";
            case kErrorRequestTimedOut:
                return "Request timed out.";
        }

        return "Unknown error.";
    }
}