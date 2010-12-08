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

rdnt `expr  5 \* 60` -H 200 -w 8   -v 2
rdnt `expr  5 \* 60` -H 190 -w 7   -v 2
rdnt `expr  5 \* 60` -H 180 -w 6   -v 2
rdnt `expr 75 \* 60` -H 180 -w 5.5 -v 2
rdnt `expr  5 \* 60` -H 180 -w 5   -v 2
rdnt `expr  5 \* 60` -H 180 -w 4   -v 2
rdnt `expr  5 \* 60` -H 180 -w 3   -v 2
rdnt `expr 90 \* 60` -H 180 -w 2   -v 2
rdnt `expr  5 \* 60` -H 200 -w 8   -v 3
rdnt -               -H 600 -w 12  -v 3.5


