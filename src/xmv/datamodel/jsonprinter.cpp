#include "jsonprinter.h"


JsonPrinter::JsonPrinter(std::ostream& stream)
  : stream(&stream), ownStream(false)
{
    states.push(State::Init);
}

JsonPrinter::JsonPrinter(std::ostream* stream)
  : stream(stream), ownStream(true)
{
    states.push(State::Init);
}

JsonPrinter::~JsonPrinter()
{
    if (ownStream)
        delete(stream);
}

void JsonPrinter::startObject()  {
    *stream << '{';
    states.push(State::InObject);
}

void JsonPrinter::endObject() {
    *stream << '}';
}

void JsonPrinter::startProperty(const std::string &name) {
    if (states.top() != State::InObject)
        throw JsonPrinter::InvalidStateException {};
    *stream << '"' << name << '"' << ':';
}

void JsonPrinter::endProperty() {

}

void JsonPrinter::writeNumber(int value) {
    *stream << std::to_string(value);
}

void JsonPrinter::writeString(const std::string& value) {
    *stream << '"' << value << '"';
}
