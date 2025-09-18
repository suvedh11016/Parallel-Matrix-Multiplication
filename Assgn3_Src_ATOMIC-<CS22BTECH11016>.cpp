#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <sys/time.h>

using namespace std;

using Matrix = vector<vector<int>>;

Matrix A;
Matrix result;
atomic<size_t> counter(0);
std::atomic_flag tas_lock = ATOMIC_FLAG_INIT;
std::atomic<int> cas_lock(0);
std::atomic<int> bounded_cas_lock(0);
size_t N, K, rowInc;
mutex mtx;
vector<bool> waiting;
bool go = false;

void multiply_matrix(size_t start_row, size_t end_row)
{
    for (size_t i = start_row; i < end_row; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            for (size_t k = 0; k < N; ++k)
            {
                result[i][j] += A[i][k] * A[k][j];
            }
        }
    }
}

void tas_worker_function(size_t local_rowInc)
{
    size_t local_counter;
    while (true)
    {
        while (tas_lock.test_and_set(std::memory_order_acquire))
            ;

        local_counter = counter;
        counter = counter + local_rowInc;

        tas_lock.clear(std::memory_order_release);
        size_t start_row = local_counter;
        size_t end_row = min(local_counter + local_rowInc, N);
        multiply_matrix(start_row, end_row);


        if (local_counter >= N)
            break;
    }
}

void cas_worker_function(size_t local_rowInc)
{
    size_t local_counter;
    while (true)
    {
        int expected = 0;
        if (cas_lock.compare_exchange_weak(expected, 1, std::memory_order_acquire))
        {
            local_counter = counter;
            counter = counter + local_rowInc;

            cas_lock.store(0, std::memory_order_release);
            size_t start_row = local_counter;
            size_t end_row = min(local_counter + local_rowInc, N);
            multiply_matrix(start_row, end_row);


            if (local_counter >= N)
                break;
        }
    }
}

void bounded_cas_worker_function(size_t i, size_t local_rowInc)
{
    while (true)
    {
        waiting[i] = true;
        int expected = 0;

        while (waiting[i] && !bounded_cas_lock.compare_exchange_weak(expected, 1, std::memory_order_acquire))
        {
            expected = 0; 
        }

        waiting[i] = false;

    
        size_t start_row = counter;
        size_t end_row = min(counter + local_rowInc, N); 
        multiply_matrix(start_row, end_row);
        counter = end_row;

        size_t j = (i + 1) % K;
        while ((j != i) && !waiting[j])
            j = (j + 1) % K;

        if (j == i)
            bounded_cas_lock.store(0, std::memory_order_release);
        else
            waiting[j] = false;

        
        if (counter >= N)
            break;
    }
}

void atomic_worker_function(size_t local_rowInc)
{
    size_t local_counter;
    while ((local_counter = counter.fetch_add(local_rowInc, std::memory_order_relaxed)) < N)
    {
        size_t start_row = local_counter;
        size_t end_row = min(local_counter + local_rowInc, N);

        multiply_matrix(start_row, end_row);
    }
}

long long getCurrentTimeMicroseconds()
{
    timeval currentTime;
    gettimeofday(&currentTime, nullptr);
    return currentTime.tv_sec * 1000000LL + currentTime.tv_usec;
}

int main()
{
    ifstream input("inp.txt");
    input >> N >> K >> rowInc;

    A.resize(N, vector<int>(N));
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            input >> A[i][j];
        }
    }
    input.close();

    result.resize(N, vector<int>(N, 0));
    waiting.resize(N, false);

    vector<thread> threads;


    long long start_time_atomic = getCurrentTimeMicroseconds();
    for (size_t i = 0; i < K; i++)
    {
        threads.emplace_back(atomic_worker_function, rowInc);
    }
    for (auto &t : threads)
    {
        t.join();
    }
    long long end_time_atomic = getCurrentTimeMicroseconds();

    lock_guard<mutex> lock(mtx);
    ofstream output_file("out_atomic.txt", ios::trunc);



    output_file << "Method: Atomic" << endl;
    output_file << "Time taken: " << (end_time_atomic - start_time_atomic) << " microseconds" << endl;

    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            output_file << result[i][j] << " ";
        }
        output_file << endl;
    }

    output_file.close();

    return 0;
}
