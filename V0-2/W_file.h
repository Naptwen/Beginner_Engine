#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#pragma warning(disable:4996)

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

using namespace std;
void FileCreate(string NAME);
void FileRead(string NAME, string &copy);
void FileShow(string NAME);
void FileEdit(string NAME, string TEXT);
void FileAdd(string NAME, string TEXT);
void FileCopy(string ORIGINAL, string COPY);
