// Constantes globais
#define SENSOR_PIN_1 A0             // Pino analógico do primeiro sensor (A0)
#define SENSOR_PIN_2 A1             // Pino analógico do segundo sensor (A1)
#define VOLTAGE_REF 5.0             // Tensão de referência do Arduino (em Volts)
#define RESOLUTION 1024             // Resolução do ADC (10 bits)
#define SENSOR_OFFSET 0.5           // Offset do sensor (em Volts)
#define CONVERSION_FACTOR 100.0     // Fator de conversão do sensor (10mV/°C)
#define MAX_RECORDS 10              // Capacidade máxima do pseudo banco de dados
#define DELAY_TIME 20000            // Intervalo de tempo entre as leituras (em milissegundos) - 20 segundos

// Estrutura para armazenar dados de temperatura
struct TempData {
    float temperaturaC_1; // Mudança para portar os 2 sensores 
    float temperaturaF_1; // Mudança para portar os 2 sensores 
    float temperaturaC_2; // Mudança para portar os 2 sensores 
    float temperaturaF_2; // Mudança para portar os 2 sensores 
    unsigned long timestamp;
};

// Banco de dados do armazenamento das temperaturas de motor e freio
TempData tempDB[MAX_RECORDS];
int recordIndex = 0;  // Índice de armazenamento no banco de dados

// funções
float lerSensor(int sensorPin);
float calcularTemperaturaC(float valorSensor);
float calcularTemperaturaF(float temperaturaC);
void armazenarDados(float temperaturaC_1, float temperaturaF_1, float temperaturaC_2, float temperaturaF_2);
void enviarDadosIoT();
void exibirBancoDados();

// Função de inicialização de telemetria
void setup() {
    pinMode(SENSOR_PIN_1, INPUT);   // Configura o pino do primeiro sensor do motor como entrada
    pinMode(SENSOR_PIN_2, INPUT);   // Configura o pino do segundo sensor do freio como entrada
    Serial.begin(9600);             // Inicializa a comunicação serial entre sensores e arduino microcontrolador
    Serial.println("Iniciando sistema de monitoramento de temperatura...");
}

// Função principal de loop
void loop() {
    // ler os valores dos sensores
    float valorSensor1 = lerSensor(SENSOR_PIN_1);
    float valorSensor2 = lerSensor(SENSOR_PIN_2);
    
    // Calcula as temperaturas em Celsius e Fahrenheit para os sensores de motor e freios
    float temperaturaC_1 = calcularTemperaturaC(valorSensor1);
    float temperaturaF_1 = calcularTemperaturaF(temperaturaC_1);
    float temperaturaC_2 = calcularTemperaturaC(valorSensor2);
    float temperaturaF_2 = calcularTemperaturaF(temperaturaC_2);

    // Armazena os dados no banco de dados
    armazenarDados(temperaturaC_1, temperaturaF_1, temperaturaC_2, temperaturaF_2);

    // faz o envio dos dados para um servidor de telemetria
    enviarDadosIoT();

    // Exibe o banco de dados completo com telemetria
    exibirBancoDados();

    // Aguardando antes da próxima leitura
    delay(DELAY_TIME);  // O delay foi ajustado para 20 segundos
}

// Função para ler o valor do sensor de temperatura
float lerSensor(int sensorPin) {
    return analogRead(sensorPin);
}

// Função para calcular a temperatura em Celsius
float calcularTemperaturaC(float valorSensor) {
    float voltage = valorSensor * (VOLTAGE_REF / RESOLUTION);  // Converte o valor para tensão
    return (voltage - SENSOR_OFFSET) * CONVERSION_FACTOR;      // Calcula e retorna a temperatura em °C
}

// Função para calcular a temperatura em Fahrenheit
float calcularTemperaturaF(float temperaturaC) {
    return (temperaturaC * 1.8) + 32;  // Converte de Celsius para Fahrenheit
}

// Função para armazenar os dados no pseudo banco de dados
void armazenarDados(float temperaturaC_1, float temperaturaF_1, float temperaturaC_2, float temperaturaF_2) {
    // Armazena os dados no índice atual
    tempDB[recordIndex].temperaturaC_1 = temperaturaC_1; //ajustado para os 2 sensores
    tempDB[recordIndex].temperaturaF_1 = temperaturaF_1;
    tempDB[recordIndex].temperaturaC_2 = temperaturaC_2;
    tempDB[recordIndex].temperaturaF_2 = temperaturaF_2;
    tempDB[recordIndex].timestamp = millis();  // Armazena o tempo em milissegundos

    // Atualiza o índice do banco de dados em ciclos
    recordIndex = (recordIndex + 1) % MAX_RECORDS;
}

// Função para simular o envio dos dados para um servidor da equipe de telemetria
void enviarDadosIoT() {
    Serial.println("Enviando dados para o servidor para telemetria");

    // Exibe as temperaturas de cada sensor de forma separada
    Serial.print("Sensor 1 (Motor) - Temperatura (°C): ");
    Serial.print(tempDB[recordIndex].temperaturaC_1);
    Serial.print(", Temperatura (°F): ");
    Serial.println(tempDB[recordIndex].temperaturaF_1);

    Serial.print("Sensor 2 (Freios) - Temperatura (°C): ");
    Serial.print(tempDB[recordIndex].temperaturaC_2);
    Serial.print(", Temperatura (°F): ");
    Serial.println(tempDB[recordIndex].temperaturaF_2);

    Serial.print("Timestamp: ");
    Serial.println(tempDB[recordIndex].timestamp);
}

// Função para exibição da Telemetria do carro simulado 
void exibirBancoDados() {
    Serial.println("Exibindo dados de telemetria:");
    for (int i = 0; i < MAX_RECORDS; i++) {
        Serial.print("Registro ");
        Serial.print(i + 1);
        Serial.print(" - Sensor 1 (Motor) - Celsius: ");
        Serial.print(tempDB[i].temperaturaC_1);
        Serial.print(", Fahrenheit: ");
        Serial.print(tempDB[i].temperaturaF_1);
        Serial.print(" | Sensor 2 (Freios) - Celsius: ");
        Serial.print(tempDB[i].temperaturaC_2);
        Serial.print(", Fahrenheit: ");
        Serial.print(tempDB[i].temperaturaF_2);
        Serial.print(", Timestamp: ");
        Serial.println(tempDB[i].timestamp);
    }
}
