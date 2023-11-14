import json

# Read the JSON file
with open('output4.json', 'r') as file:
    data = json.load(file)

# Iterate through each object in the JSON data
for entry in data:
    # Check if both "x" and "y" are even
    if entry['x'] % 2 == 0:
        entry['x'] += 1
    if entry['y'] % 2 == 0:
        entry['y'] += 1

# Write the modified data back to the JSON file
with open('output_final4.json', 'w') as file:
    json.dump(data, file, indent=2)

# Print the modified data (optional)
print(json.dumps(data, indent=2))
