#include "stdafx.h"
#include "HQDataBase.h"
#include "sqlite3/CppSQLite3.h"
#include "ProductAdvertise.h"
#include "Questions.h"

// questions
const std::string createQuestionsTableSql =
    "CREATE TABLE IF NOT EXISTS questionsTable"
    "("
	"    [id]            INTEGER NOT NULL PRIMARY KEY,"
	"    [content]       TEXT NOT NULL,"
	"    [OptionA]       TEXT NOT NULL,"
	"    [OptionB]       TEXT NOT NULL,"
	"    [OptionC]       TEXT NOT NULL,"
	"    [OptionD]       TEXT NOT NULL,"
	"    [timeout]       INTEGER,"
	"    [total]         INTEGER,"
	"    [result]        INTEGER"
    ");";

const std::string selectQuestionsCount =
    "SELECT * FROM questionsTable";

const std::string selectQuestionsById =
    "SELECT * FROM questionsTable where id=?";
const std::string selectTotlOptions =
    "SELECT total FROM questionsTable where id=?";
// advertise
const std::string createProductAdvertise =
    "CREATE TABLE IF NOT EXISTS productAdvertiseTable"
    "("
	"    [id]            INTEGER NOT NULL PRIMARY KEY,"
	"    [productID]     VARCHAR,"
	"    [title]         TEXT NOT NULL,"
	"    [Color]         VARCHAR NOT NULL,"
	"    [Sold]          INTEGER NOT NULL,"
	"    [Price]         numeric(18, 2),"
	"    [Currency]      VARCHAR NOT NULL,"
	"    [GroupDeal]     numeric(18, 2),"
	"    [reserve1]      VARCHAR DEFAULT NULL,"
	"    [reserve2]      VARCHAR DEFAULT NULL,"
	"    [reserve3]      VARCHAR DEFAULT NULL"
    ");";

const std::string selectProductAdvertiseCount =
    "SELECT * FROM productAdvertiseTable";

const std::string selectProductAdById =
    "SELECT * FROM productAdvertiseTable where id=?";

IHQDataBase* getHQDataBase()
{
	static CHQDataBase hqDB;
	return &hqDB;
}

CHQDataBase::CHQDataBase()
	: m_pSqliteDB(new CppSQLite3DB())
{
	_startup();
}

CHQDataBase::~CHQDataBase()
{
	_closeDB();
	delete m_pSqliteDB;
	m_pSqliteDB = NULL;
}

void CHQDataBase::_startup()
{
	try{
		TCHAR szPath[MAX_PATH] = { 0 };
		char path[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szPath, MAX_PATH);
		CString strPath = szPath;
		strPath.Replace(_T(".exe"), _T(".db"));
		WideCharToMultiByte(CP_ACP, 0, strPath.GetBuffer(0), strPath.GetLength(), path, MAX_PATH, NULL, NULL);
		m_pSqliteDB->open(path);
		_createQuestionTable();
		_createProductAdTable();
		sqliteDbPath = path;
	}
	catch (CppSQLite3Exception& sqliteException){
		std::string err_msg = sqliteException.errorMessage();
	}
	catch (...){

	}
}

void CHQDataBase::_closeDB()
{
	try{
		m_pSqliteDB->close();
	}
	catch (CppSQLite3Exception& sqliteException){
		std::string err_msg = sqliteException.errorMessage();
	}
	catch (...){

	}
}
//
bool CHQDataBase::_createQuestionTable()
{
	try{
		m_pSqliteDB->execDML(createQuestionsTableSql.c_str());
	}
	catch (CppSQLite3Exception& sqliteException){
		
	}
	catch (...){
		std::string err_msg = "error";
	}

	return true;
}


bool CHQDataBase::_createProductAdTable()
{
	try{
		m_pSqliteDB->execDML(createProductAdvertise.c_str());
	}
	catch (CppSQLite3Exception& sqliteException){
		
	}
	catch (...){

	}

	return true;
}

CString utf8toCString(const char* utf8)
{
	if (utf8 != NULL)
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
		std::wstring strW;

		strW.resize(len);

		MultiByteToWideChar(CP_UTF8, 0, utf8, -1, (LPWSTR)strW.data(), len);

		len = WideCharToMultiByte(936, 0, strW.data(), len - 1, NULL, 0, NULL, NULL);
		return strW.c_str();
	}
	return _T("");
}

//product advertisement
int CHQDataBase::sqlGetProductAdvertiseCount()
{
	CppSQLite3Table table = m_pSqliteDB->getTable(selectProductAdvertiseCount.c_str());
	return table.numRows();
}

void parseProductColumn(std::vector<std::string>& vecProp, std::string strColumn)
{
	int pos = std::string::npos;
	while ((pos = strColumn.find(",")) != std::string::npos){
		vecProp.push_back(strColumn.substr(0, pos));
		strColumn = strColumn.substr(pos + 1);
	}
	if (strColumn.length() > 0){
		vecProp.push_back(strColumn);
	}
}

bool CHQDataBase::sqlGetProductAdvertise(int index, CProductAdvertise* pAdvertise)
{
	CppSQLite3Statement stmt = m_pSqliteDB->compileStatement(selectProductAdById.c_str());
	int i = 1;
	stmt.bind(i, index + 1);

	CppSQLite3Query query = stmt.execQuery();
	bool bFind = false;
	if (!query.eof()){
		pAdvertise->id = index + 1;
		pAdvertise->productId = query.getStringField(i++);
		pAdvertise->title     = query.getStringField(i++);

		std::string colors = query.getStringField(i++);
		parseProductColumn(pAdvertise->m_vecColors, colors);
	
		pAdvertise->sold      = query.getInt64Field(i++);
		pAdvertise->price     = query.getFloatField (i++);
		pAdvertise->currrency = query.getStringField(i++);
		pAdvertise->groupDeal = query.getFloatField(i++);
		bFind = true;
	}
	return bFind; 
}

bool CHQDataBase::sqlInsertProductAdvertise()
{
	return true; 
}
bool CHQDataBase::sqlDeleteProductAdvertise(){ return true; }
bool CHQDataBase::sqlUpdateProductAdvertise(){ return true; }

//questions
bool CHQDataBase::sqlGetHQQuestionById(int index, CQuestions* pQuestion)
{ 
	CppSQLite3Statement stmt = m_pSqliteDB->compileStatement(selectQuestionsById.c_str());
	int i = 1;
	stmt.bind(i, index);
	CppSQLite3Query query = stmt.execQuery();
	if (!query.eof()){
		//int i = 2;
		pQuestion->id = index;
		pQuestion->content = query.getStringField(i++);
		pQuestion->cs_content = utf8toCString(pQuestion->content.c_str());
		pQuestion->total_option = query.getInt64Field(i + 5);
		pQuestion->timeout = query.getIntField(i + 4);
		int result = query.getIntField(i + 6);
		if (pQuestion->total_option > 4)
			pQuestion->total_option = 4;
		for (int j = 0; j < pQuestion->total_option; j++){
			pQuestion->m_vecOptions.push_back(query.getStringField(i++));
			pQuestion->m_vecCSOptions.push_back(utf8toCString(pQuestion->m_vecOptions[j].c_str()));
		}

		pQuestion->total_option = pQuestion->total_option;
		pQuestion->result = result % 4;
		
		for (size_t i = 0; i < pQuestion->total_option; i++){
			pQuestion->m_vecSpread.push_back(rand() % 100);
		}
		
	}
	
	return true; 
}

bool CHQDataBase::sqlInsertHQQuestion(){ return true; }
bool CHQDataBase::sqlDeleteHQQuestion(){ return true; }
bool CHQDataBase::sqlUpdateHQQuestion(){ return true; }

int CHQDataBase::sqlGetHQQuestionCount()
{
	CppSQLite3Table table = m_pSqliteDB->getTable(selectQuestionsCount.c_str());
	return table.numRows();
}
