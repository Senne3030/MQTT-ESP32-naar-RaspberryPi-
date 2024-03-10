from gpiozero import LED
import paho.mqtt.subscribe as subscribe

led_yellow_pin = 17
led_green_pin = 27
led_red_pin = 23
led_blue_pin = 24

mqtt_broker = "senne3030.local"
mqtt_port = 1883
mqtt_user = "senne"
mqtt_password = "senne123"


led_blue = LED(led_blue_pin)
led_red = LED(led_red_pin)
led_yellow = LED(led_yellow_pin)
led_green = LED(led_green_pin)


def on_message_received(client, userdata, message):
    topic = message.topic
    payload = message.payload.decode("utf-8")
    print("Received message on topic:", topic, "with payload:", payload)


    if topic == "huis/woonkamer/licht":
        control_led(led_yellow, payload)
    elif topic == "huis/keuken/licht":
        control_led(led_blue, payload)
    elif topic == "huis/gang/licht":
        control_led(led_green, payload)
    elif topic == "huis/slaapkamer/licht":
        control_led(led_red, payload)

def control_led(led, payload):
    if payload == "on":
        led.on()
    elif payload == "off":
        led.off()

subscribe.callback(on_message_received, topics=[("huis/woonkamer/licht", 0), ("huis/keuken/licht", 0), ("huis/gang/licht", 0), ("huis/slaapkamer/licht", 0)], hostname=mqtt_broker, port=mqtt_port, auth={'username': mqtt_user, 'password': mqtt_password})

try:
    while True:
        pass
except KeyboardInterrupt:
	print("Script Terminated.")
