#pragma once
#include <iostream>
#include <string>

using namespace std;

enum OPSType
{
	IDE,			//����������
	MAS,			//������
	NUMBER,			//�����
	SIGN,			//��������
	POINT,			//����� ��������
	IND,			//����������
	JUMPFALSE,		//����� ��� "����"
	JUMPTRUE,		//����� ��� ������������ ��������
	INP,			//����� ������
	OUTP			//����� ������
};

struct StackElem
{
	int value;				//ASCII-��� ������� ��� �������� �������
	OPSType type;
};

struct OPSStruct
{	//������ � ������� �������� � ������� ����������
	char* elem;
	OPSType type;
};

struct LexEntry
{	 // �������� �����
	char* lexptr;
	int token;
};

struct ArrEntry
{	//������ � ������� ��������
	char* masptr;
	int* mas;
};

struct ElMas
{	//��� �������, �������� ��������� �������
	int mas;
	int elem;
};

	const char EOS = '\0';					//����� ������
	const int SIZE = 500;					//������ �����
	const int NONE = -1;					//������ ������
	const int BUFFER = 10;					//������ ������
	const int NUM = 101;					//�����
	const int VAR = 102;					//����������
	const int VARIEBLE = 103;				//��� ��������� ����� VAR
	const int ARRAY = 104;					//������
	const int IF = 105;
	const int ELSE = 106;
	const int WHILE = 107;
	const int DO = 108;
	const int READ = 109;
	const int WRITE = 110;
	const int INT = 111;					//������������� ��� ����������
	const int IMAS = 112;					//������������� ��� ������� (����������� ����)
	const int INDEX = 113;					//����� ������� ��� ��� ��� ����������
	const int MJFALSE = 114;				//����� �������� �� "����" ��� ���
	const int MJTRUE = 115;					//����� ������������ �������� ��� ���
	const int LEXMAX = 1000;				//������ ������� ������
	const int SYMMAX = 100;					//������ ������� ��������

	bool OPSFlag = 0;					//����,����������� ���� ��� �� ���� ���������� ����������
	int spNumber;						//����� ������������� ���������
	int OPScount = 0;					//������� ��������� � ���
	int last_char = -1;					//��������� �������������� ������� � lexemes
	int last_entry = 0;					//��������� �������������� ������� � ������� ��������
	int lextype;						//��� �������
	int CurrSym = -1;					//����� ������������� �������
	int sym_numb_line = -1;				//����� ������� � ������ ���������
	int errorS = -1;					//���������
	int numb_line = 1;					//����� ������
	int tokenval;						//�������� ������
	int x = 10;							//���������� ����� �������� � ��� 
	int lex_Table[3][25] = {
		//0   1  2    3   4   5   6   7   8  9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24
		{ 0,  2, 18,  4,  5,  6,  7,  8,  9, 20, 17, 10, 11, 12, 13, 26, 27, 28, 19, 24, 14, 29, 30, 31, 32 }, //0
		{ 1, 21, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 23, 19, 25 ,15, 15, 15, 15, 15 }, //1
		{ 21, 3, 16, 23, 16, 16, 16, 16, 16, 23, 16, 23, 16, 16, 16, 16, 23, 23, 19, 22 ,16, 23, 16, 16, 16 }  //2
	};
	int ASCII_Table[128] = {
		//������� ���������, �� ASCI-���� ������� �������� ����� ������� � lexTab
		//����� ������� � ���������,
		//��� ������� ���� ������ ������� 
		18, 18, 18, 18, 18, 18, 18, 18, 18, 2,
		19, 18, 18, 18, 18, 18, 18, 18, 18, 18,	//10
		18, 18, 18, 18, 18, 18, 18, 18, 18, 18,	//20
		18, 18,  2, 18, 18, 24, 22, 18, 20, 18,	//30
		11, 12,  6,  7, 21,  8, 18,  5,  1,  1,	//40
		1,  1,  1,  1,  1,  1,  1,  1, 18, 15,	//50
		13, 4, 14, 18, 18,  0,  0,  0,  0,  0,	//60
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//70
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//80
		0,  9, 18, 10, 23, 18, 18,  0,  0,  0,	//90
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//100
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	//110
		0,  0,  0, 18, 18, 18, 18
	};
	
	char lexemes[LEXMAX];				//������ ������
	char lexbuf[BUFFER];				//����� �������
	char* Text_of_the_Program;


	LexEntry  symtable[SYMMAX];			//������� ��������
	LexEntry  number[SYMMAX];			//���������� � �� ��������
	ArrEntry  massiv[SYMMAX];			//������� � �� ����������
	ElMas     passport[1000];			//������� �������
	OPSStruct OPSarr[9999];



	class Stack
	{
		StackElem stck[SIZE];
		int StackSize;			 //������� ������ �����
	public:
		Stack();
		~Stack() {};
		void push(StackElem i);
		StackElem pop();
	};
	   
	Stack::Stack()
	{
		StackSize = 0;
	}

	void Stack::push(StackElem i)
	{
		if (StackSize == SIZE) {
			cout << "���� �����!. \n";
			system("PAUSE");
			exit(0);
		}
		stck[StackSize] = i;
		StackSize++;
	}

	StackElem Stack::pop()
	{
		if (StackSize == 0) {
			cout << "���� ����!.\n";
			system("PAUSE");
			exit(0);
		}
		StackSize--;
		return stck[StackSize];
	}



	int insert_the_Table(char s[], int tok)					//
	{
		//��������� ����� ������� � ���������� ��������� � ������� �������� ��� s
		int len;
		len = strlen(s);									//��������� ����� ������
		if (last_entry + 1 >= SYMMAX)						//������ ����������
		{
			cout << "������� �������� �����������!";
			system("PAUSE");
			exit(0);
		}
		if (last_char + len + 1 >= LEXMAX)					//������ ������ ����������
		{
			cout << "������ ������ ����������!";
			system("PAUSE");
			exit(0);
		}
		for (int i = 0; i < last_entry; i++) {				//���� �� ��������� ��������� � ����� ������-������
			if (tok == symtable[i].token && numb_line != 1)
			{
				cout << "������. �� �� �������� ��� ����������!\n";
				cout << "������: " << numb_line << "  ������: " << sym_numb_line << '\n';
				system("PAUSE");
				exit(0);
			}
		}

		last_entry++;												//��������� � ��������� ������� ��� ������ � ������� ��������
		symtable[last_entry].token = tok;							//������������� �����, ����������� �������� �����
		symtable[last_entry].lexptr = &lexemes[last_char + 1];		//����� ������ ������� � ������� ������
		if (tok == VAR)
			number[last_entry].lexptr = &lexemes[last_char + 1];
		if (tok == ARRAY)
			massiv[last_entry].masptr = &lexemes[last_char + 1];
		last_char = last_char + len + 1;							//��������� ��������� ������� � ������� ������
		strcpy(symtable[last_entry].lexptr, s);						//��������� ������ ������
		return last_entry;
	}

	void init()
	{	//�������� �������� ���� � ������� ��������
		LexEntry  KEYWORDS[] = {	//������� �������� ����
			(char*)"var", VARIEBLE,
			(char*)"if", IF,
			(char*)"else", ELSE,
			(char*)"while", WHILE,
			(char*)"do", DO,
			(char*)"read", READ,
			(char*)"write", WRITE,
			(char*)"int",  INT,
			(char*)"array", IMAS,
			0, 0
		};
		LexEntry* p = KEYWORDS;
		for (; p->token; p++)
			insert_the_Table(p->lexptr, p->token); //����� - ���� ��� ��������� �����.
	}



	class TXT_to_mass_char
	{
	public:
		TXT_to_mass_char(const char*);
		~TXT_to_mass_char() {};
		bool exist();
	private:
		bool empty = 0;
		bool record(FILE*);
	};

	TXT_to_mass_char::TXT_to_mass_char(const char* file_name)
	{
		setlocale(LC_ALL, "rus");
		FILE* fp;
		fopen_s(&fp, file_name, "r");
		if (record(fp))
		{
			cout << "���� ������ �������!" << endl;
			empty = 1;
			fclose(fp); 	//�������� �����
			init();															//����������  ������� ��������� �������	
		}
	}

	bool TXT_to_mass_char::exist()
	{
		if (empty == 0)
			return false;
		return true;
	}

	bool TXT_to_mass_char::record(FILE* fp)
	{
		if (fp == NULL)
		{
			cout << "���� �� ������!";
			return false;
		}
		else
		{
			fseek(fp, 0, SEEK_END);
			int fileSize = ftell(fp);
			if (fileSize == 0)
			{
				cout << "���� ����!" << endl;
				fclose(fp);
				return false;
			}
			Text_of_the_Program = new char[fileSize];
			fseek(fp, 0, SEEK_SET);

			int i = -1;
			while (i < fileSize - 1)
				Text_of_the_Program[++i] = getc(fp);

			Text_of_the_Program[i] = '.';									//�������� ������ - ������������
		}
		return true;
	}
	


	const char* choice()
	{
		setlocale(LC_ALL, "rus");
		int i = 0;
		const char* choice [4] { "test.txt" ,"sort.txt" ,"sort_check.txt" ,"sravn.txt" };
		cout << "����� ��� ����������" << endl;
		for (auto a : choice)
		{
			cout << i++ << " " << a << endl;
		}
		cout << "�������� ���������� ����: "; 
		cin >> i;
		cout << endl << "�� �������: "<< choice[i] << endl;
		return choice[i];
	};

	int  switchtype(StackElem elem)
	{
		switch (elem.type)
		{
		case IDE:
			return number[elem.value].token;
		case MAS:
			return massiv[passport[elem.value].mas].mas[passport[elem.value].elem];
		case NUMBER:
			return elem.value;
		default:
			return 0;
		}
	}