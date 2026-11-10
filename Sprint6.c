
#include <stdio.h>
#include <string.h>
#include <math.h>
 
#define MAX_LEITURAS 100
#define SENSOR_MIN 0
#define SENSOR_MAX 1023
#define TEMP_MIN -10.0
#define TEMP_MAX 190.0
 
// Calibração de todos os sensores (único sítio a alterar).
// O mesmo índice nos três arrays corresponde ao mesmo sensor.
static const char  *ID[]     = {"TEMP1", "TEMP2", "TEMP3"};
static const double GANHO[]  = {260, 250, 240};
static const double DESVIO[] = {-20, -15, -10};
#define NUM_SENSORES ((int)(sizeof ID / sizeof ID[0]))
 
// Devolve o índice do sensor ou -1 se for desconhecido
int procurarSensor(const char *id) {
    for (int i = 0; i < NUM_SENSORES; i++) {
        if (strcmp(ID[i], id) == 0) return i;
    }
    return -1;
}
 
// Converte valor do sensor em temperatura, com a calibração desse sensor
double sensorParaTemperatura(int sensor, int valor) {
    return GANHO[sensor] * valor / 1023.0 + DESVIO[sensor];
}
 
// Verifica se valor do sensor é válido
int valorSensorValido(int valor) {
    return valor >= SENSOR_MIN && valor <= SENSOR_MAX;
}
 
// Verifica se temperatura é válida
int temperaturaValida(double temperatura) {
    return temperatura >= TEMP_MIN && temperatura <= TEMP_MAX;
}
 
// Verifica se a linha é "FIM"
int eFim(const char *linha) {
    char palavra[32];
    return sscanf(linha, "%31s", palavra) == 1 && strcmp(palavra, "FIM") == 0;
}
 
// Lê "SENSOR INSTANTE VALOR"; devolve 1 se o formato estiver certo
int lerCampos(const char *linha, char *id, long *instante, int *valor) {
    int consumidos = 0;
    return sscanf(linha, "%31s %ld %d %n", id, instante, valor, &consumidos) == 3
           && linha[consumidos] == '\0';
}
 
// Calcula estatísticas: min, max, média, desvio-padrão
void calcularEstatisticas(const double *temperaturas, int n, double *min, double *max, double *media, double *desvioPadrao) {
    if (n <= 0) {
        *min = *max = *media = *desvioPadrao = 0.0;
        return;
    }
    *min = temperaturas[0];
    *max = temperaturas[0];
    double soma = 0.0;
    for (int i = 0; i < n; i++) {
        if (temperaturas[i] < *min) *min = temperaturas[i];
        if (temperaturas[i] > *max) *max = temperaturas[i];
        soma += temperaturas[i];
    }
    *media = soma / n;
    double somaDesviosQuadrados = 0.0;
    for (int i = 0; i < n; i++) {
        double desvio = temperaturas[i] - *media;
        somaDesviosQuadrados += desvio * desvio;
    }
    *desvioPadrao = sqrt(somaDesviosQuadrados / n);
}
 
// Apresenta lista de leituras registadas
void apresentarLeituras(const int *sensores, const long *instantes, const int *valores, const double *temperaturas, int n) {
    printf("\nSensor  Instante  Valor  Temperatura\n");
    for (int i = 0; i < n; i++) {
        printf("%-7s %8ld %6d %8.2f °C\n", ID[sensores[i]], instantes[i], valores[i], temperaturas[i]);
    }
}
 
// Apresenta estatísticas
void apresentarEstatisticas(int leituras, double min, double max, double media, double desvioPadrao) {
    printf("  Número de valores lidos: %d\n", leituras);
    printf("  Temperatura mínima: %.2f °C\n", min);
    printf("  Temperatura máxima: %.2f °C\n", max);
    printf("  Temperatura média: %.2f °C\n", media);
    printf("  Desvio-padrão: %.2f °C\n", desvioPadrao);
}
 
// Para cada sensor com leituras, copia as suas temperaturas e calcula/apresenta as estatísticas
void apresentarEstatisticasPorSensor(const int *sensores, const double *temperaturas, int n) {
    printf("\n");
    for (int s = 0; s < NUM_SENSORES; s++) {
        double temps[MAX_LEITURAS];
        int c = 0;
        for (int i = 0; i < n; i++) {
            if (sensores[i] == s) temps[c++] = temperaturas[i];
        }
        if (c == 0) continue;
 
        double min, max, media, desvioPadrao;
        calcularEstatisticas(temps, c, &min, &max, &media, &desvioPadrao);
        printf("%s:\n", ID[s]);
        apresentarEstatisticas(c, min, max, media, desvioPadrao);
    }
}
 
// Lê leituras até FIM ou até ao limite; devolve quantas foram registadas
int lerLeituras(int *sensores, long *instantes, int *valores, double *temperaturas, int maxLeituras) {
    char linha[256], id[32];
    int leituras = 0;
 
    printf("Introduza leituras (SENSOR INSTANTE VALOR) ou FIM para terminar:\n");
    while (leituras < maxLeituras && fgets(linha, sizeof linha, stdin)) {
        if (eFim(linha)) break;
 
        long instante;
        int valor;
        if (!lerCampos(linha, id, &instante, &valor)) {
            printf("Linha inválida\n");
            continue;
        }
        int s = procurarSensor(id);
        if (s < 0) {
            printf("Sensor desconhecido\n");
            continue;
        }
        double temperatura = sensorParaTemperatura(s, valor);
        if (!valorSensorValido(valor) || !temperaturaValida(temperatura)) {
            printf("Valor fora da gama\n");
            continue;
        }
        printf("%.2f °C\n", temperatura);
        sensores[leituras] = s;
        instantes[leituras] = instante;
        valores[leituras] = valor;
        temperaturas[leituras] = temperatura;
        leituras++;
    }
    return leituras;
}
 
int main(void) {
    int sensores[MAX_LEITURAS], valores[MAX_LEITURAS];
    long instantes[MAX_LEITURAS];
    double temperaturas[MAX_LEITURAS];
 
    int leituras = lerLeituras(sensores, instantes, valores, temperaturas, MAX_LEITURAS);
    if (leituras == 0) {
        printf("Nenhum valor foi lido.\n");
        return 0;
    }
    apresentarLeituras(sensores, instantes, valores, temperaturas, leituras);
    apresentarEstatisticasPorSensor(sensores, temperaturas, leituras);
    return 0;
}
 