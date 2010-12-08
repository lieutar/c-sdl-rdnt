#! /bin/sh

time=0

rdnt(){
  t=$1
  sleep=$time
  shift;
  tt=
  if ! test $t = -; then
    time=`expr $time + $t`
    tt="-t $t"
  fi 
  (\
    sleep $sleep;\
    ./rdnt $* $tt
  ) &
}

rdnt `expr 40 \* 60` -H 100 -w 5.5 -v 2.5
rdnt `expr  5 \* 60` -H 200 -w 8   -v 3
rdnt -               -H 600 -w 12  -v 3


