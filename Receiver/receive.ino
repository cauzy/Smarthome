#include <VirtualWire.h>

#define RECEIVE 19

bool m_FirstRecv;
uint8_t m_LastRecv;
uint8_t m_ErrorCnt;

union float2bytes
{
  float f;
  uint8_t bytes[4];
};

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  vw_set_ptt_inverted(true);
  vw_set_rx_pin(RECEIVE);
  vw_setup(4000);

  vw_rx_start();

  digitalWrite(LED_BUILTIN, 1);
  delay(1000);
  digitalWrite(LED_BUILTIN, 0);

  Serial.begin(9600);

  m_LastRecv = 0;
  m_FirstRecv = true;
  m_ErrorCnt = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    if(buflen != 5)
    {
      m_ErrorCnt++;
      return;
    }
    
    if (m_FirstRecv == true)
    {
      m_FirstRecv = false;
      m_LastRecv = buf[4];
      return;
    }

    if (buf[4] == m_LastRecv)
    {
      //check for same message
      return;
    }

    float2bytes f2b;
    memcpy(f2b.bytes, buf, 4);
    
    if (buf[4] != 0 || m_LastRecv != 255)
    {
      if (buf[4] != (m_LastRecv + 1))
      {
        Serial.println("ERR");
        m_ErrorCnt++;
      }
    }

    m_LastRecv = buf[4];
    Serial.print(m_LastRecv);
    Serial.print(", ErrCnt = ");
    Serial.println(m_ErrorCnt);
    Serial.println(f2b.f);
  }

}
