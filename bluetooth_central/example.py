import simplepyble
from struct import pack, unpack


def to_string(arr):
    # Convert an array of ints to a csv string representation
    return ','.join([str(i) for i in arr])

def connect(file_idx):
    adapters = simplepyble.Adapter.get_adapters()

    if len(adapters) == 0:
        print("No adapters found")

    # Query the user to pick an adapter
    print("Please select an adapter:")
    if len(adapters) == 1:
        choice = 0;
    else: 
        for i, adapter in enumerate(adapters):
            print(f"{i}: {adapter.identifier()} [{adapter.address()}]")
        choice = int(input("Enter choice: "))
    adapter = adapters[choice]

    print(f"Selected adapter: {adapter.identifier()} [{adapter.address()}]")

    adapter.set_callback_on_scan_start(lambda: print("Scan started."))
    adapter.set_callback_on_scan_stop(lambda: print("Scan complete."))
    # adapter.set_callback_on_scan_found(lambda peripheral: print(f"Found {peripheral.identifier()} [{peripheral.address()}]"))

    # Scan for 5 seconds
    adapter.scan_for(5000)
    peripherals = adapter.scan_get_results()

    # Query the user to pick a peripheral
    peripheral_short = False
    print("Please select a peripheral:")
    for i, peripheral in enumerate(peripherals):
        if (peripheral.identifier() == "Arduino" and peripheral.address() == "D17D793C-1E73-27E5-AAD1-80BA67CD1AA5"):
            choice = i
            peripheral_short = True
            break
        print(f"{i}: {peripheral.identifier()} [{peripheral.address()}]")
    
    if (not peripheral_short):
        choice = int(input("Enter choice: "))
    else :
        print("Selecting Arduino automatically...")
    peripheral = peripherals[choice]


    print(f"Connecting to: {peripheral.identifier()} [{peripheral.address()}]")
    input("Press any key to continue: ")
    peripheral.connect()

    print("Successfully connected, listing services...")
    services = peripheral.services()
    service_characteristic_pair = []
    for service in services:
        for characteristic in service.characteristics():
            service_characteristic_pair.append((service.uuid(), characteristic.uuid()))

    # Query the user to pick a service/characteristic pair
    if len(service_characteristic_pair) == 1:
        choice = 0
    else: 
        service_uuid = None
        imu_characteristic_uuid = None
        control_characteristic_uuid = None
        time_characteristic_uuid = None
        service_characteristic_ids = ["0000180a", 
                                      "00002a57-0000-1000-8000-00805f9b34fb", 
                                      "00008ad3-0000-1000-8000-00805f9b34fb", 
                                      "0000bc76-0000-1000-8000-00805f9b34fb"]
        print("Please select a service/characteristic pair:")
        # for i, (service_uuid, characteristic) in enumerate(service_characteristic_pair):
        service_uuid = service_characteristic_pair[0][0]
        if service_uuid.startswith(service_characteristic_ids[0]):
            print("Desired Service is present")
            imu_characteristic_uuid = service_characteristic_ids[1]
            control_characteristic_uuid = service_characteristic_ids[2]
            time_characteristic_uuid = service_characteristic_ids[3]
            # choice = i
        else :
            print("Desired Service not present")
            return
            # print(f"{i}: {service_uuid} {characteristic}")
    #     choice = int(input("Enter choice: "))
    # service_uuid, characteristic_uuid = service_characteristic_pair[choice]

    # Read the content from the characteristics
    # define list of exercises 
    exercises = ["bicep curl", "squat", "lat raise"]
    
    # Create and Open a csv file to write to
    f = open("../IMU_data/data"+str(file_idx)+".csv", "w")
    
    while True:
        imu_contents = peripheral.read(service_uuid, imu_characteristic_uuid)
        control_contents = peripheral.read(service_uuid, control_characteristic_uuid)
        time_contents = peripheral.read(service_uuid, time_characteristic_uuid)
        
        ctr_b = bytearray(control_contents)
        ctr_array = list(unpack('>'+'h'*(len(ctr_b)//2),ctr_b))
        # print(ctr_array) 
        # The contents are: [save, person, exercise, baseline]
        # save = 0 to save file and quit, 1 otherwise
        # baseline = 1 to save file as a baseline, 0 otherwise
        # person = person id number
        # exercise = index in | exercises[] = ["bicep curl", "squat", "lat raise"]
        if ctr_array[0] == 0:
            # save file and quit
            if ctr_array[3] == 1:
                # save like a baseline file
                f.write("\nBASELINE FILE\n")
                pass
            # save like a normal file
            f.close()
            print("RECORDED FILE" + "data" + str(file_idx) + ".csv")
            peripheral.disconnect()
            break
        
        imu_b = bytearray(imu_contents)
        imu_intarray = list(unpack('>'+'h'*(len(imu_b)//2),imu_b))
        print(imu_intarray)
        f.write(to_string(imu_intarray)+",")
        
        time_b = bytearray(time_contents)
        time_long = unpack('<'+'L'*(len(time_b)//4),time_b)[0]
        # print("Time :  ", time_long)
        
        f.write(str(time_long) + ",")
        
        # add person int, and exercise string to file
        f.write(str(ctr_array[1]) + "," + exercises[ctr_array[2]] + "\n")
        
        # for i in contents:
        #     print(unpack('H', pack('h', i))[0])
        #     intarray.append(unpack('H', pack('b', i))[0])
        #     # print(unpack('B', pack('b', i))[0]),
        # print(intarray)
        # b.extend(contents.encode())
        # print(f"Contents: {b}")


if __name__ == "__main__":
    file_idx = 28
    while True:
        connect(file_idx)
        file_idx += 1