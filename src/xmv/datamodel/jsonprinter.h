#ifndef JSON_PRINTER
#define JSON_PRINTER

#include <ostream>
#include <stack>
#include <type_traits>

/**
 * @brief The JsonPrinter class
 *
 * Issues:
 * TODO: start/endproperty is annoying and unnecessary, when a property value
 * is completed, the property itself is also complete. Remove it and the InProperty state.
 *
 * TODO: Currently, only complete strings can be serialized. Maybe provide start/end string
 * functions just like object and array (and add an InString state). (DONE!)
 *
 * TODO: Provide both a 'function call'-style API and the streaming API?
 *
 * TODO: Use different names for all json types (writeNumber, writeString, etc.) or use a single
 * overloaded write function?
 *
 * TODO: Use std::string or const char* for property names and string values?
 */
class JsonPrinter
{
public:
    JsonPrinter(std::ostream&);
    JsonPrinter(std::ostream*);
    ~JsonPrinter();

    void startObject();
    void endObject();

    void startArray();
    void endArray();

    void startProperty(const std::string& name);
    void endProperty();

    void startString();
    void endString();

    template<typename T>
    typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type
    writeNumber(T value);

    void writeString(const std::string& value);
    void writeBool(bool value);
    void writeNull();

    template<typename T>
    typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type
    writeNumberProperty(const std::string& name, T value);

    void writeStringProperty(const std::string& name, const std::string& value);
    void writeBoolProperty(const std::string& name, bool value);
    void writeNullProperty(const std::string& name);

    class InvalidStateException
    {
    };

private:
    std::ostream*   stream;
    bool            ownStream;

    bool            firstItem   = true;     // to skip ',' on first property/element

    enum class State {
        Init,
        InObject,
        InProperty,
        InString,
        InArray
    };
    std::stack<State>       states;

    void encodeString(const std::string& str);

};

template<typename T>
typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type
JsonPrinter::writeNumber(T value) {
    if (states.top() != State::InProperty && states.top() != State::InArray)
        throw JsonPrinter::InvalidStateException {};

    if (states.top() == State::InArray && !firstItem)
        *stream << ',';
    else
        firstItem = false;

    *stream << value;
}

template<typename T>
typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type
JsonPrinter::writeNumberProperty(const std::string& name, T value) {
    startProperty(name);
    writeNumber(value);
    endProperty();
}


template<typename T>
struct JsonProperty {
    std::string name;
    T value;
};

struct JsonNull {};
struct JsonStartObject {};
struct JsonEndObject {};
struct JsonStartArray {};
struct JsonEndArray {};
struct JsonEndProperty {};
struct JsonStartString {};
struct JsonEndString {};

constexpr JsonNull jsonnull;
constexpr JsonStartObject json_startobj;
constexpr JsonEndObject json_endobj;
constexpr JsonStartArray json_startarray;
constexpr JsonEndArray json_endarray;
constexpr JsonEndProperty json_endprop;
constexpr JsonStartString json_startstr;
constexpr JsonEndString json_endstr;

template<typename T>
inline constexpr JsonProperty<T> jsonprop(const std::string& name, T value) {
    return JsonProperty<T> {name, value};
}

template<typename T>
inline typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value, JsonPrinter&>::type
operator <<(JsonPrinter& pr, JsonProperty<T> prop) {
    pr.startProperty(prop.name);
    pr.writeNumber(prop.value);
    pr.endProperty();
    return pr;
}

inline JsonPrinter& operator <<(JsonPrinter& pr, JsonProperty<const char*> prop) {
    pr.startProperty(prop.name);
    pr.writeString(prop.value);
    pr.endProperty();
    return pr;
}

inline JsonPrinter& operator <<(JsonPrinter& pr, JsonProperty<bool> prop) {
    pr.startProperty(prop.name);
    pr.writeBool(prop.value);
    pr.endProperty();
    return pr;
}

inline JsonPrinter& operator <<(JsonPrinter& pr, JsonProperty<JsonNull> prop) {
    pr.startProperty(prop.name);
    pr.writeNull();
    pr.endProperty();
    return pr;
}

inline JsonPrinter& operator <<(JsonPrinter& pr, JsonProperty<JsonStartArray> prop) {
    pr.startProperty(prop.name);
    pr.startArray();
    return pr;
}

inline JsonPrinter& operator <<(JsonPrinter& pr, JsonProperty<JsonStartObject> prop) {
    pr.startProperty(prop.name);
    pr.startObject();
    return pr;
}

// make sure chars are treated as a string instead of a number
inline JsonPrinter& operator <<(JsonPrinter& pr, char value) {
    char str[2] {value, 0};
    pr.writeString(str);
    return pr;
}

template<typename T>
inline typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value, JsonPrinter&>::type
operator <<(JsonPrinter& pr, T value) {
    pr.writeNumber(value);
    return pr;
}

inline JsonPrinter& operator <<(JsonPrinter& pr, const char* value) {
    pr.writeString(value);
    return pr;
}

inline JsonPrinter& operator <<(JsonPrinter& pr, bool value) {
    pr.writeBool(value);
    return pr;
}

inline JsonPrinter& operator <<(JsonPrinter& pr, JsonNull) {
    pr.writeNull();
    return pr;
}

inline JsonPrinter& operator <<(JsonPrinter& pr, JsonStartObject) {
    pr.startObject();
    return pr;
}

inline JsonPrinter& operator <<(JsonPrinter& pr, JsonEndObject) {
    pr.endObject();
    return pr;
}

inline JsonPrinter& operator <<(JsonPrinter& pr, JsonStartArray) {
    pr.startArray();
    return pr;
}

inline JsonPrinter& operator <<(JsonPrinter& pr, JsonEndArray) {
    pr.endArray();
    return pr;
}

inline JsonPrinter& operator <<(JsonPrinter& pr, JsonEndProperty) {
    pr.endProperty();
    return pr;
}

inline JsonPrinter& operator <<(JsonPrinter& pr, JsonStartString) {
    pr.startString();
    return pr;
}

inline JsonPrinter& operator <<(JsonPrinter& pr, JsonEndString) {
    pr.endString();
    return pr;
}

#endif // JSON_PRINTER

