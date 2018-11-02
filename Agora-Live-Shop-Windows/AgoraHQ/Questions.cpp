#include"stdafx.h"
#include "Questions.h"
#include "IHQDataBase.h"
#include "commonFun.h"
#include "rapidjson/prettywriter.h"  
#include "rapidjson/stringbuffer.h"  

CQuestions::CQuestions()
	: type("quiz")
	, id(id)
	, content("")
	, timeout(30)
	, total_option(4)
	, result(0)
{
	
}

CQuestionsModule* getQuestionsModule()
{
	unsigned int uid = 0;
	static CQuestionsModule questionModule;
	return &questionModule;
}

CQuestionsModule::CQuestionsModule()
{
	count = getHQDataBase()->sqlGetHQQuestionCount();
	if (count > 0){
		//_getFirstQuestion();
	}
}

void _generatRandSpread(std::vector<int>& vecSpread, int size)
{
	int max = int(USESRS_COUNT/(float)size);
	int sum = 0;
	for (size_t i = 0; i < size; i++){
		vecSpread[i] = rand() % max;
		sum += vecSpread[i];
	}
	vecSpread[size - 1] = USESRS_COUNT - sum;
}

int CQuestionsModule::_getFirstQuestion()
{
	cur_index = 0;
	CQuestions question;
	if (getHQDataBase()->sqlGetHQQuestionById(cur_index + 1, &question)){
		_generatRandSpread(question.m_vecSpread, question.total_option);
		question.no_answer = rand() % 30;
		m_vecQuestions.push_back(question);
	}
	return cur_index;
}

void CQuestionsModule::_getNextQuestion()
{
	CQuestions question;
	if (count > 0 )
		cur_index++;

	if (getHQDataBase()->sqlGetHQQuestionById(cur_index + 1, &question)){
		m_vecQuestions.push_back(question);
	}
}

int CQuestionsModule::getResult()
{
	if (count == 0)
		return -1;

	return m_vecQuestions[cur_index].result;
}
/*
bool CQuestionsModule::getSpread(std::vector<int>& )
{
	if (count == 0)
		return false;

}
*/

std::string CQuestionsModule::getCurrentQuestionJson()
{
	if (count == 0)
		return "";

	CQuestions question = m_vecQuestions[cur_index];

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	document.AddMember("type", question.type.c_str(), allocator);
	//question
	rapidjson::Value questionJS(rapidjson::kObjectType);
	questionJS.AddMember("id", question.id, allocator);
	questionJS.AddMember("content", question.content.c_str(), allocator);
	rapidjson::Value options(kArrayType);
	int num = question.m_vecOptions.size();
	for (size_t i = 0; i < num; i++){
		options.PushBack(question.m_vecOptions[i].c_str(), allocator);
	}
	questionJS.AddMember("options", options, allocator);

	questionJS.AddMember("timeout", question.timeout, allocator);
	questionJS.AddMember("total", question.total_option, allocator);
	document.AddMember("question", questionJS, allocator);
	//result
	rapidjson::Value resultJS(kObjectType);
	resultJS.AddMember("result", question.result, allocator);
	resultJS.AddMember("total", count, allocator);

	rapidjson::Value spread(kObjectType);
	spread.AddMember("0", question.m_vecSpread[0], allocator);
	spread.AddMember("1", question.m_vecSpread[1], allocator);
	spread.AddMember("2", question.m_vecSpread[2], allocator);
	spread.AddMember("3", question.m_vecSpread[3], allocator);
	spread.AddMember("-1", question.no_answer, allocator);
	resultJS.AddMember("spread", spread, allocator);
	
	document.AddMember("result", resultJS, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<StringBuffer> pwriter(buffer);
	document.Accept(pwriter);

	return buffer.GetString();
}

bool CQuestionsModule::getQuestions(CQuestions& question)
{
	if (count == 0)
		return false;
	question = m_vecQuestions[cur_index];
	return true;
}

bool CQuestionsModule::getQuestionOptions(std::vector<std::string>& options)
{
	if (count == 0)
		return false;
	options = m_vecQuestions[cur_index].m_vecOptions;
	return true;
}

bool CQuestionsModule::next()
{
	if (count == 0 || cur_index == count - 1)
		return false;

	_getNextQuestion();
	if (cur_index == count - 1){
		status = Quiz_No_more;
	}
	else
		status = Quiz_Going_on;
	return true;
}

bool CQuestionsModule::current(CQuestions& question)
{
	if (count == 0)
		return false;

	question = m_vecQuestions[cur_index];
	return true;
}

bool CQuestionsModule::IsLastQuestion()
{
	if (count == 0)
		return false;

	return cur_index == count - 1;
}