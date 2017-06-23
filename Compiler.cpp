//sriram
//compiler design

#include<stdio.h>
#include<io.h>
#include<stdlib.h>
#include<string.h>
static int address = 0, index = 0,Lindex=0,Iindex=1,stop=-1;
void printLabelTable(struct LabelTable *labelTable);
int getLabelAddress(struct LabelTable *label_Table, char* str);

struct LabelTable
{
	char name[10];
	int Address;
};
struct symbolTable
{
	char name[100];
	int Address;
	int size;
};

struct opcode
{
	char ins[6];
	int op, nop;
};

struct opcode opc[14] = { { "MOV", 1, 2 }, { "MOV", 2, 2 }, { "ADD", 3, 3 }, { "SUB", 4, 3 }, { "MUL", 5, 3 }, { "JMP", 6, 1 },
{ "IF", 7, 4 }, { "EQ", 8, 0 }, { "LT", 9, 0 }, { "GT", 10, 0 }, { "LTEQ", 11, 0 }, { "GTEQ", 12, 0 }, { "PRINT", 13, 1 }, { "READ", 14, 1 } };

struct IntermediateTable
{
	int insNo;
	int op;
	int parameters[4];
};

void push(int*stack, int num)
{
	stack[++stop] = num;
}

char *getInst(char *str)
{
	int i;
	char insc[10];
	for (i = 0; str[i] != ' '; i++)
	{
		insc[i] = str[i];
	}
	insc[i] = '\0';
	return insc;
}


char *getToken(char *str, int *i)
{
	int j = *i, index = 0;
	char *token = (char *)malloc(sizeof(char) * 20);

	for (; (str[j] == ' ' || str[j] == ',' || str[j] == '\t') && str[j]; j++);
	for (; str[j] != ' ' && str[j] != ',' && str[j] != '\n' && str[j]; j++) token[index++] = str[j];
	token[index] = '\0';
	*i = j;
	return token;
}


int getparams(int op)
{
	for (int j = 0; j < 14; j++)
	{
		if (op == opc[j].op)
			return opc[j].nop;
	}
	return -1;
}


int getNum(char* str, int start,int end)
{
	int prod = 0;
	for (int k = start; k < end; k++)
	{
		int r = str[k] - '0';
		prod = prod * 10 + r;

	}
	return prod;
}

int getOpcode(char* str)
{
	int i;
	for (i = 0; i < 14; i++)
	{
		
		if (strcmp(opc[i].ins, str) == 0)
			return opc[i].op;
	}
}


int checkParams(char *str)
{
	int i;
	for (i = 0; i < 14; i++)
	{
		if (strcmp(opc[i].ins, str) == 0)
			return opc[i].nop;
	}
}


void CreateSymbolTable(struct symbolTable *s, char *name, int size)
{
	strcpy(s[index].name, name);
	s[index].size = size;
	s[index].Address = address;
	address = address + size;
	index++;
}


int getSymbolAddress(char* str,struct symbolTable *s)
{
	
	for (int i = 0; i < address; i++)
	{
		if (strlen(str) == 1){
			if (strcmp(str, s[i].name) == 0)
			{
					return s[i].Address;
			}
		}
		else
		{
			if (str[0] == s[i].name[0])
			{
				int no = getNum(str, 2, strlen(str) - 1);
				return s[i].Address + no;
			}
		}
	}
}

void displaySymbolTable(struct symbolTable *s)
{
	printf("Symbol Table:\nName\tAddress\tSize\n");
	for (int i = 0; i < index; i++)
	{
		if (strlen(s[i].name)>1)
			printf("%c\t", s[i].name[0]);
		else
			printf("%s\t", s[i].name);
		printf("%d\n",  s[i].size);
	}
}

void printIntermediateTable(struct IntermediateTable *intermediateTable, int index)
{
	printf("Intermediate Table:\nInsNo\topcode\tparameters\n");
	for (int ind = 1; ind <= index; ind++){
		printf("%d\t", intermediateTable[ind].insNo);
		printf("%d\t", intermediateTable[ind].op);
		int nop = getparams(intermediateTable[ind].op);

		for (int i = 0; i < nop; i++)
		{
			printf("%d\t", intermediateTable[ind].parameters[i]);
		}
		printf("\n");
	}
}


void loadFile()
{
	struct symbolTable s[100];
	struct IntermediateTable intermediateTable[100];
	struct LabelTable label_Table[30];
	//fflush(stdin);
	int stack[100];
	int MemoryTable[100];
	FILE *fps=NULL;
	char *result;
	
	char expression[20];
	fps = fopen("CompilerInput.txt", "r");
	//fseek(fps, 0, SEEK_END);
	
	int l = ftell(fps);

	printf("%d\n", l);
	
	while (fgets(expression, sizeof(expression), fps))
	{
		int i = 0; //inst_no++;
		
		expression[strlen(expression) - 1] = '\0';
		printf("exp==%s\n", expression);
		while (expression[i] != '\0')
		{
			result=getToken(expression, &i);
			printf("rs=%s\n", result);
				if (strcmp(result, "DATA") == 0)
				{
					result = getToken(expression, &i);
					printf("%s\n", result);
					if (strlen(result) > 1)//array variable
					{
						
						int prod =getNum(result, 2, strlen(result) - 1);
						CreateSymbolTable(s , result, prod);
					}
					else	
						CreateSymbolTable(s, result, strlen(result));
				}
				else if (strcmp(result, "CONST") == 0)
				{
					result = getToken(expression, &i);
					printf("%s\n", result);
					CreateSymbolTable(s, result, 0);
					result = getToken(expression, &i);
					result = getToken(expression, &i);
					printf("%s\n", result);
					int value = getNum(result, 0,strlen(result));
				}
				else if (strcmp(result, "START:") == 0)
				{
					continue;
				}
				else if (result[1] == ':')//Label
				{
					label_Table[Lindex].Address = Iindex;
					label_Table[Lindex].name[0]=result[0];
					label_Table[Lindex].name[1] = '\0';
					Lindex++;
				}
				else if (strcmp(result, "IF") == 0)
				{
		
					intermediateTable[Iindex].insNo = Iindex;
					intermediateTable[Iindex].op = 7;
						result = getToken(expression, &i);
						printf("%s\n", result);
						intermediateTable[Iindex].parameters[0] = result[0] - 'A';
						
						//param2 opertor
						result = getToken(expression, &i);
						printf("%s\n", result);
						intermediateTable[Iindex].parameters[1]=getOpcode(result);
			
						//param3
						result = getToken(expression, &i);
						printf("%s\n", result);
							intermediateTable[Iindex].parameters[2] = result[0] - 'A';
							
						
						result = getToken(expression, &i);
					intermediateTable[Iindex].parameters[3] = -1;
					
					push(stack, intermediateTable[Iindex].insNo);
					Iindex++;
				
				}
				else if (strcmp(result, "ELSE") == 0)
				{
					intermediateTable[Iindex].insNo = Iindex; //add ins no
					intermediateTable[Iindex].op = 6;		  
					intermediateTable[Iindex].parameters[0] = -1; //in place of * assign -1

					push(stack, Iindex);   //push  ins no value to stack
					Iindex++;
					strcpy(label_Table[Lindex].name,result); ///copy name to  label table 
					label_Table[Lindex].Address = Iindex;
					Lindex++;
				}
				else if (strcmp(result, "ENDIF") == 0)
				{
					strcpy(label_Table[Lindex].name, "ENDIF");
					label_Table[Lindex].Address = Iindex;
					
					int m = Lindex;
					intermediateTable[stack[stop--]].parameters[0] = label_Table[m].Address;//get top value and assign it to else
					intermediateTable[stack[stop--]].parameters[3] = label_Table[m - 1].Address;//top value and assign it to if

				}
				else if (strcmp("JMP", result) == 0)
				{
					result = getToken(expression, &i);
					intermediateTable[Iindex].insNo = Iindex;
					intermediateTable[Iindex].op = 6;
					intermediateTable[Iindex].parameters[0] = getLabelAddress(label_Table, result);//get address of label and assign it to paraaetr
					Iindex++;
				}
				else if (strcmp("END", result) == 0)
				{
					fclose(fps);
					printf("reached end\n");
					break;
				}
				else if (result != "JMP" && result != "END")
				{
					//initIntermdiateTable(intermediateTable);
					int nop = checkParams(result);
					int opno;
					if (strcmp(result, "MOV") == 0)
					{
						if (expression[strlen(expression) - 1] == 'X')
						{
							opno = 1;
						}
						else
							opno = 2;
					}
					else
						opno = getOpcode(result);
					intermediateTable[Iindex].insNo = Iindex;
					intermediateTable[Iindex].op = opno;
					for(int l = 0; l < nop; l++)
					{
						result = getToken(expression, &i);
				
					   if (result[1] == 'X')
						{
							intermediateTable[Iindex].parameters[l] = result[0] - 'A';
						}
					   else
					   {
						   int symadd = getSymbolAddress(result, s);
						   intermediateTable[Iindex].parameters[l] = symadd;// printf("index=%d\n", Iindex);
					   }
					}
					
					Iindex++;
					printf("Iindex %d", Iindex);
				}
				
		}
		
	}
	displaySymbolTable(s);
	printLabelTable(label_Table);
	printIntermediateTable(intermediateTable, Iindex-1);
	fclose(fps);
}
int getLabelAddress(struct LabelTable *label_Table,char* x)
{
	for (int i = 0; i < Lindex; i++)
	{
		if (strcmp(x, label_Table[i].name)==0)
		{
			return label_Table[i].Address;
		}
	}
}

void printLabelTable(struct LabelTable *labelTable)
{
	printf("Label Table:\nName\tAddress\n");
	for (int i = 0; i < Lindex; i++)
	{
		printf("%s\t", labelTable[i].name);
		printf("%d\n", labelTable[i].Address);
	}
}

int main()
{
	loadFile();
	getchar();
	return 0;
}

/*

DATA B
DATA A
DATA C[4]
DATA D
CONST E = 0
START:
READ AX
READ BX
MOV A, AX
MOV B, BX
ADD CX, AX, BX
X:
READ AX
SUB DX, AX, BX
PRINT DX
PRINT CX
IF CX EQ DX THEN
MOV C[0], CX
PRINT C[0]
ELSE
MOV C[1], DX
PRINT C[1]
JUMP X
PRINT E
END

*/