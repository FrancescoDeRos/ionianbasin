#!/bin/bash
# -------------------------
#INPUT
STATION_FILE="STATIONSLL.dat"
CMTSOLUTION_FILE="CMTSOLUTION_20250416"
OUTPUT_FILE="distances_20250416.dat"
EARTH_RADIUS=6371.0
# -------------------------
# get EQ Coordinates from CMTSOLUTION
SOURCE_LAT=$(grep 'latitude:' "$CMTSOLUTION_FILE" | awk '{print $NF}')
SOURCE_LON=$(grep 'longitude:' "$CMTSOLUTION_FILE" | awk '{print $NF}')
SOURCE_DEPTH_KM=$(grep 'depth:' "$CMTSOLUTION_FILE" | awk '{print $NF}')
if [[ -z $SOURCE_LAT || -z $SOURCE_LON || -z $SOURCE_DEPTH_KM ]]; then
  echo "no source coordinates extracted from input CMTSOLUTION."
  exit 1
fi
echo "# source: lat=$SOURCE_LAT lon=$SOURCE_LON depth_km=$SOURCE_DEPTH_KM"
echo "# station distance_km azimuth_deg backazimuth_deg" > "$OUTPUT_FILE"
#loop all stations
awk -v elrad=$EARTH_RADIUS \
    -v srclat="$SOURCE_LAT" -v srclon="$SOURCE_LON" -v srcdepth="$SOURCE_DEPTH_KM" \
    '
    function deg2rad(d) { return d * atan2(1,1) / 45 }
    function rad2deg(r) { return r * 45 / atan2(1,1) }
    BEGIN {
      pi = atan2(0, -1)
    }

    {
      station = $1
      network = $2
      stalat = $3
      stalon = $4
      elevation_m = $5
      burial_m = $6
      #convert to radians
      lat1 = deg2rad(srclat)
      lon1 = deg2rad(srclon)
      lat2 = deg2rad(stalat)
      lon2 = deg2rad(stalon)
      dlat = lat2 - lat1
      dlon = lon2 - lon1
      #haversine form
      a = sin(dlat/2)^2 + cos(lat1)*cos(lat2)*sin(dlon/2)^2
      c = 2 * atan2(sqrt(a), sqrt(1 - a))
      surface_dist_km = elrad * c
      #elevation correction
      station_elev_km = elevation_m / 1000.0
      vertical_diff_km = srcdepth - station_elev_km
      total_dist_km = sqrt(surface_dist_km^2 + vertical_diff_km^2)
      #azimuth (source -> station)
      y = sin(dlon) * cos(lat2)
      x = cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(dlon)
      azimuth_rad = atan2(y, x)
      azimuth_deg = rad2deg(azimuth_rad)
      if (azimuth_deg < 0) azimuth_deg += 360
      #back-azimuth (station -> source)
      yb = sin(-dlon) * cos(lat1)
      xb = cos(lat2)*sin(lat1) - sin(lat2)*cos(lat1)*cos(-dlon)
      backaz_rad = atan2(yb, xb)
      backaz_deg = rad2deg(backaz_rad)
      if (backaz_deg < 0) backaz_deg += 360
      printf("%s %.3f %.2f %.2f\n", station, total_dist_km, azimuth_deg, backaz_deg)
    }
    ' "$STATION_FILE" >> "$OUTPUT_FILE"
echo "go output to $OUTPUT_FILE"
