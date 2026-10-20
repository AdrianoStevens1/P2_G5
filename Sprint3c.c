#include <stdio.h>
int main (void)

{
    int valor;
    float temp;

    do {

        printf("Digite o valor lido pelo sensor:");
        if (scanf("%d", &valor) !=1)
        {
            printf("Valor inválido. Insira um valor válido.\n");
            while (getchar() != '\n');
            continue;
        }

        if (valor == -1)
        {
            printf("Programa encerrado.\n");
            break;
        }

        temp = 260.0 * valor / 1023 - 20;

        if (temp < -10 || temp > 190)
        {
            printf("Valor fora da gama.\n");
        }

        else 
        {
            printf("O valor da temperatura é de %.2fºC\n", temp);
        }

        } while (valor != -1);

    return 0;
}