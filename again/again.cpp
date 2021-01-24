#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "constant.h"
using namespace std;

void init();													// инициализация таблицы лексем
int insert_the_Table(char s[], int tok);						// вставка значений

int Lexer();													//лексический анализатор
void OPS_formation(int sym, int tval);							//формирование опс
void Go_to_next(int sym);										//переходит к следующему токену

void MultDivide();												//порождающее правило V
void Inside_Mult_Div();											//порождающее правило F

void PlusMinus();												//порождающее правило U
void Inside_Plus_Minus();										//порождающее правило T

void Expression();												//синтаксис выражений. Порождающее правило E
void Program_part();											//"тело" программы. Порождающее правило S
void Declaration_Var_Arr();										//объявление переменной или массива. Правило А 
void ProgExpr();												//часть программы, вспомогат. для S
char getsym();

void condition();												//условие. Порождающее правило G
void compare();													//сравнение. Порождающее правило Q
void StartSA();													//исходная функция для синт.анализатора

int lookup_Table(char s[]);										//поиск в таблице символов
int insert_the_Table(char s[], int tok);						//добавление в таблицу
void setSize();													//устанавливает размер массива
void error(const char* er);										//вывод ошибок
void output_ops();												//вывод опс	
int operand(char, int, int);									//функция для выбора операнда + - * /

void OPS_Procession();											//обработка ОПС






char getsym()
{	//взятие очередного символа текста
	sym_numb_line++;							//тот символ, который мы из строки изымаем
	CurrSym++;
	return Text_of_the_Program[CurrSym];
}

int Lexer()
{	
	/*состояние ЛА может принимать значения:
	0 - начальное
	1, 2 - промежуточные
	3 - правильное конечное
	4 - ошибочное конечное	*/
	int sym, symCount, look, state = 0;
	while (state < 3)
	{
		sym = getsym();
		spNumber = lex_Table[state][ASCII_Table[sym]];				//номер семантич прогр 
		switch (spNumber)
		{
			case 0:														//начало лексемы
				state = 1;
				symCount = 0;											// сколько у нас символов в лексеме
				lexbuf[symCount] = sym;	
				symCount++;
				break;
			case 1:														//накопление лексемы
				state = 1;
				lexbuf[symCount] = sym;
				symCount++;
				break;	
			case 2:														//Начало числа
				state = 2;
				tokenval = sym - '0';									//из аски получить реальное число отнимаем от него аски код 0
				break;
			case 3:														//накопление числа
				state = 2;
				tokenval = tokenval * 10 + sym - '0';
				break;
			case 14:												//конец файла
				state = 3;
				return EOF;
				break;
			case 15:												//имя
				state = 3;
				lexbuf[symCount] = EOS;
				if (sym != '.')
				{
					CurrSym--;										//какой символ брать из строки файла
					sym_numb_line--;
				}
				look = lookup_Table(lexbuf);						//поиск слова в таблице символов
				if (look == 0)
				{
					if (sym == '[')
						look = insert_the_Table(lexbuf, ARRAY);
					else
						look = insert_the_Table(lexbuf, VAR);
				}
				tokenval = look;
				return symtable[look].token;
				break;
			case 16:												//константа
				state = 3;
				CurrSym--;
				sym_numb_line--;
				return NUM;
				break;
			case 18:												//пропуск символа
				state = 0;
				break;
			case 19:												//не символ языка
				state = 4;
				tokenval = NONE;
				return sym;
				break;
			case 21:												//неверная лексема
				state = 4;
				cout << "Неверная лексема!";
				break;
			case 22:												//константа, переход на новую строку
				state = 3;
				return NUM;
				break;
			case 23:												//другая ошибка
				state = 4;
				cout << "Ошибка!";
				break;
			case 24:												//переход на новую строку
				state = 0;
				numb_line++;
				sym_numb_line = -1;
				break;
			case 25:												//имя, переход на новую строку
				state = 3;
				numb_line++;
				sym_numb_line = -1;
				lexbuf[symCount] = EOS;								//конец строки
				if (sym != '.')
				{
					CurrSym--;
					sym_numb_line--;
				}
				look = lookup_Table(lexbuf);						//поиск слова в таблице символов
				if (look == 0)
				{
					if (sym == '[')
						look = insert_the_Table(lexbuf, ARRAY);
					else
						look = insert_the_Table(lexbuf, VAR);
				}
				tokenval = look;
				return symtable[look].token;
				break;
			case 4:
				//  :
			case 5:
				//  /
			case 6:
				//  *
			case 7:
				//  +
			case 8:
				//  -
			case 9:
				//  (
			case 10:
				//  )
			case 11:
				//  <
			case 12:
				//  >
			case 13:
				//  ]
			case 17:
				// [
			case 20:
				//  ^
			case 26:
				//  {
			case 27:
				//  }
			case 28:
				// ,
			case 30:
				// !
			case 29:
				state = 3;											//успех - конечное состояние , распознанная лексема
				tokenval = NONE;
				return sym;
				break;
		}

	}
}



void StartSA()
{
	if (lextype != EOF)
		switch (lextype)
		{
			case VARIEBLE:
				Go_to_next(VARIEBLE);
			case INT:
				Go_to_next(INT);
				if (numb_line > 1)
				{
					cout << "Error.You can't declare variable not in 0 line \n"
					<< "Строка: " << numb_line << " символ: " << sym_numb_line << "\n";
					system("PAUSE");
					exit(0);
				}
				Declaration_Var_Arr();
				Go_to_next(';');
				StartSA();
				break;
			case IMAS:
				Go_to_next(IMAS);
				Declaration_Var_Arr();
				Go_to_next(';');
				StartSA();
				break;
			case READ:
				if (numb_line == 1 && sym_numb_line != 0 || sym_numb_line == 0)
				{
					cout << "Ошибка, не объявлена переменная для чтения\n";
					system("PAUSE");
					exit(0);
				}
				Program_part();
				break;
			case WRITE:
				if (numb_line == 1)
				{
					cout << "Ошибка, не объявлена переменная для записи\n";
					system("PAUSE");
					exit(0);
				}
				Program_part();
				break;
			default:
				Program_part();
		}
	return;
}

void setSize()
{
	if (lextype == NUM)
		massiv[last_entry].mas = new int[tokenval];
}

void Go_to_next(int sym)
{	//переход к следующей лексеме 
	if (lextype == sym)
		lextype = Lexer();
	else
	{
		cout << "Переход к следующей лексеме не может быть произведен!]\n" << "строка: " << numb_line << "  символ: " << sym_numb_line << '\n';
		system("PAUSE");
		exit(0);
	}
}

void Declaration_Var_Arr()
{
	switch (lextype)
	{
	case VAR:
		Go_to_next(VAR);
		while (lextype != ';')
		{
			Go_to_next(',');
			Go_to_next(VAR);
		}
		OPSFlag = 1;
		break;
	case ARRAY:
		Go_to_next(ARRAY);
		Go_to_next('[');
		setSize();
		Go_to_next(NUM);
		Go_to_next(']');
		while (lextype != ';')
		{
			Go_to_next(',');
			Go_to_next(ARRAY);
			Go_to_next('[');
			setSize();
			Go_to_next(NUM);
			Go_to_next(']');
		}
		OPSFlag = 1;
		break;
	default:
		cout << "syntax ERROR. Ошибка в описании переменных." << '\n';
		cout << "line: " << numb_line << " sym: " << sym_numb_line << "\n";
		system("PAUSE");
		exit(0);
		return;
	}
}

void Program_part()
{
	int pointJF, pointJ;
	switch (lextype)
	{
		case VAR:				//описание переменных
			if (OPSFlag == 0)
			{
				cout << "У вас нет описанных переменных!\n";
				system("Pause");
				exit(0);
			}
			OPS_formation(VAR, tokenval);
			Go_to_next(VAR);
			Go_to_next('=');
			Expression();
			OPS_formation('=', NONE);
			Go_to_next(';');
			Program_part();
			break;
		case ARRAY:
			if (OPSFlag == 0)
			{
				cout << "У вас нет описанных переменных!\n";
				system("Pause");
				exit(0);
			}
			OPS_formation(ARRAY, tokenval);
			Go_to_next(ARRAY);
			Go_to_next('[');
			Expression();
			Go_to_next(']');
			OPS_formation(INDEX, 0);
			Go_to_next('=');
			Expression();
			OPS_formation('=', NONE);
			Go_to_next(';');
			Program_part();
			break;
		case IF:
			if (OPSFlag == 0)
			{
				cout << "У вас нет описанных переменных!\n";
				system("Pause");
				exit(0);
			}
			Go_to_next(IF);
			Go_to_next('(');
			condition();
			Go_to_next(')');
			pointJF = OPScount;								//место для будущей мектки в ОПС
			OPScount++;
			OPS_formation(MJFALSE, 0);
			ProgExpr();
			switch (lextype)
			{
				case ELSE:
					Go_to_next(ELSE);
					pointJ = OPScount;							//место для будущей мектки в ОПС
					OPScount++;									//к следующему элементу в ОПС
					OPS_formation(MJTRUE, 0);					//заносим <jtrue> в ОПС 
					OPSarr[pointJF].elem = new char[x];
					_itoa(OPScount, OPSarr[pointJF].elem, 10);	//преобразование в адрес перехода для <jfalse>
					OPSarr[pointJ].type = OPSarr[pointJF].type = POINT;
					ProgExpr();
					OPSarr[pointJ].elem = new char[x];
					_itoa(OPScount, OPSarr[pointJ].elem, 10);
					break;
				default:
					OPSarr[pointJF].elem = new char[x];
					_itoa(OPScount, OPSarr[pointJF].elem, 10);	//преобразование в адрес перехода для <jfalse>
					OPSarr[pointJF].type = POINT;
			}
			Go_to_next(';');
			Program_part();
			break;
		case WHILE:
			if (OPSFlag == 0)
			{
				cout << "У вас нет описанных переменных!\n";
				system("Pause");
				exit(0);
			}
			Go_to_next(WHILE);
			Go_to_next('(');
			pointJ = OPScount;							//адрес перехода для <jtrue>
			condition();								//условие
			Go_to_next(')');
			pointJF = OPScount;							//место для будущей мектки в ОПС
			OPScount++;									//к следующему элементу в ОПС
		case DO:
			Go_to_next(DO);
			OPS_formation(MJFALSE, 0);					//заносим <jfalse> в ОПС 
			ProgExpr();
			OPSarr[OPScount].elem = new char[x];
			_itoa(pointJ, OPSarr[OPScount].elem, 10);
			OPSarr[OPScount].type = OPSarr[pointJF].type = POINT;
			OPScount++;
			OPS_formation(MJTRUE, 0);					//заносим <jtrue> в ОПС 
			OPSarr[pointJF].elem = new char[x];
			_itoa(OPScount, OPSarr[pointJF].elem, 10);
			Go_to_next(';');
			Program_part();
			break;
		case READ:
			if (OPSFlag == 0)
			{
				cout << "Ошибка, не объявлена переменная\n";
				system("Pause");
				exit(0);
			}
			Go_to_next(READ);
			Go_to_next('(');
			switch (lextype)
			{
				case VAR:
					OPS_formation(VAR, tokenval);
					Go_to_next(VAR);
					break;
				case ARRAY:
					OPS_formation(ARRAY, tokenval);
					Go_to_next(ARRAY);
					Go_to_next('[');
					Expression();
					Go_to_next(']');
					OPS_formation(INDEX, 0);
					break;
				default:
					cout << "Ошибка чтения!";
			}
			Go_to_next(')');
			OPS_formation(READ, 0);
			Go_to_next(';');
			Program_part();
			break;
		case WRITE:
			if (OPSFlag == 0)
			{
				cout << "Ошибка, не объявлена переменная\n";
				system("Pause");
				exit(0);
			}
			Go_to_next(WRITE);
			Go_to_next('(');
			Expression();
			Go_to_next(')');
			OPS_formation(WRITE, 0);
			Go_to_next(';');
			Program_part();
			break;
		case INT:
			StartSA();
		case IMAS:
			StartSA();
		default:
			return;
	}
}

void ops_count(OPSType vari, const char* sign)
{	
	OPSarr[OPScount].elem = (char*)sign;
	OPSarr[OPScount].type = vari;
	OPScount++;
}												//накопление и вывод ОПС в консоль

void OPS_formation(int sym, int tval)
{

	OPSarr[OPScount].elem = new char[x];
	switch (sym)
	{
		case '+':
			ops_count(SIGN, "+"); break;
		case '-':
			ops_count(SIGN, "-"); break;
		case '*':
			ops_count(SIGN, "*"); break;
		case '/':
			ops_count(SIGN, "/"); break;
		case '=':
			ops_count(SIGN, "="); break;
		case '>':
			ops_count(SIGN, ">"); break;
		case '<':
			ops_count(SIGN, "<"); break;
		case ':':
			ops_count(SIGN, ":"); break;
		case '!':
			ops_count(SIGN, "!"); break;
		case NUM:
			_itoa(tval, OPSarr[OPScount].elem, 10);
			OPSarr[OPScount].type = NUMBER;
			OPScount++;  break;
		case ARRAY:
			ops_count(MAS, symtable[tval].lexptr); break;
		case VAR:
			ops_count(IDE, symtable[tval].lexptr); break;
		case INDEX:
			ops_count(IND, "<index>"); break;
		case MJFALSE:
			ops_count(JUMPFALSE, "<jfalse>"); break;
		case MJTRUE:
			ops_count(JUMPTRUE, "<jtrue>"); break;
		case READ:
			ops_count(INP, "<re>"); break;
		case WRITE:
			ops_count(OUTP, "<wr>"); break;
		default:
			cout << '\n';
	}
}

void condition()
{
	switch (lextype)
	{
		case NUM:
			OPS_formation(NUM, tokenval);
			Go_to_next(NUM);
			MultDivide();
			PlusMinus();
			compare();
			break;
		case VAR:
			OPS_formation(VAR, tokenval);
			Go_to_next(VAR);
			MultDivide();
			PlusMinus();
			compare();
			break;
		case ARRAY:
			OPS_formation(ARRAY, tokenval);
			Go_to_next(ARRAY);
			Go_to_next('[');
			Expression();
			Go_to_next(']');
			OPS_formation(INDEX, 0);
			MultDivide();
			PlusMinus();
			compare();
			break;
		case '(':
			Go_to_next('(');
			Expression();
			Go_to_next(')');
			MultDivide();
			PlusMinus();
			compare();
			break;
		default:
			cout << "Синтаксическая ошибка в условии!" << '\n';
			cout << "Строка = " << numb_line << "  символ = " << errorS;
			system("PAUSE");
			exit(0);
	}

}

void compare()
{
	int sym;
	switch (lextype)
	{
		case ':':
		case '>':
		case '<':
		case '!':
			sym = lextype;
			Go_to_next(sym);
			Expression();
			OPS_formation(sym, NONE);
			break;
		default:
			error("Ошибка сравнения!");
			exit(0);
	}
}


void Inside_Plus_Minus()
{
	switch (lextype)
	{
	case NUM:
		OPS_formation(NUM, tokenval);
		Go_to_next(NUM);
		MultDivide();
		break;
	case VAR:
		OPS_formation(VAR, tokenval);
		Go_to_next(VAR);
		MultDivide();
		break;
	case ARRAY:
		OPS_formation(ARRAY, tokenval);
		Go_to_next(ARRAY);
		Go_to_next('[');
		Expression();
		Go_to_next(']');
		OPS_formation(INDEX, 0);
		MultDivide();
		break;
	case '(':
		Go_to_next('(');
		Expression();
		Go_to_next(')');
		MultDivide();
		break;
	default:
		error("Ошибка синтаксиса!");
		exit(0);
	}

}

void PlusMinus()
{
	switch (lextype)
	{
		int sym;
		case '+': case '-':
			sym = lextype;
			Go_to_next(sym);
			Inside_Plus_Minus();
			PlusMinus();
			OPS_formation(sym, NONE);
			break;
		default:
			return;
	}
}

void Inside_Mult_Div()
{
	switch (lextype)
	{
		case NUM:
			OPS_formation(NUM, tokenval);
			Go_to_next(NUM);
			break;
		case VAR:
			OPS_formation(VAR, tokenval);
			Go_to_next(VAR);
			break;
		case ARRAY:
			OPS_formation(ARRAY, tokenval);
			Go_to_next(ARRAY);
			Go_to_next('[');
			Expression();
			Go_to_next(']');
			OPS_formation(INDEX, 0);
			break;
		case '(':
			Go_to_next('(');
			Expression();
			Go_to_next(')');
			break;
		default:
			error("Ошибка синтаксиса!");
			exit(0);
	}
}

void MultDivide()
{
	switch (lextype)
	{
		int sym;
		case '*':
		case '/':
			sym = lextype;
			Go_to_next(lextype);
			Inside_Mult_Div();
			MultDivide();
			OPS_formation(sym, NONE);
			break;
		default:
			return;
	}
}

void Expression()
{
	switch (lextype)
	{
	case NUM:
		OPS_formation(NUM, tokenval);
		Go_to_next(NUM);
		MultDivide();
		PlusMinus();
		break;
	case VAR:
		OPS_formation(VAR, tokenval);
		Go_to_next(VAR);
		MultDivide();
		PlusMinus();
		break;
	case ARRAY:
		OPS_formation(ARRAY, tokenval);
		Go_to_next(ARRAY);
		Go_to_next('[');
		Expression();
		Go_to_next(']');
		OPS_formation(INDEX, 0);
		MultDivide();
		PlusMinus();
		break;
	case '(':
		Go_to_next('(');
		Expression();
		Go_to_next(')');
		MultDivide();
		PlusMinus();
		break;
	default:
		cout << "Ошибка синтаксиса выражений!" << '\n';
		cout << "Строка	= " << numb_line << "  символ = " << errorS;
		system("PAUSE");
		exit(0);
	}
}

void ProgExpr()
{
	int pointJF, pointJ;
	switch (lextype)
	{
		case VAR:
			OPS_formation(VAR, tokenval);
			Go_to_next(VAR);
			Go_to_next('=');
			Expression();
			OPS_formation('=', NONE);
			break;
		case ARRAY:
			OPS_formation(ARRAY, tokenval);
			Go_to_next(ARRAY);
			Go_to_next('[');
			Expression();
			Go_to_next(']');
			OPS_formation(INDEX, 0);
			Go_to_next('=');
			Expression();
			OPS_formation('=', NONE);
			break;
		case IF:
			Go_to_next(IF);
			Go_to_next('(');
			condition();
			Go_to_next(')');
			pointJF = OPScount;										//место для будущей метки в ОПС
			OPScount++;
			OPS_formation(MJFALSE, 0);
			ProgExpr();
			switch (lextype)
			{
				case ELSE:
					Go_to_next(ELSE);
					pointJ = OPScount;									//место для будущей мектки в ОПС
					OPScount++;											//к следующему элементу в ОПС
					OPS_formation(MJTRUE, 0);							//заносим <jtrue> в ОПС 
					OPSarr[pointJF].elem = new char[x];
					_itoa(OPScount, OPSarr[pointJF].elem, 10);			//адрес перехода для <jfalse>
					OPSarr[pointJ].type = OPSarr[pointJF].type = POINT;
					ProgExpr();
					OPSarr[pointJ].elem = new char[x];
					_itoa(OPScount, OPSarr[pointJ].elem, 10);
					break;
				default:
					OPSarr[pointJF].elem = new char[x];
					_itoa(OPScount, OPSarr[pointJF].elem, 10);
					OPSarr[pointJF].type = POINT;
			}
			break;
		case WHILE:
			Go_to_next(WHILE);
			Go_to_next('(');
			pointJ = OPScount;										//адрес перехода для <jtrue>
			condition();											//условие
			Go_to_next(')');
		case DO:
			Go_to_next(DO);
			pointJF = OPScount;										//место для будущей мектки в ОПС
			OPScount++;												//к следующему элементу в ОПС
			OPS_formation(MJFALSE, 0);								//заносим <jfalse> в ОПС 
			ProgExpr();
			OPSarr[OPScount].elem = new char[x];
			_itoa(pointJ, OPSarr[OPScount].elem, 10);
			OPSarr[OPScount].type = OPSarr[pointJF].type = POINT;
			OPScount++;
			OPS_formation(MJTRUE, 0);								//заносим <jtrue> в ОПС 
			OPSarr[pointJF].elem = new char[x];
			_itoa(OPScount, OPSarr[pointJF].elem, 10);
			break;
		case '{':
			Go_to_next('{');
			Program_part();
			Go_to_next('}');
			break;
		case READ:
			Go_to_next(READ);
			Go_to_next('(');
			switch (lextype)
			{
				case VAR:
					OPS_formation(VAR, tokenval);
					Go_to_next(VAR);
					break;
				case ARRAY:
					OPS_formation(ARRAY, tokenval);
					Go_to_next(ARRAY);
					Go_to_next('[');
					Expression();
					Go_to_next(']');
					OPS_formation(INDEX, 0);
					break;
				default:
					cout << "Ошибка чтения\n";
					cout << "Строка: " << numb_line << " символ: " << sym_numb_line << "\n";
			}
			Go_to_next(')');
			OPS_formation(READ, 0);
			break;
		case WRITE:
			Go_to_next(WRITE);
			Go_to_next('(');
			Expression();
			Go_to_next(')');
			OPS_formation(WRITE, 0);
			break;
		default:
			error("Синтаксическая ошибка");
			exit(0);
	}
}



void OPS_Procession()
{
	int len = OPScount;
	int current = 0, look, res, ps = 0;
	char sym;
	Stack st;
	StackElem element;
	StackElem element1;
	while (current < len)
	{
		switch (OPSarr[current].type)
		{
			case IDE:										// переменная
				look = lookup_Table(OPSarr[current].elem);	//на какой позиции в таблице переменных хранится переменная
				element.value = look;						//позиция
				element.type = IDE;							//тип
				st.push(element);							//помещаем в стек
				current++;									//переходим к следующему элементу
				break;
			case NUMBER:
				element.value = atoi(OPSarr[current].elem); // atoi преобразует строку string в целое значение типа int
				element.type = NUMBER;
				st.push(element);							//помещаем в стек
				current++;									//переходим к cледующему элементу
				break;
			case MAS:
				look = lookup_Table(OPSarr[current].elem);	//на какой позиции в таблице массивов хранится массив
				element.value = look;						//позиция
				element.type = MAS;							//тип
				st.push(element);							//помещаем в стек
				current++;									//переходим к следующему элементу
				break;
			case IND:										// реализация индексирования
				if (ps > 100) ps = 0;
				element = st.pop();							//индекс элемента
				element1 = st.pop();						//массив
				switch (element.type)
				{
					case IDE:
						passport[ps].elem = number[element.value].token;
						break;
					case NUMBER:
						passport[ps].elem = element.value;
						break;
					default:
						break;
				};
				passport[ps].mas = element1.value;
				element.value = ps;
				ps++;
				element.type = MAS;
				st.push(element);
				current++;
				break;
			case SIGN:
				sym = OPSarr[current].elem[0];
				element = st.pop();							//правое значение
				element1 = st.pop();						//левое значение
				switch (sym)
				{
					case '+':
					case '-':
					case '*':
					case '/':
					case '>':
					case '<':
					case ':':
					case '!':
					case '^':
					case '?':
						// дописать >= <=
						res = operand(sym, switchtype(element1), switchtype(element));
						break;
					case '=':
						switch (element1.type)
						{
							case IDE:
								number[element1.value].token = switchtype(element);
								break;
							case MAS:
								massiv[passport[element1.value].mas].mas[passport[element1.value].elem] = switchtype(element);
								break;
							default:
								break;
						}
						break;
				}
				if (sym != '=')
				{
					element.value = res;
					element.type = NUMBER;
					st.push(element);
				}
				current++;
				break;
			case POINT:
				look = atoi(OPSarr[current].elem);
				current++;
				if (OPSarr[current].type == JUMPFALSE)
				{
					element = st.pop();
					if (element.value == 1)
						current++;
					else current = look;
				}
				if (OPSarr[current].type == JUMPTRUE)
					current = look;
				break;
			case OUTP:
				element = st.pop();
				switch (element.type)
				{
					case IDE:
						current--;
						look = lookup_Table(OPSarr[current].elem);
						cout << number[look].token << '\n';
						current = current + 2;
						break;
					case NUMBER:
						cout << element.value << '\n';
						current++;
						break;
					case MAS:
						cout << massiv[passport[element.value].mas].mas[passport[element.value].elem] << '\n';
						current++;
						break;
					default:
						break;
				}
				break;
			case INP:
				element = st.pop();
				switch (element.type)
				{
					case IDE:
						current--;
						look = lookup_Table(OPSarr[current].elem);
						cin >> number[look].token;
						current = current + 2;
						break;
					case MAS:
						cin >> massiv[passport[element.value].mas].mas[passport[element.value].elem];
						current++;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
}



int operand(char oper, int a, int b)
{
	switch (oper)
	{
	case '+':
		return a + b;
	case '-':
		return a - b;
	case '*':
		return a * b;
	case '/':
		return a / b;
	case '>':
		return a > b;
	case '<':
		return a < b;
	case ':':
		return a == b;
	case '!':
		return a != b;
	case '?':
		return a != b;
	case '^':
		return a != b;
	default:
		error("неизвестный операнд");
		break;
	}
}

int lookup_Table(char sym[])
{	//возвращает положение в таблице символов для sym
	int p;
	for (p = last_entry; p > 0; p--)
		if (strcmp(symtable[p].lexptr, sym) == 0)
			return p;
	return 0;
}




int main()
{
	/*for (size_t i = 0; i < 128; i++)
	{
		cout <<"    "<< i <<"  "<< (char)i << endl;*/
	setlocale(LC_ALL, "rus");
	TXT_to_mass_char test("test.txt");							//запись файла тхт в массив char
	if (!test.exist())											// проверка на существование файла
		return 0;
	lextype = Lexer();											//запускаем лексический анализатор
	StartSA();													//запускаем синтаксический анализ

	OPSarr[OPScount].elem = new char[2];
	OPSarr[OPScount].elem = (char*)".";
	OPSarr[OPScount].type = SIGN;								//заносим ограничитель в опс

	output_ops();												//выводим опс

	OPS_Procession();

	system("pause");
}




void error(const char* er)
{
	cout << er << "/nСтрока: " << numb_line << " символ: " << sym_numb_line << "\n";
	system("PAUSE");
}

void output_ops()
{
	const char* a[] = { ":",">","!","<","." };
	cout << "Вывод ОПС:\n";
	for (int f = 0; f < OPScount + 1; f++)
	{
		cout << OPSarr[f].elem << "  ";
		for (auto i : a)
			if (OPSarr[f].elem == i)
				cout << "\n";
	}
	cout << "\nРабота программы:\n";
}