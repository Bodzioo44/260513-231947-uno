import sys
Import("env")

current_env = env["PIOENV"]

PORTS = {
    "transmitter": {
        "windows": "COM4",
        "linux": "/dev/ttyACM0"
    },
    "receiver": {
        "windows": "COM5",
        "linux": "/dev/ttyACM0"
    }
}

if sys.platform.startswith("win"):
    os_key = "windows"
elif sys.platform.startswith("darwin"):
    os_key = "macos"
else:
    os_key = "linux"

target_port = PORTS[current_env][os_key]

env.Replace(UPLOAD_PORT=target_port)
env.Replace(MONITOR_PORT=target_port)

print(f"--> [OS: {os_key.upper()}] | [ENV: {current_env.upper()}] | Target Port -> {target_port}")