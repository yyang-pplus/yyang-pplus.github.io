#include <exception>


struct ExitException : public std::exception {
    int error_code;

    ExitException(const int c) : error_code(c) {
    }
};


int main() try {
    // main body
} catch (const ExitException &e) {
    return e.error_code;
} catch (const std::exception &) {
} catch (...) {
    // ensure destuctors of local objects are called
}
