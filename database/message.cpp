#include "message.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void Message::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Message` (`id_message` INT NOT NULL AUTO_INCREMENT,"
                        << "`id_sender` INT NOT NULL,"
                        << "`id_chat` INT NOT NULL,"
                        << "`text_message` VARCHAR(1024) NULL,"
                        << "`date_time` DATETIME DEFAULT NOW(),"
                        << "`is_changed` BOOLEAN DEFAULT FALSE,"
                        << "PRIMARY KEY (`id_message`));", 
                now;
            
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr Message::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id_message", _id_message);
        root->set("id_sender", _id_sender);
        root->set("id_chat", _id_chat);
        root->set("is_changed", _is_changed);
        root->set("text_message", _text_message);
        root->set("datetime", _datetime);

        return root;
    }

    Message Message::fromJSON(const std::string &str)
    {
        Message message;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        message.id_message() = object->getValue<long>("id_message");
        message.id_sender() = object->getValue<long>("id_sender");
        message.id_chat() = object->getValue<long>("id_chat");
        message.is_changed() = object->getValue<long>("is_changed");
        message.text_message() = object->getValue<long>("id_message");
        message.datetime() = object->getValue<long>("datetime");

        return message;
        
    }

    std::vector<Message> Message::load_message(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Message a;
            std::vector<Message> result;
            select << "SELECT id_sender, text_message FROM Message where id_chat=?",
                into(a._id_sender),
                into(a._text_message),
                use(id),
                range(0, 1); //  iterate over result set one row at a time

            Poco::Data::RecordSet rs(select);
            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            
        }
        return {};
    }

    void Message::save_to_mysql()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);
            insert << "INSERT INTO `Message` (id_sender,id_chat,text_message) VALUES(?, ?, ?)",
                use(_id_sender),
                use(_id_chat),
                use(_text_message);
            insert.execute();
            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id_message),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id_message << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    long Message::get_id_message() const
    {
        return _id_message;
    }

    long Message::get_id_sender() const
    {
        return _id_sender;
    }
    long Message::get_id_chat() const
    {
        return _id_chat;
    }
    const std::string &Message::get_text_message() const
    {
        return _text_message;
    }

    const std::string &Message::get_datetime() const
    {
        return _datetime;
    }

    long &Message::id_message()
    {
        return _id_message;
    }

    long &Message::id_sender()
    {
        return _id_sender;
    }

    long &Message::id_chat()
    {
        return _id_chat;
    }

    bool &Message::is_changed()
    {
        return _is_changed;
    }

    std::string &Message::text_message()
    {
        return _text_message;
    }

    std::string &Message::datetime()
    {
        return _datetime;
    }
}