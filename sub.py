import paho.mqtt.client as mqtt_client
import serial
import random
import time

UNIQUE_ID = 999

port = "COM9"
ser = serial.Serial(port, "9600")

def on_message(client, data,message):
    data = int(message.payload.decode("utf-8"))
    topic = message.topic

    print(f"recieved message: {data}")

    if ("/bright" in topic):
        if (data == 1):
            ser.write("1".encode())
        else:
            ser.write("0".encode())
    if("/length" in topic):
        if (data == 1):
            ser.write("1".encode())
        else:
            ser.write("0".encode())

broker = "broker.emqx.io"
client = mqtt_client.Client(f"lab_{random.randint(10000, 99999)}")
client.on_message = on_message

try:
    client.connect(broker)
except Exception:
    print("Failed to connect")
    exit()

client.loop_start()

print("Subscribing")
client.subscribe(f"lab/{UNIQUE_ID}/bright")
client.subscribe(f"lab/{UNIQUE_ID}/length")

time.sleep(600)

client.disconnect()
client.loop_stop()

print("stop communication")