#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 15        // Pino GPIO15 do ESP32 para o DHT22
#define DHTTYPE DHT22    // Tipo de sensor DHT (DHT22)
DHT dht(DHTPIN, DHTTYPE);

#define POT_PIN 34       // Pino GPIO34 do ESP32 para o Potenciômetro

// Credenciais
const char* ssid = "Wokwi-GUEST";    // Rede Wi-Fi
const char* password = "";           // Senha da rede Wi-Fi
const char* apiKey = "9S57D76C87Z162OP";  // API Key
const char* server = "http://api.thingspeak.com"; // Servidor ThingSpeak

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Entrada analógica
  pinMode(POT_PIN, INPUT);

  // Iniciallização e loop de verificação da rede Wi-FI
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" conectado!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int potValue = analogRead(POT_PIN); // Leitura do valor do Potênciometro
    float speed = map(potValue, 0, 4095, 0, 322); // Mapeamento do valor do potenciômetro para simular a velocidade do veículo (0 a 322 km/h, maior velocidade já registrada por um carro de Fórmula E)

    if (isnan(h) || isnan(t)) {
      Serial.println("Falha ao ler o sensor DHT22!");
      return;
    }

    // Envio de dados para o ThingSpeak
    HTTPClient http;
    String url = String(server) + "/update?api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h) + "&field3=" + String(speed);
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode > 0) {
      String payload = http.getString(); // Resposta da requisição HTTP
      Serial.println("Dados enviados ao ThingSpeak.");
      Serial.print("Código HTTP: ");
      Serial.println(httpCode);
      Serial.println("Resposta: ");
      Serial.println(payload);
    } else {
      Serial.print("Erro ao enviar dados. Código HTTP: ");
      Serial.println(httpCode);
    }
    
    http.end();
  } else {
    Serial.println("WiFi não conectado. Tentando reconectar...");
  }
  
  // Espera 15 segundos de espera para enviar a requisição novamente
  delay(15000);
}
