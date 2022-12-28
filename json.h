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
class JsonObject;
typedef JsonObject Json;
typedef std::vector<JsonObject> JsonArray;
typedef std::map<std::string, JsonObject> JsonDict;

std::string getIndent(int indent);
std::string formatLine(const std::string& value, int indent, bool end);
std::string formatLine(const std::string& key, const std::string& value, int indent, bool end);

std::ostream& operator << (std::ostream& o, JsonArray& a);
std::ostream& operator << (std::ostream& o, JsonDict& d);

// Values
enum ValueType
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
class Value
{
public:
    /// <summary>
    /// Format the current value to a string.
    /// </summary>
    /// <returns>The string-formatted value.</returns>
    virtual std::string format() = 0;
};
typedef Value value_t;

class NullValue
    : public value_t
{
    void* m_value = nullptr;
public:
    NullValue() { };
    std::string format();
    std::ostream& operator << (std::ostream& o)
    {
        return o << format();
    }
};

class BoolValue
    : public value_t
{
    bool m_value;
public:
    BoolValue(bool value)
        : m_value(value) { };
    BoolValue(const BoolValue& other)
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

class IntValue
    : public value_t
{
    int m_value;
public:
    IntValue(int value)
        : m_value(value) { };
    IntValue(const IntValue& other)
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

class DoubleValue
    : public value_t
{
    double m_value;
public:
    DoubleValue(double value)
        : m_value(value) { };
    DoubleValue(const DoubleValue& other)
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

class StringValue
    : public value_t
{
    std::string m_value;
public:
    StringValue(std::string value)
        : m_value(value) { };
    StringValue(const StringValue& other)
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

class ArrayValue
    : public value_t
{
    JsonArray m_value;
public:
    ArrayValue(const JsonArray value);
    ArrayValue(const ArrayValue& other)
    {
        *this = other;
    }
    JsonArray value();
    std::string format();

    const ArrayValue& operator = (const ArrayValue& other)
    {
        m_value = other.m_value;
        return *this;
    }
    JsonObject& operator [] (const int index)
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
    friend std::ostream& operator << (std::ostream& o, ArrayValue& a);
};

class DictValue
    : public value_t
{
    JsonDict m_value;
public:
    DictValue(const JsonDict value);
    DictValue(const DictValue& other)
    {
        *this = other;
    }
    JsonDict value();
    std::string format();

    const DictValue& operator = (const DictValue& other)
    {
        m_value = other.m_value;
        return *this;
    }
    JsonObject& operator [] (const std::string key)
    {
        return m_value[key];
    }
    friend std::ostream& operator << (std::ostream& o, DictValue& d);
};

/// <summary>
/// Base JSON object. Contains a wrapper for each possible value type, with constructors and accessors for each.
/// </summary>
class JsonObject
{
    std::unique_ptr<value_t> m_value;
    ValueType m_type;

public:
    // Constructors
    JsonObject();                         // Default
    JsonObject(const JsonObject& other);        // Copy
    JsonObject(bool value);               // Bool
    JsonObject(int value);                // Integer
    JsonObject(double value);             // Double
    JsonObject(const std::string& value); // String
    JsonObject(const JsonArray& value);     // Array
    JsonObject(const JsonDict& value);      // Dictionary

    // Destructor
    ~JsonObject() { };

    // Methods
    ValueType type() { return m_type; }

    BoolValue& asBool() const;
    IntValue& asInt() const;
    DoubleValue& asDouble() const;
    StringValue& asString() const;
    ArrayValue& asArray() const;
    DictValue& asDict() const;

    bool getBool() const;
    int getInt() const;
    double getDouble() const;
    std::string getString() const;
    JsonArray getArray() const;
    JsonDict getDict() const;

    std::string format() const;

    // Operators
    const JsonObject& operator = (const JsonObject& other);
    JsonObject& operator [] (const std::string& key);
    JsonObject& operator [] (int index);
    friend std::ostream& operator << (std::ostream& o, JsonObject& j);
    friend std::ostream& operator << (std::ostream& o, const JsonObject& j);
};

JSON_NAMESPACE_CLOSE

#endif
