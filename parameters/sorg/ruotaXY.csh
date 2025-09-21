#/bin/csh 
chsac cmpinc 0 -f *CXZ*.sac
chsac cmpinc 90 -f *CXY*.sac
chsac cmpinc 90 -f *CXX*.sac
chsac cmpaz  0 -f *CXZ*.sac  *CXY*.sac
chsac cmpaz 90 -f *CXX*.sac

ls *CXY*.sac > listaY.txt
sed -e 's/CXY/CXX/g' listaY.txt > listaX.txt
sed -e 's/CXY/CXR/g' listaY.txt > listaR.txt
sed -e 's/CXY/CXT/g' listaY.txt > listaT.txt
paste listaY.txt listaX.txt listaR.txt listaT.txt | awk '{print "r "$1,$2"\nrotate to gc \nw "$3,$4}' > macroRT

sac << fine
m macroRT
quit
fine
#ch lovrok TRUE -f *.sac
#chsac kcmpnm CXR -f *.CXR*
#chsac kcmpnm CXT -f *.CXT*

foreach cmp(R CXR T CXT)
echo $cmp
sac << fine
r *.${cmp}*.sac
ch lovrok TRUE
ch kcmpnm ${cmp}
wh
w over
q
fine
end

rm lista?.txt

