#!/bin/bash

input_file="output1.json"
output_file="output_final11.json"

# Check if the input file exists
if [ ! -f "$input_file" ]; then
    echo "Input file not found!"
    exit 1
fi

# Process each line in the input file
echo "[" > "$output_file"  # Start of JSON array
while IFS= read -r line; do
    # Extract x, y, and rssi values from the modified JSON
    comment_x=$(echo "$line" | awk -F '\"x\": ' '{print $2}' | awk -F ',' '{print $1}')
    comment_y=$(echo "$line" | awk -F '\"y\": ' '{print $2}' | awk -F ',' '{print $1}')
    rssi=$(echo "$line" | awk -F '\"rssi\": ' '{print $2}')

    # Increment x and y if they are even
    if (( comment_x % 2 == 0 )); then
        ((comment_x++))
    fi

    if (( comment_y % 2 == 0 )); then
        ((comment_y++))
    fi

    # Output in JSON format
    echo "  {" >> "$output_file"
    echo "    \"x\": $comment_x," >> "$output_file"
    echo "    \"y\": $comment_y," >> "$output_file"
    echo "    \"rssi\": $rssi" >> "$output_file"
    echo "  }," >> "$output_file"

done < "$input_file"

# Remove the trailing comma from the last entry in the JSON array
truncate -s-2 "$output_file"

echo "]" >> "$output_file"  # End of JSON array
echo "Final modification complete. Results saved to $output_file"

