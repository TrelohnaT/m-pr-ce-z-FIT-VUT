//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
	//konstruktor, vytvori prazdny seznam

	Element_t stuff;
	Element_t Nstuff;

	int ER = 1; //chybový hlasic

	GetHead(ER);

	if{ER == NULL}
	{
		printf("Seznam je prazdy.\n");
		return 0;
	}
	else
	{

	}

}

PriorityQueue::~PriorityQueue()
{

}

void PriorityQueue::Insert(int value)
{
	Element_t *stuff;
	Element_t *Istuff; //vkladaný prvek
	stuff = GetHead(); //ukazatel na první prvek
	
	/*Nastavi ukazatel stuff na prvek, kteru je pred budoucim novym prvkem
	 *Pokud je seznam prazdny, */
	while (stuff->pNext->value < value || stuff->pNext == NULL)
	{
		stuff = stuff->pNext;
	}

	if (stuff->pNext == NULL)
	{
		stuff = stuff->pNext;
		stuff->pPrev = start;
		stuff->value = value;
		return;

	if(stuff->pNext->value < value)





}

bool PriorityQueue::Remove(int value)
{
	Element_t *Bstuff; //ukazatel na strukturu pred hledaným
	Element_t *Rstuff; //ukazatel na strukturu na mazani
	Element_t *Nstuff; //ukazatel na strukturu po hledaným

	int lf = 0; //look for

	Find(&lf);

	int blf = lf - 1; //before look for
	int nlf = lf + 1; //next look for

	int i = 0;
	do
	{
		Bstuff = Bstuff->pNext;
		i++;

	}while(i != blf)


	int j = 0;
	do
	{
		Nstuff = Nstuff->pNext;
		j++;
	}while(j != nlf)
	
	


    return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
	
	while (stuff->pNext != NULL)
	{
		if (value == stuff->value)
		{
			return stuff->value;
		}
		stuff = stuff->pNext;
	}
    return NULL;
}

PriorityQueue::Element_t *PriorityQueue::GetHead(ER)
{


	if{ == NULL}
	{
		return NULL;
	}

	else
	{
		return 1;
	}


    
}

/*** Konec souboru tdd_code.cpp ***/
