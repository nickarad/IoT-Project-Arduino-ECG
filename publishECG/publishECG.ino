#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#define ORG "j6n17e"
#define DEVICE_TYPE "arduino_ecg"
#define DEVICE_ID "ece8067"
#define TOKEN "PYrXrkZIsUmdQ*n8X0"

// Update this to either the MAC address found on the sticker on your ethernet shield (newer shields)
// or a different random hexadecimal value (change at least the last four bytes)
byte mac[]    = {0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
// char macstr[] = "ece8067";
// // Note this next value is only used if you intend to test against a local MQTT server
// byte localserver[] = {192, 168, 1, 98 };
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

// Callback function header
void callback(char* publishTopic, char* payload, unsigned int payloadLength);
//=================================================================================================


EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

float getECG(void);
String payload;


void setup()
{
	
  // Start the ethernet client, open up serial port for debugging, and attach the DHT11 sensor
	Ethernet.begin(mac, ip);
	Serial.begin(9600);

	// String payload = "{\"d\":";
	// payload += 0;
	// payload += "}";

	if (!client.connected()) {
		Serial.print("Reconnecting client to ");
		Serial.println(server);
		
		// client.subscribe("inTopic");
	while (!client.connect(clientId, authMethod, token)) {
		Serial.print(".");
		//delay(500);
	}
	Serial.println();
	}

	// if (client.connect(clientId, authMethod, token)) {
    // 	client.publish(publishTopic, (char *)payload.c_str());
	// 	Serial.print("Sending payload: ");
	// 	Serial.println(payload);
	// }
	// else
	// {
	// 	Serial.print("Publish failed ");
	// }

}

void loop()
{

  	//float ECG = getECG();	
	int ecg=analogRead(0);	
	payload = "{\"d\":";
	payload += ecg;
	payload += "}";
	Serial.print("Sending payload: ");
	Serial.println(payload);
	client.publish(publishTopic, (char *)payload.c_str());
	
	 //delay(1);
	client.loop();
}


void callback(char* publishTopic, char* payload, unsigned int length) {
	// In order to republish this payload, a copy must be made
	// as the orignal payload buffer will be overwritten whilst
	// constructing the PUBLISH packet.

	// Allocate the correct amount of memory for the payload copy
	byte* p = (byte*)malloc(length);
	// Copy the payload to the new buffer
	memcpy(p,payload,length);
	// client.publish("outTopic", p, length);
	client.publish(publishTopic, (char *)payload);
	// Free the memory
	free(p);
} 

// float getECG(void)
// 	{
// 		float analog0;
// 		// Read from analogic in. 
// 		analog0=analogRead(0);
// 		// binary to voltage conversion
// 		return analog0 = (float)analog0 * 3.3 / 1023.0;   
// 	}

