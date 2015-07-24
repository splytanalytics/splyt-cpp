/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#ifndef SPLYT_UNREAL_HTTP_INTERFACE_H_
#define SPLYT_UNREAL_HTTP_INTERFACE_H_

#include <string>
#include <stdexcept>

#include "EngineMinimal.h"
#include "Http.h"
#include "util/util.h"
#include "network/http_interface.h"

namespace splytapi
{
    class Splyt;

    class OverrideHttpInterface : public splytapi::HttpInterface
    {
        private:
            bool response_received;
            std::string http_response;
            virtual void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

        public:
            OverrideHttpInterface();
            ~OverrideHttpInterface(){}
            virtual std::string Post(std::string url, std::string path, std::string headers[], int header_count, std::string content, long timeout);
    };
}
#endif  // SPLYT_UNREAL_HTTP_INTERFACE_H_