# DPLL-solver
DPLL solver written in C++ with shell tests
  
  
**DPLL solver:**
* run with argument: **./DPLL formula.cnf**
* the argument is **DIMACS-format** file 
* return values:
  * **SATISFIABLE**
  * **UNSATISFIABLE**
  * **UNKNOWN** - if SIGINT (Ctrl+C) was sent
  
  
**Shell tests:**
* GENERATED tests: Random-k-SAT generating on [Tough SAT](https://toughsat.appspot.com/)

* SATISFIABLE tests: Uniform Random-3-SAT from [SATLIB](http://cs.ubc.ca/~hoos/SATLIB/benchm.html)
  * uf20-91:  20 variables, 91 clauses - 1000 instances
  * uf50-218: 50 variables, 218 clauses - 1000 instances
  * uf75-325: 75 variables, 325 clauses - 100 instances
  * uf100-430: 100 variables, 430 clauses - 1000 instances
  * uf125-538: 125 variables, 538 clauses - 100 instances 
  * uf150-645: 150 variables, 645 clauses - 100 instances  
  * uf175-753: 175 variables, 753 clauses - 100 instances 
  
* UNSATISFIABLE tests: Uniform Random-3-SAT from [SATLIB](http://cs.ubc.ca/~hoos/SATLIB/benchm.html)
  * uf20-91:  20 variables, 91 clauses - 1000 instances
  * uf50-218: 50 variables, 218 clauses - 1000 instances
  * uf75-325: 75 variables, 325 clauses - 100 instances
  * uf100-430: 100 variables, 430 clauses - 1000 instances
  * uf125-538: 125 variables, 538 clauses - 100 instances 
  * uf150-645: 150 variables, 645 clauses - 100 instances  
  * uf175-753: 175 variables, 753 clauses - 100 instances 
