/*
  MQTT IOT Example
  - continuously obtains values from the Virtuabotix DHT11 temperature/humidity sensor
  - formats the results as a JSON string for the IBM IOT example
  - connects to an MQTT server (either local or at the IBM IOT Cloud)
  - and publishes the JSON String to the topic named quickstart:MY_MAC_ADDRESS
*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>


#define ORG "weipxj"
#define DEVICE_TYPE "arduino_ecg"
#define DEVICE_ID "ece8067"
#define TOKEN "PYrXrkZIsUmdQ*n8X0"


// Update this to either the MAC address found on the sticker on your ethernet shield (newer shields)
// or a different random hexadecimal value (change at least the last four bytes)
byte mac[]    = {0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
char macstr[] = "ece8067";
// Note this next value is only used if you intend to test against a local MQTT server
byte localserver[] = {192, 168, 1, 98 };
// Update this value to an appropriate open IP on your local network
byte ip[]     = {192, 168, 1, 20 };

//=================================================================================================

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

const char publishTopic[] = "iot-2/evt/status/fmt/json";
const char responseTopic[] = "iotdm-1/response";
const char manageTopic[] = "iotdevice-1/mgmt/manage";
const char updateTopic[] = "iotdm-1/device/update";
const char rebootTopic[] = "iotdm-1/mgmt/initiate/device/reboot";

void callback(char* publishTopic, char* payload, unsigned int payloadLength);
//=================================================================================================


EthernetClient ethClient;
PubSubClient client(server, 1883, 0, ethClient);

float getECG(void);

void setup()
{

  // Start the ethernet client, open up serial port for debugging, and attach the DHT11 sensor
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  if (!!!client.connected()) {
    Serial.print("Reconnecting client to ");
    Serial.println(server);
    while (!!!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      //delay(500);
    }
    Serial.println();
  }

}
//int counter = 0;
void loop()
{

  float ECG = getECG();		
  String payload = "{\"d\":";
  payload += ECG;
  payload += "}";
  /*const size_t bufferSize = 2*JSON_OBJECT_SIZE(1) + 20;
    DynamicJsonBuffer jsonBuffer(bufferSize);

    const char* payload = "a";*/

  Serial.print("Sending payload: ");
  Serial.println(payload);
  // client.publish(publishTopic, payload);
  //if(client.connected())
  //{
  // client.publish(publishTopic, (char *)payload.c_str());
  if (client.publish(publishTopic, (char *)payload.c_str())) {
    Serial.println("Publish ok");
    if (!!!client.connected()) {
      Serial.print("Reconnecting client to ");
      Serial.println(server);
      while (!!!client.connect(clientId, authMethod, token)) {
        Serial.print(".");
        //delay(500);
      }
      Serial.println();
    }

  } else {
    Serial.println("Publish failed");
    if (!!!client.connected()) {
      Serial.print("Reconnecting client to ");
      Serial.println(server);
      while (!!!client.connect(clientId, authMethod, token)) {
        Serial.print(".");
        //delay(500);
      }
      Serial.println();
    }

  }
  delay(1);
}

String buildJson() {
  String payload = "{\"d\": {\"value\": 20}}";
  return payload;
}


void callback(char* publishTopic, char* payload, unsigned int length) {
Serial.println("callback invoked");
} 

float getECG(void)
	{
		float analog0;
		// Read from analogic in. 
		analog0=analogRead(0);
		// binary to voltage conversion
		return analog0 = (float)analog0 * 3.3 / 1023.0;   
	}

