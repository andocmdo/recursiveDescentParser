#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef  enum {PLUS, MINUS, DIVIDE, MULT, REMAINDER, POWER, LPAREN, RPAREN, NUMBER, ERROR, EOL} TokenType;

struct Token {
	TokenType type;
	int value;
};

struct Token token;

int ch = 0;

int statement(void);
int expr(void);
int term(void);
int power(void);
int factor(void);
int factor1(void);
void error(void);
int parse(void);
int match(TokenType tkType);
int getNumber(void);
int isDigit(int someChar);
struct Token getToken(void);
int raiseToPower(int base, int exponent);


// meat and potatoes
int main() {
	printf("Input: ");
	//token = getToken();
	int finalVal = statement();
	//int finalVal = expr();
	printf("Result: %d\n", finalVal );
}


int statement(void) {
	// command -> expr '\n'
	//printf("start statement\n");
	int statementVal = 0;
	/*
	do {
		token = getToken();
		printf("token value: %d, token type: %d\n", token.value, token.type);
		statementVal = expr();
		//token = getToken();
		printf("token value: %d, token type: %d\n", token.value, token.type);
	} while (token.type != EOL | token.type != ERROR) ;
	*/
	token = getToken();
	statementVal = expr();
	if (token.type != EOL) {
		printf("You've made a horrible mistake...\n");
	}
	return statementVal;
}


int expr(void) {
	// expr -> term { + term | -term }
	//printf("start expr\n");
	int currentTermVal = term();
	while (token.type == PLUS || token.type == MINUS) {
		TokenType currentTokenType = token.type;
		token = getToken();
		switch (currentTokenType) {
			case PLUS:
				currentTermVal = currentTermVal + term();
				break;

			case MINUS:
				currentTermVal = currentTermVal - term();
				break;

			default:
				printf("error in expr\n");
				error();
		}
	}
	return currentTermVal;
}

int term(void) {
	// term -> power { * power | / power | % power }
	//printf("start term\n");
	int currentPowerVal = power();
	while (token.type == MULT || token.type == DIVIDE || token.type == REMAINDER) {
		TokenType currentTokenType = token.type;
		token = getToken();
		switch (currentTokenType) {
			case MULT:
				currentPowerVal = currentPowerVal * power();
				break;

			case DIVIDE:
				currentPowerVal = currentPowerVal / power();
				break;

			case REMAINDER:
				currentPowerVal = currentPowerVal % power();
				break;

			default:
				printf("error in term()\n");
				error();
		}
	}
	return currentPowerVal;
}

int power(void) {
	// power -> factor [ ^power ]
	//printf("start power\n");
	int currentFactorVal = factor();
	while (token.type == POWER) {
		TokenType currentTokenType = token.type;
		token = getToken();
		int exp = 0;
		switch (currentTokenType) {
			case POWER:
				exp = factor();
				currentFactorVal = raiseToPower(currentFactorVal, exp);
				break;

			default:
			printf("error in power()\n");
			error();
		}
	}
	return currentFactorVal;
}

int factor(void) {  // I couldn't figure out why there was two implementations of factor, but my program works fine...
	// factor -> [-] factor1
	//printf("start factor\n");
	int currentFactor1Val = factor1();
	return currentFactor1Val;
}

int factor1(void) {
	// factor1 -> ( expr ) | NUMBER
	//printf("start factor1\n");
	int thisVal = 0;
	switch (token.type) {
		case NUMBER:
			thisVal = token.value;
			//token = getToken();
			break;

		case LPAREN:
			token = getToken();
			thisVal = expr();
			//token = 
			if (token.type != RPAREN) {
				printf("error in factor1(), missing RPAREN\n");
				error();
			}
			break;

		case RPAREN:
			//token = getToken();
			printf("error in factor1(), RPAREN with no LPAREN\n");
			error();
			break;

		default:
			printf("error in factor1(), should have seen a number or LPAREN\n");
			error();
	}
	token = getToken();
	return thisVal;
}

//int parse(void); // I never used this function...

//int match(TokenType tkType); // not sure what this one was for...

int getNumber(void) {
	//printf("start getNumber\n");
	int total=0;
	do {
		total = total * 10 + (ch - 48); // using ascii code to convert to int (x - 48)
		ch = getchar();
		//printf("char was: %c \n", ch);
	} while (isDigit(ch));
	//printf("the number was: %d\n", total);
	ungetc(ch, stdin);
	return total;
}

int isDigit(int someChar) {
	//printf("called isDigit\n");
	if ( (someChar <= 57) && (someChar >= 48) ) {
		// numbers, 48-57 in ascii
		//printf("it is a digit\n");
		return 1;
	} else {
		//printf("no it is not a digit\n");
		return 0;
	}
}

struct Token getToken(void) {
	//printf("start getToken\n");
	ch = getchar();
	//printf("char was: %c\n", ch);
	while (ch == 32) {
		// get whitespace
		//printf("getToken -> in whitespace loop\n");
		ch = getchar();
		//printf("char was: %c \n", ch);
	}
	if (isDigit(ch)) {
		// get numbers, 48-57 in ascii
		//printf("getToken -> isDigit\n");
		token.type = NUMBER;
		token.value = getNumber();
		printf("%d\tNUMBER\n", token.value);
	} else {
		switch (ch) {
			case '+' :
				//printf("found +\n");
				token.type = PLUS;
				token.value = 0;
				printf("+\tPLUS\n");
				break;

			case '-' :
				//printf("found -\n");
				token.type = MINUS;
				token.value = 0;
				printf("-\tMINUS\n");
				break;

			case '/' :
				token.type = DIVIDE;
				token.value = 0;
				printf("/\tDIVIDE\n");
				break;

			case '*' :
				token.type = MULT;
				token.value = 0;
				printf("*\tMULT\n");
				break;

			case '%' :
				token.type = REMAINDER;
				token.value = 0;
				printf("%\tREMAINDER\n");
				break;

			case '^' :
				token.type = POWER;
				token.value = 0;
				printf("^\tPOWER\n");
				break;

			case '(' :
				token.type = LPAREN;
				token.value = 0;
				printf("(\tLPAREN\n");
				break;

			case ')' :
				token.type = RPAREN;
				token.value = 0;
				printf(")\tRPAREN\n");
				break;

			case 10 : // newline in linux
				token.type = EOL;
				token.value = 0;
				printf(" \tEOL\n");
				break;

			case -1 : //error with getchar
				token.type = ERROR;
				token.value = 0;
				printf("-1\tERROR\n");

			default:
				printf("Invalid character: %c\n", ch);
				token.type = ERROR;
				token.value = 0;
				error();
		}
	}
	return token;
}
// My own power function for integers
int raiseToPower(int base, int exponent) {
	int i;
	int result = 1;
	for (i = 0; i < exponent; i++) {
		result *= base;
	}
	return result;
}

void error(void) {
	printf("Something went wrong... the last char was: %d (decimal code in ASCII)\n", ch);
	exit(ch);
}


