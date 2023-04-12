#include <cassert>

#include "assert-on-concept.hpp"
#include "assertion-as-comment.hpp"
#include "assertion-as-precondition.hpp"
#include "expects-as-precondition.hpp"
#include "if-as-precondition.hpp"


int main() {
    f(0);

    Area(1, 2);
    TriangleArea(1, 2);
    SquareArea(2);
}
