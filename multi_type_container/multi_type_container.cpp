// multi_type_container.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "container_type.hpp"

int main()
{
	multi_type_container::Container<C, C1, C2> container;
	container.emplace<C>(1, 5);
	container.emplace<C1>(2, 6, 60);
	container.emplace<C2>(3, 7, 70);

	C& c = container.get_item<C>(1);
	C& c1 = container.get_item<C>(2);
	C& c2 = container.get_item<C>(3);

	std::cout << c.a << "\n";
	std::cout << c1.a << "\n";
	std::cout << c2.a << "\n";

    std::cout << "Hello World!\n"; 
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
