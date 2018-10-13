#include "Storage.hpp"
#include <iostream>

int main() {
    auto storage = Storage::getInstance();
    storage->createUser(User("\"test3\"", "12,34\"", "test@ppp.com","13000000000"));
    std::vector<std::string> vec {"\"test3\"", "liuhaohua"};
    storage->createMeeting(Meeting("liuhaohua", vec, Date(2018, 10, 20, 23, 23), Date(2018, 10, 20, 23, 33), "haha"));
    storage->sync();
    return 0;
}