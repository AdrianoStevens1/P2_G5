#include <stdio.h>
#include <math.h>
#define MAX_LEITURAS 100

int main(void) {
    int valor;
    int numeroLeituras;
    int leituras = 0;
    int terminar = 0;
    int valores[MAX_LEITURAS];
    double temperaturas[MAX_LEITURAS];
    double somaTemperaturas = 0.0;

    printf("Quantas leituras pretende introduzir (1 a %d)? ", MAX_LEITURAS);
    if (scanf("%d", &numeroLeituras) != 1 ||
        numeroLeituras < 1 || numeroLeituras > MAX_LEITURAS) {
        printf("Número de leituras inválido.\n");
        return 1;
    }

    while (leituras < numeroLeituras && terminar == 0) {
        printf("Introduza o valor do sensor (0 a 1023) ou -1 para terminar: ");

        if (scanf("%d", &valor) != 1) {
            printf("Entrada inválida.\n");
            return 1;
        }

        if (valor == -1) {
            printf("Programa terminado.\n");
            terminar = 1;
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
        valores[leituras] = valor;
        temperaturas[leituras] = temperatura;
        somaTemperaturas += temperatura;
        leituras++;
    
    }

    if (leituras == 0) {
        printf("Nenhum valor foi lido.\n");
    } else {
        for (int i = 0; i < leituras; i++) {
            printf("%d -> %.2f °C\n", valores[i], temperaturas[i]);
        }
    }

    printf("Número de valores lidos: %d\n", leituras);
    if (leituras > 0) {
        double temperaturaMinima = temperaturas[0];
        double temperaturaMaxima = temperaturaMinima;
        double temperaturaMedia = somaTemperaturas / leituras;
        double somaDesviosQuadrados = 0.0;

        for (int i = 0; i < leituras; i++) {
            if (temperaturas[i] < temperaturaMinima) {
                temperaturaMinima = temperaturas[i];
            }
            if (temperaturas[i] > temperaturaMaxima) {
                temperaturaMaxima = temperaturas[i];
            }
            double desvio = temperaturas[i] - temperaturaMedia;
            somaDesviosQuadrados += desvio * desvio;
        }

        double desvioPadrao = sqrt(somaDesviosQuadrados / leituras);
        printf("Temperatura mínima: %.2f °C\n", temperaturaMinima);
        printf("Temperatura máxima: %.2f °C\n", temperaturaMaxima);
        printf("Temperatura média: %.2f °C\n", temperaturaMedia);
        printf("Desvio-padrão: %.2f °C\n", desvioPadrao);
    } else {
        printf("Temperatura mínima: N/A\n");
        printf("Temperatura máxima: N/A\n");
        printf("Temperatura média: N/A\n");
        printf("Desvio-padrão: N/A\n");
    }

    return 0;
}