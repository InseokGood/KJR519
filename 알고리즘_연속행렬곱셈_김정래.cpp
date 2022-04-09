#include <stdio.h>

int main(void) {

    static int n = 4, INF = 99999;
    int i, j, k, L;
    int a, b;
    int temp;
    int C[4][4];
    int d[5] = { 10,20,5,15,30 };

    for (int i = 1; i < n + 1; i++) {
        C[i-1][i-1] = 0;
    }

    for (int L = 1; L < n; L++) {
        for (int i = 1; i < n - L + 1; i++) {
            j = i + L;
            C[i-1][j-1] = INF;
            for (int k = i; k < j; k++) {
                temp = C[i-1][k-1] + C[k][j-1] + d[i - 1] * d[k] * d[j];
                    if (temp < C[i-1][j-1]) {
                        C[i-1][j-1] = temp;
                    }
            }

        }
    }
    

    for (int a=0; a < n; a++) {
        for (int b=0; b < n; b++) {
            if (0 < C[a][b] < INF) {
                printf(" %d ", C[a][b]);
            }
            
        }
        printf("\n");
    }

    printf("\n");
    printf("최소 곱셈 횟수 : %d ", C[0][n-1]);


    return 0;

}