#include "W_file.h"
void FileCreate(string NAME)
{
	FILE *fp = fopen(NAME.c_str(), "r");
	if (fp == NULL)
	{
		fp = fopen(NAME.c_str(), "w");
		cout <<  KYEL << "[File] : CREATED SUCCESS" << endl;
		fclose(fp);
	}
	else
	{
		cout <<  KRED << "[File] : It is already EXSIST" << endl;
		fclose(fp);
	}
}
void FileRead(string NAME, string &copy)
{
	FILE *fp = fopen(NAME.c_str(), "r");
	char buffer = ' ';
	cout << "[File] : FILE OPENING . . ." << endl;
	int _size = 0;
	if (fp == NULL)
	{
		cout <<  KRED << "[File] : FAILED!" << endl;
	}
	else
	{
		cout << "[File] : SUCESS READING File" << endl;
		while (fscanf(fp, "%c", &buffer) != EOF)
		{
			copy += buffer;
		}
		fclose(fp);
		cout << "[FILE] : READING <"<<copy << '>'<<endl;
	}
}
void FileShow(string NAME)
{
	{
		FILE *fp = fopen(NAME.c_str(), "r");
		char buffer = ' ';
		cout << "[File] : FILE OPENING . . ." << endl;
		int size = 0;
		if (fp == NULL)
		{
			cout << KRED << "[File] : FAILED!" << endl;
		}
		else
		{
			cout << "[File] : SUCESS SHOWING File" << endl;
			while (fscanf(fp, "%c", &buffer) != EOF)
			{
				cout << buffer;
				size++;
			}
			cout << endl;
			//fscanf(fp, " %[^$]", buffer);cout << buffer << endl;
			fclose(fp);
		}
	}
}
void FileEdit(string NAME, string TEXT)
{
	FILE *fp = fopen(NAME.c_str(), "r");
	cout << "[File] : FILE OPENING . . ." << endl;
	if (fp == NULL)
	{
		cout << KRED << "[File] : FAILED!" << endl;
	}
	else
	{
		fclose(fp);
		fp = fopen(NAME.c_str(), "w");
		fprintf(fp, TEXT.c_str());
		fclose(fp);
		cout  << KYEL << "[File] : SUCESS Editing TEXT" << endl;
	}
}
void FileAdd(string NAME, string TEXT)
{
	FILE *fp = fopen(NAME.c_str(), "r");
	cout << "[File] : FILE OPENING . . ." << endl;

	if (fp == NULL)
	{
		cout << KRED << "[File] : FAILED!" << endl;
	}
	else
	{
		fclose(fp);
		fp = fopen(NAME.c_str(), "a");
		fprintf(fp, TEXT.c_str());
		fclose(fp);
		cout << KYEL << "[File] : SUCESS Adding TEXT" << endl;
	}
}
void FileCopy(string ORIGINAL, string COPY)
{
	FILE *fp = fopen(ORIGINAL.c_str(), "r");
	FILE *fp2 = fopen(COPY.c_str(), "r");
	cout << "[File] : FILE OPENING . . ." << endl;
	if (fp == NULL || fp2 == NULL)
	{
		cout << KRED << "[File] : FAILED!" << endl;
	}
	else
	{
		fclose(fp);
		fp = fopen(ORIGINAL.c_str(), "w");
		string buffer = "";
		fscanf(fp2, " %[^$]", buffer);
		fprintf(fp, buffer.c_str());
		fclose(fp);
		fclose(fp2);
		cout << KYEL << "[File] : COPYING SUCESS"<< endl;
	}
}