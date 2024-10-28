#include <iostream>
#include "server.hh"

using namespace std;

int main() {
    Server s(49153);

    s.bindAndListen();

    return 0;
}