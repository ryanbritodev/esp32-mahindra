# Monitoramento de Temperatura e Umidade com ESP32 e IoT

Este projeto criado como parte da entrega da Sprint 3 para o Challenge da Tech Mahindra na FIAP demonstra uma aplicação de Internet das Coisas (IoT) utilizando o microcontrolador ESP32 para monitorar dados ambientais. O sistema mede continuamente a temperatura e a umidade usando um sensor DHT22 e envia os dados para a nuvem via Wi-Fi.

Os dados são transmitidos para este canal do ThingSpeak, onde são armazenados, visualizados e analisados em tempo real. O objetivo deste projeto é demonstrar as capacidades da tecnologia IoT na criação de um sistema de monitoramento inteligente que pode ser acessado de qualquer lugar.

Requisitos:
- Microcontrolador: ESP32
- Sensor: DHT22 (Temperatura e Umidade)
- Comunicação: Wi-Fi (Protocolo HTTP)
- Plataforma de Nuvem: ThingSpeak

Campos de Dados:
- Campo 1: Temperatura (°C)
- Campo 2: Umidade (%)
