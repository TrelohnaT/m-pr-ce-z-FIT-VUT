/* ----------------------------------------------
	Předmět: IFJ (+ IAL)
	Tým: 107 var. I
	Autor:
		Dolecek, Vaclav (xdolec03)
-------------------------------------------------
		Jaksik, Ales (xjaksi01)
        Pospisil, Filip (xpospi0f)
        Vlasakova, Nela (xvlasa14)
-------------------------------------------------
 Datum: 23. 11. 2019
 Účel: Lexikální analýza
 Popis: Lexikální analýza
 ------------------------------------------------
		      (c) UniSoftCorn
 ----------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "lex_anal.h"

/*definované stavy scenneru*/
#define stateStart 101          //done
#define stateWord 102           //done
#define stateNumber 103         //done
#define stateEol 104            //done
#define stateComent 105         //done
#define stateLess 106           //done
#define stateBigger 107         //done
#define stateExclamMark 108     //done
#define stateEscape 109         //done
#define stateEq 110             //done
#define stateSpace 111          //done
#define stateString 112         //done
#define stateEscapeHex_1 113    //done
#define stateEscapeHex_2 114    //done
#define stateIndent 115         //done
#define stateDedent 116         //done
#define stateStringBig 117      //

 #define varLenght     10 //začáteční délka varStringu


/*smaže obsah*/
void varStringClean(Token *vr)
{
    /*delka bude 0*/
    vr->lenght = 0;
    /*a strig je prázdný*/
    vr->str[vr->lenght] = '\0';
}

/*uvolní paměť zabíranou proměnlivým stringem*/
void varStringDestroy(Token *vr)
{
    /*uvolnění paměti*/
    free(vr->str);
}

/*vloží do proměnlivého stringu char*/
bool varStringChar(Token *vr, char c)
{
    /*pokud délka stringu větší než alokované místo*/
   if(vr->lenght+1 > vr->space)
   {
       /*vypočítá se nová velikost*/
       unsigned int bigger_space = vr->lenght + varLenght;
       /*reallokace místa pro string*/
       vr->str = (char *) realloc(vr->str, bigger_space);
       /*pokud se to nepovede*/
       if(vr->str == NULL)
       {
           return false;    //vrátím false
       }
       /*přepíše se allokované velikost*/
       vr->space = bigger_space;
   }
   /*vloží se charakter do stringu a přičte se délka*/
   vr->str[vr->lenght++] = c;
   vr->str[vr->lenght] = '\0';

   return true;     //vrátím NULL
}

/*vytvořím proměnlivý string*/
bool varStringInit(Token *vr)
{
    /*alokuji místo pro string*/
    vr->str = (char *) malloc(varLenght);
    /*pokud se alokace nepovedla*/
    if(vr->str == NULL)
    {
        return false;   //varíc false
    }
    /*vyčitím allokované místo*/
    varStringClean(vr);
    /*zapíši velikost alokovaného prostoru*/
    vr->space = varLenght;
    return true;    //vracím true
}

/*vloží se string do varString*/
bool varStringConstChar(Token *vr, const char *conChar)
{
    /*změříme délku vládaného stringu*/
    unsigned int conStringLenght = (unsigned int) strlen(conChar);
    /*pokud je delší než allokované místo*/
    if(vr->lenght + conStringLenght+1 >= vr->space)
    {
        /*reallokuji místo pro nový string*/
        vr->str = (char *) realloc(vr->str,vr->lenght + conStringLenght+1);
        /*pokud se allokace nepovedla*/
        if(vr->str == NULL)
        {
            return false;   //vracím false
        }
        /*zapíšu si velikost nově allokovaného místa*/
        vr->space = vr->lenght + conStringLenght+1;
    }
    /*přidám délku nového řetězce do celkové délky*/
    vr->lenght = vr->lenght + conStringLenght;
    /*načtu samotný řetězec*/
    strcat(vr->str, conChar);
    /*poslední znak bude ukončovací*/
    vr->str[vr->lenght] = '\0';

    return true;    //vracím true

}

/*porovná řetězec s řetězce uvniřt varStringu*/
int varStringCmp(Token *vr, const char *conChar)
{
    return strcmp(vr->str, conChar);
}

/*tatot funkce sklopíruje string z jednoho tokenu do druhého*/
bool varStringCopy(Token *src, Token *des)
{ 
    unsigned int newSpace = src->lenght;
    /*kontrola zda je dostatek místa pro nový string*/
    if(newSpace >= des->space)
    {
        /*realokace*/
        des->str = (char *) realloc(des->str, newSpace +5);
        if(des->str == NULL)
        {
            return false;
        }
        des->space = newSpace + des->str + 5;
    }
    /*následné kopírování*/
    strcpy(des->str, src->str);
    des->lenght = newSpace;
    return true;
}


/*myslím že nazev mluví za vše*/
    static int eolFlag = 0;

/*základní funkce*/
int getToken(Token *token)
{
    /*kontrola ukazatele na soubor*/
    if(stdin == NULL)
    {
        fprintf(stderr,"Na stdin není nic, co bych čísti mohl což zapříčinilo tuto chybu\n");
       return ERROR_COMPILATOR;     //vrátím chybu
    }

    /*ukazatel na vrchol*/
    static int scale = 0;
    /*zásobník*/
    static int dentStack[101];
    /*počítadlo mezer*/
    static int dentActuall = 0;
    /*místo pro hodnotu stavu*/
    static int state = stateStart;
    /*tolik dedentu bude vráceno*/
    static int dedentRest = 0;

    static int stringBigcont = 0;

    static int floatFlag = 0;



    floatFlag = 0;

    char character[2];
    /*nastavení dna zásobníku*/
    dentStack[0] = 0;

    /*pokud se inicializace VariableStringu nepovede*/
    if(varStringInit(token) == false)
    {
        fprintf(stderr, "Nepovedla se inicializace Variabilního Stringu\n");
        return ERROR_COMPILATOR;
    }
    /*deklarace a inicializace*/
    //token->string = var_string;
    /*nastavení vychozího stavu pro tokenType*/
    token->tokenType = TTempty;

    /*hlavní čtecí ciklus*/
    while (true)
    {
        
        /*znak je ze souboru nahrád do promněné character*/
        character[0] = (char) getc(stdin);
        /*automat:
        **zde se přepínají jednotlivé stavy deterministického automatu
        */
        switch (state)
        {
        /*výchozí stav, je nastaven už ze startu nebo po dočnetí řádku*/
        case stateStart:
            /*pokud je znak mezera*/
            if(character[0] == 32 && character[0] != 13)
            {
                /*je nastaven stav pro obsluhu mezer*/
                state = stateSpace;
                if(eolFlag == 1)
                {
                    dentActuall++;
                }
                /*aktualní znak je vrácen zpět do čteného streamu pro další zpracování*/
                ungetc(character[0], stdin);
            }
            /*pokud je znak písmeno nebo číslo*/
            else if(isalnum(character[0]) != 0)
            {
                if(dentActuall >= 0 && eolFlag == 1)
                {
                    state = stateSpace;
                    if(token->tokenType != TTdedent)
                    {
                        ungetc(character[0], stdin);
                    }
                    break;
                }
                /*pokud je znak písmeno*/
                else if(isalpha(character[0]) != 0 || character[0] == '_')
                {
                    /*je uloženo do variabilního stringu pro další zpracování
                    **pokud se vložení nezadaří*/
                    if(varStringChar(token, character[0]) == false)
                    {
                        /*je variabilníString vyčištěn*/
                        varStringDestroy(token);
                        /*a vrací se chyba*/
                        fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                        return ERROR_COMPILATOR;
                    }
                    /*pokud však ano, je změněn stav pro další zpracovávání*/
                    state = stateWord;
                    if(dentActuall < 0 && eolFlag != 1)
                    {
                        ungetc(character[0], stdin);
                    }
                }
                /*pokud je znak číslo*/
                else if (isdigit(character[0]) != 0)
                {
                    /*je uloženo do variabilního stringu pro další zpracování
                    **pokud se vložení nezadaří*/
                    if(varStringChar(token, character[0]) == false)
                    {
                        /*je variabilníString vyčištěn*/
                        varStringDestroy(token);
                        /*a vrací se chyba*/
                        fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                        return ERROR_COMPILATOR;
                    }
                    /*pokud však ano, je změněn stav pro další zpracovávání*/
                    //ungetc(character[0], stdin);
                    state = stateNumber;
                }
            }
            /*pokud je znak znak komentáře*/
            else if(character[0] == '#')
            {
                /*je změněn stav pro další zpracovávání*/
                state = stateComent;
            }
            /*pokud je znak znak nového řádku*/
            else if(character[0] == 13 || character[0] == 10)
            {
                /*je změněn stav pro další zpracovávání*/
                state = stateEol;
                //ungetc(character[0], stdin);
            }
            /*pokud je znak plus*/
            else if(character[0] == '+')
            {
                /*token bude typu plus*/
                token->tokenType = TTplus;
                /*token neobsahuje klíčové slovo*/
                token->keyWord = KWnotKW;
                return NOERROR_OK;
            }
            /*pokud je znak mínus*/
            else if(character[0] == '-')
            {
                /*token bude typu mínus*/
                token->tokenType = TTminus;
                /*tokne neobsahuje klíčové slovo*/
                token->keyWord = KWnotKW;
                return NOERROR_OK;
            }
            /*pokud je znak krát*/
            else if(character[0] == '*')
            {
                /*token bude typu mul*/
                token->tokenType = TTmul;
                /*token neobsahuje klíčové slovo*/
                token->keyWord = KWnotKW;
                return NOERROR_OK;
            }
            /*pokud je znak čárka*/
            else if(character[0] == ',')
            {
                /*token bude typu comma*/
                token->tokenType = TTcomma;
                /*token neobsahuje klíčové slovo*/
                token->keyWord = KWnotKW;
                return NOERROR_OK;
            }
            /*pokud je znak krát*/
            else if(character[0] == '/')
            {
                /*tokne bude typu div*/
                token->tokenType = TTdiv;
                /*token neobsahuje klíčové slovo*/
                token->keyWord = KWnotKW;
                return NOERROR_OK;
            }
            /*pokud má znak hodnotu 92 je ASCII hodnota pro \
            **jelikož tento znak stojí mimo string, neberu to jako escape sekvenci*/
            else if(character[0] == 92)
            {
                /*uloží se tento znak do variabliního stringu
                **pokud se tak nepovede*/
                if(varStringChar(token, character[0]) == false)
                {
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    return ERROR_COMPILATOR;
                }
                /*přejdu do stavu pro slovo*/
                state = stateWord;
            }
            /*pokud je znak většítko*/
            else if(character[0] == '>')
            {
                /*přejdu do stavu pro většítko*/
                state = stateBigger;
            }
            /*pokud je znak menšítko*/
            else if(character[0] == '<')
            {
                /*přejdu do stavu pro menšítko*/
                state = stateLess;
            }
            /*pokud je znak rovnítko*/
            else if(character[0] == '=')
            {
                /*přejdu do stavu pro rovnítko*/
                state = stateEq;
            }
            /*pokud je znak levá závorka*/
            else if(character[0] == '(')
            {
                /*token bude typu bracketLeft*/
                token->tokenType = TTbracketLeft;
                /*token neobsahuje klíčové slovo*/
                token->keyWord = KWnotKW;
                return NOERROR_OK;
            }
            /*pokud je znak pravá závorka*/
            else if(character[0] == ')')
            {
                /*tokne bude typu bracketRight*/
                token->tokenType = TTbracketRight;
                /*token neobsahuje klíčové slovo*/
                token->keyWord = KWnotKW;
                return NOERROR_OK;
            }
            /*pokud je znak dvojtečka*/
            else if(character[0] == ':')
            {
                /*token bude typu colon*/
                token->tokenType = TTcolon;
                /*token neobsahuje klíčové slovo*/
                token->keyWord = KWnotKW;
                state = stateStart;
                //ungetc(character[0], stdin);
                return NOERROR_OK;
            }
            /*pokud je znak vykřičník*/
            else if(character[0] == '!')
            {
                /*přepne se stav pro obsloužení tohoto znaku*/
                state = stateExclamMark;
            }
            /*pokd má znak hotnotu 39 což je ASCII  pro ' tedy začátek stringu*/
            else if(character[0] == 39)
            {
                /*přepne se stav pro obsloužení tohot znaku*/
                if(varStringChar(token, '\'') == false)
                {
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    return ERROR_COMPILATOR;
                }
                state = stateString;
            }
            else if(character[0] == '"')
            {
                state = stateEscape;
                ungetc(character[0], stdin);
            }
            /*pokud nic nevyhovuje, něco se podělalo*/
            else if(character[0] == EOF)
            {
                /*pokud už nezbyl žádný dedent, končím*/
                if(scale == 0)
                {
                    token->tokenType = TTEof;
                    token->keyWord = KWnotKW;
                    return NOERROR_OK;
                }
                /*jinak vracím dedenty*/
                else
                {
                    
                    ungetc(character[0], stdin);
                    token->tokenType = TTdedent;
                    token->keyWord = KWnotKW;
                    scale--;
                    return NOERROR_OK;
                }
                                             //pro testovací učely
            }
            else
            {
                /*je variabliní string zničen a vrácí se chyba*/
                return ERROR_LEX;
            }
        break;

        case stateEq:
            if(character[0] == '=')
            {
                token->tokenType = TTeq;

                token->keyWord = KWnotKW;

                state = stateStart;
                return NOERROR_OK;
            }
            else
            {
                token->tokenType = TTass;

                token->keyWord = KWnotKW;

                state = stateStart;
                return NOERROR_OK;
            }

        break;

        /*stav pro obsloužení mezery*/
        case stateSpace:
            /*byla nalezena nějaká mezera*/
            if((isalnum(character[0]) != 0) && (eolFlag == 1))
            {
                if(dentStack[scale] < dentActuall)
                {
                    state = stateIndent;
                    scale = scale + 1;
                    dentStack[scale] = dentActuall;
                    ungetc(character[0], stdin);
                }
                else if(dentStack[scale] > dentActuall)
                {
                    for(dedentRest = 1;dentActuall != dentStack[scale - dedentRest]; dedentRest++)
                    {
                        if(scale == dedentRest)
                        {
                            return ERROR_LEX;
                        }
                    }
                    state = stateDedent;
                    ungetc(character[0], stdin);

                }
                 else
                {
                    
                    state = stateStart;
                    dentActuall = 0;
                    eolFlag = 0;
                    ungetc(character[0], stdin);
                }
            }
            else
            {
                state = stateStart;
                //ungetc(character[0], stdin);
                if(eolFlag != 1)
                {
                    dentActuall = 0;
                }
            }

        break;

        case stateIndent:
            token->tokenType = TTindent;
            token->keyWord = KWnotKW;
            dentActuall = 0;
            state = stateStart;
            eolFlag = 0;
            ungetc(character[0],stdin);
            return NOERROR_OK;
        break;

        case stateDedent:
            token->tokenType = TTdedent;
            token->keyWord = KWnotKW;
            state = stateStart;
            eolFlag = 0;
            dentStack[scale] = 0;
            scale = scale - 1;
            if(dentActuall != dentStack[scale] - 1 && token->tokenType != TTdedent)
            {
                ungetc(character[0],stdin);
            }
            dentActuall = 0;
            if(dedentRest != 1 )
            {
                eolFlag = 1;
                state = stateDedent;
                dedentRest--;
            }
            return NOERROR_OK;
        break;

        /*stav pro obsluhu strigu*/
        case stateString:
            /*pokud je ASCII menší než 32 (což je mezera)*/
            if(character[0] < 32)
            {
                ungetc(character[0], stdin);
                state = stateStart;
            }
            /*pokud je zaznamenáno zpětné lomítko*/
            else if(character[0] == '\\')
            {
                /*stav je nastaven pro obsluhu escape sekvencí*/
                state = stateEscape;
            }
            /*pokud je ASCII 39, což je znak '*/
            else if(character[0] == 39)    //39 = ' (tedy konec strigu)
            {
                /*je to začátek stringu, takže je znaku uložen do Variabilního stringu*/
                if(varStringChar(token, character[0]) == false)
                {
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    return ERROR_COMPILATOR;
                }
                /*token je typu string*/
                token->tokenType = TTstring;
                /*token neobsahuje klíčové slovo*/
                token->keyWord = KWnotKW;
                state = stateStart;
                return NOERROR_OK;
            }
            else if(character[0] == 34)
            {
                if(varStringChar(token, character[0]) == false)
                {
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    return ERROR_COMPILATOR;
                }
                stringBigcont++;
                if(stringBigcont == 3)
                {
                    stringBigcont = 0;
                    state = stateStringBig;
                }
            }
            /*když to bude prostě písmeno*/
            else
            {
                /*tak to prostě uložím do Variabilního stringu*/
                if(varStringChar(token, character[0]) == false)
                {
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    return ERROR_COMPILATOR;
                }
            }
        break;

        /*pro obsluhu slov*/
        case stateWord:
            /*pokud je to to písmeno nebo číslo nebo podtržítko*/
            if(isalnum(character[0]) != 0 || character[0] == '_')
            {
                /*uložím to do Variabilního stringu*/
                if(varStringChar(token, character[0]) == false)
                {
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    return ERROR_COMPILATOR;
                }
            }
            /*pokud je to něco jíného, nejedná se o slovo*/
            else
            {
                /*stav je změněn na start*/
                state = stateStart;
                /*znak je vrácen do čtecího streamu*/
                ungetc(character[0], stdin);
                /*to co mám ve Variabilní stringu pošlu k analýze a pak končím*/
                return keyWordFinder(token);
            }
        break;

        /*pro obsluhu komenářů*/
        case stateComent:
            /*pokud narazím na znak konce řádku nebo souboru*/
            if(character[0] == '\n' || character[0] == EOF)
            {
                /*stav měním na start*/
                state = stateStart;
                /*a najdený znak vracím do streamu pro čtení*/
                //ungetc(character[0], stdin);
            }
        break;

        /*pro obsluhu většítka*/
        case stateBigger:
            /*pokud další znak je rovnítko*/
            if(character[0] == '=')
            {
                /*token je typu biggerEq*/
                token->tokenType = TTbiggerEq;
                /*token není klíčové slovo*/
                token->keyWord = KWnotKW;
                state = stateStart;
                /*končím s uspěchem*/
                return NOERROR_OK;
            }
            /*pokud je za ním něco jiného*/
            else
            {
                /*token je typu bigger*/
                token->tokenType = TTbigger;
                /*token není klíčové slovo*/
                token->keyWord = KWnotKW;
                /*znak je vrácen do čtecího streamu*/
                //ungetc(character[0], stdin);
                state = stateStart;
                /*končím s uspěchem*/
                return NOERROR_OK;
            }
        break;

        /*pro obsluhu menšítka*/
        case stateLess:
            /*pokud další znak je rovnítko*/
            if(character[0] == '=')
            {
                /*token je typu lessEq*/
                token->tokenType = TTlessEq;
                /*token neobsahuje klíčové slovo*/
                token->keyWord = KWnotKW;
                state = stateStart;
                /*končím s úspěchem*/
                return NOERROR_OK;
            }
            /*pokud je za ním něco jiného*/
            else
            {
                /*token je typu less*/
                token->tokenType = TTless;
                /*token neobsahuje klíčové slovo*/
                token->keyWord = KWnotKW;
                /*znak je vrácen do čtecího streamu*/
                //ungetc(character[0], stdin);
                state = stateStart;
                /*končím s uspěchem*/
                return NOERROR_OK;
            }
        break;

        /*pro obsluhu EOL*/
        case stateEol:
            /*pokud je znak mezera*/
            if(character[0] == 10)
            {
                break;
            }

            /*znak je vrácen do čtecího streamu*/
            if(eolFlag == 0 || token->tokenType == TTdedent)
            {
                ungetc(character[0], stdin);
            }
            if(isalpha(character[0]) != 0)
            {
                ungetc(character[0], stdin);
            }
            token->tokenType = TTEol;
            dentActuall = 0;
            state = stateStart;
            eolFlag = 1;
            return NOERROR_OK;
        break;

        /*pro obsluhu čísla*/
        case stateNumber:
            /*pokud je znak číslo*/
            if(isdigit(character[0]) != 0)
            {
                /*pokud se nepodaří ho vložit do variabilního stringu*/
                if(varStringChar(token, character[0]) == false)
                {
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    return ERROR_COMPILATOR;
                }
            }
            /*pokud je nalezena tečka nebo e*/
            else if(character[0] == '.' || tolower(character[0]) == 'e')
            {
                /*pokud se nepodaří ho vložit do variabilního stringu*/
                if(varStringChar(token, character[0]) == false)
                {
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    return ERROR_COMPILATOR;
                }
                /*token je typu Float*/
                token->tokenType = TTnumberFloat;
                /*token není klíčové slovo*/
                token->keyWord = KWnotKW;

                floatFlag = 1;
            }
            /*pokud najdu cokoliv jiného*/
            else
            {
                /*znak je vrácen do čtecího streamu*/
                ungetc(character[0], stdin);
                if(floatFlag != 1)
                {
                    token->tokenType = TTnumberInt;
                    token->keyWord = KWnotKW;
                }
                state = stateStart;
                return NOERROR_OK;
            }
        break;

        /*pro obsluhu vykřičníku*/
        case stateExclamMark:
            /*pokud je další znak rovnítko*/
            if(character[0] == '=')
            {
                /*token je typu tonEq*/
                token->tokenType = TTnotEq;
                /*neobsahuje klíčové slovo*/
                token->keyWord = KWnotKW;
                /*končím v pořádku*/
                return NOERROR_OK;
            }
            /*pokud tam stojí sám*/
            else
            {
                fprintf(stderr, "Račte býti informování, že samotný '!' není legální\n");
                /*vrátím to do načítacího stringu*/
                ungetc(character[0], stdin);
                /*vracím chybu*/
                return ERROR_LEX;
            }
        break;

        /*pro obsluhu escape sekvencí*/
        case stateEscape:
            /*pokud je to menší než 32 (což je mezera)*/
            if(character[0] < 32 )
            {
                fprintf(stderr, "Byval byl nalezen znak, jenž legálnosti postrádá\n");
                /*je variabliní string zničen a vrácí se chyba*/
                varStringDestroy(token);
                return ERROR_LEX;
            }
            /*pokud je narazím na uvozovky*/
            else if(character[0] == '"')
            {
                /*přiřadím \" do promněné charakter*/
                character[0] = '\"';
                /*pokud se mi toto nepodaří vložit do variabilního stringu*/
                if(varStringChar(token, character[0]) == false)
                {
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    return ERROR_COMPILATOR;
                }
                token->tokenType = TTstring;
                /*stav je změněn pro obsluhu stringu*/
                state = stateString;
                stringBigcont++;
            }
            /*pokud je znak roven ASCII hodnotě pro '*/
            else if(character[0] == 39)
            {
                /*přiřadím \' do promněné charakter*/
                character[0] = '\'';
                /*pokud se mi toto nepodaří vložit do variabilního stringu*/
                if(varStringChar(token, character[0]) == false)
                {
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    return ERROR_COMPILATOR;
                }
                /*stav je změněn pro obsluhu stringu*/
                state = stateString;
            }
            /*pokud je další znak n*/
            else if(character[0] == 'n')
            {
                /*přiřadím \n do promněné charakter*/
                character[0] = '\n';
                /*pokud se mi to nepodaří vložit do variabilního stringu*/
                if(varStringChar(token, character[0]) == false)
                {
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    return ERROR_COMPILATOR;
                }
                /*stav je zmněněn pro obsluhu stringu*/
                state = stateString;
            }
            /*pokud je další znak t*/
            else if(character[0] == 't')
            {
                /*přiřadím \t do promněné charakter*/
                character[0] = '\t';
                /*pokud se mi to nepodaří vloži do variabilního stringu*/
                if(varStringChar(token, character[0]) == false)
                {
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    return ERROR_COMPILATOR;
                }
                /*svat je změněn zpět na string*/
                state = stateString;
            }
            /*pokud je další znak \*/
            else if(character[0] == '\\')
            {
                //character == '\\';
                /*pokud se mi to nepodaří vložit do variabilního stringu*/
                if(varStringChar(token, character[0]) == false)
                {
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    return ERROR_COMPILATOR;
                }
                /*stav je změněn zpátky na string*/
                state = stateString;
            }
            /*pokud je další znak x*/
            else if(character[0] == 'x')
            {
                /*stav je změněn pro obsluhu HEX escape sekvencí*/
                state = stateEscapeHex_1;
            }
            /*pokud je to cokoliv jiného*/
            else
            {
                /*pokud se nepodaří vložit do variabilního stringu*/
                if(varStringChar(token, character[0]) == false)
                {
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    return ERROR_COMPILATOR;
                }
                /*stav je změněn zpět na string*/
                state = stateString;
            }
        break;

        case stateStringBig:
            if(character[0] != '"')
            {
                if(varStringChar(token, character[0]) == false)
                {
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    return ERROR_COMPILATOR;
                }
            }
            else
            {
                stringBigcont++;
                if(varStringChar(token, character[0]) == false)
                {
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    return ERROR_COMPILATOR;
                }
            }
            if(stringBigcont == 3)
            {
                state = stateStart;
                return NOERROR_OK;
            }

        break;
        /*pro obsluhu HEX escape sekvence (první hex znak)*/
        case stateEscapeHex_1:
            /*pokud je to hex číslo*/
            if(isdigit(character[0]) != 0 || (tolower(character[0]) > 96 && tolower(character[0]) < 103))
            {
                /*pokud se mi to nepodaří vložit do variabilního stringu*/
               if(varStringChar(token, character[0]) == false)
                {
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    return ERROR_COMPILATOR;
                }
                /*jinak pokračuji*/
                state = stateEscapeHex_2;
            }
            /*pokud to není hex číslo*/
            else
            {
                fprintf(stderr, "Vězte, že při volání escape sekvence byl nalezen znak, jenž neodpovídá HEX číslu\n");
                /*je variabliní string zničen a vrácí se chyba*/
                varStringDestroy(token);
                return ERROR_COMPILATOR;
            }
        break;

        /*pro obsluhu HEX escape sekvence (druhý hex znak)*/
        case stateEscapeHex_2:
            /*pokud to je hex číslo*/
            if(isdigit(character[0]) != 0 || (tolower(character[0]) > 96 && tolower(character[0]) < 103))
            {
                /*pokud se nepodaří vložit do variabliního stringu*/
               if(varStringChar(token, character[0]) == false)
                {
                    fprintf(stderr, "Nepovedlo se vložení znaku do Variabilního Stringu\n");
                    /*je variabliní string zničen a vrácí se chyba*/
                    varStringDestroy(token);
                    return ERROR_COMPILATOR;
                }
                /*stav bude zae string*/
                state = stateString;
            }
            /*pokud to není hex číslo*/
            else
            {
                fprintf(stderr, "Vězte, že při volání escape sekvence byl nalezen znak, jenž neodpovídá HEX číslu\n");
                /*je variabliní string zničen a vrácí se chyba*/
                varStringDestroy(token);
                return ERROR_COMPILATOR;
            }
        break;

        /*pokud nic z tohoto, něco se solidně posralo*/
        default:
            /*decentní připomenutí*/
            return ERROR_LEX;
        break;
        }
    }
}


/*zjistí zda je příchozí string klíčové slovo*/
int keyWordFinder (Token *token)
{
    if(varStringCmp(token, "if") == 0)
    {
        token->keyWord = KWif;
    }
    else if(varStringCmp(token, "def") == 0)
    {
        token->keyWord = KWdef;
    }
    else if(varStringCmp(token, "else") == 0)
    {
        token->keyWord = KWelse;
    }
    else if(varStringCmp(token, "none") == 0)
    {
        token->keyWord = KWnone;
    }
    else if(varStringCmp(token, "pass") == 0)
    {
        token->keyWord = KWpass;
    }
    else if(varStringCmp(token, "return") == 0)
    {
        token->keyWord = KWreturn;
    }
    else if(varStringCmp(token, "while") == 0)
    {
        token->keyWord = KWwhile;
    }
    else if(varStringCmp(token, "break") == 0)
    {
        token->keyWord = KWbreak;
    }
    else
    {
        /*token bude typu identifier*/
        token->tokenType = TTidentifier;
        /*token nebude obsahovat klíčové slovo*/
        token->keyWord = KWnotKW;
    }
    /*pokud token není identifikátor*/
    if(token->tokenType != TTidentifier)
    {
        /*je to typ klíčové slovo*/
        token->tokenType = TTkeyWord;
    }
    /*je variabliní string zničen a vrácí se nechyba*/
    eolFlag = 0;
    return NOERROR_OK;
}
