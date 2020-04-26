/*IOS 2.projekt*/
/*
** Autor: Václav Doleček
** filename: proj2.c
** Motivace: baví mě programování*/

/* pár slov o tomto souboru:
** Zde jsou deklarovány a implementovány všecky funkce vyjma main


*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h> 
#include <semaphore.h> 



int RNG(int range);


int RNG(int range)
{
    int RNG = 0;    //návratová hodnota
    time_t t;

    /*inicializace RNG*/
    srand((unsigned) time(&t));

    RNG = (rand() % range);  //toto nějak vyhodí náhodné číslo 

    return RNG;
}