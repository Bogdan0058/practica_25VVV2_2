#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>


typedef struct {
    int* array;
    int size;
    long long swaps;
    double time;
} SortResult;



int* create_array(int size) {
    int* arr = (int*)malloc(size * sizeof(int));
    if (arr == NULL) {
        printf("Ошибка: недостаточно памяти!\n");
        exit(1);
    }
    return arr;
}

void free_array(int* arr) {
    if (arr != NULL) {
        free(arr);
    }
}

int* copy_array(int* src, int size) {
    int* dst = create_array(size);
    for (int i = 0; i < size; i++) {
        dst[i] = src[i];
    }
    return dst;
}


SortResult shaker_sort(int* arr, int size) {
    SortResult result;
    result.array = copy_array(arr, size);
    result.size = size;
    result.swaps = 0;

    clock_t start = clock();

    int left = 0;
    int right = size - 1;
    int swapped = 1;

    while (swapped&& left < right) {
        swapped = 0;

        
        for (int i = left; i < right; i++) {
            if (result.array[i] > result.array[i + 1]) {
                int temp = result.array[i];
                result.array[i] = result.array[i + 1];
                result.array[i + 1] = temp;
                result.swaps++;
                swapped = 1;
            }
        }
        right--;

        if (!swapped) break;
        swapped = 0;

       
        for (int i = right; i > left; i--) {
            if (result.array[i] < result.array[i - 1]) {
                int temp = result.array[i];
                result.array[i] = result.array[i - 1];
                result.array[i - 1] = temp;
                result.swaps++;
                swapped = 1;
            }
        }
        left++;
    }

    clock_t end = clock();
    result.time = (double)(end - start) / CLOCKS_PER_SEC;

    return result;
}



int* read_from_file(const char* filename, int* size) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "r");

    if (err != 0 || file == NULL) {
        printf("Ошибка: не удалось открыть файл '%s'\n", filename);
        return NULL;
    }

    int capacity = 10;
    int* arr = create_array(capacity);
    int count = 0;
    int num;

    while (fscanf_s(file, "%d", &num) == 1) {
        if (count >= capacity) {
            capacity *= 2;
            arr = (int*)realloc(arr, capacity * sizeof(int));
            if (arr == NULL) {
                printf("Ошибка: недостаточно памяти!\n");
                fclose(file);
                return NULL;
            }
        }
        arr[count++] = num;

       
        char ch;
        fscanf_s(file, "%c", &ch, 1);
        if (ch != ',' && ch != ' ' && ch != '\n' && ch != '\r') {
            fseek(file, -1, SEEK_CUR);
        }
    }

    if (count == 0) {
        printf("Ошибка: файл пуст или содержит некорректные данные\n");
        free(arr);
        fclose(file);
        return NULL;
    }

    arr = (int*)realloc(arr, count * sizeof(int));
    *size = count;
    fclose(file);
    return arr;
}

int write_to_file(const char* filename, int* arr, int size) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "w");

    if (err != 0 || file == NULL) {
        printf("Ошибка: не удалось создать файл '%s'\n", filename);
        return 0;
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%d", arr[i]);
        if (i < size - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");

    fclose(file);
    return 1;
}



int* generate_random_array(int size, int min_val, int max_val) {
    int* arr = create_array(size);
    for (int i = 0; i < size; i++) {
        arr[i] = min_val + rand() % (max_val - min_val + 1);
    }
    return arr;
}

int* generate_sorted_array(int size) {
    int* arr = create_array(size);
    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }
    return arr;
}

int* generate_reversed_array(int size) {
    int* arr = create_array(size);
    for (int i = 0; i < size; i++) {
        arr[i] = size - i;
    }
    return arr;
}



void print_array(int* arr, int size) {
    if (arr == NULL) {
        printf("Массив пуст!\n");
        return;
    }

    if (size > 50) {
        printf("Массив слишком большой (размер: %d), вывожу первые 20 элементов:\n", size);
        for (int i = 0; i < 20; i++) {
            printf("%d", arr[i]);
            if (i < 19) printf(", ");
        }
        printf(" ...\n");
    }
    else {
        for (int i = 0; i < size; i++) {
            printf("%d", arr[i]);
            if (i < size - 1) printf(", ");
        }
        printf("\n");
    }
}

int is_sorted(int* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

void clear_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_header() {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║          ШЕЙКЕРНАЯ СОРТИРОВКА (Cocktail Shaker)        ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

void print_menu() {
    printf("┌────────────────────────────────────────────────────────┐\n");
    printf("│                      ГЛАВНОЕ МЕНЮ                     │\n");
    printf("├────────────────────────────────────────────────────────┤\n");
    printf("│  1. Загрузить массив из файла                        │\n");
    printf("│  2. Сгенерировать случайный массив                   │\n");
    printf("│  3. Выполнить шейкерную сортировку                  │\n");
    printf("│  4. Показать текущий массив                         │\n");
    printf("│  5. Тест производительности на разных данных        │\n");
    printf("│  6. Сохранить результат в файл                      │\n");
    printf("│  7. Информация об алгоритме                         │\n");
    printf("│  0. Выход                                           │\n");
    printf("└────────────────────────────────────────────────────────┘\n");
    printf("\n");
}

void print_info() {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║              ИНФОРМАЦИЯ ОБ АЛГОРИТМЕ                   ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("ШЕЙКЕРНАЯ СОРТИРОВКА (Cocktail Shaker Sort)\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("\n");
    printf("• Это улучшенная версия пузырьковой сортировки\n");
    printf("• Движется в обе стороны: слева направо и справа налево\n");
    printf("• Большие элементы всплывают в конец\n");
    printf("• Маленькие элементы тонут в начало\n");
    printf("• Эффективнее пузырьковой для почти отсортированных массивов\n");
    printf("\n");
    printf("СЛОЖНОСТЬ АЛГОРИТМА:\n");
    printf("• Лучший случай: O(n)  (массив уже отсортирован)\n");
    printf("• Средний случай: O(n²)\n");
    printf("• Худший случай:  O(n²) (массив отсортирован в обратном порядке)\n");
    printf("\n");
    printf("ПРИМЕР РАБОТЫ:\n");
    printf("  [5, 1, 4, 2, 8]  →  [1, 2, 4, 5, 8]\n");
    printf("\n");
    printf("Нажмите Enter для продолжения...");
    getchar();
}

void print_sort_result(SortResult result) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║                  РЕЗУЛЬТАТ СОРТИРОВКИ                  ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("  Размер массива:      %d\n", result.size);
    printf("  Количество обменов:  %lld\n", result.swaps);
    printf("  Время выполнения:    %.6f секунд\n", result.time);
    printf("  Массив отсортирован: %s\n", is_sorted(result.array, result.size) ? "ДА ✓" : "НЕТ ✗");
    printf("\n");
}

void print_test_results(int size, SortResult r1, SortResult r2, SortResult r3) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║            ТЕСТ ПРОИЗВОДИТЕЛЬНОСТИ                     ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("  Размер массива: %d\n", size);
    printf("\n");
    printf("  ┌─────────────────────┬─────────────┬─────────────┐\n");
    printf("  │      Тип данных     │ Перестановок│  Время (сек)│\n");
    printf("  ├─────────────────────┼─────────────┼─────────────┤\n");
    printf("  │ Случайный массив    │ %11lld │ %11.6f │\n", r1.swaps, r1.time);
    printf("  │ Отсортированный     │ %11lld │ %11.6f │\n", r2.swaps, r2.time);
    printf("  │ Инвертированный     │ %11lld │ %11.6f │\n", r3.swaps, r3.time);
    printf("  └─────────────────────┴─────────────┴─────────────┘\n");
    printf("\n");
    printf("  ВЫВОДЫ:\n");
    printf("  • Лучший случай: отсортированный массив (0 перестановок)\n");
    printf("  • Худший случай: инвертированный массив\n");
    printf("  • Шейкерная сортировка эффективна для почти отсортированных данных\n");
    printf("\n");
}



int main() {
    srand((unsigned int)time(NULL));
    setlocale(LC_ALL, "rus");
    int choice;
    int* original_arr = NULL;
    int size = 0;
    char filename[256];

    while (1) {
        print_header();

        if (original_arr != NULL) {
            printf("  Текущий массив: %d элементов\n", size);
            printf("  Статус: %s\n\n", is_sorted(original_arr, size) ? "ОТСОРТИРОВАН" : "НЕ ОТСОРТИРОВАН");
        }
        else {
            printf("  Массив не загружен\n\n");
        }

        print_menu();
        printf("  Ваш выбор: ");

        if (scanf_s("%d", &choice) != 1) {
            printf("  Ошибка ввода!\n");
            clear_input();
            continue;
        }
        clear_input();

        switch (choice) {
        case 0: {
            free_array(original_arr);
            printf("\n  Программа завершена. До свидания!\n\n");
            return 0;
        }

        case 1: {
            printf("\n  Введите имя файла (например, input.csv): ");
            fgets(filename, sizeof(filename), stdin);
            size_t len = strlen(filename);
            if (len > 0 && filename[len - 1] == '\n') {
                filename[len - 1] = '\0';
            }

            free_array(original_arr);
            original_arr = read_from_file(filename, &size);

            if (original_arr != NULL) {
                printf("\n  ✓ Успешно прочитано %d элементов\n", size);
                printf("  Массив: ");
                print_array(original_arr, size);
            }
            printf("\n  Нажмите Enter для продолжения...");
            getchar();
            break;
        }

        case 2: {
            int min_val, max_val;
            printf("\n  Введите размер массива: ");
            if (scanf_s("%d", &size) != 1 || size <= 0) {
                printf("  Некорректный размер!\n");
                clear_input();
                break;
            }
            printf("  Введите минимальное значение: ");
            scanf_s("%d", &min_val);
            printf("  Введите максимальное значение: ");
            scanf_s("%d", &max_val);

            if (min_val > max_val) {
                printf("  Ошибка: минимум больше максимума!\n");
                break;
            }

            free_array(original_arr);
            original_arr = generate_random_array(size, min_val, max_val);

            printf("\n  ✓ Сгенерирован массив из %d элементов\n", size);
            printf("  Массив: ");
            print_array(original_arr, size);
            printf("\n  Нажмите Enter для продолжения...");
            getchar();
            break;
        }

        case 3: {
            if (original_arr == NULL) {
                printf("\n  ! Ошибка: сначала загрузите или сгенерируйте массив!\n");
                printf("\n  Нажмите Enter для продолжения...");
                getchar();
                break;
            }

            printf("\n  Выполняется сортировка...\n");
            SortResult result = shaker_sort(original_arr, size);

            print_sort_result(result);

            free_array(original_arr);
            original_arr = result.array;

            printf("  Отсортированный массив: ");
            print_array(original_arr, size);
            printf("\n  Нажмите Enter для продолжения...");
            getchar();
            break;
        }

        case 4: {
            if (original_arr == NULL) {
                printf("\n  ! Массив не загружен!\n");
            }
            else {
                printf("\n  Текущий массив (%d элементов):\n  ", size);
                print_array(original_arr, size);
            }
            printf("\n  Нажмите Enter для продолжения...");
            getchar();
            break;
        }

        case 5: {
            if (original_arr == NULL) {
                printf("\n  ! Ошибка: сначала загрузите или сгенерируйте массив!\n");
                printf("\n  Нажмите Enter для продолжения...");
                getchar();
                break;
            }

            printf("\n  Выполняется тестирование...\n");

            
            int* rand_arr = copy_array(original_arr, size);
            SortResult r1 = shaker_sort(rand_arr, size);
            free_array(rand_arr);

           
            int* sorted_arr = generate_sorted_array(size);
            SortResult r2 = shaker_sort(sorted_arr, size);
            free_array(sorted_arr);

            
            int* rev_arr = generate_reversed_array(size);
            SortResult r3 = shaker_sort(rev_arr, size);
            free_array(rev_arr);

            print_test_results(size, r1, r2, r3);

            free_array(r1.array);
            free_array(r2.array);
            free_array(r3.array);

            printf("  Нажмите Enter для продолжения...");
            getchar();
            break;
        }

        case 6: {
            if (original_arr == NULL) {
                printf("\n  ! Ошибка: нет данных для сохранения!\n");
                printf("\n  Нажмите Enter для продолжения...");
                getchar();
                break;
            }

            printf("\n  Введите имя файла для сохранения: ");
            fgets(filename, sizeof(filename), stdin);
            size_t len = strlen(filename);
            if (len > 0 && filename[len - 1] == '\n') {
                filename[len - 1] = '\0';
            }

            if (write_to_file(filename, original_arr, size)) {
                printf("\n  ✓ Массив успешно сохранён в файл '%s'\n", filename);
            }
            else {
                printf("\n  ! Ошибка при сохранении!\n");
            }
            printf("\n  Нажмите Enter для продолжения...");
            getchar();
            break;
        }

        case 7: {
            print_info();
            break;
        }

        default: {
            printf("\n  ! Неверный выбор! Попробуйте снова.\n");
            printf("\n  Нажмите Enter для продолжения...");
            getchar();
            break;
        }
        }
    }

    return 0;
}