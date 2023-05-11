#ifndef P_AUTHOR_H
#define P_AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class P2P{
        private:
            long _id;
            long _id_sender;
            long _id_receiver;
            
            std::string _text_message;

        public:

            static P2P fromJSON(const std::string & str);

            long             get_id() const;
            long             get_id_sender() const;
            long             get_id_receiver() const;
            const std::string &get_text_message() const;

            long&        id();
            long&        id_sender();
            long&        id_receiver();
            std::string &text_message();

            static void init();
            static std::vector<P2P>load_message(int s_id, int r_id);
            static void save_to_mysql(long s_id, long r_id,std::string &text);
            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif