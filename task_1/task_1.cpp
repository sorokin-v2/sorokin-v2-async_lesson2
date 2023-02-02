﻿#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

using namespace std::chrono_literals;

std::atomic<int> client_count{0};

std::once_flag o_flag;

void start_queue()
{
    client_count.store(1, std::memory_order_seq_cst);
    std::cout << "Добавляем первого клиента" << std::endl;
}

void add_client(const int count)
{
    std::call_once(o_flag, start_queue);
    for (int i = 2; i <= count; ++i)
    {
        std::this_thread::sleep_for(1s);
        client_count.fetch_add(1, std::memory_order_acquire);
        std::cout << "Добавили клиента: " << client_count << std::endl;
    }
}

void del_client()
{
    while (client_count > 0)
    {
        std::call_once(o_flag, start_queue);
        std::this_thread::sleep_for(2s);
        std::cout << "Обрабатываем клиента: " << client_count << std::endl;
        client_count.fetch_sub(1, std::memory_order_acquire);
    }
}


int main()
{
    setlocale(LC_ALL, "Russian");
    std::thread t1(add_client, 10);
    std::thread t2(del_client);
    t1.join();
    t2.join();
    return 0;
}
