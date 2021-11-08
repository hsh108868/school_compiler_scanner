#include <stdio.h>
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

typedef struct s_tokenType { /* 파서에게 넘겨주는 구조체 == 스캐너의 출력 */
	int number; /* 토큰 번호 */
	union {
		char id[ID_LENGTH];
		int num;
	} value; /* 토큰 벨류 */
}	t_tokenType;

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

void	scanner(FILE *fp, t_tokenType *token)
{
	
}

int	main(int argc, char *argv[])
{
	int	i;
	t_tokenType token;
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
