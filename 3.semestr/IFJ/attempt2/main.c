/*Projekt IFJ compiler z IFJ19 do IFJcode19*/
/*----------Vyrobil: UniSoftCorn-----------*/
/*členové vývojového týmu:
** Václav Doleček
** Aleš Jakošík
** Filip Pospíšil
** Nela Vlasáková
   
** Autor:
   Václav Doleček

   Motivace:
** Udělat tento předmět */

// Pokus: 2


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lex_anal.h"

void print_token(Token *token);

int main()
{
    
    Token *token;
/*kontrola argumentů*/
    int error_caller = 0;
    token = (struct itemToken*) malloc(sizeof(struct itemToken));
    //token->string = var_string;
    //varString *var_string;
    while(token->tokenType != 26 && error_caller != 1)
    {
        error_caller = getToken(token);
    //    printf("\n");    
    //    printf("    error_caller = %d    <---\n", error_caller);
    //    printf("    token type %d       <---\n", token->tokenType);
    //    printf("    Key Word %d          <---\n", token->keyWord);
    //    printf("    string: %s          <---\n\n", token->str);
        generator_caller(token);
    //    printf("\n");
    //printf("Print varString in main: %s\n", token->string->str);
    }

    return 0;
    
}
