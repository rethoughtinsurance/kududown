#!/bin/bash

for a in `ls resources/avsc`
do
  name='resources/avsc/'$a;

  n=$(echo $name| cut -d'.' -f 1)
  n2='avro/'$(echo $n| cut -d'/' -f 3)'.h'
  echo $n2;
  cmd='avrogencpp -i '$name' -o '$n2' -n rtip'
#  echo 'command is' $cmd
  `$cmd`
done

