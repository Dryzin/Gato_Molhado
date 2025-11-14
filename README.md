# 💧 Gato_Molhado: Umidificador de Ambiente Inteligente (ESP8266 + Blynk)

## 💡 Apresentação do Projeto

O **Gato_Molhado** é um sistema de IoT (Internet das Coisas) desenvolvido para **monitorar e controlar ativamente a umidade de um ambiente**, como um terrário, estufa ou quarto.

Utilizando um microcontrolador **ESP8266** e a plataforma **Blynk**, o projeto combina leitura de dados em tempo real com lógica de automação para manter a umidade dentro de uma faixa ideal, acionando um umidificador (via relé) sempre que necessário.

## O principal objetivo do projeto é:

1.  **Garantir o Conforto/Saúde Ambiental:** Manter níveis de umidade (e temperatura) saudáveis ou ideais para um determinado ambiente, de forma autônoma.
2.  **Oferecer Controle Híbrido:** Permitir que o usuário utilize o sistema no modo totalmente automático (baseado em limite) ou assuma o controle manual a qualquer momento via smartphone.
3.  **Visualização Remota:** Apresentar os dados de clima e o status do sistema em tempo real através do aplicativo Blynk.

![Dashboard](/Gato_Molhado/dashboard.png)

## O que o Projeto Realiza (Funcionalidades)

| Funcionalidade | Descrição Detalhada |
| :--- | :--- |
| **Monitoramento de Clima** | Lê e envia a **Temperatura** (em °C) e a **Umidade Relativa** (em %) do ambiente para o aplicativo Blynk a cada 5 segundos. |
| **Automação Inteligente** | Opera no **Modo Automático**. Liga o umidificador (relé) se a umidade cair **abaixo do limite de 55%** (valor configurável) e o desliga quando a umidade é restabelecida. |
| **Controle Manual Remoto** | O usuário pode anular a automação a qualquer momento usando um *Switch* no Blynk para **Ligar ou Desligar o Umidificador** imediatamente. |
| **Indicador de Status** | O aplicativo exibe o **Modo de Controle** atual ("AUTOMÁTICO" ou "MANUAL") e um LED (V4) indica se o relé está atualmente **LIGADO** ou **DESLIGADO**. |
| **Sincronização de Estado** | O projeto garante que o estado do relé (LIGADO/DESLIGADO) seja sincronizado entre o hardware e o aplicativo, especialmente ao conectar ou alternar modos. |

## Arquitetura do Sistema

O sistema é baseado em quatro pilares:

1.  **Sensor (DHT11):** Coleta os dados ambientais (Temperatura e Umidade).
2.  **Microcontrolador (ESP8266):** Executa a lógica de automação e gerencia a conexão Wi-Fi.
3.  **Atuador (Módulo de Relé):** Atua como um interruptor digital para ligar ou desligar o umidificador de ambiente.
4.  **Plataforma (Blynk):** Fornece a interface de usuário (Dashboard) e o servidor para comunicação remota via Internet.

### Mapeamento de Pinos Principais

| Componente | Pino do ESP8266 (NodeMCU) | Função |
| :--- | :--- | :--- |
| **Relé** (Sinal) | **D1** (GPIO 5) | Acionamento do Umidificador |
| **Sensor DHT11** (Dados) | **D2** (GPIO 4) | Leitura de Clima |

---

## Próximos Passos

Para começar a usar este projeto, você precisará:

1.  Montar o circuito (ESP8266, DHT11 e Relé).
2.  Configurar o ambiente no Arduino IDE (bibliotecas Blynk e DHT).
3.  Personalizar as credenciais de Wi-Fi e Blynk no código.
4.  Realizar o upload do código para o ESP8266.