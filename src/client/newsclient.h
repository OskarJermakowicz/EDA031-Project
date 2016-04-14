#ifndef EDA031_PROJECT_NEWCLIENT_H
#define EDA031_PROJECT_NEWCLIENT_H

#include "../library/connection.h"
#include <regex>

class NewsClient {

public:
    NewsClient(const char *addr, int port);

    void run();

private:
    const string help_text = "Available newsgroup commands:\nlist\t\t\t\t\tLists newsgroups\ncreate <name>\t\t\t\tCreates a newsgroup\ndelete <ID>\t\t\t\tDeletes newsgroup with ID number <ID>\n\nAvailable article commands:\nlist <ID>\t\t\t\tLists articles in newsgroup with ID number <ID>\ncreate <ID> <title> <author> <body>\tCreates a new article in newsgroup with ID number <ID>\ndelete <ID1> <ID2>\t\t\tDeletes article with ID number <ID2> in newsgroup with ID number <ID1>\nget<ID1> <ID2>\t\t\t\tGets article with ID number <ID2> in newsgroup with ID number <ID1>";
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

    std::vector <std::string> parse_line(std::string line);

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
