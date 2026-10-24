filenames=("12" "12_2" "12_3" "1000" "10000" "100000" "P180000_rand" "P180000" "P180000_2" "P180000_3" "P180000_4" "P180000_5")

for file in "${filenames[@]}"; do
    ./bin/mps inputs/$file.in outputs/$file.out
done
