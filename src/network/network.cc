/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#include "network/network.h"

namespace splytapi
{
    Network::Network(Splyt* sp)
    {
        s = sp;
    }

    Network::~Network()
    {
        splytapi::Log::Info("Freeing network memory.");
        delete httpint;
        httpint = NULL;
    }

    Json::Value Network::Init(HttpInterface* a)
    {
        this->httpint = a;
        Log::Info("Network init.");

        Json::Value json;
        std::string ts = Util::GetTimestampStr();
        json.append(ts);
        json.append(ts);
        json.append(s->user_id);
        json.append(s->device_id);
        json.append("");
        json.append("");

        SplytResponse resp = Network::Call("application_init", json);

        if (!resp.IsSuccessful()) {
            splytapi::ThrowDummyResponseException("Failed to initialize Splyt: " + resp.GetErrorMessage());
        }

        return resp.GetContent();
    }

    SplytResponse Network::Call(std::string sub_path, Json::Value content, std::string context, NetworkCallback callback)
    {
        if(!this->httpint) {
            splytapi::ThrowDummyResponseException("No HTTP implementation available. Did you call splyt::Init()?");
        }

        std::string path = "/" + Config::kSsfApp + "/ws/interface/" + sub_path;

        //Build query string.
        std::string query = "?";
        query.append("&ssf_output=");
        query.append(Config::kSsfOutput);
        query.append("&ssf_cust_id=");
        query.append(s->customer_id);
        query.append("&ssf_ws_version=");
        query.append(Config::kSsfVersion);
        query.append("&ssf_sdk=");
        query.append(SPLYT_SDK_NAME);
        query.append("&ssf_sdk_version=");
        query.append(SPLYT_SDK_VERSION);
        query.append("&ssf_sdk_contextname=");
        query.append(context);

        std::string headers[] = {
            "ssf-use-positional-post-params: true",
            "ssf-contents-not-url-encoded: true"
        };

        Json::FastWriter fast_writer;
        std::string str_response = "";

        try {
            str_response = this->httpint->Post(Config::kNetworkHost, path + query, headers, 2, fast_writer.write(content), Config::kNetworkTimeout, callback);
        } catch (std::runtime_error e) {
            std::string err = e.what();
            splytapi::ThrowDummyResponseException("Network Error: " + err);
        }

        SplytResponse resp = Network::ParseResponse(str_response);
        return resp;
    }

    SplytResponse Network::ParseResponse(std::string str_response)
    {
        Json::Value root;
        Json::Reader reader;
        bool parsingSuccessful = reader.parse(str_response, root);

        if (parsingSuccessful) {
            if (Config::kDebugLog) {
                Log::Info("RESPONSE: " + root.toStyledString());
            }

            int error_code = root.get("error", -1).asInt();
            bool success = (error_code == kErrorSuccess);

            SplytResponse response(success);

            if (!success) {
                std::string err = Network::InterpretError(error_code);
                Log::Error(err);
                response.SetErrorMessage(err);
            } else {
                if (root["data"].isNull()) {
                    response.SetContent(root);
                } else {
                    response.SetContent(root["data"]);
                }
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