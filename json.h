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
    bool value();
    std::string format();
};

class int_value
    : public value_t
{
    int m_value;
public:
    int_value(int value)
        : m_value(value) { };
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
    std::string value();
    std::string format();
};

class array_value
    : public value_t
{
    std::vector<json> m_value;
public:
    array_value(const std::vector<json>& value);
    std::vector<json> value();
    std::string format();
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
    json();
    json(bool value);
    json(int value);
    json(double value);
    json(const std::string& value);
    json(const std::vector<json>& value);
    ~json() { };

    // Methods
    value_type type() { return m_type; }

    bool get_bool();
    int get_int();
    double get_double();
    std::string get_string();
    std::string format();

    // Operators
    friend std::ostream& operator << (std::ostream& o, json& v);
};

std::ostream& operator << (std::ostream& o, json& j)
{
    return o << j.format();
}

#endif