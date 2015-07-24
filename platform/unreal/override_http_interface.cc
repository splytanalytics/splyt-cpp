/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#include "override_http_interface.h"

namespace splytapi
{
    OverrideHttpInterface::OverrideHttpInterface()
    {

    }

    void OverrideHttpInterface::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
    {
        FString UE4Str = Response->GetContentAsString();
        std::string content(TCHAR_TO_UTF8(*UE4Str));
        this->http_response = content;
        splytapi::Log::Info("RESPONSE CONTENT: " +  this->http_response);
        response_received = true;
    }

    std::string OverrideHttpInterface::Post(std::string url, std::string path, std::string headers[], int header_count, std::string stdcontent, long timeout)
    {
        //Construct URL.
        std::string stdfull_url = url + path;
        FString full_url(stdfull_url.c_str());
        splytapi::Log::Info("URL: " + url + path);
        splytapi::Log::Info("REQUEST CONTENT: " + stdcontent);

        FString content(stdcontent.c_str());

        TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
        HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));

        //Add headers.
        for (int i=0; i<header_count; i++) {
            std::string header = headers[i];
            splytapi::Log::Info("REQUEST HEADER: " + header);

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
        HttpRequest->OnProcessRequestComplete().BindRaw(this, &OverrideHttpInterface::OnResponseReceived);
        if (HttpRequest->ProcessRequest()) {
            long ntimeout = Util::GetTimestamp() + (timeout*1000);
            long last_tick = Util::GetTimestamp();
            while (!this->response_received) {
                FGenericPlatformProcess::Sleep(0.0f);
                long cur_timestamp = Util::GetTimestamp();

                //HttpRequest->Tick(cur_timestamp - last_tick);
                FHttpModule::Get().GetHttpManager().Tick(cur_timestamp - last_tick);

                if (cur_timestamp >= ntimeout) {
                    splytapi::Log::Error("HTTP request timed out.");
                    break;
                }
            }
            this->response_received = false;
        } else {
            splytapi::Log::Error("Failed to process HTTP request.");
        }

        return http_response;
    }

    inline FString ToFString(std::string x)
    {
        FString f(x.c_str());
        return f;
    }
}