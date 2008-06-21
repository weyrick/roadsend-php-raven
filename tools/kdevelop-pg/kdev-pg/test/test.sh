#!/bin/bash
for case_name in `ls case*.g`; do
    case_number=`echo $case_name | sed -E 's/case([0-9]+)\.g/\1/'`
    expected_name="expected$case_number"
    actual_name=`echo $case_name | sed -E 's/case([0-9]+)/actual\1/'`
    kdev-pg $case_name 2>$actual_name
    diff=`diff -U3 $expected_name $actual_name`
    if [ ${#diff} -eq 0 ]; then
        echo "Test case #$case_number passed"
    else
        echo "Test case #$case_number failed"
        echo "  difference is: $diff"
        echo "================================="
    fi
done
