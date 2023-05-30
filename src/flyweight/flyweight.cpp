#include "flyweight.h"

const std::string &string_flyweight::get_value() const
{
    return _value;
}

void string_flyweight::set_value(
        const std::string &value)
{
    _value = value;
}

string_flyweight_factory &string_flyweight_factory::get_instance()
{
    static string_flyweight_factory instance;

    return instance;
}

std::shared_ptr<string_flyweight> string_flyweight_factory::get_string_flyweight(
        const std::string &value)
{
    auto it = _string_flyweights.find(value);

    if (it != _string_flyweights.end())
    {
        return it->second;
    }
    else
    {
        auto flyweight = std::make_shared<string_flyweight>();
        flyweight->set_value(value);
        _string_flyweights[value] = flyweight;

        return flyweight;
    }
}