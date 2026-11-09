#include <stdio.h>
#include <math.h>

#define MAX 5
#define TEMP_MIN -10
#define TEMP_MAX 190

float converter_sensor_para_temp(int valor) {
    return 260.0 * valor / 1023.0 - 20.0;
}

int verificar_validade(float temp){
    if (temp >= TEMP_MIN && temp <= TEMP_MAX) {
        return 1;
    }
    else {
        return 0;
    }
}

int ler_valores(int valores[], float temperaturas[]) {
    int i = 0;
    int valor;
    float temp;

    do {
            printf("Digite o valor registado pelo sensor: ");
            while (scanf("%d", &valor) != 1)
            {
                printf("Valor Inválido. Digite um valor válido: ");
                while((getchar() != '\n'));
            } 

            if (valor == -1)
            {
                printf("Programa Terminado.\n");
                break;
            }

            temp = converter_sensor_para_temp(valor);

            if (verificar_validade(temp)) 
            {
                printf("O valor da temperatura é de: %.2fºC\n", temp);
            }
            else {
                printf("Valor fora da Gama.\n");
            }

            temperaturas[i] = temp;
            valores[i] = valor;

            i++; 

    } while (i < MAX);

    return i;
}

float calcular_media(float soma, int i) {
    if (i == 0) 
        { 
            return 0; 
        }
    return soma / i;
}

float minimo(float temperaturas[], int i){
    float min = temperaturas[0];
    for (int k = 1; k < i; k++) {
        if (temperaturas[k] < min) {
            min = temperaturas[k];
        }
    }
    return min;
}

float maximo(float temperaturas[], int i) {
    float max = temperaturas[0];
    for (int k = 1; k < i; k++) {
        if (temperaturas[k] > max) {
            max = temperaturas[k];
        }
    }
    return max;
}

float calcular_desvio_padrao(float temperaturas[], int i, float media) {
    if (i<=0) {
        return 0;
    }
    float soma_quadrados = 0;
    for (int k = 0; k < i; k++) {
        soma_quadrados += (temperaturas[k] - media) * (temperaturas[k] - media);
    }
    return sqrt(soma_quadrados / i);
}

int main(void) {
    int valores[MAX];
    float temperaturas[MAX];
    float min = 0, max = 0, soma = 0, media = 0;
    int i = 0, j = 0, k = 0;
    float soma_quadrados = 0, desvio_padrao = 0;

    i = ler_valores(valores, temperaturas);

    if (i > 0) {
        for (k = 0; k < i; k++) {
            soma += temperaturas[k];
        }

        media = calcular_media(soma, i);
        max = maximo(temperaturas, i);
        min = minimo(temperaturas, i);
        desvio_padrao = calcular_desvio_padrao (temperaturas, i, media);

        printf("\nValores lidos\nSensor ======> Temperatura:\n");

        do 
        {
            printf("%d ==========> %.2fºC\n", valores[j], temperaturas[j]);
            j++;
        } while (j < i);

        printf("Número de valores lidos: %d\n", i);
        printf("Temperatura média: %.4fºC\n", media); 
        printf("Temperatura mínima: %.4fºC\n", min);
        printf("Temperatura máxima: %.4fºC\n", max);
        printf("Desvio Padrão: %.4f\n", desvio_padrao);
    }
    
    return 0;
}