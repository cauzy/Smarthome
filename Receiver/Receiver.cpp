// Do not remove the include below
#include "Receiver.h"
#include "Common.h"

#include <VirtualWire.h>

#define RECEIVE 19

bool m_FirstRecv;
uint8_t m_LastRecv;
uint8_t m_ErrorCnt;

//The setup function is called once at startup of the sketch
void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);

	vw_set_ptt_inverted(true);
	vw_set_rx_pin(RECEIVE);
	vw_setup(4000);

	vw_rx_start();

	digitalWrite(LED_BUILTIN, 1);
	delay(1000);
	digitalWrite(LED_BUILTIN, 0);

	Serial.begin(9600);
	Serial.println("Receiver up and running!");

	m_LastRecv = 0;
	m_FirstRecv = true;
	m_ErrorCnt = 0;
}

// The loop function is called in an endless loop
void loop()
{
	// put your main code here, to run repeatedly:
	uint8_t buflen = VW_MAX_MESSAGE_LEN;

	Message message;
	uint8_t data[sizeof(Message)];

	if (vw_get_message(data, &buflen)) // Non-blocking
	{
		memcpy(&message, data, sizeof(Message));

		if(buflen != sizeof(Message))
		{
			m_ErrorCnt++;
			return;
		}

		if (m_FirstRecv == true)
		{
			m_FirstRecv = false;
			m_LastRecv = message.CycleCount;
			return;
		}

		if (message.CycleCount == m_LastRecv)
		{
			//check for same message
			return;
		}

		if (message.CycleCount != 0 || m_LastRecv != 255)
		{
			if (message.CycleCount != (m_LastRecv + 1))
			{
				Serial.println("ERR");
				m_ErrorCnt++;
			}
		}

		m_LastRecv = message.CycleCount;;

		char buffer[256];
		sprintf(buffer, "Address: %d, CyleCount: %d, Temp: %f", message.Address, message.CycleCount, message.Temperature);

		Serial.println(buffer);
	}

}
