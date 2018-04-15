//
// Created by active on 18-4-15.
//

#include "TcpFork.h"

int main() {
    TcpFork tf(12343);
    tf.execute();
    return 0;
}