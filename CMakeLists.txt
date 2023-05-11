cmake_minimum_required(VERSION 3.2)

project(hl_server C CXX)

SET (EXAMPLE_BINARY "hl_mai_lab_01")
SET (OTHER_SERVICE "other_service")
SET (P2P_SERVICE "p2p_service")
SET (GC_SERVICE "group_chat_service")

find_package(OpenSSL)
find_package(Threads)
find_package(ZLIB)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL)


if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})
include_directories("/usr/include/mysql")
include_directories("/usr/local/include/mysql")
link_directories("/usr/local/lib")

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${EXAMPLE_BINARY} main.cpp 
                                 config/config.cpp 
                                 database/database.cpp
                                 database/list_users.cpp
                                 database/user.cpp
                                 database/message.cpp)


target_include_directories(${EXAMPLE_BINARY} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${EXAMPLE_BINARY} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${EXAMPLE_BINARY} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${EXAMPLE_BINARY} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${EXAMPLE_BINARY} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB)




add_executable(${OTHER_SERVICE} other_main.cpp)
target_include_directories(${OTHER_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${OTHER_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${OTHER_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${OTHER_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${OTHER_SERVICE} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB)


#### Мои серивисы ###################################################################

#Сервис групповых чатов
add_executable(${GC_SERVICE} group_chat_main.cpp 
                             config/config.cpp 
                             database/database.cpp
                             database/list_users.cpp
                             database/group_chat.cpp
                             database/message.cpp)


target_include_directories(${GC_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${GC_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${GC_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${GC_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${GC_SERVICE} PRIVATE 
                         ${CMAKE_THREAD_LIBS_INIT} 
                         ${Poco_LIBRARIES}
                         "PocoData"
                         "PocoDataMySQL"
                         "mysqlclient"
                         ZLIB::ZLIB)

#Сервис P2P
add_executable(${P2P_SERVICE} p2p_main.cpp 
                             config/config.cpp 
                             database/database.cpp
                             database/p2p.cpp)


target_include_directories(${P2P_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${P2P_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${P2P_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${P2P_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${P2P_SERVICE} PRIVATE 
                         ${CMAKE_THREAD_LIBS_INIT} 
                         ${Poco_LIBRARIES}
                         "PocoData"
                         "PocoDataMySQL"
                         "mysqlclient"
                         ZLIB::ZLIB)



# add_executable(${MESSAGE_SERVICE} message_main.cpp 
#                              config/config.cpp 
#                              database/database.cpp
#                              database/message.cpp)


# target_include_directories(${MESSAGE_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
# target_compile_options(${MESSAGE_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
# set_target_properties(${MESSAGE_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
# set_target_properties(${MESSAGE_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

# target_link_libraries(${MESSAGE_SERVICE} PRIVATE 
#                          ${CMAKE_THREAD_LIBS_INIT} 
#                          ${Poco_LIBRARIES}
#                          "PocoData"
#                          "PocoDataMySQL"
#                          "mysqlclient"
#                          ZLIB::ZLIB)





