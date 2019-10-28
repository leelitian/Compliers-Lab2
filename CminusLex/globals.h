#pragma once

/* �ʷ���Ԫ������ */
enum TokenType
{
	ENDFILE, ERROR,
	/* ������ */
	ELSE, IF, INT, RETURN, VOID, WHILE,
	/* ���ַ��ʷ���Ԫ */
	ID, NUM,
	/* �����ַ� */
	PLUS, MINUS, TIMES, OVER, LT, LE, GT, GE, EQ, NEQ, ASSIGN, SEMI, COMMA, LPAREN, RPAREN, LBRACKET, RBRACKET, LBRACE, RBRACE
//	  +     -      *     /     <  <=   >  >=  ==  !=     =      ;      ,       (       )       [         ]         {      }
};

/* DFA״̬ */
enum StateType { START, LBUF, RBUF, INCOMMENT, INNUM, INID, INEQ, INLE, INGE, INNEQ, DONE };

TokenType getToken(void);