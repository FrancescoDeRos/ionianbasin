#!/bin/bash
#round next integer (ceil) ~ awk
ceil() {
  awk -v n="$1" 'BEGIN {print (n == int(n)) ? n : int(n)+1}'
}
#calc float w/ awk
calc() {
  awk "BEGIN {print $*}"
}
echo "Francesco De Rose -- UniMunster Geophysik 03062025 -- : )"
echo "the scope of this program is to find the suitable choice for nx, ny, nz and nlower_nodes in the notebook, any suggestion is appreciated."
echo "Usage: $0 f_max vs_min thickness_upperlayer_split xmin xmax ymin ymax zmin zmax_topog"
  echo "Ex.: $0 1.0 2370 20000 500000 1129518 3984725 4449825 -80000 2726"
# --- check input modello v rho ---
if [ "$#" -ne 9 ]; then
  echo "Ex.: $0 1.0(Hz) 2370(m/s) 20000(m) 500000(m) 1129518(m) 3984725(m) 4449825(m) -80000(m) 2726(m)(highest topography point)"
  exit 1
fi
# --- input ---
F_MAX="$1"
VS_MIN="$2"
SPLIT_DEPTH_THICKNESS="$3" #upper layer refined
XMIN="$4"
XMAX="$5"
YMIN="$6"
YMAX="$7"
ZMIN_MODEL_BOTTOM="$8"
ZMAX_SURFACE_TOPO="${9}"   #max topography
POINTS_PER_WAVELENGTH=5 # Komatitsch2005 - SPECFEM3D con NGLL=5
echo "==============input=================="
echo "=f_max Target: ${F_MAX} Hz"
echo "=Vs minima (Vs_min): ${VS_MIN} m/s"
echo "=refined upper layer (split_depth_thickness): ${SPLIT_DEPTH_THICKNESS} m"
echo "=length X(longitude): from ${XMIN} m to ${XMAX} m"
echo "=length Y(latitude): from ${YMIN} m to ${YMAX} m"
echo "=bottom mesh (zmin_model_bottom): ${ZMIN_MODEL_BOTTOM} m"
echo "=Topograf Z (zmax_surface_topo): ${ZMAX_SURFACE_TOPO} m (max elevation)"
echo "=Points per minimum wavelength: ${POINTS_PER_WAVELENGTH}"
echo "==================================="
# --- mesh length and depth of layers ---
LX=$(calc "$XMAX - $XMIN")
LY=$(calc "$YMAX - $YMIN")
#upper layer thickness (input inside notebook)
H_UPPER=${SPLIT_DEPTH_THICKNESS}
# z coordin split of interface betwnn two layers
# (w.r.t z=0, zmax_surface_topo highest topography point)
Z_SPLIT_INTERFACE=$(calc "$ZMAX_SURFACE_TOPO - $H_UPPER")
#lower layer thicknes
H_LOWER=$(calc "$Z_SPLIT_INTERFACE - $ZMIN_MODEL_BOTTOM")
if (( $(echo "$H_LOWER <= 0" | bc -l) )); then
  echo "[ATTENZIONE] thickness of lower layer (H_lower = ${H_LOWER}m) is NULL or negative. Check your input."
  echo "split_depth_thickness must be lower than the total depth w.r.t. the highest topog. value."
  echo "check that Z_SPLIT_INTERFACE (${Z_SPLIT_INTERFACE}m) is above ZMIN_MODEL_BOTTOM (${ZMIN_MODEL_BOTTOM}m)."
  if (( $(echo "$H_LOWER < 0" | bc -l) )); then H_LOWER=0; fi # H_LOWER sets 0 if it's negative for the nodes
fi
if (( $(echo "$H_UPPER < 0" | bc -l) )); then 
    echo "[ERROR] thickness upper layer (H_upper = ${H_UPPER}m) is negative. Use a reasonable value suitable for depth in input."
    exit 1
fi
echo "===============mesh model dimensions==============="
echo "==Lx: ${LX} m"
echo "==Ly: ${LY} m"
echo "==H_upper (thickness of upper layer): ${H_UPPER} m"
echo "==H_lower (thickness of lower layer): ${H_LOWER} m"
echo "==================================================="
# --- spacing GLL points required stability-dispersion ---
DENOM_HGLL=$(calc "${F_MAX} * ${POINTS_PER_WAVELENGTH}")
if (( $(echo "$DENOM_HGLL == 0" | bc -l) )); then
  echo "[ERROR] D(x) for h_GLL is NULL (i.e. F_MAX or POINTS_PER_WAVELENGTH is NULL, check your input)."
  exit 1
fi
H_GLL_REQ=$(calc "${VS_MIN} / ${DENOM_HGLL}")
if (( $(echo "$H_GLL_REQ <= 0" | bc -l) )); then
  echo "[ERROR] h_GLL_req is NULL or negative. Check F_MAX or VS_MIN."
  exit 1
fi
echo "===============refinement required==============="
echo "==h_GLL required (h_GLL_req): ${H_GLL_REQ} m (for f_max = ${F_MAX} Hz)"
echo "================================================="
# --- nodes nx ny nz suggested for stability---
#nx
NX_FLOAT=$(calc "(${LX} / ${H_GLL_REQ}) + 1")
NX_REC=$(ceil "$NX_FLOAT")
#2 nodes minimum if the dimension is > 0.
[ "$NX_REC" -lt 2 ] && [ $(echo "$LX > 0" | bc -l) -eq 1 ] && NX_REC=2
#ny
NY_FLOAT=$(calc "(${LY} / ${H_GLL_REQ}) + 1")
NY_REC=$(ceil "$NY_FLOAT")
[ "$NY_REC" -lt 2 ] && [ $(echo "$LY > 0" | bc -l) -eq 1 ] && NY_REC=2
#nlower_nodes i.e lower layer thickness parameter
#'nlower_nodes' in python notebook
if (( $(echo "$H_LOWER > 0" | bc -l) )); then
  NLOWER_NODES_FLOAT=$(calc "(${H_LOWER} / ${H_GLL_REQ}) + 1")
  NLOWER_NODES_REC=$(ceil "$NLOWER_NODES_FLOAT")
  [ "$NLOWER_NODES_REC" -lt 2 ] && NLOWER_NODES_REC=2
else
  NLOWER_NODES_REC=1 # IF H_LOWER == 0, 1 node is that limit.
fi
#nodes H_upper i.e. 'nz - nlower_nodes' 
if (( $(echo "$H_UPPER > 0" | bc -l) )); then
  NUPPER_SEGMENT_NODES_FLOAT=$(calc "(${H_UPPER} / ${H_GLL_REQ}) + 1")
  NUPPER_SEGMENT_NODES_REC=$(ceil "$NUPPER_SEGMENT_NODES_FLOAT")
  [ "$NUPPER_SEGMENT_NODES_REC" -lt 2 ] && NUPPER_SEGMENT_NODES_REC=2
else
  NUPPER_SEGMENT_NODES_REC=1 #IF H_LOWER == 0, 1 node(surface) is that limit.
fi
#final nz suggested e.g. nlower_nodes + (upper layer nodes)
NZ_REC=$(calc "${NLOWER_NODES_REC} + ${NUPPER_SEGMENT_NODES_REC}")
echo "====================reccomended nodes for stability============================="
echo "==nx reccomended: ${NX_REC}"
echo "==ny reccomended: ${NY_REC}"
echo "==nlower_nodes reccomended: ${NLOWER_NODES_REC}"
echo "==upper layer nodes reccomended ('nz - nlower_nodes'): ${NUPPER_SEGMENT_NODES_REC}"
echo "==nz reccomended: ${NZ_REC}"
echo "================================================================================"
# --- spacings in each direction ---
echo "===============spacings in each direction==============="
#Dx
if [ "$NX_REC" -gt 1 ] && [ $(echo "$LX > 0" | bc -l) -eq 1 ]; then
  DX_ACTUAL=$(calc "${LX} / (${NX_REC} - 1)")
  echo "Dx: ${DX_ACTUAL} m"
else
  echo "Dx: N/A (nx = ${NX_REC}, Lx = ${LX}m)"
fi
#Dy
if [ "$NY_REC" -gt 1 ] && [ $(echo "$LY > 0" | bc -l) -eq 1 ]; then
  DY_ACTUAL=$(calc "${LY} / (${NY_REC} - 1)")
  echo "Dy: ${DY_ACTUAL} m"
else
  echo "Dy: N/A (ny = ${NY_REC}, Ly = ${LY}m)"
fi
#Dz_lower
if [ "$NLOWER_NODES_REC" -gt 1 ] && [ $(echo "$H_LOWER > 0" | bc -l) -eq 1 ]; then
  DZ_LOWER_ACTUAL=$(calc "${H_LOWER} / (${NLOWER_NODES_REC} - 1)")
  echo "Dz_lower (per H_lower=${H_LOWER}m): ${DZ_LOWER_ACTUAL} m"
else
  echo "Dz_lower: N/A (nlower_nodes_rec = ${NLOWER_NODES_REC}, H_lower = ${H_LOWER}m)"
fi
#Dz_upper
if [ "$NUPPER_SEGMENT_NODES_REC" -gt 1 ] && [ $(echo "$H_UPPER > 0" | bc -l) -eq 1 ]; then
  DZ_UPPER_ACTUAL=$(calc "${H_UPPER} / (${NUPPER_SEGMENT_NODES_REC} - 1)")
  echo "Dz_upper (per H_upper=${H_UPPER}m): ${DZ_UPPER_ACTUAL} m"
else
  echo "Dz_upper: N/A (nupper_segment_nodes_rec = ${NUPPER_SEGMENT_NODES_REC}, H_upper = ${H_UPPER}m)"
fi
echo "nodi.mesh end. Thanks : )"
