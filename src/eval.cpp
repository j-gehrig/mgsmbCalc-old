#include <cstddef>
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#include <cfloat>
#include <cmath>
#include <map>
#include <vector>
#include "../include/math.h" // my own library for math functions https://github.com/j-gehrig/math

#define stringSizeType  std::string::size_type

using namespace std;

bool isMathOperatorOrDecimalPoint(char c) {
    const char operators[5] = {'+', '-', '*', '/', ','};
    for(char operator : operators) {
        if(c == operator) return true;
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

string evalArithmeticOperators(string &term, int mode) {
    // This function calculates the parts of the term with the basic arithmetic operators (+-*/)
    // The mode determines the order of the calculations

    char operators[4] = {'*', '/', '+', '-'};
    int operatorOneIndex = 2 * mode; // either  2 * 0 = 0  or  2 * 1 = 2
    int operatorTwoIndex = 2 * mode + 1;
    if(mode != 0 || mode != 1) {
        return;
    }

    while(true) {
        // Loops until the operators of the mode are not part of the term anymore (except a possible sign at the beginning)
        for(stringSizeType i = 1; i < term.size(); i++) { // Go through every character of the term
            if(term[i] == operators[operatorOneIndex] || term[i] == operatorTwoIndex) {
                // Checks whether current operator is the one corresponding to the mode
                double leftNumber; // Number to the left of the operator
                double rightNumber; // Number to the right of the operator
                double calculatedNumber;

                stringSizeType startIndexOfLeftNumber;
                stringSizeType endIndexOfLeftNumber;

                char operator = term[i];
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
                    if(isStartOfLeftNumber(firstChar, secondChar, mode, operator) && term[0] != '-') { 
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

                ostringstream stream;
                if(operator == '*') {
                    calculatedNumber = leftNumber * rightNumber;
                } else if(operator == '/') {
                    calculatedNumber = leftNumber / rightNumber;
                } else if(operator == '+') {
                    calculatedNumber = leftNumber + rightNumber;
                } else if(operator == '-') {
                    calculatedNumber = leftNumber - rightNumber;
                }

                if(startIndexOfLeftNumber == 0) {
                    stream << calculatedNumber << term.substr(endIndexOfLeftNumber+1);
                    term = stream.str();
                } else if(endIndexOfRightNumber == term.size()-1) {
                    stream << term.substr(0, startIndexOfLeftNumber) << calculatedNumber;
                    term = stream.str();
                } else {
                    stream << term.substr(0, startIndexOfLeftNumber) << calculatedNumber << term.substr(endIndexOfLeftNumber+1);
                    term = stream.str();
                }
                break;
            }
        }
        if((term.substr(1).find(operators[operatorOneIndex]) != std::string::npos) || (term.substr(1).find(operators[operatorOneIndex]) != std::string::npos)) {
            continue;
        } else if(mode == 3) {
            break;
        } else {
            mode++;
        }
    }

    return;
}

string checkBrackets(string term) {
    vector<stringSizeType> openedBrackets{}; // total opened brackets
    vector<stringSizeType> closedBrackets{}; // total closed brackets

    for(stringSizeType i = 0; i < term.size(); i++) {
        if(term[i] == '(') {
            openedBrackets.push_back(i);
        } else if(term[i] == ')') {
            closedBrackets.push_back(i);
        }
    }

    int bracketDiff = static_cast<int>(closedBrackets.size() - openedBrackets.size());
    vector<stringSizeType> newArray;
    if(bracketDiff < 0) { // less closed than opened
        for(;bracketDiff < 0; bracketDiff++) {
            term = term.append(")");
        }
    } else if(bracketDiff > 0) {
        for(stringSizeType i = term.size(); i > 0; i--) { // i = term.size() to solve warn
            if(term[i-1] == ')') {
                term = term.substr(0, i-2) + term.substr(i);
            }
        }
    }
    return term;
}

string calcBracket(string term) {
    map<stringSizeType,stringSizeType> bracketPair{}; // stores every pair of brackets
    vector<stringSizeType> openedBrackets{}; // total opened brackets
    vector<stringSizeType> closedBrackets{}; // total closed brackets
    vector<stringSizeType> stillOpenedBrackets{}; // opened brackets which haven't been asigned a closed bracket yet

    for(stringSizeType i = 0; i < term.size(); i++) {
        if(term[i] == '(') {
            openedBrackets.push_back(i);
            stillOpenedBrackets.push_back(i);
        } else if(term[i] == ')') {
            closedBrackets.push_back(i);
            bracketPair[stillOpenedBrackets[stillOpenedBrackets.size()-1]] = i;
            stillOpenedBrackets.pop_back();
        }
    }

    stringSizeType startIndex = openedBrackets[openedBrackets.size()-1];
    stringSizeType endIndex = bracketPair[startIndex];
    stringSizeType delta = endIndex-startIndex-1;

    string centerBracket = term.substr(startIndex+1,delta);
    string evalCenterBracket = "";
    stringSizeType lastComma = 0;
    for(stringSizeType i = 0; i < centerBracket.size(); i++) {
        if(centerBracket[i] == ',') {
            evalCenterBracket = centerBracket.substr(lastComma, i-1-lastComma);
            evalArithmeticOperators(evalCenterBracket);
            evalCenterBracket += ",";
            lastComma = i+1;
        }
    }
    if(evalCenterBracket.empty()) {
        evalCenterBracket = centerBracket
        evalArithmeticOperators(evalArithmeticOperators, 0);
    }

    return (term.substr(0,startIndex)+evalCenterBracket+term.substr(endIndex+1));
}

string evalBracketStep(string term) {
    int i = 0;
    while(term.find('(') != string::npos || term.find(')') != string::npos) {
        term = calcBracket(term);
        i++;
        if(i == 1000) { break; }
    }

    return term;
}

string calcMathFunction(string term, string mathFunction) {
    string ans = term;
    ostringstream strs; // for conversion
    vector<double> params = {};
    stringSizeType newParamIndex = 0;

    if(term.size() > 1) {
        for(stringSizeType i = 0; i < term.size(); i++) {
            if(term[i] == ',') {
                params.push_back(stod(term.substr(newParamIndex, i-1-newParamIndex)));
                newParamIndex = i+1;
            }
        }
    }

    if(params.empty()) {
        params.push_back(stod(term));
    }

    if(mathFunction == "pow") {
        if(params.size() != 2) {
            cout << "Invalid amout of parameters! (2 needed)" << endl;
            return "0";
        }

        strs << Math::pow(params[0], params[1]);

        ans = strs.str();
        return ans;
    } else if(mathFunction == "exp") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }

        strs << Math::exp(params[0]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "log") {
        if(params.size() != 2) {
            cout << "Invalid amout of parameters! (2 needed)" << endl;
            return "0";
        }

        strs << Math::log(params[0], params[1]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "ln") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }

        strs << Math::ln(params[0]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "sin") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        
        strs << Math::sin(params[0]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "cos") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        
        strs << Math::cos(params[0]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "tan") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        
        strs << Math::tan(params[0]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "arcsin") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        
        strs << Math::arcsin(params[0]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "arccos") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        
        strs << Math::arccos(params[0]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "arctan") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        strs << Math::arctan(params[0]);
        ans = strs.str();
        return ans;
    } else {
        cout << "Error: Mathematical function \"" << mathFunction << "\" is invalid!" << endl; 
        return "0";
    }
}

string evalMathFunctions(string term) {
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
                                bracketResult = calcBracket("(" + bracketContent + ")"); // todo more parameter

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
                                bracketResult = calcBracket("(" + bracketContent + ")");

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
                        bracketResult = calcBracket("(" + bracketContent + ")");

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
}

namespace Eval {
    double eval(string term) {
        term = checkBrackets(term);

        term = evalMathFunctions(term);

        evalArithmeticOperators(term,0);
    
        return std::atof(term.c_str());
    }
}
/* TODO:
    - Fix issue with number*mathfunction
    - Add more comments
    - Make code more readable
    - Use references in functions
    - Testing
*/