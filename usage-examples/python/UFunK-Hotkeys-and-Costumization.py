# Example: UFunK-Pad Key-Handling and Costumization per serial-interface (Python 3 on Windows)

# required libs: 
#   keyboard    (needs installation : https://github.com/boppreh/keyboard)
#   serial      (should be installed)
#   atexit      (should be installed)
#   random      (should be installed)
import keyboard
import serial
import atexit
import random

# name of the serial-port the UFunK-Pad is connected to e.g. COM1, COM7, COM24 
#   (on linux  serial-devices are located under the /dev/ directory. names could be e.g /dev/ttyACM0 , /dev/ttySUSB1 )
portname = 'COM7'
# open the serial-port
port = serial.Serial(portname, 9600, timeout=4, rtscts=True)

# define function to be run on script-exit
def cleanup():
    # remove all hotkeys
    keyboard.unhook_all()
    # write text on Infoline2 of UFunK-Pad
    port.write(bytes('I2TLScript Closed\n', 'ascii'))
    # close serial-port
    port.close()
# register cleanup-function to be executed on script-exit
atexit.register(cleanup)


# write text on Infoline1 of UFunK-Pad
# UFunK-Pad is custumized via LF-terminated command-strings sent over the serial-port
# refer to UFunK-Pad-Serial-Control-Codes.txt for command-strings
port.write(bytes('I1TL  Python-Example\n', 'ascii'))
# read and print confirmation message
print(port.readline())
# change color off Infoline1 to blue
port.write(bytes('I1TC948\n', 'ascii'))

# change background/border/text color of all buttons
# all colors are RGB 565 in decimal
# 65224 = yellow in rgb 565
# 948 = blue in rgb 565
for FKey in range(13, 25):
    # normal state
    port.write(bytes('F'+str(FKey)+'CF65224\n', 'ascii'))
    port.write(bytes('F'+str(FKey)+'CL948\n', 'ascii'))
    port.write(bytes('F'+str(FKey)+'CB948\n', 'ascii'))
    # pushed state
    port.write(bytes('F'+str(FKey)+'PF948\n', 'ascii'))
    port.write(bytes('F'+str(FKey)+'PL65224\n', 'ascii'))
    port.write(bytes('F'+str(FKey)+'PB65224\n', 'ascii'))
    # draw button
    port.write(bytes('F'+str(FKey)+'DB\n', 'ascii'))
    

# define hotkeys

# F13 - send Ctrl+A / select all
keyboard.add_hotkey('f13', lambda: keyboard.press_and_release('ctrl+a'))    
# name the button
port.write(bytes('F13TLSel  All\n', 'ascii'))
port.write(bytes('F13DB\n', 'ascii'))

# F14 - send Ctrl+X / Cut
keyboard.add_hotkey('f14', lambda: keyboard.press_and_release('ctrl+x'))    
# name the button
port.write(bytes('F14TLCut  \n', 'ascii'))
port.write(bytes('F14DB\n', 'ascii'))

# F15 - send Ctrl+C / Copy
keyboard.add_hotkey('f15', lambda: keyboard.press_and_release('ctrl+c'))    
# name the button
port.write(bytes('F15TLCopy \n', 'ascii'))
port.write(bytes('F15DB\n', 'ascii'))

# F16 - send Ctrl+V / Paste
keyboard.add_hotkey('f16', lambda: keyboard.press_and_release('ctrl+v'))    
# name the button
port.write(bytes('F16TLPaste\n', 'ascii'))
port.write(bytes('F16DB\n', 'ascii'))

# F21 - send some Text
keyboard.add_hotkey('f21', lambda: keyboard.write('Hello from my UFunK-Pad'))    
# name the button
port.write(bytes('F21TLHello\n', 'ascii'))
# change color of text label to black
port.write(bytes('F21CL0\n', 'ascii'))
port.write(bytes('F21DB\n', 'ascii'))

# F22 - a counter
myNumber = 0
def mycounter():
    global myNumber
    myNumber += 1
    # update the button name
    port.write(bytes('F22TL'+str(myNumber)+'\n', 'ascii'))
    port.write(bytes('F22DB  All\n', 'ascii'))
    
keyboard.add_hotkey('f22', lambda: mycounter())
# name the button
port.write(bytes('F22TL'+str(myNumber)+'\n', 'ascii'))
# color the button background white
port.write(bytes('F22CF65535\n', 'ascii'))
port.write(bytes('F22DB\n', 'ascii'))

# F23 - randomly colorchanging button
keyboard.add_hotkey('f23', lambda: port.write(bytes('F23CF'+str(random.randrange(65535))+'\n', 'ascii')))
# delete the button name
port.write(bytes('F23TL\n', 'ascii'))
# set button border to black
port.write(bytes('F23CB0\n', 'ascii'))
# set button pushed color to black
port.write(bytes('F23PF0\n', 'ascii'))
# draw button
port.write(bytes('F23DB\n', 'ascii'))

# F24 - switch color of Infoline1
L1state = True
def myswitch():
    global L1state
    L1state = not L1state
    if L1state:
        port.write(bytes('I1TC948\n', 'ascii'))
        port.write(bytes('F24TLB\n', 'ascii'))
    else:
        port.write(bytes('I1TC65224\n', 'ascii'))
        port.write(bytes('F24TLY\n', 'ascii'))
    port.write(bytes('F24DB\n', 'ascii'))

keyboard.add_hotkey('f24', lambda: myswitch())
port.write(bytes('F24TLB\n', 'ascii'))
port.write(bytes('F24DB\n', 'ascii'))

# ---

# write text on Infoline2 of UFunK-Pad
port.write(bytes('I2TLScript Running\n', 'ascii'))


# prevent script from closing until Ctrl+C is pressed
print('Press Ctrl+C to Exit')
try:
    keyboard.wait()
except KeyboardInterrupt:
    print('Closing')