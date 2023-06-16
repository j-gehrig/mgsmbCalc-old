#include <iostream>
#include <string>
#include <sstream>
#include <cfloat>
#include <cmath>
#include <map>
#include <vector>

#define stringSizeType  std::string::size_type

using namespace std;

double round_up(double value, int decimal_places) {
    const double multiplier = std::pow(10.0, decimal_places);
    return std::ceil(value * multiplier) / multiplier;
}

bool isMathOperator(char c) {
    const char oprChars[5] = {'+', '-', '*', '/', ','};
    for(char i : oprChars) {
        if(c == i) return true;
    }
    return false;
}
bool isBracket(char c) {
    const char brackets[2] = {'(', ')'};
    for(char i : brackets) {
        if(c == i) return true;
    }
    return false;
}
bool isPartOfMathFunction(char c) {
    return (!isdigit(c) && !isBracket(c) && !isMathOperator(c));
}
bool isStartOfNewLeftNumber(char a, char b, int mode, char mathOperator) {
    bool res = (!isdigit(b) && b != '.')
                && (mode == 1 
                    && (b == '+' || b == '-')
                    && a != mathOperator);
    //cout << "isStartOfNewLeftNumber: " << res << endl;
    return res;
} 
bool isEndOfNewRightNumber(char b) {
    bool res = (!isdigit(b) && b != '.');
    //cout << "isEndOfNewRightNumber: " << res << endl;
    return res;
} 

string evalStep(string term, int mode) {
    string newterm = term;

    //cout << "____" << endl;
    //cout << "mode: " << mode << endl;

    char opChar1;
    char opChar2;
    if(mode == 1) {
        opChar1 = '*';
        opChar2 = '/';
    } else if(mode == 2) {
        opChar1 = '+';
        opChar2 = '-';
    } else {
        return term;
    }

    if(term.size() < 2) {
        return term;
    }

    for(stringSizeType i = 1; i < term.size(); i++) {
        if(term[i] == opChar1 || term[i] == opChar2) {
            double number1;
            double number2;
            double newnumber;
            stringSizeType startIndex;
            stringSizeType endIndex;

            char mathOperator = term[i];

            for(stringSizeType j = i-1;; j--) {
                char a = term[j];
                char b = term[j];
                if(j > 0) {
                    a = term[j-1];
                }
                //cout << "j: " << j << "; a: " << a << "; b: " << b << endl;
                if(isStartOfNewLeftNumber(a, b, mode, mathOperator) && term[0] != '-') { 
                    number1 = ::atof(term.substr(j+1,i-j-1).c_str());
                    startIndex = j+1;
                    break;
                } 
                if(j == 0) {
                    stringSizeType dx = i;
                    if(dx == 0) {
                        float number_char = (float)(term[0]-'0');
                        number1 = number_char;
                    } else {
                        number1 = ::atof(term.substr(0,dx).c_str());
                    }
                    startIndex = 0;
                    break;
                }
            }
            //cout << "number 1: " << number1 << "; startIndex: " << startIndex << endl;
            for(stringSizeType j = i+1; j < term.size(); j++) { 
                if(j == term.size()-1) {
                    stringSizeType dx = j-i;
                    if(dx == 0) {
                        float number_char = (float)(term[i+1]-'0');
                        number2 = number_char;
                    } else {
                        number2 = ::atof(term.substr(i+1,dx).c_str());
                    }
                    endIndex = j;
                    break;
                }
                char b = term[j+1];
                //cout << "j: " << j << "; a: " << a << "; b: " << b << endl;
                if(isEndOfNewRightNumber(b)) {
                    stringSizeType dx = j-i;
                    if(dx == 0) {
                        float number_char = (float)(term[i+1]-'0');
                        number2 = number_char;
                    } else {
                        number2 = ::atof(term.substr(i+1,dx).c_str());
                    }
                    endIndex = j;
                    break;
                } 
            }
            //cout << "number 2: " << number2 << "; endIndex: " << endIndex << endl;

            ostringstream stream;
            if(mathOperator == '*') {
                newnumber = number1 * number2;
            } else if(mathOperator == '/') {
                newnumber = number1 / number2;
            } else if(mathOperator == '+') {
                newnumber = number1 + number2;
            } else if(mathOperator == '-') {
                newnumber = number1 - number2;
            }

            if(startIndex == 0) {
                stream << newnumber << term.substr(endIndex+1);
                newterm = stream.str();
            } else {
                stream << term.substr(0, startIndex) << newnumber << term.substr(endIndex+1);
                newterm = stream.str();
            }
            //cout << term << " => " << newterm << endl;
            newterm = evalStep(newterm, mode);
            break;
        }
    }
    mode++;
    newterm = evalStep(newterm, mode);

    return newterm;
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

string replaceBracket(string term) {
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
    string centerBracket = evalStep(term.substr(startIndex+1,delta),1);
    return (term.substr(0,startIndex)+centerBracket+term.substr(endIndex+1));
}

string evalBracketStep(string term) {
    int i = 0;
    while(term.find('(') != string::npos || term.find(')') != string::npos) {
        term = replaceBracket(term);
        i++;
        if(i == 1000) { break; }
    }

    return term;
}

string calcMathFunction(string term, string mathFunction) {
    string ans = term;
    vector<string> params = {};
    stringSizeType newParamIndex = 0;

    for(stringSizeType i = 0; term.size(); i++) {
        if(term[i] == ',') {
            params.push_back(term.substr(newParamIndex, i-1-newParamIndex));
            newParamIndex = i+1;
        }
    }

    if(params.size() == 0) {
        params.push_back(term);
    }

    if(mathFunction == "pow") {
        if(params.size() != 2) {
            cout << "Invalid amout of parameters! (2 needed)" << endl;
            return "0";
        }

        ans = pow(stod(params[0]), stod(params[1]));
        return ans;
    } else if(mathFunction == "exp") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }

        ans = exp(stod(params[0]));
        return ans;
    } else if(mathFunction == "log") {
        if(params.size() != 2) {
            cout << "Invalid amout of parameters! (2 needed)" << endl;
            return "0";
        }

        ans = log(stod(params[0]), stod(params[1]));
        return ans;
    } else if(mathFunction == "ln") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }

        ans = log(stod(params[0]));
        return ans;
    } else if(mathFunction == "sin") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        
        ans = sin(stod(params[0]));
        return ans;
    } else if(mathFunction == "cos") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        
        ans = cos(stod(params[0]));
        return ans;
    } else if(mathFunction == "tan") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        
        ans = tan(stod(params[0]));
        return ans;
    } else if(mathFunction == "arcsin") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        
        ans = asin(stod(params[0]));
        return ans;
    } else if(mathFunction == "arccos") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        
        ans = acos(stod(params[0]));
        return ans;
    } else if(mathFunction == "arctan") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        ans = atan(stod(params[0]));
        return ans;
    } else {
        cout << "Error: Mathematical function \"" << mathFunction << "\" is invalid!" << endl; 
        return "0";
    }
}

string evalMathFunctionStep(string term) {
    stringSizeType startIndex;
    stringSizeType endIndex;
    string mathFunction;
    for(stringSizeType i = 0; i < term.size(); i++) {
        if(isPartOfMathFunction(term[i])) {
            for(stringSizeType j = i; j < term.size(); j++) {
                if(term[j] == '(') {
                    startIndex = j;
                } else if(term[j] == ')') {
                    endIndex = j;
                    mathFunction = term.substr(startIndex, startIndex-endIndex);

                    term = term.substr(0, i-1) + "(" + calcMathFunction + term.substr(endIndex);
                    i = j;
                }
            }
        }
    }
}

double eval(string term) {
    term = checkBrackets(term);

    term = evalBracketStep(term);

    term = evalStep(term,1);
    
    return ::atof(term.c_str());
}

int main() {
    const int digits = 25;

    string term;
    cout << "C++ calculator by mgsmemebook." << endl
         << "Please enter the term, which is to be calculated:" << endl << endl;
    getline(cin, term);
    const string input = term;

    cout << endl << "calculating term \"" << input << "\"..." << endl;

    double ans = round_up(eval(term), digits);

    cout << "answer: " << ans << endl << endl;
    return 0;
}