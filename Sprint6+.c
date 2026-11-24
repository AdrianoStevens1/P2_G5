#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_LEITURAS 5
#define TEMP_MIN -10
#define TEMP_MAX 190

// Estrutura dos sensores
struct Sensor {
    char nome[10];
    float ganho;
    float desvio;
};

// Dados de calibracao dos sensores
struct Sensor sensores[] = {
    {"TEMP1", 260, -20},
    {"TEMP2", 250, -15},
    {"TEMP3", 240, -10}
};

#define NUM_SENSORES (sizeof(sensores) / sizeof(sensores[0]))

// Estrutura das leituras
struct Leitura {
    int sensor;
    int instante;
    int valor;
    float temperatura;
};


// Funcao para identificar o sensor
int procurarSensor(char nome[]) {

    for (int i = 0; i < (int)NUM_SENSORES; i++) {

        if (strcmp(nome, sensores[i].nome) == 0) {
            return i;
        }
    }

    return -1;
}


// Funcao para converter o valor em temperatura
float converterParaTemperatura(int x, int sensor) {

    return sensores[sensor].ganho * x / 1023.0
           + sensores[sensor].desvio;
}


// Funcao para verificar a validade da temperatura
int verificarTemperaturaValida(float temp) {

    if (temp < TEMP_MIN || temp > TEMP_MAX) {
        return 0;
    }

    return 1;
}


// Funcao para verificar se uma leitura e duplicada
int leituraDuplicada(struct Leitura leituras[], int n,
                     int sensor, int instante) {

    for (int i = 0; i < n; i++) {

        if (leituras[i].sensor == sensor &&
            leituras[i].instante == instante) {

            return 1;
        }
    }

    return 0;
}


// Funcao para ler as temperaturas
int lerTemperaturas(struct Leitura leituras[],
                    int max_leituras) {

    char linha[100];
    char nome[10];
    char extra;

    int x, instante, sensor;
    int n = 0;
    float temp;

    printf("\n========================================\n");
    printf("       CONVERSOR DE TEMPERATURAS\n");
    printf("========================================\n");
    printf("Formato: SENSOR INSTANTE VALOR\n");
    printf("Introduza FIM para terminar.\n\n");

    while (n < max_leituras) {

        printf("Leitura [%d/%d]: ", n + 1, max_leituras);

        // Ler a linha completa
        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            break;
        }

        // Ler o nome do sensor e ignorar linhas em branco
        if (sscanf(linha, "%9s", nome) != 1) {
            continue;
        }

        // Verificar se o utilizador quer terminar
        if (strcmp(nome, "FIM") == 0) {

            if (sscanf(linha, "%9s %c", nome, &extra) == 1) {
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

        // Verificar os tres campos da linha
        if (sscanf(linha, "%9s %d %d %c",
                   nome, &instante, &x, &extra) != 3) {

            printf("Linha invalida\n");
            continue;
        }

        // Verificar o valor lido pelo sensor
        if (x < 0 || x > 1023) {
            printf("Linha invalida\n");
            continue;
        }

        // Converter o valor em temperatura
        temp = converterParaTemperatura(x, sensor);

        // Verificar a validade da temperatura
        if (verificarTemperaturaValida(temp) == 0) {
            printf("Valor fora da gama\n");
            continue;
        }

        // Verificar se a leitura ja existe
        if (leituraDuplicada(leituras, n, sensor, instante) == 1) {
            printf("Leitura duplicada\n");
            continue;
        }

        // Guardar os dados da leitura
        leituras[n].sensor = sensor;
        leituras[n].instante = instante;
        leituras[n].valor = x;
        leituras[n].temperatura = temp;

        printf("Temperatura convertida: %.2f C\n", temp);

        n++;
    }

    return n;
}


// Funcao para calcular minimo, maximo e media
int calcularEstatisticas(struct Leitura leituras[],
                         int n, int sensor,
                         float *minimo, float *maximo,
                         float *media, int *instanteMin,
                         int *instanteMax) {

    float soma = 0;
    int total = 0;

    for (int i = 0; i < n; i++) {

        if (leituras[i].sensor == sensor) {

            // Inicializar minimo e maximo
            if (total == 0) {

                *minimo = leituras[i].temperatura;
                *maximo = leituras[i].temperatura;

                *instanteMin = leituras[i].instante;
                *instanteMax = leituras[i].instante;
            }

            // Somar as temperaturas
            soma = soma + leituras[i].temperatura;

            // Verificar a temperatura minima
            if (leituras[i].temperatura < *minimo) {

                *minimo = leituras[i].temperatura;
                *instanteMin = leituras[i].instante;
            }

            // Verificar a temperatura maxima
            if (leituras[i].temperatura > *maximo) {

                *maximo = leituras[i].temperatura;
                *instanteMax = leituras[i].instante;
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
float calcularDesvioPadrao(struct Leitura leituras[],
                           int n, int sensor,
                           float media, int total) {

    float soma_desvios = 0;
    float diferenca;

    if (total == 0) {
        return 0;
    }

    for (int i = 0; i < n; i++) {

        if (leituras[i].sensor == sensor) {

            diferenca = leituras[i].temperatura - media;

            soma_desvios = soma_desvios
                          + diferenca * diferenca;
        }
    }

    return sqrt(soma_desvios / total);
}


// Funcao para mostrar as temperaturas
void mostrarTemperaturas(struct Leitura leituras[], int n) {

    printf("\n========================================\n");
    printf("          TEMPERATURAS LIDAS\n");
    printf("========================================\n");

    for (int i = 0; i < n; i++) {

        printf("[%d] Sensor: %s | Instante: %d | "
               "Valor: %d | Temp: %.2f C\n",

               i + 1,
               sensores[leituras[i].sensor].nome,
               leituras[i].instante,
               leituras[i].valor,
               leituras[i].temperatura);
    }
}


// Funcao para mostrar as estatisticas
void mostrarEstatisticas(struct Leitura leituras[], int n) {

    float minimo, maximo, media, desvio;
    int total;
    int instanteMin, instanteMax;

    printf("\n========================================\n");
    printf("          RESUMO ESTATISTICO\n");
    printf("========================================\n");

    for (int s = 0; s < (int)NUM_SENSORES; s++) {

        total = calcularEstatisticas(
            leituras, n, s,
            &minimo, &maximo, &media,
            &instanteMin, &instanteMax
        );

        if (total > 0) {

            desvio = calcularDesvioPadrao(
                leituras, n, s, media, total
            );

            printf("\nSensor: %s\n", sensores[s].nome);
            printf("----------------------------------------\n");

            printf("Total de valores : %d\n", total);
            printf("Minimo           : %.2f C\n", minimo);
            printf("Instante minimo  : %d\n", instanteMin);
            printf("Maximo           : %.2f C\n", maximo);
            printf("Instante maximo  : %d\n", instanteMax);
            printf("Media            : %.2f C\n", media);
            printf("Desvio-Padrao    : %.2f C\n", desvio);
        }
    }

    printf("========================================\n");
}


// Programa principal
int main(void) {

    struct Leitura leituras[MAX_LEITURAS];

    int n;

    // Ler os valores
    n = lerTemperaturas(leituras, MAX_LEITURAS);

    // Verificar se existem leituras validas
    if (n == 0) {

        printf("\nNao existem temperaturas validas registadas.\n");

        return 0;
    }

    // Mostrar as temperaturas
    mostrarTemperaturas(leituras, n);

    // Mostrar as estatisticas
    mostrarEstatisticas(leituras, n);

    return 0;
}
