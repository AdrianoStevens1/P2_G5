#include <stdio.h>

int main(void) {
    int valor;
    int fim = 0;

    while (!fim) {
        printf("Introduza o valor do sensor (0 a 1023) ou -1 para terminar: ");

        if (scanf("%d", &valor) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
            printf("Entrada inválida.\n");
            continue;
        }

        if (valor == -1) {
            printf("Programa terminado.\n");
            fim = 1;
            continue;
        }

        if (valor < 0 || valor > 1023) {
            printf("Valor fora da gama.\n");
            continue;
        }

        double temperatura = 260.0 * valor / 1023.0 - 20.0;

        if (temperatura < -10.0 || temperatura > 190.0) {
            printf("Valor fora da gama.\n");
            continue;
        }

        printf("Temperatura: %.2f °C\n", temperatura);
    }

    return 0;
}