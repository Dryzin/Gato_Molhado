#define BLYNK_TEMPLATE_ID ""************"" 
#define BLYNK_TEMPLATE_NAME "Sensor de Clima"
#define BLYNK_AUTH_TOKEN "************"

// 2. Credenciais de Conexão
char ssid[] = "redewifi";  
char pass[] = "senha";    

// 3. Incluir as bibliotecas
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h> 

// 4. Definições de Pinos e Tipos
#define PINO_RELE D1        // Pino D1 (GPIO 5) para controlar o Relé
#define PINO_DHT D2         // Pino D2 (GPIO 4) para a leitura do DHT11
#define TIPO_DHT DHT11      // Modelo do sensor

// 5. Constantes de Controle
#define UMIDADE_LIMITE 55   // Porcentagem de Umidade Mínima para LIGAR AUTOMATICAMENTE

// 6. Definições de Pinos Virtuais Blynk 
#define V_TEMPERATURA V1    // Envia Temperatura (V1)
#define V_UMIDADE V2        // Envia Umidade (V2 - Crítico para a Automação)
#define V_RELE_MANUAL V3    // Controle manual (Switch/Botão)
#define V_STATUS_RELE V4    // Status do Relé (LED)
#define V_AUTOMACAO_STATUS V5 // Modo de Controle (Automático/Manual)

// 7. Variáveis Globais
DHT dht(PINO_DHT, TIPO_DHT);
BlynkTimer timer; 

bool modoManualAtivo = false; 
bool estadoReléAutomático = false; 

// --- Garante a estabilidade da conexão e sincroniza estados ---
BLYNK_CONNECTED() {
  Serial.println("\n** CONECTADO AO BLYNK. SINCRONIZANDO ESTADOS. **");
  Blynk.syncVirtual(V_RELE_MANUAL); 
}
// -----------------------------------------------------------


// 8. Função de Leitura e Envio de Dados
void enviarDadosDHT() {
  Serial.println("\n----------------------------");
  Serial.println("--- Ciclo de Leitura DHT ---"); 

  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();

  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("ERRO: Falha ao ler o sensor DHT! Verifique a fiação (D2).");
    return;
  }

  // Logs no Serial
  Serial.print("Umidade Lida: "); Serial.print(umidade); Serial.print(" % | Temperatura Lida: "); 
  Serial.print(temperatura); Serial.println(" °C");
  Serial.print("Limite de Umidade: "); Serial.print(UMIDADE_LIMITE); Serial.println(" %");

  // Envia os valores para o Blynk 
  Blynk.virtualWrite(V_UMIDADE, umidade);
  Blynk.virtualWrite(V_TEMPERATURA, temperatura);

 // 3. Lógica de Automação
  if (!modoManualAtivo) {
    Serial.println("MODO DE CONTROLE: AUTOMÁTICO");
    
    if (umidade < UMIDADE_LIMITE) {
      if (!estadoReléAutomático) {
        // CORRIGIDO: LIGA Umidificador (Relé ON = HIGH para ATIVO-ALTO)
        digitalWrite(PINO_RELE, HIGH); 
        estadoReléAutomático = true;
        Serial.println(">>> AÇÃO: LIGANDO RELÉ. Umidade < Limite. (Comando HIGH)");
      } else {
        Serial.println(">>> STATUS: Umidificador LIGADO (Manutenção).");
      }
    } else { // Umidade >= Limite
      if (estadoReléAutomático) {
        // CORRIGIDO: DESLIGA Umidificador (Relé OFF = LOW para ATIVO-ALTO)
        digitalWrite(PINO_RELE, LOW);
        estadoReléAutomático = false;
        Serial.println(">>> AÇÃO: DESLIGANDO RELÉ. Umidade >= Limite. (Comando LOW)");
      } else {
         Serial.println(">>> STATUS: Umidificador DESLIGADO (Manutenção).");
      }
    }
    // Atualiza o widget de status do relé e modo de automação
    Blynk.virtualWrite(V_STATUS_RELE, estadoReléAutomático ? 1 : 0);
    Blynk.virtualWrite(V_AUTOMACAO_STATUS, "AUTOMÁTICO");
  } else {
    // Modo Manual
    Serial.println("MODO DE CONTROLE: MANUAL ATIVO. Automação Suspensa.");
    Blynk.virtualWrite(V_AUTOMACAO_STATUS, "MANUAL");
  }
  Serial.println("----------------------------");
}

// 9. Função para Ligar/Desligar o Relé (usada pelo controle manual)
void controleRelé(bool estado) {
    // CORRIGIDO: Relé ATIVO-ALTO: estado = true (ligado) -> HIGH no pino
    digitalWrite(PINO_RELE, estado ? HIGH : LOW);
    Blynk.virtualWrite(V_STATUS_RELE, estado ? 1 : 0); 
    estadoReléAutomático = estado; 
}

// 10. Função para o Controle Manual (recebe dados do Blynk - V3)
BLYNK_WRITE(V_RELE_MANUAL) {
  int estadoManual = param.asInt();
  
  modoManualAtivo = true; 

  if (estadoManual == 1) {
    controleRelé(true); 
    Serial.println("\n** COMANDO MANUAL: LIGAR (V3) **");
  } else {
    controleRelé(false);
    Serial.println("\n** COMANDO MANUAL: DESLIGAR (V3) **");
  }
  Blynk.virtualWrite(V_AUTOMACAO_STATUS, "MANUAL"); 
}

// 11. Função para voltar ao modo Automático (recebe dados do Blynk - V5)
BLYNK_WRITE(V_AUTOMACAO_STATUS) {
    if (param.asInt() == 1) { 
        modoManualAtivo = false;
        Serial.println("\n** MODO AUTOMÁTICO REATIVADO (V5) **");
        Blynk.virtualWrite(V_AUTOMACAO_STATUS, "AUTOMÁTICO"); 
    }
}


void setup() {
  Serial.begin(115200);
  Serial.println("\n--- Iniciando Sistema Umidificador Automático ---");
  
  // Configuração inicial do Relé
  pinMode(PINO_RELE, OUTPUT);
  // CORRIGIDO: DESLIGA por segurança (LOW para ATIVO-ALTO)
  digitalWrite(PINO_RELE, LOW); 
  Serial.println("PINO D1 (Relé) CONFIGURADO. Inicialmente DESLIGADO (LOW).");

  // Inicia o DHT
  dht.begin();
  Serial.println("Sensor DHT11 (D2) INICIADO.");

  // Conecta ao Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.print("Conectando ao Wi-Fi (");
  Serial.print(ssid);
  Serial.println(") e Servidor Blynk...");

  // Agenda a leitura do sensor e a lógica de automação para rodar a cada 5 segundos
  timer.setInterval(5000L, enviarDadosDHT); 
}

void loop() {
  Blynk.run();
  timer.run();
}