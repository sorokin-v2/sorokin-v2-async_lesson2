#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <windows.h>

using namespace std::chrono_literals;

int set_cursor_pos(const short int row, const short int col)
{
	COORD coord{ col, row };
	HANDLE hd_out = GetStdHandle(STD_OUTPUT_HANDLE);
	return SetConsoleCursorPosition(hd_out, coord);
}

std::mutex mt;
const int tab_width = 10;

void print_bar(const int thread_num, const int elem_count, const char filler)
{
	auto start = std::chrono::high_resolution_clock::now();
	std::unique_lock<std::mutex> ul(mt);;
	set_cursor_pos(thread_num, 0);
	std::cout << (thread_num + 1) << "  " << std::this_thread::get_id();
	ul.unlock();
	for (int i = 0; i < elem_count; ++i)
	{
		ul.lock();
		set_cursor_pos(thread_num, (tab_width + i));
		std::cout << filler;
		std::this_thread::sleep_for(200ms);	//Имитация задачи, работающей с разделяемыми ресурсами
		ul.unlock();
		std::this_thread::sleep_for(300ms);	//Имитация задачи, НЕ работающей с разделяемыми ресурсами
	}	
	auto stop = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> exec_time = stop - start;
	ul.lock();
	set_cursor_pos(thread_num, (tab_width + elem_count));
	std::cout << " exec_time = " << exec_time.count() << "ms" << std::endl;
}

int main()
{
	int thread_count = 8, element_count = 10;
	std::vector<std::thread> threads;
	try
	{
		for (int i = 0; i < thread_count; ++i)
		{
			threads.push_back(std::thread(print_bar, i, element_count, '*'));
		}
	}
	catch (std::runtime_error& e) 
	{ 
		mt.lock();
		std::cout << e.what() << std::endl; 
		mt.unlock();
	}
	for (auto& th : threads)
	{
		th.join();
	}
	set_cursor_pos(thread_count, 0);	//Здесь все потоки уже отработали, по-этому использовать мютекс не обязательно.
	return 0;
}

