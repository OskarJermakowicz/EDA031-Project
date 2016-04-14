#ifndef EDA031_PROJECT_STORAGE_H
#define EDA031_PROJECT_STORAGE_H

#include <vector>
#include <string>

struct article {
	std::string title;
    std::string author;
    std::string text;
    article(const std::string &t, const std::string &a, const std::string &txt) : title(t), author(a), text(txt) {}
};


class storage {
public:

    virtual ~storage() = default;

    virtual std::vector <std::pair<int, std::string>> list_ng() const = 0;

    virtual void create_ng(const std::string &ng) = 0;

    virtual void delete_ng(int ng) = 0;

    virtual std::vector <std::pair<int, std::string>> list_art(int ng) const = 0;

    virtual void create_art(int ng, const std::string &title, const std::string &author, const std::string &text) = 0;

    virtual void delete_art(int ng, int art) = 0;

    virtual article get_art(int ng, int art) const = 0;

};

#endif //EDA031_PROJECT_STORAGE_H
