// Example of the different modes of the X.509 validation options
// in the WiFiClientBearSSL object
//
// Jul 2019 by Taejun Kim at www.kist.ac.kr

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>

const char *ssid = "i2r";  // 와이파이 이름
const char *pass = "00000000";      // 와이파이 비밀번호
const char *thingId = "abc";          // 사물 이름 (thing ID)
const char *host = "a8i4lgiqa43pw-ats.iot.us-west-2.amazonaws.com"; // AWS IoT Core 주소

// 사물 인증서 (파일 이름: xxxxxxxxxx-certificate.pem.crt)
const char cert_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAJ21gkV7uYRyYloKhP3J9LSvMviLMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xOTA3MDEwODQw
MjFaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDMXtLCDHsSJMGj0YW7
AYkLmiSiy/c04zsYiRKiugAOWdm9ePnwBAB2uBWLuze1NmftB6g+7G/vDog0ed6p
5J+flXB+C57FXepImn992AJgy2+zlNmEz4XmG8SnnUhI8J7v2ufMhYWMr1m65RJj
mR8RtAoxjmcD7f6PBzNjDgsO9D3hRYwD1YmWk3VuJ8wKI8Z0OEvZxLZlcNUHsv/5
+fIpsDY/l10kyNNhf3DsyS3csYwk9GHTfgerMJCYT+31QeI1nTTjMH/V7QMHGygv
9KahzDXkNnpxW0spQclewDj0k5v511YtMOZRkFaofwr3i3kTw+IIA/F3uP/UnEfI
EOU/AgMBAAGjYDBeMB8GA1UdIwQYMBaAFJ4ZUgC+5dDTg1hV5tXXPolPwq7qMB0G
A1UdDgQWBBS7B0IfwzMIV2WsEDjb6lpLGVC74zAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEApPL5ZHJyufhZOcvT8wcwSzMg
MasR3yJBjFfBjrOq/TvFBccoke7CEo6jDT5ruXiQFKXiR4TlvWJsOzjF/RHyfPz1
QPMoVWtmKrIZP255ji9J9WIcm5oRREg7MHEod9ml+KNXC+Ro0+6RZ9ABa4zTj6mF
kj+3iMB5tr3b45ClhREh3o0g8qGYJyrz/rUksfMwNI6ZWsrHGkb2uRbHJ6Pzj+ni
R24DpUltoH7FOiAKmpE87tomNE+uNbqHRCxIkZYaXoWghNeAtrV7xoKzWQzAVUuX
nZBSbXq1UfXOIY/W8aORlIL8V05GK9u87lMKu0vWXxhZi7swAIBpNOhK2jCB7g==
-----END CERTIFICATE-----
)EOF";
// 사물 인증서 프라이빗 키 (파일 이름: xxxxxxxxxx-private.pem.key)
const char key_str[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAzF7Swgx7EiTBo9GFuwGJC5okosv3NOM7GIkSoroADlnZvXj5
8AQAdrgVi7s3tTZn7QeoPuxv7w6INHneqeSfn5VwfguexV3qSJp/fdgCYMtvs5TZ
hM+F5hvEp51ISPCe79rnzIWFjK9ZuuUSY5kfEbQKMY5nA+3+jwczYw4LDvQ94UWM
A9WJlpN1bifMCiPGdDhL2cS2ZXDVB7L/+fnyKbA2P5ddJMjTYX9w7Mkt3LGMJPRh
034HqzCQmE/t9UHiNZ004zB/1e0DBxsoL/Smocw15DZ6cVtLKUHJXsA49JOb+ddW
LTDmUZBWqH8K94t5E8PiCAPxd7j/1JxHyBDlPwIDAQABAoIBAQCxd84wr8HXeY+l
f/ZO9ABb0NjrfY8HoCLbJXzHThlqXN/Vxs3TfMYiUax0EHlJpRsOG84gBhUVVFs2
pnWStnNektiCu/h9jxY5QeBgGUnHYCF5olJZIBQ4Q/i7TLtOi5SY2FDdqzzTdBnJ
T85uKrNuHheT/QK7yNl6vlgDNlin/CaJ3CuMDzjRekk6N2cZGETSyugmZieFIcfE
Ex7NI84eqWis8u+TC+uPo5Q7dhgpxv6jNIMf5ciW1IOYhkZAkJWk8kG/BCep7uMr
9+trqf04fzL7SUipXWLrX2V+rb8012ilItM2vuYxMJxs8hZyPk/Cud7UzaHBxovO
PkBY+RtxAoGBAO3UClMUpD64InyFptYw8Arpcudq+v3QDnPtFuFkl0pBNNWr4Q8n
mbEQphaoLWSyk9sig4Eja3E+pAvRLsGWxuerBn9bTOGjkgpdW4YWJKjbFKdKiTBv
HeAEEUXqWjQKZ/WS3CEPWOkrAeOyIC11XoHB9NUxdAQTv5FspxPuogkTAoGBANv8
V2cRdRZcesQ4tMZzqNxI/2hU2WcPGnAeufCAYBz4eJv4qliBlM12Fw+wo5Qe6z7L
x/ImAXlH9Fswy8O3U+DUvVi+FeEWI6xCc/cMlwwmlWPa3+CnqUvVXqT8InIfX740
efW6YEi43+EsaUSlDWNGW5IstAKpStEkEZoy6ESlAoGAcd1QKCC81i5wjG+sxeXe
N0s3sSZeDsDa+pOrnbP8XxfDBP3qncfW5JhU/In+WbTJ52Op1F0x5qEYB3RaT2Mx
zd7rGHSM5Ybbt8ykshjN6m4hgErGTgMVKZio8HFYSIwm48MxUz620cO+ftZcY4dK
/RLwdlXb9svBrw13HKrmtzMCgYB4Xdso/wlU4ecehHSNfW1wktSFLqAB3ua1YGqW
6HcugtnjZa03XjegMDQwFpN6kWxgYLoXiaoWxUMzvkBP53iStXShIOjxzt5X+8hd
7dqcAGSPTYxf7P0aDDaMCZWDYh1OAoKU7JSQVe/R0i4LLFBl+HJUSfd42U6hnp2v
BJnILQKBgCE1HGiEUC5RQ9O0e+se1z47M3D2S7T87v2tdlnXLSuLXVmBNydwER68
JpkKflIpqFrN7Fw3tiOlMUiboJW8CXv9nJPEI3xJjN3e+6pZlrkrtH+R52v6x9vp
W1OYpX+3mvHMKPyU5KX/dAvREE+4piqThqEECYILfhswfixsEuLJ
-----END RSA PRIVATE KEY-----
)EOF";
// Amazon Trust Services(ATS) 엔드포인트 CA 인증서 (서버인증 > "RSA 2048비트 키: Amazon Root CA 1" 다운로드)
const char ca_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

X509List ca(ca_str);
X509List cert(cert_str);
PrivateKey key(key_str);
WiFiClientSecure wifiClient;
PubSubClient client(host, 8883, callback, wifiClient); //set  MQTT port number to 8883 as per //standard

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(thingId)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      char buf[256];
      wifiClient.getLastSSLError(buf,256);
      Serial.print("WiFiClientSecure SSL error: ");
      Serial.println(buf);

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Set time via NTP, as required for x.509 validation
void setClock() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  Serial.println();

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  wifiClient.setTrustAnchors(&ca);
  wifiClient.setClientRSACert(&cert, &key);
  Serial.println("Certifications and key are set");

  setClock();
}

long lastMsg = 0;
char msg[50];
int value = 0;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
    Serial.print("Heap: "); Serial.println(ESP.getFreeHeap()); //Low heap can cause problems
  }
}
