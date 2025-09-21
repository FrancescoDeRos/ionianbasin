#!/bin/csh

mkdir comuni
saclst kstnm f CAL*sac | awk '{print $2}' | sort | uniq > lical
saclst kstnm f 20*d.sac  | awk '{print $2}' | sort | uniq > lirea
comm lical lirea -1 -2 > stacom.txt
mv stacom.txt comuni/.
cd comuni
foreach sta(`cat stacom.txt`)
cp ../*${sta}*sac .
end

