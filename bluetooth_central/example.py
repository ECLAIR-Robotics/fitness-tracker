import simplepyble
from struct import pack, unpack

if __name__ == "__main__":
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
    adapter.set_callback_on_scan_found(lambda peripheral: print(f"Found {peripheral.identifier()} [{peripheral.address()}]"))

    # Scan for 5 seconds
    adapter.scan_for(5000)
    peripherals = adapter.scan_get_results()

    # Query the user to pick a peripheral
    print("Please select a peripheral:")
    for i, peripheral in enumerate(peripherals):
        print(f"{i}: {peripheral.identifier()} [{peripheral.address()}]")

    choice = int(input("Enter choice: "))
    peripheral = peripherals[choice]

    print(f"Connecting to: {peripheral.identifier()} [{peripheral.address()}]")
    peripheral.connect()

    print("Successfully connected, listing services...")
    services = peripheral.services()
    service_characteristic_pair = []
    for service in services:
        for characteristic in service.characteristics():
            service_characteristic_pair.append((service.uuid(), characteristic.uuid()))

    # Query the user to pick a service/characteristic pair
    if len(service_characteristic_pair) == 1:
        choice = 0;
    else: 
        print("Please select a service/characteristic pair:")
        for i, (service_uuid, characteristic) in enumerate(service_characteristic_pair):
            print(f"{i}: {service_uuid} {characteristic}")
        choice = int(input("Enter choice: "))
    service_uuid, characteristic_uuid = service_characteristic_pair[choice]

    # Write the content to the characteristi
    contents = bytearray(18)
    while True:
        contents = peripheral.read(service_uuid, characteristic_uuid)
        intarray = []
        # s = "ABCD"
        b = bytearray(contents) 
        print(b)
        intarray = list(unpack('>'+'h'*(len(b)//2),b))
        print(intarray)
        if intarray[0] == 0:
            peripheral.disconnect()
        # for i in contents:
        #     print(unpack('H', pack('h', i))[0])
        #     intarray.append(unpack('H', pack('b', i))[0])
        #     # print(unpack('B', pack('b', i))[0]),
        # print(intarray)
        # b.extend(contents.encode())
        # print(f"Contents: {b}")