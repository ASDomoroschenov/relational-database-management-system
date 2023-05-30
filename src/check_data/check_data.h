#ifndef CHECK_DATA_H
#define CHECK_DATA_H

#include <iostream>

bool check_id(
        const std::string &id);

bool check_description(
        const std::string &description);

bool check_name(
        const std::string &name);

bool check_surname(
        const std::string &surname);

bool check_patronymic(
        const std::string &patronymic);

bool check_mail(
        const std::string &mail);

bool check_phone(
        const std::string &phone);

bool check_user_comment(
        const std::string &user_comment);

bool check_date_time_delivery(
        const std::string &date_time_delivery);

#endif // CHECK_DATA_H