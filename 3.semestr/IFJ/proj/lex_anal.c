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
#include <ctype.h>

#include "lex_anal.h"

/*globální kktiny*/

pointerToken* ptrToken;

/*stavy skeneru*/
#define StateStar 100;
#define StateComent 101;
#define StateChar 102;
#define StateNumber 103;

#define StatePlus 104;
#define StateMinus 105;
#define StateMun 106;
#define StateDiv 107;

#define StateKewWord 108;
#define StateIndentifer 109;

#define StateLess 110;
#define StateMore 111;
#define StateLessEq 112;
#define StateMoreEq 113;
#define StateEq 114;
#define StateNotEq 115;


/*vyčistí buffer od zbytku předchozích slov*/
void clearBuffer(char buffer[]);
void clearBuffer(char buffer[])
{
    int j = 0;
    for(; j != 128; j++)
    {
        buffer[j] = 0;
    }
    buffer[j+1] = 0;
}

/*vytiske to co je v bufferu, použito na kontrolní vypisy*/
void printWord(char buffer[], int wordCharacter);
void printWord(char buffer[], int wordCharacter)
{
    printf("I found '");
    for(int p = 0; p < wordCharacter; p++)
    {
        if(buffer[wordCharacter] != 0)
        {
                printf("%c", buffer[p]);
        }
    }
    printf("'\n");
    
}

/*zkontroluje zda tento charakter je pismeno
**Používá se to pro určení, zda klíčové slovo končí tam kde má
**Zde se nastavují oddělovače
*/
int isitallow(char character);
int isitallow(char character)
{
    if((character >= 48 && character <= 57) || (character >= 65 && character <= 90) || (character >= 97 && character <= 122))
    {
        if(character !=' ' && character != '\n')
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
    
}

/*Rozseká rádek na slova a ty pak vrátí prostřednictvým bufferWord*/
char lineCutter(FILE *filename)
{
/*deklarace a inicializace promněných a ukazatelů*/
    FILE *programIFJ19;

    programIFJ19 = filename;

    char bufferLine[1024];  //1024 protože se mi to čislo líbí protože se to prý používá pro buffery
    char bufferWord[128];   //zde se budou ukládat slova z radku
    int lineLenght = 0;     //delka radku
    int wordCharacter = 0;  //pozice pismene ve slovu

    int cleanFlag = 0;     //vyčistit pole
    //int digitFlag = 0;     //signal pro číslo
    //int comentFlag = 0;    //je nalezen komentař


    tokenInit(ptrToken);

    /*
    ** input: surový kus kodu
    ** output: tokeny
    ** provede Lexikální analýzu a převede surový kod na tokeny
    */
    while(fscanf(programIFJ19, "%1023c[^\n, ]", bufferLine) != EOF)
    {
        /*změří kolik znaku má každý řádek*/
        lineLenght = strlen(bufferLine);
        printf("%s\n",bufferLine);
        /*prototyp procházení buffr po písmenkách*/
        for (int i = 0; i < lineLenght; i++)
        {
            /*nahrávání jednoho slova do bufferWord*/
            bufferWord[wordCharacter] = bufferLine[i];
            
            /*detektor mezer*/
            if(bufferWord[wordCharacter] == ' ')
            {
                //digitFlag = 0;
            }

            /*detektor čísel
            ** pokud symbol vstupující do bufferWord bude číslo, toto to zjistí*/
            /*if(isdigit(bufferWord[wordCharacter]))
            {
                digitFlag = 1;
            }*/
            
            /*
            ** input: všecky znaky na které jen jde narazit
            ** output: znaky z ASCII tabulky od 32 do 126
            ** Zamezuje pronikání nechtěných znaku do systému a případné jeho rozbití
            */
            if((bufferWord[wordCharacter] >= 32 || bufferWord[wordCharacter] <= 126) || bufferWord[wordCharacter] == '\n')
            {
                /*
                ** input: celý řádek v bufferLine
                ** output: jedno slovo v bufferWord
                ** zde projdou jen písmena nebo čísla, která pak tvoří slova
                */
                if((bufferWord[wordCharacter] > 57 || bufferWord[wordCharacter] < 48) && (bufferWord[wordCharacter] > 90 || bufferWord[wordCharacter] < 65) && (bufferWord[wordCharacter] > 122 || bufferWord[wordCharacter] < 97) && (bufferWord[wordCharacter] != 95))
                {
                   /* if(digitFlag == 1)
                    {*/
                       //printf("found number \n");
                    /*   digitFlag = 0;
                    }*/

                    /*
                    ** input: slova
                    ** output: tokeny(v tuto chvíli jen hlašení o nálezech)
                    ** rozezná které slovo se nachází v bufferWord a podle toho
                    ** předá potřebný Token
                    ** toto se dívá na první pismeno jako první
                    */

                    switch (bufferWord[wordCharacter - wordCharacter])
                    {
                    case 'i':
                        if(bufferWord[1] == 'f')
                        {
                            if(isitallow(bufferWord[2]) == 0)
                            {
                                printWord(bufferWord, wordCharacter);
                            }
                            else
                            {
                                printf("I found keyword if\n");

                            }
                        }
                        else if(bufferWord[2] == 'n')
                        {
                            if(bufferWord[3] == 'p')
                            {
                                if(bufferWord[4] == 'u')
                                {
                                    if(bufferWord[5] == 't')
                                    {
                                        if(bufferWord[6] == 's')
                                        {
                                            if(isitallow(bufferWord[7]) == 0)
                                            {
                                                printWord(bufferWord, wordCharacter);
                                            }
                                            else
                                            {
                                                printf("I found keyword inputs\n");
                                            }
                                        }
                                        else if(bufferWord[6] == 'i')
                                        {
                                            if(isitallow(bufferWord[7]) == 0)
                                            {
                                                printWord(bufferWord, wordCharacter);
                                            }
                                            else
                                            {
                                                printf("I found keyword inputi\n");
                                            }
                                        }
                                        else if(bufferWord[6] == 'f')
                                        {
                                            if(isitallow(bufferWord[7]) == 0)
                                            {
                                                printWord(bufferWord, wordCharacter);
                                            }
                                            else
                                            {
                                                printf("I found keyword inputf\n");
                                            }
                                        }
                                        else
                                        {
                                            printWord(bufferWord, wordCharacter);
                                        }
                                    }
                                    else
                                    {
                                        printWord(bufferWord, wordCharacter);
                                    }
                                }
                                else
                                {
                                    printWord(bufferWord, wordCharacter);
                                }
                            }
                            else
                            {
                                printWord(bufferWord, wordCharacter);
                            }
                        }
                        else
                        {
                            printWord(bufferWord, wordCharacter);
                        }
                    break;

                    case 'e':
                        if(bufferWord[1] == 'l')
                        {
                            if (bufferWord[2] == 's')
                            {
                                if(bufferWord[3] == 'e')
                                {
                                    if(isitallow(bufferWord[4]) == 0)
                                    {
                                        printWord(bufferWord, wordCharacter);
                                    }
                                    else
                                    {
                                        printf("I found keyword else\n");
                                    }
                                }
                                else
                                {
                                    printWord(bufferWord, wordCharacter);
                                }    
                            }
                            else if(bufferWord[2] == 'i')
                            {
                                if(bufferWord[3] == 'f')
                                {
                                    if(isitallow(bufferWord[4]) == 0)
                                    {
                                        printWord(bufferWord, wordCharacter);
                                    }
                                    else
                                    {
                                        printf("I found keyword elif\n");
                                    }
                                }
                                else
                                {
                                    printWord(bufferWord, wordCharacter);
                                }
                            }
                            else
                            {
                                printWord(bufferWord, wordCharacter);
                            }   
                        }
                        else
                        {
                           printWord(bufferWord, wordCharacter);
                        }  
                    break;

                    case 'w':
                        if(bufferWord[1] == 'h')
                        {
                            if(bufferWord[2] == 'i')
                            {
                                if(bufferWord[3] == 'l')
                                {
                                    if(bufferWord[4] == 'e')
                                    {
                                        if(isitallow(bufferWord[5]) == 0)
                                        {
                                            printWord(bufferWord, wordCharacter);
                                        }
                                        else
                                        {
                                            printf("I found keyword while\n");
                                        }
                                    }
                                    else
                                    {
                                        printWord(bufferWord, wordCharacter);
                                    }
                                }
                                else
                                {
                                    printWord(bufferWord, wordCharacter);
                                }
                            }
                            else
                            {
                               printWord(bufferWord, wordCharacter); 
                            }  
                        }
                        else
                        {
                           printWord(bufferWord, wordCharacter);
                        }
                    break;

                    case 'p':
                        if(bufferWord[1] == 'r')
                        {
                            if(bufferWord[2] == 'i')
                            {
                                if(bufferWord[3] == 'n')
                                {
                                    if(bufferWord[4] == 't')
                                    {
                                        if(isitallow(bufferWord[5]) == 0)
                                        {
                                            printWord(bufferWord, wordCharacter);
                                        }
                                        else
                                        {
                                            printf("I found keyword print\n");
                                        }
                                    }
                                    else
                                    {
                                        printWord(bufferWord, wordCharacter);
                                    }
                                }
                                else
                                {
                                    printWord(bufferWord, wordCharacter);
                                }
                            }
                            else
                            {
                                printWord(bufferWord, wordCharacter);
                            }
                        }
                        else if(bufferWord[1] == 'a')
                        {
                            if(bufferWord[2] == 's')
                            {
                                if(bufferWord[3] == 's')
                                {
                                    if(isitallow(bufferWord[4]) == 0)
                                        {
                                            printWord(bufferWord, wordCharacter);
                                        }
                                        else
                                        {
                                            printf("I found keyword pass\n");
                                        }
                                }
                                else
                                {
                                    printWord(bufferWord, wordCharacter);
                                }
                            }
                            else
                            {
                                printWord(bufferWord, wordCharacter);
                            }
                        }
                        else
                        {
                            printWord(bufferWord, wordCharacter);
                        }
                    break;

                    case 'f':
                        if(bufferWord[1] == 'o')
                        {
                            if(bufferWord[2] == 'r')
                            {
                                if(isitallow(bufferWord[3]) == 0)
                                {
                                    printWord(bufferWord, wordCharacter);
                                }
                                else
                                {
                                    printf("I found keyword for\n");
                                }
                            }
                            else
                            {
                                printWord(bufferWord, wordCharacter);
                            }
                        }
                        else if(bufferWord[1] == 'a')
                        {
                            if(bufferWord[2] == 'l')
                            {
                                if(bufferWord[3] == 's')
                                {
                                    if(bufferWord[4] == 'e')
                                    {
                                        if(isitallow(bufferWord[5]) == 0)
                                        {
                                            printWord(bufferWord, wordCharacter);
                                        }
                                        else
                                        {
                                            printf("I found keyword false\n");
                                        }
                                    }
                                    else
                                    {
                                        printWord(bufferWord, wordCharacter);
                                    }
                                }
                                else
                                {
                                    printWord(bufferWord, wordCharacter);
                                }
                            }
                            else
                            {
                                printWord(bufferWord, wordCharacter);
                            }
                        }
                        else
                        {
                            printWord(bufferWord, wordCharacter);
                        }
                    break;

                    case 'c':
                        if(bufferWord[1] == 'o')
                        {
                            if(bufferWord[2] == 'n')
                            {
                                if(bufferWord[3] == 't')
                                {
                                    if(bufferWord[4] == 'i')
                                    {
                                        if(bufferWord[5] == 'n')
                                        {
                                            if(bufferWord[6] == 'u')
                                            {
                                                if(bufferWord[7] == 'e')
                                                {
                                                    if(isitallow(bufferWord[5]) == 0)
                                                    {
                                                        printWord(bufferWord, wordCharacter);
                                                    }
                                                    else
                                                    {
                                                        printf("I found keyword continue\n");
                                                    }
                                                }
                                                else
                                                {
                                                    printWord(bufferWord, wordCharacter);
                                                }
                                            }
                                            else
                                            {
                                                printWord(bufferWord, wordCharacter);
                                            }
                                        }
                                        else
                                        {
                                            printWord(bufferWord, wordCharacter);
                                        }
                                    }
                                    else
                                    {
                                        printWord(bufferWord, wordCharacter);
                                    }
                                }
                                else
                                {
                                    printWord(bufferWord, wordCharacter);
                                }
                            }
                            else
                            {
                                printWord(bufferWord, wordCharacter);
                            }   
                        }
                        else
                        {
                            printWord(bufferWord, wordCharacter);
                        }
                    break;

                    case 'r':
                        if(bufferWord[1] == 'e')
                        {
                            if(bufferWord[2] == 't')
                            {
                                if(bufferWord[3] == 'u')
                                {
                                    if(bufferWord[4] == 'r')
                                    {
                                        if(bufferWord[5] == 'n')
                                        {
                                            if(isitallow(bufferWord[6]) == 0)
                                            {
                                                printWord(bufferWord, wordCharacter);
                                            }
                                            else
                                            {
                                                printf("I found keyword return\n");
                                            }
                                        }
                                        else
                                        {
                                            printWord(bufferWord, wordCharacter);
                                        }
                                    }
                                    else
                                    {
                                        printWord(bufferWord, wordCharacter);
                                    }
                                }
                                else
                                {
                                    printWord(bufferWord, wordCharacter);
                                }
                            }
                            else
                            {
                                printWord(bufferWord, wordCharacter);
                            }
                        }
                        else
                        {
                            printWord(bufferWord, wordCharacter);
                        }
                    break;

                    case 'n':
                        if(bufferWord[1] == 'o')
                        {
                            if(bufferWord[2] == 'n')
                            {
                                if(bufferWord[3] == 'e')
                                {
                                    if(isitallow(bufferWord[4]) == 0)
                                    {
                                        printWord(bufferWord, wordCharacter);
                                    }
                                    else
                                    {
                                        printf("I found keyword none\n");
                                    }
                                }
                                else
                                {
                                    printWord(bufferWord, wordCharacter);
                                }
                            }
                            else
                            {
                                printWord(bufferWord, wordCharacter);
                            }
                        }
                        else
                        {
                            printWord(bufferWord, wordCharacter);
                        }
                    break;

                    case 't':
                        if(bufferWord[1] == 'r')
                        {
                            if(bufferWord[2] == 'u')
                            {
                                if(bufferWord[3] == 'e')
                                {
                                    if(isitallow(bufferWord[4]) == 0)
                                    {
                                        printWord(bufferWord, wordCharacter);
                                    }
                                    else
                                    {
                                        printf("I found keyword true\n");
                                    }
                                }
                                else
                                {
                                    printWord(bufferWord, wordCharacter);
                                }
                            }
                            else
                            {
                                printWord(bufferWord, wordCharacter);
                            }
                        }
                        else
                        {
                            printWord(bufferWord, wordCharacter);
                        }
                    break;

                    case 'd':
                        if(bufferWord[1] == 'e')
                        {
                            if(bufferWord[2] == 'f')
                            {
                                if(isitallow(bufferWord[3]) == 0)
                                {
                                    printWord(bufferWord, wordCharacter);
                                }
                                else
                                {
                                    printf("I found keyword def\n");
                                }
                            }
                            else
                            {
                                printWord(bufferWord, wordCharacter);
                            }
                        }
                        else
                        {
                            printWord(bufferWord, wordCharacter);
                        }
                    break;

                    case 'b':
                        if(bufferWord[1] == 'r')
                        {
                            if(bufferWord[2] == 'e')
                            {
                                if(bufferWord[3] == 'a')
                                {
                                    if(bufferWord[4] == 'k')
                                    {
                                        if(isitallow(bufferWord[5]) == 0)
                                        {
                                            printWord(bufferWord, wordCharacter);
                                        }
                                        else
                                        {
                                            printf("I found keyword break\n");
                                        }
                                    }
                                    else
                                    {
                                        printWord(bufferWord, wordCharacter);
                                    }
                                }
                                else
                                {
                                    printWord(bufferWord, wordCharacter);
                                }
                            }
                            else
                            {
                                printWord(bufferWord, wordCharacter);
                            }
                        }
                        else
                        {
                            printWord(bufferWord, wordCharacter);
                        }
                    break;

                    case 'l':
                        if(bufferWord[1] == 'e')
                        {
                            if(bufferWord[2] == 'n')
                            {
                                if(isitallow(bufferWord[3]) == 0)
                                {
                                    printWord(bufferWord, wordCharacter);
                                }
                                else
                                {
                                    printf("I found keyword len\n");
                                }
                            }
                            else
                            {
                                printWord(bufferWord, wordCharacter);
                            }
                        }
                        else
                        {
                            printWord(bufferWord, wordCharacter);
                        }
                    break;

                    case 's':
                        if(bufferWord[1] == 'u')
                        {
                            if(bufferWord[2] == 'b')
                            {
                                if(bufferWord[3] == 's')
                                {
                                    if(bufferWord[4] == 't')
                                    {
                                        if(bufferWord[5] == 'r')
                                        {
                                           if(isitallow(bufferWord[6]) == 0)
                                            {
                                                printWord(bufferWord, wordCharacter);
                                            }
                                            else
                                            {
                                                printf("I found keyword substr\n");
                                            } 
                                        }
                                        else
                                        {
                                            printWord(bufferWord, wordCharacter);
                                        }
                                    }
                                    else
                                    {
                                        printWord(bufferWord, wordCharacter);
                                    }
                                }
                                else
                                {
                                    printWord(bufferWord, wordCharacter);
                                }
                            }
                            else
                            {
                                printWord(bufferWord, wordCharacter);
                            }
                        }
                        else
                        {
                            printWord(bufferWord, wordCharacter);
                        }
                    break;

                    default:
                        
                            /*zde se tiskne to, co není KW a ani se k tomu neblíží*/
                            printWord(bufferWord, wordCharacter);
                            cleanFlag = 1;
                        
                    break;
                    }//konec switche
                    if(cleanFlag == 1)
                    {
                        clearBuffer(bufferWord);
                    }

                    return 0;
                }
            }
        }//konec Foru
    }//konec While
    return 0;
}//konec funkce
                     




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

    
    ptrToken = (pointerToken*) malloc(sizeof(pointerToken));

    errorCaller = lineCutter(programIFJ19);

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