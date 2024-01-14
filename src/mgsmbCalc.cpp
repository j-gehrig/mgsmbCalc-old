#include <iostream>
#include <string>
#include <cmath>
#include "eval.h"

double round_up(double value, int decimal_places) {
    const double multiplier = std::pow(10.0, decimal_places);
    return std::ceil(value * multiplier) / multiplier;
}

int main() {
    const int digits = 25;

    std::cout << " -- mgsmbCalc -- \n"
              << "C++ calculator made by Johannes Gehrig Riveros \n"
              << "https://github.com/j-gehrig\n\n";

    while(true) {
        std::cout << "Please enter the term, which is to be calculated:\n\n";
        char input[1024];
        std::cin >> input;
        const char* term = input;

        std::cout << "\nCalculating term \"" << term << "\"...\n";

        double ans = round_up(Eval::eval(term), digits);

        std::cout << "answer: " << ans << "\n\nStop program? (y/N) ";

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer

        char response;
        std::cin.get(response);

        std::cout << "\n\n";

        if(response == 'y' || response == 'Y') break;
    }
    std::cout << std::endl;
    return 0;
}
