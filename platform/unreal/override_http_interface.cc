/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#include "override_http_interface.h"

namespace splytapi
{
    OverrideHttpInterface::OverrideHttpInterface()
    {

    }

    void OverrideHttpInterface::OnResponseReceivedAsync(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, NetworkCallback callback)
    {
        FString UE4Str = Response->GetContentAsString();
        std::string content(TCHAR_TO_UTF8(*UE4Str));
        SplytResponse response = Network::ParseResponse(content);
        callback(response);
    }

    void OverrideHttpInterface::OnResponseReceivedSync(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
    {
        FString UE4Str = Response->GetContentAsString();
        std::string content(TCHAR_TO_UTF8(*UE4Str));
        this->http_response = content;
        this->response_valid = bWasSuccessful;
        this->response_received = true;
    }

    std::string OverrideHttpInterface::Post(std::string url, std::string path, std::string headers[], int header_count, std::string stdcontent, long timeout, NetworkCallback callback)
    {
        //Construct URL.
        std::string stdfull_url = url + path;
        FString full_url(stdfull_url.c_str());
        splytapi::Log::Info(stdfull_url);
        splytapi::Log::Info("REQUEST: " + stdcontent);

        FString content(stdcontent.c_str());

        TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
        FHttpModule::Get().SetHttpTimeout((float)(timeout + 5));

        HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));

        //Add headers.
        for (int i=0; i<header_count; i++) {
            std::string header = headers[i];

            int del_pos = header.find(":");
            std::string key = header.substr(0, del_pos);
            key = Util::Trim(key);
            std::string value = header.substr(del_pos + 1, header.size());
            value = Util::Trim(value);

            FString uekey(key.c_str());
            FString uevalue(value.c_str());
            HttpRequest->SetHeader(uekey, uevalue);
        }

        HttpRequest->SetURL(full_url);
        HttpRequest->SetVerb(TEXT("POST"));
        HttpRequest->SetContentAsString(content);
        if (callback != NULL) {
            HttpRequest->OnProcessRequestComplete().BindRaw(this, &OverrideHttpInterface::OnResponseReceivedAsync, callback);
        } else {
            HttpRequest->OnProcessRequestComplete().BindRaw(this, &OverrideHttpInterface::OnResponseReceivedSync);
        }

        bool process_result = HttpRequest->ProcessRequest();
        if (!process_result) {
            throw std::runtime_error("Failed to process HTTP request.");
        }

        if (callback != NULL) {
            this->http_response = "";
            return this->http_response;
        }

        if (process_result) {
            double ntimeout = FPlatformTime::Seconds() + timeout;
            double last_tick = FPlatformTime::Seconds();
            while (!this->response_received) {
                double cur_timestamp = FPlatformTime::Seconds();
                double delta =  cur_timestamp - last_tick;

                last_tick = FPlatformTime::Seconds();
                FHttpModule::Get().GetHttpManager().Tick(delta);

                if (cur_timestamp >= ntimeout) {
                    throw std::runtime_error("HTTP request timed out.");
                }
            }
        }

        if (!this->response_valid || this->http_response.empty()) {
            throw std::runtime_error("Invalid HTTP response.");
        }

        this->response_received = false;

        return this->http_response;
    }
}