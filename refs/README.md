Documentation, references and such
---------------------
#### tutorial per una semplice simulazione con l'internal mesher
* https://hackmd.io/@loyn/Bkg0pp3Zp
#### specfem python utilities
* https://github.com/aklimase/SPECFEM_valid/tree/master
#### specfem utility with examples to implement external tomographic model
* https://github.com/thurinj/specfem_tomo_helper/tree/main
#### notebook per i vari esempi con il mesher interno che ho provato ad eseguire intorno a feb/mar
* https://github.com/SeisSCOPED/HPS-book/tree/main/book/chapters/specfem
#### script utili opensource di Chow, consultati per raffigurare le sezioni in latitudine e longitudine
* https://github.com/bch0w/simutils/tree/master
#### spiegazione dettagliata dei vari parametri
* https://ranoriginals.github.io/2020/03/sem3d/
---------------------
#### archivio per le zone sismogeniche usate per la creazione del modello ICLARC
* http://zonesismiche.mi.ingv.it/
#### sito homepage cubit
* https://coreform.com/cubit_help/cubithelp.htm#t=cubit_users_manual.html
#### cataloghi consultati per la modellazione della sorgente sismica e il confronto Tp Ts
* https://www.emsc-csem.org/
* https://www.globalcmt.org/CMTsearch.html
* https://opencfs.gitlab.io/userdocu/Tutorials/Meshing/Cubit/
* https://wiki.archive.geodynamics.org/software:specfem3d:start
* https://www.visiblegeology.com/
* https://www.bluegreenatlas.com/italy_calabria.html
* https://www.emsc-csem.org/Earthquake_map/view_data.php
#### terremoti utilizzati nelle simulazioni:
* CZLIDO2022: https://www.emsc-csem.org/Earthquake_information/earthquake.php?id=1178343
* IONIO2025: https://www.emsc-csem.org/Earthquake_information/earthquake_data.php?id=1795369
* IONIO2012: https://www.emsc-csem.org/Earthquake_information/earthquake_data.php?id=276667
* SQUILLACE2014: https://www.emsc-csem.org/Earthquake_information/earthquake.php?id=370016
* GREECE2015: https://www.emsc-csem.org/Earthquake_information/earthquake.php?id=470390
* GREECE2018: https://www.emsc-csem.org/Earthquake_information/earthquake.php?id=720235
* GREECE2020: https://www.emsc-csem.org/Earthquake_information/earthquake.php?id=840373
* GREECE2022: https://www.emsc-csem.org/Earthquake_information/earthquake.php?id=1166495
* GREECE2024: https://www.emsc-csem.org/Earthquake_information/earthquake.php?id=1639797
---------------------
#### codice per inserire i tempi teorici nell'header dei file .sac per la procedura di analisi
* https://github.com/ghfbsd/sacbook/blob/master/methods/split/ttsac
* https://github.com/ghfbsd/sacbook/tree/master/methods
* https://github.com/msthorne/SACTOOLS/tree/v1.0.0
* https://hackmd.io/@HuWanLin/SAC
---------------------
#### pagine dettagliate su utilizzo dei nodi e del carico di Newton cluster (necessaria VPN unical)
* http://newton-1.hpcc.unical.it/ganglia/?r=hour&cs=&ce=&c=NEWTON+Cluster&h=&tab=m&vn=&hide-hf=false&m=mem_free&sh=1&z=small&hc=4&host_regex=&max_graphs=0&s=by+name
* http://newton-1.hpcc.unical.it/twiki/bin/view/NewtonWiki/WebHome
---------------------
##### pagina per informazioni su file parametri da usare accoppiati con il mesher interno
* https://hackmd.io/@loyn/Bkg0pp3Zp
* https://docs.xumijian.me/seismo/specfem3d/mesh.html
---------------------
###### varie pagine per lavorare senza fronzoli con SAC
* https://seisman-github-io.translate.goog/SAC_Docs_zh/appendix/naming-convections/?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=it
* https://web.mst.edu/sgao/manuals/sac/
* https://www.slideshare.net/slideshow/seismic-analysis-code-sac/256205677
* https://seisman-github-io.translate.goog/SAC_Docs_zh/graphics/plot-commands/?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=it#plotpk
* https://seisman-github-io.translate.goog/SAC_Docs_zh/data-process/picking-phase/?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=it
* https://moodle.glg.miamioh.edu/brudzimr/tutorials/SAC/
###### vari altri strumenti usati di qui e di là
* http://eqseis.geosc.psu.edu/cammon/HTML/CJA_Guide/sac_guide.html
* https://users.earth.ox.ac.uk/~smachine/cgi/index.php
* https://pdf-cordinate-extractor.vercel.app/
---------------------
###### pagina FAQ per visualizzare i file della topografia della Calabria importati in Paraview
* https://discourse.paraview.org/t/visualization-of-digital-elevation-model-tiff-file-with-x-y-and-z-axis-in-3d-view/15986/7
---------------------
### SPECFEM3D related
Description of the files:
* stf_iclarc.txt : Source time function of Gaussian type.
* stf_imp.txt : Source time function of impulsive type, single non zero value at 1000th step.
* stf_cosine*.txt : STF of different half duration implemented in later runs in Mw > 5.0 Eqs
* CMTSOLUTION_*: Parameter files defining the Moment Tensor sources necessary to simulate the seismic signal. Default source time function is a smoothed Heaviside. External ones have been defined and implemented.

