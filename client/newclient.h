#ifndef EDA031_PROJECT_NEWCLIENT_H
#define EDA031_PROJECT_NEWCLIENT_H

#include "../library/connection.h"
#include <regex>

class NewsClient {

public:
    NewsClient(const char *addr, int port);

    void run();

private:
    const string help_text = "Available commands: (parameters within <> are optional and parameters within '' are mandatory)\nlist <number>\t\tLists newsgroups, or articles of newsgroup with id number\ncreate 'name'\t\tCreates a newsgroup with name\ncreate ''\ndelete\nget";
    const std::string INTEGER = "(0|[1-9][0-9]*)";
    const std::string TEXT = "(?:([^\\s\"]+)|\"([^\"]*)\")";
    const std::string WHITESPACE = "(?:\\s+)";

    const std::string HELP = "(help)";
    const std::string LIST = "(list)(?:" + WHITESPACE + INTEGER + ")?";
    const std::string CREATE =
            "(create)" + WHITESPACE + "(?:" + TEXT + "|" + INTEGER + WHITESPACE + TEXT + WHITESPACE + TEXT +
            WHITESPACE + TEXT + ")";
    const std::string DELETE = "(delete)" + WHITESPACE + INTEGER + "(?:" + WHITESPACE + INTEGER + ")?";
    const std::string GET = "(get)" + WHITESPACE + INTEGER + WHITESPACE + INTEGER;

    const std::string LINE =
            "^" + WHITESPACE + "?(?:" + LIST + "|" + CREATE + "|" + DELETE + "|" + GET + "|" + HELP + ")" + WHITESPACE +
            "?$";


    std::shared_ptr <Connection> conn;

    void handle_command(std::string line);

    void list_ng();

    void create_ng(std::string ng);

    void delete_ng(int ng);

    void list_art(int ng);

    void create_art(int ng, std::string title, std::string author, std::string text);

    void delete_art(int ng, int art);

    void get_art(int ng, int art);

};

#endif //EDA031_PROJECT_NEWCLIENT_H
