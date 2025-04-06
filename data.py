#Run pip install pyserial 

from serial.tools import list_ports
import serial
import time
import csv
from datetime import datetime

# Loop through existing COM ports and print them to terminal
ports = list_ports.comports()
for port in ports: print(port)

# Create data file
f = open("data.csv", "w",newline='')
f.truncate() # Used to clear any existing data

# Target Arduino serial port
serialCom = serial.Serial('COM3', 9600)

# Reset the arduino
serialCom.setDTR(False)
time.sleep(1)
serialCom.flushInput() # Clear the serial com
serialCom.setDTR(True)

def isfloat(num):
    try:
        float(num)
        return True
    except ValueError:
        return False

rows = 20 # Number of data rows to record

for row in range(rows):
    try:
        # Read in a line of data
        s_bytes = serialCom.readline()
        # Decode the binary
        decoded_bytes = s_bytes.decode("utf-8").strip('\r\n')

        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        
        #Parse lines
        if row == 0: #Target headers
            values = decoded_bytes.split(",")
            values.insert(0, "Timestamp")
        else: # Target data rows
            values = [float(x) for x in decoded_bytes.split()]

            # Skip non data like readings
            # Source ChatGPT
            # Prompt: how to skip non-data readings
            if len(values) != 3 and not all(isfloat(v) for v in values):
                continue

            values.insert(0, timestamp)

        print(values) #Print the data

        #Write data to CSV

        writer = csv.writer(f,delimiter=",")
        writer.writerow(values)
    except:
        print("ERROR, Line was not recorded.")

f.close() #Close CSV file


