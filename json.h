#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "inspection.h"

#define JSON_NAMESPACE_OPEN namespace JSON {
#define JSON_NAMESPACE_CLOSE }
#define JSON_NAMESPACE_USING using namespace JSON;

JSON_NAMESPACE_OPEN

// Globals
static int CURRENT_INDENT = 0;

// Forward declaration
class json;
typedef json json_t;
typedef std::vector<json> array_t;
typedef std::map<std::string, json> dict_t;

std::string get_indent(int indent);
std::string format_line(const std::string& value, int indent, bool end);
std::string format_dict(const std::string& key, const std::string& value, int indent, bool end);

std::ostream& operator << (std::ostream& o, array_t& a);
std::ostream& operator << (std::ostream& o, dict_t& d);

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
    std::ostream& operator << (std::ostream& o)
    {
        return o << format();
    }
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
        *this = other;
    }
    bool value();
    std::string format();
    std::ostream& operator << (std::ostream& o)
    {
        return o << format();
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
        *this = other;
    }
    int value();
    std::string format();
    std::ostream& operator << (std::ostream& o)
    {
        return o << format();
    }
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
        *this = other;
    }
    double value();
    std::string format();
    std::ostream& operator << (std::ostream& o)
    {
        return o << format();
    }
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
        *this = other;
    }
    std::string value();
    std::string format();
    std::ostream& operator << (std::ostream& o)
    {
        return o << format();
    }
};

class array_value
    : public value_t
{
    array_t m_value;
public:
    array_value(const array_t value);
    array_value(const array_value& other)
    {
        *this = other;
    }
    array_t value();
    std::string format();

    const array_value& operator = (const array_value& other)
    {
        m_value = other.m_value;
        return *this;
    }
    json& operator [] (const int index)
    {
        if (index > m_value.size())
        {
            throw std::runtime_error("Index out of bounds.");
        }
        return m_value[index];
    }
    std::ostream& operator << (std::ostream& o)
    {
        return o << format();
    }
    friend std::ostream& operator << (std::ostream& o, array_value& a);
};

class dict_value
    : public value_t
{
    dict_t m_value;
public:
    dict_value(const dict_t value);
    dict_value(const dict_value& other)
    {
        *this = other;
    }
    dict_t value();
    std::string format();

    const dict_value& operator = (const dict_value& other)
    {
        m_value = other.m_value;
        return *this;
    }
    json& operator [] (const std::string key)
    {
        return m_value[key];
    }
    friend std::ostream& operator << (std::ostream& o, dict_value& d);
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
    json();                         // Default
    json(const json& other);        // Copy
    json(bool value);               // Bool
    json(int value);                // Integer
    json(double value);             // Double
    json(const std::string& value); // String
    json(const array_t& value);     // Array
    json(const dict_t& value);      // Dictionary

    // Destructor
    ~json() { };

    // Methods
    value_type type() { return m_type; }

    bool_value& as_bool() const;
    int_value& as_int() const;
    double_value& as_double() const;
    string_value& as_string() const;
    array_value& as_array() const;
    dict_value& as_dict() const;

    bool get_bool() const;
    int get_int() const;
    double get_double() const;
    std::string get_string() const;
    array_t get_array() const;
    dict_t get_dict() const;

    std::string format() const;

    // Operators
    const json& operator = (const json& other);
    json& operator [] (const std::string& key);
    json& operator [] (int index);
    friend std::ostream& operator << (std::ostream& o, json& j);
    friend std::ostream& operator << (std::ostream& o, const json& j);
};

JSON_NAMESPACE_CLOSE

#endif
