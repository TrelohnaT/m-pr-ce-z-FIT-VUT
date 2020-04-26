
/*zde jsou deklarovány a implementovány funkce ke kompileru*/
/*pro projekt IFJ*/
#include <stdio.h>
#include <stdlib.h>
//#include "lex_anal.c"

/*funkce pro přístup*/
char lineCutter(FILE *filename);

/*všechna klíčová slova která se vyskytují v Pythonu a které tedy budou použity v tokenech*/
typedef enum
{
    KWdef,
    KWelse,
    KWif,
    KWnone,
    KWpass,
    KWreturn,
    KWwhile,
    KWelif,
    KWinputs,
    KWinputi,
    KWinputf,
    KWprint,
    KWlen,
    KWsubstr,
    KWcontinue,
    KWbreak,
    KWfor,

} tKeyWord;

/*Všechny možnosti tokenu*/
typedef enum
{
    TTminus,
    TTplus,
    TTmul,
    TTdiv,

    TTbigger,
    TTless,
    TTbiggerEq,
    TTlessEq,
    TTeq,
    TTnotEq,

    TTbracketLeft,
    TTbracketRight,
    TTdoubleSlash,

    TTindentifier,
    TTnumberFloat,
    TTnumberInt,
    TTstring,

} tTokenType;

/*token samotný*/
typedef struct tToken
{
    tTokenType tokenType;
    tKeyWord keyWord;

    char string;
    int intNum;
    double doubleNum;
      
    struct tToken *nextToken;

} *Token;

typedef struct 
{
    Token Act;
} pointerToken;

/*funkce pro Token*/

/*inicializace ukazatele na token*/
void tokenInit(pointerToken *pt);
/*chybové volání*/
void Error();
/*plnění tokenu*/
void tokenMake(pointerToken *pt, tTokenType TT, tKeyWord KW, char str, int iNum, double dNum);


/*funkce pro čtení samotné*/
char lineCutter(FILE *filename);



