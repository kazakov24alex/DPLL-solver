#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <csignal>
#include <list>


using namespace std;


// TODO: DEBUG
string show_expr(const std::list<std::list<int>> expr) {
    string result;
    for (auto it_clauses = expr.begin(); it_clauses != expr.end(); ++it_clauses) {
        result += "{";
        for (auto it_var = (*it_clauses).begin(); it_var != (*it_clauses).end(); ++it_var)
            result += (to_string)(*it_var) + ",";
        result += "\b}\n";
    }
    return result;
}
// TODO: DEBUG
string show_set(const set<int> &var_set) {
    string result = "{";
    for(set<int>::const_iterator it = var_set.begin(); it != var_set.end(); ++it) {
        result += (to_string)(*it) + ",";
    }
    if(result.length() != 1)
        result += "\b";
    return result += "}";
}

void sigint_handler(int s) {
    cout << "UNKNOWN" << endl;
    exit(s);
}

bool dimacs_parser(string filename, std::list<std::list<int>> &expr, unsigned int& var_num, unsigned int& clauses_num) {
    string lit;
    int var;
    string line;

    ifstream fs(filename);
    if (fs.is_open()) {
        while (fs.good()) {
            while (lit != "cnf") {
                fs >> lit;
            }

            fs >> var_num;
            fs >> clauses_num;

            for(int i=0; i < clauses_num; i++) {
                std::list<int> clause;
                while (1) {
                    fs >> var;

                    if (var == 0) {
                        break;
                    } else {
                        clause.push_back(var);
                    }
                }
                expr.push_back(clause);
            }
            fs.close();
            return true;
        }

        return true;
    } else {
        return false;
    }
}



//======================================================================================================================
// Auxiliary functions
//======================================================================================================================

int isPureLiteral(const std::list<std::list<int>> &expr, int literal) {
    int search = 0;
    for (auto it_clauses = expr.begin(); it_clauses != expr.end(); ++it_clauses) {
        for (auto it_var = (*it_clauses).begin(); it_var != (*it_clauses).end(); ++it_var) {
            int var = (*it_var);
            if(literal == abs(var)) {
                if(search == 0) {
                    search = var;
                }else if(search*var < 0) {
                    return 0;
                }
                break;
            }
        }
    }
    return search;
}

void simplification(std::list<std::list<int>> &expr, int lit) {
    for (auto it_clauses = expr.begin(); it_clauses != expr.end(); ++it_clauses) {
        for (auto it_var = (*it_clauses).begin(); it_var != (*it_clauses).end(); ++it_var) {
            if (abs(lit) == abs(*it_var)) {
                if ((*it_var) * lit > 0 ) {
                    it_clauses = expr.erase(it_clauses);
                    it_clauses--;
                    break;
                } else {
                    it_var = (*it_clauses).erase(it_var);
                    it_var--;
                    break;
                }
            }
        }
    }
}



//======================================================================================================================
// Algorithm functions
//======================================================================================================================

bool isEmptyClause(const std::list<std::list<int>> &expr) {
    for (auto it_clauses = expr.begin(); it_clauses != expr.end(); ++it_clauses) {
        if((*it_clauses).empty()) {
            return true;
        }
    }
    return false;
}

bool unitPropagation(std::list<std::list<int>> &expr, set<int> &var_true, set<int> &var_false, set<int> &var_unset) {
    bool flag = false;

    while(1) {
        int unit = 0;

        for (auto it_clause = expr.begin(); it_clause != expr.end(); ++it_clause) {
            if ((*it_clause).size() == 1) {
                auto it_var = (*it_clause).begin();
                unit = (*it_var);
            }
        }

        if (unit == 0)
            return flag;
        else
            flag = true;

        var_unset.erase(abs(unit));
        if (unit > 0)
            var_true.insert(abs(unit));
        else
            var_false.insert(abs(unit));

        simplification(expr, unit);
    }

}


bool pureLiteralElimination(std::list<std::list<int>> &expr, set<int> &var_true, set<int> &var_false, set<int> &var_unset) {
    bool flag = false;

    vector<int> pure_literals;

    for(set<int>::iterator var_it = var_unset.begin(); var_it != var_unset.end(); var_it++) {
        int pure_lit = isPureLiteral(expr, *var_it);
        if(pure_lit != 0) {
            flag = true;
            var_unset.erase(abs(pure_lit));

            simplification(expr, pure_lit);
        }
    }

    for(int i = 0; i<pure_literals.size(); i++) {
        var_unset.erase(abs(pure_literals[i]));
        if(pure_literals[i] > 0)
            var_true.insert(abs(pure_literals[i]));
        else
            var_false.insert(abs(pure_literals[i]));
    }

    return flag;
}


int chooseLiteral(const std::list<std::list<int>> &expr, set<int> var_unset) {
    int var = 0;
    int max = 0;

    for(set<int>::iterator set_var_it = var_unset.begin(); set_var_it != var_unset.end(); set_var_it++) {
        int num = 0;
        for (auto it_clauses = expr.begin(); it_clauses != expr.end(); ++it_clauses) {
            for (auto it_var = (*it_clauses).begin(); it_var != (*it_clauses).end(); ++it_var) {
                if(abs(*it_var) == *set_var_it) {
                    num++;
                }
            }
        }

        if(num > max) {
            max = num;
            var = (*set_var_it);
        }
    }

    return var;
}



//======================================================================================================================
// DPLL algorithm
//======================================================================================================================

bool DPLL(std::list<std::list<int>> expr, set<int> var_true, set<int> var_false, set<int> var_unset) {

    while(1) {

        if(expr.empty()) {
            return true;
        }

        if(isEmptyClause(expr)) {
            return false;
        }

        bool simplificationFlag = false;

        if (unitPropagation(expr, var_true, var_false, var_unset)) {
            simplificationFlag = true;
        }

        /*
        if (pureLiteralElimination(expr, var_true, var_false, var_unset)) {
            simplificationFlag = true;
        }
        */

        if (!simplificationFlag) {
            break;
        }
    }


    int lit = chooseLiteral(expr, var_unset);
    var_unset.erase(abs(lit));
    std::list<std::list<int>> new_expr(expr);

    simplification(new_expr, lit);
    set<int> new_var_true = var_true;
    new_var_true.insert(lit);
    if (DPLL(new_expr, new_var_true, var_false, var_unset)) {
        return true;
    }

    std::list<std::list<int>> new_expr2(expr);
    simplification(new_expr2, -lit);
    set<int> new_var_false = var_false;
    new_var_false.insert(lit);
    if (DPLL(new_expr2, var_true, new_var_false, var_unset)) {
        return true;
    }

    return false;
}


int main(int argc, char** argv) {
    signal(SIGINT, sigint_handler);

    unsigned int var_num = 0;
    unsigned int clauses_num = 0;
    std::list<std::list<int>> expression;
    set<int> var_true, var_false, var_unset;

    if(argc > 1) {
        if(!dimacs_parser(string(argv[1]), expression, var_num, clauses_num)) {
            cout << "Unable to open file " << string(argv[1]) << endl;
            return -1;
        }
    } else {
        return -1;
    }

    for(int i = 1; i<=var_num; var_unset.insert(i++)) {};


    if(DPLL(expression, var_true, var_false, var_unset)) {
        cout << "SATISFIABLE" << endl;
    } else {
        cout << "UNSATISFIABLE" << endl;
        return -1;
    };

    return 0;
}
