#!/bin/bash
COMMAND=$1
STAGE=$2

STAGE_FOLDER="etapa-$STAGE"

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

  export)
  TARGZ_FILE=etapa$STAGE.tgz
  # Create workdir
  rm -r .tmp
  mkdir .tmp

  # Export relevant files
  cp $STAGE_FOLDER/Makefile .tmp
  cp $STAGE_FOLDER/main.c .tmp
  cp $STAGE_FOLDER/parser.y .tmp
  cp $STAGE_FOLDER/scanner.l .tmp
  cd .tmp
  tar cvzf $TARGZ_FILE .
  # Copy final submission file
  cp $TARGZ_FILE ../submissions/$TARGZ_FILE

  # Test if our export is OK
  rm testDir
  mkdir testDir
  cp $TARGZ_FILE.tgz testDir
  cd testDir
  tar -xzf $TARGZ_FILE.tgz
  make
  ./etapa$STAGE
  ;;

  *)
  echo "Unkown command `$COMMAND`! Available commands are
  run stageNumber
  test stageNumber
  export stageNumber"
  exit 1
  ;;
esac
