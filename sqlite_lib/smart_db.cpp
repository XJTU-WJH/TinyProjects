#include"smart_db.hpp"

explicit SmartDB::SmartDB(const string& filename):m_dbHandle(nullptr),m_statement(nullptr)
{
    if(!Open(filename))
    {
        cout<<"build database failed! "<<filename<<"doesn't exists!"<<endl;
        exit(1);
    }
}

SmartDB::~SmartDB()
{
    if(!Close())
    {
        cout<<"Close SmartDB failed!"<<endl;
        exit(1);
    }
}

/*****************************基础接口*****************************/
bool SmartDB::Open(const string& filename)
{
    m_code=sqlite3_open(filename.data(),&m_dbHandle);
    return SQLITE_OK==m_code;
}

bool SmartDB::Close()
{
    if(m_dbHandle==nullptr)
        return true;
    sqlite3_finalize(m_statement);
    m_code=CloseDBHandle();
    m_dbHandle=nullptr;
    m_dbHandle=nullptr;
    return SQLITE_OK==m_code;
}

int SmartDB::CloseDBHandle()
{
    int code=sqlite3_close(m_dbHandle);
    while(code==SQLITE_BUSY)
    {
        code=SQLITE_OK;
        sqlite3_stmt* stmt=sqlite3_next_stmt(m_dbHandle,NULL);
        if(stmt==nullptr)
            break;
        code=sqlite3_finalize(stmt);
        if(code==SQLITE_OK)
        {
            code=sqlite3_close(m_dbHandle);
        }
    }
    return code;
}
/*****************************执行接口*****************************/
static int code;
int BindParams(sqlite3_stmt* statement,int current)
{
    return SQLITE_OK;
}

template<typename T,typename... Args>
int BindParams(sqlite3_stmt* statement,int current,T&& first,Args&&... args)
{
    BindValue(statement,current,first);
    if(code!=SQLITE_OK)
        return code;
    BindParams(statement,current+1,forward<Args>(args)...);
    return code;
}

template<typename T>
typename enable_if<is_floating_point<T>::value>::type
BindValue(sqlite3_stmt* statement,int current,T t)
{
    code=sqlite3_bind_double(statement,current,t);
}

template<typename T>
typename enable_if<is_integral<T>::value>::type
BindValue(sqlite3_stmt* statement,int current,T t)
{
    code=sqlite3_bind_int(statement,current,t);
}

template<typename T>
typename enable_if<is_same<T,int64_t>::value||is_same<T,uint64_t>::value>::type
BindValue(sqlite3_stmt* statement,int current,T t)
{
    code=sqlite3_bind_int64(statement,current,t);
}

template<typename T>
typename enable_if<!is_same<T,int64_t>::value&&!is_same<T,uint64_t>::value>::type
BindValue(sqlite3_stmt* statement,int current,T t)
{
    code=sqlite3_bind_int(statement,current,t);
}

template<typename T>
typename enable_if<is_same<string,T>::value>::type
BindValue(sqlite3_stmt* statement,int current,const T& t)
{
    code=sqlite3_bind_text(statement,current,t.data(),t.length(),SQLITE_TRANSIENT);
}

template<typename T>
typename enable_if<is_same<T,char*>::value||is_same<T,const char*>::value>::type
BindValue(sqlite3_stmt* statement,int current,T t)
{
    code=sqlite3_bind_text(statement,current,t,strlen(t)+1,SQLITE_TRANSIENT);
}

template<typename T>
typename enable_if<is_same<blob,T>::value>::type
BindValue(sqlite3_stmt* statement,int current,const T& t)
{
    code=sqlite3_bind_blob(statement,current,,,SQLITE_TRANSIENT);
}

template<typename T>
typename enable_if<is_same<nullptr_t,T>::value>::type
BindValue(sqlite3_stmt* statement,int current,const T& t)
{
    code=sqlite3_bind_null(statement,current);
}
