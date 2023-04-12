#include <gsl/gsl>

auto SquareArea(const int side) {
    Expects(side > 0);
    return side * side;
}
