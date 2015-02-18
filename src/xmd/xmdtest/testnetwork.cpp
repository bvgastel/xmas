#include <gtest/gtest.h>

#include <sstream>

#include "network.h"
#include "xmdexception.h"

namespace {

class NetworkTest : public ::testing::Test
{
protected:
    NetworkTest() {
    }

    virtual ~NetworkTest() {
    }

    virtual void SetUp() {
        m_invalid_json << "This is invalid json. ()";
        m_valid_json_invalid_network << "\"invalid\" { }";

        m_valid_network << "\"NETWORK\" { }";
        m_network1 = new Network(m_valid_network);
    }

    virtual void TearDown() {
        delete m_network1;
    }

public:
    std::stringstream m_invalid_json;
    std::stringstream m_valid_json_invalid_network;
    std::stringstream m_valid_network;
    Network *m_network1;


};

TEST_F(NetworkTest, creationTest) {
    Network *n;
    EXPECT_THROW(n = new Network("dummy filename"), XmdException);
    EXPECT_THROW(n = new Network(m_invalid_json), XmdException);
    EXPECT_EQ(m_network1->size(), 0);
}


} // namespace
