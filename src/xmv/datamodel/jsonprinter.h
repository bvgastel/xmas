#ifndef JSON_PRINTER
#define JSON_PRINTER

#include <ostream>
#include <stack>
#include <type_traits>

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

struct JsonNull {
};
constexpr JsonNull jsonnull;

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

#endif // JSON_PRINTER

