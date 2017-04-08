#!/bin/sh


function ProgressBar {
# Process data
    let "_progress=(${1}*100/${2}*100)/100"
    let "_done=(${_progress}*4)/10"
    let "_left=40-$_done"
# Build progressbar string lengths
    _fill=$(printf "%${_done}s")
    _empty=$(printf "%${_left}s")

# Progress : [########################################] 100%
printf "\rProgress : [${_fill// /#}${_empty// /-}] ${1}/${2}"
}

# Variables
_start=1

# This accounts as the "totalState" variable for the ProgressBar function
_end=100

count_all=0
count_ok=0

# Failed test
failed_tests=""


printf 'TEST: Uniform Random-3-SAT, phase transition region, unforced filtered\n'
printf '125 variables, 538 clauses - 100 instances, all SATISFIABLE\n'
printf '\n'


printf 'Set the number of tests >> '
read num

if [[ $num -le 0 || $num -gt _end ]]; then
	printf 'incorrect number of tests\n'
	exit
else
	_end=$num
fi


START=$(date +%s)


for test_idx in $(seq ${_start} ${_end}); # num test: [1, 1000]
do	
	RES="SATISFIABLE"
	RES_OUR=$(../../../DPLL ./data/uf125-538/uf125-0${test_idx}.cnf)

	if [[ $RES == $RES_OUR ]]; then	
		count_ok=$((count_ok+1))
	else
		failed_tests=$failed_tests" \tuf125-0${test_idx}.cnf\n"	
	fi
	count_all=$((count_all+1))

        ProgressBar ${test_idx} ${_end}
done

printf '\nFinished!\nPASSED: '
echo $count_ok/$count_all

if [[ $count_ok != $_end ]]; then 
	printf 'FAILED:\n'
	echo -e $failed_tests
fi



END=$(date +%s)
DIFF=$(( $END - $START ))
echo "$DIFF seconds"
