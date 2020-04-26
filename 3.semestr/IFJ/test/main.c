/* ----------------------------------------------
	Předmět: IFJ (+ IAL)
	Tým: 107 var. I
	Autoři: 
            Vlasáková, Nela (xvlasa14)
			Jakšík, Aleš (xjaksi01)
			Doleček, Václav (xdolec03)
			Pospíšil, FIlip (xpospi0f)
-------------------------------------------------
 Datum: 23. 11. 2019
 ------------------------------------------------
			(c) UniSoftCorn
 ----------------------------------------------*/
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "errors.h"
#include "parser.h"


int main() {
    
    int result;

    result = parse();

	fprintf(stderr, "ERROR %d\n", result);

    return result;
}
