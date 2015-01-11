#include <iostream>
#include "Grid.h"
#include "AutoPlanningMethods.h"
#include "rice/Class.hpp"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

using namespace Rice;

int main(int argc, char** argv)
{
//	vector<Position*> teste;
//	Grid *g = new Grid(teste,10);



//	string x = AutoPlanning::executeAutoPlan();
	std::cout << "aaaccccc";
	return 0;
}
string teste()
{
//	vector<Position*> teste;
//	Grid *g = new Grid(teste,10);



//	string x = AutoPlanning::executeAutoPlan();
	return "aaaccccc";
}
extern "C"

void Init_Siris()
{
  Class rb_c = define_class("RiceTest")
   .define_method("teste", &teste);

}
