#include <cassert>

#include "stl/hello.h"

int main() {
    const auto greeting = stl::hello();
    assert(greeting == "hello world");
    return 0;
}
