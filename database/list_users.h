#ifndef LU_AUTHOR_H
#define LU_AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class ListUsers{
        private:
            long _id_chat;
            long _id_user;

        public:

            static ListUsers fromJSON(const std::string & str);

            long             get_id_chat() const;
            long             get_id_user() const;

            long&        id_chat();
            long&        id_user();

            static void init();

            static std::vector<ListUsers> search(std::string name);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}
#endif
