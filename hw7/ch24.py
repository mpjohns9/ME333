import serial
ser = serial.Serial('/dev/ttyUSB0',230400,rtscts=1)
print('Opening port: ')
print(ser.name)
ser.write(b'5.0 0.7\n') # Kp Ki
sampnum = 0
read_samples = 10
ADCval = []
ref = []
while read_samples > 1:
    data_read = ser.read_until(b'\n',50)
    data_text = str(data_read,'utf-8')
    data = list(map(int,data_text.split()))
    if(len(data)==3):
        read_samples = data[0]
        ADCval.append(data[1])
        ref.append(data[2])
        sampnum = sampnum + 1
# plot it
import matplotlib.pyplot as plt 
t = range(len(ADCval)) # time array
plt.plot(t,ADCval,'r*-',t,ref,'b*-')
plt.ylabel('value')
plt.xlabel('sample')
plt.show()
# be sure to close the port
ser.close()