# Mahindra Racing e IoT: Monitoramento de Temperatura, Velocidade e Umidade na Fórmula E com o ESP32

<div align="left">
   <img src="https://resources.formula-e.pulselive.com/photo-resources/2023/12/12/1db5e5a8-79de-4c4e-9464-858ae488412f/LJP-Mahindra-S10-Livery-Launch-10-3-.jpg?width=1440&height=810" alt="Carro Mahindra Racing" width="500px"/> 
   <img src="https://zbotic.in/wp-content/uploads/2022/10/4d35zrjm.png" alt="ESP32" width="282px"/>
</div>

<br>

Este projeto desenvolvido como parte da entrega para a Sprint 3 do Challenge da Tech Mahindra na [FIAP](https://github.com/FIAP) demonstra uma aplicação de Internet das Coisas (IoT) usando o microcontrolador ESP32 para monitorar dados ambientais e a velocidade de um carro de Fórmula E. Utilizando um sensor DHT22 e um Potenciômetro (utilizado para simular um velocímetro), o sistema mede continuamente a temperatura e umidade do ambiente onde se encontra o veículo, bem como sua velocidade atual, enviando esses dados para a nuvem via Wi-Fi, onde são armazenados e analisados em tempo real através do software ThingSpeak.

<br>

<img src="https://blog.indobot.co.id/wp-content/uploads/2022/01/4-2.png" alt="ThingSpeak" width="702px"/>

## Objetivo

O objetivo deste projeto é demonstrar como o uso de sistemas IoT pode ser aplicado no monitoramento inteligente de um veículo de Fórmula E. A aplicação permite o acompanhamento em tempo real das condições climáticas, como temperatura e umidade, além da velocidade do veículo. Esses dados são essenciais para evitar acidentes e otimizar estratégias de corrida, como a troca de pneus e ajustes no desempenho do carro, com base nas condições ambientais. Através da plataforma ThingSpeak, é possível visualizar e analisar esses dados de qualquer lugar, permitindo decisões rápidas durante as corridas.

<br>

<img src="https://github.com/ryanbritodev/esp32-mahindra/blob/main/README.images/charts-thingspeak.png?raw=true" alt="Charts ThingSpeak"/>

## Componentes Utilizados

- **Microcontrolador:** ESP32
- **Sensores:** DHT22 (Temperatura e Umidade) e Potênciometro (Velocidade)
- **Comunicação:** Wi-Fi (Protocolo HTTP)
- **Plataforma de Nuvem:** ThingSpeak

<br>

<img src="https://github.com/ryanbritodev/esp32-mahindra/blob/main/README.images/esp32.png?raw=true" alt="Wokwi Circuit"/>

## Funcionalidades

- Medição contínua da temperatura e umidade ambiente, bem como a velocidade atual do veículo.
- Transmissão dos dados para o canal ThingSpeak.
- Visualização dos dados em tempo real através da plataforma ThingSpeak.
- Interface web para visualização dos dados (utilizando HTML, CSS e JavaScript).

## Estrutura Principal do Projeto

1. **Código-fonte:**
   - Simulação do projeto utilizando o simulador [Wokwi](https://wokwi.com/projects/407869921950393345).
   - Código para o microcontrolador ESP32 para coletar dados do sensor DHT22 e do Potênciometro e enviá-los para o ThingSpeak via Wi-Fi.

3. **ThingSpeak:**
   - Canal no ThingSpeak para receber, armazenar e exportar os dados de temperatura, umidade e temperatura.
     
4. **Interface Web:**
   - Uma página web com HTML, CSS e JavaScript para exibir os dados em tempo real, incluindo:
     - Interface gráfica front-end, utilizando ícones, imagens e HTML semântico.
     - Dados de temperatura.
     - Dados de umidade.
     - Dados da velocidade atual do veículo.
     - Informações em tempo real atualizadas a cada 15 segundos.

## Requisitos

- **Hardware:**
  - Microcontrolador ESP32.
  - Sensor DHT22.
  - Potênciometro ou Velocímetro.
  - Acesso à rede Wi-Fi.

- **Software:**
  - Arduino IDE ou similar para programação do ESP32.
  - Conta no ThingSpeak para criar e gerenciar o canal, bem como seus campos.

## Configuração

### 1. Configuração do ESP32

- **Bibliotecas Necessárias:**
  - `DHT` - Biblioteca para o sensor DHT22.
  - `WiFi` - Biblioteca para a conexão Wi-Fi.
  - `HTTPClient` - Biblioteca para comunicação HTTP.

# Código-Fonte utilizado no projeto:

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
2. Crie um novo canal e anote sua respectiva API Key para uso no código do ESP32.
3. Adicione três campos:
   - **Field1:** Temperatura (°C)
   - **Field2:** Umidade (%)
   - **Field3:** Velocidade (km/h)
   
## 3. Interface Web

- **Arquivos:**
  - `index.html`: Arquivo HTML para exibição dos dados.
  - `style.css`: Arquivo CSS para estilização da página.
  - `script.js`: Arquivo para tratamento dos dados recebidos da API key.

- **Link da Página:** [Monitoramento de Temperatura e Umidade](https://ryanbritodev.github.io/esp32-mahindra/src/html/index.html)

## Créditos

### Gostaríamos de agradecer à __FIAP__, à __Tech Mahindra__ e ao nosso professor pela oportunidade incrível de realizar esse projeto e também aos participantes desse trabalho.
- Prof. Paulo Marcotti PF2150
- Arthur Cotrick Pagani RM554510
- Diogo Leles Franciulli RM558487
- Felipe Sousa de Oliveira RM559085
- Ryan Brito Pereira Ramos RM554497

## Licença

Este projeto está licenciado sob a Licença MIT - veja o [LICENSE](LICENSE) para mais detalhes.
