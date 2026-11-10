#include <stdio.h>
#include <math.h>

#define MAX_LEITURAS 5
#define TEMP_MIN -10
#define TEMP_MAX 190
#define NUM_SENSORES 3

// Dados dos sensores e respetivas calibracoes
char nomesSensores[NUM_SENSORES][10] = {
    "TEMP1", "TEMP2", "TEMP3"
};

float ganhos[NUM_SENSORES] = {260, 250, 240};
float desvios[NUM_SENSORES] = {-20, -15, -10};

// Funcao para comparar nomes
int compararNomes(char a[], char b[]) {
    int i = 0;

    while (a[i] == b[i] && a[i] != '\0') {
        i++;
    }

    if (a[i] == b[i]) {
        return 1;
    }

    return 0;
}

// Funcao para identificar o sensor
int procurarSensor(char nome[]) {
    for (int i = 0; i < NUM_SENSORES; i++) {
        if (compararNomes(nome, nomesSensores[i]) == 1) {
            return i;
        }
    }

    return -1;
}

// 1.a Funcao para converter o valor em temperatura
float converterParaTemperatura(int x, int sensor) {
    return ganhos[sensor] * x / 1023.0f + desvios[sensor];
}

// 1.b Funcao para verificar a validade da temperatura
int verificarTemperaturaValida(float temp) {
    if (temp < TEMP_MIN || temp > TEMP_MAX) {
        return 0;
    }

    return 1;
}

// 1.c Funcao para ler as temperaturas
int lerTemperaturas(int sensores[], int instantes[],
                    int valores[], float temperaturas[],
                    int max_leituras) {

    char linha[100];
    char nome[10];
    char extra;

    int x, instante, sensor;
    int n = 0;
    float temp;

    printf("\n========================================\n");
    printf("     === CONVERSOR DE TEMPERATURAS ===\n");
    printf("========================================\n");
    printf("Formato: SENSOR INSTANTE VALOR\n");
    printf("Introduza FIM para terminar.\n\n");

    while (n < max_leituras) {
 printf("Leitura [%d/%d]: ", n + 1, max_leituras);

        // Ler o nome do sensor
        if (scanf("%9s", nome) != 1) {
            break;
        }
         
        // Ler o nome do sensor
        if (sscanf(linha, "%9s", nome) != 1) {
            printf("Linha invalida\n");
            continue;
        }

        // Verificar se o utilizador quer terminar
        if (compararNomes(nome, "FIM") == 1) {
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

        // Verificar o valor lido
        if (x < 0 || x > 1023) {
            printf("Linha invalida\n");
            continue;
        }

        // Converter o valor em temperatura
        temp = converterParaTemperatura(x, sensor);

        if (!verificarTemperaturaValida(temp)) {
            printf("Valor fora da gama\n");
        }
        else {
            printf("Temperatura convertida: %.2f C\n", temp);

            sensores[n] = sensor;
            instantes[n] = instante;
            valores[n] = x;
            temperaturas[n] = temp;

            n++;
        }
    }

    return n;
}

// 3. Funcao para calcular minimo, maximo e media
int calcularEstatisticas(float temperaturas[], int sensores[],
                         int n, int sensor, float *minimo,
                         float *maximo, float *media) {

    float soma = 0;
    int total = 0;

    for (int i = 0; i < n; i++) {

        if (sensores[i] == sensor) {

            if (total == 0) {
                *minimo = temperaturas[i];
                *maximo = temperaturas[i];
            }

            soma += temperaturas[i];

            if (temperaturas[i] < *minimo) {
                *minimo = temperaturas[i];
            }

            if (temperaturas[i] > *maximo) {
                *maximo = temperaturas[i];
            }

            total++;
        }
    }

    if (total > 0) {
        *media = soma / total;
    }

    return total;
}

// 1.d Funcao para calcular o desvio-padrao
float calcularDesvioPadrao(float temperaturas[], int sensores[],
                           int n, int sensor, float media, int total) {

    float soma_desvios = 0;

    for (int i = 0; i < n; i++) {

        if (sensores[i] == sensor) {
            soma_desvios += (temperaturas[i] - media)
                          * (temperaturas[i] - media);
        }
    }

    return sqrt(soma_desvios / total);
}

// 1.e Funcao para mostrar as temperaturas
void mostrarTemperaturas(int sensores[], int instantes[],
                         int valores[], float temperaturas[], int n) {

    printf("\n========================================\n");
    printf("          TEMPERATURAS LIDAS\n");
    printf("========================================\n");

    for (int i = 0; i < n; i++) {

        printf("[%d] Sensor: %s | Instante: %d | Valor: %d | Temp: %.2f C\n",
               i + 1,
               nomesSensores[sensores[i]],
               instantes[i],
               valores[i],
               temperaturas[i]);
    }
}

// 1.f Funcao para mostrar as estatisticas
void mostrarEstatisticas(int sensores[], float temperaturas[], int n) {

    float minimo, maximo, media, desvio;
    int total;

    printf("\n========================================\n");
    printf("          RESUMO ESTATISTICO\n");
    printf("========================================\n");

    for (int s = 0; s < NUM_SENSORES; s++) {

        total = calcularEstatisticas(
            temperaturas, sensores, n, s,
            &minimo, &maximo, &media
        );

        if (total > 0) {

            desvio = calcularDesvioPadrao(
                temperaturas, sensores, n, s, media, total
            );

            printf("\nSensor: %s\n", nomesSensores[s]);
            printf("Total de valores : %d\n", total);
            printf("Minimo           : %.2f C\n", minimo);
            printf("Maximo           : %.2f C\n", maximo);
            printf("Media            : %.2f C\n", media);
            printf("Desvio-Padrao    : %.2f\n", desvio);
        }
    }

    printf("========================================\n");
}

int main(void) {

    int sensores[MAX_LEITURAS];
    int instantes[MAX_LEITURAS];
    int valores[MAX_LEITURAS];
    float temperaturas[MAX_LEITURAS];

    int n;

    // Ler os valores
    n = lerTemperaturas(sensores, instantes, valores,
                        temperaturas, MAX_LEITURAS);

    if (n == 0) {
        printf("\nNao existem temperaturas validas registadas.\n");
        return 0;
    }

    // Mostrar as temperaturas
    mostrarTemperaturas(sensores, instantes, valores,
                        temperaturas, n);

    // Mostrar as estatisticas
    mostrarEstatisticas(sensores, temperaturas, n);

    return 0;
}
