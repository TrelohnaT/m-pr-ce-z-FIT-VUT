/*
** program pro počítání K-means u IZU 2.ukol
** Autor: Václav Doleček (xdolec03)
** Datum: 23.04.2020
** Motivace: abych nezešílel z toho to počítat ručně
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double counting(double X_b, double Y_b, double Z_b, double X_a, double Y_a, double Z_a);

/*jelikož jsem hodný budu to krmit jen správnými argumenty, tedy čísly*/
int main(int argc, char *argv[])
{
    printf("arguments: %d\n", argc);

    /*Středové body*/
    /*1. shluk*/
    //[0,-1,-2] => [2.11,0.56,1.78] => [2.29,0.29,2.14] => [-0.25,0.5,3.5]
    double X_b_1 = -0.25;
    double Y_b_1 = 0.5;
    double Z_b_1 = 3.5;

    /*2. shluk*/
    //[1,2,-4] => [0.5,4,1] => [0.5,4,1] => [0.5,4,1]
    double X_b_2 = 0.5;
    double Y_b_2 = 4.0;
    double Z_b_2 = 1.0;

    /*3. shluk*/
    //[-1,1,-4] => [-1,1,-4] =>  [5,-1,-1]  => [5,0,0]
    double X_b_3 = 5.0;
    double Y_b_3 = 0.0;
    double Z_b_3 = 0.0;

    /*získáváni argumentů*/
    double X_a = atoi(argv[1]);
    double Y_a = atoi(argv[2]);
    double Z_a = atoi(argv[3]);

    /*samotné počítání*/
    double result_1 = counting(X_b_1, Y_b_1, Z_b_1, X_a, Y_a, Z_a);
    double result_2 = counting(X_b_2, Y_b_2, Z_b_2, X_a, Y_a, Z_a);
    double result_3 = counting(X_b_3, Y_b_3, Z_b_3, X_a, Y_a, Z_a);

    /*tisk výsledku*/
    printf("result for 1.: %lf\n", result_1);
    printf("result for 2.: %lf\n", result_2);
    printf("result for 3.: %lf\n", result_3);

    return 0;
}

double counting(double X_b, double Y_b, double Z_b, double X_a, double Y_a, double Z_a)
{
    /*
    result = sqrt(pow(X_b - X_a, 2) + pow(Y_b - Y_a, 2) + pow(Z_b - Z_a, 2))
    result = sqrt(pow(sub_1, 2) + pow(sub_2, 2) + pow(sub_3, 2))
    result = sqrt(pow_sub_1 + pow_sub_2 + pow_sub_3)
    result = sqrt(add)
    */

    /*odčítání závorek*/
    double sub_1 = X_b - X_a;
    double sub_2 = Y_b - Y_a;
    double sub_3 = Z_b - Z_a;

    /*mocnění závorek*/
    double pow_sub_1 = pow(sub_1, 2.0);
    double pow_sub_2 = pow(sub_2, 2.0);
    double pow_sub_3 = pow(sub_3, 2.0);

    /*sčítání závorek*/
    double add = pow_sub_1 + pow_sub_2 + pow_sub_3;

    /*odmocnění*/
    double result = sqrt(add);

    /*vrácení výsledku*/
    return result;

}