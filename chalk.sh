#!/bin/bash
COMMAND=$1
STAGE=$2
OPTION=$3

set -e

STAGE_FOLDER="etapa-$STAGE"
TARGZ_FILE=etapa$STAGE.tgz

case $COMMAND in
  run)
  cd $STAGE_FOLDER
  make
  ./etapa$STAGE
  ;;

  test)
  cd $STAGE_FOLDER
  make test
  bash -c "./tests/test"
  ;;

  visualize)
  cd $STAGE_FOLDER
  make
  rm -f ../test_output.txt
  ./etapa$STAGE < ../test_input.txt > ../test_output.txt 
  xdot ../test_output.txt
  ;;


  clean)
  cd $STAGE_FOLDER
  make clean
  ;;

  export)
  # Create workdir
  rm -rf .tmp
  mkdir .tmp

  # Export relevant files
  cp $STAGE_FOLDER/Makefile .tmp
  cp $STAGE_FOLDER/main.c .tmp
  cp $STAGE_FOLDER/parser.y .tmp
  cp $STAGE_FOLDER/scanner.l .tmp
  cp $STAGE_FOLDER/asd.c .tmp
  cp $STAGE_FOLDER/asd.h .tmp
  
  cd .tmp
  touch $TARGZ_FILE
  tar cvzf $TARGZ_FILE --exclude=$TARGZ_FILE . 
  # Copy final submission file
  cp $TARGZ_FILE ../submissions/$TARGZ_FILE

  # Test if our export is OK
  rm -rf testDir
  mkdir testDir
  cp $TARGZ_FILE testDir
  cd testDir
  tar -xzf $TARGZ_FILE 
  if [ "$OPTION" == "--run" ];  then
    make
    ./etapa$STAGE
  fi
  ;;

  *)
  echo "Unkown command `$COMMAND`! Available commands are
  run stageNumber
  test stageNumber
  export stageNumber
  visualize stageNumber"
  exit 1
  ;;
esac
