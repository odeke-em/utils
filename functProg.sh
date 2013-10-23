#!/bin/bash
#Author: Emmanuel Odeke <odeke@ualberta.ca>

#Cheer up your shell with functional programming concepts!

function map() {
  declare -a mappedResult 
  test $# -gt 1 || eval "echo Two args expected \<iterator\> \<function\> && exit"
  mapFunc=$1
  shift 1

  for arg in $*
  do
    result=$(eval $mapFunc "$arg")
    mappedResult+=$result" "
  done
  echo "${mappedResult}"
}

function reduce() {
  declare -a reducedResults
  reductionFunc=$1;
  shift 1

  prevElem=""
  currentElem=""
  while [ $# -gt 0 ]
  do 
    #test $currentElem || break;
    prevElem=$currentElem
    currentElem=$1

    shift 1
    result=$($reductionFunc "$currentElem"  "$prevElem")
    reducedResults+=$result" "
  done

  echo "${reducedResults:0}"
}

function rmSpaces() {
  test -e $1 || eval "echo Expecting valid existant filepaths && exit";
  #echo "In rmSpaces"
  #Making the path lower case and transforming all spaces to underscores
  modF=$(echo "$@" | tr A-Z a-z | sed s/" "/_/g)
  echo "$modF"
}

function main() {
  mapRes=$(map "expr 2 \* " $(seq 1 10))
  reduce "echo " $(ls /)

  for mp in ${mapRes}
  do
    echo "$mp";
  done
}

if [ $# -lt 1 ] 
then
  echo "Usage:: $0"
else
  if [ $1 == "main" ]
  then 
    main
  fi
fi
