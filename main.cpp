#include <iostream>
#include <vector>
#include <set>
#include <fstream>


using namespace std;

// TODO: DEBUG
string show_expr(const vector<vector<int>> &expr) {
    string result;
    for (vector<vector<int>>::const_iterator it_clauses = expr.begin(); it_clauses != expr.end(); ++it_clauses) {
        result += "{";
        for (vector<int>::const_iterator it_var = (*it_clauses).begin(); it_var != (*it_clauses).end(); ++it_var)
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

bool dimacs_parser(string filename, vector<vector<int>> &expr, unsigned int& var_num, unsigned int& clauses_num) {
    string lit;
    int var;
    string line;

    //cout << "FILENAME=" << filename << endl;
    ifstream fs(filename);
    if (fs.is_open()) {
        while (fs.good()) {
            while (lit != "cnf") {
                fs >> lit;
            }

            fs >> var_num;
            fs >> clauses_num;

            for(int i=0; i < clauses_num; i++) {
                vector<int> clause;
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

int isPureLiteral(const vector<vector<int>> &expr, int literal) {
    int search = 0;
    for (int clause_idx = 0; clause_idx < expr.size(); clause_idx++) {
        for (int var_idx = 0; var_idx < expr[clause_idx].size(); var_idx++) {
            int var = expr[clause_idx][var_idx];
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

vector<vector<int>> simplification(vector<vector<int>> expr, int lit) {
    vector<int> idx_clauses_to_delete;

    for (int clause_idx = 0; clause_idx < expr.size(); clause_idx++) {
        vector<int> idx_vars_to_delete;
        for (int var_idx = 0; var_idx < expr[clause_idx].size(); var_idx++) {
            if (abs(lit) == abs(expr[clause_idx][var_idx])) {
                if (expr[clause_idx][var_idx] * lit > 0 ) {
                    idx_clauses_to_delete.push_back(clause_idx);
                    break;
                } else {
                    idx_vars_to_delete.push_back(var_idx);
                    break;
                }
            }
        }

        for (int i = 0; i < idx_vars_to_delete.size(); i++) {
            expr[clause_idx].erase(expr[clause_idx-i].begin() + idx_vars_to_delete[i]);
        }
    }

    for (int i = 0; i < idx_clauses_to_delete.size(); i++) {
        expr.erase(expr.begin() + idx_clauses_to_delete[i]-i);
    }

    return expr;
}



//======================================================================================================================
// Algorithm functions
//======================================================================================================================

bool isEmptyClause(const vector<vector<int>> &expr) {
    for(int i = 0; i<expr.size(); i++) {
        if(expr[i].empty()) {
            return true;
        }
    }
    return false;
}

bool unitPropagation(vector<vector<int>> &expr, set<int> &var_true, set<int> &var_false, set<int> &var_unset) {
    bool flag = false;

    while(1) {
        int unit = 0;
        vector<vector<int>>::iterator unit_it;
        for (vector<vector<int>>::iterator it = expr.begin(); it != expr.end(); ++it) {
            if ((*it).size() == 1) {
                unit = (*it)[0];
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

        expr = simplification(expr, unit);
    }

}

bool pureLiteralElimination(vector<vector<int>> &expr, set<int> &var_true, set<int> &var_false, set<int> &var_unset) {
    bool flag = false;

    vector<int> pure_literals;

    for(set<int>::iterator var_it = var_unset.begin(); var_it != var_unset.end(); var_it++) {
        int pure_lit = isPureLiteral(expr, *var_it);
        if(pure_lit != 0) {
            flag = true;
            pure_literals.push_back(pure_lit);

            expr = simplification(expr, pure_lit);
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

int chooseLiteral(const vector<vector<int>> &expr, set<int> var_unset) {
    int var = 0;
    int max = 0;

    for(set<int>::iterator var_it = var_unset.begin(); var_it != var_unset.end(); var_it++) {
        int num = 0;
        for (int clause_idx = 0; clause_idx < expr.size(); clause_idx++) {
            for (int var_idx = 0; var_idx < expr[clause_idx].size(); var_idx++) {
                if(abs(expr[clause_idx][var_idx]) == *var_it) {
                    num++;
                }
            }
        }

        if(num > max) {
            max = num;
            var = (*var_it);
        }
    }

    return var;
}



//======================================================================================================================
// DPLL algorithm
//======================================================================================================================

bool DPLL(vector<vector<int>> expr, set<int> var_true, set<int> var_false, set<int> var_unset) {

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

        if (pureLiteralElimination(expr, var_true, var_false, var_unset)) {
            simplificationFlag = true;
        }

        if (!simplificationFlag) {
            break;
        }
    }


    int lit = chooseLiteral(expr, var_unset);
    var_unset.erase(abs(lit));
    vector<vector<int>> new_expr;

    new_expr = simplification(expr, lit);
    set<int> new_var_true = var_true;
    new_var_true.insert(lit);
    if (DPLL(new_expr, new_var_true, var_false, var_unset)) {
        return true;
    }


    new_expr = simplification(expr, -lit);
    set<int> new_var_false = var_false;
    new_var_false.insert(lit);
    if (DPLL(new_expr, var_true, new_var_false, var_unset)) {
        return true;
    }

    return false;
}


int main(int argc, char** argv) {

    unsigned int var_num = 0;
    unsigned int clauses_num = 0;
    vector<vector<int>> expression;
    set<int> var_true, var_false, var_unset;

    if(argc > 1) {
        if(!dimacs_parser(string(argv[1]), expression, var_num, clauses_num)) {
            cout << "Unable to open file " << string(argv[1]) << endl;
            return -1;
        }
    } else {
        return -1;
    }

    for(int i = 1; i<=var_num; var_unset.insert(i++));


    if(DPLL(expression, var_true, var_false, var_unset)) {
        cout << "SATISFIABLE" << endl;
    } else {
        cout << "UNSATISFIABLE" << endl;
        return -1;
    };


    return 0;
}
