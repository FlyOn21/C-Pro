//
// Created by flyon21 on 09.11.24.
//
#include <vector>
#include <list>
#include <set>
#include "Logger.h"

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::list<int> lst = {10, 20, 30};
    std::set<int> st = {100, 200, 300};
    Logger& logger = Logger::getInstance();
    logger.logContainer(vec);
    logger.logContainer(lst);
    logger.logContainer(st);
    logger.logContainer({1000, 2000, 3000});
    return 0;
}