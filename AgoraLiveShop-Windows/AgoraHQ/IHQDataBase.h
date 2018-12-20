#pragma once

class CProductAdvertise;
class CQuestions;
class IHQDataBase
{
public:
	//产品广告
	virtual bool sqlGetProductAdvertise(int index, CProductAdvertise* pAdvertise) = 0;
	virtual int sqlGetProductAdvertiseCount() = 0;
	virtual bool sqlInsertProductAdvertise() = 0;
	virtual bool sqlDeleteProductAdvertise() = 0;
	virtual bool sqlUpdateProductAdvertise() = 0;
	//问题
	virtual bool sqlGetHQQuestionById(int index, CQuestions* pQuestion) = 0;
	virtual bool sqlInsertHQQuestion() = 0;
	virtual bool sqlDeleteHQQuestion() = 0;
	virtual bool sqlUpdateHQQuestion() = 0;
	virtual int sqlGetHQQuestionCount() = 0;

};

IHQDataBase* getHQDataBase();