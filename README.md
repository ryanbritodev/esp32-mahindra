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
