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
            evalCenterBracket += evalStep(centerBracket.substr(lastComma, i-1-lastComma), 1) + ",";
            lastComma = i+1;
        }
    }
    if(evalCenterBracket.empty()) {
        evalCenterBracket = evalStep(centerBracket,1);
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

        strs << pow(params[0], params[1]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "exp") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }

        strs << exp(params[0]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "log") {
        if(params.size() != 2) {
            cout << "Invalid amout of parameters! (2 needed)" << endl;
            return "0";
        }

        strs << log(params[1]) / log(params[0]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "ln") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }

        strs << log(params[0]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "sin") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        
        strs << sin(params[0]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "cos") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        
        strs << cos(params[0]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "tan") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        
        strs << tan(params[0]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "arcsin") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        
        strs << asin(params[0]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "arccos") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        
        strs << acos(params[0]);
        ans = strs.str();
        return ans;
    } else if(mathFunction == "arctan") {
        if(params.size() != 1) {
            cout << "Invalid amout of parameters! (1 needed)" << endl;
            return "0";
        }
        strs << atan(params[0]);
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
                            cout << "Test: k = " << k << endl;
                            if(!isPartOfMathFunction(term[k])) {
                                string mathFunction = term.substr(k+1, j-k-1);
                                cout << "Test: mathFunction = " << mathFunction << endl;
                                bracketContent = term.substr(startIndex+1, endIndex-startIndex-1);
                                cout << "Test: bracketContent = " << bracketContent << endl;
                                bracketResult = calcBracket("(" + bracketContent + ")"); // todo more parameter
                                cout << "Test: bracketResult = " << bracketResult << endl;

                                string newTerm = term.substr(0, k+1) + calcMathFunction(bracketResult, mathFunction);
                                if(endIndex < term.size()-1) {
                                    newTerm += term.substr(endIndex+1);
                                }
                                cout << "Test: newTerm = " << newTerm << endl;
                                term = newTerm;
                                break;
                            }
                            if(k == 0) {
                                string mathFunction = term.substr(0, j);
                                bracketContent = term.substr(startIndex+1, endIndex-startIndex-1);
                                bracketResult = calcBracket("(" + bracketContent + ")");
                                cout << "Test: mathFunction = " << mathFunction << endl;
                                cout << "Test: bracketContent = " << bracketContent << endl;
                                cout << "Test: bracketResult = " << bracketResult << endl;

                                string newTerm = calcMathFunction(bracketResult, mathFunction);
                                cout << "Test: newTerm = " << newTerm << endl;
                                if(endIndex < term.size()-1) {
                                    newTerm += term.substr(endIndex+1);
                                cout << "Test: newTerm = " << newTerm << endl;
                                }
                                cout << "Test: newTerm = " << newTerm << endl;
                                term = newTerm;
                                break;
                            }
                        }

                        i = 0;
                        break;
                    } else {
                        bracketContent = term.substr(startIndex+1, endIndex-startIndex-1);
                        bracketResult = calcBracket("(" + bracketContent + ")");
                        cout << "Test: bracketContent = " << bracketContent << endl;
                        cout << "Test: bracketResult = " << bracketResult << endl;

                        string newTerm = "";
                        if(startIndex != 0) {
                            newTerm += term.substr(0, startIndex);
                        }
                        newTerm += bracketResult;
                        if(endIndex < term.size()-1) {
                            newTerm += term.substr(endIndex+1);
                        }
                        cout << "Test: newTerm = " << newTerm << endl;
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

double eval(string term) {
    term = checkBrackets(term);

    term = evalMathFunctions(term);

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
