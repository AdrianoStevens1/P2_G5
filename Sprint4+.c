#include <stdio.h>
#include <math.h>

#define MAX_LEITURAS 5

int main(void)
{
    int x;
    int i;
    int n = 0;
    
    float temp;
    float temperaturas[MAX_LEITURAS];
    float soma = 0;
    float media;
    float minimo, maximo;
    float soma_desvios = 0;
    float desvio;
    
    // Cabeçalho Inicial
    printf("\n========================================\n");
    printf("     === CONVERSOR DE TEMPERATURAS ===   \n");
    printf("========================================\n");
    printf("Insira os valores (ou -1 para terminar):\n\n");
    
    while (n < MAX_LEITURAS) {
        printf("Valor [%d/%d]: ", n + 1, MAX_LEITURAS);
        
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

        temp = 260.0f * x / 1023.0f - 20.0f;

        if (temp < -10.0f || temp > 190.0f) {
            printf(" [!] Aviso: Valor fora da gama (-10ºC a 190ºC)\n");
        } else {
            printf(" -> Temperatura convertida: %.2f ºC\n", temp);
            temperaturas[n] = temp;
            n++;
        }
    }
    
    if (n == 0) {
        printf("\n----------------------------------------\n");
        printf(" [!] Não existem temperaturas válidas registadas.\n");
        printf("----------------------------------------\n");
        return 0;
    }

    
    printf("\n========================================\n");
    printf("          TEMPERATURAS LIDAS            \n");
    printf("========================================\n");
    
    for (i = 0; i < n; i++) {
        printf(" [%d] %.2f ºC\n", i + 1, temperaturas[i]);
    }

    minimo = temperaturas[0];
    maximo = temperaturas[0];
    
    for (i = 0; i < n; i++) {
        soma += temperaturas[i];
        if (temperaturas[i] < minimo) minimo = temperaturas[i];
        if (temperaturas[i] > maximo) maximo = temperaturas[i];
    }
    
    media = soma / n;
    
    for (i = 0; i < n; i++) {
        soma_desvios += (temperaturas[i] - media) * (temperaturas[i] - media);
    }
    
    desvio = sqrt(soma_desvios / n);
    
    printf("\n========================================\n");
    printf("          RESUMO ESTATÍSTICO            \n");
    printf("========================================\n");
    printf(" Total de valores : %d\n", n);
    printf(" Mínimo           : %.2f ºC\n", minimo);
    printf(" Máximo           : %.2f ºC\n", maximo);
    printf(" Média            : %.2f ºC\n", media);
    printf(" Desvio-Padrão    : %.2f\n", desvio);
    printf("========================================\n");
    
    return 0;
}
