#!/bin/sh

LOG=log.txt

[ -f $LOG ] && rm $LOG
touch $LOG
echo lpc var cls miniSAT OurSolver Res >> $LOG


function ProgressBar {
# Process data
    let "_progress=(${1}*100/${2}*100)/100"
    let "_done=(${_progress}*4)/10"
    let "_left=40-$_done"
# Build progressbar string lengths
    _fill=$(printf "%${_done}s")
    _empty=$(printf "%${_left}s")

# Progress : [########################################] 100%
printf "\rProgress : [${_fill// /#}${_empty// /-}] ${_progress}%%"
}

# Variables
_start=2

# This accounts as the "totalState" variable for the ProgressBar function
_end=5

clause_from=2
clause_to=8

count_all=0
count_ok=0

[ -d ./data/tmp ] && rm -rf ./data/tmp
mkdir ./data/tmp

for numvar in $(seq ${_start} ${_end}); # num var: [2, 10000]
do
	for numclause in $(seq ${clause_from} ${clause_to}); # num clauses: [2, 10000]
	do
		for lits_per_clause in `seq 3 $(($numvar>20?20:$numvar))` # lits per clause: [3, 20]
		do
			wget https://toughsat.appspot.com/generate --post-data "type=random_ksat&lits_per_clause=$lits_per_clause&numvariables=$numvar&numclauses=$numclause&generate=Generate" \
              -O data/tmp/${lits_per_clause}_${numvar}_${numclause}.dimacs > /dev/null 2>&1

			RES=$(./MiniSat_v1.14_linux data/tmp/${lits_per_clause}_${numvar}_${numclause}.dimacs | tail -n 1 )

	                RES_OUR=$(../../DPLL data/tmp/${lits_per_clause}_${numvar}_${numclause}.dimacs)
			echo -n ${lits_per_clause} ${numvar} ${numclause} $RES $RES_OUR" " >> $LOG
			#RES_OUR='qq'
			if [[ $RES == $RES_OUR ]]; then
				count_ok=$((count_ok+1))
				echo OK >> $LOG
			else
				echo Fail >> $LOG
			fi
			count_all=$((count_all+1))


		done
	done
        ProgressBar $numvar ${_end}
done

printf '\nFinished!\n'
echo $count_ok/$count_all
echo $count_ok/$count_all >> $LOG
