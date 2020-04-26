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

/*
**otevře soubor a vytvoří na něj ukazatel
*/
int main(int argc, char const *argv[])
{
    FILE *programIFJ19;

    int errorCaller = 0;

/*kontrola argumentů*/
    programIFJ19 = fopen(argv[1], "r");
    if (programIFJ19 == NULL)
    {
        printf("Zadaný program je prázndný\n");
        return 0;
    }

    if (argc <= 1)
    {
        printf("Zadejte soubor který chcete překládat\n");
        return 0;
    }

    errorCaller = lex_anal(&eToken);

    if(errorCaller == 0)
    {
        printf("Vše v pohodě\n");
    }
    else
    {
        printf("něco se pojebalo\n");
    }
    

    /*ÚklKeyWord po sobě*/
    fclose(programIFJ19);

    return 0;
}
