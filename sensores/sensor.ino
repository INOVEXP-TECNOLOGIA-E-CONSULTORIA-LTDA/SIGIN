#include <OneWire.h>
#include <DallasTemperature.h>
#include "WiFi.h"
#include "HTTPClient.h"

// Dados da rede Wi-Fi (Wokwi ignora esses valores, mas eles são necessários para compilação)
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Substitua com o URL da API para a qual você quer enviar os dados
const char* serverName = "https://royal-forest-wpasclefbxlm.vapor-farm-a1.com/api/sensorReport";

// Pino onde o sensor DS18B20 está conectado
const int oneWireBus = 26;

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  // Inicializa o sensor de temperatura DS18B20
  sensors.begin();

  // Conecta ao Wi-Fi (no Wokwi isso é simulado e não requer as credenciais reais)
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Solicita a temperatura ao sensor
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);

  if (isnan(temperatureC)) {
    Serial.println("Error reading temperature!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println("°C");

    if(WiFi.status() == WL_CONNECTED) {
      // Se estiver conectado ao Wi-Fi, envia a temperatura para a API
      HTTPClient http;

      // Inicia conexão com a API
      http.begin(serverName);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      // Prepara os dados para enviar
      String httpRequestData = "codigo_identificacao=123&valor=" + String(temperatureC);

      // Faz uma requisição POST
      int httpResponseCode = http.POST(httpRequestData);

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Fecha a conexão
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
  }
  // Aguarda um tempo antes de fazer uma nova leitura
  delay(2000); // Delay de 10 segundos
}
