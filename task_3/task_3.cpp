#include <iostream>
#include <mutex>

std::mutex ext_mt;

void swap_lock(std::mutex& m1, std::mutex& m2)
{
    if (&m1 != &m2)
    {
        ext_mt.lock();
        std::swap(m1, m2);
        ext_mt.unlock();
    }
}

void swap_scoped_lock(std::mutex& m1, std::mutex& m2)
{
    if (&m1 != &m2)
    {
        std::scoped_lock lock{ m1, m2 };
        std::swap(m1, m2);
    }
}

void swap_unique_lock(std::mutex& m1, std::mutex& m2)
{
    if (&m1 != &m2)
    {
        std::unique_lock <std::mutex> lock1(m1);
        std::unique_lock <std::mutex> lock2(m2);
        std::swap(m1, m2);
    }
}

int main()
{
    std::mutex m1, m2;
    swap_lock(std::ref(m1), std::ref(m2));
    swap_scoped_lock(std::ref(m1), std::ref(m2));
    swap_unique_lock(std::ref(m1), std::ref(m2));

    std::cout << "Ok" << std::endl;
    return 0;
}

