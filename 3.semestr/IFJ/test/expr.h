/* ----------------------------------------------
	Předmět: IFJ (+ IAL)
	Tým: 107 var. I
	Autor:
		Vlasakova, Nela (xvlasa14)
        Pospisil, Filip (xpospi0f)
-------------------------------------------------
		Jaksik, Ales (xjaksi01)
        Dolecek, Vaclav (xdolec03)
-------------------------------------------------
 Datum: 23. 11. 2019
 Účel: Precedenčí tabulka, stack, lineární seznam
 Popis: Hlavičkový soubor pro expr.c
 ------------------------------------------------
		      (c) UniSoftCorn
 ----------------------------------------------*/

 #ifndef EXPR_H
 #define EXPR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "parser.h"

#define SIZE 18		// jak velka je tabulka? 18 x 18

extern dataType finalType;

/* prirazeni indexu k jednotlivym polozkam v tabulce */
typedef enum precTabulkaSymboly {
	plus,			// +, 0
	minus,		// -, 1
	times,		// *, 2
	divide,		// /, 3
	less,			// <, 4
	more,			// >, 5
	lessEq,		// <=, 6
	moreEq,		// >=, 7
	eqEq,			// ==. 8
	lBracket,		// (, 9
	rBracket,		// ), 10
	divDiv,		// //, 11
	identifier,		// i, 12
	notEq,		// !=, 13
	integer,		// int, 14
	flt,			// float, 15
	strg,			// string, 16
	dollar,		// dollar sign, 17
	Ecko			// E, kvuli pravidel, 18
} precTableSym;


typedef enum precTabulkaPravidla {
      addE = 100,             // E + E
      subE = 101,             // E - E
      mulE = 102,             // E * E
      divE = 103,             // E / E
      eLe = 104,              // E < E
      eMe = 105,              // E > E
      eLEe = 106,             // E <= E
      eMEe = 107,             // E >= E
      eEQe = 108,             // E == E
      eDDe = 109,             // E // E
      iOp = 110,               // i
      eNEQe = 112,            // E != E
      BRAC = 113,             // (i)
	  noRule = 114,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     pravidlo
} precRules;

typedef struct stackItem {
    int symbol;   // symbol odpovidjici precedencni tabulce
    struct stackItem* next; // Ukazatel na n�sleduj�c� polo�ku.
    dataType datatype;      // datov� typ z tabulky symbol�
    bool isZero;
    char *key;
} sItem;

typedef struct {
    sItem* top; // Ukazatel na vrchol z�sobn�ku.
} stack;

/* - - - - - - - - - STRUKTURY PRO SEZNAM VYRAZU - - - - - - - - - */
typedef struct element {
      tTokenType tokenType;
      precTableSym symbol;
      struct element *rptr;
      struct element *lptr;
      bool isZero;
      char *key;
} *elementPtr;

typedef struct {
      elementPtr First;
      elementPtr Last;
      elementPtr Act;
} list;

/* - - - - - - - - - STRUKTURY PRO SEZNAM PROMENNYCH - - - - - - - - - */
typedef struct var {
      char *name;
      struct var *rptr;
      struct var *lptr;
} *variablePtr;

typedef struct {
      variablePtr First;
      variablePtr Last;
      variablePtr Act;
} varList;

/* - - - - - - - - - DEKLARACE FUNKCI - - - - - - - - - */
int rules(int numOperands, sItem *firstOp, sItem *secondOp, sItem *thirdOp);	// pravidla
int tokenToSymbol(Token *token); 			// funkce vyhodi symbol z precedencky podle tokenu
int analyze(stack *expStack, list *expList, stack *varStack, tBSTNodePtr *symTab, tBSTNodePtr *localTree);	// funkce analyzujici vyraz
int processToken(Token *token, tBSTNodePtr *symTab, tBSTNodePtr *localTree);			// hlavni funkce zpracovavajici tokeny
/* - - - - - - - - - DEKLARACE FUNKCI - - - - - - - - - */
void listInit(list* L);       // inicializace seznamu
void listInitVar(varList *L);
int insertLast(list *L, Token *token, int symbol);   // vlozeni do seznamu na konec seznamu
void insertVar(varList *L, Token *token); // vlozeni do seznamu pro generovani
void deleteFirst(list *L);                // odstraneni prvniho prvku jen tak for fun
void removeList(list *L);                 // odstraneni celeho seznamu, lol;

void stackInit(stack *);
int stackPop(stack *);
int stackPush(stack *, int, dataType, bool);
int stackPushList(stack *, elementPtr *, dataType);
int trashStack(stack *);
#endif