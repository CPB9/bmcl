#include "bmcl/Logging.h"

#include <iostream>

#include <gtest/gtest.h>

static std::string buffer;
static bmcl::LogLevel lastLevel;

static void handler(bmcl::LogLevel level, const char* msg)
{
    lastLevel = level;
    buffer.append(msg);
}

class Logging : public ::testing::Test {
protected:
    void SetUp() override
    {
        bmcl::setLogHandler(handler);
        lastLevel = bmcl::LogLevel::None;
        buffer.clear();
    }

    void TearDown() override
    {
        bmcl::setDefaulLogHandler();
    }

    void expectLastLevel(bmcl::LogLevel level)
    {
        EXPECT_EQ(level, lastLevel);
    }

    void expectMsg(const char* msg)
    {
        EXPECT_EQ(msg, buffer);
    }
};

TEST_F(Logging, basic)
{
    BMCL_DEBUG() << "test1";
    expectLastLevel(bmcl::LogLevel::Debug);
    expectMsg("test1");
}

TEST_F(Logging, builtin)
{
    BMCL_WARNING() << "test" << 123 << ' ' << 321;
    expectLastLevel(bmcl::LogLevel::Warning);
    expectMsg("test123 321");
}

TEST_F(Logging, customLevel)
{
    bmcl::setLogLevel(bmcl::LogLevel::None);
    BMCL_CRITICAL() << "none";
    bmcl::setLogLevel(bmcl::LogLevel::Critical);
    BMCL_CRITICAL() << "critical";
    BMCL_DEBUG() << "none";
    bmcl::setLogLevel(bmcl::LogLevel::Debug);
    BMCL_DEBUG() << "debug";
    expectLastLevel(bmcl::LogLevel::Debug);
    expectMsg("criticaldebug");
}

#if BMCL_HAVE_QT

#include <QString>
#include <QByteArray>

TEST_F(Logging, qt)
{
    BMCL_INFO() << "test" << QString("asd") << QByteArray("321");
    expectLastLevel(bmcl::LogLevel::Info);
    expectMsg("testasd321");
}

#endif
