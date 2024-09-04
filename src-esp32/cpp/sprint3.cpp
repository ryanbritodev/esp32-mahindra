/* 
------------------ FIAP --------------------
SPRINT 3 - TECH MAHINDRA
EDGE COMPUTING & COMPUTER SYSTEMS
Participantes:
Prof. Paulo Marcotti PF2150
Arthur Cotrick Pagani RM554510
Diogo Leles Franciulli RM558487
Felipe Sousa de Oliveira RM559085
Ryan Brito Pereira Ramos RM554497
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 15 // Pino GPIO15 do ESP32 para o DHT22
#define DHTTYPE DHT22 // Tipo de sensor DHT (DHT22)
DHT dht(DHTPIN, DHTTYPE);

#define POT_PIN 34 // Pino GPIO34 do ESP32 para o Potenciômetro

// Pinos do sensor ultrassônico
#define TRIGGER_PIN 5
#define ECHO_PIN 18

// Pinos do LED e Buzzer
#define ALERT_LED_PIN 2 // Pino GPIO2 do ESP32 para o LED Vermelho
#define BUZZER_PIN 4 // Pino GPIO4 do ESP32 para o Buzzer

// Credenciais
const char* ssid = "Wokwi-GUEST"; // Rede Wi-Fi
const char* password = ""; // Senha da rede Wi-Fi
const char* apiKey = "9S57D76C87Z162OP"; // API Key
const char* server = "http://api.thingspeak.com"; // Servidor ThingSpeak

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Configuração dos pinos
  pinMode(POT_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ALERT_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Inicialização do estado dos LEDs e Buzzer
  digitalWrite(ALERT_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Inicialização e loop de verificação da rede Wi-Fi
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
    // Leitura dos sensores
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int potValue = analogRead(POT_PIN); // Leitura do valor do Potenciômetro
    float speed = map(potValue, 0, 4095, 0, 322); // Mapeamento do valor do potenciômetro para simular a velocidade de um carro da Fórmula E (0 a 322 km/h)

    if (isnan(h) || isnan(t)) {
      Serial.println("Falha ao ler o sensor DHT22!");
      return;
    }

    // Leitura do sensor ultrassônico
    long duracao, distancia;
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    duracao = pulseIn(ECHO_PIN, HIGH);
    distancia = (duracao / 2) * 0.0344; // Conversão da distância em cm

    // Controle do LED e Buzzer com base na distância
    if (distancia <= 100) { // Distância crítica de 100 cm (1 metro)
      // Mantém o LED aceso
      digitalWrite(ALERT_LED_PIN, HIGH);
      // Ativa o Buzzer
      digitalWrite(BUZZER_PIN, HIGH);
    } else {
      // Desliga o LED e o Buzzer quando a distância é maior que 100 cm
      digitalWrite(ALERT_LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
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
  
  // Espera 15 segundos para enviar a requisição novamente
  delay(15000);
}
