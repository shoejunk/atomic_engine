#include "utility.h"

namespace atom
{
#ifdef _DEBUG
    c_logger<true, false> debug;
    c_logger<true, true> debugln;
#else
    c_logger<true, false, true> debug;
    c_logger<true, true, true> debugln;
#endif
    c_logger<true, false, false, std::cout> log("log.txt");
    c_logger<true, true, false, std::cout> logln("log.txt");
    c_logger<true, false, false, std::cerr> error("log.txt");
    c_logger<true, true, false, std::cerr> errorln("log.txt");
}
