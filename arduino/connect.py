import serial
ser = serial.Serial('/dev/tty.usbmodem11301', 9600, timeout=1)
ser.reset_input_buffer()


def main():
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(line)

def calibrate():
    print("All bottles must be empty")

    results = ser.readline().decode('utf-8').rstrip()
    file_of_res = open("weight_data.txt", "w")
    for n, res in enumerate(results.split(" ")):
        file_of_res.write(f"{n} : {res}")
    file_of_res.close()


def drink(drink_id):
    ser.write(b'a')
    ser.write(int(drink_id[0]))
    res = ser.readline ().decode ('utf-8').rstrip ()
    print(res)
    passcode = drink_id[2:]
    for num in passcode.split("\t"):
        ser.write(int(num[0]))
        res = ser.readline().decode('utf-8').rstrip()
        print(res)
        ser.write(int(str(num)[1:]))
        out = ser.readline().decode('utf-8').rstrip()
        print(out)



if __name__ == '__main__':
   # drink ("3\t0150\t2100\t3200")
    ser.write(b"d")
    main()