# https://console.bluemix.net/docs/services/IoT/applications/libraries/python.html#python
import ibmiotf.application
# ============================ IBM Credentials ====================================================================================================================
options = {
    "org": "j6n17e",
    "id": "ece8067",
    "auth-method": "use-token-auth",
    "auth-key": "a-j6n17e-mzrhpysawv",
    "auth-token": "PYrXrkZIsUmdQ*n8X0",
    "clean-session": "true"
  }

client = ibmiotf.application.Client(options)
client.connect()
client.subscribeToDeviceEvents(deviceType="arduino_ecg", deviceId="ece8067", msgFormat="json")
