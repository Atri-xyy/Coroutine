#include <iostream>
#include "Schedule.h"

void func1() {
	std::cout << "222" << std::endl;
	co_yield();
	std::cout << "222222" << std::endl;
}

void func() {
	std::cout << "111" << std::endl;
	co_go(std::bind(func1));
	std::cout << "111111" << std::endl;
}

int main(void)
{
	co_go(std::bind(func));
	return 0;
}