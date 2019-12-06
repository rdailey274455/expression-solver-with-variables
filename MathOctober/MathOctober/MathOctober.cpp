// MathOctober.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "RadExp.cpp"

int main_di();

int main()
	{
	return main_di();
	}

int main_di()
	{
	std::cout<<"f(x,y) = ";
	std::string f_s;
	std::cin>>f_s;
	RadExp f(f_s,nullptr);
	//std::cout<<f.toString();
	std::cout<<"x = ";
	double x;
	std::cin>>x;
	std::cout<<"y = ";
	double y;
	std::cin>>y;
	std::map<std::string,double> twoVars;
	twoVars["x"]=x;
	twoVars["y"]=y;
	std::cout<<"f("+std::to_string(x)+","+std::to_string(y)+") = "<<f.evaluate(twoVars)<<std::endl;

	//std::cout<<"Press any key to exit . . .";
	//std::cin;
	return 0;
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
