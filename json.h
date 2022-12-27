#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "inspection.h"

// Forward declaration
class json;
typedef json json_t;

// Values
enum value_type
{
    Null,           // nullptr
    Bool,           // true, false
    Int,            // 1, 2, 3
    Double,         // 3.14, 7.62, 50.50
    String,         // "This is a string."
    Array,          // { 1, 2, 3, 4, 5 }
    Dictionary      // { {"Key 1", 5}, {"Key 2", 10} }
};

/// <summary>
/// Base class for all JSON value types.
/// </summary>
class base_value
{
public:
    /// <summary>
    /// Format the current value to a string.
    /// </summary>
    /// <returns>The string-formatted value.</returns>
    virtual std::string format() = 0;
};
typedef base_value value_t;

class null_value
    : public value_t
{
    void* m_value = nullptr;
public:
    null_value() { };
    std::string format();
};

class bool_value
    : public value_t
{
    bool m_value;
public:
    bool_value(bool value)
        : m_value(value) { };
    bool_value(const bool_value& other)
    {
        std::cout << other.m_value << std::endl;
        *this = other;
    }
    bool value();
    std::string format();

    const bool_value& operator = (const bool_value& other)
    {
        std::cout << other.m_value << std::endl;
        m_value = other.m_value;
        return *this;
    }
};

class int_value
    : public value_t
{
    int m_value;
public:
    int_value(int value)
        : m_value(value) { };
    int_value(const int_value& other)
    {
        m_value = other.m_value;
    }
    int value();
    std::string format();
};

class double_value
    : public value_t
{
    double m_value;
public:
    double_value(double value)
        : m_value(value) { };
    double_value(const double_value& other)
    {
        m_value = other.m_value;
    }
    double value();
    std::string format();
};

class string_value
    : public value_t
{
    std::string m_value;
public:
    string_value(std::string value)
        : m_value(value) { };
    string_value(const string_value& other)
    {
        m_value = other.m_value;
    }
    std::string value();
    std::string format();
};

class array_value
    : public value_t
{
    std::vector<json> m_value;
public:
    array_value(const std::vector<json> value);
    array_value(const array_value& other)
    {
        *this = other;
    }
    std::vector<json> value();
    std::string format();

    const array_value& operator = (const array_value& other)
    {
        m_value = other.m_value;
    }
};

/// <summary>
/// Base JSON object. Contains a wrapper for each possible value type, with constructors and accessors for each.
/// </summary>
class json
{
    std::unique_ptr<value_t> m_value;
    value_type m_type;

public:
    // Constructors
    json();                                 // Default
    json(const json& other);                // Copy
    json(bool value);                       // Bool
    json(int value);                        // Integer
    json(double value);                     // Double
    json(const std::string& value);         // String
    json(const std::vector<json>& value);   // Array

    // Destructor
    ~json() { };

    // Methods
    value_type type() { return m_type; }

    bool get_bool() const;
    int get_int() const;
    double get_double() const;
    std::string get_string() const;
    std::vector<json> get_array() const;
    std::string format();

    // Operators
    const json& operator = (const json& other)
    {
        switch (other.m_type)
        {
            case (Bool):
            {
                //std::cout << &*this << " = " << other.get_bool() << std::endl;
                m_value = std::make_unique<bool_value>(other.get_bool());
                //std::cout << static_cast<bool_value*>(m_value.get())->value() << std::endl;
                break;
            }
        }
        m_type = other.m_type;
        return *this;
    }
    friend std::ostream& operator << (std::ostream& o, json& j)
    {
        return o << j.format();
    }
};

#endif