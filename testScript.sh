#!/bin/bash

count=0
echo Test script: A sucessful test has no output. 
echo Unsuccessful will say failed
echo ___________________________________________
for file in `ls tests/*.mi`; do
    name=${file%.*}
    echo Running test $name
    in_file=${name}.mi
    #cat $in_file
    #printf "\nOut File:"
    out_file=${name}.out
    
    ./vm_riskxvii $in_file < ${name}.in | diff - $out_file || echo "Test $name: failed"
    printf "\n\n"
    count=$((count+1))
done
echo "Finished running $count tests!"