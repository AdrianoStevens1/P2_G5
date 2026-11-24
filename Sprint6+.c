#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_LEITURAS 7
#define TEMP_MIN -10
#define TEMP_MAX 190
#define NUM_SENSORES 3

// Dados de calibracao dos sensores

char nomesSensores[NUM_SENSORES][10] = {
    "TEMP1", "TEMP2", "TEMP3"
};

float ganhos[NUM_SENSORES] = {260, 250, 240};
float desvios[NUM_SENSORES] = {-20, -15, -10};


// Funcao para identificar o sensor

int procurarSensor(char nome[]) {

    for (int i = 0; i < NUM_SENSORES; i++) {

        if (strcmp(nome, nomesSensores[i]) == 0) {
            return i;
        }
    }

    return -1;
}


// Funcao para converter o valor em temperatura

float converterParaTemperatura(int x, int sensor) {

    return ganhos[sensor] * x / 1023.0
           + desvios[sensor];
}


// Funcao para verificar a validade da temperatura

int verificarTemperaturaValida(float temp) {

    if (temp < TEMP_MIN || temp > TEMP_MAX) {
        return 0;
    }

    return 1;
}


// Funcao para verificar se uma leitura e duplicada

int leituraDuplicada(int sensoresLidos[],
                     int instantes[],
                     int n, int sensor, int instante) {

    for (int i = 0; i < n; i++) {

        if (sensoresLidos[i] == sensor &&
            instantes[i] == instante) {

            return 1;
        }
    }

    return 0;
}


// Funcao para ler as temperaturas

int lerTemperaturas(int sensoresLidos[],
                    int instantes[],
                    int valores[],
                    float temperaturas[],
                    int max_leituras) {

    char linha[100];
    char nome[10];
    char extra;

    int x, instante, sensor;
    int n = 0;
    float temp;

    while (n < max_leituras) {

        // Ler a linha completa

        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            break;
        }

        // Ler o nome e ignorar linhas em branco

        if (sscanf(linha, "%9s", nome) != 1) {
            continue;
        }

        // Verificar se o utilizador quer terminar

        if (strcmp(nome, "FIM") == 0) {

            if (sscanf(linha, "%9s %c",
                       nome, &extra) == 1) {
                break;
            }

            printf("Linha invalida\n");
            continue;
        }

        // Verificar se o sensor existe

        sensor = procurarSensor(nome);

        if (sensor == -1) {
            printf("Sensor desconhecido\n");
            continue;
        }

        // Verificar os campos da linha

        if (sscanf(linha, "%9s %d %d %c",
                   nome, &instante, &x, &extra) != 3) {

            printf("Linha invalida\n");
            continue;
        }

        // Verificar o valor digital

        if (x < 0 || x > 1023) {
            printf("Linha invalida\n");
            continue;
        }

        // Converter o valor em temperatura

        temp = converterParaTemperatura(x, sensor);

        // Verificar a gama da temperatura

        if (verificarTemperaturaValida(temp) == 0) {
            printf("Valor fora da gama\n");
            continue;
        }

        // Verificar se a leitura e duplicada

        if (leituraDuplicada(sensoresLidos, instantes,
                            n, sensor, instante) == 1) {

            printf("Leitura duplicada\n");
            continue;
        }

        // Guardar os dados nos vetores

        sensoresLidos[n] = sensor;
        instantes[n] = instante;
        valores[n] = x;
        temperaturas[n] = temp;

        printf("%.2f\n", temp);

        n++;
    }

    return n;
}


// Funcao para calcular minimo, maximo e media

int calcularEstatisticas(int sensoresLidos[],
                         int instantes[],
                         float temperaturas[],
                         int n, int sensor,
                         float *minimo,
                         float *maximo,
                         float *media,
                         int *instanteMin,
                         int *instanteMax) {

    float soma = 0;
    int total = 0;

    for (int i = 0; i < n; i++) {

        if (sensoresLidos[i] == sensor) {

            // Inicializar minimo e maximo

            if (total == 0) {

                *minimo = temperaturas[i];
                *maximo = temperaturas[i];

                *instanteMin = instantes[i];
                *instanteMax = instantes[i];
            }

            // Somar as temperaturas

            soma = soma + temperaturas[i];

            // Verificar a temperatura minima

            if (temperaturas[i] < *minimo) {

                *minimo = temperaturas[i];
                *instanteMin = instantes[i];
            }

            // Verificar a temperatura maxima

            if (temperaturas[i] > *maximo) {

                *maximo = temperaturas[i];
                *instanteMax = instantes[i];
            }

            total++;
        }
    }

    // Calcular a media

    if (total > 0) {
        *media = soma / total;
    }

    return total;
}


// Funcao para calcular o desvio-padrao

float calcularDesvioPadrao(int sensoresLidos[],
                           float temperaturas[],
                           int n, int sensor,
                           float media, int total) {

    float soma_desvios = 0;
    float diferenca;

    if (total == 0) {
        return 0;
    }

    for (int i = 0; i < n; i++) {

        if (sensoresLidos[i] == sensor) {

            diferenca = temperaturas[i] - media;

            soma_desvios = soma_desvios
                          + diferenca * diferenca;
        }
    }

    return sqrt(soma_desvios / total);
}


// Funcao para mostrar as temperaturas

void mostrarTemperaturas(int sensoresLidos[],
                         int instantes[],
                         int valores[],
                         float temperaturas[],
                         int n) {

    printf("\nLeituras registadas ");
    printf("(sensor instante: valor -> temperatura):\n\n");

    for (int i = 0; i < n; i++) {

        printf("%s %d: %d -> %.2f\n",
               nomesSensores[sensoresLidos[i]],
               instantes[i],
               valores[i],
               temperaturas[i]);
    }
}


// Funcao para mostrar as estatisticas

void mostrarEstatisticas(int sensoresLidos[],
                         int instantes[],
                         float temperaturas[],
                         int n) {

    float minimo, maximo, media, desvio;
    int total;
    int instanteMin, instanteMax;

    for (int s = 0; s < NUM_SENSORES; s++) {

        total = calcularEstatisticas(
            sensoresLidos,
            instantes,
            temperaturas,
            n,
            s,
            &minimo,
            &maximo,
            &media,
            &instanteMin,
            &instanteMax
        );

        if (total > 0) {

            desvio = calcularDesvioPadrao(
                sensoresLidos,
                temperaturas,
                n,
                s,
                media,
                total
            );

            printf("\nSensor %s\n", nomesSensores[s]);

            printf("Numero de valores lidos: %d\n", total);
            printf("Temperatura minima: %.2f\n", minimo);
            printf("Instante minimo: %d\n", instanteMin);
            printf("Temperatura maxima: %.2f\n", maximo);
            printf("Instante maximo: %d\n", instanteMax);
            printf("Temperatura media: %.2f\n", media);
            printf("Desvio-padrao: %.2f\n", desvio);
        }
    }
}


// Programa principal

int main(void) {

    int sensoresLidos[MAX_LEITURAS];
    int instantes[MAX_LEITURAS];
    int valores[MAX_LEITURAS];
    float temperaturas[MAX_LEITURAS];

    int n;

    // Ler os valores

    n = lerTemperaturas(
        sensoresLidos,
        instantes,
        valores,
        temperaturas,
        MAX_LEITURAS
    );

    // Verificar se existem leituras validas

    if (n == 0) {

        printf("\nNao existem temperaturas validas registadas.\n");

        return 0;
    }

    // Mostrar as temperaturas

    mostrarTemperaturas(
        sensoresLidos,
        instantes,
        valores,
        temperaturas,
        n
    );

    // Mostrar as estatisticas

    mostrarEstatisticas(
        sensoresLidos,
        instantes,
        temperaturas,
        n
    );

    return 0;
}
