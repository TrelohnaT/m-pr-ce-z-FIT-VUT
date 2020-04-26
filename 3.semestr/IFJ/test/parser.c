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
 Popis: Parser
 ------------------------------------------------
		      (c) UniSoftCorn
 ----------------------------------------------*/

#include "parser.h"
#include "errors.h"

bool localTab = false;


/**
 * Hlavni funkce parseru, podle LL-gramatiky kontroluje <body>
 */
int parse()
{
    tBSTNodePtr symTab;
    tBSTNodePtr localTree;

    BSTInit(&symTab);
    BSTInit(&localTree);

    // pridani vestavenych funkci
    buildInFunc(&symTab);

    // vytvoreni tokenu
    Token *token;
    token = (Token *) malloc(sizeof(Token));

    int result = body(token, &symTab, &localTree);

    BSTDispose(&symTab);
    BSTDispose(&localTree);

    return result;
}


/**
 * Ve funkci body se
 * vytvari funkce,
 * vola prikaz,
 * je prazdny radek,
 * nebo konec
 */
int body(Token *token, tBSTNodePtr *symTab, tBSTNodePtr *localTree)
{
    int result;

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }

    while (token->tokenType != TTEof)
    {
        if (token->keyWord == KWdef)  // def
        {
            result = funcDef(token, symTab, localTree);
        }
        else if (token->tokenType == TTEol)   // eol/line
        {
            continue;
        }
        else
        {
            result = command(token, symTab, localTree);
        }

        if (result != NOERROR_OK)
        {
            return result;
        }
        /* ----------------------- get token ----------------------- */
        if ((result = getToken(token)) != NOERROR_OK)
        {
            return result;
        }
    }

    return result;
}

/**
 * Kontrola zda se jedna o:
 *  id, if, nebo while
 */
int command(Token *token, tBSTNodePtr *symTab, tBSTNodePtr *localTree)
{
    int result;

    if (token->keyWord == KWif)  // if
    {
        result = cIf(token, symTab, localTree);
    }
    else if (token->keyWord == KWwhile)   // while
    {
        result = cWhile(token, symTab, localTree);
    }
    else if (token->tokenType == TTidentifier) // id
    {
        result = id(token, symTab, localTree);
    }
    else
    {
        result = ERROR_SEM;
    }

    return result;
}

/**
 * Objevil se identikator
 * Pokud nasleduje:
 *  zavorka, jedna se o funkci
 *  rovnase, prirazuji <id> nebo <item>
 *  eol, ano beru v uvahu, ze jen tak stoji
 */
int id(Token *token, tBSTNodePtr *symTab, tBSTNodePtr *localTree)
{
    int result;
    int err;
    tBSTNodePtr exist;

    // pokud identifikator kontroluji jeho existenci
    if (localTab)
    {
        exist->TBSNodeCont = BSTSearch(symTab, token->str);
    }
    else
    {
        exist->TBSNodeCont = BSTSearch(localTree, token->str);
    }

    // pamatovat klic pro pozdejsi doplneni informaci
    char *addPar = token->str;

    // pokud existuje
    if (exist->TBSNodeCont != NULL)
    {

        // zjistuji zda se jedna o volani funkce
        if (exist->TBSNodeCont->nType == ntFunc)
        {
            // kontrola volani funkce
            result = func(token, symTab, localTree);
            if (result != NOERROR_OK)
            {
                return result;
            }
        }
        // jinak se jedna o var
        else
        {
            // na radku muze stat ale nic se s nim nedeje
            /* ----------------------- get token ----------------------- */
            if ((result = getToken(token)) != NOERROR_OK)
            {
                return result;
            }
            if (token->tokenType != TTEol)    // eol
            {
                return ERROR_SYN;
            }
        }


    }
    else
    {
        // definuji identifikator
        // ale pokud jsem ve funkci bude lokalni tabulka
        if (localTab)
        {
            err = BSTInsert(localTree, token->str, createCont(ntVar, 0, true, typeNo));
        }
        else
        {
            err = BSTInsert(symTab, token->str, createCont(ntVar, 0, true, typeNo));
        }

        if (err != NOERROR_OK)
        {
            return err;
        }

        /* ----------------------- get token ----------------------- */
        if ((result = getToken(token)) != NOERROR_OK)
        {
            return result;
        }

        if (token->tokenType == TTEol)    // eol
        {
            result = NOERROR_OK;
        }
        else if (token->tokenType == TTass)    // rovnitko
        {
            // divam se co nasleduje
            /* ----------------------- get token ----------------------- */
            if ((result = getToken(token)) != NOERROR_OK)
            {
                return result;
            }

            // pokud se jedna o identifikator kontroluji zda definovany
            if (token->tokenType == TTidentifier)
            {
                // pokud je definovany zjistim zda var nebo func
                // pokud ne chyba
                // pokud var -> expression
                // pokud func -> func()
                // tam bude zkontrolovan konec radku
                tBSTNodePtr je_func;
                if (localTab)
                {
                    je_func->TBSNodeCont = BSTSearch(symTab, token->str);
                }
                else
                {
                    je_func->TBSNodeCont = BSTSearch(localTree, token->str);
                }

                    if (je_func->TBSNodeCont != NULL)
                    {
                        if (je_func->TBSNodeCont->nType == ntFunc)  // jedna se o funkci
                        {
                            result = func(token, symTab, localTree);
                        }
                        else
                        {
                            // definovany var, pasilam na kontrolu vyrazu
                            result = processToken(token, symTab, localTree);
                            // aktualizuji datovy typ
                            BSTContActual(symTab, addPar, finalType, 0);
                        }
                    }
                    else
                    {
                        return ERROR_SEM_VAR;   // prirazuji nedefinovany identifikator
                    }
                }

                // pokud to neni identifikator poslu na kontrolu
                else
                {
                    result = processToken(token, symTab, localTree);
                }
            }
        else
        {
            result = ERROR_SEM;
        }
    }
    return result;
}





/**
 * Kontrola zda je spravne napsana definice funkce
 */
int funcDef(Token *token, tBSTNodePtr *symTab, tBSTNodePtr *localTree)
{
    int result;

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }

    if (token->tokenType != TTidentifier) // id
    {
        return ERROR_SYN;
    }

    // definice probehla ok pridam do tabulky  
    result = BSTInsert(symTab, token->str, createCont(ntFunc, 0, true, typeNo));
    // kontrola uspesneho vlozani
    if (result != NOERROR_OK)
    {
        return result;
    }

    // zapamatji si id funkce pro pozdejsi doplneni informaci
    char *add = token->str;

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }
    if (token->tokenType != TTbracketLeft)     // leva zavorka
    {
        return ERROR_SYN;
    }

    // pokud dalsi neni prava zavorka zjistuji pocet parametru
    int countPar = 0; // pocet parametru funkce
    int countComma = 0; // pocet carek oddelujicich parametry

    while (token->tokenType != TTbracketRight)
    {
        /* ----------------------- get token ----------------------- */
        if ((result = getToken(token)) != NOERROR_OK)
        {
            return result;
        }

        if (token->tokenType == TTEof)
        {
            return ERROR_SYN;
        }

        if (token->tokenType == TTidentifier || token->tokenType == TTnumberFloat || token->tokenType == TTnumberInt || token->tokenType == TTstring)
        {
            countPar++;
        }
        else if (token->tokenType == TTcomma)     // carka
        {
            countComma++;
        }
    }

    // pokud ma funkce parametry, je pocet carek o jednu mene nez parametru
    if ((countPar != (countComma+1)) && (countPar != 0))
    {
        return ERROR_SYN;
    }

    if (token->tokenType != TTbracketRight)      // prava zavorka
    {
        return ERROR_SYN;
    }
    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }

    if (token->tokenType != TTcolon)      // dvojtecka
    {
        return ERROR_SYN;
    }

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }

    if (token->tokenType != TTEol)      // konec radku
    {
        return ERROR_SYN;
    }
    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }

    if (token->tokenType != TTindent)      // INDENT
    {
        return ERROR_SYN;
    }

    // funkce pro kontrolu tela definice
    result = def_body(token, symTab, localTree);

    if (result != NOERROR_OK)
    {
        return result;
    }

    // aktualizace hodnot v symtab
    BSTContActual(symTab, add, finalType, countPar);


    return result;

}

/**
 * Funkce kontroluje telo funkce
 */
int def_body(Token *token, tBSTNodePtr *symTab, tBSTNodePtr *localTree)
{
    int result;

    // nachazim se ve funkci
    // id se mi bude ukladat do lokalni tabulky
    localTab = true;

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }

    while ((token->tokenType != TTdedent) && (token->keyWord != KWreturn) && (token->tokenType != TTEof))
    {
        if (token->tokenType == TTEol)   // eol/line
        {
             continue;
        }
        else
        {
            result = command(token, symTab, localTree);
            if (result != NOERROR_OK)
            {
                return result;
            }
        }


        /* ----------------------- get token ----------------------- */
        if ((result = getToken(token)) != NOERROR_OK)
        {
            return result;
        }
    }

    if (token->keyWord == KWreturn)      // return
    {
        /* ----------------------- get token ----------------------- */
        if ((result = getToken(token)) != NOERROR_OK)
        {
            return result;
        }
        if (token->tokenType == TTidentifier)
        {
            // pokud identifikator kontroluji jeho existenci
            tBSTNodePtr je_funkce;
            je_funkce->TBSNodeCont = BSTSearch(symTab, token->str);

            // pokud neexistuje, chyba
            if (je_funkce->TBSNodeCont == NULL)
            {
                return ERROR_SEM_VAR;
            }
            if (je_funkce->TBSNodeCont->nType == ntFunc)  // jedna se o funkci
            {
                return ERROR_SYN;
            }
            else
            {
                // definovany var, pasilam na kontrolu vyrazu
                result = processToken(token, symTab, localTree);
            }
            /* ----------------------- get token ----------------------- */
            if ((result = getToken(token)) != NOERROR_OK)
            {
                return result;
            }
            if (token->tokenType != TTdedent)
            {
                return ERROR_SYN;
            }
        }

        // pokud to neni identifikator jeste muze jit o NONE
        else if (token->keyWord == KWnone)
        {
            /* ----------------------- get token ----------------------- */
            if ((result = getToken(token)) != NOERROR_OK)
            {
                return result;
            }

            // ted ocekavam pouze konec radku
            if (token->tokenType != TTEol)
            {
                return ERROR_SYN;
            }
            /* ----------------------- get token ----------------------- */
            if ((result = getToken(token)) != NOERROR_OK)
            {
                return result;
            }
            if (token->tokenType != TTdedent)
            {
                return ERROR_SYN;
            }
        }

        // jinak to poslu na kontrolu
        else
        {
            result = processToken(token, symTab, localTree);
        }
    }
    else if (token->tokenType != TTdedent)
    {
        return ERROR_SYN;
    }

    // na konci tela definice funkce uvolnim lokalni var
    BSTDispose(localTree);
    localTab = false;

    return result;
}




/**
 * Funkce pro volani funkce
 * Prijde id o kterem se zjistilo ze je funkce
 * kontroluje pocet parametru funkce a syntax
 */
int func(Token *token, tBSTNodePtr *symTab, tBSTNodePtr *localTree)
{
    int result;
    tBSTNodePtr function;
    // znova si ji najdu, token se nezmenil
    if (localTab)
    {
        function->TBSNodeCont = BSTSearch(symTab, token->str);
    }
    else
    {
        function->TBSNodeCont = BSTSearch(localTree, token->str);
    }
    int noParam = function->TBSNodeCont->noParams;


    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }

    // vzdy zaciname zavorkou
    if (token->tokenType != TTbracketLeft)
    {
        // pokud neni, je zle
        return ERROR_SYN;
    }

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }

    // pomocne promenne pro pocitani parametru a carek
    int param = 0;
    int comma = 0;

    // cyklus kontroluje pocet parametru
    // konci pokud dosel prave zavorky nebo nejsou tokeny
    while ((token->tokenType != TTbracketRight) && (token->tokenType != TTEof))
    {
        // pokud najdu identifikator, zjistim zda je ok
        if (token->tokenType == TTidentifier)
        {
            tBSTNodePtr exist;
            if (localTab)
            {
                exist->TBSNodeCont = BSTSearch(symTab, token->str);
            }
            else
            {
                exist->TBSNodeCont = BSTSearch(localTree, token->str);
            }

            if (exist->TBSNodeCont == NULL)
            {
                return ERROR_SEM_VAR;
            }
            else
            {
                // zjistim zda je to var
                if (exist->TBSNodeCont->nType == ntVar)
                {
                    // pak prictu to pocitani parametru
                    param++;
                }
                else
                {
                    return ERROR_SEM_VAR;
                }
            }
        }

        // pokud je to item
        else if (token->tokenType == TTidentifier || token->tokenType == TTnumberFloat || token->tokenType == TTnumberInt || token->tokenType == TTstring)
        {
            param++;
        }

        // je to carka
        else if (token->tokenType == TTcomma)
        {
            comma++;
        }
        else
        {
            return ERROR_SEM_ARG;
        }

        /* ----------------------- get token ----------------------- */
        if ((result = getToken(token)) != NOERROR_OK)
        {
            return result;
        }
    }

    // pokud pocitani ani nezacalo
    if ((param == 0) && (comma == 0))
    {
        // kontroluji zda se shoduje pocet parametru
        if (noParam != 0)
        {
            return ERROR_SEM_VAR;
        }

    }

    // pocet carek bude o jednu mene nez argumentu
    else if (param == comma+1)
    {
        // kvuli funkci typu print(), ta muze mit neomezene argumentu (-1)
        if (noParam != -1)
        {
            // jinak kontroluji jejich pocet
            if (noParam != param)
            {
                return ERROR_SEM_ARG;
            }
        }
    }
    else
    {
        return ERROR_SEM_VAR;
    }


    // tak musi by prava zavorka
    if (token->tokenType == TTbracketRight)
    {
        return NOERROR_OK;
    }
    else
    {
        return ERROR_SYN;
    }
}

/**
 * Funkce kontrolujici volani if
 */
int cIf(Token *token, tBSTNodePtr *symTab, tBSTNodePtr *localTree)
{
    int result;

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }

    // musi se jednat o expression

    // je identifikator
    if (token->tokenType == TTidentifier)
    {
        tBSTNodePtr varr;
        if (localTab)
        {
            varr->TBSNodeCont = BSTSearch(symTab, token->str);
        }
        else
        {
            varr->TBSNodeCont = BSTSearch(localTree, token->str);
        }

        // existuje?
        if (varr->TBSNodeCont == NULL)
        {
            return ERROR_SEM_VAR;
        }

        // pokud funkce, velka spatna
        if (varr->TBSNodeCont->nType == ntVar)
        {
            result = processToken(token, symTab, localTree);
        }
        else
        {
            return ERROR_SYN;
        }
    }

    // je leva zavorka
    else if (token->tokenType == TTbracketLeft)
    {
        result = processToken(token, symTab, localTree);
    }

    // jikak je to zle
    else
    {
        if (token->tokenType == TTnumberFloat || token->tokenType == TTnumberInt || token->tokenType == TTstring)
        {
            processToken(token, symTab, localTree);
        }
        else
        {
            return ERROR_SYN;
        }
    }

    // pokud expression obevi neco necekaneho
    if (result != NOERROR_OK)
    {
        return result;
    }


    // v expression konci dvojteckou overim si
    if (token->tokenType != TTcolon)        // dvojtecka
    {
        return ERROR_SYN;
    }

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }
    if (token->tokenType != TTEol)          // eol
    {
        return ERROR_SYN;
    }

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }
    if (token->tokenType != TTindent)          // indent
    {
        return ERROR_SYN;
    }

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }

    // bude nasledovat telo funkce
    result = commandBody(token, symTab, localTree);
    if (result != NOERROR_OK)
    {
        return result;
    }

    // doslo az k dedentu, kontrola
    if (token->tokenType != TTdedent)          // dedent
    {
        return ERROR_SYN;
    }

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }
    if (token->keyWord != KWelse)          // else
    {
        return ERROR_SYN;
    }

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }
    if (token->tokenType != TTcolon)          // dvojtecka
    {
        return ERROR_SYN;
    }

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }
    if (token->tokenType != TTEol)          // eol
    {
        return ERROR_SYN;
    }

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }
    if (token->tokenType != TTindent)          // indent
    {
        return ERROR_SYN;
    }

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }

    // nasleduje telo elsu
    result = commandBody(token, symTab, localTree);
    if (result != NOERROR_OK)
    {
        return result;
    }

    // dedent byl overen na konci tela
    // pokud jsme se dostali zde, je snad vse ok
    return NOERROR_OK;
}


/**
 * Kontrola cyklu While
 */
int cWhile(Token *token, tBSTNodePtr *symTab, tBSTNodePtr *localTree)
{
    int result;

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }

    // musi se jednat o expression

    // je identifikator
    if (token->tokenType == TTidentifier)
    {
        tBSTNodePtr varr;
        if (localTab)
        {
            varr->TBSNodeCont = BSTSearch(symTab, token->str);
        }
        else
        {
            varr->TBSNodeCont = BSTSearch(localTree, token->str);
        }

        // existuje?
        if (varr->TBSNodeCont == NULL)
        {
            return ERROR_SEM_VAR;
        }

        // pokud funkce, velka spatna
        if (varr->TBSNodeCont->nType == ntVar)
        {
            result = processToken(token, symTab, localTree);
        }
        else
        {
            return ERROR_SYN;
        }
    }

    // je leva zavorka
    else if (token->tokenType == TTbracketLeft)
    {
        result = processToken(token, symTab, localTree);
    }

    // jinak kontroluji item
    else
    {
        if (token->tokenType == TTnumberFloat || token->tokenType == TTnumberInt || token->tokenType == TTstring)
        {
            result = processToken(token, symTab, localTree);
        }
        else
        {
            return ERROR_SYN;
        }


    }

    // pokud expression obevi neco necekaneho
    if (result != NOERROR_OK)
    {
        return result;
    }

    // vyraz koncil dvojteckou
    // nasleduje konec radku
    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }
    if (token->tokenType != TTEol)          // eol
    {
        return ERROR_SYN;
    }

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }
    if (token->tokenType != TTindent)          // indent
    {
        return ERROR_SYN;
    }

    /* ----------------------- get token ----------------------- */
    if ((result = getToken(token)) != NOERROR_OK)
    {
        return result;
    }

    // token je zacatek tela cyklu
    result = commandBody(token, symTab, localTree);
    if (result != NOERROR_OK)
    {
        return result;
    }

    // cyklus probehl a pokud neocitla chyba jsme ve zdarnem konci cyklu
    return NOERROR_OK;
}

/**
 * Kontrola tela IF a WHILE
 */
int commandBody(Token *token, tBSTNodePtr *symTab, tBSTNodePtr *localTree)
{
    int result;

    // pokud dostanu okamzite dedent, chybi telo, spatne
    if (token->tokenType == TTdedent)
    {
        return ERROR_SYN;
    }

    // telo bude bezet dokud nedojde dedentu, nebo nedojdou tokeny
    while ((token->tokenType != TTdedent) && token->tokenType != TTEof)
    {
        // v tele funkce se mohou vyskytovat pouze prikazy a prazdny radek
        if (token->tokenType == TTEol)
        {
            continue;
        }
        else
        {
            result = command(token, symTab, localTree);
        }
        if (result != NOERROR_OK)
        {
            return result;
        }
        /* ----------------------- get token ----------------------- */
        if ((result = getToken(token)) != NOERROR_OK)
        {
            return result;
        }
    }

    // znovu kontroluji dedent, ted musi byt
    if (token->tokenType != TTdedent)
    {
        return ERROR_SYN;
    }

    // telo probehle spravne
    return NOERROR_OK;

}



/* ----------------------------------------------------------------- */
/*                           IN PARSE BASE                           */

/**
 * Prida do tabulky symbolu vestavene funkce
 */
void buildInFunc(tBSTNodePtr *symTab)
{
    BSTInsert(symTab, "inputs", createCont(ntFunc, 0, true, typeString));
    BSTInsert(symTab, "inputi", createCont(ntFunc, 0, true, typeInt));
    BSTInsert(symTab, "inputf", createCont(ntFunc, 0, true, typeFloat));
    BSTInsert(symTab, "print", createCont(ntFunc, -1, true, typeNo));
    BSTInsert(symTab, "len", createCont(ntFunc, 1, true, typeInt));
    BSTInsert(symTab, "substr", createCont(ntFunc, 3, true, typeString));
    BSTInsert(symTab, "ord", createCont(ntFunc, 2, true, typeInt));
    BSTInsert(symTab, "chr", createCont(ntFunc, 1, true, typeInt));
    return;
}
