import os
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import matplotlib.ticker as ticker
import matplotlib.cm as cm
import matplotlib.colors as mcolors
from matplotlib.cm import get_cmap
import pygmt
import matplotlib_inline
matplotlib_inline.backend_inline.set_matplotlib_formats('retina')
plt.rcParams.update({'font.size': 18})
pathi = "/home/francesco/Immagini/"#-----------------salvatagiioooo
region = [15.4,17.4,37.76,40.25] 
proj = "M4i"
import argparse
def main():
    print('metrics: misfit_norm_amp, misfit_amp, cross_corr')
    parser = argparse.ArgumentParser(description="Run plotting with metric and file path")
    parser.add_argument("-M", "--metric", required=True,
                        help="Metric to use (e.g., misfit_norm_amp, misfit_amp, cross_corr)")
    parser.add_argument("-F", "--filepath", required=True,
                        help="Path to the input file or directory")
    parser.add_argument("-E", "--event", required=True,
                        help="string event to save (YYMMDD-MODEL)")
    parser.add_argument("-C", "--cbar",  default="",required=True,
                        help="colorbar")    
    args = parser.parse_args()
    print(f"Selected metric: {args.metric}")
    print(f"File path: {args.filepath}")
    print(f"Eq: {args.event}")
    print(f"cbar plotted: {args.cbar}")
    df1 = pd.read_csv(args.filepath + "misfit.fin.txt",sep='\s+',header=None,names=[
            "STA1", "CMP1", "STA2", "CMP2","misfit_norm_amp", "misfit_amp", "cross_corr",
            "Lat", "Lon", "LAT", "LON"],usecols=["STA1", "CMP1", "STA2", "CMP2", "cross_corr", "Lat", "Lon"])
    df1.columns = df1.columns.str.strip()
    stations = pd.read_csv("/home/francesco/dati/gmt-6.5.0/ionian/stazioni.txt", sep='\s+', names=["Name", "Net", "Lat", "Lon", "Elev", "Extra"])
    for col in ["cross_corr", "Lat", "Lon"]:
        df1[col] = pd.to_numeric(df1[col], errors="coerce")
    components = {"CXY": "North", "CXX": "East", "CXZ": "Vertical"}
    region = [15.4,17.4,37.76,40.25] 
    proj = "M6i"               
    for comp, comp_name in components.items():
        subset = df1[df1["CMP2"] == comp]   
        if subset.empty:
            print(f"No data for {comp}")
            continue
        fig = pygmt.Figure()
        pygmt.config(FONT_TITLE="18p,Helvetica-Bold", MAP_FRAME_TYPE="plain", MAP_TITLE_OFFSET="-0.1", FORMAT_GEO_MAP="ddd.x")
        fig.basemap(region=region, projection=proj,frame=["af", f"+t{comp_name} component"])
        fig.coast(shorelines="1/0.5p,black", borders="1/0.5p,black", land="lightgray")
        if args.metric == "cross_corr":
            pygmt.makecpt(cmap="vik", series=[0, 1], output=pathi+"metric_crosscorr.cpt")
            fig.colorbar(cmap=pathi+"metric_crosscorr.cpt", frame=f"+l{args.cbar}", position="JBC+o8.0c/-24c+w23.7c/0.4c+v",box="+gwhite+p0.2p")
            print("Range of cross_corr:", subset[args.metric].min(), subset[args.metric].max())
            fig.plot(x=subset["Lon"],y=subset["Lat"],style="c0.5c",fill=subset[args.metric],cmap=pathi+"metric_crosscorr.cpt",pen="0.8p,black")
        else: 
            print("NO!")
        fig.text(x=subset["Lon"],y=subset["Lat"],text=subset["STA1"],font="9p,Helvetica,black",offset="0.2c/0.2c",justify="LM")
        outfile = f"{pathi}{args.cbar}_{args.event}_{comp_name}.pdf"
        fig.savefig(outfile, dpi=300)
        print(f"Saved {outfile}")    
if __name__ == "__main__":
    main()
