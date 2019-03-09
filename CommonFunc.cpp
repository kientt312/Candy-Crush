#include "CommonFunc.h"

std::string convertIntToString(int num){
    std::stringstream ss;
    ss << num;
    return ss.str();
}
