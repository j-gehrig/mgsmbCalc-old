#include <cstddef>
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#include <cfloat>
#include <cmath>
#include "../include/math.h" // my own library for math functions https://github.com/j-gehrig/math

#define stringSizeType  std::string::size_type

using namespace std;

bool isMathOperatorOrDecimalPoint(char c) {
    const char operators[5] = {'+', '-', '*', '/', ','};
    for(char operatorVar : operators) {
        if(c == operatorVar) return true;
    }
    return false;
}
bool isBracket(char c) { 
    const char brackets[2] = {'(', ')'};
    for(char bracket : brackets) {
        if(c == bracket) return true;
    }
    return false;
}
bool isPartOfMathFunction(char c) { 
    return (!isdigit(c) && !isBracket(c) && !isMathOperatorOrDecimalPoint(c));
}
bool isStartOfLeftNumber(char firstChar, char secondChar, int mode, char mathOperator) {
    bool res = (!isdigit(secondChar) && secondChar != '.')
                && (mode == 1 
                    && (secondChar == '+' || secondChar == '-')
                    && firstChar != mathOperator);
    return res;
} 
bool isEndOfRightNumber(char b) {
    bool res = (!isdigit(b) && b != '.');
    return res;
}


// -- eval functions --

void evalArithmeticOperators(string &term, int mode) {
    // This function calculates the parts of the term with the basic arithmetic operators (+-*/)
    // The mode determines the order of the calculations

    char operators[4] = {'*', '/', '+', '-'};
    int operatorOneIndex = 2 * mode; // either  2 * 0 = 0  or  2 * 1 = 2
    int operatorTwoIndex = 2 * mode + 1;

    while(mode == 0 || mode == 1) {
        // Loops until all the operators are calculated (except a possible sign at the beginning)
        if((term.substr(1).find(operators[operatorOneIndex]) == std::string::npos) && (term.substr(1).find(operators[operatorTwoIndex]) == std::string::npos)) {
            mode++;
        }
        for(stringSizeType i = 1; i < term.size(); i++) { // Go through every character of the term
            if(term[i] == operators[operatorOneIndex] || term[i] == operatorTwoIndex) {
                // Checks whether current operator is the one corresponding to the mode
                double leftNumber; // Number to the left of the operator
                double rightNumber; // Number to the right of the operator
                double calculatedNumber;

                stringSizeType startIndexOfLeftNumber;
                stringSizeType endIndexOfRightNumber;

                char operatorVar = term[i];
                stringSizeType operatorIndex = i;

                // Consider making seperate functions
                for(stringSizeType leftCharIndex = operatorIndex-1; ; leftCharIndex--) { 
                    // Goes through the chars to the left of the operator to find the left Number
                    char secondChar = term[leftCharIndex];
                    char firstChar;
                    if(leftCharIndex > 0) { // This makes sure the index is never out of bounds
                        firstChar = term[leftCharIndex-1];
                    } else {
                        firstChar = secondChar;
                    }
                    if(isStartOfLeftNumber(firstChar, secondChar, mode, operatorVar) && term[0] != '-') { 
                        // Checks whether the beginning of the first number, which is being calculated, is reached
                        // Also makes sure a negative sign isn't being interpreted as an operator
                        startIndexOfLeftNumber = leftCharIndex+1;
                        stringSizeType substrIndexDelta = operatorIndex-1 - startIndexOfLeftNumber-1;
                        leftNumber = ::atof(term.substr(startIndexOfLeftNumber,substrIndexDelta).c_str());
                        // Substring is converted to C String first, so it can be converted to float using atof
                        break; // Left Number is found
                    } 
                    if(leftCharIndex == 0) {
                        // The left number is the first number of the term
                        leftNumber = ::atof(term.substr(0,operatorIndex).c_str()); 
                        // Substring is converted to C String first, so it can be converted to float using atof
                        startIndexOfLeftNumber = 0;
                        break; // Left Number is found
                    }
                }

                for(stringSizeType rightCharIndex = operatorIndex+1; rightCharIndex < term.size(); rightCharIndex++) { 
                    // Goes through the chars to the right of the operator to find the right Number
                
                    if(rightCharIndex == term.size()-1) { // Checks whether the last character is reached
                        stringSizeType deltaIndex = rightCharIndex-operatorIndex; 
                        // The number of chars the loop has gone through since the beginning of the loop
                        if(deltaIndex == 1) { // Right number is the last digit
                            rightNumber = (double)(term[operatorIndex+1]-'0'); // digit is converted to int and then cast to double
                        } else {
                            rightNumber = ::atof(term.substr(operatorIndex+1,deltaIndex).c_str());
                            // Substring is converted to C String first, so it can be converted to float using atof
                        }
                        endIndexOfRightNumber = rightCharIndex;
                        break; // Left Number is found
                    }

                    char secondRightChar = term[rightCharIndex+1];
                    if(isEndOfRightNumber(secondRightChar)) {
                        stringSizeType deltaIndex = rightCharIndex-operatorIndex;
                        // The number of chars the loop has gone through since the beginning of the loop
                        rightNumber = ::atof(term.substr(operatorIndex+1,deltaIndex).c_str());
                        // Substring is converted to C String first, so it can be converted to float using atof
                        endIndexOfRightNumber = rightCharIndex;
                        break; // Left Number is found
                    } 
                }

                if(operatorVar == '*') {
                    calculatedNumber = leftNumber * rightNumber;
                } else if(operatorVar == '/') {
                    calculatedNumber = leftNumber / rightNumber;
                } else if(operatorVar == '+') {
                    calculatedNumber = leftNumber + rightNumber;
                } else if(operatorVar == '-') {
                    calculatedNumber = leftNumber - rightNumber;
                }

                ostringstream stream;
                if(startIndexOfLeftNumber == 0) {
                    stream << calculatedNumber << term.substr(endIndexOfRightNumber+1);
                    term = stream.str();
                } else if(endIndexOfRightNumber == term.size()-1) {
                    stream << term.substr(0, startIndexOfLeftNumber) << calculatedNumber;
                    term = stream.str();
                } else {
                    stream << term.substr(0, startIndexOfLeftNumber) << calculatedNumber << term.substr(endIndexOfRightNumber+1);
                    term = stream.str();
                }
                break; // Breaks out of for loop
            }
        }
    }

    return;
}

void addMissingBrackets(string &term) {
    int openedBracketCount = 0; // total opened brackets
    int closedBracketCount = 0; // total closed brackets

    for(stringSizeType charIndex = 0; charIndex < term.size(); charIndex++) { 
        // Loops through the term to find all brackets in the term
        if(term[charIndex] == '(') {
            openedBracketCount++;
        } else if(term[charIndex] == ')') {
            closedBracketCount++;
        }
    }

    int bracketCountDifference = closedBracketCount - openedBracketCount;
    // The difference of the closed and opened brackets
    for(;bracketCountDifference < 0; bracketCountDifference++) {
        // Less closed than opened; need to add closed brackets at the end
        term = term.append(")"); 
    }   
    for(;bracketCountDifference > 0; bracketCountDifference--) {
        // More closed than opened; need to add opened brackets at the beginning
        term = "(" + term;
    }
    return;
}

double calcMathFunction(string &mathFunction, const int argumentCount, const char** arguments) {
    if(mathFunction == "pow") {
        if(argumentCount != 2) {
            cout << "Invalid amout of arguments! (2 needed)" << endl;
            return 0;
        }
        return Math::pow(std::strtod(arguments[0], nullptr), std::strtod(arguments[1], nullptr));

    } else if(mathFunction == "exp") {
        if(argumentCount != 1) {
            cout << "Invalid amout of arguments! (1 needed)" << endl;
            return 0;
        }
        return Math::exp(std::strtod(arguments[0], nullptr));

    } else if(mathFunction == "log") {
        if(argumentCount != 2) {
            cout << "Invalid amout of arguments! (2 needed)" << endl;
            return 0;
        }
        return Math::log(std::strtod(arguments[0], nullptr), std::strtod(arguments[1], nullptr));

    } else if(mathFunction == "ln") {
        if(argumentCount != 1) {
            cout << "Invalid amout of arguments! (1 needed)" << endl;
            return 0;
        }
        return Math::ln(std::strtod(arguments[0], nullptr));

    } else if(mathFunction == "sin") {
        if(argumentCount != 1) {
            cout << "Invalid amout of arguments! (1 needed)" << endl;
            return 0;
        }
        return Math::sin(std::strtod(arguments[0], nullptr));

    } else if(mathFunction == "cos") {
        if(argumentCount != 1) {
            cout << "Invalid amout of arguments! (1 needed)" << endl;
            return 0;
        }
        return Math::cos(std::strtod(arguments[0], nullptr));

    } else if(mathFunction == "tan") {
        if(argumentCount != 1) {
            cout << "Invalid amout of arguments! (1 needed)" << endl;
            return 0;
        }
        return Math::tan(std::strtod(arguments[0], nullptr));

    } else if(mathFunction == "arcsin") {
        if(argumentCount != 1) {
            cout << "Invalid amout of arguments! (1 needed)" << endl;
            return 0;
        }
        return Math::arcsin(std::strtod(arguments[0], nullptr));

    } else if(mathFunction == "arccos") {
        if(argumentCount != 1) {
            cout << "Invalid amout of arguments! (1 needed)" << endl;
            return 0;
        }
        return Math::arccos(std::strtod(arguments[0], nullptr));

    } else if(mathFunction == "arctan") {
        if(argumentCount != 1) {
            cout << "Invalid amout of arguments! (1 needed)" << endl;
            return 0;
        }
        return Math::arctan(std::strtod(arguments[0], nullptr));

    } else {
        cout << "Error: Mathematical function \"" << mathFunction << "\" is invalid!" << endl; 
        return 0;
    }
}

void evalInnerBracket(string &term) { // This function calculates the last inner bracket pair
    stringSizeType innerOpenedBracketIndex;
    stringSizeType innerClosedBracketIndex;
    stringSizeType lastClosedBracketIndex;

    for(stringSizeType charIndex = 0; charIndex < term.size(); charIndex++) {
        // Loops through term to find bracket pairs
        if(term[charIndex] == ')') {
            lastClosedBracketIndex = charIndex;
        } else if(term[charIndex] == '(') {
            // The last opened bracket is part of the last inner bracket pair
            innerOpenedBracketIndex = charIndex;
            innerClosedBracketIndex = lastClosedBracketIndex; 
            // The last closed bracket which is found is part of the bracket pair; all other brackets should be ignored
        }
    }

    stringSizeType bracketPairIndexDelta = innerClosedBracketIndex-innerOpenedBracketIndex-1;

    string centerBracketContent = term.substr(innerOpenedBracketIndex+1, bracketPairIndexDelta);
    string evalFunctionArgument = "";

    stringSizeType argIndex = 0;
    int argumentCount = 0;
    const char* arguments[] = {};
    for(stringSizeType charIndex = 0; charIndex < centerBracketContent.size(); charIndex++) {
        // Loops through bracket content to evaluate possible function arguments 
        // (seperated by commas) or the bracket content
        if(centerBracketContent[charIndex] == ',') {
            evalFunctionArgument = centerBracketContent.substr(argIndex, charIndex-1-argIndex);
            evalArithmeticOperators(evalFunctionArgument, 0);
            // A function argument is evaluated

            argIndex = charIndex+1;
            arguments[argumentCount] = evalFunctionArgument.c_str();
            argumentCount++;
        } else if(charIndex == centerBracketContent.size()-1) {
            evalFunctionArgument = centerBracketContent.substr(argIndex, charIndex-1-argIndex);
            evalArithmeticOperators(evalFunctionArgument, 0);
            // The last function argument or bracket content is evaluated

            arguments[argumentCount] = evalFunctionArgument.c_str();
            argumentCount++;
        }
    }
    arguments[argumentCount] = nullptr;

    if(isPartOfMathFunction(term[innerOpenedBracketIndex-1])) {
        // The mathematical function is being calculated here
        string mathFunction = "";
        stringSizeType mathFunctionIndex = innerOpenedBracketIndex-1;
        const char** argumentArray = arguments;

        for(; isPartOfMathFunction(term[mathFunctionIndex]); mathFunctionIndex--) {
            // Get math function
            mathFunction = term[mathFunctionIndex] + mathFunction;
        }
        mathFunctionIndex++;

        string evalMathFunction = std::to_string(calcMathFunction(mathFunction, argumentCount, argumentArray));
        term = term.substr(0,mathFunctionIndex) + evalMathFunction + term.substr(innerClosedBracketIndex+1);
        // Bracket is replaced with it's evaluated content
    } else {
        term = term.substr(0,innerClosedBracketIndex) + evalFunctionArgument + term.substr(innerClosedBracketIndex+1);
        // Bracket is replaced with it's evaluated content
    }

    return;
}

void evalBrackets(string &term) {
    int i = 0;
    while((term.find('(') != string::npos || term.find(')') != string::npos) && i < 1000) {
        evalInnerBracket(term);
        i++;
    }

    return;
}

/*string evalMathFunctions(string term) {
    string bracketContent;
    string bracketResult;
    for(stringSizeType i = 0; i < term.size(); i++) {
        if(term[i] == ')') {
            stringSizeType endIndex = i;
            for(stringSizeType j = i; j >= 0; j--) {
                if(term[j] == '(') {
                    stringSizeType startIndex = j;
                    if(j != 0 && isPartOfMathFunction(term[j-1])) {
                        for(stringSizeType k = j-1; k >= 0; k--) {
                            if(!isPartOfMathFunction(term[k])) {
                                string mathFunction = term.substr(k+1, j-k-1);
                                bracketContent = term.substr(startIndex+1, endIndex-startIndex-1);
                                bracketResult = "(" + bracketContent + ")";
                                evalInnerBracket(bracketResult); // todo: more parameters

                                string newTerm = term.substr(0, k+1) + calcMathFunction(bracketResult, mathFunction);
                                if(endIndex < term.size()-1) {
                                    newTerm += term.substr(endIndex+1);
                                }
                                term = newTerm;
                                break;
                            }
                            if(k == 0) {
                                string mathFunction = term.substr(0, j);
                                bracketContent = term.substr(startIndex+1, endIndex-startIndex-1);
                                bracketResult = "(" + bracketContent + ")";
                                evalInnerBracket(bracketResult);

                                string newTerm = calcMathFunction(bracketResult, mathFunction);
                                if(endIndex < term.size()-1) {
                                    newTerm += term.substr(endIndex+1);
                                }
                                term = newTerm;
                                break;
                            }
                        }

                        i = 0;
                        break;
                    } else {
                        bracketContent = term.substr(startIndex+1, endIndex-startIndex-1);
                        bracketResult = "(" + bracketContent + ")";
                        evalInnerBracket(bracketResult);

                        string newTerm = "";
                        if(startIndex != 0) {
                            newTerm += term.substr(0, startIndex);
                        }
                        newTerm += bracketResult;
                        if(endIndex < term.size()-1) {
                            newTerm += term.substr(endIndex+1);
                        }
                        term = newTerm;

                        i = 0;
                        break;
                    }
                }
            }
        }
    }
    return term;
}*/

namespace Eval {
    double eval(string term) {
        addMissingBrackets(term);

        //term = evalMathFunctions(term);

        evalBrackets(term);

        evalArithmeticOperators(term,0);
    
        return std::atof(term.c_str());
    }
}
/* TODO:
    - Fix brackets
    - Fix issue with number*mathfunction
    - Testing
*/