#include "group_chat.h"
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

    void GroupChat::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `GroupChat` (`id_chat` INT NOT NULL AUTO_INCREMENT,"
                        << "`name` VARCHAR(256) NOT NULL,"
                        << "`title` VARCHAR(1024) NULL,"
                        << "PRIMARY KEY (`id_chat`));",
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

    Poco::JSON::Object::Ptr GroupChat::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id_chat);
        root->set("first_name", _title);

        return root;
    }

    GroupChat GroupChat::fromJSON(const std::string &str)
    {
        GroupChat group_chat;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        group_chat.id_chat() = object->getValue<long>("id_chat");
        group_chat.name() = object->getValue<std::string>("name");
        group_chat.title() = object->getValue<std::string>("title");
        
        return group_chat;
    }

    std::optional<GroupChat> GroupChat::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            GroupChat a;
            select << "SELECT id_chat, name, title FROM GroupChat where id=?",
                into(a._id_chat),
                into(a._name),
                into(a._title),
                use(id),
                range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return a;
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

    std::vector<GroupChat> GroupChat::search(std::string name)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<GroupChat> result;
            GroupChat a;
            name += "%";
            select << "SELECT id_chat, name, title FROM GroupChat where name LIKE ? and last_name LIKE ?",
                into(a._id_chat),
                into(a._name),
                into(a._title),
                use(name),
                range(0, 1); //  iterate over result set one row at a time

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
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    void GroupChat::save_to_mysql()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO GroupChat (name,title) VALUES(?, ?)",

                use(_name),
                use(_title);

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id_chat),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id_chat << std::endl;
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

    long GroupChat::get_id_chat() const
    {
        return _id_chat;
    }

    const std::string &GroupChat::get_name() const
    {
        return _name;
    }

    const std::string &GroupChat::get_title() const
    {
        return _title;
    }

    long &GroupChat::id_chat()
    {
        return _id_chat;
    }

    std::string &GroupChat::name()
    {
        return _name;
    }

    std::string &GroupChat::title()
    {
        return _title;
    }
}