#include <iostream>
#include "SpLog.h"

int main(){
    std::cout << "Hellow Sparrow Engine." << std::endl;

    spe::SpLog::Init();
//    spe::SpLog::GetLoggerInstance()->trace("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);
    SpLogTrace("Hello spdlog: {0}, {1}, {3}", __FUNCTION__, 1, 0.14f, true);

    return EXIT_SUCCESS;
}