#include "p2p_server/http_p2p_server.h"


int main(int argc, char*argv[]) 
{
    HTTPp2pServer app;
    return app.run(argc, argv);
}