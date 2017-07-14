// Do not remove the include below
#include "Sender.h"
#include "Common.h"

#include <OneWire.h>
#include <DallasTemperature.h>
#include <VirtualWire.h>

#define ONE_WIRE_BUS 7
#define TRANSMIT 8

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

uint8_t m_Counter;

//The setup function is called once at startup of the sketch
void setup()
{
	// initialize digital pin LED_BUILTIN as an output.
	pinMode(LED_BUILTIN, OUTPUT);

	vw_set_ptt_inverted(true);
	vw_set_tx_pin(TRANSMIT);
	vw_setup(4000);

	Serial.begin(9600);
	sensors.begin();

	m_Counter = 0;
}

// The loop function is called in an endless loop
void loop()
{
	digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(500);                       // wait for a second
	digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
	delay(500);

	sensors.requestTemperatures();

	Message message;
	message.Temperature = sensors.getTempCByIndex(0);
	message.CycleCount = m_Counter;
	message.Address = 1;

	uint8_t data[sizeof(Message)];

	memcpy(data, &message, sizeof(Message));

	for (int i = 0; i < 3; i++)
	{
		vw_send(data, sizeof(Message));
		vw_wait_tx();
	}

	m_Counter++;
}
