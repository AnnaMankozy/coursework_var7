#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>

#define R 8.31696
#define T0 400.0
#define T 1200.0
#define DT 60.0
#define B 50.0
#define M 28.96
#define K 1.402
#define K_G ((K - 1)/K)

struct VariantData {
    double Ph;
    double A;
    double P;
};

double calculate_P0(double t, double Ph, double A) {
    if (t <= T / 3.0) {
        return Ph + 3.0 * A * t / T;
    } else if (t <= 2.0 * T / 3.0) {
        return Ph + A;
    } else {
        return Ph + A - (t - 2.0 * T / 3.0) * 3.0 * A / T;
    }
}

double calculate_Tc(double t) {
    if (t <= T / 3.0) {
        return T0;
    } else if (t <= 2.0 * T / 3.0) {
        return T0 + (t - T / 3.0) * 3.0 * B / T;
    } else {
        return T0 + B;
    }
}

double calculate_w(double P, double P0, double Tc) {
    double fraction = pow(P / P0, K_G);
    return sqrt((2.0 * R * Tc) / (K_G * M)) * sqrt(1.0 - fraction);
}

int main() {
    system("chcp 65001");
    struct VariantData variants[3];
    
    FILE *input = fopen("input.txt", "r");
    if (!input) {
        printf("Помилка при відкритті файлу input.txt\n");
        return 1;
    }
    
    for (int i = 0; i < 3; i++) {
        if (fscanf(input, "%lf %lf %lf", &variants[i].Ph, &variants[i].A, &variants[i].P) != 3) {
            printf("Помилка при читанні даних з файлу\n");
            fclose(input);
            return 1;
        }
    }
    fclose(input);

    FILE *output = fopen("output.txt", "w");
    if (!output) {
        printf("Помилка при відкритті файлу output.txt\n");
        return 1;
    }

    fprintf(output, "Time\t    w1         \tw2         \tw3\n");

    for (double t = 0.0; t <= T; t += DT) {
        fprintf(output, "%.2f", t);

        for (int i = 0; i < 3; i++) {
            double P0 = calculate_P0(t, variants[i].Ph, variants[i].A);
            double Tc = calculate_Tc(t);
            double w = calculate_w(variants[i].P, P0, Tc);
            fprintf(output, "\t%.6f", w);
        }

        fprintf(output, "\n");
    }

    fclose(output);

    printf("Результати збережено в output.txt\n");
    return 0;
}