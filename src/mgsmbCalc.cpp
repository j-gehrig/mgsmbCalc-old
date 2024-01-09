#include <iostream>
#include "eval.h"

int main() {
    const int digits = 25;

    char term[];
    std::cout << " -- mgsmbCalc -- \n
                C++ calculator made by Johannes Gehrig Riveros \n
                https://github.com/j-gehrig\n\n";

    while(true) {
        std::cout << "Please enter the term, which is to be calculated:\n\n";
        cin >> term;
        const char input[] = term;

        std::cout << "\nCalculating term \"" << input << "\"...\n";

        double ans = round_up(eval(term), digits);

        std::cout << "answer: " << ans << "\n\nStop program? (y/N) ";

        char response[];
        cin >> response;

        std::cout << "\n\n";

        if(response == "y" || response == "Y") break;
    }
    cout << endl;
    return 0;
}
