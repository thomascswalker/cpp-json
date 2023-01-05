#ifndef JSON_H
#define JSON_H

#define DEBUG_TYPE false

#define JSON_NAMESPACE_OPEN namespace JSON {
#define JSON_NAMESPACE_CLOSE }
#define JSON_NAMESPACE_USING using namespace JSON;

#define IS_NUMBER(x)    (((x - 48) | (57 - x)) >= 0 || x == 46)
#define IS_QUOTE(x)     x == 34
#define IS_NOT_QUOTE(x) x != 34
#define IS_COMMA(x)     x == 44
#define IS_LBRACE(x)    x == 91
#define IS_RBRACE(x)    x == 93
#define IS_LBRACKET(x)  x == 123
#define IS_RBRACKET(x)  x == 125
#define IS_COLON(x)     x == 58

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

/// <summary>
/// Load the given file as a JsonObject.
/// </summary>
/// <param name="filename">The full .json filename.</param>
/// <returns>The JsonObject equivalent of the contents of the file.</returns>
JsonObject loadFile(std::string filename);

/// <summary>
/// Load the given string as a JsonObject.
/// </summary>
/// <param name="string">The formatted JSON string.</param>
/// <returns>The JsonObject equivalent of the JSON string.</returns>
JsonObject loadString(std::string string);

std::ostream& operator << (std::ostream& o, JsonArray& a);
std::ostream& operator << (std::ostream& o, JsonDict& d);

/// <summary>
/// JSON value types, with numbers slightly modified for C++.
/// Number becomes both Int and Double.
/// </summary>
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
/// <summary>
/// Typedef alias for Value.
/// </summary>
typedef Value value_t;

/// <summary>
/// Null JSON value. Represents the 'null' keyword.
/// </summary>
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

/// <summary>
/// Boolean JSON value. Represents the 'true' and 'false' keywords.
/// </summary>
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

/// <summary>
/// Number JSON value. Represents numbers which have no decimals (integers).
/// </summary>
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

/// <summary>
/// Number JSON value. Represents numbers which have decimals.
/// </summary>
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

/// <summary>
/// String JSON value. Does not contain wrapped quotes, they are not necessary because
/// of the defined type.
/// </summary>
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

/// <summary>
/// Array JSON value. Contains a single array of [JsonObject, ...].
/// This is defined with the typedef JsonArray.
/// </summary>
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

/// <summary>
/// Array JSON value. Contains a single map of {{std::string, JsonObject}, ...}.
/// This is defined with the typedef JsonDict.
/// </summary>
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

    
    /// <summary>
    /// Returns the ValueType of this JsonObject.
    /// </summary>
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

    /// <summary>
    /// Formats this JsonObject as a std::string.
    /// </summary>
    std::string format() const;

    // Operators
    const JsonObject& operator = (const JsonObject& other);
    JsonObject& operator [] (const std::string& key);
    JsonObject& operator [] (int index);
    friend std::ostream& operator << (std::ostream& o, JsonObject& j);
    friend std::ostream& operator << (std::ostream& o, const JsonObject& j);
};

/// <summary>
/// Token struct for lexing. 
/// </summary>
struct Token
{
    enum TokenType
    {   
        Null,
        LBrace,
        RBrace,
        LBracket,
        RBracket,
        Colon,
        Comma,
        Number,
        String,
        Boolean
    };

    TokenType type    = TokenType::Null;
    std::string value = "";
};

/// <summary>
/// Lexer for tokenizing the given input string. By default, the input string will be sanitized
/// (removing whitespace, new lines, returns, end of file, etc.)
/// </summary>
class Lexer
{
    std::string m_string;
    int m_offset = 0;

    /// <summary>
    /// Sanitizes the given input string. This will remove new lines, end of lines, and whitespace outside
    /// of strings.
    /// </summary>
    /// <param name="input">The input string to sanitize.</param>
    /// <returns>The sanitized string.</returns>
    std::string sanitize(std::string& input)
    {
        std::string output;

        // Whether we're inside a string or not at the current character
        bool inString = false;

        // For every character in the input string...
        for (char& c : input)
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
        // Sanitize input string
        m_string = sanitize(string);

        // While we are able to continue, keep going to the next token
        while (canContinue())
        {
            // Get next token
            Token t = next();

            // TODO: Figure out why type is sometimes invalid.
            // This is a catch-all for invalid enum values.
            if (t.type < 0)
            {
                continue;
            }

            // Add to token array.
            tokens.push_back(t);
        }
    };

    /// <summary>
    /// Determines if we can continue tokenization if the current character position is not at the end
    /// of the sanitized string.
    /// </summary>
    bool canContinue()
    {
        return m_offset < m_string.size();
    }

    /// <summary>
    /// Determines the next token and adds it to the array of tokens. This will increment `m_offset` by
    /// however long the token is determined to be.
    /// </summary>
    /// <returns>The token which is constructed.</returns>
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
            return Token(Token::Number, number);
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
            return Token(Token::String, string);
        }

        // Booleans
        if (m_string.substr(m_offset, 4) == "true")
        {
            m_offset += 4;
            return Token(Token::Boolean, "true");
        }
        if (m_string.substr(m_offset, 5) == "false")
        {
            m_offset += 5;
            return Token(Token::Boolean, "false");
        }

        // Null
        if (m_string.substr(m_offset, 4) == "null")
        {
            m_offset += 4;
            return Token(Token::Null);
        }

        // Separators
        if (IS_COMMA(m_string[m_offset]))
        {
            m_offset++;
            return Token(Token::Comma);
        }

        if (IS_LBRACE(m_string[m_offset]))
        {
            m_offset++;
            return Token(Token::LBrace);
        }

        if (IS_RBRACE(m_string[m_offset]))
        {
            m_offset++;
            return Token(Token::RBrace);
        }

        if (IS_LBRACKET(m_string[m_offset]))
        {
            m_offset++;
            return Token(Token::LBracket);
        }

        if (IS_RBRACKET(m_string[m_offset]))
        {
            m_offset++;
            return Token(Token::RBracket);
        }

        if (IS_COLON(m_string[m_offset]))
        {
            m_offset++;
            return Token(Token::Colon);
        }

        // In all other instances, we have a malformed JSON file. Throw an error.
        std::string msg("Invalid character " + m_string[m_offset]);
        throw std::runtime_error(msg);
    }
};

/// <summary>
/// Parser which ingests a Lexer (essentially a list of tokens) and builds an Abstract Syntax Tree (AST) from it.
/// The final output of this AST is a JsonObject itself.
/// </summary>
class Parser
{
    // The lexer which contains the tokens to parse.
    Lexer* m_lexer;

    // The output JsonObject.
    JsonObject m_json;

    // The current token pointer.
    Token* current;

    // The current token position.
    int pos = 0;

    /// <summary>
    /// Iterate to the next token as well as bump the position by 1.
    /// </summary>
    void next()
    {
        current++;
        pos++;
    }

    /// <summary>
    /// Determines if we can continue if the current position is not at the end of the token list.
    /// </summary>
    bool canContinue()
    {
        return pos < m_lexer->tokens.size();
    }

    /// <summary>
    /// Recursively parse the current token. Assuming the JSON string is valid and begins with either
    /// a dictionary or an array, the first token will begin the recursion.
    /// </summary>
    /// <returns></returns>
    JsonObject parse()
    {
        // Null
        switch (current->type)
        {
            case (Token::Null):
            {
                next(); // Go to next token
                return JsonObject();
            }

            // Booleans
            case (Token::Boolean):
            {
                std::string value = current->value;
                next(); // Go to next token
                return (value == "true" ? JsonObject(true) : JsonObject(false));
            }

            // Numbers
            case (Token::Number):
            {
                std::string value = current->value;
                next(); // Go to next token
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
            case (Token::String):
            {
                std::string value = current->value;
                next(); // Go to next token
                return JsonObject(value);
            }

            // Arrays
            case (Token::LBrace):
            {
                next();  // Skip start brace
                JsonArray array;
                while (current->type != Token::RBrace)
                {
                    // Skip commas
                    if (current->type == Token::Comma)
                    {
                        next(); // Go to next token
                        continue;
                    }
                    JsonObject value = parse(); // Recursively parse value

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
            case (Token::LBracket):
            {
                next();  // Skip start bracket
                JsonDict dict;

                while (current->type != Token::RBracket &&
                       current->type > 0)
                {
                    // Parse key
                    if (current->type != Token::String)
                    {
                        throw std::runtime_error("Expected string key");
                    }
                    std::string key = current->value;
                    next(); // Move from key to expected colon

                    // Parse value
                    if (current->type != Token::Colon)
                    {
                        throw std::runtime_error("Expected colon");
                    }
                    next(); // Move from colon to expected value

                    // Construct dict obj
                    JsonObject value = parse(); // Recursively parse value
                    dict[key] = value;

                    // If there's a comma, skip it
                    if (current->type == Token::Comma)
                    {
                        next();
                        continue;
                    }
                    // If we're at the end of the dictionary, break the loop
                    if (current->type == Token::RBracket)
                    {
                        break;
                    }
                }

                next(); // Skip end bracket
                return JsonObject(dict);
            }

            default:
            {
                throw std::runtime_error("Unable to parse!");
            }
        }
    }

public:
    Parser(Lexer* lexer)
        : m_lexer(lexer)
    {
        current = &m_lexer->tokens[0];
        m_json = parse();
    };

    /// <summary>
    /// Returns the JsonObject which was parsed from the file (or string).
    /// </summary>
    JsonObject& get() { return m_json; }
};

////////////////////
// IMPLEMENTATION //
////////////////////

std::string getIndent(int indent = 0)
{
    auto size = indent * 4;
    return std::string(size, ' ');
}

std::string formatLine(const std::string& value, int indent, bool end)
{
    std::string line;
    line += getIndent(indent);
    line += value;
    if (!end)
    {
        line += ",";
    }
    line += "\n";
    return line;
}

std::string formatLine(const std::string& key, const std::string& value, int indent, bool end)
{
    std::string line;
    line += getIndent(indent);
    line += "\"" + key + "\"" + ": " + value;
    if (!end)
    {
        line += ",";
    }
    line += "\n";
    return line;
}

// General operators
std::ostream& operator << (std::ostream& o, JsonArray& a)
{
    return o << JsonObject(a).format();
}

std::ostream& operator << (std::ostream& o, JsonDict& d)
{
    return o << JsonObject(d).format();
}

// Null
std::string NullValue::format()
{
    return "Null";
}

// Bool
bool BoolValue::value()
{
    return m_value;
}

std::string BoolValue::format()
{
    std::string boolString(m_value == true ? std::string("true") : std::string("false"));
#if DEBUG_TYPE == true
    boolString += " (bool)";
#endif
    return boolString;
}

// Int
int IntValue::value()
{
    return m_value;
}

std::string IntValue::format()
{
    std::string intString = std::to_string(m_value);
#if DEBUG_TYPE == true
    intString += " (int)";
#endif
    return intString;
}

// Double
double DoubleValue::value()
{
    return m_value;
}

std::string DoubleValue::format()
{
    std::string doubleString = std::to_string(m_value);
#if DEBUG_TYPE == true
    doubleString += " (double)";
#endif
    return doubleString;
}

// String
std::string StringValue::value()
{
    return m_value;
}

std::string StringValue::format()
{

    std::string string = "\"" + m_value + "\"";
#if DEBUG_TYPE == true
    string += " (string)";
#endif
    return string;
}

// Array
ArrayValue::ArrayValue(const JsonArray value)
{
    m_value.clear();
    for (const JsonObject& v : value)
    {
        m_value.emplace_back(v);
    }
};

JsonArray ArrayValue::value()
{
    return m_value;
}

std::string ArrayValue::format()
{
    std::string arrayString = "[\n";
    CURRENT_INDENT++;
    int count = 1;
    for (auto& v : m_value)
    {
        bool at_end = (count == m_value.size());
        arrayString += formatLine(v.format(), CURRENT_INDENT, at_end);
        count++;
    }
    CURRENT_INDENT--;
    arrayString += getIndent(CURRENT_INDENT) + "]";
    return arrayString;
}

// Dictionary
DictValue::DictValue(const JsonDict value)
{
    for (const auto& [k, v] : value)
    {
        m_value[k] = v;
    }
};

JsonDict DictValue::value()
{
    return m_value;
}

std::string DictValue::format()
{
    std::string dictString = "{\n";
    CURRENT_INDENT++;
    int count = 1;
    for (auto& [k, v] : m_value)
    {
        bool at_end = (count == m_value.size());
        std::string new_line = (v.type() == Dictionary || v.type() == Array) ? ("\n" + getIndent(CURRENT_INDENT)) : "";
        dictString += formatLine(k, new_line + v.format(), CURRENT_INDENT, at_end);
        count++;
    }
    CURRENT_INDENT--;
    dictString += getIndent(CURRENT_INDENT) + "}";
    return dictString;
}

// JSON Object
JsonObject::JsonObject()
{
    m_value = nullptr;
    m_type = Null;
}
JsonObject::JsonObject(const JsonObject& other)
{
    *this = other;
}
JsonObject::JsonObject(bool value)
{
    m_value = std::make_unique<BoolValue>(value);
    m_type = Bool;
}
JsonObject::JsonObject(int value)
{
    m_value = std::make_unique<IntValue>(value);
    m_type = Int;
}
JsonObject::JsonObject(double value)
{
    m_value = std::make_unique<DoubleValue>(value);
    m_type = Double;
}
JsonObject::JsonObject(const std::string& value)
{
    m_value = std::make_unique<StringValue>(value);
    m_type = String;
}
JsonObject::JsonObject(const JsonArray& value)
{

    m_value = std::make_unique<ArrayValue>(value);
    m_type = Array;
}
JsonObject::JsonObject(const JsonDict& value)
{
    m_value = std::make_unique<DictValue>(value);
    m_type = Dictionary;
}

BoolValue& JsonObject::asBool() const
{
    return *static_cast<BoolValue*>(m_value.get());
}

IntValue& JsonObject::asInt() const
{
    return *static_cast<IntValue*>(m_value.get());
}

DoubleValue& JsonObject::asDouble() const
{
    return *static_cast<DoubleValue*>(m_value.get());
}

StringValue& JsonObject::asString() const
{
    return *static_cast<StringValue*>(m_value.get());
}

ArrayValue& JsonObject::asArray() const
{
    return *static_cast<ArrayValue*>(m_value.get());
}

DictValue& JsonObject::asDict() const
{
    return *static_cast<DictValue*>(m_value.get());
}

bool JsonObject::getBool() const
{
    return asBool().value();
}

int JsonObject::getInt() const
{
    return asInt().value();
}

double JsonObject::getDouble() const
{
    return asDouble().value();
}

std::string JsonObject::getString() const
{
    return asString().value();
}

JsonArray JsonObject::getArray() const
{
    return asArray().value();
}

JsonDict JsonObject::getDict() const
{
    return asDict().value();
}

std::string JsonObject::format() const
{
    if (m_value.get() == nullptr)
    {
        return "NULL";
    }

    return m_value.get()->format();
}

const JsonObject& JsonObject::operator = (const JsonObject& other)
{
    switch (other.m_type)
    {
        case (Bool):
        {
            m_value = std::make_unique<BoolValue>(other.getBool());
            break;
        }
        case (Int):
        {
            m_value = std::make_unique<IntValue>(other.getInt());
            break;
        }
        case (Double):
        {
            m_value = std::make_unique<DoubleValue>(other.getDouble());
            break;
        }
        case (String):
        {
            m_value = std::make_unique<StringValue>(other.getString());
            break;
        }
        case (Array):
        {
            m_value = std::make_unique<ArrayValue>(other.getArray());
            break;
        }
        case (Dictionary):
        {
            m_value = std::make_unique<DictValue>(other.getDict());
            break;
        }
        case (Null):
            break;
    }
    m_type = other.m_type;
    return *this;
}

JsonObject& JsonObject::operator[](const std::string& key)
{
    if (m_type != Dictionary && m_value == nullptr)
    {
        throw std::runtime_error("Invalid type, wanted Dictionary");
    }
    return asDict()[key];
}

JsonObject& JsonObject::operator[](int index)
{
    std::cout << m_type << std::endl;
    if (m_type != Array)
    {
        throw std::runtime_error("Invalid type, wanted Array");
    }
    return asArray()[index];
}

std::ostream& operator<<(std::ostream& o, ArrayValue& a)
{
    return o << a.format();
}

std::ostream& operator << (std::ostream& o, DictValue& d)
{
    return o << d.format();
}

std::ostream& operator << (std::ostream& o, JsonObject& j)
{
    return o << j.format();
}

std::ostream& operator << (std::ostream& o, const JsonObject& j)
{
    return o << j.format();
}

JsonObject loadFile(std::string filename)
{
    // Read file contents
    std::ifstream file(filename);    //taking file as inputstream
    std::string data;
    if (file)
    {
        std::ostringstream stream;      // New stream
        stream << file.rdbuf();         // Reading data
        data = stream.str();            // Put stream to data string
    }
    else
    {
        throw std::runtime_error("File not found: " + filename);
    }

    // Tokenize string
    Lexer* l = new Lexer(data);

    // Parse string into JSON object
    Parser* p = new Parser(l);
    return p->get();
}

JsonObject loadString(std::string string)
{
    // Tokenize string
    Lexer* l = new Lexer(string);

    // Parse string into JSON object
    Parser* p = new Parser(l);
    return p->get();
}

JSON_NAMESPACE_CLOSE

#endif