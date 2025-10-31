#include <stdio.h>
#include <stdlib.h> /* для atof() */

#include <windows.h>

#define MAXOP 100 /* макс. размер операнда или оператора */
#define NUMBER '0' /* признак числа */

int getop (char []);
void push (double);
double pop (void);
void print_last(void);
void replace(void);
void doubler(void);
void clear(void);

/* калькулятор с обратной польской записью */
int main ()
{
	// Переключаем консоль в UTF-8
    SetConsoleOutputCP(65001); // 65001 = UTF-8
    SetConsoleCP(65001);
	
	int type;
	double op2, op1;
	char s[MAXOP];
	while ((type = getop (s)) != EOF) {
		switch (type) {
		case NUMBER:
			push (atof (s));
			break;
		case '+':
			push (pop() + pop());
			break;
		case '*':
			push (pop() * pop());
			break;
		case '-':
			op2 = pop();
			push (pop() - op2);
			break;
		case '/' :
			op2 = pop();
			if (op2 != 0.0)
				push (pop() / op2);
			else
				printf("ошибка: деление на нуль\n");
			break;
		case '%':
			op2 = pop();
			op1 = pop();
			if (op2 != 0.0 && (int)op2==op2 && (int)op1==op1)
				push ((int)op1 % (int)op2);
			else if (op2 == 0.0)
				printf("ошибка: взятия остатка от делния на нуль\n");
			else
				printf("ошибка: числа дробные\n");
			break;
        case 'P': // печать последнего
            print_last();
            break;
        case 'R': // менять местами два верхних элемента 
            replace();
            break;
        case 'D': // дублировать элемент в стеке
            doubler();
            break;
        case 'C':
            clear();
            break;
		case '\n' :
			printf("\t%.8g\n", pop());
			break;
		default:
			printf("ошибка: неизвестная операция %s\n", s);
			break;
		}
	}
	return 0;
}

#define MAXVAL 100 /* максимальная глубина стека */

int sp = 0; /* следующая свободная позиция в стеке */
double val[ MAXVAL ]; /* стек */

/* push: положить значение f в стек */
void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("ошибка: стек полон, %g не помещается\n", f);
}

/* pop: взять с вершины стека и выдать в качестве результата */
double pop(void)
{
	if (sp > 0)
		return val[--sp];
	else {
		printf("ошибка: стек пуст\n");
		return 0.0;
	}
}

#include <ctype.h>

int getch(void);
void ungetch(int);

/* getop: получает следующий оператор или операнд */
int getop(char s[])
{
	int i, c, c2;
	while ((s[0] = c = getch()) == ' ' || c == '\t' )
		;
	s[1] = '\0';
	
	i = 0;
	if (c =='-'){
		c2 = getch();
		
		if (isdigit(c2)){
			s[i] = c;
			c = c2;
		}
		ungetch(c2);
	}
	if (!isdigit(c) && c != '.')
		return c; /* не число */

	if (isdigit(c)) /* накапливаем целую часть */
		while (isdigit(s[++i] = c = getch())) /* мы сохраняем цифру, начиная со следующей 
									        потому что в s[0] уже есть первая цифра */
			;
	if (c == '.') /* накапливаем дробную часть */
		while (isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';
	if (c != EOF)
		ungetch(c);
	return NUMBER;
}

#define BUFSIZE 100

char buf[BUFSIZE]; /* буфер для ungetch */
int bufp = 0; /* след, свободная позиция в буфере */

int getch(void) /* взять (возможно возвращенный) символ */
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /* вернуть символ на ввод */
{
	if (bufp >= BUFSIZE)
		printf ("ungetch: слишком много символов\n");
	else
		buf[bufp++] = c;
}

void print_last(void)
{
    if (sp > 0)
        printf("%f", val[sp-1]);
    else
        printf("Нет элемента в стеке\n");
}

void doubler(void)
{
    int op = pop();
    push(op);
    push(op);
}

void replace(void)
{
    float op_1, op_2;
    op_1 = pop();
    op_2 = pop();
    push(op_1);
    push(op_2);
}

void clear(void)
{
    memset(val, 0, sizeof(val));
    sp = 0;
}