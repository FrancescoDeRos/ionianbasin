#!/bin/sh
for i in *.sac
do
sac << qsac > /dev/null
r $i
ch lovrok true
ch lcalda true
ch EVLA  38.8898  EVLO 16.4857    EVDP 26
wh
quit
qsac
done

