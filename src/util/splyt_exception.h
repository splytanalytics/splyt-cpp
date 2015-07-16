/*
** Copyright (c) 2015 Knetik, Inc. All rights reserved.
*/
#ifndef SPLYT_SPLYT_RESPONSE_H_
#define SPLYT_SPLYT_RESPONSE_H_

#include <stdexcept>

#include "splyt.h"

namespace splytapi
{
    class splyt_exception : public std::runtime_error
    {
        private:
            SplytResponse response;

        public:
            splyt_exception(SplytResponse resp) : std::runtime_error(resp.GetErrorMessage().c_str()), response(resp){}
            ~splyt_exception(void) throw(){}

            SplytResponse GetResponse()
            {
                return response;
            }
    };
}
#endif  // SPLYT_SPLYT_RESPONSE_H_