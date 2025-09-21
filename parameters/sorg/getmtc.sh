#!/bin/bash
#read long as x coord and lat as y coord
input="$1"
output="CMTSOLUTION"
if [[ ! -f "$input" ]]; then
  echo "usage: $0 <input_file>"
  exit 1
fi
> "$output"

awk -F"|" -v out="$output" '
NR==1 {
  for (i=1; i<=NF; i++) {
    gsub(/^#/, "", $i)
    h[$i] = i
  }
  next
}
{
  ev_id = $h["ev_unid"]
  ev_time = $h["ev_event_time"]
  region = $h["mt_region"]
  lat = $h["mt_latitude"]
  lon = $h["mt_longitude"]
  depth = $h["mt_depth"]
  mrr = $h["mt_mrr"]
  mtt = $h["mt_mtt"]
  mpp = $h["mt_mpp"]
  mrt = $h["mt_mrt"]
  mrp = $h["mt_mrp"]
  mtp = $h["mt_mtp"]
  magtype = $h["ev_mag_type"]
  magval  = $h["ev_mag_value"]
  gsub(/ UTC/, "", ev_time)
  split(ev_time, dt, /[- :]/)
  year = dt[1]; mon = dt[2]; day = dt[3]; hour = dt[4]; min = dt[5]; sec = dt[6]"."dt[7]
  printf("PDE %s %s %s %s %s %s  %8.4f  %8.4f  %4.1f %4.1f %4.1f %s\n", year, mon, day, hour, min, sec, lat, lon, depth, magtype, magval, region) >> out
  printf("event name:     %s\n", ev_id)             >> out
  print  "time shift:         0.0000"               >> out
  print  "half duration:      0.0000"               >> out
  printf("latitude:        %12.4f\n", lat)          >> out
  printf("longitude:       %12.4f\n", lon)          >> out
  printf("depth:           %12.4f\n", depth)        >> out
  printf("Mrr:           %14.6E\n", mrr)            >> out
  printf("Mtt:           %14.6E\n", mtt)            >> out
  printf("Mpp:           %14.6E\n", mpp)            >> out
  printf("Mrt:           %14.6E\n", mrt)            >> out
  printf("Mrp:           %14.6E\n", mrp)            >> out
  printf("Mtp:           %14.6E\n", mtp)            >> out
  print  ""                                         >> out
}
' "$input"
