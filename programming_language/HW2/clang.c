#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 99999
typedef short flag;

/*
	<expr> → <term> {+ <term> | - <term>}
	<term> → <factor> {* <factor> | / <factor>}
	<factor> → [ - ] ( <number> | (<expr>) )
	<number> → <digit> {<digit>}
	<digit> → 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
*/

char input_buffer[MAX_BUFFER];
int len, iterator;
flag error_flag;

void getInputString()
{
	gets(input_buffer);
	len = strlen(input_buffer);
	// 마지막에 추가
	input_buffer[len] = '$';
	iterator = 0;
}

long expr();
long term();
long factor();
long number();
int digit();
char getCurChar();
flag is_valid(char a) {
	switch(a) 
	{
		case '+':
		case '-':
		case '*':
		case '/':
		case '(':
		case ')':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case ' ':
		case '\t':
		case '$':
			return 1;
		default:
			return 0;
	}
}

int main()
{
	long res = 0;
	while (!error_flag)
	{
		printf(">>");
		getInputString();
		res = expr();
		if(getCurChar() != '$')
			error_flag = 1;
		if (error_flag)
		{
			printf("syntax error!!\n");
			break;
		}
		printf("%ld\n", res);
	}
	exit(0);
}

char getCurChar()
{
	return input_buffer[iterator];
}

void moveNext()
{
	iterator++;
}

void ffSpace()
{
	while (getCurChar() == ' ' || getCurChar() == '\t')
		moveNext();
}

int digit()
{
	char cur_char = getCurChar();
	if(!is_valid(cur_char)) {
		error_flag = 1;
		return -1;
	}
	int digit = cur_char - '0';
	if (digit < 0 || digit > 9) {
		return -1;
	}

	return digit;
}

long number()
{
	long res = 0, temp_digit;
	ffSpace();
#ifdef DEBUG
	printf("\t\t\tBegin number\n");
#endif
	while ((temp_digit = digit()) >= 0)
	{
		res = res * 10 + temp_digit;
#ifdef DEBUG
		printf("\t\t\t\ttemp_res : %ld\t", res);
#endif
		moveNext();
	}

#ifdef DEBUG
	printf("\t\t\tEnd number\n");
#endif
	return res;
}

long factor()
{
	long res;
	ffSpace();
	char token = getCurChar();
#ifdef DEBUG
	printf("\t\tBegin factor\n");
#endif
	if (token == '(')
	{
		moveNext();
		ffSpace();
		res = expr();
		token = getCurChar();
		if (token != ')')
		{
			error_flag = -1;
		}
		moveNext();
	}
	else
	{
		if (getCurChar() == '-')
		{
			moveNext();
			ffSpace();
			res = -1;
		}
		else
			res = 1;

		// number 실행
		res = res * number();
	}
#ifdef DEBUG
	printf("\t\t\tres : %ld\n", res);
	printf("\t\tEnd factor\n");
#endif
	return res;
}

long term()
{
#ifdef DEBUG
	printf("\tstart term\n");
#endif
	if (error_flag)
		return -1;
	long res = factor();
	ffSpace();
	char c;
	while (!error_flag)
	{
		c = getCurChar();

		if (c == '*')
		{
			moveNext();
			ffSpace();
			res *= factor();
		}
		else if (c == '/')
		{
			moveNext();
			ffSpace();
			res /= factor();
		}
		else
			break;
	}
#ifdef DEBUG
	printf("\t\tres : %ld\n", res);
	printf("\tEnd factor\n");
#endif
	return res;
}

long expr()
{
	if (error_flag)
		return -1;

#ifdef DEBUG
	printf("start expr\n");
#endif
	long res = term();
	ffSpace();
	char c;
	while (!error_flag)
	{
		c = getCurChar();
		if (c == '+' || c == '-')
		{
			moveNext();
			ffSpace();

			if (c == '+')
			{
				res += term();
			}
			if (c == '-')
			{
				res -= term();
			}
		}
		else
			break;

#ifdef DEBUG
		printf("\ttemp_res : %ld\n", res);
#endif
		ffSpace();
	}
#ifdef DEBUG
	printf("\tres : %ld\n", res);
	printf("End exper\n");
#endif
	return res;
}
