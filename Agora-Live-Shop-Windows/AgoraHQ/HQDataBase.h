#include "IHQDataBase.h"
#include <string>
class CppSQLite3DB;
class CHQDataBase : public IHQDataBase
{
public:
	CHQDataBase();
	~CHQDataBase();
	//product advertisement
	virtual bool sqlGetProductAdvertise(int index, CProductAdvertise* pAdvertise) override;
	virtual int sqlGetProductAdvertiseCount() override;
	virtual bool sqlInsertProductAdvertise() override;
	virtual bool sqlDeleteProductAdvertise() override;
	virtual bool sqlUpdateProductAdvertise() override;
	
	//questions
	virtual bool sqlGetHQQuestionById(int index, CQuestions* pQuestion) override;
	virtual bool sqlInsertHQQuestion() override;
	virtual bool sqlDeleteHQQuestion() override;
	virtual bool sqlUpdateHQQuestion() override;
	virtual int sqlGetHQQuestionCount() override;
private:
	CppSQLite3DB* m_pSqliteDB;
	std::string sqliteDbPath;

	void _startup();
	void _closeDB();
	bool _createQuestionTable();
	bool _createProductAdTable();
};
