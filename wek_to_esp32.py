import serial
from pythonosc.dispatcher import Dispatcher
from pythonosc.osc_server import BlockingOSCUDPServer

esp = serial.Serial("/dev/cu.usbserial-0001", 9600)

def on_output(address, *args):
    cls = int(round(args[0]))
    print("Class:", cls)
    esp.write(str(cls).encode())

dispatcher = Dispatcher()
dispatcher.map("/wek/outputs", on_output)

print("Forwarding Wekinator to ESP32...")
BlockingOSCUDPServer(("127.0.0.1", 12000), dispatcher).serve_forever()