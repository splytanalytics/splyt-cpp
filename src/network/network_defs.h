#ifndef SPLYT_NETWORK_DEFS_H_
#define SPLYT_NETWORK_DEFS_H_

namespace splyt
{
    class NetworkResponse
    {
        private:
            bool success;
            std::string error_message;
            Json::Value content;

        public:
            NetworkResponse(bool s) {
                success = s;
            }

            void SetErrorMessage(std::string err) {
                error_message = err;
            }

            void SetContent(Json::Value c) {
                content = c;
            }

            bool IsSuccessful() {
                return success;
            }

            std::string GetErrorMessage() {
                return error_message;
            }

            Json::Value GetContent() {
                return content;
            }
    };

    typedef void (*NetworkCallback)(Json::Value);
}
#endif  // SPLYT_NETWORK_DEFS_H_