import RPi.GPIO as GPIO
from time import time
import subprocess

# Create a dictionary to map shifted and masked IR codes to their meanings
ir_code_map = {
    (0xF700FF & 0xFFFF) : "IR_BPLUS",
    (0xF7807F & 0xFFFF) : "IR_BMINUS",
    (0xF740BF & 0xFFFF) : "IR_OFF",
    (0xF7C03F & 0xFFFF) : "IR_ON",
    (0xF720DF & 0xFFFF) : "IR_R",
    (0xF7A05F & 0xFFFF) : "IR_G",
    (0xF7609F & 0xFFFF) : "IR_B",
    (0xF7E01F & 0xFFFF) : "IR_W",
    (0xF710EF & 0xFFFF) : "IR_R1",
    (0xF7906F & 0xFFFF) : "IR_G1",
    (0xF750AF & 0xFFFF) : "IR_B1",
    (0xF7D02F & 0xFFFF) : "IR_FLASH",
    (0xF730CF & 0xFFFF) : "IR_R2",
    (0xF7B04F & 0xFFFF) : "IR_G2",
    (0xF7708F & 0xFFFF) : "IR_B2",
    (0xF7F00F & 0xFFFF) : "IR_STROBE",
    (0xF708F7 & 0xFFFF) : "IR_R3",
    (0xF78877 & 0xFFFF) : "IR_G3",
    (0xF748B7 & 0xFFFF) : "IR_B3",
    (0xF7C837 & 0xFFFF) : "IR_FADE",
    (0xF728D7 & 0xFFFF) : "IR_R4",
    (0xF7A857 & 0xFFFF) : "IR_G4",
    (0xF76897 & 0xFFFF) : "IR_B4",
    (0xF7E817 & 0xFFFF) : "IR_SMOOTH"
}

color_map = {
    "IR_R": "0xFF0000",   # Red
    "IR_G": "0x00FF00",   # Green
    "IR_B": "0x0000FF",   # Blue
    "IR_W": "0xFFFFFF",   # White
    "IR_R1": "0xFF1A00",  # Orange
    "IR_G1": "0x00FF00",  # 
    "IR_B1" : "0x0000FF",  #
    "IR_R2" : "0xFF6A00",  # 
    "IR_G2" : "0x00FF00",  # 
    "IR_B2" : "0x0000FF",  #
    "IR_R3" : "0xFF6A00",  # 
    "IR_G3" : "0x00FF00",  # 
    "IR_B3" : "0x0000FF",  # 
    "IR_R4" : "0xFF6A00",  # 
    "IR_G4" : "0x00FF00",  # 
    "IR_B4" : "0x0000FF"   # 
}

executable_file = "/home/rpi/NeoPixel/build/neoLight_program"

ir_pin = 32
def setup() :
    GPIO.setmode(GPIO.BOARD)  # Numbers GPIOs by physical location
    GPIO.setup(ir_pin, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)


    def binary_aquire(pin, duration) :
    # aquires data as quickly as possible
    t0 = time()
    results = []
    while (time() - t0) < duration:
results.append(GPIO.input(pin))
return results


def on_ir_receive(pinNo, bouncetime = 100) :
    # when edge detect is called(which requires less CPU than constant
        # data acquisition), we acquire data as quickly as possible
    data = binary_aquire(pinNo, bouncetime / 1000.0)
    oldval = 1
    lastindex = 0
    index = 0

    #print('len data, data: ', len(data), data)
    #print('len gaps,gaps', len(gaps), gaps)
    if len(data) < bouncetime:
return
rate = len(data) / (bouncetime / 1000.0)
pulses = []
i_break = 0
# detect run lengths using the acquisition rate to turn the times in to microseconds
for i in range(1, len(data)) :
    if (data[i] != data[i - 1]) or (i == len(data) - 1) :
        pulses.append((data[i - 1], int((i - i_break) / rate * 1e6)))
        i_break = i

        outbin = ""
        for val, us in pulses :
if val != 1 :
    continue
    if outbin and us > 2000:
break
elif us < 1000 :
    outbin += "0"
    elif 1000 < us < 2000 :
    outbin += "1"
    try :
    return int(outbin, 2)
    except ValueError :
# probably an empty code
return None


def destroy() :
    GPIO.cleanup()


    if __name__ == "__main__" :
        setup()
        try :
        print("Starting IR Listener")
        process = None
        while True :
            # print("Waiting for signal")
            GPIO.wait_for_edge(ir_pin, GPIO.FALLING)
            code = on_ir_receive(ir_pin)
            if code:
trimmed_code = code & 0xFFFF
#print(hex(trimmed_code))
if trimmed_code in ir_code_map :
button = ir_code_map[trimmed_code]
print(button)
if button == "IR_ON" and not process :
    # Run the executable in the background
    process = subprocess.Popen([executable_file],
        stdin = subprocess.PIPE,
        stdout = None,
        stderr = None,
        text = True,
        shell = True)

    pid = process.pid
    print(pid)

    elif process :
if (button in color_map) :
    # Color
    process.stdin.write(color_map[button] + "\n")
else :
    # Function
    process.stdin.write(button + "\n")
    process.stdin.flush()

    if button == "IR_OFF":
process = None

except KeyboardInterrupt :
# User pressed CTRL - C
# Reset GPIO settings
print("Ctrl-C pressed!")
except RuntimeError :
# this gets thrown when control C gets pressed
# because wait_for_edge doesn't properly pass this on
pass

print("Quitting")
destroy()
