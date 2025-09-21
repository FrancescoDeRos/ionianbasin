#!/bin/sh
for i in *.sac
do
sac << qsac > /dev/null
r $i
ch lovrok true
ch lcalda true
ch EVLA  37.4747  EVLO 16.7988    EVDP 38.4 
ch nzyear 2012
ch nzjday 186 
ch nzhour 11
ch nzmin 12 
ch nzsec 09
ch nzmsec 930
ch O GMT 2012 186 11 12 14 000 
ch mag 4.5
wh
quit
qsac
done

