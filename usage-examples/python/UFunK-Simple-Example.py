# Example: UFunK-Pad Simple Example (Python 3 on Windows)

# required libs: 
#   keyboard    (needs installation : https://github.com/boppreh/keyboard)
#   serial      (should be installed)
#   atexit      (should be installed)
import keyboard
import serial
import atexit

# name of the serial-port the UFunK-Pad is connected to e.g. COM1, COM7, COM24 
#   (on linux  serial-devices are located under the /dev/ directory. names could be e.g /dev/ttyACM0 , /dev/ttySUSB1 )
portname = 'COM7'
# open the serial-port
port = serial.Serial(portname, 9600, timeout=4, rtscts=True)

# define function to be run on script-exit
def cleanup():
    # remove all hotkeys
    keyboard.unhook_all()
    # close serial-port
    port.close()
# register cleanup-function to be executed on script-exit
atexit.register(cleanup)

# A Hotkey - F13 prints text in console
keyboard.add_hotkey('f13', lambda: print('f13 was pressed!'))

# Write some text on Infoline1 of the UFunK-Pad
port.write(bytes('I1TLHELLOWORLD\n', 'ascii'))

# prevent script from closing until Ctrl+C is pressed
print('Press Ctrl+C to Exit')
try:
    keyboard.wait()
except KeyboardInterrupt:
    print('Closing')