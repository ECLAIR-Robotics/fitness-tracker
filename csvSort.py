import os
import csv

# path to the folder containing the csv files
src_path = './IMU_data'

def csvSort(src_path):

    # go through directory and find csv files
    for item in os.listdir(src_path):
        item_path = os.path.join(src_path, item)
        #default out
        put_path = src_path
        print(item_path)
        #check if my item is a file and ends with .csv
        if os.path.isfile(item_path) and item_path.endswith('.csv'):
            #file found, open and read the first line
            with open(item_path, 'r') as file:
                reader = csv.reader(file)
                header = next(reader)
                #check 12th element of the header, and os path join src_path and the last element
                put_path = os.path.join(src_path, header[11])

                #check to see if this path doesn't exist and create it
                if not os.path.exists(put_path):
                    os.makedirs(put_path)
                #now check the 10th element and os path join put_path and the exercise name
                
                grade = header[10]
                
                is_failure = False
                #read through the whole file to make sure that the grade is consistent
                for row in reader:
                    #check to see if grade goes up
                    try:
                        if row[10] > grade:
                            is_failure = True
                            break
                    except IndexError:
                        continue

                if is_failure:
                    print("Failure")
                    put_path = os.path.join(put_path, "to_failure")
                    if not os.path.exists(put_path):
                        os.makedirs(put_path)
                else: # if the grade is consistent, then move the file to a directory based on grade
                    put_path = os.path.join(put_path, header[10])
                
                if not os.path.exists(put_path):
                    os.makedirs(put_path)
                #close the file 
                                
                #move the file to the new directory
                os.rename(item_path, os.path.join(put_path, item))
                #move the rep_count file cursor to the next line
    print("Done")
    return

def main():
    csvSort(src_path)

if __name__ == "__main__":
    main()