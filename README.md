# Отчёт. Домашнее задание 2. 
## _Русанов Андрей, БПИ219_

## Вариант 30
> Разработать программу численного интегрирования функции y =
a + b ∗ x^3
(задаётся действительными числами а,b) в определённом
диапазоне целых (задаётся так же) методом трапеций (точность
вычислений = 0.0001).


## На 6 баллов
- Решение на языке С представлено в файле [**main.c**]
```sh
#include <stdio.h>
#include <limits.h>

double f(double a, double b, double x) {
    return a + b * x * x * x;
}

double integrate(double a, double b, int lower, int upper) {
    double step = 0.0000001;
    double res = 0;
    for (double i = lower; i < upper; i += step) {
        res += (f(a, b, i + step) + f(a, b, i)) * step / 2;
    }
    return res;
}

int main() {
    double a = 0;
    double b = 0;
    int lower = 0;
    int upper = INT_MIN;
    double res;
    printf("Insert a(double):");
    scanf("%lf", &a);
    printf("Insert b(double):");
    scanf("%lf", &b);
    printf("Inser the lower bound(int):");
    scanf("%d", &lower);
    while (upper <= lower) {
        printf("Insert the upper bound(int):");
        scanf("%d", &upper);
    }
    res = integrate(a, b, lower, upper);
    printf("%f", res);
}

```
- Откомпилирована ассемблерная программа. Из ассемблерной программы убраны лишние макросы, добавлены комментарии, поясняющие эквивалентное представление переменных в программе на C.
```sh
$gcc -masm=intel -fno-asynchronous-unwind-tables -fno-jump-tables -fno-stack-protector -fno-exceptions 4points.c -S -o 4points.s
```
- Добавлены функции с предачей по параметрам
- Некторые переменные перенесены в регистры

Изменения в функции integrate:

- Первый аргумент функции a (-8[rbp]) замененён на xmm4
- Второй аргумент функции и (-16[rbp]) заменена на xmm5
- Третий аргумент функции и (-24[rbp]) заменена на xmm6

Изменения в функции f:

- переменная step (-24[rbp]) заменена на xmm7

В файле [*tests.txt*](./4_points/tests.txt) представлены тесты.
В файле [*results-c.txt*](./4_points/results-c.txt) представлены результаты для программы на языке С.
В файле [*results-s.txt*](./4_points/results-s.txt) представлены результаты для программы на языке ассемблера.