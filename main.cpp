/*
 * Readers-Writers problem solution using STL
 *
 * Writers-preference.
 * "No writer, once added to the queue, shall be kept waiting
 * longer than absolutely necessary."
 *
 * Readers and writers amount, read/write delay
 * or random delay generation option (adding 0-3 to user-defined)
 * can be changed by #define directives
 *
 * P.S. #define directives was one of conditions of this task
 */



#define READERS_AMOUNT 5
#define WRITERS_AMOUNT 4

#define REPEATS_AMOUNT 3

#define READ_DELAY 2
#define WRITE_DELAY 2

#define RANDOMIZE_DELAY true  // Randomly add 0-3 to upper values



#include <iostream>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <syncstream>
#include <thread>


class Delayer {
 public:
  Delayer() : eng(r()), dist(0, 3) {}

  void delay() {
    uint randomized = 0;

    if (RANDOMIZE_DELAY) {
      randomized = dist(eng);
    }

    std::this_thread::sleep_for(std::chrono::seconds(READ_DELAY + randomized));
  }

 private:
  std::random_device r = {};
  std::mt19937 eng;
  std::uniform_int_distribution<uint> dist;
};

struct SharedResource {
  explicit SharedResource(uint32_t value_) : value(value_) {}
  uint32_t value;

  mutable std::shared_mutex mtx;
};

void read(const SharedResource& resource) {
  std::shared_lock r_guard(resource.mtx);
  std::osyncstream{std::cout} << resource.value << std::endl;
}

void write(SharedResource& resource) {
  std::unique_lock w_guard(resource.mtx);
  ++resource.value;
}

void reader(const SharedResource& resource) {
  Delayer delayer;
  uint32_t repeats_num = REPEATS_AMOUNT;

  while (repeats_num > 0) {
    read(resource);

    delayer.delay();

    --repeats_num;
  }
}

void writer(SharedResource& resource) {
  Delayer delayer;
  uint32_t repeats_num = REPEATS_AMOUNT;

  while (repeats_num > 0) {
    write(resource);

    delayer.delay();

    --repeats_num;
  }
}

int main() {
  SharedResource resource{1};  // Shared resource for read-write
  std::thread threads[READERS_AMOUNT + WRITERS_AMOUNT];
  std::size_t i = 0;

  for (; i < READERS_AMOUNT; ++i) {
    threads[i] = std::thread(reader, std::cref(resource));
  }

  for (int j = 0; j < WRITERS_AMOUNT; ++j, ++i) {
    threads[i] = std::thread(writer, std::ref(resource));
  }

  for (i = 0; i < READERS_AMOUNT + WRITERS_AMOUNT; ++i) {
    threads[i].join();
  }
}