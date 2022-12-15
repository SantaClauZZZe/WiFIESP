#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

String ip = "IP unset";

String password = "12341234"; // Для режима: Access point (точка доступа) (phone (or another device) -> esp8266)
String ssid_pref = "BASE";

String passwordCLI = "148869420"; // Для режима: STA (esp8266 -> router)
String ssidCLI = "L";

ESP8266WiFiMulti wifiClient;

String id()
{
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH] - 1, HEX);

  macID.toUpperCase();

  return macID;
}

bool StartAPMode()
{
  IPAddress apIP(192, 164, 4, 2);
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255,255,255,0));
  WiFi.softAP(ssid_pref + id(), password);
  Serial.println("WiFi AP is up with: " + ssid_pref + id());

  return true;
}

bool StartCLI()
{
    wifiClient.addAP(ssidCLI.c_str(), passwordCLI.c_str());

    while (wifiClient.run() != WL_CONNECTED) { }

    return true;
}

void WiFi_init(bool ap_mode)
{
  if (ap_mode)
  {
    ip = WiFi.softAPIP().toString();
    StartAPMode();
  }
  else
  {
    StartCLI();
    ip = WiFi.localIP().toString();
  }

  Serial.print("IP is = ");
  Serial.println(ip);
}