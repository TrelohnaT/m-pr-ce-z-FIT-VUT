/*IOS 2.projekt*/
/*
** Autor: Václav Doleček
** filename: proj2.c
** Motivace: baví mě programování


*/



#include "proj2.h"

/*
** Poznámky pro programátora
** all_father => proces který spouští program
** all_father+1 => judge
** all_father+1 < => imigranti

*/



int main(int argc, char *argv[])
{

    
    /*Kontrola, zda je správný počet argumentů*/
    if (argc == 6)
    {
        int PI = atoi(argv[1]);         //Počet vygenerovaných přistěhovalů
        int IG = atoi(argv[2]);         //Maximalní doba generovaní přisěhovalce
        int JG = atoi(argv[3]);         //Maximalní doba pro vstup soudce
        int IT = atoi(argv[4]);         //Maximalni doba pro vyzvednutí potvrzeni
        int JT = atoi(argv[5]);         //Maximalni doba pro vydání potvrzeni
        
        int all_father = getpid();
        printf("all_father %d\n", all_father);


        /*kontrola zda argumenty splňují požadavky*/

        /*P >= 1*/
        if (PI < 1)
        {
            printf("Bad argumet PI\n");
            return 1;
        }
        /*IG >= 0 && IG <= 2000*/
        if (IG < 0 || IG > 2000)
        {
            printf("Bad argumet GI\n");
            return 1;
        }
        /*JG >= 0 && JG <= 2000*/
        if (JG < 0 || JG > 2000)
        {
            printf("Bad argumet GI\n");
            return 1;
        }
        /*IT >= 0 && IT <= 2000*/
        if (IT < 0 || IT > 2000)
        {
            printf("Bad argumet IT\n");
            return 1;
        }
        /*JT >= 0 && JT <= 2000*/
        if (JT < 0 || JT > 2000)
        {
            printf("Bad argumet ´JT\n");
            return 1;
        }

        /*deklarace listu s imigranty*/
        //int list_of_imigrants[PI];

        /*shared memory*/
        int *step_counter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        *step_counter = 0;
        int *NE = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        *NE = 0;
        int *NC = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        *NC = 0;
        int *NB = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        *NB = 0;
        int *shared_memory = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

        /*ostatní promněné*/
        int last_imigrant = 0;
        

        
        /*Kontrolni vypis argumentů*/
        printf("PI: %d\n", PI);
        printf("IG: %d\n", IG);
        printf("JG: %d\n", JG);
        printf("IT: %d\n", IT);
        printf("JT: %d\n", JT);

        

        /*zde je Syn*/
        if ( fork() == 0 ) 
        {

            printf("Judge: %d with parent: %d\n", getpid(), getppid());
            

        }
        /*zde je rodic*/
        else
        {
            printf("Parent: %d\n", getpid());

            for ( int i = 0; i < PI; i++)
            {
                if ( fork() == 0  && getpid() != all_father)
                {
                    usleep(RNG(IG) * 1000);
                    printf("Imigrant: %d with parent: %d count: %d\n", getpid(), getppid(), i);
                    *shared_memory = getpid();
                    break;
                    
                }
                else
                {
                    //printf("Parent of Imigrants: %d\n", getpid());
                }

                if (i == (PI-1))
                {
                    last_imigrant = getpid();
                }
                wait(NULL);
            }
        }
        

        if (getpid() > all_father+1 )
        {
            printf("Imigrant enters\n");
        }




        wait(NULL); 

        if (getpid() == all_father)
        {
            printf("last migrant: %d\n", last_imigrant);
            
        }

        return 0;
    }
    else
    {
        printf("Bad count of arguments\n");
        return 1;
    }
    
}







