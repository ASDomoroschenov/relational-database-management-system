#ifndef DELIVERY_DATA_H
#define DELIVERY_DATA_H

#include "flyweight.h"

class delivery_data
{

private:

    std::shared_ptr<string_flyweight> _description;
    std::shared_ptr<string_flyweight> _surname;
    std::shared_ptr<string_flyweight> _name;
    std::shared_ptr<string_flyweight> _patronymic;
    std::shared_ptr<string_flyweight> _mail;
    std::shared_ptr<string_flyweight> _phone;
    std::shared_ptr<string_flyweight> _user_comment;
    std::shared_ptr<string_flyweight> _date_time_delivery;

public:

    std::shared_ptr<string_flyweight> get_description() const;

    void set_description(
            const std::string &description);


    std::shared_ptr<string_flyweight> get_surname() const;

    void set_surname(
            const std::string &surname);


    std::shared_ptr<string_flyweight> get_name() const;

    void set_name(
            const std::string &name);


    std::shared_ptr<string_flyweight> get_patronymic() const;

    void set_patronymic(
            const std::string &patronymic);


    std::shared_ptr<string_flyweight> get_mail() const;

    void set_mail(
            const std::string &mail);


    std::shared_ptr<string_flyweight> get_phone() const;

    void set_phone(
            const std::string &phone);


    std::shared_ptr<string_flyweight> get_user_comment() const;

    void set_user_comment(
            const std::string &user_comment);


    std::shared_ptr<string_flyweight> get_date_time_delivery() const;

    void set_date_time_delivery(
            const std::string &date_time_delivery);
};

#endif