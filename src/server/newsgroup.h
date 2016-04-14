#ifndef EDA031_PROJECT_NEWSGROUP_H
#define EDA031_PROJECT_NEWSGROUP_H

#include <vector>
#include "storage.h"
#include <map>
#include <set>


class Newsgroup {
public:
    Newsgroup(const std::string &new_name) : art_id(0), name(new_name) {}
    std::string get_name() const {return name;}
    std::vector<std::pair<int, std::string>> list_art() const;
    void create_art(const std::string &title, const std::string &author, const std::string &text);
    void delete_art(const int &ng);
    article get_art(const int &ng) const;
private:
    int art_id;
    std::string name;
    std::map<int, article> art_map;
};

#endif //EDA031_PROJECT_INMEM_STORAGE_H
