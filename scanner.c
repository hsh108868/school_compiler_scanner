#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum tsymbol { tnull = -1, /* 토큰들의 토큰 번호 */
	tnot, tnotequ, tmod, tmodAssign, tident, tnumber, /* 0 1 2 3 4 5*/
	tand, tlparen, trparen, tmul, tmulAssign, tplus, /* 6 7 8 9 10 11 */
	tinc, taddAssign, tcomma, tminus, tdec, tsubAssign, /* 12 13 14 15 16 17 */
	tdiv, tdivAssign, tsemicolon, tless, tlesse, tassgin, /* 18 19 20 21 22 23 */
	tequal, tgreat, tgreate, tlbracket, trbracket, teof, /* 24 25 26 27 28 29 */
	/* word symbols */
	tconst, telse, tif, tint, treturn, tvoid, /* 30 31 32 33 34 35 */
	twhile, tlbrace, tor, trbrace /* 36 37 38 39 */
};

#define NO_KEYWORDS 7 /* 단어 심벌의 개수 */
#define ID_LENGTH 12 /* 유효한 길이를 정의하는 상수 */

typedef struct tokenType { /* 파서에게 넘겨주는 구조체 == 스캐너의 출력 */
	int number; /* 토큰 번호 */
	union {
		char id[ID_LENGTH];
		int num;
	} value; /* 토큰 벨류 */
}	tokenType;

/* 각 지정어의 스트링 값을 갖는 배열 */
char	*keyword[NO_KEYWORDS] = {
	"const", "else", "if", "int", "return", "void", "while"
};

/* 각 지정어에 해당하는 토큰 번호를 갖는 배열 */
enum tsymbol tnum[NO_KEYWORDS] = {
	tconst, telse, tif, tint, treturn, tvoid, twhile
};

void	lexicalError(int n)
{
	printf("*** Lexical Error : ");
	switch (n)
	{
		case 1: printf("an identifiler length must be less than 12.\n");
			break ;
		case 2: printf("next character must be &.\n");
			break ;
		case 3: printf("next character must be |.\n");
			break ;
		case 4: printf("invalid character!!!\n");
			break ;
	}
}

int	superLetter(char ch)
{
	if (isalpha(ch) || ch == '_')
		return (1);
	else
		return (0);
}

int	superLetterOrDigit(char ch)
{
	if (isalnum(ch) || ch == '_')
		return (1);
	else
		return (0);
}

struct tokenType scanner()
{
	struct tokenType token;
	int i, index;
	char ch, id[ID_LENGTH];

	token.number = tnull;
	do
	{
		while (isspace(ch = getchar())) ; // state 1: skip blanks
		if (superLetter(ch)) // identifier or keyword
		{
			i = 0;
			do
			{
				if (i < ID_LENGTH)
					id[i++] = ch;
				ch = getchar();
			} while (superLetterOrDigit(ch));
			if (i >= ID_LENGTH)
				lexicalError(1);
			id[i] = '\0';
			ungetc(ch, stdin); // retract
			// find the identifier in the keyword table
			for (index = 0; index < NO_KEYWORDS; index++)
			{
				if (!strcmp(id, keyword[index]))
					break ;
			}
			if (index < NO_KEYWORDS) // found, keyword exit
				token.number = tnum[index];
			else
			{
				token.number = tident;
				strcpy(token.value.id, id);
			}
		} // end of identifier or keyword
		else if (isdigit(ch)) // integer constant
		{
			token.number = tnumber;
			token.value.num = getIntNum(ch); // 해당 함수가 라이브러리에 없어용
		}
		else switch (ch) // special character
		{
			case '/': // state 10
				ch = getchar();
				if (ch == '*') // text comment
				{
					do
					{
						while (ch != '*')
							ch = getchar();
						ch = getchar();
					} while (ch != '/');
				}
				else if (ch == '/') // line comment
					while (getchar() != '\n')
						;
				else if (ch == '=')
					token.number = tdivAssign;
				else
				{
					token.number = tdiv;
					ungetc(ch, stdin); // retract
				}
				break ;
			case '!': // state 17
				ch = getchar();
				if (ch == '=')
					token.number = tnotequ;
				else
				{
					token.number = tnot;
					ungetc(ch, stdin); // retract
				}
				break ;
			case '%': // state 20
				ch = getchar();
				if (ch == '=')
					token.number = tmodAssign;
				else
				{
					token.number = tmod;
					ungetc(ch, stdin);
				}
				break ;
			case '&': // state 23
				ch = getchar();
				if (ch == '&')
					token.number = tand;
				else
				{
					lexicalError(2);
					ungetc(ch, stdin); // retract
				}
				break ;
			case '*': // state 25
				ch = getchar();
				if (ch == '=')
					token.number = tmulAssign;
				else
				{
					token.number = tmul;
					ungetc(ch, stdin); // retract
				}
				break ;
			case '+': // state 28
				ch = getchar();
				if (ch == '+')
					token.number = tinc;
				else if (ch == '=')
					token.number = taddAssign;
				else
				{
					token.number = tplus;
					ungetc(ch , stdin); // retract
				}
				break ;
			case '-': // state 32
				ch = getchar();
				if (ch == '-')
					token.number = tdec;
				else if (ch == '=')
					token.number = tsubAssign;
				else
				{
					token.number = tminus;
					ungetc(ch, stdin); // retract
				}
				break ;
			case '<': // state 36
				ch = getchar();
				if (ch == '=')
					token.number = tlesse;
				else
				{
					token.number = tless;
					ungetc(ch, stdin); // retract
				}
				break ;
			case '=': // state 39
				ch = getchar();
				if (ch == '=')
					token.number = tequal;
				else
				{
					token.number = tassgin;
					ungetc(ch , stdin); // retract
				}
				break ;
			case '>': // state 42
				ch = getchar();
				if (ch == '=')
					token.number = tgreate;
				else
				{
					token.number = tgreat;
					ungetc(ch, stdin); // retract
				}
				break ;
			case '|': // state 45
				ch = getchar();
				if (ch == '|')
					token.number = tor;
				else
				{
					lexicalError(3);
					ungetc(ch, stdin); // retract
				}
				break ;
			case '(':
				token.number = tlparen;
				break ;
			case ')':
				token.number = trparen;
				break ;
			case ',':
				token.number = tcomma;
				break ;
			case ';':
				token.number = tsemicolon;
				break ;
			case '[':
				token.number = tlbracket;
				break ;
			case ']':
				token.number = trbracket;
				break ;
			case '{':
				token.number = tlbrace;
				break ;
			case '}':
				token.number = trbrace;
				break ;
			case EOF:
				token.number = teof;
				break ;
			default:
			{
				printf("Current character: %c", ch);
				lexicalError(4);
				break ;
			}
		} // switch end
	} while (token.number == tnull);
	return token;
} // end of scanner

int	main(int argc, char *argv[])
{
	int	i;
	tokenType token;
	FILE *fp;

	if ((fp = fopen("prime.mc", "r")) == NULL)
	{
		printf("File open error!\n");
		exit(0);
	}
	i = 0;
	while (token.number != teof)
	{
		scanner(fp, &token);
		if (token.number == tnumber) /* 토큰이 상수일 때 */
			printf("Token: %12d:(%2d, %7d)\n", token.value.num, token.number, token.value.num);
		else if (token.number == tident) /* 토큰이 명칭일 때 */
			printf("Token: %12s:(%2d, %7s)\n", token.value.id, token.number, token.value.id);
		else if (token.number == teof)
			break ;
		else
			printf("Token: %12s:(%2d, %7d)\n", token.value.id, token.number, 0);
	}
	getchar();
	fclose(fp);
	return (0);
}
