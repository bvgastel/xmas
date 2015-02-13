#include "jsonprinter.h"

#include <string>
#include "gtest/gtest.h"

class JsonPrinterTest : public ::testing::Test
{
protected:
    std::stringstream   stream;
    JsonPrinter         pr;

    JsonPrinterTest() : pr(stream)
    {

    }
};

TEST_F(JsonPrinterTest, EmptyObject) {
    pr.startObject();
    pr.endObject();

    EXPECT_EQ("{}", stream.str());
}

TEST_F(JsonPrinterTest, SingleProperty) {
    pr.startObject();
    pr.startProperty("test-property");
    pr.writeString("test-value");
    pr.endProperty();
    pr.endObject();

    EXPECT_EQ("{\"test-property\":\"test-value\"}", stream.str());
}

TEST_F(JsonPrinterTest, MultipleProperties) {
    pr.startObject();
    pr.startProperty("foo");
    pr.writeString("bar");
    pr.endProperty();
    pr.startProperty("asdf");
    pr.writeNumber(0.12);
    pr.endProperty();
    pr.endObject();

    EXPECT_EQ("{\"foo\":\"bar\",\"asdf\":0.12}", stream.str());
}

TEST_F(JsonPrinterTest, PropertyOutsideObject) {

    ASSERT_THROW({
         pr.startProperty("test");
         pr.writeString("qwerty");
         pr.endProperty();
     }, JsonPrinter::InvalidStateException);
}

TEST_F(JsonPrinterTest, MissingPropertyValue) {
    ASSERT_THROW({
        pr.startObject();
        pr.startProperty("foo");
        pr.endObject();
    }, JsonPrinter::InvalidStateException);
}

TEST_F(JsonPrinterTest, BadPropertyValue) {
    ASSERT_THROW({
        pr.startObject();
        pr.startProperty("foo");
        pr.startProperty("bar");
        pr.endObject();
    }, JsonPrinter::InvalidStateException);
}

TEST_F(JsonPrinterTest, PlainValueInObject) {
    ASSERT_THROW({
        pr.startObject();
        pr.writeNumber(100);
        pr.endObject();
    }, JsonPrinter::InvalidStateException);
}

TEST_F(JsonPrinterTest, PropertyInArray) {
    ASSERT_THROW({
        pr.startArray();
        pr.writeNumberProperty("bad-property", 100);
        pr.endArray();
    }, JsonPrinter::InvalidStateException);
}

TEST_F(JsonPrinterTest, NestedObjects) {
    pr.startObject();
    pr.writeNumberProperty("test-a", 12);
    pr.startProperty("foo");
    pr.startObject();
    pr.writeStringProperty("bar", "nested object!");
    pr.endObject();
    pr.endProperty();
    pr.writeNumberProperty("test-b", -23);
    pr.endObject();

    EXPECT_EQ(R"({"test-a":12,"foo":{"bar":"nested object!"},"test-b":-23})", stream.str());
}

TEST_F(JsonPrinterTest, StreamingNestedObjects) {
    pr << json_startobj << jsonprop("test-a", 12) << jsonprop("foo", json_startobj) <<
          jsonprop("bar", "nested object!") << json_endobj << json_endprop <<
          jsonprop("test-b", -23) << json_endobj;

    EXPECT_EQ(R"({"test-a":12,"foo":{"bar":"nested object!"},"test-b":-23})", stream.str());
}


TEST_F(JsonPrinterTest, EmptyArray) {
    pr.startArray();
    pr.endArray();

    EXPECT_EQ("[]", stream.str());
}

TEST_F(JsonPrinterTest, SingleElement) {
    pr.startArray();
    pr.writeBool(true);
    pr.endArray();

    EXPECT_EQ("[true]", stream.str());
}

TEST_F(JsonPrinterTest, MultipleElements) {
    pr.startArray();
    pr.writeNumber(42);
    pr.writeString("JSON!");
    pr.writeBool(false);
    pr.writeNull();
    pr.endArray();

    EXPECT_EQ(R"([42,"JSON!",false,null])", stream.str());
}

TEST_F(JsonPrinterTest, ArrayInObject) {
    stream.precision(2);
    stream << std::scientific;

    pr.startObject();
    pr.startProperty("array");
    pr.startArray();
    pr.writeNull();
    pr.writeBool(true);
    pr.writeNumber(1e12);

    pr.startObject();
    pr.endObject();

    pr.startArray();
    pr.endArray();

    pr.endArray();
    pr.endProperty();
    pr.endObject();

    EXPECT_EQ(R"({"array":[null,true,1.00e+12,{},[]]})", stream.str());
}

TEST_F(JsonPrinterTest, StreamingArrayInObject) {
    stream.precision(2);
    stream << std::scientific;

    pr << json_startobj << jsonprop("array", json_startarray) <<
          jsonnull << true << 1e12 << json_startobj << json_endobj <<
          json_startarray << json_endarray << json_endarray <<
          json_endprop << json_endobj;


    EXPECT_EQ(R"({"array":[null,true,1.00e+12,{},[]]})", stream.str());
}

TEST_F(JsonPrinterTest, ArrayOfObjects) {
    pr.startArray();
    pr.startObject();
    pr.endObject();
    pr.startObject();
    pr.endObject();
    pr.endArray();

    EXPECT_EQ(R"([{},{}])", stream.str());
}

TEST_F(JsonPrinterTest, ArrayOfArrays) {
    pr.startArray();
    pr.startArray();
    pr.endArray();
    pr.startArray();
    pr.endArray();
    pr.endArray();

    EXPECT_EQ(R"([[],[]])", stream.str());
}

TEST_F(JsonPrinterTest, StringEncoding) {
    pr.startArray();
    pr.writeString("According to the JSON spec \"quotation marks\" and \\reverse solidi\\ must be escaped!");
    pr.writeString("multiple\nline\nstring\n");
    pr.endArray();

    EXPECT_EQ(R"(["According to the JSON spec \"quotation marks\" and \\reverse solidi\\ must be escaped!",)"
               R"("multiple\nline\nstring\n"])", stream.str());
}

TEST_F(JsonPrinterTest, StreamingObject) {
    pr << json_startobj << jsonprop("num", -5.55) << jsonprop("str", "<< streaming operator <<") <<
          jsonprop("test", true) << jsonprop("nil", jsonnull) << json_endobj;

    EXPECT_EQ(R"({"num":-5.55,"str":"<< streaming operator <<","test":true,"nil":null})", stream.str());
}

TEST_F(JsonPrinterTest, StreamingArray) {
    pr << json_startarray << -5.55 << "<< streaming operator <<" << true << "" << 'x' << jsonnull << json_endarray;

    EXPECT_EQ(R"([-5.55,"<< streaming operator <<",true,"","x",null])", stream.str());
}

TEST_F(JsonPrinterTest, StreamingPlainValueInObject) {
    ASSERT_THROW({
        pr << json_startobj << 100 << json_endobj;
    }, JsonPrinter::InvalidStateException);
}

TEST_F(JsonPrinterTest, StreamingPropertyInArray) {
    ASSERT_THROW({
        pr << json_startarray << jsonprop("bad-property", 100) << json_endarray;
    }, JsonPrinter::InvalidStateException);
}

TEST_F(JsonPrinterTest, PartialString) {
    pr << json_startarray << json_startstr;
    pr << "This" <<  " string is streamed in " << "using multiple parts";
    pr << json_endstr << json_endarray;

    EXPECT_EQ(R"(["This string is streamed in using multiple parts"])", stream.str());
}

TEST_F(JsonPrinterTest, PartialStringEncode) {
    pr << json_startarray << json_startstr;
    pr << "This" << '\n' <<  "string is \"encoded\" and\nstreamed in " << "using multiple parts";
    pr << json_endstr << json_endarray;

    EXPECT_EQ(R"(["This\nstring is \"encoded\" and\nstreamed in using multiple parts"])", stream.str());
}
