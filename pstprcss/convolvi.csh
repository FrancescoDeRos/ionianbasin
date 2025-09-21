#!/bin/csh
ls CAL*sac > li
foreach f1(`cat li`)
set f2=`echo $f1 | sed -e 's/.sac/c.sac/g'`
echo $f1 $f2
sac << qsac
r cos09.sac $f1
convolve
dc 1
w $f2
q
qsac
end

