#pragma once
#include<iostream>
#include<string>
#include"sqlite3/sqlite3.h"
#include"non_copyable.hpp"
using namespace std;

class SmartDB:NonCopyable
{
public:
    SmartDB(){}
    explicit SmartDB(const string& filename);
    ~SmartDB();
    int GetLastErrorCode()
    {
        return m_code;
    }

private:
    bool Open(const string& filename);
    bool Close();
    int CloseDBHandle();

private:
    sqlite3* m_dbHandle;
    sqlite3_stmt* m_statement;
    int m_code;
};
