#!/bin/bash
#Author: Emmanuel Odeke <odeke@ualberta.ca>

#Cheer up your shell with functional programming concepts!

function map() {
  declare -a mappedResult 
  test $# -gt 1 || eval "echo Expected:: [\<function\>,\<iterator\>] && exit"
  mapFunc=$1
  shift 1

  for arg in $*
  do
    echo $arg
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
    prevElem=$currentElem
    currentElem=$1

    shift 1
    result=$($reductionFunc "$currentElem"  "$prevElem")
    reducedResults+=$result" "
  done

  echo "${reducedResults:0}"
}

function rmSpaces() {
  echo $1
  test -e $1 || eval "echo Expecting valid existant filepaths && exit";

  #Transforming all spaces to underscores
  modF=$(echo "$@" | sed s/" "/_/g)
  echo "$modF"
}

function main() {
  mapRes=$(map "rmSpaces" $(find ~/Downloads))

  for mp in ${mapRes}
  do
    echo "$mp";
  done
}

triggerArg="main"

[ $# -lt 1 ] && echo "Usage:: $0 $triggerArg" && exit;

if [ $1 ==  $triggerArg ]
then 
  main
else
  echo "Psst, try passing '$triggerArg' as the first argument"
fi
