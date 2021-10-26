/*
 * Данная программа реализует схему решения задачи
 * "читатели-писатели", используя STL.
 *
 * Приоритет писателя.
 * "Как только появился хоть один писатель, никого больше не пускать.
 * Все остальные могут простаивать"
 *
 * Количество читателей, писателей и повторений, задержка, а также возможность
 * случайно генерировать задержку (добавляя случайное число от 0-3
 * к установленной) чтения/записи устанавливаются директивами препроцессора.
 */



#define READERS_NUM 5  // Количество читателей
#define WRITERS_NUM 3  // Количество писателей

#define NUM_OF_REPEATS 3  // Количество повторений

#define READ_DELAY 2   // Задержка чтения
#define WRITE_DELAY 3  // Задержка записи

#define RANDOMIZE_DELAY true  // Рандомизировать задержку



#include <iostream>
#include <thread>
#include <shared_mutex>
#include <mutex>
#include <vector>

/// Общий ресурс для чтения-записи
int resource = 1;

// Mutex для читателей

std::shared_mutex rw_mutex;
std::mutex output_mutex;

void output() {
    std::shared_lock r_guard(rw_mutex);

    std::lock_guard output_guard(output_mutex);
    std::cout << resource << std::endl;
}

void write() {
    std::unique_lock w_guard(rw_mutex);
    ++resource;
}

void reader() {
    int repeats_num = NUM_OF_REPEATS;

    while (repeats_num > 0) {
        output();

        // Ожидаем
        int randomized = 0;

        if (RANDOMIZE_DELAY) {
            randomized = rand() % 4;
        }

        sleep(READ_DELAY + randomized);

        --repeats_num;
    }
}

void writer() {
    int repeats_num = NUM_OF_REPEATS;

    while (repeats_num > 0) {
        write();

        // Ожидаем
        int randomized = 0;

        if (RANDOMIZE_DELAY) {
            randomized = rand() % 4;
        }

        sleep(WRITE_DELAY + randomized);

        --repeats_num;
    }
}

int main() {
    std::thread threads[READERS_NUM + WRITERS_NUM];

    std::size_t i = 0;

    for (; i < READERS_NUM; ++i) {
        threads[i] = std::thread(reader);
    }

    for (int j = 0; j < WRITERS_NUM; ++j, ++i) {
        threads[i] = std::thread(writer);
    }

    for (i = 0; i < READERS_NUM + WRITERS_NUM; ++i) {
        threads[i].join();
    }
}