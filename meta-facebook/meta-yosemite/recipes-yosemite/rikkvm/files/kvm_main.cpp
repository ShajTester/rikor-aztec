#include "ikvm_args.hpp"
#include "ikvm_manager.hpp"
#include "stdio.h"

//using namespace std;

int main(int argc, char* argv[])
{
    ikvm::Args args(argc, argv);
    ikvm::Manager manager(args);

    manager.run();
    printf("iKVM VNC server started\n");

    return 0;
}
