#include "stdio.h"
#include "glpk.h"
#include "rice/Class.hpp"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

using namespace Rice;

int main(int argc, const char *argv[])
{
    printf("GLPK Version: %s\n", glp_version());
    return 0;
}

void Init_Siris()
{
  Class rb_c = define_class("Siris")
   .define_method("getResponse", &main);

}