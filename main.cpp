#include <iostream>

#include "Application.h"

int main(int argc, char** argv)
{
    std::cout << "Hello RGS! " << std::endl;

    RGS::Application app("RGS", 800, 600);

    app.Run();

    return 0;
}