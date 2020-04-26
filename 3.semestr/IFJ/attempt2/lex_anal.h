/*Projekt IFJ compiler z IFJ19 do IFJcode19*/
/*----------Vyrobil: UniSoftCorn-----------*/
/*členové vývojového týmu:
** Václav Doleček
** Aleš Jakšík
** Filip Pospíšil
** Nela Vlasáková

** Autor:
   Václav Doleček (xdolec03)

   Motivace:
** Udělat tento předmět */

// Pokus: 2

/*
** Poznámky pro prográmátora (tedy pro mě ode mě)
** 1)pořadá neměň, prostě to jinak nefunguje správně
*/
//#ifndef ANAL_H
#define ANAL_H

/*zde jsou deklarovány a implementovány funkce ke kompileru*/
/*pro projekt IFJ*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "errors.h"
//#include "generator.h"
//#include "variable_string.h"
/*všechna klíčová slova která se vyskytují v Pythonu a které tedy budou použity v tokenech*/
typedef enum
{
    KWdef,      //0
    KWelse,     //1
    KWif,       //2
    KWnone,     //3
    KWpass,     //4
    KWreturn,   //5
    KWwhile,    //6
    KWbreak,    //7
    KWnotKW,    //8

} tKeyWord;

/*Všechny možnosti tokenu*/
typedef enum
{
    TTminus,        //0
    TTplus,         //1
    TTmul,          //2
    TTdiv,          //3
    TTidiv,         //4
    TTass,          //5

    TTbigger,       //6
    TTless,         //7
    TTbiggerEq,     //8
    TTlessEq,       //9
    TTeq,           //10
    TTnotEq,        //11

    TTbracketLeft,  //12
    TTbracketRight, //13
    TTdoubleSlash,  //14

    TTidentifier,   //15
    TTkeyWord,      //16
    TTnumberFloat,  //17
    TTnumberInt,    //18
    TTstring,       //19

    TTindent,       //20
    TTdedent,       //21

    TTcolon,        //22
    TTcomma,        //23
    TTempty,        //24
    TTEol,          //25
    TTEof,          //26

} tTokenType;


/*token samotný*/
typedef struct itemToken
{
    tTokenType tokenType;
    tKeyWord keyWord;

    unsigned int lenght;    //délka řetězce
    unsigned int space;     //kolik místa v paměti potřebuje
    char *str;              //string samotný  
} Token;

/*vytvoří token*/
int getToken(Token *token);

/*rozpozná KeyWords od indetifikátoru*/
int keyWordFinder (Token *token);

/*rozpozná co je to za druh číslo*/
int numberConverter(Token *token);

void setVarString(Token*vs);

/*vyčistí proměnlivý string od informací které nese*/
void varStringClean(Token*vr);

/*uvoní paměť kterou Tokenzabírá*/
void varStringDestroy(Token*vr);

/*vloží stringu do proměnlivého stringu pomocí proměnlivého ukazatele*/
bool varStringChar(Token*vr, char c);

/*inicialnizuje proměnlivý string*/
bool varStringInit(Token*vr);

/*zkopíruje obsah jednoho do druhého*/
bool varStringCopy(Token*src, Token*des);

/*vložení stringu do proměnlivého stringu pomocí neproměnlivéhu ukazatele*/
bool varStringConstChar(Token*vr, const char *conChar);

/*porovnání strigu s obsahem proměnlivého stringu*/
int varStringCmp(Token*vr, const char *conChar);
/*generování kodu*/
/*Hlavní spoštěcí funkce*/
int generator_caller(Token *token);
/*rozhodovací jednotka*/
int generator_brain(Token *token);

//#endif