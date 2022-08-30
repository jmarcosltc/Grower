/*
 João Marcos Carneiro

 This code is part of the EasyGrow(tm) automatic grower automation system

 (c) Copyright 2022 - João Marcos & Antonio Victor / EasyGrow(TM)
 This code is copyrighted and is under an MIT License. You are not allowed to use it commercially
 without explicit written approval.

 GitHub: https://github.com/jmarcosltc
 */

#include <dht.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Variáveis de tempo ---

/*
 * Previous millis: variável para definir o tempo de começo de um ciclo
 * Current millis: tempo atual do ciclo
 * Intevalo: Tempo de duração do ciclo
 */

unsigned long previousMillis = 0;
unsigned long segundoPreviousMillis = 0;
unsigned long currentMillis;
unsigned long segundoCurrentMillis;
const unsigned long intervalo = 7000;
const unsigned long segundoIntervalo = 800;
// --- Variáveis de tempo ---

// --- Variáveis do display ---
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET 4     // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int displayUmidade;
int percentageMoisture;
// --- Variáveis do display ---

// --- Variáveis do DHT11 ---
const int dhtPin = 5;
#define dhtType DHT11

float temperatura = 0x00;
float umidade = 0x00;

dht my_dht;
// --- Variáveis do DHT11 ---

// --- Variáveis do Mositure ---
float moisturePin = A1;
float moistureValue = 0;
float moisture = 0;

const int WET = 489;  // molhado
const int DRY = 1020; // seco
// --- Variáveis do Mositure ---

void setup()
{
  // setup
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  Serial.println("Feito setup");
}

void loop()
{

  currentMillis = millis();
  segundoCurrentMillis = millis();
  delay(10);

  /*
   * Se o tempo atual menos o tempo de início do ciclo for maior ou igual ao intervalo,
   * ele executa uma tarefa dentro do bloco de código. Se não,
   * ele passa para a próxima tarefa.
   *
   * Ex: Intervalo de 1 segundo
   * 512ms - 0 = 512ms
   * Como só se passou meio segundo, ele passa para a próxima tarefa
   * Na volta do loop:
   * 1024 - 0 = 1024ms
   * 1024 >= 1000ms
   * Ele irá rodar o bloco de código, e passará para a próxima tarefa.
   */

  if (currentMillis - previousMillis >= intervalo)
  {

    // --- Moisture Sensor OUTPUT ---
    moisture = moistureMain();

    // --- Irrigation OUTPUT ---
    pumpMain(moisture);

    // Salvar a última vez que o comando foi executado
    previousMillis = currentMillis;
  }

  if (segundoCurrentMillis - segundoPreviousMillis >= segundoIntervalo)
  {
    // --- Display OUTPUT ---
    displayMain();
    // --- Display OUTPUT ---
    segundoPreviousMillis = segundoCurrentMillis;
  }
}

// Display
// float temp, float moisture, float umidity
int displayMain()
{

  display.clearDisplay();
  delay(1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(F("Temp: "));
  display.print(dhtTempMain());
  display.print(F(" C"));
  display.setCursor(0, 8);
  display.print(F("Umidade: "));
  display.print(dhtUmidMain());
  display.print(F("%"));

  displayUmidade = moistureMain();
  percentageMoisture = map(displayUmidade, WET, DRY, 100, 0);

  display.setCursor(0, 16);
  display.print(F("Umidade da terra: "));
  display.print(percentageMoisture);
  display.print(F("%"));
  display.display();
  delay(200);
}

// Irrigation
int displayPump()
{
}

int pumpMain(float moistureValue)
{

  while (moistureValue >= 590 && moistureValue <= 936)
  {
    // acionar a pump e começar processo de irrigação
  }

  if (moistureValue < 590)
  {
    Serial.println("Terra molhada.");
  }
  else if (moistureValue >= 590 && moistureValue < 936)
  {
    Serial.println("Terra umida.");
  }
  else
  {
    Serial.println("Terra seca.");
  }
  delay(200);

  return 0;
}

// Moisture Function
int moistureMain()
{

  moistureValue = analogRead(moisturePin);
  delay(200);

  return moistureValue;
}

// DHT11 Functions
float dhtTempMain()
{
  my_dht.read11(dhtPin);
  temperatura = my_dht.temperature;

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");

  Serial.println("Aguardando nova medição de temperatura e umidade do ar...");
  delay(10);

  return temperatura;
}

float dhtUmidMain()
{
  my_dht.read11(dhtPin);
  umidade = my_dht.humidity;

  Serial.print("Umidade: ");
  Serial.println(umidade);
  delay(10);

  return umidade;
}
