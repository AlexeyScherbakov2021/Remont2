#include "testbase.h"

#include <QtTest>

// add necessary includes here

class RemTest : public QObject
{
    Q_OBJECT

public:
    RemTest();
    ~RemTest();

private:
    TestBase tstBase;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void TestURAItem();
};

RemTest::RemTest()
{
}

RemTest::~RemTest() {}

void RemTest::initTestCase() {}

void RemTest::cleanupTestCase() {}

void RemTest::TestURAItem()
{
    tstBase.TestURAItem();
}

QTEST_APPLESS_MAIN(RemTest)

#include "tst_remtest.moc"
