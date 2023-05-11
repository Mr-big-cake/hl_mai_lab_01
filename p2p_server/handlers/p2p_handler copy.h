#ifndef USEHANDLER_H
#define USEHANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>
#include <iostream>
#include <fstream>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../../database/p2p.h"
#include "../../helper.h"

static bool hasSubstr(const std::string &str, const std::string &substr)
{
    if (str.size() < substr.size())
        return false;
    for (size_t i = 0; i <= str.size() - substr.size(); ++i)
    {
        bool ok{true};
        for (size_t j = 0; ok && (j < substr.size()); ++j)
            ok = (str[i + j] == substr[j]);
        if (ok)
            return true;
    }
    return false;
}

class P2PHandler : public HTTPRequestHandler
{
public:
    P2PHandler(const std::string &format) : _format(format)
    {
    }

    void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
    // Логика микросервиса - handleRequest. 
    // На вход подается строка запроса request. Задача - заполнить выход - response
    {
        HTMLForm form(request, request.stream());
        try
        {
            //hasSubstr(request.getURI(), "/create_p"
            //request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST
            if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST) // Если `post`
            {
                std::cout<< "POST=OK ";
                if (hasSubstr(request.getURI(), "/send")) // M
                {
                    if (form.has("id_sender") && form.has("id_receiver")&& form.has("text_message"))
                    {
                        database::P2P m;
                        auto id_sender = stoi(form.get("id_sender"));
                        auto id_receiver = stoi(form.get("id_receiver"));
                        auto text_message = form.get("text_message");
                        database::P2P::save_to_mysql(id_sender, id_receiver, text_message); //Просто создаем в БД новую запись
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream &ostr = response.send();
                        ostr << id_sender;
                        std::cout << "OK:send" << std::endl;
                        return;
                    }
                }
            }
            else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET) // Если `get`
            {
                if (hasSubstr(request.getURI(), "/load")) // M
                {
                    if (form.has("id_chat")&&form.has("is_receiver"))
                    {
                        int id_chat = stoi(form.get("id_chat")); 
                        int id_recevier = stoi(form.get("id_recevier")); 
                        auto results = database::P2P::load_message(id_chat, id_recevier);
                        Poco::JSON::Array arr;
                        for (auto s : results)
                            arr.add(s.toJSON());
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(arr, ostr);
                        std::cout << "OK:p2p-load" << std::endl;
                        return;
                    }
                }
            }
        }
        catch (...)
        {
        }

        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("type", "/errors/not_found");
        root->set("title", "Internal exception");
        root->set("status", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        root->set("detail", "request ot found");
        root->set("instance", "/p");
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }

private:
    std::string _format;
};
#endif