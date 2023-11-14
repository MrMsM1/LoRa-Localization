#!/bin/bash

input_file="../data4.txt"
output_file="output4.json"

# Check if the input file exists
if [ ! -f "$input_file" ]; then
    echo "Input file not found!"
    exit 1
fi

# Process each line in the input file
echo "[" > "$output_file"  # Start of JSON array
while IFS= read -r line; do
    # Extract Comment and RSSI values using awk
    comment_x=$(echo "$line" | awk -F 'Comment: ' '{print $2}' | awk -F ',' '{print $1}')
    comment_y=$(echo "$line" | awk -F 'Comment: ' '{print $2}' | awk -F ',' '{print $2}')
    rssi=$(echo "$line" | awk -F 'RSSI: ' '{print $2}' | awk -F ',' '{print $1}')

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
echo "Extraction complete. Results saved to $output_file"
