#ifndef EDA031_PROJECT_STORAGE_H
#define EDA031_PROJECT_STORAGE_H

#include <vector>
#include <string>

using namespace std;

struct article {
    string author;
    string title;
    string text;
};


class storage {
public:

    virtual vector <pair<int, string>> list_ng() const = 0;

    virtual void create_ng(const string &ng) = 0;

    virtual void delete_ng(int ng) = 0;

    virtual vector <pair<int, string>> list_art(int ng) const = 0;

    virtual void create_art(int ng, const string &title, const string &author, const string &text) = 0;

    virtual void delete_art(int ng, int art) = 0;

    virtual article get_art(int ng, int art) const = 0;

};

#endif //EDA031_PROJECT_STORAGE_H
