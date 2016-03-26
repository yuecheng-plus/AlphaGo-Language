#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define BUFSIZE 20 //����������
#define LENGTH 10  //���ű��б�������ֳ���
#define TABLESIZE 1000000 //���ű�Ĵ�С

char keyword[][LENGTH] =
{
	"and",
	"array",
	"begin",
	"case",
	"const",
	"div",
	"do",
	"downto",
	"else",
	"end",
	"file",
	"for",
	"function",
	"goto",
	"if",
	"in",
	"label",
	"mod",
	"nil",
	"not",
	"of",
	"or",
	"packed",
	"procedure",
	"program",
	"record",
	"repeat",
	"set",
	"then",
	"to",
	"type",
	"until",
	"var",
	"while",
	"with",
	"int",
	"real",
	"string"
};

struct token {
	char name[LENGTH];
	char value[LENGTH];
	int flag;
};
struct token SymbolTable[TABLESIZE];

char buffer[BUFSIZE] = { NULL }; //������
//int p, q; //������˫ָ�� q��ǰָ��
int p = 0;
int q = 0;
int flag = 0; //˫���������

/*
������������
1.ά��˫������
2.gtchar
3.retract
4.copytoken
*/
void InputBuffer(int p, int q)
{
	if(flag && q >= BUFSIZE / 2)
	{
		int j;
		for (j = 0; j < BUFSIZE / 2; j++)
		{
			buffer[j] = getchar();
		}
		flag = 0;
	}
	else if (!flag && p < BUFSIZE && q < BUFSIZE / 2)
	{
		int k;
		for (k = BUFSIZE / 2; k < BUFSIZE; k++)
		{
			buffer[k] = getchar();
		}
		flag = 1;
	}
}


/*
ÿ�δӻ�������ȡһ���ַ���������ǰָ�룬��黺����״̬
*/
char gtchar()
{

	char ch = buffer[q];
	q++;
	if (q >= BUFSIZE) q = q % BUFSIZE;
	return ch;
}
/*
����ǰָ�����һλ
*/
void retract(int n)
{
	q = q - 1;
	if(q < 0)
        q = BUFSIZE + q;
}

/*
��ʶ�𵽵��ַ�������ű��� �����Ԫ��Ϊ �ṹ��
*/
int InsertTable(char *name, char *value)
{
	int j = 0;
	for (; j < TABLESIZE; j++)
	{
		if (!SymbolTable[j].flag)
		{
			strcpy(SymbolTable[j].name, name);
			strcpy(SymbolTable[j].value, value);
			SymbolTable[j].flag = 1;
			printf("(%s, %s)\n", name, value);
			return 0;
		}
	}
	return -1; //����ʧ��
}

void strcp(char *s, char *t)
{
    while((*s++ = *t++) != '\0')
        ;
}

/*
����ű��в���ƥ����ַ�
���Ȳ��ҹؼ��ֱ�ȷ���Ƿ�Ϊ�ؼ��֣�����ǹؼ��ַ��عؼ����ڹؼ��ֱ��е��±�
���򷵻�-1 ��ʾ��ǰ�ַ���Ϊ��ʶ��
*/
int install(char *ch)
{
	int i;
	for (i = 0; i < 38; i++)
	{
		if (!strcmp(keyword[i], ch)) //��ǰ�ַ���Ϊ�ؼ���
		{
			if (InsertTable(keyword[i], "0"))
			{
				printf("insert keyword %s failed!\n", keyword[i]);
			}
			return i;
		}
	}
	if (InsertTable("ID", ch))
    {
        printf("insert ID %s failed!\n", ch);
    }
	return -2;
}
/*
���ƻ������п�ʼָ��p����ǰָ��q����ַ�
*/
char *copyToken()
{
	char token[BUFSIZE] = { '\0' };
	int i, j, k;
	j = 0;
	if(p < q)
    {
        for (i = p; i < q; i++)
        {
            token[j++] = buffer[i];
        }
    }
    else if(p > q)
    {
        for(i = p; i < BUFSIZE; i++)
            token[j++] = buffer[i];
        for(k = 0; k < q; k++)
            token[j++] = buffer[k];
    }
	p = q; //����ָ��
	return token;
}
/*
������ɨ���ַ������ݶ����ַ���������״̬��Ӧ��Ӧ�Ķ���
*/

void ScanToken()
{
	InputBuffer(p, q);
	char ch;
	char token[LENGTH] = {'\0'};
	 //ch = gtchar();
	while ((ch = gtchar())!= EOF)
	{
	    printf("p = %d, q = %d\n", p, q);
		while (ch == '\n' || ch == '\t' || isspace(ch))
		{
		    p++;
		    if (p >= BUFSIZE)  p = p % BUFSIZE;
			ch = gtchar();
			InputBuffer(p, q);
			continue;
		}
		if (isalpha(ch))
		{

			ch = gtchar();
			while (isalnum(ch))
				ch = gtchar();
			retract(1);
			strcp(token, copyToken());
			InputBuffer(p, q);
			install(token);
			memset(token, 0x00, sizeof(token));
		}
		else if (isdigit(ch))
		{
			ch = gtchar();
			while (isdigit(ch))
			{
				ch = gtchar();
			}
			retract(1);
			strcpy(token, copyToken());
			InputBuffer(p, q);
			if (InsertTable("INT", token))
			{
				printf("insert INT %s failed!\n", token);
			}
			memset(token, 0x00, sizeof(token));
		}
		else
		{
			switch (ch)
			{


			case '*':
				ch = gtchar();
				if (ch == '*')
				{
					retract(1);
					strcpy(token, copyToken());
					InputBuffer(p, q);
					if (InsertTable("EXP", "0"))
					{
						printf("insert EXP failed!\n");
					}
					memset(token, 0x00, sizeof(token));
				}
				break;
			case ':':
				ch = gtchar();
				if (ch == '=')
				{
					strcpy(token, copyToken());
					InputBuffer(p, q);
					if (InsertTable("ASSIGN", "0"))
					{
						printf("insert ASSIGN failed!\n");
					}
					memset(token, 0x00, sizeof(token));
				}
				else
                {
                    retract(1);
                    strcpy(token, copyToken());
					InputBuffer(p, q);
					if (InsertTable("COLON", "0"))
					{
						printf("insert ASSIGN failed!\n");
					}
					memset(token, 0x00, sizeof(token));
                }
				break;
			case '<':
				ch = gtchar();
				if (ch == '=')
				{
					strcpy(token, copyToken());
					InputBuffer(p, q);
					if (InsertTable("LE", "0"))
					{
						printf("insert LE failed!\n");
					}
					memset(token, 0x00, sizeof(token));
				}
				else if (ch == '>')
				{
					strcpy(token, copyToken());
					InputBuffer(p, q);
					if (InsertTable("NE", 0))
					{
						printf("insert NE failed!\n");
					}
					memset(token, 0x00, sizeof(token));
				}
				else
				{
					retract(1);
					strcpy(token, copyToken());
					InputBuffer(p, q);
					if (InsertTable("LT", "0"))
					{
						printf("insert LT failed!\n");
					}
					memset(token, 0x00, sizeof(token));
				}
				break;
			case '=':
				strcpy(token, copyToken());
				InputBuffer(p, q);
				if (InsertTable("EQ", "0"))
				{
					printf("insert EQ failed!\n");
				}
				memset(token, 0x00, sizeof(token));
				break;
			case '>':
				ch = gtchar();
				if (ch == '=')
				{
					strcpy(token, copyToken());
					InputBuffer(p, q);
					if (InsertTable("GE", "0"))
					{
						printf("insert GE failed!\n");
					}
					memset(token, 0x00, sizeof(token));
				}
				else
				{
					retract(1);
					strcpy(token, copyToken());
					InputBuffer(p, q);
					if (InsertTable("GT", "0"))
					{
						printf("insert GT failed!\n");
					}
					memset(token, 0x00, sizeof(token));
				}
				break;
            case '.' :
                ch = gtchar();
                if(ch == '.')
                {
                    strcpy(token, copyToken());
					InputBuffer(p, q);
					if (InsertTable("RANGE", "0"))
					{
						printf("insert RANGE failed!\n");
					}
					memset(token, 0x00, sizeof(token));
                }
                else
                {
                    retract(1);
					strcpy(token, copyToken());
					InputBuffer(p, q);
					if (InsertTable("F_STOP", "0"))
					{
						printf("insert F_STOP failed!\n");
					}
					memset(token, 0x00, sizeof(token));
                }
                break;
			case '+':
				strcpy(token, copyToken());
				InputBuffer(p, q);
				if (InsertTable("PLUS", "0"))
				{
					printf("insert PLUS failed!\n");
				}
				memset(token, 0x00, sizeof(token));
                break;
			case '-':
				strcpy(token, copyToken());
				InputBuffer(p, q);
				if (InsertTable("MINUS", "0"))
				{
					printf("insert MINUS failed!\n");
				}
				memset(token, 0x00, sizeof(token));
				break;
			case '/':
				strcpy(token, copyToken());
				InputBuffer(p, q);
				if (InsertTable("DIV", "0"))
				{
					printf("insert DIV failed!\n");
				}
				memset(token, 0x00, sizeof(token));
				break;
			case ',':
				strcpy(token, copyToken());
				InputBuffer(p, q);
				if (InsertTable("COMMA", "0"))
				{
					printf("insert COMMA failed!\n");
				}
				memset(token, 0x00, sizeof(token));
				break;
			case ';':
				strcpy(token, copyToken());
				InputBuffer(p, q);
				if (InsertTable("SEMIC", "0"))
				{
					printf("insert SEMIC failed!\n");
				}
				memset(token, 0x00, sizeof(token));
				break;
			case '(':
				strcpy(token, copyToken());
				InputBuffer(p, q);
				if (InsertTable("LR_BRAC", "0"))
				{
					printf("insert LR_BRAC", "0");
				}
				memset(token, 0x00, sizeof(token));
				break;
			case ')':
				strcpy(token, copyToken());
				InputBuffer(p, q);
				if (InsertTable("RR_BRAC", "0"))
				{
					printf("insert RR_BRAC", "0");
				}
				memset(token, 0x00, sizeof(token));
				break;
            case '[':
                strcpy(token, copyToken());
                InputBuffer(p, q);
				if (InsertTable("LS_BRAC", "0"))
				{
					printf("insert LS_BRAC", "0");
				}
				memset(token, 0x00, sizeof(token));
				break;
            case ']':
                strcpy(token, copyToken());
                InputBuffer(p, q);
				if (InsertTable("RS_BRAC", "0"))
				{
					printf("insert RS_BRAC", "0");
				}
				memset(token, 0x00, sizeof(token));
				break;
			default:
				//HandError();
				break;
			}
		}
	}
}

int main()
{
	freopen("input.pas", "r", stdin);
	//char ch;
	//ch = getchar();
	//putchar(ch);
	//init the symbol table

	memset(SymbolTable, 0x00, sizeof(SymbolTable));
	//init the buffer
	int i;
    for (i = 0; i < BUFSIZE; i++)
    {
        buffer[i] = getchar();
    }
    flag = 1;
	ScanToken();
	return 0;
}
