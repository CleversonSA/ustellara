import csv

with open('power_sample.csv') as file:
    reader = csv.reader(file, delimiter = ',')

    count = 0
    for row in reader:
        col_count = 0
        db_item_count = 0
        db_avg = 0
        db_sum = 0.0

        for col in row:
           if col_count > 6 and col != 'nan':
             db_item_count +=1
             db_sum += float(col.strip())
           col_count += 1

        db_avg = db_sum / db_item_count
        print(row[

        if count > 2:
           break

        count += 1

