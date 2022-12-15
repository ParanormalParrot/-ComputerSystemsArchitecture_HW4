#include <pthread.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>


// Портфель
int briefcase = 0;
// Число участков
int number_of_areas;
// Координаты скоровища
int tr_i, tr_j;
// Найдено ли сокровище
bool treasure_found = false;
bool isFileIO = false;
// Матрица острова
bool island[20][20];
// Строка для записи результата программы
std::string answer;
// Аргументы коммандной строки
char **params;

// Мьютекс портфеля задач
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
// Мьютекс вывода информации о потоке и его работе.
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

// Функция потоков - групп пиратов
void *pirate_group(void *ptr) {
    int group_number = *((int *) ptr);
    while (!treasure_found) {
        pthread_mutex_lock(&mutex1);
        briefcase += 1;
        pthread_mutex_unlock(&mutex1);
        int area_number = briefcase;
        int area_square = (400 / number_of_areas) + 1;
        // Определяем начальное положение группы пиратов на участке в зависимрсти от его номера
        int x = (area_square * (area_number - 1)) / 20;
        int y = (area_square * (area_number - 1)) % 20;
        pthread_mutex_lock(&mutex2);
        // Вывод сообщения о том, что  группа переместилась на новый участок
        if (isFileIO) {
            std::ofstream out;
            out.open(params[3], std::ios::app);
            out << "Group №" << group_number << " is exploring area №" << area_number << ".\n";;
            out.close();
        }
        std::cout << "Group №" << group_number << " is exploring area №" << area_number << "\n";
        pthread_mutex_unlock(&mutex2);
        if (area_number <= number_of_areas) {
            for (int i = 0; i < area_square && (x < 20 && y < 20) && !treasure_found; ++i) {
                if (x <= 19) {
                    // Поиск сокровища
                    if (island[x][y]) {
                        treasure_found = true;
                        pthread_mutex_lock(&mutex2);
                        if (isFileIO) {
                            std::ofstream out;
                            out.open(params[3], std::ios::app);
                            out << "Group №" << group_number << " has found treasure in area №" << area_number << "!"
                                << "\n";
                            out.close();
                        }
                        // Вывод сообщения об успехе.
                        std::cout << "Group №" << group_number << " has found treasure in area №" << area_number << "!"
                                  << "\n";
                        pthread_mutex_unlock(&mutex2);
                        answer = "Group №" + std::to_string(group_number) + " has found treasure in area №" +
                                 std::to_string(area_number) + " at coordinates " + std::to_string(x) + " " +
                                 std::to_string(y) + ".\n";
                        return nullptr;
                    }
                }
                x++;
                if (x > 19) {
                    x = 0;
                    y++;
                }
            }
            pthread_mutex_lock(&mutex2);
            // Вывод сообщения о том, что  группа полностью исследовала участок и не нашла клад.
            if (isFileIO) {
                std::ofstream out;
                out.open(params[3], std::ios::app);
                out << "Group №" << group_number << " has explored area №" << area_number << " and found no treasure"
                    << ".\n";
                out.close();
            }
            std::cout << "Group №" << group_number << " has explored area №" << area_number << " and found no treasure"
                      << ".\n";
            pthread_mutex_unlock(&mutex2);
        } else {
            return nullptr;
        }
    }
    return nullptr;
}

int main(int argc, char **argv) {
    int number_of_groups;
    bool flag = false;
    params = argv;
    if (argc == 6) {
        // Ввод через аргументы командной строки
        if (*argv[1] == 'i') {
            try {
                tr_i = std::stoi(argv[2]);
                if (tr_i < 0 || tr_i > 19) {
                    flag = true;
                }
                tr_j = std::stoi(argv[3]);
                if (tr_j < 0 || tr_j > 19) {
                    flag = true;
                }
                number_of_groups = std::stoi(argv[4]);
                if (number_of_groups < 1) {
                    flag = true;
                }
                number_of_areas = std::stoi(argv[5]);
                if (number_of_areas < 1) {
                    flag = true;
                }
            } catch (...) {
                std::cout << "Incorrect value\n";
                flag = true;
            }
        } else if (*argv[1] == 'f') {
            isFileIO = true;
        } else {
            std::cout << "Incorrect input\n";
            return 0;
        }

    } else if (argc == 2 && *argv[1] == 'r') {
        // Случайная генерация значений
        srand((unsigned int)time(NULL));
        tr_i = rand() % 19;
        tr_j = rand() % 19;
        number_of_groups = rand() % 45 + 1;
        number_of_areas = rand() % 55 + 1;
        std::cout << "treasure vertical coordinate: " << tr_i << "\n";
        std::cout << "treasure horizontal coordinate: " << tr_j << "\n";
        std::cout << "number of groups: " << number_of_groups << "\n";
        std::cout << "number of areas: " << number_of_groups << "\n";
    } else {
        flag = true;
    }
    if (flag) {
        // Ввод через консоль
        std::cout << "Введите координату сокровища по вертикали(0 - 19):";
        std::cin >> tr_i;
        while (tr_i < 0 || tr_i > 19) {
            std::cout << "Введите координату сокровища по вертикали(0 - 19):";
            std::cout << "Неверный ввод!";
            std::cin >> tr_i;
        }
        std::cout << "Введите координату сокровища по горизонтали(0 - 19):";
        std::cin >> tr_j;
        while (tr_j < 0 || tr_j > 19) {
            std::cout << "Введите координату сокровища по горизонтали(0 - 19):";
            std::cout << "Неверный ввод!";
            std::cin >> tr_j;
        }
        std::cout << "Введите число групп пиратов(число потоков):";
        std::cin >> number_of_groups;
        while (number_of_groups < 1) {
            std::cout << "Введите число групп пиратов(число потоков):";
            std::cout << "Неверный ввод!";
            std::cin >> number_of_groups;
        }
        std::cout << "Введите число участков острова(1 - 50):";
        std::cin >> number_of_areas;
        while (number_of_areas < 1 || number_of_areas > 150) {
            std::cout << "Неверный ввод!";
            std::cout << "Введите число участков острова(1 - 50):";
            std::cin >> number_of_groups;
        }
    }
    if (isFileIO) {
        // Ввод через файл
        FILE *f = fopen(argv[2], "r");
        fscanf(f, "%d %d %d %d", &tr_i, &tr_j, &number_of_groups, &number_of_areas);
        fclose(f);
        std::ofstream out;
        out.open(params[3]);
        out << "";
        out.close();
    }
    // Отмечаем положение клада на острове
    island[tr_i][tr_j] = true;
    pthread_t thread[number_of_groups];
    int groups[number_of_groups];
    // Запуск потоков - групп пиратов
    for (int i = 0; i < number_of_groups; ++i) {
        groups[i] = i + 1;
        pthread_create(&thread[i], nullptr, pirate_group, (void *) (groups + i));
    }
    // Закрываем потоки
    for (int i = 0; i < number_of_groups; ++i) {
        pthread_join(thread[i], nullptr);
    }
    // Выводим результат
    if (isFileIO) {
        std::ofstream out;
        out.open(params[3], std::ios::app);
        out << answer;
        out.close();
    }
    std::cout << answer;
    return 0;
}