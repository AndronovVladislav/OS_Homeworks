#!/bin/bash

# превращает аргумент командной строки в целое число
indicator=$(($1))

# рекурсивая функция вычисления факториала, использующая оператор if-then-else
function factorial {
    if [ $1 -eq 0 ]
    then
        echo 1
    else
        local temp=$(( $1 - 1 ))
        local result=$(factorial $temp)
        echo $(( $result * $1 ))
    fi
}

# проверяем, что пользователь не ввёл отрицательное число с помощью оператор while
while [[ $indicator < 0 ]]
do
    echo -n "Invalid number for factorial calculation, enter another: "
    read indicator
done

factorial $indicator