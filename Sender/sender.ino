#include <OneWire.h>
#include <DallasTemperature.h>
#include <VirtualWire.h>

#define ONE_WIRE_BUS 7
#define TRANSMIT 8

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

union float2bytes
{
  float f;
  uint8_t bytes[4];
};

uint8_t m_Counter;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  vw_set_ptt_inverted(true);
  vw_set_tx_pin(TRANSMIT);
  vw_setup(4000);

  Serial.begin(9600);
  sensors.begin();

  m_Counter = 0;

}

// the loop function runs over and over again forever
void loop() {

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(500);

  sensors.requestTemperatures();

  float2bytes f2b;
  f2b.f = sensors.getTempCByIndex(0);

  uint8_t data[5];
  memcpy(data, f2b.bytes, 4);
  data[4] = m_Counter;

  for (int i = 0; i < 3; i++)
  {
    vw_send(data, 5);
    vw_wait_tx();
  }
  
  m_Counter++;
  Serial.println(f2b.f);
}
