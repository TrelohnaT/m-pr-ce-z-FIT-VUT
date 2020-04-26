/* ----------------------------------------------
	Předmět: IFJ (+ IAL)
	Tým: 107 var. I
	Autoři: Vl454k0v4, N3l4 (xvlasa14)
		  	J4k51k, 4l35 (xjaksi01)
			D0l3č3k, V4cl4v (xdolec03)
			P05p1511, F1l1p (xpospi0f)
-------------------------------------------------
 Datum: 23. 11. 2019
 Účel: GENERATOR MADAFAKA KÓDU
 Popis: WIP
 ------------------------------------------------
			(c) UniSoftCorn
 ----------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lex_anal.h"



/*toto bude vypisovat jednotlivé příkazy
** něco jako mozek generatoru
** podle toho co mu příjde rozhodne, co se má vypisovat
*/
static int focusFlag = 0;

static int fisrtFlag = 0;

static int ifCounter = 0;

static int onlyOnce = 0;

int errorCaller = 0;

bool localTab = false;

void header(){
    printf(".IFJcode19\n");
}

Token *token;

char handyField[50];

int generator_caller(Token *token)
{
    //handyField = (char *) malloc(50*sizeof(char));
    strcpy(token->str, handyField);

    if(onlyOnce == 0)
    {
        
        header();
        onlyOnce++;
    }
    if(fisrtFlag == 0)
    {
        /*rozhodne co se bude dít tak, že přepíše focusFlag*/
        errorCaller = generator_brain(token);
        fisrtFlag = 1;
    }
    if(token->tokenType == TTEol)
    {
        //printf("I was here\n");
        fisrtFlag = 0;
    }
   // printf("I was here\n");
    switch (focusFlag)
    {
        /*funkce print*/
    case 30:
        return inFooPrint(token);
    break;

    case 31:

    break;

    case 32:

    break;

    case 33:

    break;

    /*funkce len*/
    case 34:
        return inFooLen(token);
    break;

    case 35:

    break;

    /*funkce ord*/
    case 36:
        return inFooOrd(token);
    break;

    case 37:

    break;
    /*přikaz def*/
    case 160:
        return orderDef(token);
    break;

    case 162:
        
        
    break;

    case 200:
    /*přiřazeni (Assignment)*/
        if(token->tokenType == TTass)
        {   
            return orderAss(token);
        }
    break;
        
    default:
    break;
    }
}

int generator_brain(Token *token)
{   
    
    if(token->tokenType == TTkeyWord)
    {
        /*zde se rozhodne, jaké klíčové slovo se 
        **bude obsluhovat nastavením focusFlagu
        **pro další volání této funkce
        **16 = TTKeyWord 0-7 = KW(konkretní)*/
        if(token->keyWord == KWdef)
        {
            focusFlag = 160;
        }
        else if(token->keyWord == KWif)
        {
            focusFlag = 162;
        }
        else if(token->keyWord == KWelse)
        {
            focusFlag = 161;
        }
        else if(token->keyWord == KWnone)
        {
            focusFlag = 163;
        }
        else if(token->keyWord == KWpass)
        {
            focusFlag = 164;
        }
        else if(token->keyWord == KWreturn)
        {
            focusFlag = 165;
        }
        else if(token->keyWord == KWwhile)
        {
            focusFlag = 166;
        }
        else if(token->keyWord == KWbreak)
        {
            focusFlag = 167;
        }    
    }
    else if(token->tokenType == TTidentifier)
    {
        /*stejný princim focusFlagu jako předtím
        **0-26tokenType 8 = KWnotKW
        **26-50 zapuštěné funkce*/
        if(varStringCmp(token, "print") == 0)
        {
           // printf("I was in print\n");
            focusFlag = 30;
            return NOERROR_OK;
        }
        else if(varStringCmp(token, "inputs") == 0)
        {
            focusFlag = 31;
            return NOERROR_OK;
        }
        else if(varStringCmp(token, "inputi") == 0)
        {
            focusFlag = 32;
            return NOERROR_OK;
        }
        else if(varStringCmp(token, "inputf") == 0)
        {
            focusFlag = 33;
            return NOERROR_OK;
        }
        else if(varStringCmp(token, "len") == 0)
        {
            focusFlag = 34;
            return NOERROR_OK;
        }
        else if(varStringCmp(token, "substr") == 0)
        {
            focusFlag = 35;
            return NOERROR_OK;
        }
        else if(varStringCmp(token, "ord") == 0)
        {
            focusFlag = 36;
            return NOERROR_OK;
        }
        else if(varStringCmp(token, "chr") == 0)
        {
            focusFlag = 37;
            return NOERROR_OK;
        }
        else
        {
            focusFlag = 200;
            return NOERROR_OK;
        }  
    }   
}
/*zde jsou deklarovány funkce*/

/*tato obsatarává operátor přiřazení*/
int orderAss(Token *token)
{
    printf("DEFVAR %s \n", handyField);
    if(token->tokenType == TTass)
    {
        printf("MOVE %s %s\n", handyField, token->str);
    }
    fisrtFlag = 0;
    
}
/*pro klíčové slovo def*/
int orderDef(Token *token)
{
    if(token->tokenType == TTidentifier)
    {
        if(varStringCmp(token, "def") == 0)
        {
            
        }
    }
}
/*pri řanořenou funkci print*/
int inFooPrint(Token *token)
{   
    if(token->tokenType == TTidentifier)
    {   
        //printf("I am in print\n");
        if(varStringCmp(token, "print") != 0)
        {
            printf("%s ", token->str);
            return NOERROR_OK;
        }
    }
    else if(token->tokenType == TTbracketRight)
    {
        printf("\n");
        return NOERROR_OK;
    }
    return ERROR_COMPILATOR;
}
/*pro řanořenou funkci ord*/
int inFooOrd(Token *token)
{
    if(token->tokenType == TTidentifier)
    {   
        //printf("I am in print\n");
        if(varStringCmp(token, "ord") != 0)
        {
            printf("%d ", token->str[0]);
            return NOERROR_OK;
        }
    }
    else if(token->tokenType == TTbracketRight)
    {
        printf("\n");
        return NOERROR_OK;
    }
    return ERROR_COMPILATOR;
}
/*pro řanořenou funkci len*/
int inFooLen(Token *token)
{
    if(token->lenght != 0)
    {
        printf("%d \n", token->lenght);
        return NOERROR_OK;
    }
    else if(token->tokenType == TTbracketRight)
    {
        printf("\n");
        return NOERROR_OK;
    }
    return ERROR_COMPILATOR;
    
}

