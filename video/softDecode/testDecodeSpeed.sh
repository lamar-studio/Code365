#!/bin/bash
 
suffix=' ms'
totalTime=0
sumResult=0
average=0
for (( i=0; i<200; i++ ))
do
    tmpVar=$i$suffix
    result=`grep -wo "$tmpVar" $1 | wc -l`
    sumResult=$(expr $result + $sumResult)
    totalTime=$(expr $totalTime + $(expr $i \* $result ))
    if [ $result -gt 0 -a $result -gt 200 ]; then
        echo "$i$suffix have frame $result counts"
#        echo "sumResult:$sumResult"
#        echo "totalTime:$totalTime"
    fi
done
    average=$(expr $totalTime / $sumResult)
    echo "average:$average ms"

