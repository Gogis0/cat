#!/bin/bash

REPS=5

print_success() {
    # Print output in green
    printf "\e[0;32m  [✔] $1\e[0m\n"
}

print_fail() {
    # Print output in red
    printf "\e[0;31m  [✖] $1 \e[0m\n"
}

# run a single .in, compare to corresponding .out
run () {
    # echo -n "$2: $("du -h $2 | cut -f1") "
    echo "$2: "

    COUNTER=0
    my_runtimes=()
    cat_runtimes=()
    while [ $COUNTER -le $REPS ]; do
        # mu run
        START=$(date +%s.%N)

        DIFF=$(./$1 < $2 | diff - ${2/in/in} )
        if [ "$DIFF" != "" ]; then
            echo -e "WRONG ANSWER:\n$DIFF "
            break
        fi
        
        my_time=$(echo "$(date +%s.%N) - $START" | bc)
        
        # cat run
        START=$(date +%s.%N)
        DIFF=$(cat < $2 | diff - ${2/in/in} )
        cat_time=$(echo "$(date +%s.%N) - $START" | bc)

        if [ $(echo "$my_time<$cat_time" | bc -l) -eq 1 ]; then
            print_success "ja: $my_time  cat: $cat_time"
        else
            print_fail "ja: $my_time  cat: $cat_time"
        fi

       let COUNTER=COUNTER+1

    done
    
}

# if test case # is specified run it, else run all of them
if [ "$#" -eq 2 ]; then
    run $1 $(echo "tests/$2.in")
else
    for input in tests/*.in; do
        run $1 $input
    done
fi
