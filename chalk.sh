#!/bin/bash
COMMAND=$1
STAGE=$2

case $COMMAND in 
  run)
  cd "etapa-$STAGE"
  make
  ./etapa$STAGE
  ;;

  test)
  cd "etapa-$STAGE"
  make test
  bash -c "./tests/test"
  ;;

  *)
  echo "Unkown command `$COMMAND`! Available commands are
  run stageNumber
  test stageNumber"
  exit 1
  ;;
esac
