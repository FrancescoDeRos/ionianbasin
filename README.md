# ionianbasin
This repository collects all material related to my master's thesis work for the Ionian basin region. If you have problems executing the code and reproducing the outputs check inside the folder specs/ about the flags/options used on the machines used during the the simulation runs. If you still have issues after this tinkering step do not hesitate to contact me and help solving any problem. The goal is always to start from the end point at which I stopped.

This main branch includes:
        
    Thesis (.tex, .pdf and all other outputs from Texmaker as well as figures .png & jpegs) (work in progress...)

    Thesis presentation (.odp with TexMaths extension, be sure to have that extension installed otherwise some figures won't properly render; work in progress...)   

    Description of all PCs used throughout the thesis work (configuration flags selected during the software setup, CPU, RAM usage, time for each run etc.):   #check the folder specs/#

    Complete simulation setup parameter files related to SPECFEM3D Cartesian (Par_file, CMTSOLUTION, STATIONS, STFext etc.); #check the folder parameters/#

    Meshing and model-building utilities (many thanks to Dr. Rafael Abreu for the help in the mesh modelling); #check the folder meshing/# 

    3D and tomographic velocity model scripts (mostly GMT related codes); #check the folder models/#  

    Post-processing and plotting scripts (mostly JupyterNotebook, gnuplot and python); #check the folder pstprcss/ and for the output check fgs/#

    Documentation and notes on simulation parameters and workflow (all the articles that have been read during the thesis writing). #check the folder refs/# 

The aim is to maintain a reproducible and transparent archive of the numerical tests conducted for regional waveform modeling studies in the area that has been studied. I hope whichever user interested in these kind of simulations will find this repository useful for its studies.
