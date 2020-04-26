 /* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, 2014-2018
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {

	/*přjde-li ukazatel ukazující na NULL*/
	if(ptrht == NULL)
	{
		return; //je konec
	}
	/*pokud ne*/
	else
	{
		/*projedeme celou Hash table*/
		for(int i = 0; i < HTSIZE; i++)
		{
			/*na všechy ukazatele nastavíme na NULL*/
			(*ptrht)[i] = NULL;
		}
	}

	
	


 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

	/*ukazatel na hledaný prvek*/
	tHTItem **hledany_prvek;

	/*přjde-li ukazatelé ukazující na NULL*/
	if(ptrht == NULL || key == NULL)
	{
		return; //je konec
	}
	/*ale pokud ne*/
	else
	{
		/*najdeme adresu pro zadaný klíč*/
		hledany_prvek = &(*ptrht)[hashCode(key)];
		/*projedeme celý seznam*/
		while(*hledany_prvek != NULL)
		{
			/*key každého prvku porovnáme s hledaným key*/
			if(!strcmp(key, (*hledany_prvek)->key))
			{
				/*je-li nalezena shoda, vracíme ukazatel na něj*/
				return *hledany_prvek;
			}
			/*pokud ne, posouváme se dál*/
			hledany_prvek = &(*hledany_prvek)->ptrnext;
		}	
		/*skončí-li seznam a my nic nenajdeme, není tam*/
		return NULL;
	}


 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {

	/*deklarace ukazatelu*/
	tHTItem **novy_prvek, *stary_prvek = NULL;
	/*přjde-li ukazatelé ukazující na NULL*/
	if(ptrht == NULL || key == NULL)
	{
		return; //je konec
	}
	/*ale pokud ne*/
	else
	{
		/*hledáme jesli už neexistuje prvek s těmito daty*/
		stary_prvek = (htSearch(ptrht, key));
		/*hledaní mista pro novy prvek*/
		novy_prvek = &(*ptrht)[hashCode(key)];
		/*pokud ano*/
		if(stary_prvek != NULL)
		{
			/*data jsou aktualizována*/
			stary_prvek->data = data;
		}
		/*pokud ne*/
		else
		{
			/*pokud na této pozici je prvek*/
			if(*novy_prvek != NULL)
			{
				/*je přepsán/aktualizován*/
				stary_prvek = *novy_prvek;
			}
			/*alókace místa pro nový prvek*/
			(*novy_prvek) = malloc(sizeof(struct tHTItem));
			/*inicializace nového prvku*/
			(*novy_prvek)->data = data;
			(*novy_prvek)->key = key;
			(*novy_prvek)->ptrnext = stary_prvek;
		}
	}
	
	 

 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {
	/*ukazatel na prvek v tabulce*/
	tHTItem *hledany_prvek;
	/*najdení hledaného prvku*/
	hledany_prvek = htSearch(ptrht, key);
	/*pokud jsou parametry neplatne*/
	if( ptrht == NULL || key == NULL)
	{
		return;	//je konec
	}
	/*pokud ale ne*/
	else
	{
		/*pokud je prvek najden*/
		if(hledany_prvek != NULL)
		{
			/*přečteme ho (vrátíme data)*/
			return &hledany_prvek->data;
		}
		else
		{
			/*jinak vracíme NULL*/
			return NULL;
		}
	}



 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {

	tHTItem **hledany_prvek, *mazany_prvek;

	/*pokud dostamene neplatné parametry*/
	if(ptrht == NULL || key == NULL)
	{
		return;	//je konec
	}
	/*pokud ale ne*/
	else
	{
		hledany_prvek = &(*ptrht)[hashCode(key)];
		
		/*projedeme celý seznam*/
		while(*hledany_prvek != NULL)
		{
			/*key každého prvku porovnáme s hledaným key*/
			if(!strcmp(key, (*hledany_prvek)->key))
			{
				/*předáváme je ukazateli na mazaný prvek*/
				mazany_prvek = *hledany_prvek;
				
				/*paměť mazaného prvku je uvolněna*/
				free(mazany_prvek);	
			}
			
			//*hledany_prvek = (*hledany_prvek)->ptrnext;
			hledany_prvek = &(*hledany_prvek)->ptrnext;
		}
		
	}





 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {

	tHTItem *mazany_item, *nasledujici_item;
	/*pokud je argument neplatny*/
	if(ptrht == NULL)
	{
		return;	//je konec
	}
	/*pokud platný je*/
	else
	{
		/*projedu celý seznam*/
		for(int i = 0; i < HTSIZE; i++)
		{
			/*mazany_item ukazuje na aktualni prvek*/
			mazany_item = &(*ptrht)[i];
			/*pokud je mazany_item platny*/
			if(mazany_item != NULL)
			{
				/*dokud je mazany_item platny*/
				while(mazany_item != NULL)
				{
					/*nasledující item za mazanym*/
					nasledujici_item = mazany_item->ptrnext;
					/*uvolněni mazaneho*/
					free(mazany_item);
					/*aktualizování mazaneho*/
					mazany_item = nasledujici_item;
				}
				/*zrušení ukazatele na smazany item*/
				(*ptrht)[i] = NULL;
			}
		}
	}
	

 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}
