#ifndef TESTBASE_H
#define TESTBASE_H

#include <QtTest>
#include <repomssql.h>
// #include <models/Items.h>

class TestBase
{
public:
    TestBase();

    void TestURAItem();

private:
     RepoMSSQL repo;

};

#endif // TESTBASE_H
