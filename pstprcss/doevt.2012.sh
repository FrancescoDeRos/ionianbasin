#!/bin/sh
for i in *.sac
do
sac << qsac > /dev/null
r $i
ch lovrok true
ch lcalda true
ch EVLA  37.475  EVLO 16.799    EVDP 38.4
wh
quit
qsac
done

