#include <stdio.h>         // Для введення та виведення       
#include <math.h>          // Для математичних функцій
#include <stdlib.h>        // Для виведення укр. тексту в терміналі

#define R 8.31696          // Універсальна газова потоку
#define T0 400.0           // Початковий час
#define T 1200.0           // Загальний час
#define DT 60.0            // Крок зміни часу
#define B 50.0             // Зміна температури
#define M 28.96            // Молекулярна маса газу
#define K 1.402            // Відношення темлоємностей
#define K_G ((K - 1)/K)    // Допоміжна стала для обчислень

struct VariantData {       // Структура для зберігання параментрів варіанта
    double Ph;             // Початковий тиск 
    double A;              // Амплітуда зміни тиску
    double P;              // Тиск 
};

double calculate_P0(double t, double Ph, double A) {        // Обчислення тиску в посудині P0 залежно від часу
    if (t <= T / 3.0) {                                     // Якщо час знаходиться у першій третині періоду
        return Ph + 3.0 * A * t / T;                        
    } else if (t <= 2.0 * T / 3.0) {                        // Якщо час знаходиться у другій третині періоду
        return Ph + A;                                      
    } else {                                                // Якщо час знаходиться у третій третині періоду
        return Ph + A - (t - 2.0 * T / 3.0) * 3.0 * A / T;  
}
}

double calculate_Tc(double t) {                             // Обчислення температури в посудині Tc залежно від часу
    if (t <= T / 3.0) {                                     // Якщо час знаходиться у першій третині
        return T0;                                          
    } else if (t <= 2.0 * T / 3.0) {                        // Якщо час знаходиться у другій третині
        return T0 + (t - T / 3.0) * 3.0 * B / T;            
    } else {                                                // Якщо час знаходиться у третій третині 
        return T0 + B;                                      
    }
}
double calculate_w(double P, double P0, double Tc) {        // Обчислення зміни швидкості витікання ідеального газу з посудини під тиском
    double fraction = pow(P / P0, K_G);
    if (P >= P0){
        return 0.0;
    }
    return sqrt((2.0 * R * Tc) / (K_G * M)) * sqrt(1.0 - fraction);
}

int main() {
    system("chcp 65001");                                                 // Для підтримки в коді українських символів

    struct VariantData variants[3];                                       // Масив з трьох наборів вхідних параметрів
    
    FILE *input = fopen("input.txt", "r");                                // Відкриття вхідного файлу
    if (!input) {
        printf("Помилка при відкритті файлу input.txt\n");
        return 1;
    }
    
    for (int i = 0; i < 3; i+=1) {                                        // Зчитування трьох рядків з вхідного файлу "input.txt"
        if (fscanf(input, "%lf %lf %lf", &variants[i].Ph, &variants[i].A, &variants[i].P) != 3) {
            printf("Помилка при читанні даних з файлу\n");
            fclose(input);
            return 1;
        }
    }
    fclose(input);                                                       // Закриття вхідного файлу


    FILE *output = fopen("output.txt", "w");                             // Відкриття вихідного файлу
    if (!output) {
        printf("Помилка при відкритті файлу output.txt\n");
        return 1;
    }

    fprintf(output, "Time\t    w1         \tw2         \tw3\n");         // Запис заголовка таблиці у файл

    for (double t = 0.0; t <= T; t += DT) {                              // Основний цикл по часу від 0 до T з кроком DT
        fprintf(output, "%.2f", t);                                      // Запис часу

        for (int i = 0; i < 3; i+=1) {                                   // Обчислення витрат для кожного з варіанту
            double P0 = calculate_P0(t, variants[i].Ph, variants[i].A);  
            double Tc = calculate_Tc(t);                                 
            double w = calculate_w(variants[i].P, P0, Tc);               
            fprintf(output, "\t%.6f", w);                                // Запис результату у файл
        }

        fprintf(output, "\n");                                           // Перехід на новий рядок
    }

    fclose(output);                                                      // Закриття вихідного файлу

    printf("Результати збережено в output.txt\n");                       // Повідомлення про успішне завершення
    return 0;
}