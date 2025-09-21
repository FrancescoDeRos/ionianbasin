#!/bin/bash
nodi_non_funzionanti="8 9 10 20 22 23 26 27"
#Raw 8 nodes
combinazioni_8_raw=(
    "1-8"
    "1-7,9"
    "5-6,13-17,19"
    "24-31"
)
combinazioni_4_raw=(
    "22,24-26"
    "27-30"
    "9,13-15"
    "1-4"
    "2-5"
    "28-31"
    "24-27"
    "5-7,9"
    "6,7,9,13"
    "13-16"
    "28-31"
    "13,14,15,19"
)
espandi_range() {
    local riga="$1"
    local nodi=""
    IFS=',' read -ra parts <<< "$riga"
    for part in "${parts[@]}"; do
        if [[ "$part" == *-* ]]; then
            start=$(echo "$part" | cut -d'-' -f1)
            end=$(echo "$part" | cut -d'-' -f2)
            for ((i=start; i<=end; i++)); do
                nodi="$nodi $i"
            done
        else
            nodi="$nodi $part"
        fi
    done
    echo "$nodi" | tr ' ' '\n' | sort -n | uniq | tr '\n' ' '
}
combo_8_tutte=()
for riga in "${combinazioni_8_raw[@]}"; do
    combo_8_tutte+=("$(espandi_range "$riga")")
done
combo_4_tutte=()
for riga in "${combinazioni_4_raw[@]}"; do
    combo_4_tutte+=("$(espandi_range "$riga")")
done
read -p "Enter occupied nodes (e.g., 1,2,3,4): " input_nodi_occupati
nodi_occupati=$(echo "$input_nodi_occupati" | tr ',' ' ' | xargs)
nodi_esclusi=$(echo "$nodi_non_funzionanti $nodi_occupati" | tr ' ' '\n' | sort -n | uniq | tr '\n' ' ')
filtra_combinazioni() {
    local -n combinations_ref=$1 
    local excluded_nodes="$2"
    local filtered_output=""

    for combo_str in "${combinations_ref[@]}"; do
        local is_valid=true
        for node in $combo_str; do
            if echo "$excluded_nodes" | grep -wq "$node"; then
                is_valid=false
                break
            fi
        done
        if "$is_valid"; then
            echo "[$(echo "$combo_str" | tr ' ' ',' | sed 's/,$//')]"
        fi
    done
}
echo "Combinazioni disponibili da 8 nodi:"
filtra_combinazioni combo_8_tutte "$nodi_esclusi"
echo " Combinazioni disponibili da 4 nodi:"
filtra_combinazioni combo_4_tutte "$nodi_esclusi"