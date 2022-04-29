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
	iterator = 0;
}

long expr();
long term();
long factor();
long number();
int digit();

int main() {
	getInputString();
	expr();

	exit(0);
}

char getCurChar() {
	return input_buffer[iterator];
}

void moveNext() {
	if(iterator < len - 1)
		iterator++;
	else
		error_flag = 1;	
}

void moveBack() {
	if(iterator > 0)
	iterator--;
	else
		error_flag = 1;
}

void ffSpace() {
	while(getCurChar() == ' ')
		moveNext();
}

int digit()
{
	int digit = getCurChar() - '0';
	if(digit < 0 || digit > 9)
		return -1;
	return digit;
}

long number() {
	if(error_flag)
		return -1;
	long res= 0, temp_digit;
	ffSpace();
#ifdef DEBUG
	printf("\t\t\tBegin number\n");
#endif
	while((temp_digit = digit()) >= 0 && !error_flag)
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

long factor() {
	if(error_flag)
		return -1;
	long res;
	ffSpace();
	char token = getCurChar();
#ifdef DEBUG
	printf("\t\tBegin factor\n");
#endif
	if(token == '(') {
		moveNext();
		ffSpace();
		res = expr();
		token = getCurChar();
		if(token != ')') {
			fprintf(stderr, ") is not appeared!");
			error_flag = -1;	
		}
	}
	else
	{
		if(getCurChar() == '-') {
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

long term() {
#ifdef DEBUG
	printf("\tstart term\n");
#endif
	if(error_flag)
		return -1;
	long res = factor();
	ffSpace();
	char c; 
	while(!error_flag) {
		c = getCurChar();
		if(c == '*') {
			moveNext();
			ffSpace();
			res *= factor();
		}
		else if(c == '/') {
			moveNext();
			ffSpace();
			res /= factor();
		}
		else
			break;
		ffSpace();
	}
#ifdef DEBUG
	printf("\t\tres : %ld\n", res);
	printf("\tEnd factor\n");
#endif
	return res;
}

long expr() {
	if(error_flag)
		return -1;

#ifdef DEBUG
	printf("start expr\n");
#endif
	long res = term();
	ffSpace();
	char c;
	while(!error_flag) {
		c = getCurChar();
		if(c == '+') {
			moveNext();
			res += term();
		}
		if(c == '-') {
			moveNext();
			res -= term();
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







