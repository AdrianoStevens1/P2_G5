#include <stdio.h>
#include <math.h>

#define MAX_LEITURAS 5

// 1.a Função para converter o valor do sensor em temperatura
float converterParaTemperatura(int x) {
    return 260.0f * (float)x / 1023.0f - 20.0f;
}


// 1.b Função para verificar se uma temperatura é válida.
int verificarTemperaturaválida(float temp){
  if (temp < -10.0f || temp > 190.0f){
  return 0; // Inválida
  }
  return 1; // Válida
}

// 1.c Função para ler a sequência de valores.
int lerTemperaturas(float temperaturas[], int max_leituras){
    int x;
    int n = 0;
    float temp;
    // Cabeçalho Inicial
    printf("\n========================================\n");
    printf("     === CONVERSOR DE TEMPERATURAS ===   \n");
    printf("========================================\n");
    printf("Insira os valores (ou -1 para terminar):\n\n");
    
    while (n < max_leituras) {
        printf("Valor [%d/%d]: ", n + 1, max_leituras);
        
        while (scanf("%d", &x) != 1) {
            int c = getchar();
            if (c == EOF) {
                return 1;
            }
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Entrada inválida. Tente novamente: ");
        }
        
        if (x == -1) {
            break;
        }

        // Chamada da função de conversão
        temp = converterParaTemperatura(x);
        
        
        if (!verificarTemperaturaválida(temp)){
            printf(" [!] Aviso: Valor fora da gama (-10ºC a 190ºC)\n");
        } else {
            printf(" -> Temperatura convertida: %.2f ºC\n", temp);
            temperaturas[n] = temp;
            n++;
        }
    }
    return n; // Devolve o número total de temperaturas válidas guardadas
}

// 3. Função única para cálculo de Mínimo, Máximo e Média (evita ciclos duplicados)
void calcularEstatisticas(float temperaturas[], int n, float *minimo, float *maximo, float *media) {
    float soma = 0;
    *minimo = temperaturas[0];
    *maximo = temperaturas[0];
    
    for (int i = 0; i < n; i++) {
        soma += temperaturas[i];
        if (temperaturas[i] < *minimo) {
            *minimo = temperaturas[i];
        }
        if (temperaturas[i] > *maximo) {
            *maximo = temperaturas[i];
        }
    }
    
    *media = soma / n;
}

// 1.d Função para calcular o desvio-padrão
float calcularDesvioPadrao(float temperaturas[], int n, float media) {
    float soma_desvios = 0;
    for (int i = 0; i < n; i++) {
        soma_desvios += (temperaturas[i] - media) * (temperaturas[i] - media);
    }
    return sqrt(soma_desvios / n);
}

// 1.e Função para apresentar a lista de valores lidos
void mostrarTemperaturas(float temperaturas[], int n) {
    printf("\n========================================\n");
    printf("          TEMPERATURAS LIDAS            \n");
    printf("========================================\n");
    
    for (int i = 0; i < n; i++) {
        printf(" [%d] %.2f ºC\n", i + 1, temperaturas[i]);
    }
}
// 1.e Função para apresentar o resumo estatístico
void mostrarEstatisticas(int n, float minimo, float maximo, float media, float desvio) {
    printf("\n========================================\n");
    printf("          RESUMO ESTATÍSTICO            \n");
    printf("========================================\n");
    printf(" Total de valores : %d\n", n);
    printf(" Mínimo           : %.2f ºC\n", minimo);
    printf(" Máximo           : %.2f ºC\n", maximo);
    printf(" Média            : %.2f ºC\n", media);
    printf(" Desvio-Padrão    : %.2f\n", desvio);
    printf("========================================\n");
}
int main(void)
{
    float temperaturas[MAX_LEITURAS];
    int n;
    int i;
    
    float media;
    float minimo, maximo;
    float desvio;
    
    // Chamada da função de leitura
    n = lerTemperaturas(temperaturas, MAX_LEITURAS);
    
    if (n == 0) {
        printf("\n----------------------------------------\n");
        printf(" [!] Não existem temperaturas válidas registadas.\n");
        printf("----------------------------------------\n");
        return 0;
    }

  //  Mostrar temperaturas lidas (através da nova função)
    mostrarTemperaturas(temperaturas, n);
    
    // 3. Calcular estatísticas de uma só vez (sem código repetido)
    calcularEstatisticas(temperaturas, n, &minimo, &maximo, &media);
    desvio = calcularDesvioPadrao(temperaturas, n, media);
    
    // Mostrar o resumo estatístico (através da nova função)
    mostrarEstatisticas(n, minimo, maximo, media, desvio);
    
    return 0;
}
