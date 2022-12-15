import serial as s
import paho.mqtt.client as mqtt_client
import random

port="COM5"
arduino = s.Serial(port=port, baudrate=9600)    # Подключение к ардуино

broker = "broker.emqx.io"
my_id = 999
client = mqtt_client.Client(f'lab_{random.randint(10000, 999999)}')

client.connect(broker)

max_val = 0

try:
    while True:
        while arduino.inWaiting() == 0:    # Ждем данные
            pass

        val = arduino.readline()
        val = ''.join(filter(str.isdigit, str(val)))    # Достаем цифры из строки данных

        if (len(val) > 1) and (len(val) < 10):
            if val[0] == "1":    # Если код данных == 1, то это результат измерения расстояния
                val = int(val[1:])

                print("Расстояние: ", val)

                if val < 20:
                    client.publish(f'lab/{my_id}/length', 1)
                    print("YES")
                else:
                    client.publish(f'lab/{my_id}/length', 0)
            elif val[0] == "0":    # Если код данных == 0, то это результат измерения уровня освещенности
                val = int(val[1:])

                print("Освещение: ", val)
                print("Максимум: ", max_val)
                print("Порог: ", max_val / 2)

                if val > max_val:
                    max_val = val
                if val < max_val / 2:
                    client.publish(f'lab/{my_id}/bright', 1)
                    print("YES")
                else:
                    client.publish(f'lab/{my_id}/bright', 0)
                    
                print()
except KeyboardInterrupt:
    arduino.close()
    client.disconnect()
    print("Program finished!")