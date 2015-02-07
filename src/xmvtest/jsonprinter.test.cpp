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

    EXPECT_EQ(stream.str(), "{}");
}

TEST_F(JsonPrinterTest, SingleProperty) {
    pr.startObject();
    pr.startProperty("test-property");
    pr.writeString("test-value");
    pr.endProperty();
    pr.endObject();

    EXPECT_EQ(stream.str(), "{\"test-property\":\"test-value\"}");
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

    EXPECT_EQ(stream.str(), "{\"foo\":\"bar\",\"asdf\":0.12}");
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

    EXPECT_EQ(stream.str(), R"({"test-a":12,"foo":{"bar":"nested object!"},"test-b":-23})");
}


TEST_F(JsonPrinterTest, EmptyArray) {
    pr.startArray();
    pr.endArray();

    EXPECT_EQ(stream.str(), "[]");
}

TEST_F(JsonPrinterTest, SingleElement) {
    pr.startArray();
    pr.writeBool(true);
    pr.endArray();

    EXPECT_EQ(stream.str(), "[true]");
}

TEST_F(JsonPrinterTest, MultipleElements) {
    pr.startArray();
    pr.writeNumber(42);
    pr.writeString("JSON!");
    pr.writeBool(false);
    pr.writeNull();
    pr.endArray();

    EXPECT_EQ(stream.str(), R"([42,"JSON!",false,null])");
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

    EXPECT_EQ(stream.str(), R"({"array":[null,true,1.00e+12,{},[]]})");
}

TEST_F(JsonPrinterTest, ArrayOfObjects) {
    pr.startArray();
    pr.startObject();
    pr.endObject();
    pr.startObject();
    pr.endObject();
    pr.endArray();

    EXPECT_EQ(stream.str(), R"([{},{}])");
}

TEST_F(JsonPrinterTest, ArrayOfArrays) {
    pr.startArray();
    pr.startArray();
    pr.endArray();
    pr.startArray();
    pr.endArray();
    pr.endArray();

    EXPECT_EQ(stream.str(), R"([[],[]])");
}
