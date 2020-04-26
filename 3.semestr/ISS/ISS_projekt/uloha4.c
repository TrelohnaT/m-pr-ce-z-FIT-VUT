#include <stdio.h>
#include <stdlib.h>


int main()
{
    int B = 16;     //Nc
    int f[16];      //počet f (B-1)
    int k = 0;      //aktualní vzorek
    int P[256];     //parametry pro dany vzorek
    int X[73000];   //zde je uložený vstupní signál 

    

    for(int i = 0; i != 73000; i++)
    {
        
    }


    /*
    ** Jsem si vědom že toto je poměkud dřevorubecké řešení,
    ** ale holt nejsem tak zdatný v tomto a mělo by to splnit,
    ** to co má.
    ** P[k] je výsledek výpočtu středního výkonu po Furierovy transformace
    */

    for(k = 0;k <= (B-1);k++)
    {
        f[0] = f[0] + P[k];
        k++;
    }

    for(k = B; k <= 2*B-1;k++)
    {
        f[1] = f[1] + P[k];
        k++;
    }

    for(k = 2*B; k <= 3*B-1;k++)
    {
        f[2] = f[2] + P[k];
        k++;
    }

    for(k = 3*B; k <= 4*B-1;k++)
    {
        f[3] = f[3] + P[k];
        k++;
    }

    for(k = 4*B; k <= 5*B-1;k++)
    {
        f[4] = f[4] + P[k];
        k++;
    }

    for(k = 5*B; k <= 6*B-1;k++)
    {
        f[5] = f[5] + P[k];
        k++;
    }

    for(k = 6*B; k <= 7*B-1;k++)
    {
        f[6] = f[6] + P[k];
        k++;
    }

    for(k = 7*B; k <= 8*B-1;k++)
    {
        f[7] = f[7] + P[k];
        k++;
    }

    for(k = 8*B; k <= 9*B-1;k++)
    {
        f[8] = f[8] + P[k];
        k++;
    }

    for(k = 9*B; k <= 10*B-1;k++)
    {
        f[9] = f[9] + P[k];
        k++;
    }

    for(k = 10*B; k <= 11*B-1;k++)
    {
        f[10] = f[10] + P[k];
        k++;
    }
    
    for(k = 11*B; k <= 12*B-1;k++)
    {
        f[11] = f[11] + P[k];
        k++;
    }

    for(k = 12*B; k <= 13*B-1;k++)
    {
        f[12] = f[12] + P[k];
        k++;
    }

    for(k = 13*B; k <= 14*B-1;k++)
    {
        f[13] = f[13] + P[k];
        k++;
    }

    for(k = 14*B; k <= 15*B-1;k++)
    {
        f[14] = f[14] + P[k];
        k++;
    }

    for(k = 15*B; k <= 16*B-1;k++)
    {
        f[15] = f[15] + P[k];
        k++;
    }

    return 0;
}



