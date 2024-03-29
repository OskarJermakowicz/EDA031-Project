#ifndef EDA031_PROJECT_NEWSSERVER_H
#define EDA031_PROJECT_NEWSSERVER_H

#include "../library/server.h"
#include "storage.h"


class NewsServer : public Server {

public:
    NewsServer(int port);

    void run();

private:

    std::unique_ptr <storage> db;

    void handle_request(const std::shared_ptr <Connection> &conn);

    void list_ng(const std::shared_ptr <Connection> &conn);

    void create_ng(const std::shared_ptr <Connection> &conn);

    void delete_ng(const std::shared_ptr <Connection> &conn);

    void list_art(const std::shared_ptr <Connection> &conn);

    void create_art(const std::shared_ptr <Connection> &conn);

    void delete_art(const std::shared_ptr <Connection> &conn);

    void get_art(const std::shared_ptr <Connection> &conn);
};


#endif //EDA031_PROJECT_NEWSSERVER_H
