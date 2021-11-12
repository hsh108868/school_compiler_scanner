#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define NO_KEYWORDS 7 /* 단어 심벌의 개수 */
#define ID_LENGTH 12  /* 유효한 길이를 정의하는 상수 */

char id[ID_LENGTH];

enum tsymbol
{
	tnull = -1,
	/* 토큰들의 토큰 번호 */ tnot,
	tnotequ,
	tmod,
	tmodAssign,
	tident,
	tnumber, /* 0 1 2 3 4 5*/
	tand,
	tlparen,
	trparen,
	tmul,
	tmulAssign,
	tplus, /* 6 7 8 9 10 11 */
	tinc,
	taddAssign,
	tcomma,
	tminus,
	tdec,
	tsubAssign, /* 12 13 14 15 16 17 */
	tdiv,
	tdivAssign,
	tsemicolon,
	tless,
	tlesse,
	tassgin, /* 18 19 20 21 22 23 */
	tequal,
	tgreat,
	tgreate,
	tlbracket,
	trbracket,
	teof, /* 24 25 26 27 28 29 */
	/* word symbols */
	tconst,
	telse,
	tif,
	tint,
	treturn,
	tvoid, /* 30 31 32 33 34 35 */
	twhile,
	tlbrace,
	tor,
	trbrace /* 36 37 38 39 */
};

typedef struct tokenType
{				/* 파서에게 넘겨주는 구조체 == 스캐너의 출력 */
	int number; /* 토큰 번호 */
	union
	{
		char id[ID_LENGTH];
		int num;
	} value; /* 토큰 벨류 */
} tokenType;

/* 각 지정어의 스트링 값을 갖는 배열 */
char *keyword[NO_KEYWORDS] = {
	"const", "else", "if", "int", "return", "void", "while"};

/* 각 지정어에 해당하는 토큰 번호를 갖는 배열 */
enum tsymbol tnum[NO_KEYWORDS] = {
	tconst, telse, tif, tint, treturn, tvoid, twhile};

void lexicalError(int n)
{
	printf("*** Lexical Error : ");
	switch (n)
	{
	case 1:
		printf("an identifiler length must be less than 12.\n");
		break;
	case 2:
		printf("next character must be &.\n");
		break;
	case 3:
		printf("next character must be |.\n");
		break;
	case 4:
		printf("invalid character!!!\n");
		break;
	}
}

int superLetter(char ch) // '알파벳' 혹은 '_' 일 때 트루
{
	if (isalpha(ch) || ch == '_')
		return (1);
	else
		return (0);
}

int superLetterOrDigit(char ch) // '알파벳' 혹은 '숫자' 혹은 '_' 일 때 트루
{
	if (isalnum(ch) || ch == '_')
		return (1);
	else
		return (0);
}

int getIntNum(char firstCharacter, FILE *source_file)
{
	int num = 0;
	char ch;

	if (firstCharacter != '0')
	{
		ch = firstCharacter;
		do
		{
			num = num * 10 + (int)(ch - '0');
			ch = fgetc(source_file);
		} while (isdigit(ch));
	}
	ungetc(ch, source_file); // retract

	return num;
}

struct tokenType scanner(FILE *source_file)
{
	struct tokenType token;
	int i, index;
	char ch, id[ID_LENGTH];

	token.number = tnull;
	do
	{
		while (isspace(ch = fgetc(source_file))) // state 1: skip blanks
			;
		if (superLetter(ch)) // identifier or keyword
		{
			i = 0;
			do
			{
				if (i < ID_LENGTH)
					id[i++] = ch;
				ch = fgetc(source_file);
			} while (superLetterOrDigit(ch));
			if (i >= ID_LENGTH)
			{
				lexicalError(1);
				break;
			}
			id[i] = '\0';
			ungetc(ch, stdin); // retract
			// find the identifier in the keyword table
			for (index = 0; index < NO_KEYWORDS; index++)
			{
				if (!strcmp(id, keyword[index]))
					break;
			}
			if (index < NO_KEYWORDS) // found, keyword exit
			{
				token.number = tnum[index];
				strcpy(token.value.id, id);
			}
			else // not found, identifier exit
			{
				token.number = tident;
				strcpy(token.value.id, id);
			}
		}					  // end of identifier or keyword
		else if (isdigit(ch)) // integer constant
		{
			token.number = tnumber;
			token.value.num = getIntNum(ch, source_file);
		}
		else
			switch (ch) // special character
			{
			case '/': // state 10
				ch = fgetc(source_file);
				if (ch == '*') // text comment
				{
					do
					{
						while (ch != '*')
							ch = fgetc(source_file);
						ch = fgetc(source_file);
					} while (ch != '/');
				}
				else if (ch == '/') // line comment
					while (fgetc(source_file) != '\n')
						;
				else if (ch == '=')
				{
					token.number = tdivAssign;
					token.value.id[0] = '/';
					token.value.id[1] = '=';
					token.value.id[2] = '\0';
				}
				else
				{
					token.number = tdiv;
					token.value.id[0] = '/';
					token.value.id[1] = '\0';
					ungetc(ch, stdin); // retract
				}
				break;
			case '!': // state 17
				ch = fgetc(source_file);
				if (ch == '=')
				{
					token.number = tnotequ;
					token.value.id[0] = '!';
					token.value.id[1] = '=';
					token.value.id[2] = '\0';
				}
				else
				{
					token.number = tnot;
					token.value.id[0] = '!';
					token.value.id[1] = '\0';
					ungetc(ch, source_file); // retract
				}
				break;
			case '%': // state 20
				ch = fgetc(source_file);
				if (ch == '=')
				{
					token.number = tmodAssign;
					token.value.id[0] = '%';
					token.value.id[1] = '=';
					token.value.id[2] = '\0';
				}
				else
				{
					token.number = tmod;
					token.value.id[0] = '%';
					token.value.id[1] = '\0';
					ungetc(ch, source_file);
				}
				break;
			case '&': // state 23
				ch = fgetc(source_file);
				if (ch == '&')
				{
					token.number = tand;
					token.value.id[0] = '&';
					token.value.id[1] = '\0';
				}
				else
				{
					lexicalError(2);
					ungetc(ch, source_file); // retract
				}
				break;
			case '*': // state 25
				ch = fgetc(source_file);
				if (ch == '=')
				{
					token.number = tmulAssign;
					token.value.id[0] = '*';
					token.value.id[1] = '=';
					token.value.id[2] = '\0';
				}
				else
				{
					token.number = tmul;
					token.value.id[0] = '*';
					token.value.id[1] = '\0';
					ungetc(ch, source_file); // retract
				}
				break;
			case '+': // state 28
				ch = fgetc(source_file);
				if (ch == '+')
				{
					token.number = tinc;
					token.value.id[0] = '+';
					token.value.id[1] = '+';
					token.value.id[2] = '\0';
				}
				else if (ch == '=')
				{
					token.number = taddAssign;
					token.value.id[0] = '+';
					token.value.id[1] = '=';
					token.value.id[2] = '\0';
				}
				else
				{
					token.number = tplus;
					token.value.id[0] = '+';
					token.value.id[1] = '\0';
					ungetc(ch, source_file); // retract
				}
				break;
			case '-': // state 32
				ch = fgetc(source_file);
				if (ch == '-')
				{
					token.number = tdec;
					token.value.id[0] = '-';
					token.value.id[1] = '-';
					token.value.id[2] = '\0';
				}
				else if (ch == '=')
				{
					token.number = tsubAssign;
					token.value.id[0] = '-';
					token.value.id[1] = '=';
					token.value.id[2] = '\0';
				}
				else
				{
					token.number = tminus;
					token.value.id[0] = '-';
					token.value.id[1] = '\0';
					ungetc(ch, source_file); // retract
				}
				break;
			case '<': // state 36
				ch = fgetc(source_file);
				if (ch == '=')
				{
					token.number = tlesse;
					token.value.id[0] = '<';
					token.value.id[1] = '=';
					token.value.id[2] = '\0';
				}
				else
				{
					token.number = tless;
					token.value.id[0] = '<';
					token.value.id[1] = '\0';
					ungetc(ch, source_file); // retract
				}
				break;
			case '=': // state 39
				ch = fgetc(source_file);
				if (ch == '=')
				{
					token.number = tequal;
					token.value.id[0] = '=';
					token.value.id[1] = '=';
					token.value.id[2] = '\0';
				}
				else
				{
					token.number = tassgin;
					token.value.id[0] = '=';
					token.value.id[1] = '\0';
					ungetc(ch, source_file); // retract
				}
				break;
			case '>': // state 42
				ch = fgetc(source_file);
				if (ch == '=')
				{
					token.number = tgreate;
					token.value.id[0] = '>';
					token.value.id[1] = '=';
					token.value.id[2] = '\0';
				}
				else
				{
					token.number = tgreat;
					token.value.id[0] = '>';
					token.value.id[1] = '\0';
					ungetc(ch, source_file); // retract
				}
				break;
			case '|': // state 45
				ch = fgetc(source_file);
				if (ch == '|')
				{
					token.number = tor;
					token.value.id[0] = '|';
					token.value.id[1] = '|';
					token.value.id[2] = '\0';
				}
				else
				{
					lexicalError(3);
					ungetc(ch, source_file); // retract
				}
				break;
			case '(':
				token.number = tlparen;
				token.value.id[0] = '(';
				token.value.id[1] = '\0';
				break;
			case ')':
				token.number = trparen;
				token.value.id[0] = ')';
				token.value.id[1] = '\0';
				break;
			case ',':
				token.number = tcomma;
				token.value.id[0] = ',';
				token.value.id[1] = '\0';
				break;
			case ';':
				token.number = tsemicolon;
				token.value.id[0] = ';';
				token.value.id[1] = '\0';
				break;
			case '[':
				token.number = tlbracket;
				token.value.id[0] = '[';
				token.value.id[1] = '\0';
				break;
			case ']':
				token.number = trbracket;
				token.value.id[0] = ']';
				token.value.id[1] = '\0';
				break;
			case '{':
				token.number = tlbrace;
				token.value.id[0] = '{';
				token.value.id[1] = '\0';
				break;
			case '}':
				token.number = trbrace;
				token.value.id[0] = '}';
				token.value.id[1] = '\0';
				break;
			case EOF:
				token.number = teof;
				break;
			default:
			{
				printf("Current character: %c", ch);
				lexicalError(4);
				break;
			}
			} // switch end
	} while (token.number == tnull);
	return token;
} // end of scanner

int main(void)
{
	tokenType token;
	FILE *source_file;
	token.number = tnull;

	if ((source_file = fopen("prime.mc", "r")) == NULL)
	{
		printf("File open error!\n");
	}
	while (token.number != teof)
	{
		token = scanner(source_file);
		if (token.number == tnumber) // 토큰이 상수일 때
			printf("Token: %d : (%d, %d)\n", token.value.num, token.number, token.value.num);
		else if (token.number == tident) // 토큰이 명칭일 때
			printf("Token: %s : (%d, %s)\n", token.value.id, token.number, token.value.id);
		else if (token.number == teof)
			break;
		else // 토큰이 명령어, 연산자 일때
			printf("Token: %s : (%d, %d)\n", token.value.id, token.number, 0);
	}
	fgetc(source_file);
	fclose(source_file);
}
