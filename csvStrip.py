import os
import csv

src_path = './IMU_data/bicep curl/2/'
dest_path = './IMU_data/bicep curl/2/Stripped/'

def csvStrip(src_path, dest_path):
    for item in os.listdir(src_path):
        item_path = os.path.join(src_path, item)
        print(item_path)
        if (os.path.isfile(item_path)) and item_path.endswith('.csv'):
            print("READING: " + item_path + " ............")
            with open(item_path, 'r') as file:
                reader = csv.reader(file)
                # create a new file with the same name as the old file name
                new_file_path = os.path.join(dest_path, item)
                os.makedirs(os.path.dirname(new_file_path), exist_ok=True)
                with open(new_file_path, 'w') as new_file:
                    writer = csv.writer(new_file)
                    # loop through filtering out the last 4 columns and write to the new file
                    for row in reader:
                        writer.writerow(row[:-4])
                new_file.close()
            file.close()
    print("Done")

def main():
    #check if the destination path exists, if not create it
    if not os.path.exists(dest_path):
        os.makedirs(dest_path)
    csvStrip(src_path, dest_path)

if __name__ == "__main__":
    main()


            