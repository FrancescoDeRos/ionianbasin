import pandas as pd
#faseP
file1 = "/home/common/g.200.3d.24.imp/comuni/ondaS/misfit.fin.txt"
file2 = "/home/common/g.200.3st.24.imp/comuni/ondaS/misfit.fin.txt"
out = "differences.txt"
cols = ["STA1", "CMP1", "STA2", "CMP2", 
        "misfit_norm_amp", "misfit_amp", "cross_corr",
        "Lat", "Lon", "LAT", "LON"]
df1 = pd.read_csv(file1, sep="\s+", names=cols, engine="python")
df2 = pd.read_csv(file2, sep="\s+", names=cols, engine="python")
merged = pd.merge(
    df1, df2, 
    on=["STA1", "CMP1", "STA2", "CMP2", "Lat", "Lon"], 
    suffixes=("_f1", "_f2")
)
merged["deltachi"] = merged["misfit_norm_amp_f1"] - merged["misfit_norm_amp_f2"]
merged["deltaCC"] = merged["cross_corr_f1"] - merged["cross_corr_f2"]
outcols = ["STA1", "CMP1", "STA2", "CMP2", "deltachi", "deltaCC", "Lat", "Lon"]
result = merged[outcols]
result.to_csv(out, sep=" ", index=False, float_format="%.6f")

print(f"Saved differences to {out}")
import os
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import matplotlib_inline
import pygmt
import argparse
matplotlib_inline.backend_inline.set_matplotlib_formats('retina')
plt.rcParams.update({'font.size': 18})
pathi = "/home/francesco/Immagini/"
region = [15.4, 17.4, 37.76, 40.25]
proj = "M6i"
def main():
    print('metrics: deltachi, deltaCC')
    parser = argparse.ArgumentParser(description="Run plotting with metric and file path")
    parser.add_argument("-M", "--metric", required=True,
                        help="Metric to use (deltachi or deltaCC)")
    parser.add_argument("-F", "--filepath", required=True,
                        help="Path to the input file (differences.txt)")
    parser.add_argument("-E", "--event", required=True,
                        help="string event to save (YYMMDD-MODEL)")
    parser.add_argument("-C", "--cbar", default="", required=True,
                        help="colorbar label")    
    args = parser.parse_args()
    print(f"Selected metric: {args.metric}")
    print(f"File path: {args.filepath}")
    print(f"Eq: {args.event}")
    print(f"cbar plotted: {args.cbar}")
    df = pd.read_csv(args.filepath, sep='\s+', header=0,
        names=["STA1", "CMP1", "STA2", "CMP2", "deltachi", "deltaCC", "Lat", "Lon"], usecols=["STA1", "CMP1", "STA2", "CMP2", "deltachi", "Lat", "Lon"])
    df.columns = df.columns.str.strip()
    for col in ["deltachi", "Lat", "Lon"]:
        df[col] = pd.to_numeric(df[col], errors="coerce")
    components = {"CXY": "North", "CXX": "East", "CXZ": "Vertical"}
    for comp, comp_name in components.items():
        subset = df[df["CMP2"] == comp]
        if subset.empty:
            print(f"No data for {comp}")
            continue
        fig = pygmt.Figure()
        print("Range of metric:", subset[args.metric].min(), subset[args.metric].max())
        if args.metric == "deltachi":
            pygmt.config(FONT_TITLE="18p,Helvetica-Bold", MAP_FRAME_TYPE="plain", 
                         MAP_TITLE_OFFSET="-0.1", FORMAT_GEO_MAP="ddd.x")
            fig.basemap(region=region, projection=proj,
                        frame=["af", f"+t{comp_name} component"])
            fig.coast(shorelines="1/0.5p,black", borders="1/0.5p,black", land="lightgray")
            pygmt.makecpt(cmap="bam", series=[subset[args.metric].min(), subset[args.metric].max()], output=pathi+"metric_diff.cpt")
            fig.plot(x=subset["Lon"], y=subset["Lat"], style="c0.5c",
                     fill=subset[args.metric], cmap=pathi+"metric_diff.cpt", pen="0.8p,black")
            fig.colorbar(cmap=pathi+"metric_diff.cpt",
                         frame=f"+l{args.cbar}",
                         position="JBC+o8.0c/-24c+w23.7c/0.4c+v", box="+gwhite+p0.2p")
            fig.text(x=subset["Lon"], y=subset["Lat"], text=subset["STA1"],
                     font="9p,Helvetica,black", offset="0.2c/0.2c", justify="LM")
            outfile = f"{pathi}{args.cbar}_{args.event}_{comp_name}.pdf"
            fig.savefig(outfile, dpi=300)
            print(f"Saved {outfile}")  
        else:
            print("No!")
if __name__ == "__main__":
    main()
