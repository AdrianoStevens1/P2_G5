#include <stdio.h>

int main(void)
{
    int x;
    float temp;

    while (1) {
        while (scanf("%d", &x) != 1) {
            while (getchar() != '\n'); 

        if (x == -1) {
            break;
        }


        temp = 260.0 * x / 1023 - 20;

        if (temp < -10 || temp > 190) {
            printf("Valor fora da gama\n");
        } else {
            printf("%.2f\n", temp);
        }
    }

    return 0;
}
