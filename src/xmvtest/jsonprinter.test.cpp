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
    pr.writeNumber(12);
    pr.endProperty();
    pr.endObject();

    EXPECT_EQ(stream.str(), "{\"foo\":\"bar\",\"asdf\":12}");
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
