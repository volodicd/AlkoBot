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
    ser.write("calibrate")
    results = ser.readline().decode('utf-8').rstrip()
    file_of_res = open("weight_data.txt", "w")
    for n, res in enumerate(results.split(" ")):
        file_of_res.write(f"{n} : {res}")
    file_of_res.close()


def drink(pump_nums, weights, passcode):
    standart_weight = open("weight_data.txt", "r")
    for pump_num, weight in pump_nums, weights:
        ser.write(f"{pump_num}:{weight}")
        res = ser.readline().decode('utf-8').rstrip()


if __name__ == '__main__':
    main()