#include "p2p.h"
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

    void P2P::init()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `P2P` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`id_sender` INT NOT NULL,"
                        << "`id_receiver` INT NOT NULL,"
                        << "`text_message` VARCHAR(1024) NULL,"
                        << "PRIMARY KEY (`id`));", 
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

    Poco::JSON::Object::Ptr P2P::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("id", _id);
        root->set("id_sender", _id_sender);
        root->set("id_receiver", _id_receiver);
        root->set("text_message", _text_message);
        return root;
    }

    P2P P2P::fromJSON(const std::string &str)
    {
        P2P message;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        message.id() = object->getValue<long>("id");
        message.id_sender() = object->getValue<long>("id_sender");
        message.id_receiver() = object->getValue<long>("id_receiver");
        message.text_message() = object->getValue<long>("id_message");
        return message;
    }

    std::vector<P2P> P2P::load_message(int s_id,  int r_id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            P2P a;
            std::vector<P2P> result;
            select << "SELECT id_sender, id_receiver, text_message FROM P2P where id_sender = ? and id_receiver = ?",
                into(a._id_sender),
                into(a._id_receiver),
                into(a._text_message),
                use(s_id),
                use(r_id),
                range(0, 1); //  iterate over result set one row at a time
            Poco::Data::RecordSet rs(select);
            while (!select.done())
            {   
                if (select.execute()){
                    result.push_back(a);
                }
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

    void P2P::save_to_mysql(long s_id, long r_id, std::string &text)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);
            insert << "INSERT INTO `P2P` (id_sender,id_receiver,text_message) VALUES(?, ?, ?)",
                use(s_id),
                use(r_id),
                use(text);
            insert.execute();
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

    long P2P::get_id() const
    {
        return _id;
    }

    long P2P::get_id_sender() const
    {
        return _id_sender;
    }
    long P2P::get_id_receiver() const
    {
        return _id_receiver;
    }
    const std::string &P2P::get_text_message() const
    {
        return _text_message;
    }

    long &P2P::id()
    {
        return _id;
    }

    long &P2P::id_sender()
    {
        return _id_sender;
    }
    long &P2P::id_receiver()
    {
        return _id_receiver;
    }

    std::string &P2P::text_message()
    {
        return _text_message;
    }

}