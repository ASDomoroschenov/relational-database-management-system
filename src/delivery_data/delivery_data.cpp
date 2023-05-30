#include "delivery_data.h"

std::shared_ptr<string_flyweight> delivery_data::get_description() const
{
    return _description;
}

void delivery_data::set_description(
        const std::string &description)
{
    string_flyweight_factory &factory = string_flyweight_factory::get_instance();

    _description = factory.get_string_flyweight(description);
}


std::shared_ptr<string_flyweight> delivery_data::get_surname() const
{
    return _surname;
}

void delivery_data::set_surname(
        const std::string &surname)
{
    string_flyweight_factory &factory = string_flyweight_factory::get_instance();

    _surname = factory.get_string_flyweight(surname);
}


std::shared_ptr<string_flyweight> delivery_data::get_name() const
{
    return _name;
}

void delivery_data::set_name(
        const std::string &name)
{
    string_flyweight_factory &factory = string_flyweight_factory::get_instance();

    _name = factory.get_string_flyweight(name);
}


std::shared_ptr<string_flyweight> delivery_data::get_patronymic() const
{
    return _patronymic;
}

void delivery_data::set_patronymic(
        const std::string &patronymic)
{
    string_flyweight_factory &factory = string_flyweight_factory::get_instance();

    _patronymic = factory.get_string_flyweight(patronymic);
}


std::shared_ptr<string_flyweight> delivery_data::get_mail() const
{
    return _mail;
}

void delivery_data::set_mail(
        const std::string &mail)
{
    string_flyweight_factory &factory = string_flyweight_factory::get_instance();

    _mail = factory.get_string_flyweight(mail);
}


std::shared_ptr<string_flyweight> delivery_data::get_phone() const
{
    return _phone;
}

void delivery_data::set_phone(
        const std::string &phone)
{
    string_flyweight_factory &factory = string_flyweight_factory::get_instance();

    _phone = factory.get_string_flyweight(phone);
}


std::shared_ptr<string_flyweight> delivery_data::get_user_comment() const
{
    return _user_comment;
}

void delivery_data::set_user_comment(
        const std::string &user_comment)
{
    string_flyweight_factory &factory = string_flyweight_factory::get_instance();

    _user_comment = factory.get_string_flyweight(user_comment);
}


std::shared_ptr<string_flyweight> delivery_data::get_date_time_delivery() const
{
    return _date_time_delivery;
}

void delivery_data::set_date_time_delivery(
        const std::string &date_time_delivery)
{
    string_flyweight_factory &factory = string_flyweight_factory::get_instance();

    _date_time_delivery = factory.get_string_flyweight(date_time_delivery);
}