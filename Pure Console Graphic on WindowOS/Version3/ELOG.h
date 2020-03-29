#pragma once
#include <fstream>
#include <string>
#pragma warning(disable : 4996)
#include <ctime>

#define elog LOG _logtxt; _logtxt

using namespace std;

const std::string currentDateTime();

class LOG
{
private:
	string file_name = "LOG.txt";
public:
	void LOG::operator << (string A);
	void LOG::Clearlog();
	void LOG::Filelog(string txt, ...);
};