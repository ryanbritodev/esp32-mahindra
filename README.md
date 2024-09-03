# Monitoramento de Temperatura e Umidade com ESP32 e IoT

Este projeto demonstra uma aplicação de Internet das Coisas (IoT) usando o microcontrolador ESP32 para monitorar dados ambientais. Utilizando um sensor DHT22, o sistema mede continuamente a temperatura e a umidade e envia esses dados para a nuvem via Wi-Fi, onde são armazenados e analisados em tempo real através do ThingSpeak.

## Objetivo

O objetivo deste projeto é mostrar como a tecnologia IoT pode ser utilizada para criar um sistema de monitoramento inteligente. A aplicação permite o acompanhamento das condições ambientais de qualquer lugar, utilizando a plataforma ThingSpeak para visualização e análise dos dados.

## Componentes Utilizados

- **Microcontrolador:** ESP32
- **Sensor:** DHT22 (Temperatura e Umidade)
- **Comunicação:** Wi-Fi (Protocolo HTTP)
- **Plataforma de Nuvem:** ThingSpeak

## Funcionalidades

- Medição contínua da temperatura e umidade ambiente.
- Transmissão dos dados para o canal ThingSpeak.
- Visualização dos dados em tempo real através da plataforma ThingSpeak.
- Interface web para visualização dos dados (utilizando HTML e CSS).

## Estrutura do Projeto

1. **ESP32 Code:**
   - Código para o microcontrolador ESP32 para coletar dados do sensor DHT22 e enviá-los para o ThingSpeak via Wi-Fi.

2. **ThingSpeak:**
   - Canal no ThingSpeak para receber e armazenar os dados de temperatura e umidade.
   - Visualização dos dados em tempo real na plataforma ThingSpeak.

3. **Interface Web:**
   - Uma página HTML/CSS para exibir os dados em tempo real. A página inclui:
     - Dados de temperatura.
     - Dados de umidade.
     - Informações em tempo real atualizadas a cada 15 segundos.

## Requisitos

- **Hardware:**
  - Microcontrolador ESP32.
  - Sensor DHT22.
  - Acesso à rede Wi-Fi.

- **Software:**
  - Arduino IDE ou similar para programação do ESP32.
  - Conta no ThingSpeak para criar e gerenciar o canal.

## Configuração

### 1. Configuração do ESP32

- **Bibliotecas Necessárias:**
  - `DHT` - Biblioteca para o sensor DHT22.
  - `WiFi` - Biblioteca para a conexão Wi-Fi.
  - `HTTPClient` - Biblioteca para comunicação HTTP.

- **Código de Exemplo:**

```cpp
#include <WiFi.h>
#include <DHT.h>
#include <HTTPClient.h>

// Configurações Wi-Fi
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Configurações ThingSpeak
const char* thingSpeakAPIKey = "YOUR_THINGSPEAK_API_KEY";
const char* thingSpeakURL = "http://api.thingspeak.com/update";

// Configurações do sensor DHT22
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  HTTPClient http;
  http.begin(thingSpeakURL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String postData = "api_key=" + String(thingSpeakAPIKey) + "&field1=" + String(temperature) + "&field2=" + String(humidity);
  int httpResponseCode = http.POST(postData);
  
  if (httpResponseCode > 0) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Error sending data");
  }
  http.end();
  
  delay(15000); // Aguarda 15 segundos
}
```

## 2. Configuração do ThingSpeak

1. Crie uma conta no [ThingSpeak](https://thingspeak.com/).
2. Crie um novo canal e anote o API Key para uso no código ESP32.
3. Adicione dois campos:
   - **Field1:** Temperatura (°C)
   - **Field2:** Umidade (%)

## 3. Interface Web

- **Arquivos:**
  - `index.html`: Arquivo HTML para exibição dos dados.
  - `style.css`: Arquivo CSS para estilização da página.

- **Link da Página:** [Monitoramento de Temperatura e Umidade](https://ryanbritodev.github.io/esp32-mahindra/src/html/index.html)

## Contribuições

Contribuições são bem-vindas! Se você tem sugestões para melhorias ou correções, sinta-se à vontade para abrir uma issue ou enviar um pull request.

## Licença

Este projeto está licenciado sob a Licença MIT - veja o [LICENSE](LICENSE) para mais detalhes.
