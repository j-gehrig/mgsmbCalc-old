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

    std::string term;
    std::cout << " -- mgsmbCalc -- \n"
              << "C++ calculator made by Johannes Gehrig Riveros \n"
              << "https://github.com/j-gehrig\n\n";

    while(true) {
        std::cout << "Please enter the term, which is to be calculated:\n\n";
        std::getline(std::cin, term);
        const std::string input = term;

        std::cout << "\nCalculating term \"" << input << "\"...\n";

        double ans;
        if(term.size() > 2) { 
            ans = round_up(Eval::eval(term), digits);
        } else { // Has to be only a number; skips the evaluation functions
            ans = std::atof(term.c_str());
        }

        std::cout << "answer: " << ans << "\n\nStop program? (y/N) ";

        std::string response;
        std::getline(std::cin, response);

        std::cout << "\n\n";

        if(response == "y" || response == "Y") break;
    }
    std::cout << std::endl;
    return 0;
}
