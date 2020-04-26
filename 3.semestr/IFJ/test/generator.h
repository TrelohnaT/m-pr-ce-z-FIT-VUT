/* ----------------------------------------------
	Předmět: IFJ (+ IAL)
	Tým: 107 var. I
	Autoři: Vlasáková, Nela (xvlasa14)
		  	Jakšík, Aleš (xjaksi01)
			Doleček, Václav (xdolec03)
			Pospíšil, FIlip (xpospi0f)
-------------------------------------------------
 Datum: 23. 11. 2019
 Účel: hlavickovy soubor pro expr.c
 Popis: WIP
 ------------------------------------------------
			(c) UniSoftCorn
 ----------------------------------------------*/

#ifndef EXPR_H
#define EXPR_H

#include <stdio.h>
#include <stdbool.h>

//#include "symtable.h"
//#include "lex_anal.h"
//#include "errors.h"
//#include "expr.h"
//#include "stack.h"


/* Item zásobníku pro Printer
** prostě zsobník a prostě to potřebuju
*/
typedef struct 
{
    char *strVar;
    int type;
    struct stackItem* nextItem; 
} stackItem;

/* Ukazatel na Vrchol zásobníku
** s tímto zásobníkem by měly být kompatibilní některé funkce z stack.h
*/
typedef struct
{
    stackItem* topGenStack;
} stackGen;

/* Tří adresný kod
** zde se budou předávat to jaká operace se má tisknout
** operator <- =,+,-,*,/ (čísla jsou podle Neliného expr.h)
** operand1 <- promněná/číslo která se nahraje
** operand2 <- promněná/čislo která se přidá (při +,-,*,/)
** operadn3 <- promněná/číslo která se uloží
*/
typedef struct 
{
    int operator;
    char *operand1;
    char *operand2;
    char *operand3;
} TAD;



#endif
