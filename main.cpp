#include "main.h"
#include <thread>

int main()
{
	std::thread t1([]()
		{
			websocket_example();
		}
	);
	std::thread t2([]()
		{
			multiple_app_instances();
		}
	);
	t1.join();
	t2.join();
}