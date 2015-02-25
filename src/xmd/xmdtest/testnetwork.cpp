#include <gtest/gtest.h>

#include <sstream>

#include "xmapper/network.h"
#include "xmapper/controller.h"
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

        m_controller = new Controller();

        m_invalid_json << "This is invalid json. ()";
        m_valid_json_invalid_network << "\"invalid\" { }";

        m_valid_network << "{ \"VARS\" :[], \"NETWORK\" : [] }";
        m_network1 = new Network(m_controller, m_valid_network);
    }

    virtual void TearDown() {
        delete m_network1;
    }

public:
    std::stringstream m_invalid_json;
    std::stringstream m_valid_json_invalid_network;
    std::stringstream m_valid_network;
    Controller *m_controller;
    Network *m_network1;


};

TEST_F(NetworkTest, creationTest) {
    Network *n;
    Q_UNUSED(n) //to avoid warning n not used. n is not seen by compiler in EXPECT_THROW
    QUrl dummyUrl = QUrl("dummy filename");
    EXPECT_THROW(n = new Network(m_controller, dummyUrl), XmdException);
    EXPECT_THROW(n = new Network(m_controller, m_invalid_json), XmdException);
    EXPECT_EQ(m_network1->size(), size_t(0)); // avoid signed vs unsigned warning in gtest --> size_t(0) used
    bitpowder::lib::unused(n);
}


} // namespace
