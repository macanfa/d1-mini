#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#define led_on 0
#define led_off 1

const char *ssid = "<SSID>";
const char *password = "<PASSWORD>";
const int led_pin = 2;

int counter = 0;

AsyncWebServer server(80);

void trenner()
{
	Serial.println("-------------------------------------------------------");
}

void boardInfo()
{
	trenner();
#if defined(ARDUINO_ARCH_ESP8266)
	Serial.print("getChipId      : ");
	Serial.println(ESP.getChipId());
	Serial.print("getCoreVersion : ");
	Serial.println(ESP.getCoreVersion());
	Serial.print("getFullVersion : ");
	Serial.println(ESP.getFullVersion());
	Serial.print("CpuFreqMHz     : ");
	Serial.println(ESP.getCpuFreqMHz());
	Serial.print("getBootMode    : ");
	Serial.println(ESP.getBootMode());
	Serial.print("getBootVersion : ");
	Serial.println(ESP.getBootVersion());
	Serial.print("CycleCount     : ");
	Serial.println(ESP.getCycleCount());
	Serial.print("getFlashChipId : ");
	Serial.println(ESP.getFlashChipId());
	Serial.print("FlashChipMode  : ");
	Serial.println(ESP.getFlashChipMode());
	Serial.print("FlashChipSize  : ");
	Serial.println(ESP.getFlashChipSize());
	Serial.print("FlashChipSpeed : ");
	Serial.println(ESP.getFlashChipSpeed());
	Serial.print("SketchSize     : ");
	Serial.println(ESP.getSketchSize());
	Serial.print("SketchMD5      : ");
	Serial.println(ESP.getSketchMD5());
	Serial.print("FreeSketchSpace: ");
	Serial.println(ESP.getFreeSketchSpace());
	Serial.print("CycleCount     : ");
	Serial.println(ESP.getCycleCount());
	Serial.print("FreeHeap       : ");
	Serial.println(ESP.getFreeHeap());
	Serial.print("getSdkVersion  : ");
	Serial.println(ESP.getSdkVersion());
#elif defined(ARDUINO_ARCH_ESP32)
	Serial.print("ChipModel      : ");
	Serial.println(ESP.getChipModel());
	Serial.print("ChipRevision   : ");
	Serial.println(ESP.getChipRevision());
	Serial.print("ChipCores      : ");
	Serial.println(ESP.getChipCores());
	Serial.print("CpuFreqMHz     : ");
	Serial.println(ESP.getCpuFreqMHz());
	Serial.print("CycleCount     : ");
	Serial.println(ESP.getCycleCount());
	Serial.print("EfuseMac       : ");
	Serial.println(ESP.getEfuseMac());
	Serial.print("FlashChipMode  : ");
	Serial.println(ESP.getFlashChipMode());
	Serial.print("FlashChipSize  : ");
	Serial.println(ESP.getFlashChipSize());
	Serial.print("FlashChipSpeed : ");
	Serial.println(ESP.getFlashChipSpeed());
	Serial.print("SketchSize     : ");
	Serial.println(ESP.getSketchSize());
	Serial.print("SketchMD5      : ");
	Serial.println(ESP.getSketchMD5());
	Serial.print("FreeSketchSpace: ");
	Serial.println(ESP.getFreeSketchSpace());
	Serial.print("CycleCount     : ");
	Serial.println(ESP.getCycleCount());
	Serial.print("FreeHeap       : ");
	Serial.println(ESP.getFreeHeap());
	Serial.print("FreePsram      : ");
	Serial.println(ESP.getFreePsram());
	Serial.print("PsramSize      : ");
	Serial.println(ESP.getPsramSize());
	Serial.print("HeapSize       : ");
	Serial.println(ESP.getHeapSize());
	Serial.print("MaxAllocHeap   : ");
	Serial.println(ESP.getMaxAllocHeap());
	Serial.print("MaxAllocPsram  : ");
	Serial.println(ESP.getMaxAllocPsram());
	Serial.print("MinFreeHeap    : ");
	Serial.println(ESP.getMinFreeHeap());
	Serial.print("MinFreePsram   : ");
	Serial.println(ESP.getMinFreePsram());
#endif
}

void initWiFi()
{
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println();
	trenner();
	Serial.print("Connected to         : ");
	Serial.println(ssid);
	Serial.print("RSSI                 : ");
	Serial.println(WiFi.RSSI());
	Serial.print("Hostname             : ");
	Serial.println(WiFi.getHostname());
	Serial.print("Mac Address          : ");
	Serial.println(WiFi.macAddress());
	Serial.print("IP Address           : ");
	Serial.println(WiFi.localIP());
	Serial.print("SubnetMask           : ");
	Serial.println(WiFi.subnetMask());
	Serial.print("Gateway IP           : ");
	Serial.println(WiFi.gatewayIP());
	Serial.print("DNS IP               : ");
	Serial.println(WiFi.dnsIP());
}

void notFound(AsyncWebServerRequest *request)
{
	request->send(404, "text/plain", "404 Not found!");
}

String webPage()
{
	counter++;
	return String(counter);
}

bool ledON()
{
	digitalWrite(led_pin, led_on);
	return true;
}
bool ledOFF()
{
	digitalWrite(led_pin, led_off);
	return false;
}

float volt()
{
	unsigned int raw = 0;
	float volt = 0.0;
	pinMode(A0, INPUT);
	raw = analogRead(A0);
	volt = raw / 1023.0;
	volt *= 4.2;

	return volt;
}

void setup()
{
	Serial.begin(115200);
	pinMode(led_pin, OUTPUT);
	digitalWrite(led_pin, LOW);
	digitalWrite(led_pin, led_on);

	initWiFi();

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send_P(200, "text/plain", webPage().c_str()); });

	server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send_P(200, "text/plain", String(ledON()).c_str()); });

	server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send_P(200, "text/plain", String(ledOFF()).c_str()); });

	server.on("/volt", HTTP_GET, [](AsyncWebServerRequest *request)
			  { request->send_P(200, "text/plain", String(volt()).c_str()); });

	server.onNotFound(notFound);

	server.begin();

	trenner();
	Serial.print("WebFrontEnd          : ");
	Serial.print("http://");
	Serial.println(WiFi.localIP());
	digitalWrite(led_pin, led_off);
	boardInfo();
}

void loop()
{
}
