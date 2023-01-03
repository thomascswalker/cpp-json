#ifndef JSON_H
#define JSON_H

#define DEBUG_TYPE true

#define JSON_NAMESPACE_OPEN namespace JSON {
#define JSON_NAMESPACE_CLOSE }
#define JSON_NAMESPACE_USING using namespace JSON;

#define IS_NUMBER(x) (((x - 48) | (57 - x)) >= 0 || x == 46)
#define IS_QUOTE(x) x == 34
#define IS_NOT_QUOTE(x) x != 34
#define IS_COMMA(x) x == 44
#define IS_LBRACE(x) x == 91
#define IS_RBRACE(x) x == 93
#define IS_LBRACKET(x) x == 123
#define IS_RBRACKET(x) x == 125
#define IS_COLON(x) x == 58

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

JSON_NAMESPACE_OPEN

// Globals
static int CURRENT_INDENT = 0;

// Forward declaration
class JsonObject;
typedef JsonObject Json;
typedef std::vector<JsonObject> JsonArray;
typedef std::map<std::string, JsonObject> JsonDict;

struct Token;
class Lexer;
class Parser;

std::string getIndent(int indent);
std::string formatLine(const std::string& value, int indent, bool end);
std::string formatLine(const std::string& key, const std::string& value, int indent, bool end);
JsonObject& loadFile(std::string filename);

std::ostream& operator << (std::ostream& o, JsonArray& a);
std::ostream& operator << (std::ostream& o, JsonDict& d);

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
    JsonObject(const JsonObject& other);  // Copy
    JsonObject(bool value);               // Bool
    JsonObject(int value);                // Integer
    JsonObject(double value);             // Double
    JsonObject(const std::string& value); // String
    JsonObject(const JsonArray& value);   // Array
    JsonObject(const JsonDict& value);    // Dictionary

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

struct Token
{
    enum TokenType
    {   
        NULLVALUE,
        LBRACE,
        RBRACE,
        LBRACKET,
        RBRACKET,
        COLON,
        COMMA,
        NUMBER,
        STRING,
        BOOLEAN
    };

    static std::string getTypeString(TokenType t)
    {
        switch (t)
        {
            case (NULLVALUE): return "NULL";
            case (LBRACE): return "LBRACE";
            case (RBRACE): return "RBRACE";
            case (LBRACKET): return "LBRACKET";
            case (RBRACKET): return "RBRACKET";
            case (COLON): return "COLON";
            case (COMMA): return "COMMA";
            case (NUMBER): return "NUMBER";
            case (STRING): return "STRING";
            case (BOOLEAN): return "BOOLEAN";
        }
    }

    TokenType tokenType = TokenType::NULLVALUE;
    std::string value = "";
    int start = 0;
    int end = 0;
};

class Lexer
{
    std::string m_string;
    int m_offset = 0;
    Token m_lookAhead;

    std::string sanitize(std::string& input)
    {
        std::string output;
        bool inString = false;

        // For every character in the input string...
        for (auto& c : input)
        {
            // Remove new lines and end of lines
            if (c == '\n' || c == '\0')
            {
                continue;
            }

            // Remove spaces ONLY when outside of a string
            if (c == ' ' && inString == false)
            {
                continue;
            }

            // Flip-flop context of in or outside of a string
            if (c == '"')
            {
                inString = !inString;
            }

            // Append current character to sanitized output
            output += c;
        }

        return output;
    }

public:
    std::vector<Token> tokens;

    Lexer(std::string string)
    {
        m_string = sanitize(string);
        while (canContinue())
        {
            Token t = next();
            if (t.tokenType < 0)
            {
                continue;
            }
            tokens.push_back(t);
        }
    };

    bool canContinue()
    {
        return m_offset < m_string.size();
    }

    Token next()
    {
        int start = m_offset;

        // Numbers
        if (IS_NUMBER(m_string[m_offset]))
        {
            std::string number;
            while (IS_NUMBER(m_string[m_offset]))
            {
                number += m_string[m_offset];
                m_offset++;
            }
            return Token(Token::NUMBER, number, start, m_offset);
        }

        // Strings
        if (IS_QUOTE(m_string[m_offset]))
        {
            std::string string;

            // Skip entry quote
            m_offset++;

            // Build the string from contents inside the quotes
            while (IS_NOT_QUOTE(m_string[m_offset]))
            {
                string += m_string[m_offset];
                m_offset++;
            }

            // Skip exit quote
            m_offset++;
            return Token(Token::STRING, string, start, m_offset);
        }

        // Booleans
        if (m_string.substr(m_offset, 4) == "true")
        {
            m_offset += 4;
            return Token(Token::BOOLEAN, "true", start, m_offset);
        }
        if (m_string.substr(m_offset, 5) == "false")
        {
            m_offset += 5;
            return Token(Token::BOOLEAN, "false", start, m_offset);
        }

        // Separators
        if (IS_COMMA(m_string[m_offset]))
        {
            m_offset++;
            return Token(Token::COMMA, ",", start, m_offset);
        }

        if (IS_LBRACE(m_string[m_offset]))
        {
            m_offset++;
            return Token(Token::LBRACE, "[", start, m_offset);
        }

        if (IS_RBRACE(m_string[m_offset]))
        {
            m_offset++;
            return Token(Token::RBRACE, "]", start, m_offset);
        }

        if (IS_LBRACKET(m_string[m_offset]))
        {
            m_offset++;
            return Token(Token::LBRACKET, "{", start, m_offset);
        }

        if (IS_RBRACKET(m_string[m_offset]))
        {
            m_offset++;
            return Token(Token::RBRACKET, "}", start, m_offset);
        }

        if (IS_COLON(m_string[m_offset]))
        {
            m_offset++;
            return Token(Token::COLON, ":", start, m_offset);
        }

        std::string msg("Invalid character " + m_string[m_offset]);
        throw std::runtime_error(msg);
    }
};

class Parser
{
    Lexer* m_lexer;
    JsonObject m_json;

    Token* current;
    int pos = 0;

    void next()
    {
        current++;
        pos++;
        //std::cout << current->value << std::endl;
    }

    bool canContinue()
    {
        return pos < m_lexer->tokens.size();
    }

public:
    Parser(Lexer* lexer)
        : m_lexer(lexer)
    {
        current = &m_lexer->tokens[0];
        m_json = parse();
    };

    JsonObject parse()
    {
        // Null
        if (current->tokenType == Token::NULLVALUE)
        {
            return JsonObject();
        }

        // Numbers
        if (current->tokenType == Token::NUMBER)
        {
            std::string value = current->value;
            next();
            // Decimal values
            if (value.find(".") != std::string::npos)
            {
                return JsonObject(std::stod(value));
            }
            // Integer values
            else
            {
                return JsonObject(std::stoi(value));
            }
        }

        // Strings
        if (current->tokenType == Token::STRING)
        {
            std::string value = current->value;
            next();
            return JsonObject(value);
        }

        // Booleans
        if (current->tokenType == Token::BOOLEAN)
        {
            std::string value = current->value;
            next();
            return (value == "true" ? JsonObject(true) : JsonObject(false));
        }

        // Arrays
        if (current->tokenType == Token::LBRACE)
        {
            next();  // Skip start brace
            JsonArray array;
            while (current->tokenType != Token::RBRACE)
            {
                // Skip commas
                if (current->tokenType == Token::COMMA)
                {
                    next();
                    continue;
                }
                JsonObject value = parse(); // Parse the value at this position

                // TODO: Figure out why this is needed, otherwise it breaks
                if (value.type() == Null)
                {
                    break;
                }

                // Add to our array the value we parsed
                array.push_back(value);
            }

            next(); // Skip end brace
            return JsonObject(array);
        }

        // Dictionaries
        if (current->tokenType == Token::LBRACKET)
        {
            next();  // Skip start bracket
            JsonDict dict;

            while (current->tokenType != Token::RBRACKET &&
                   current->tokenType > 0)
            {
                // Parse key
                if (current->tokenType != Token::STRING)
                {
                    throw std::runtime_error("Expected string key");
                }
                std::string key = current->value;
                next(); // Move from key to expected colon

                // Parse value
                if (current->tokenType != Token::COLON)
                {
                    throw std::runtime_error("Expected colon");
                }
                next(); // Move from colon to expected value

                // Construct dict obj
                JsonObject value = parse(); // Parse value
                dict[key] = value;

                // If there's a comma, skip it
                if (current->tokenType == Token::COMMA)
                {
                    next();
                    continue;
                }
                // If we're at the end of the dictionary, break the loop
                if (current->tokenType == Token::RBRACKET)
                {
                    break;
                }
            }

            next(); // Skip end bracket
            return JsonObject(dict);
        }

        throw std::runtime_error("Unable to parse!");
    }

    JsonObject& get() { return m_json; }
};

JSON_NAMESPACE_CLOSE

#endif
