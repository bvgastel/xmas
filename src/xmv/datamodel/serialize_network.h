#ifndef SERIALIZE_NETWORK
#define SERIALIZE_NETWORK

#include <ostream>
#include <set>

class XMASComponent;

void serialize_network(std::ostream& os, std::set<XMASComponent*>& c);

class JsonPrinter
{
public:
    JsonPrinter(std::ostream&);
    JsonPrinter(std::ostream*);
    ~JsonPrinter();

    int getIndent() const                       { return indent; }
    void setIndent(int value)                   { indent = value; }

    void startObject();
    void endObject();

    void startArray();
    void endArray();

    void startProperty(const std::string& name);
    void endProperty();

    void writeNumber(const std::string& value);
    void writeString(const std::string& value);

    void writeNumberProperty(const std::string& name, const std::string& value);
    void writeStringProperty(const std::string& name, const std::string& value);

private:
    std::ostream*   stream;
    bool            ownStream;
    bool            firstItem = true;
    int             indent = 2;
    int             depth = 0;

    const std::string do_indent() const {
        return std::string(depth * indent, ' ');
    }
};

#endif // SERIALIZE_NETWORK

