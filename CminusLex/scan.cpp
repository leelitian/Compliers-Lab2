#include "globals.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <unordered_map>
using namespace std;

extern ifstream source;

static string lineBuf;        // ���ļ�����һ����Ϊ����
static size_t linepos;        // ��һ���е�����
static int lineno;            // �к�
static bool EOF_flag = false; // ���ھ�����EOFʱ���˺����Ĺ���

bool EchoSource = true;       // ��ʾԴ�ļ�
bool TraceScan = true;        // ��ʾ�ʷ��������

/* �ӻ�������ȡ��һ���ַ� */
char getNextChar(void)
{
	/* ��ǰ�ж�ȡ��ϣ����Զ�ȡ��һ�� */
	if (linepos >= lineBuf.length())
	{
		lineno++;
		if (getline(source, lineBuf))
		{
			lineBuf += "\n";
			if (EchoSource) cout << setw(4) << lineno << ": " << lineBuf;
			linepos = 0;
			return lineBuf[linepos++];
		}
		else
		{
			EOF_flag = true;
			return EOF;
		}
	}
	else return lineBuf[linepos++];
}

/* ����һ���ַ� */
void ungetNextChar()
{
	if (!EOF_flag) linepos--;
}

/* �ؼ��ֱ� <�ؼ��֣�����> */
unordered_map<string, TokenType> reservedMap =
{
	{"else", ELSE}, {"if", IF}, {"int", INT},
	{"return", RETURN}, {"void", VOID}, {"while", WHILE}
};

/* ��ؼ��ֱ��ж��ַ�����TokenType��ID����reserved word */
TokenType reservedLookup(string s)
{
	return (reservedMap.find(s) == reservedMap.end()) ? ID : reservedMap[s];
}

/* ��ӡ�ʷ������Ľ�� */
void printToken(TokenType token, string tokenString)
{
	switch (token)
	{
	/* �ؼ������� */
	case ELSE:
	case IF:
	case INT:
	case RETURN:
	case VOID:
	case WHILE:
		cout << "reserved word: " << tokenString << endl;
		break;
	/* ר�÷������� */
	case PLUS:
		cout << "+\n";
		break;
	case MINUS:
		cout << "-\n";
		break;
	case TIMES:
		cout << "*\n";
		break;
	case OVER:
		cout << "/\n";
		break;
	case LT:
		cout << "<\n";
		break;
	case LE:
		cout << "<=\n";
		break;
	case GT:		
		cout << ">\n";
		break;
	case GE:
		cout << ">=\n";
		break;
	case EQ:
		cout << "==\n";
		break;
	case NEQ:
		cout << "!=\n";
		break;
	case ASSIGN:
		cout << "=\n";
		break;
	case SEMI:
		cout << ";\n";
		break;
	case COMMA:
		cout << ",\n";
		break;
	case LPAREN:
		cout << "(\n";
		break;
	case RPAREN:
		cout << ")\n";
		break;
	case LBRACKET:
		cout << "[\n";
		break;
	case RBRACKET:
		cout << "]\n";
		break;
	case LBRACE:
		cout << "{\n";
		break;
	case RBRACE:
		cout << "}\n";
		break;
	/* �������� */
	case ENDFILE:
		cout << "EOF\n";
		break;
	case ERROR:
		cout << "ERROR: " << tokenString << endl;
		break;
	case ID:
		cout << "ID, name = " << tokenString << endl;
		break;
	case NUM:
		cout << "NUM, val = " << tokenString << endl;
		break;
	default:
		cout << "Unknown token :" << token << endl;
		break;
	}
}

/* ��ȡ��һ���ʷ���Ԫ������ */
TokenType getToken(void)
{
	string tokenString;
	TokenType currentToken;
	StateType state = START;
	bool save; // �Ƿ񱣴浽 tokenString
	while (state != DONE)
	{
		char c = getNextChar();
		save = true;
		switch (state)
		{
		case START:
		{
			if (isdigit(c)) state = INNUM;
			else if (isalpha(c)) state = INID;
			else if ((c == ' ') || (c == '\t') || (c == '\n')) save = false;
			else if (c == '=') state = INEQ;
			else if (c == '<') state = INLE;
			else if (c == '>') state = INGE;
			else if (c == '!') state = INNEQ;
			else if (c == '/') state = LBUF;
			else
			{
				state = DONE;
				switch (c)
				{
				case EOF:
					save = false;
					currentToken = ENDFILE;
					break;
				case '+':
					currentToken = PLUS;
					break;
				case '-':
					currentToken = MINUS;
					break;
				case '*':
					currentToken = TIMES;
					break;
				case '(':
					currentToken = LPAREN;
					break;
				case ')':
					currentToken = RPAREN;
					break;
				case '[':
					currentToken = LBRACKET;
					break;
				case ']':
					currentToken = RBRACKET;
					break;
				case '{':
					currentToken = LBRACE;
					break;
				case '}':
					currentToken = RBRACE;
					break;
				case ';':
					currentToken = SEMI;
					break;
				case ',':
					currentToken = COMMA;
					break;
				default:
					/* ERROR: ��������δ�����ַ� */
					currentToken = ERROR;
					break;
				}
			}
			break;
		}
		case LBUF:
		{
			if (c == '*')
			{
				/* ����INCOMMENT״̬�����tokenString */
				tokenString.clear();
				save = false;
				state = INCOMMENT;
			}
			else if (c == EOF)
			{
				state = DONE;
				currentToken = ENDFILE;
			}
			else
			{
				ungetNextChar();
				currentToken = OVER;
				state = DONE;
			}
			break;
		}
		case INCOMMENT:
		{
			save = false;
			if (c == '*') state = RBUF;
			else if (c == EOF)
			{
				state = DONE;
				currentToken = ENDFILE;
			}
			break;
		}
		case RBUF:
		{
			save = false;
			if (c == '/') state = START;
			else if (c == '*') state = RBUF;
			else if (c == EOF)
			{
				state = DONE;
				currentToken = ENDFILE;
			}
			else state = INCOMMENT;
			break;
		}
		case INNUM:
		{
			if (!isdigit(c))
			{
				ungetNextChar();
				save = false;
				state = DONE;
				currentToken = NUM;
			}
			break;
		}
		case INID:
		{
			if (!isalpha(c))
			{
				ungetNextChar();
				save = false;
				state = DONE;
				currentToken = ID;
			}
			break;
		}
		case INEQ:
		{
			if (c == '=')
			{
				state = DONE;
				currentToken = EQ;
			}
			else
			{
				ungetNextChar();
				save = false;
				state = DONE;
				currentToken = ASSIGN;
			}
			break;
		}
		case INLE:
		{
			if (c == '=')
			{
				state = DONE;
				currentToken = LE;
			}
			else
			{
				ungetNextChar();
				save = false;
				state = DONE;
				currentToken = LT;
			}
			break;
		}
		case INGE:
		{
			if (c == '=')
			{
				state = DONE;
				currentToken = GE;
			}
			else
			{
				ungetNextChar();
				save = false;
				state = DONE;
				currentToken = GT;
			}
			break;
		}
		case INNEQ:
		{
			if (c == '=')
			{
				state = DONE;
				currentToken = NEQ;
			}
			else
			{
				ungetNextChar();
				save = false;
				state = DONE;
				/* ERROR: !������ֲ���=���ַ� */
				currentToken = ERROR;
			}
			break;
		}
		case DONE:
		default:
		{
			cout << "Scanner Bug:state=" << state << "\n";
			state = DONE;
			currentToken = ERROR;
			break;
		}
		}

		if(save) tokenString += c;
		/* ����ID����Ҫ������Ƿ�Ϊ�ؼ������� */
		if (state == DONE && currentToken == ID)
			currentToken = reservedLookup(tokenString);
	}
	
	/* ��ӡ�кźʹʷ���Ԫ����Ϣ */
	if (TraceScan)
	{
		cout << "\t" << lineno << ": ";
		printToken(currentToken, tokenString);
	}
	return currentToken;
}