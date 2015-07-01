#include <stdio.h>

#include "splyt.h"
#include "network/network.h"

namespace splyt
{
    /** Initialize the Splyt SDK.

        This must be called first to use any
        functions provided in the SDK.
    */
    void Init(HttpInterface& httpint) {
        printf("Splyt init.\n");

        Network::Init(httpint);
    }
}
