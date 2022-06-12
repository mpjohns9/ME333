# chapter 28 in python

# sudo apt-get install python3-pip
# python3 -m pip install pyserial
# sudo apt-get install python3-matplotlib

import serial
import matplotlib.pyplot as plt 
from statistics import mean 
from genref import genRef

def read_plot_matrix():
    n_str = ser.read_until(b'\n');  # get the number of data points to receive
    n_int = int(n_str) # turn it into an int
    print('Data length = ' + str(n_int))
    ref = []
    data = []
    data_received = 0
    while data_received < n_int:
        dat_str = ser.read_until(b'\n');  # get the data as a string, ints seperated by spaces
        dat_f = list(map(float,dat_str.split())) # now the data is a list
        # print(dat_str)
        ref.append(dat_f[0])
        data.append(dat_f[1])
        data_received = data_received + 1
    meanzip = zip(ref,data)
    meanlist = []
    for i,j in meanzip:
        meanlist.append(abs(i-j))
    score = mean(meanlist)
    t = range(len(ref)) # index array
    plt.plot(t,ref,'r*-',t,data,'b*-')
    plt.title('Score = ' + str(score))
    plt.ylabel('value')
    plt.xlabel('index')
    plt.show()

ser = serial.Serial('/dev/ttyUSB0',230400,rtscts=1)
print('Opening port: ')
print(ser.name)

has_quit = False
# menu loop

while not has_quit:
    print('PIC32 MOTOR DRIVER INTERFACE')
    # display the menu options; this list will grow
    print('\ta: Read Current Sensor (ADC counts) \tj: Get Position Gains')
    print('\tb: Read Current Sensor (mA) \t\tk: Test Current Control')
    print('\tc: Read Encoder (counts) \t\tl: Go to Angle (deg)')
    print('\td: Read Encoder (deg) \t\t\tm: Load Step Trajectory')
    print('\te: Reset Encoder \t\t\tn: Load Cubic Trajectory')
    print('\tf: Set PWM (-100 to 100) \t\to: Execute Trajectory')
    print('\tg: Set Current Gains \t\t\tp: Unpower the Motor') # '\t' is a tab
    print('\th: Get Current Gains \t\t\tq: Quit')
    print('\ti: Set Position Gains \t\t\tr: Get Mode')
    # read the user's choice
    selection = input('\nENTER COMMAND: ')
    selection_endline = selection+'\n'
     
    # send the command to the PIC32
    ser.write(selection_endline.encode()) # .encode() turns the string into a char array
    
    # take the appropriate action
    # there is no switch() in python, using if elif instead
    if (selection == 'a'):
        n_str = ser.read_until(b'\n')  
        n_int = int(n_str) # turn it into an int
        print(f'Motor current (ADC counts): {str(n_int)}\n') # print it to the screen
    if (selection == 'b'):
        n_str = ser.read_until(b'\n')  
        n_int = int(n_str) # turn it into an int
        print(f'Motor current (mA): {str(n_int)}\n') # print it to the screen
    elif (selection == 'c'):
        n_str = ser.read_until(b'\n')  
        n_int = int(n_str) # turn it into an int
        print(f'Current encoder angle (counts): {str(n_int)}\n') # print it to the screen

    elif (selection == 'd'):
        n_str = ser.read_until(b'\n')  
        n_int = int(n_str) # turn it into an int
        print(f'Current encoder angle (deg): {str(n_int)}\n') # print it to the screen

    elif (selection == 'e'):
        print('Encoder has been reset.')

    elif (selection == 'f'):
        p = input('Enter desired PWM: ')
        p_endline = f'{p}\n'
        ser.write(p_endline.encode())
        n_str = ser.read_until(b'\n')
        n_int = int(n_str)
        print(f'The PWM is set to {n_int}')

    elif (selection == 'g'):
        kp = input('Enter Kp: \n')
        ser.write(f'{kp}\n'.encode())

        ki = input('Enter Ki: \n')
        ser.write(f'{ki}\n'.encode())
    
        print(f'Current gains set.\n')

    elif (selection == 'h'):
        kp_str = str(ser.read_until(b'\n'), 'utf-8')
        ki_str = str(ser.read_until(b'\n'), 'utf-8')
        kp = float(kp_str)
        ki = float(ki_str)
        print(f'Current Kp: {kp}')   
        print(f'Current Ki: {ki}\n')

    elif (selection == 'i'):
        kp = input('Enter Kp: \n')
        ser.write(f'{kp}\n'.encode())

        ki = input('Enter Ki: \n')
        ser.write(f'{ki}\n'.encode())

        kd = input('Enter Kd: \n')
        ser.write(f'{kd}\n'.encode())
    
        print(f'Position gains set.\n')

    elif (selection == 'j'):
        kp_str = str(ser.read_until(b'\n'), 'utf-8')
        ki_str = str(ser.read_until(b'\n'), 'utf-8')
        kd_str = str(ser.read_until(b'\n'), 'utf-8')

        kp = float(kp_str)
        ki = float(ki_str)
        kd = float(kd_str)

        print(f'Current Kp: {kp}')   
        print(f'Current Ki: {ki}')
        print(f'Current Kd: {kd}\n')

    elif (selection == 'k'):
        read_plot_matrix()

    elif (selection == 'l'):
        a = input('Enter desired angle: ')
        a_endline = f'{a}\n'
        ser.write(a_endline.encode())
        a_str = ser.read_until(b'\n')
        a_int = float(a_str)
        print(f'The angle is set to {a_int}.')

    elif (selection == 'm'):
        method = 'step'
        ref = genRef(method)
        t = len(ref)
        t_endline = f'{t}\n'
        
        ser.write(t_endline.encode())
        for r in ref:
            r_endline = f'{r}\n'
            ser.write(r_endline.encode())

    elif (selection == 'n'):
        method = 'cubic'
        ref = genRef(method)
        t = len(ref)
        t_endline = f'{t}\n'
        
        ser.write(t_endline.encode())
        for r in ref:
            r_endline = f'{r}\n'
            ser.write(r_endline.encode())

        # plt.plot(t,ref,'r*-')
        # plt.ylabel('value')
        # plt.xlabel('index')
        # plt.show()

    elif (selection == 'o'):
        read_plot_matrix()
        print('Executing trajectory.\n')

    elif (selection == 'p'):
        print('Unpowered motor.\n')
    
    elif (selection == 'q'):
        print('Exiting client')
        has_quit = True; # exit client
        # be sure to close the port
        ser.close()

    elif (selection == 'r'):
        mode = ser.read_until(b'\n')
        mode_int = int(mode)
        mode_list = ['IDLE', 'PWM', 'ITEST', 'HOLD', 'TRACK']
        print(f'The current mode is {mode_list[mode_int]}.\n')

    else:
        print('Invalid Selection ' + selection_endline)



