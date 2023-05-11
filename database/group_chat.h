#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class GroupChat{
        private:
            long _id_chat;
            std::string _name;
            std::string _title;

        public:

            static GroupChat fromJSON(const std::string & str);

            long             get_id_chat() const;
            const std::string &get_name() const;
            const std::string &get_title() const;


            long&        id_chat();
            std::string &name();
            std::string &title();

            static void init();
            static std::optional<GroupChat> read_by_id(long id);
            static std::vector<GroupChat> search(std::string name);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif