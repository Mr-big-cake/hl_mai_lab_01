#include "group_chat_server/http_gc_server.h"


int main(int argc, char*argv[]) 
{
    HTTPGCServer app;
    return app.run(argc, argv);
}