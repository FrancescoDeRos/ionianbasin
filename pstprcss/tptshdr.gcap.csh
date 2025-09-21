#!/bin/csh

awk '{if(FNR>3) print $1,$5,$6}' eqloc.tempiteorici.txt > tteo.txt 
set n=`wc -l tteo.txt | awk '{print $1}'`
echo $n
set i=1
while ($i <= $n)
awk '{if(FNR == '$i') print}' tteo.txt > tempitemp.324w46w.txt
set sta=`awk '{print $1}' tempitemp.324w46w.txt`
set tp=`awk '{print $2}' tempitemp.324w46w.txt`
set ts=`awk '{print $3}' tempitemp.324w46w.txt`
echo i  $i  sta  $sta  tp $tp  ts $ts
ls ${sta}.[rtzv] > listasacsta.txt
ls *.${sta}.*sac >> listasacsta.txt
foreach fil(`cat listasacsta.txt`)
set o=`saclst o f $fil | awk '{print $2}'`
set tptp=`echo $o $tp | awk '{print $1+$2}'`
set tsts=`echo $o $ts | awk '{print $1+$2}'`
echo i  $i  sta  $sta o $o tptp $tptp tsts $tsts 
sac << qsac > /dev/null
r $fil
ch lovrok true
ch lcalda true
ch t1 $tptp
ch t2 $tsts
echo $tptp $tsts
wh
quit
qsac

end
@ i++
end

