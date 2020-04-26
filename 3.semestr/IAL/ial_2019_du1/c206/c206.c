
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2019
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    /*Kontrola, zda L není ješté inicializovaný*/
    if (L != NULL) {
        L->First = NULL;    //inicializace ukazatele na první prvek
        L->Last = NULL;     //inicializace ukazatele na posledni prvek
        L->Act = NULL;      //inicializace ukazatele na aktualni prvek
    }
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	/*deklarovaní ukazatele na mazaný prvek, ukazuje na prvni prvek seznamu*/
    tDLElemPtr mazany_prvek = L->First;
    /*dokud nebude ukazatel na prvni prvek NULL(tedy prázdny seznam)*/
    while(L->First != NULL)
    {
        L->First = L->First->rptr;      //posunutí ukazatele na prvni prvek o jeden do prava
        mazany_prvek->lptr = NULL;      //přepsání ukazatele prvního prvku na předchozí prvek(ten už ukazoval na NULL ale pro jistotu)
        mazany_prvek->rptr = NULL;      //přepsání ukazatele prvního prvku na nadcházející prvek
        free(mazany_prvek);     //uvolnění paměti který mazaný prvek zabíral
        mazany_prvek = L->First;       //nastavení ukazatele na mazaný prvek aby ukazoval na první prvek
    }
    /*po smazání seznamu se seznam znovu inicializuje*/
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	/*deklarace ukazatele na tDLElemPtr*/
    tDLElemPtr novyPrvek;
    /*alokace paměti na kterou bude tento ukazatel ukazovat*/
    novyPrvek = (struct tDLElem*) malloc(sizeof(struct tDLElem));
    /*testování, zda alokace proběhla v pořádku*/
    if(novyPrvek != NULL)
    {
        novyPrvek->data = val;              //vložení do nového prvku hodnotu
        novyPrvek->lptr = NULL;             //nastavení ukazatele na předchozí na NULL (jelikož bude vždy první)
        novyPrvek->rptr = L->First;         //nastavení ukazatele na další tak aby ukazoval na první(jelikož bude vždy před prvním)
        /*pokud sezam nebyl prázdný*/
        if(L->First != NULL)
        {
            L->First->lptr = novyPrvek;     //prvního prvku ukazatel na předchozí bude ukazovat na nově vložený
        }
        /*pokud seznam byl prázdný*/
        else
        {
            L->Last = novyPrvek;            //nový prvek je první i poslední
        }
        L->First = novyPrvek;               //ukazatel na první prvek bude ukazovat na nově vložený první prvek
    }
    else
    {
        DLError();                          //pokud se paměť nepodařilo alkovat
    }
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	

    /*deklarace ukazatele na tDLElemPtr*/
    tDLElemPtr novyPrvek;
    /*alokace paměti na kterou bude tento ukazatel ukazovat*/
    novyPrvek = (struct tDLElem*) malloc(sizeof(struct tDLElem));
    /*testování, zda alokace proběhla v pořádku*/
    if(novyPrvek != NULL)
    {
        novyPrvek->data = val;              //vložení do nového prvku hodnotu
        novyPrvek->lptr = L->Last;          //nastaveni ukazatele tak aby ukazoval na poslední prvek(jelikož bude vždy za posledním)
        novyPrvek->rptr = NULL;             //nastavení ukazatele na další prvek na NULL(jelikož bude vždy poslední)
        /*pokud je seznam neprázdný*/
        if(L->First != NULL)
        {
            L->Last->rptr = novyPrvek;      //nastavení ukazatele na další prvek tak, aby ukazoval na nově vložený prvek
        }
        /*pokud je seznam prázdný*/
        else
        {
            L->First = novyPrvek;           //poslední prvek je zároveň první prvek
        }
        L->Last = novyPrvek;                //nastavení ukazatele na poslední prvek tak, aby ukazoval na nově vložený prvek
    }
    else
    {
        DLError();                          //pokud se paměť nepodařilo alokovat
    }
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

    L->Act = L->First;                      //nastavení ukazatele na aktualní prvek tak, aby ukazoval na ten prvni
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
	L->Act = L->Last;                       //nastavení ukazatele na aktulaní prvek tak, aby ukazovat na ten poslední

}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    /*pokud je seznam prázdný*/
    if(L->First == NULL)
    {
        DLError();                          //funkce DLError byla zavolána
    }
    /*pokud je seznam neprázdný*/
    else
    {
        *val = L->First->data;               //do parametru val byla předána hodnota prvního prvku
    }
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	
    /*pokud je seznam prázdný*/
    if(L->Last == NULL)
    {
        DLError();                            //funkce DLError byla zavolána
    }
    /*pokud je seznam neprázdný*/
    else
    {
        *val = L->Last->data;                   //do parametru val byla předána hodnota prvního prvku
    }
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	/*pokud je seznam neprázdný, pokud je prázdný nic se neděje*/
    if(L->First != NULL)
    {
        /*pokud první prvek je zároveň aktulní prvek*/
        if(L->First == L->Act)
        {
            L->Act = NULL;              //aktivita je ztracena
        }
        /*pokud je v seznamu jen jeden prvek*/
        if(L->Last == L->First)
        {
            free(L->First);             //paměť prvního prvku je uvolněna
            L->First = NULL;            //ukazatel na první prvek ukazuje na NULL
            L->Last = NULL;             //ukazatel na poslední prvek ukazuje na NULL
        }
        /*pokud je v seznamu víc než jeden prvek*/
        else
        {
            L->First = L->First->rptr;      //ukazatel na první prvek je posunut tak, aby ukazoval na druhý prvek
            free(L->First->lptr);           //paměť prvního prvku je uvolněna
            L->First->lptr = NULL;          //ukazatel ukazující na předchozí prvek prvního prvku ukazuje na NULL
        }
    }
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
	/*pokud je seznam neprázdný, pokud je prázdný nic se neděje*/
    if(L->First != NULL)
    {
        /*pokud první prvek je zároveň aktulní prvek*/
        if(L->Act == L->Last)
        {
            L->Act = NULL;              //aktivita je ztracena
        }
        /*pokud je v seznamu jen jeden prvek*/
        if(L->Last == L->First)
        {
            free(L->Last);              //paměť posledního prvku smazána
            L->First = NULL;            //ukazatel na první prvek ukazuje na NULL
            L->Last = NULL;             //ukazatel na poslední prvek ukazuje na NULL            
        }
        /*pokud je v seznamu víc než jeden prvek*/
        else
        {
            L->Last = L->Last->lptr;     //ukazatel na poslední prvek je posunut o jedno do leva
            free(L->Last->rptr);         //paměť posledního prvku je uvolněna
            L->Last->rptr = NULL;        //ukazatel na další prvek posledního prvku ukazuje na NULL
        }
        
    }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
    /*pokud je seznam neaktivní, pokud je prázdný nic se neděje*/
    if(L->Act != NULL)
    {
        /*otestuje zda aktualni prvek není poslední prvek(nebo jediný)*/
        if(L->Act->rptr != NULL)
        {
            tDLElemPtr MazanyPrvek;                     //deklaruje ukazatel na tDLElemPtr
            MazanyPrvek = L->Act->rptr;                 //ukazatel na mazaný prvek ukazuje na prvek za aktualním
            L->Act->rptr = MazanyPrvek->rptr;           //ukazatel na prvek za aktualním nyní ukazuje na ten který je za mazaným
            /*pokud je mazaný prvek poslední*/
            if(MazanyPrvek == L->Last)                 
            {
                L->Last = L->Act;                       //poslední prvek je nyní ten na který ukazuje ukazatel na aktulaní prvek
            }
            /*pokud není mazaný prvek poslední*/
            else
            {
                MazanyPrvek->rptr->lptr = L->Act;       //ukazatel na předchozí prvek prvku za mazaným prvkem nyní ukazuje na aktualní prvek
            }
            free(MazanyPrvek);                          //paměť mazaného prvku je uvolněna
        }
    }
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
    /*pokud je seznam neaktivní, pokud je prázdný nic se neděje*/
    if(L->Act != NULL)
    {
         /*otestuje zda aktualni prvek není první prvek(nebo jediný)*/
        if(L->Act->lptr != NULL)
        {
            tDLElemPtr MazanyPrvek;                     //deklarace ukazatele na tDLElemPtr
            MazanyPrvek = L->Act->lptr;                 //ukazatel na mazaný prvek nyní ukazuje na ten před aktualním 
            L->Act->lptr = MazanyPrvek->lptr;           //ukazatel na předcházející prvek aktualního teď ukazuje na prvek před mazaným prvekm
            /*Pokud je mazaný prvek prví*/
            if(MazanyPrvek == L->First)         
            {
                L->First = L->Act;                      //aktualní prvek je bude nyní ten první
            }
            /*pokud tomu tak není*/
            else
            {
                MazanyPrvek->lptr->rptr = L->Act;       //ukazatel na nadcházejí prvek před mazaným prvkem nyní ukazuje na ten aktualní
            }
            free(MazanyPrvek);                          //paměť mazaného prvku je uvolněna
        }
    }
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	/*pokud je seznam neaktivní, pokud je prázdný nic se neděje*/
    if(L->Act != NULL)
    {
        tDLElemPtr novyPrvek;                                               //deklarace ukazatele novyPrvek
        novyPrvek = (struct tDLElem*) malloc(sizeof(struct tDLElem));       //alokace paměti pro nový prvek na kterou ukazuje novyPrvek
        /*pokud se alokace povedla*/
        if(novyPrvek != NULL)
        {
            novyPrvek->data = val;                       //nahrání hodnoty do nového prvku
            novyPrvek->lptr = L->Act;                    //ukazatel na nadcházející prvek ukazuje na aktualní(protože vždy bude za aktualním)
            novyPrvek->rptr = L->Act->rptr;              //ukazatel na následující prvek ukazuje na prvek který byl za aktualním
            L->Act->rptr = novyPrvek;                    //ukazatel na následující prvek aktulního prvku ukazuje na nový prvek
            /*pokud byl aktualní prvek poslední*/
            if(L->Last = L->Act)
            {
                L->Last = novyPrvek;                    //nový prvek je nyní poslední
            }
            /*pokud nebyl*/
            else
            {
                novyPrvek->rptr->lptr = novyPrvek;      //ukazatel na předešlý prvek nadcházejícího prvku ukazuje na nový prvek
            }
        }
        /*pokud se alokace nepovedla*/
        else
        {
            DLError();                                   //funkce DLError byla zavolána
        }
    }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    /*pokud je seznam neaktivní, pokud je prázdný nic se neděje*/
    if(L->Act != NULL)
    {
        tDLElemPtr novyPrvek;                                               //deklarace ukazatele na nový prvek
        novyPrvek = (struct tDLElem*) malloc(sizeof(struct tDLElem));       //alokace paměti pro nový prvek, ukazuje na ní ukazatel novyPrvek
        /*pokud se alokace povedla*/
        if(novyPrvek != NULL)
        {
            novyPrvek->data = val;                      //nahrání hodnoty do nového prvku
            novyPrvek->lptr = L->Act;                   //prvek před novým prvkem bude ted aktulní
            novyPrvek->rptr = L->Act->lptr;             //prvek za novým prvkem bude ten, který byl před aktualním prvkem
            L->Act->lptr = novyPrvek;                   //nový prvkem bude před aktualním prvkem
            /*pokud je aktualní prvek první prvek*/
            if(L->First = L->Act)
            {
                L->First = novyPrvek;                   //ukazatel na první prvek ukazuje na ten nový
            }
            /*pokud ne*/
            else
            {
                novyPrvek->lptr->rptr = novyPrvek;      //ukazatel na nadcházející prvek prvku před novým prvkem ukazuje na nový prvek
            }
        }
        /*pokud se alokace nepovedla*/
        else
        {
            DLError();                                  //funkce DLError byla zavolána
        }
    }
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
    /*pokud je ukazatel na aktualní prvek neaktivní*/
    if(L->Act == NULL)
    {
        DLError();                  //funkce DLError byla zavolána
    }	
    /*pokud je aktivní*/
    else
    {
        *val = L->Act->data;        //do parametru val byla nahrána hodnota aktualního prvku
    }
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
    /*pokud je ukazatel na aktualní prvek aktivní*/
    if(L->Act != NULL)
    {
        L->Act->data = val;         //hodnota val byla vložena do aktualního prvku
    }
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	/*pokud je ukazatel na aktualní prvek aktivní*/
    if(L->Act != NULL)
    {
        L->Act = L->Act->rptr;      //ukazatel na aktualní prvek nyní ukazuje na prvek za předtím aktualním prvkem
    }
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	/*pokud je ukazatel na aktualní prvek aktivní*/
    if(L->Act != NULL)
    {
        L->Act = L->Act->lptr;      //ukazatel na aktualní prvek nyní ukazuje na prvek před předtím aktualním prvkem
    }	
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
    /*pokud je aktualní prvek aktivní, vrátí nenulovou hodnotu*/
    return(L->Act != NULL);	
}
/*s gramatikou komentářu jsem se moc neobtěžoval, protože to stejně nebudete číst. Děkuji za pochopení*/
/* Konec c206.c*/
