#include <tuple>

#include "conditional-unused-but-set-parameter-fixed.hpp"
#include "implicit-fallthrough-fixed.hpp"
#include "switch.hpp"
#include "unused-function-fixed.hpp"
#include "unused-parameter-fixed.hpp"
#include "unused-parameter.hpp"
#include "unused-result.hpp"
#include "unused-variable-fixed.hpp"
#include "unused-variable.hpp"


int main() {
    Plus(1, 2);

    ExitSignalHandler(SIGINT);

    Pad(1);
    Pad(2ll);

    connect();
    std::ignore = connect();

    UnusedVariableWarning();
    UnusedVariableNoWarning();

    Dispatch(MyEnum::one);

    FallThroughNoWarning(2);
}
