#ifndef JSON_PRINTER
#define JSON_PRINTER

#include <ostream>
#include <stack>

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

    void writeNumber(int value);
    void writeString(const std::string& value);
    void writeBool(bool value);
    void writeNull();

    void writeNumberProperty(const std::string& name, int value);
    void writeStringProperty(const std::string& name, const std::string& value);
    void writeBoolProperty(const std::string& name, bool value);
    void writeNullProperty(const std::string& name);

    class InvalidStateException
    {
    };

private:
    std::ostream*   stream;
    bool            ownStream;

    bool            firstProperty;
    bool            firstElement;

    enum class State {
        Init,
        InObject,
        InProperty,
        InArray
    };
    std::stack<State>       states;

};

#endif // JSON_PRINTER

