/*Projekt IFJ compiler z IFJ19 do IFJcode19*/
/*----------Vyrobil: UniSoftCorn-----------*/
/*členové vývojového týmu:
** Václav Doleček
** Aleš Jakošík
** Filip 
** Nela Vlasáková
   
   Motivace:
** Udělat tento předmět */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lex_anal.h"

int errorCaller;

void Error()
{
    errorCaller = 1;
    return;
}


/*inicializace ukazatele na token*/
void tokenInit(pointerToken *pt)
{
    if(pt != NULL)
    {
        pt->Act = NULL;
    }
}

void tokenMake(pointerToken *pt, tTokenType TT, tKeyWord KW, char str, int iNum, double dNum)
{
    Token token;
    if(pt == NULL)
    {
        token = (struct tToken*) malloc(sizeof(struct tToken));
    }
    else
    {
        pt->Act = token;
    }
    

    if(token != NULL)
    {
        token->tokenType = TT;
        token->keyWord = KW;
        token->string = str;
        token->intNum = iNum;
        token->doubleNum = dNum;
    }
    else
    {
        Error();
    }
    
}

void tokenDelete(pointerToken *pt)
{
    if(pt != NULL)
    {
        Token token;

        token = pt->Act;

        free(token);

        token = NULL;
    }
}
