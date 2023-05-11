#ifndef M_AUTHOR_H
#define M_AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Message{
        private:
            long _id_message;
            long _id_sender;
            long _id_chat;
            bool _is_changed;
            
            std::string _text_message;
            std::string _datetime;

        public:

            static Message fromJSON(const std::string & str);
            long             get_id_message() const;
            long             get_id_sender() const;
            long             get_id_chat() const;

            const std::string &get_text_message() const;
            const std::string &get_datetime() const;


            long&        id_message();
            long&        id_sender();
            long&        id_chat();
            bool&        is_changed();
            std::string &text_message();
            std::string &datetime();

            static void init();
            static std::vector<Message>load_message(long id);
            void save_to_mysql();
            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif