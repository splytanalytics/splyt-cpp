#ifndef SPLYT_TRANSACTION_H_
#define SPLYT_TRANSACTION_H_

#include "splyt.h"

namespace splytapi
{

    class Transaction
    {
        private:
            Splyt* s;

        public:
            Transaction(Splyt* sp);

            /** Begin a new transaction.

                NOTE: user_id and device_id are optional, but you must at least pass one or the other.
                @param std::string transaction_id
                @param std::string category
                @param int timeout
                @param std::string context
                @param std::string user_id
                @param std::string device_id
                @param Json::Value properties - Optional.

                @return SplytResponse
                @throws splyt_exception
            */
            SplytResponse Begin(std::string transaction_id, std::string category, int timeout, std::string context, std::string user_id = "", std::string device_id = "", Json::Value properties = Json::Value::null);

            /** Update an existing transaction.

                NOTE: user_id and device_id are optional, but you must at least pass one or the other.
                @param std::string transaction_id
                @param std::string category
                @param int progress
                @param std::string context
                @param std::string user_id
                @param std::string device_id
                @param Json::Value properties - Optional.

                @return SplytResponse
                @throws splyt_exception
            */
            SplytResponse Update(std::string transaction_id, std::string category, int progress, std::string context, std::string user_id = "", std::string device_id = "", Json::Value properties = Json::Value::null);

            /** End an existing transaction.

                NOTE: user_id and device_id are optional, but you must at least pass one or the other.
                @param std::string transaction_id
                @param std::string category
                @param std::string result
                @param std::string context
                @param std::string user_id
                @param std::string device_id
                @param Json::Value properties - Optional.

                @return SplytResponse
                @throws splyt_exception
            */
            SplytResponse End(std::string transaction_id, std::string category, std::string result, std::string context, std::string user_id = "", std::string device_id = "", Json::Value properties = Json::Value::null);
    };
}
#endif  // SPLYT_TRANSACTION_H_