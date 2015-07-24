/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#include "override_http_interface.h"

namespace splytapi
{
    OverrideHttpInterface::OverrideHttpInterface()
    {

    }

    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
    {
        FString UE4Str = Response->GetContentAsString();
        std::string content(TCHAR_TO_UTF8(*UE4Str));
        splytapi::Log::Info("RESPONSE CONTENT: " +  content);
    }

    std::string OverrideHttpInterface::Post(std::string url, std::string path, std::string headers[], int header_count, std::string stdcontent, long timeout)
    {
        std::string response = "{\"test\": \"test\"}";

        //Construct URL.
        std::string stdfull_url = url + path;
        FString full_url(stdfull_url.c_str());
        splytapi::Log::Info("URL: " + url + path);

        FString content(stdcontent.c_str());

        TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
        HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
        HttpRequest->SetURL(full_url);
        HttpRequest->SetVerb(TEXT("POST"));
        HttpRequest->SetContentAsString(content);
        HttpRequest->OnProcessRequestComplete().BindRaw(this, &OverrideHttpInterface::OnResponseReceived);
        HttpRequest->ProcessRequest();

        return response;
    }

    inline FString ToFString(std::string x)
    {
        FString f(x.c_str());
        return f;
    }
}