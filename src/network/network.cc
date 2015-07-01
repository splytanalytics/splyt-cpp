#include "network/network.h"

namespace splyt
{
    HttpInterface* Network::httpint;

    void Network::Init(HttpInterface& httpint)
    {
        Network::httpint = &httpint;
        printf("Network init.\n");

        Network::Call("/isos-personalization/ws/interface/application_init");
    }

    void Network::Call(std::string path)
    {
        Network::httpint->Post(network_host, path, "");
    }
}