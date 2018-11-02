#pragma once
#include <string>

#define USESRS_COUNT 500
class CQuestions
{
public:
	CQuestions();

public:
	std::string type = "quiz"; //
	int id = 0;                //id of question
	std::string content = "";  //question body content, string
	std::vector<std::string> m_vecOptions; //options array, total count matches with total property
	int timeout = 30;
	int total_option = 4;
	int result = 0;

	CString cs_content = _T("");
	std::vector<CString> m_vecCSOptions;
	
	std::vector<int> m_vecSpread;
	int no_answer = 0;
	/*{
	"type": "quiz",
	"question" : {
	"id": 0,
	"content" : "我是一道题目",
	"options" : ["Option A", "Option B", "Option C", "Option D"], //options array, total count matches with total property
	"timeout" : 30, //timeout of this question
	"total" : 4 //total options of this question
	},
	"result": {
	"result": 0, //result of current quiz
	"total" : 12, //total question number
	"spread" : [{0: 10, 1 : 10, 2 : 10, 3 : 10, -1 : 32}] //spread of answer, -1 means number of users whose answer are not received
	}
	}*/
};


class CQuestionsModule
{
public:
	enum QuestionStatusType{
		Quiz_Init = 0,
		Game_Closed_Already,
		Quiz_Going_on,
		Quiz_No_more ,

	};
	CQuestionsModule();

	int getQuestionsCount() { return count; };
	int getCurrentIndex() { return cur_index; }
	std::string getCurrentQuestionJson();
	int _getFirstQuestion();
	void _getNextQuestion();

	bool getQuestions(CQuestions& question);
	bool getQuestionOptions(std::vector<std::string>&);
	bool next();
	bool current(CQuestions& question);
	bool IsLastQuestion();

	void ResetQuestions(){ cur_index = -1; m_vecQuestions.clear(); }
	int getResult();
	QuestionStatusType getStatus(){ return status; }
	//bool getSpread(std::vector<int>&);
	/*
private:
	int _getFirstProductAdvertise();
	int _getNextProductAdvertise();
	*/
	std::vector<CQuestions> m_vecQuestions;
	int cur_index = -1;
	int count = 0;
	QuestionStatusType status = Quiz_Init;
};
CQuestionsModule* getQuestionsModule();
