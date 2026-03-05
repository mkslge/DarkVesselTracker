import csv
import sys

input_file = sys.argv[1]
output_file = sys.argv[2]

with open(input_file, newline='') as infile, open(output_file, 'w', newline='') as outfile:
    reader = csv.reader(infile)
    writer = csv.writer(outfile)

    for row in reader:
        if len(row) < 9:
            continue  # skip rows that don't have enough columns
        new_row = row[:7] + row[-2:]
        writer.writerow(new_row)