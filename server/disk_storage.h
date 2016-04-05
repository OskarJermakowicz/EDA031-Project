#ifndef EDA031_PROJECT_DISK_STORAGE_H
#define EDA031_PROJECT_DISK_STORAGE_H

#define DATABASE_DIRECTORY "./database/"
#define DATABASE_FILENAME "database"
#define NEWSGROUP_FILENAME "newsgroup"
#define NEWSGROUP_PREFIX "newsgroup"
#define ARTICLE_PREFIX "article"

#include <string>
#include "storage.h"

/* 
 * Folder structure:
 * 
 * database/
 *  ├─ database         Contains the next newsgroup ID.
 *  ├─ newsgroup1/
 *  ├─ newsgroup2/
 *  ├─ newsgroup3/
 *  │   ├─ newsgroup    Contains the name of the newsgroup and the next article ID.
 *  │   ├─ article1     Contains article title, author and text.
 *  │   ├─ article2
 *  │   └─ article2
 *  └─ newsgroup4/
 */

class disk_storage : public storage {
private:
    static std::string get_newsgroup_directory(int ng);

    static std::string get_newsgroup_path(int ng);

    static std::string get_article_path(int ng, int art);

public:

    std::vector <std::pair<int, std::string>> list_ng() const;

    void create_ng(const string &ng);

    void delete_ng(int ng);

    std::vector <std::pair<int, std::string>> list_art(int ng) const;

    void create_art(int ng, const std::string &title, const std::string &author, const std::string &text);

    void delete_art(int ng, int art);

    article get_art(int ng, int art) const;

};


#endif //EDA031_PROJECT_DISK_STORAGE_H
