/* ----------------------------------------------
	Předmět: IFJ (+ IAL)
	Tým: 107 var. I
	Autor:
		Jaksik, Ales (xjaksi01)
-------------------------------------------------
		Dolecek, Vaclav (xdolec03)
        Pospisil, Filip (xpospi0f)
        Vlasakova, Nela (xvlasa14)
-------------------------------------------------
 Datum: 23. 11. 2019
 Účel: Parser
 Popis: Hlavičkový soubor pro parser
 ------------------------------------------------
		      (c) UniSoftCorn
 ----------------------------------------------*/

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lex_anal.h"
#include "symtable.h"
//#include "expr.h"


// pomocna pro kontrolu zanoreni
extern bool localTab;

/* jedina dulezita funkce, ktera bude volana od jinud */
int parse();
/* -------------------------------------------------- */



// podstatne pro parser
int body(Token *, tBSTNodePtr *, tBSTNodePtr *);
int funcDef(Token *, tBSTNodePtr *, tBSTNodePtr *);
int func(Token *, tBSTNodePtr *, tBSTNodePtr *);
int id(Token *, tBSTNodePtr *, tBSTNodePtr *);
int cIf(Token *, tBSTNodePtr *, tBSTNodePtr *);
int cWhile(Token *, tBSTNodePtr *, tBSTNodePtr *);
int item(Token *);
int def_body(Token *, tBSTNodePtr *, tBSTNodePtr *);
int command(Token *, tBSTNodePtr *, tBSTNodePtr *);
int commandBody(Token *, tBSTNodePtr *, tBSTNodePtr *);

// vestavene funkce
void buildInFunc(tBSTNodePtr *);

#endif
