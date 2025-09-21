#!/bin/bash
input="STATIONS"
output="STATIONS_UTM"
# EPSG:32633 for UTM zone 33N (Ionian region)
utm_zone="+proj=utm +zone=33 +datum=WGS84 +units=m +no_defs"
echo "# Converted from lat/lon to UTM (Zone 33N)" > "$output"
while read -r line; do
    [[ "$line" =~ ^#.*$ || -z "$line" ]] && echo "$line" >> "$output" && continue
    #reading
    col1=$(echo "$line" | awk '{print $1}')
    col2=$(echo "$line" | awk '{print $2}')
    lat=$(echo "$line" | awk '{print $3}')
    lon=$(echo "$line" | awk '{print $4}')
    rest=$(echo "$line" | cut -d' ' -f5-)
    if [[ "$lat" =~ ^-?[0-9]+(\.[0-9]+)?$ && "$lon" =~ ^-?[0-9]+(\.[0-9]+)?$ ]]; then
        # proj
        utm=$(echo "$lon $lat" | proj $utm_zone)
        utm_east=$(echo $utm | awk '{print $1}')
        utm_north=$(echo $utm | awk '{print $2}')
        echo "$col1 $col2 $utm_east $utm_north $rest" >> "$output"
    else
        echo "$line" >> "$output"
    fi
done < "$input"
echo "done, output written to '$output'."
