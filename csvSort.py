import os
import csv

# path to the folder containing the csv files
src1_path = './IMU_data/bicep curl/'
src2_path = './IMU_data/squat/'

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

                #check to see if this path doesn't exist and create it
                if not os.path.exists(put_path):
                    os.makedirs(put_path)
                
                #check if the file's name has MOV in it
                if "MOV" in item:
                    put_path = os.path.join(put_path, "MOVING")
                    if not os.path.exists(put_path):
                        os.makedirs(put_path)
                elif "FIND" in item:
                    put_path = os.path.join(put_path, "LOST")
                    if not os.path.exists(put_path):
                        os.makedirs(put_path)
                #normal case, categorize based on grade
                else:
                    put_path = os.path.join(put_path, header[10])
                    if not os.path.exists(put_path):
                        os.makedirs(put_path)

                #move the file to the new directory
                os.rename(item_path, os.path.join(put_path, item))
                #move the rep_count file cursor to the next line
    print("Done")
    return

def main():
    csvSort(src1_path)
    csvSort(src2_path)

if __name__ == "__main__":
    main()