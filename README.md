# ionianbasin
##### This repository collects all material related to my master's thesis work for the Ionian basin region. If you have problems executing the code and reproducing the outputs check inside the folder specs/ about the flags/options used on the machines used during the the simulation runs. If you still have issues after this tinkering step do not hesitate to contact me and help solving any problem. The goal is always to start from the end point at which I stopped.
---------------------
#### Closeup view of the interested region: the southern part of Italy, Calabria and most of the Ellenic arc.
<img src="events_runs.png" width="1000" alt="Closeup of the Ionian basin arc">
This main branch includes:
        
    Thesis (.tex, .pdf and all other outputs from Texmaker 
    as well as figures .png & jpegs) (work in progress...)

    Thesis presentation (.odp with TexMaths extension, be sure to 
    have that extension installed otherwise some figures won't render; work in progress...)

    Description of all PCs used throughout the thesis work (configuration flags selected 
    during the software setup, CPU, RAM usage, time for each run etc.): #check folder specs/#

    Complete simulation setup parameter files related to SPECFEM3D Cartesian
    (Par_file, CMTSOLUTION, STATIONS, STFext etc.); #check folder parameters/#

    Meshing and model-building utilities (many thanks to Dr. Rafael Abreu for 
    the help in the mesh modelling); #check folder meshing/# 

    3D and tomographic velocity model scripts 
    (mostly GMT related codes); #check the folder models/#

    Post-processing and plotting scripts (mostly JupyterNotebook, gnuplot and python);
    #check the folder pstprcss/ and for the output check fgs/#

    Documentation and notes on simulation parameters and workflow
    (all the articles that have been read during the thesis writing). #check the folder refs/# 

##### The aim is to maintain a reproducible and transparent archive of the numerical tests conducted for regional waveform modeling studies in the area that has been studied. I hope whichever user interested in these kind of simulations will find this repository useful for its studies.

***
#### Important lookup table for ionianbasin repository -  main sources consulted for Master's thesis work. Uploaded September 2025. Francesco De Rose. Cosenza, Calabria.
### Everything in this page is free to produce, share and build for one's needs. No limits. - To contact ---> derose.fr@gmail.com <-----
### Time and Location - Cosenza, Calabria 22092025.
***

---------------------
Simulations were run with the aid of Newton cluster, Rende(CS) (http://newton-1.hpcc.unical.it) and with the SPECFEM3D software (https://specfem.org/).
SPECFEM3D Cartesian has been used ( Komatitsch and Tromp, 2002; Komatitsch and Tromp, 2002; Komatitsch et al., 9999) published under the GPL 2 license.
Dr. Rafael Abreu's help (IPGP, https://rafaelabreuseis.github.io/)  with the meshing modeling python code and relative notebook is greatly appreciated. Uploaded in September 2025.

#### Komatitsch, D.; Tromp, J. (2002a), Spectral-element simulations of global seismic wave propagation-I. Validation, Geophysical Journal International, 149 (2) , 390-412, doi: 10.1046/j.1365-246X.2002.01653.x, url: http://doi.wiley.com/10.1046/j.1365-246X.2002.01653.x
#### Komatitsch, D.; Tromp, J. (2002b), Spectral-element simulations of global seismic wave propagation–II. Three-dimensional models, oceans, rotation and self-gravitation, Geophysical Journal International, 150 (1) , 303-318
#### Komatitsch, D.; Vilotte, J.-P.; Tromp, J.; Ampuero, J.-P.; Bai, K.; Basini, P.; Blitz, C.; Bozdag, E.; Casarotti, E.; Charles, J.; Chen, M.; Galvez, P.; Goddeke, D.; Hjorleifsdottir, V.; Labarta, J.; Le Goff, N.; Le Loher, P.; Lefebvre, M.; Liu, Q.; Luo, Y.; Maggi, A.; Magnoni, F.; Martin, R.; Matzen, R.; McRitchie, D.; Meschede, M.; Messmer, P.; Michea, D.; Nadh Somala, S.; Nissen-Meyer, T.; Peter, D.; Rietmann, M.; de Andrade, E.S. ; Savage, B.; Schuberth, B.; Sieminski, A.; Strand, L.; Tape, C.; Xie, Z.; Zhu, H. (9999), SPECFEM3D Cartesian [software], doi: 8926d3d3354b9fde5fc3b356189b8ddc4c3a30e5, url: https://geodynamics.org/cig/software/specfem3d/
---------------------
#### tutorial per una semplice simulazione con l'internal mesher
-----> https://hackmd.io/@loyn/Bkg0pp3Zp
#### specfem python utilities
----->https://github.com/aklimase/SPECFEM_valid/tree/master
#### specfem utility with examples to implement external tomographic model
https://github.com/thurinj/specfem_tomo_helper/tree/main
#### notebook per i vari esempi con il mesher interno che ho provato ad eseguire intorno a feb/mar
----->https://github.com/SeisSCOPED/HPS-book/tree/main/book/chapters/specfem
#### script utili opensource di Chow, consultati per raffigurare le sezioni in latitudine e longitudine
----->https://github.com/bch0w/simutils/tree/master
#### spiegazione dettagliata dei vari parametri
----->https://ranoriginals.github.io/2020/03/sem3d/
---------------------
#### archivio per le zone sismogeniche usate per la creazione del modello ICLARC
----->http://zonesismiche.mi.ingv.it/
#### sito homepage cubit
----->https://coreform.com/cubit_help/cubithelp.htm#t=cubit_users_manual.html
#### cataloghi consultati per la modellazione della sorgente sismica e il confronto Tp Ts
----->https://www.emsc-csem.org/
----->https://www.globalcmt.org/CMTsearch.html
----->https://opencfs.gitlab.io/userdocu/Tutorials/Meshing/Cubit/
----->https://wiki.archive.geodynamics.org/software:specfem3d:start
----->https://www.visiblegeology.com/
----->https://www.bluegreenatlas.com/italy_calabria.html
----->https://www.emsc-csem.org/Earthquake_map/view_data.php
#### terremoti utilizzati nelle simulazioni:
CZLIDO2022: https://www.emsc-csem.org/Earthquake_information/earthquake.php?id=1178343
IONIO2025: https://www.emsc-csem.org/Earthquake_information/earthquake_data.php?id=1795369
IONIO2012: https://www.emsc-csem.org/Earthquake_information/earthquake_data.php?id=276667
SQUILLACE2014: https://www.emsc-csem.org/Earthquake_information/earthquake.php?id=370016
GREECE2015: https://www.emsc-csem.org/Earthquake_information/earthquake.php?id=470390
GREECE2018: https://www.emsc-csem.org/Earthquake_information/earthquake.php?id=720235
GREECE2020: https://www.emsc-csem.org/Earthquake_information/earthquake.php?id=840373
GREECE2022: https://www.emsc-csem.org/Earthquake_information/earthquake.php?id=1166495
GREECE2024: https://www.emsc-csem.org/Earthquake_information/earthquake.php?id=1639797
---------------------
#### codice per inserire i tempi teorici nell'header dei file .sac per la procedura di analisi
----->https://github.com/ghfbsd/sacbook/blob/master/methods/split/ttsac
----->https://github.com/ghfbsd/sacbook/tree/master/methods
----->https://github.com/msthorne/SACTOOLS/tree/v1.0.0
----->https://hackmd.io/@HuWanLin/SAC
---------------------
#### pagine dettagliate su utilizzo dei nodi e del carico di Newton cluster (necessaria VPN unical)
----->http://newton-1.hpcc.unical.it/ganglia/?r=hour&cs=&ce=&c=NEWTON+Cluster&h=&tab=m&vn=&hide-hf=false&m=mem_free&sh=1&z=small&hc=4&host_regex=&max_graphs=0&s=by+name
----->http://newton-1.hpcc.unical.it/twiki/bin/view/NewtonWiki/WebHome
---------------------
##### pagina per informazioni su file parametri da usare accoppiati con il mesher interno
----->https://hackmd.io/@loyn/Bkg0pp3Zp
----->https://docs.xumijian.me/seismo/specfem3d/mesh.html
---------------------
###### varie pagine per lavorare senza fronzoli con SAC
----->https://seisman-github-io.translate.goog/SAC_Docs_zh/appendix/naming-convections/?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=it
----->https://web.mst.edu/sgao/manuals/sac/
----->https://www.slideshare.net/slideshow/seismic-analysis-code-sac/256205677
----->https://seisman-github-io.translate.goog/SAC_Docs_zh/graphics/plot-commands/?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=it#plotpk
----->https://seisman-github-io.translate.goog/SAC_Docs_zh/data-process/picking-phase/?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=it
----->https://moodle.glg.miamioh.edu/brudzimr/tutorials/SAC/
###### vari altri strumenti usati di qui e di là
----->http://eqseis.geosc.psu.edu/cammon/HTML/CJA_Guide/sac_guide.html
----->https://users.earth.ox.ac.uk/~smachine/cgi/index.php
----->https://pdf-cordinate-extractor.vercel.app/
---------------------
###### pagina FAQ per visualizzare i file della topografia della Calabria importati in Paraview
----->https://discourse.paraview.org/t/visualization-of-digital-elevation-model-tiff-file-with-x-y-and-z-axis-in-3d-view/15986/7
---------------------
### SPECFEM3D related
Description of the files:
* stf_iclarc.txt : Source time function of Gaussian type.
* stf_imp.txt : Source time function of impulsive type, single non zero value at 1000th step.
* stf_cosine*.txt : STF of different half duration implemented in later runs in Mw > 5.0 Eqs
* CMTSOLUTION_*: Parameter files defining the Moment Tensor sources necessary to simulate the seismic signal. Default source time function is a smoothed Heaviside. External ones have been defined and implemented.


