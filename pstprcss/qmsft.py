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
depth_grid = "/home/francesco/dati/gmt-6.5.0/ionian/depth_pos.grd"
thickness_grid = "/home/francesco/dati/gmt-6.5.0/ionian/cal_slab2_thk_02.24.18.grd"
region = [13.3, 19.8, 35.1, 40.2]
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
    df1 = pd.read_csv(args.filepath + "misfit.fin.txt",sep=r'\s+',header=None,names=[
            "STA1", "CMP1", "STA2", "CMP2","misfit_norm_amp", "misfit_amp", "cross_corr",
            "Lat", "Lon"],engine="python")
    stations = pd.read_csv("/home/francesco/dati/gmt-6.5.0/ionian/stazioni.txt", sep='\s+', names=["Name", "Net", "Lat", "Lon", "Elev", "Extra"])
    for col in ["misfit_norm_amp", "misfit_amp", "cross_corr", "Lat", "Lon"]:
        df1[col] = pd.to_numeric(df1[col], errors="coerce")
    components = {"CXY": "North", "CXX": "East", "CXZ": "Vertical"}
    for comp, comp_name in components.items():
        subset = df1[df1["CMP1"] == comp]
        if subset.empty:
            print(f"No data for {comp}")
            continue
        fig = pygmt.Figure()
        vals = subset[args.metric]
        print("metrica:", subset[args.metric])
        vmin, vmax = vals.min(), vals.max()
        vals_norm = (vals - vmin) / (vmax - vmin)
        pygmt.config(FONT_TITLE="18p,Helvetica-Bold", MAP_FRAME_TYPE="plain", MAP_TITLE_OFFSET="0.2")
        fig.basemap(region = [15.4, 17.4, 37.8, 40.2],projection="M4i",frame=["af", f"+t{comp_name} Component"])
        fig.coast(region=[15.4, 17.4, 37.8, 40.2], shorelines="1/0.5p,black", borders="1/0.5p,black", resolution="i")
        fig.coast(land="lightgray", water="white")
        pygmt.makecpt(cmap="vik", series=[ 0, 1])
        fig.plot(x=subset["Lon"],y=subset["Lat"],style="c0.2c",fill=vals_norm,cmap=True,pen="black")
        print("Range of misfit_norm_amp:", subset["misfit_norm_amp"].min(), subset["misfit_norm_amp"].max())
        print("Range of misfit_norm_amp:", subset[args.metric].min(), subset[args.metric].max())
        print("vals_min, vals_max:", vals_norm.min(), vals_norm.max())
        fig.text(x=subset["Lon"],y=subset["Lat"],text=subset["STA1"],font="6.2p,Helvetica,black", offset="0.1c/0.1c",justify="LM")
        fig.colorbar(cmap=True, frame=f"+l{args.cbar}", position="jBC+o5.3c/2.5c+w12c/0.3c+v") 
        fig.savefig(f"{pathi}{args.cbar}_{args.event}_{comp_name}.pdf", dpi=300)
        print(f"Saved {pathi}{args.cbar}_{args.event}_{comp_name}.pdf")
if __name__ == "__main__":
    main()
