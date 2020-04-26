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
 Popis: Precedenčí tabulka a její symboly, kontrola výrazu, stack, lineární seznam.
 ------------------------------------------------
		      (c) UniSoftCorn
 ----------------------------------------------*/

#include "expr.h"

int precTable[SIZE][SIZE] = {
// +  -  *  /  <  >  <= >= == (  )  // i  != in fl st $
{  1, 1, 2, 2, 1, 1, 1, 1, 1, 2, 1, 2, 2, 1, 2, 2, 2, 1 }, // +
{  1, 1, 2, 2, 1, 1, 1, 1, 1, 2, 1, 2, 2, 1, 2, 2, 4, 1 }, // -
{  1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 2, 4, 1 }, // *
{  1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 2, 4, 1 }, // /
{  2, 2, 2, 2, 4, 4, 4, 4, 4, 2, 1, 2, 2, 4, 2, 2, 2, 1 }, // <
{  2, 2, 2, 2, 4, 4, 4, 4, 4, 2, 1, 2, 2, 4, 2, 2, 2, 1 }, // >
{  2, 2, 2, 2, 4, 4, 4, 4, 4, 2, 1, 2, 2, 4, 2, 2, 2, 1 }, // <=
{  2, 2, 2, 2, 4, 4, 4, 4, 4, 2, 1, 2, 2, 4, 2, 2, 2, 1 }, // >=
{  2, 2, 2, 2, 4, 4, 4, 4, 4, 2, 1, 2, 2, 4, 2, 2, 2, 1 }, // ==
{  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 4 }, // (
{  1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 4, 4, 4, 1 }, // )
{  1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 4, 2, 2, 4, 1 }, // //
{  1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 4, 4, 4, 4, 1 }, // i
{  2, 2, 2, 2, 4, 4, 4, 4, 4, 2, 1, 2, 2, 4, 2, 2, 2, 1 }, // !=
{  2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 1, 1, 4, 1, 4, 4, 4, 1 }, // int
{  1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 4, 4, 4, 1 }, // float
{  1, 4, 4, 4, 1, 1, 1, 1, 1, 4, 1, 4, 4, 1, 4, 4, 4, 1 }, // string
{  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4 }, // $
};
dataType finalType = typeNo;

/* ----------------------------------------------------------
* vyhodi symbol podle tokenu
* vstup: token
* vystup: symbol
---------------------------------------------------------- */
int tokenToSymbol(Token *token){
	switch (token->tokenType) {	// pracujeme s typem tokenu z lex_anal.h
	case TTminus:
		return minus;

	case TTplus:
		return plus;

	case TTmul:
		return times;

	case TTdiv:
		return divide;

	case TTbigger:
		return more;

	case TTless:
		return less;

	case TTbiggerEq:
		return moreEq;

	case TTlessEq:
		return lessEq;

	case TTeq:
		return eqEq;

	case TTnotEq:
		return notEq;

	case TTbracketLeft:
		return lBracket;

	case TTbracketRight:
		return rBracket;

	case TTdoubleSlash:
		return divDiv;

	case TTidentifier:
		return identifier;

	case TTnumberFloat:
		return flt;

	case TTnumberInt:
		return integer;

	case TTstring:
		return strg;

	default:
		return dollar;
	}
}

/* ----------------------------------------------------------
* hlavni funkce, ktera token poslany z parseru zpracovava
* vstup: token
* vystup: errorova hodnota (error ok v případě úspěchu)
* podrobny popis:
*	- inicializace stacku a dvou seznamu. Jeden je na
*	vyrazy a bude slouzit jako "input", druhy bude
*	uchovavat promenne pro generovani kodu
*	- pushnuti dolaroveho znaku na dno stacku
* 	- cyklus probiha dokud nenarazi na EOL, : nebo NULL
*		- vlozi token do expListu (tam nahraje tokentype,
*		ziska symbol z precedencni tabulky a key, podle
* 		ktereho lze najit pak datovy typ, navic pokud
*		hodnota identifikatoru je 0, prirazi zero flag)
*		- pokud je token typu identifikator, nahraje
*		ho do varListu, kde se ulozi co ten id je
* 	- nakonec seznamu nahraje dolarovy znak, zkusim
*	jestli tam opravdu je
*	- nastaveni aktivity na prvni prvek v seznamu
*	- analyza vyrazu
---------------------------------------------------------- */
int processToken(Token *token, tBSTNodePtr *symTab, tBSTNodePtr *localTree){
	int verdict;
	stack expStack;		// stack na vyrazy
	stack varStack;		// stack na promenne/pravidla pro vaska
	list expList;		// seznam na vyrazy
	varList varList;		// seznam na vyrazy pro vaska

	stackInit(&expStack);	// stack na vyrazy
	stackInit(&varStack);
	listInit(&expList);	// list na vyrazy
	listInitVar(&varList);	// list na promenne

	verdict = stackPush(&expStack, dollar, typeNo, false);	// push dollar na dno zasobniku
	if (verdict != NOERROR_OK) {
		return verdict;
	}

	// prochazet dokud nenarazim na EOL, dvojtecku nebo NULL token
	while(token->tokenType != TTEol && token->tokenType != TTcolon && token != NULL) {
		
		printf("Vlezla jsem v process token do while. Token type je %d", token->tokenType);

		int symbol = tokenToSymbol(token);
		insertLast(&expList, token, symbol);		// vlozit token do seznamu
		if(token->tokenType == TTidentifier) {	// pokud je token identifikator
			insertVar(&varList, token);		// ulozim promennou do seznamu pro generovani

			printf("Vkladam vaskovi do listu veci. Vlozila jsem tohle %d", token->tokenType);

		}
		if ((verdict = getToken(token)) != NOERROR_OK) {	// getToken - pokud se stala chyba, vratit jaka
		return verdict;
		}
	}

	if(token == NULL){
		return ERROR_SYN;
	}

	insertLast(&expList, NULL, dollar);		// vlozeni dollaru na konec listu

	printf("vylezla jsem z while. do listu jsem dala na konec %d", expList.Last->symbol);

	if(expList.Last->symbol != dollar) {	// overeni ze tam opravdu je
		return ERROR_COMPILATOR;
	}

	expList.Act = expList.First;		// nastaveni aktivity na prvni prvek
	verdict = analyze(&expStack, &expList, &varStack, symTab, localTree);	// analyzovat nacteny vyraz
	return verdict;					// ve verdict je bud error (nejaky) nebo noerror hodnota	
}

/* ----------------------------------------------------------
* overeni ze symboly na stacku jsou v souladu s pravidly
* vstup: kolik, ceho
* vystup: pravidlo
---------------------------------------------------------- */
int rules(int numOperands, sItem* firstOp, sItem* secondOp, sItem* thirdOp) {
	switch (numOperands) {
	case 1:
		if (firstOp->symbol == identifier) {	// E -> i, pouze jeden operand
			return iOp;
		}
		else {
			return noRule;
		}

	case 3:
		if(firstOp->symbol == lBracket && thirdOp->symbol == rBracket) {		// pokud jsou zavorky
			// a uvnitr je int, float nebo string, pripadne identifikator
			if (secondOp->symbol == integer || secondOp->symbol == flt || secondOp->symbol == strg) {
				return BRAC;
			}
			if(secondOp->symbol == identifier) {
				return BRAC;
			}
			else {
				return ERROR_SYN;
			}
		}

		if (firstOp->symbol == Ecko && thirdOp->symbol == Ecko) {	// pokud je to konstrukce E _?_ E
			switch (secondOp->symbol) {
				/* - - - - - - - - - - - PLUS - - - - - - - - - - - */
				case plus :
					if(firstOp->datatype == typeString) {
						if(thirdOp->datatype == typeString) {		// string a string je ok
							finalType = typeString;
							return addE;
						}
						return ERROR_SEM_FUN;					// string a cokoliv jineho je fuj
					}
					else if(thirdOp->datatype == typeString) {		// string na tretim miste ale ne prvnim je fuj
						return ERROR_SEM_FUN;
					}
					else if(firstOp->datatype == typeInt) {
						if(thirdOp->datatype == typeFloat) {		// int plus float je float
							finalType = typeFloat;
							return addE;
						}
						if(thirdOp->datatype == typeInt) {			// int plus int je int
							finalType = typeInt;
							return addE;
						}
						else {
							return ERROR_SYN;
						}
					}
					else if(firstOp->datatype == typeFloat) {			// float plus int je float
						if(thirdOp->datatype == typeInt) {
							finalType = typeFloat;
							return addE;
						}
						if(thirdOp->datatype == typeFloat) {		// float plus float je float
							finalType = typeFloat;
							return addE;
						}
						else {
							return ERROR_SYN;
						}
					}
					else {
						return ERROR_SEM_FUN;
					}

				/* - - - - - - - - - - - MINUS - - - - - - - - - - - */
				case minus :
					if(firstOp->datatype == typeString || thirdOp->datatype == typeString) {	// string a string je ne-e
						return ERROR_SEM_FUN;
					}

					else if(firstOp->datatype == typeInt) {
						if(thirdOp->datatype == typeFloat) {			// int - float je float
							finalType = typeFloat;
							return subE;
						}
						if(thirdOp->datatype == typeInt) {				// int - int je int
							finalType = typeInt;
							return subE;
						}
						return ERROR_SYN;
					}
					else if(firstOp->datatype == typeFloat) {				
						if(thirdOp->datatype == typeInt) {				// float - int je float
							finalType = typeFloat;
							return subE;
						}
						if(thirdOp->datatype == typeFloat) {			// float - float je float
							finalType = typeFloat;
							return subE;
						}
						return ERROR_SYN;
						
					}
					else {
						return ERROR_SEM_FUN;
					}

				/* - - - - - - - - - - - TIMES - - - - - - - - - - - */
				case times :
					if(firstOp->datatype == typeString || thirdOp->datatype == typeString) {		// string a string ne-e
						return ERROR_SEM_FUN;
					}

					else if(firstOp->datatype == typeInt) {
						if(thirdOp->datatype == typeFloat) {		// int a float ok float
							finalType = typeFloat; 
							return mulE;
						}
						if(thirdOp->datatype == typeInt) {			// int a int ok int
							finalType = typeInt;
							return mulE;
						}
						return ERROR_SYN;
					}
					else if(firstOp->datatype == typeFloat) {
						if(thirdOp->datatype == typeInt) {			// float a int ok float
							finalType = typeFloat;
							return mulE;
						}
						if(thirdOp->datatype == typeFloat) {		// float a float ok float
							finalType = typeFloat;
							return mulE;
						}
						return ERROR_SYN;
					}
					else {
						return ERROR_SEM_FUN;
					}

				/* - - - - - - - - - - - DIVIDE - - - - - - - - - - - */
				case divide :
					if(thirdOp->isZero == true) {
						return ERROR_ZERO_DIV;			// deleni nulou nejde
					}
					if(firstOp->datatype == typeString || thirdOp->datatype == typeString) {
						return ERROR_SEM_FUN;			// string nejde delit
					}

					else if(firstOp->datatype == typeInt) {
						if(thirdOp->datatype == typeFloat) {	// int deleno floatem je float
							finalType = typeFloat;
							return divE;
						}
						if(thirdOp->datatype == typeInt) {		// int a int je float
							finalType = typeFloat;
							return divE;
						}
						return ERROR_SYN;
					}
					else if(firstOp->datatype == typeFloat) {		// float a int je float
						if(thirdOp->datatype == typeInt) {
							finalType = typeFloat;
							return divE;
						}
						if(thirdOp->datatype == typeFloat) {	// float a float je float
							finalType = typeFloat;
							return divE;
						}
						return ERROR_SYN;
					}
					else {
						return ERROR_SEM_FUN;
					}
				
				/* - - - - - - - - - - - DIVIDE DIVIDE - - - - - - - - - - - */
				case divDiv :
					if(thirdOp->isZero == true) {
						return ERROR_ZERO_DIV;			// deleni nulou nejde
					}
					else if(firstOp->datatype == typeInt && thirdOp->datatype == typeInt){
						finalType = typeInt;
						return eDDe;
					}
					else {
						return ERROR_SEM_FUN;
					}

				/* - - - - - - - - - - - LESS - - - - - - - - - - - */
				case less :
					if(firstOp->datatype == typeInt && thirdOp->datatype == typeInt){
						finalType = typeInt;
						return eLe;
					}
					else if(firstOp->datatype == typeFloat && thirdOp->datatype == typeFloat){
						finalType = typeInt;
						return eLe;
					}
					else if(firstOp->datatype == typeString && thirdOp->datatype == typeString){
						finalType = typeInt;
						return eLe;
					}
					else {
						return ERROR_SEM_FUN;
					}
				/* - - - - - - - - - - - MORE - - - - - - - - - - - */
				case more :
					if(firstOp->datatype == typeInt && thirdOp->datatype == typeInt){
						finalType = typeInt;
						return eMe;
					}
					else if(firstOp->datatype == typeFloat && thirdOp->datatype == typeFloat){
						finalType = typeInt;
						return eMe;
					}
					else if(firstOp->datatype == typeString && thirdOp->datatype == typeString){
						finalType = typeInt;
						return eMe;
					}
					else {
						return ERROR_SEM_FUN;
					}
				
				/* - - - - - - - - - - - LESS OR EQUAL - - - - - - - - - - - */
				case lessEq :
					if(firstOp->datatype == typeInt && thirdOp->datatype == typeInt){
						finalType = typeInt;
						return eLEe;
					}
					else if(firstOp->datatype == typeFloat && thirdOp->datatype == typeFloat){
						finalType = typeInt;
						return eLEe;
					}
					else if(firstOp->datatype == typeString && thirdOp->datatype == typeString){
						finalType = typeInt;
						return eLEe;
					}
					else {
						return ERROR_SEM_FUN;
					}
				
				/* - - - - - - - - - - - MORE OR EQUAL - - - - - - - - - - - */
				case moreEq :
					if(firstOp->datatype == typeInt && thirdOp->datatype == typeInt){
						finalType = typeInt;
						return eMEe;
					}
					else if(firstOp->datatype == typeFloat && thirdOp->datatype == typeFloat){
						finalType = typeInt;
						return eMEe;
					}
					else if(firstOp->datatype == typeString && thirdOp->datatype == typeString){
						finalType = typeInt;
						return eMEe;
					}
					else {
						return ERROR_SEM_FUN;
					}

				/* - - - - - - - - - - - DIVIDE - - - - - - - - - - - */
				case notEq :
					if(firstOp->datatype == typeString){
						if(thirdOp->datatype == typeInt || thirdOp->datatype == typeFloat){
							return ERROR_SEM_FUN;
						}
						finalType = typeInt;
						return eNEQe;
					}
					else if(firstOp->datatype == typeNo && thirdOp->datatype == typeNo){
						finalType = typeInt;
						return eNEQe;
					}
					finalType = typeInt;
					return eNEQe;


				/* - - - - - - - - - - - DIVIDE - - - - - - - - - - - */
				case eqEq :
					if(firstOp->datatype == typeString){
						if(thirdOp->datatype == typeInt || thirdOp->datatype == typeFloat){
							return ERROR_SEM_FUN;
						}
						finalType = typeInt;
						return eEQe;
					}
					else if(firstOp->datatype == typeNo && thirdOp->datatype == typeNo){
						finalType = typeNo;
						return eEQe;
					}
					finalType = typeInt;
					return eEQe;

				/* - - - - - - - - - - - DEFAULT - - - - - - - - - - - */
				default:
					return noRule;	
			}
		}
	
		else {
			return ERROR_SYN;
		}
		
	default:
		return noRule;	
	}
}

/* ----------------------------------------------------------
* analyzujici funkce
* vstup: token
* vystup: errorova hodnota (error ok v případě úspěchu)
---------------------------------------------------------- */
int analyze(stack *expStack, list *expList, stack *varStack, tBSTNodePtr *symTab, tBSTNodePtr *localTree) {
	printf("jsem v analyze. yay.");
	int stackEl, input, action, finalRule;
	tBSTNodePtr temp, tempP;
	sItem *op1, *op2, *op3;
	stack tempStack;
	stackInit(&tempStack);
	while(expList->Act->symbol <= dollar) {
		printf("jsem ve while v analyze. Ze listu do stacku davam %d", expStack->top->symbol)
		stackEl = expStack->top->symbol;	// to co je na stacku
		input = expList->Act->symbol;		// symbol reprezentujici token ktery nam dal scanner
		action = precTable[stackEl][input];

		if(action == 1) {	// pokud reduce
			if (stackEl == identifier) {
				if (rules(1, expStack->top, NULL, NULL) == iOp) {
					if (localTab) {
						temp->TBSNodeCont = BSTSearch(localTree, expStack->top->key);
					}
					else {
						temp->TBSNodeCont = BSTSearch(symTab, expStack->top->key);
					}

					stackPop(expStack);
					stackPush(expStack, Ecko, temp->TBSNodeCont->dType, expStack->top->isZero);
				}
			}

			if(stackEl == Ecko) {
				if(localTab) {
					temp->TBSNodeCont = BSTSearch(localTree, expStack->top->key);
					stackPush(&tempStack, expStack->top->symbol, temp->TBSNodeCont->dType, expStack->top->isZero);
					op1 = tempStack.top;
					stackPop(expStack);	
					
					temp->TBSNodeCont = BSTSearch(localTree, expStack->top->key);
					stackPush(&tempStack, expStack->top->symbol, temp->TBSNodeCont->dType, expStack->top->isZero);
					op2 = tempStack.top;
					stackPop(expStack);	

					temp->TBSNodeCont = BSTSearch(localTree, expStack->top->key);
					stackPush(&tempStack, expStack->top->symbol, temp->TBSNodeCont->dType, expStack->top->isZero);
					op3 = tempStack.top;
					stackPop(expStack);	
				}
				else {
					temp->TBSNodeCont = BSTSearch(symTab, expStack->top->key);
					stackPush(&tempStack, expStack->top->symbol, temp->TBSNodeCont->dType, expStack->top->isZero);
					op1 = tempStack.top;
					stackPop(expStack);	
					
					temp->TBSNodeCont = BSTSearch(symTab, expStack->top->key);
					stackPush(&tempStack, expStack->top->symbol, temp->TBSNodeCont->dType, expStack->top->isZero);
					op2 = tempStack.top;
					stackPop(expStack);	

					temp->TBSNodeCont = BSTSearch(symTab, expStack->top->key);
					stackPush(&tempStack, expStack->top->symbol, temp->TBSNodeCont->dType, expStack->top->isZero);
					op3 = tempStack.top;
					stackPop(expStack);
				}

				finalRule = rules(3, op1, op2, op3);
				if(finalRule != noRule) {
					for (int i = 0; i < 3; i++) {
						stackPop(expStack);
					}
					stackPush(expStack, Ecko, finalType, false);
					stackPush(varStack, finalRule, typeNo, false);
					trashStack(&tempStack);
					return NOERROR_OK;
				}
				else {
					return ERROR_SYN;
				}
			}
		}

		else if (action == 2) { // pokud shift
			if(localTab){
				tempP->TBSNodeCont = BSTSearch(localTree, expList->Act->key);
			}
			else {
				tempP->TBSNodeCont = BSTSearch(symTab, expList->Act->key);
			}
			stackPushList(expStack, &expList->Act, temp->TBSNodeCont->dType);	// push z listu
			expList->Act = expList->Act->rptr;				// posunu aktivitu
		}

		else if (action == 3) { // pokud =
			// TO DO
		}

		else if (action == 4) { // pokud error
			return ERROR_SYN;
		}
	}
	return ERROR_COMPILATOR;
}

/*                   FUNKCE STACKU A LISTU                */

/* ----------------------------------------------------------
 * inicializace seznamu
 * vstup: nazev seznamu ktery chceme inicializovat
 * vystup: neni lol
---------------------------------------------------------- */
void listInit (list *L){
      L->Act = NULL;
      L->First = NULL;
	L->Last = NULL;
}

void listInitVar (varList *L){
      L->Act = NULL;
      L->First = NULL;
	L->Last = NULL;
}

/* ----------------------------------------------------------
 * vlozeni noveho prvku na konec seznamu
 * vstup: kam chceme dát, token ze ktereho delame prvek
 * vystup: neni lol
---------------------------------------------------------- */
int insertLast (list *L, Token *token, int symbol){
      elementPtr newElement;        // vytvoreni noveho elementu
      newElement = (struct element*) malloc(sizeof(struct element));
      if (newElement == NULL) {
            return ERROR_COMPILATOR;
      }
      
      // nahrani hodnot do vytvoreneho prvku
      newElement->lptr = L->Last;  
      newElement->rptr = NULL;
      newElement->tokenType = token->tokenType;
      newElement->key = token->str;

      newElement->symbol = symbol;

      if (varStringCmp(token, "0") || varStringCmp(token, "0.0")) {
            newElement->isZero = true;
      }
      else{
            newElement->isZero = false;
      }
      
      
      if(L->Last != NULL){               // pokud seznam nebyl prazdny
            L->Last->rptr = newElement;  // nastavi se ze od toho soucasneho prvniho je pravy ten novy
      }
      else {                              // pokud byl prazdny
            L->First = newElement;         // prvnim prvkem je novy
      }

      L->Last = newElement;              // nakonec se na prvni nastavi novy
      return NOERROR_OK;
}
/* ----------------------------------------------------------
 * vlozeni promenne do seznamu pro generovani
 * vstup: kam chceme dát a co tam chceme dat
 * vystup: neni lol
---------------------------------------------------------- */
void insertVar (varList *L, Token *token){
      variablePtr newElement;        // vytvoreni noveho elementu
      newElement = (struct var*) malloc(sizeof(struct var));
      if (newElement == NULL) {
            return;
      }  
      
      newElement->name = token->str;

      if(L->First != NULL){               // pokud seznam nebyl prazdny
            L->Last->rptr = newElement;  // nastavi se ze od toho soucasneho prvniho je pravy ten novy
      }
      else {                              // pokud byl prazdny
            L->First = newElement;         // prvnim prvkem je novy
      }
      L->Last = newElement;              // nakonec se na prvni nastavi novy
}

/* ----------------------------------------------------------
 * vymazani prvniho prvku
 * vstup: seznam odkud
 * vystup: neni lol
---------------------------------------------------------- */
void deleteFirst(list *L){
      if (L->First != NULL){              // pokud seznam neni prazdny      
      if (L->First == L->Act) {           // kdyz je prvni prvek aktivni, deaktivuje se
            L->Act = NULL;             
      }
      if(L->First == L->Last){            // kdyz je tam jen jeden prvek
            L->First = NULL;              
            L->Last = NULL;
            free(L->First);            
      } 
      else {                              // jinak pokud tam je plno prvku
            L->First = L->First->rptr;    // prvni je prvniho nasledujici
            free(L->First->lptr);         // uvolnit pamet
            L->First->lptr = NULL;        // prvniho levy je null
        }
    }
}
/* ----------------------------------------------------------
 * smazani celeho seznamu
 * vstup: jaky list mazeme
 * vystup: neni lol
---------------------------------------------------------- */
void removeList(list *L) {
      elementPtr deleteElement;
      while((deleteElement = L->Last) != NULL){
            L->Last = deleteElement->lptr;
            deleteElement->lptr = NULL;
            deleteElement->rptr = NULL;
            free(deleteElement);
      }

      L->Act = NULL;
      L->First = NULL;
	L->Last = NULL;
}

/* ----------------------------------------------------------
* inicializace stacku
* vstup: stack
* vystup: void
---------------------------------------------------------- */
void stackInit(stack *S){
    S->top = NULL;
}

/* ----------------------------------------------------------
* popne top stacku
* vstup: stack jehoz top chceme popnout
* vystup: errorova hodnota (error ok v případě úspěchu)
---------------------------------------------------------- */
int stackPop(stack *S){
    if(S->top == NULL){
        return ERROR_COMPILATOR;
    }
    else{
        sItem *temp = S->top;   // Do docasne promenne ulozim top stacku
        S->top = temp->next;    // na vrchol vlozim nasledujici prvek
        free(temp);             // Uvolnit pamet
        return NOERROR_OK;
    }
    return ERROR_COMPILATOR;    
}

/* ----------------------------------------------------------
* pushne na stack veci
* vstup: stack, symbol z precedencky, datovy typ
* vystup: errorova hodnota (error ok v případě úspěchu)
---------------------------------------------------------- */
int stackPush(stack *S, int important, dataType datatype, bool isZero){
    sItem *new = (sItem*)malloc(sizeof(struct stackItem));

    if (new == NULL){
        return ERROR_COMPILATOR;
    }

    new->next = S->top;
    new->symbol = important;
    S->top = new;
    new->datatype = datatype;
    new->isZero = isZero;
    
    return NOERROR_OK;
}

/* ----------------------------------------------------------
* pushne na stack z listu
* vstup: stack, element listu, datovy typ
* vystup: errorova hodnota (error ok v případě úspěchu)
---------------------------------------------------------- */
int stackPushList(stack *S, elementPtr *Act, dataType datatype){
    sItem *new = (sItem*)malloc(sizeof(sItem));

    if (new == NULL){
        return ERROR_COMPILATOR;
    }

    new->symbol = (*Act)->symbol;
    new->next = S->top;
    new->isZero = (*Act)->isZero;
    new->datatype = datatype;
    new->key = (*Act)->key;

    return NOERROR_OK;
}

/* ----------------------------------------------------------
* vysype stack, vrati do inicializovaneho stavu
* vstup: co chceme vysypat
* vystup: errorova hodnota (error ok v případě úspěchu)
---------------------------------------------------------- */
int trashStack(stack *S){
    while(S->top != NULL){
        stackPop(S);
    }

    if(S->top == NULL){
        stackInit(S);
        return NOERROR_OK;
    } 
    else {
        return ERROR_COMPILATOR;
    }
}
