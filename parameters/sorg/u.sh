#!/bin/bash
export LC_NUMERIC=C

read -p "Inserisci latitudine: " lat
read -p "Inserisci longitudine: " lon

echo "$lon $lat" | proj +proj=utm +zone=34 +ellps=WGS84 +units=m +datum=WGS84 -f "%.1f"
