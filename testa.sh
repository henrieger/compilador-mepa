#!/bin/bash

for dir in testes/*
do
  if [[ -d $dir ]]
  then
    FILE=$dir/pgma.pas
    MEPA=$dir/MEPA
  else
    FILE=$dir
    MEPA="sometrash"
  fi

  echo "Testando arquivo $FILE"
  compilador $FILE > /dev/null
  if [[ $? -eq 0 && -f $MEPA ]]
  then
    diff MEPA $MEPA
  fi
  echo ""
done
