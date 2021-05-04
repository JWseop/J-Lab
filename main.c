#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 100

typedef int element;

typedef struct {                            //Data structure of Stack
element data[MAX_STACK_SIZE];
int top;
} STACK;

void init_stack(STACK *s)                   //Stack Initializer
{
s->top = -1;
}

int is_empty(STACK *s)                      //See if Stack is empty
{
return (s->top == -1);
}

int is_full(STACK *s)                       //See if Stack is full
{
return (s->top == (MAX_STACK_SIZE - 1));
}

void push(STACK *s, element item) {         //push data in Stack
if (is_full(s)) {
fprintf(stderr, "Error:Stack is full\n");
return;
}
else s->data[++(s->top)] = item;
}

element pop(STACK *s) {                     //pop data out from Stack
if (is_empty(s)) {
fprintf(stderr, "Error:Stack is empty\n");
exit(1);
}
else return s->data[(s->top)--];
}

int postfix_operator(char input[])          //operates postfix expressions
{
    int num1, num2, value, i=0;
    int len = strlen(input); // Length of input expression
    char char_passer;
    STACK s;

    init_stack(&s); // initialize declared STACK s
    for( i=0; i<len; i++){
        char_passer = input[i];
        if( char_passer != '+' && char_passer != '-' && char_passer != '*'){
            value = char_passer - '0'; // if input data is integer
            push(&s, value);           // push to STACK
        }
        else // if input data is operator
        {
            num2 = pop(&s);
            num1 = pop(&s);
            switch(char_passer){ // operate with two LastIn data from Stack, and push the result back
                case '+': push(&s,num1+num2);
                    break;
                case '-': push(&s,num1-num2);
                    break;
                case '*': push(&s,num1*num2);
                    break;
            }
    
        }
    }
    return pop(&s); // After every operation, pop the result of expression
}

void pass_result(int result_num,int *result,int result_arr[])
{
    result_arr[result_num]=(*result);
}

void return_result(int result_arr[],int how_many)
{
    FILE *of = fopen("/Users/jungwooseop/모시공/수업자료/자료구조/HW2/output-1.txt","w");
    for(int i=0;i<how_many;i++)
    {
    fprintf(of, "%d\n",result_arr[i]);
    }
    fclose(of);
}

int main(void)
{
    int length;             //check the length of input expression
    char temp[100]={0};     //input data here
    int how_many_times;     //the number of input expression
    int result;             //result of expression
    FILE *fp = fopen("/Users/jungwooseop/모시공/수업자료/자료구조/HW2/input-1.txt","r");
        fscanf(fp, "%d", &how_many_times);
    int *result_arr=(int*)malloc(sizeof(int)*how_many_times);
    
        for(int i=0;i<how_many_times;i++)
        {
            memset(temp, 0, sizeof(char)*100);          //reset temp[]
            fscanf(fp, "%s", temp);                     //pass postfix to temp[]
            length = strlen(temp);
            char* input=malloc(sizeof(char)*length);    //allocate heep memory as long as input data
            input = temp;
            result=postfix_operator(input);             //operate the expression
            pass_result(i, &result, result_arr);        //pass the result
        }
    return_result(result_arr, how_many_times);          //return result in txt file
    free(result_arr);                                   //free result heep data
    return 0;
}
