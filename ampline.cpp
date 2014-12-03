#include <Arduino.h>
#include <ampline.h>


AmperkaLine::AmperkaLine (
           int pin,
           unsigned long baseDelay,
           byte msgLen,
           byte crcLen,
           word polynom,
           word timeout
          )
          : m_pin(pin),
            m_baseDelay(baseDelay),
            m_msgLen(msgLen),
            m_crcLen(crcLen),
            m_polynom(polynom),
            m_timeout(timeout)
{
}

void AmperkaLine::line_raise(void) {
  digitalWrite(m_pin, HIGH);
}

void AmperkaLine::line_fall(void) {
  digitalWrite(m_pin, LOW);
}

bool AmperkaLine::line_pin(void) {
  return digitalRead(m_pin);
}

void AmperkaLine::send(unsigned long msg)
{
    byte crc;

    pinMode(m_pin, OUTPUT);

    line_raise();
    delayMicroseconds(70*m_baseDelay);
    line_fall();
    delayMicroseconds(10*m_baseDelay);
    line_raise();
   
    crc = 0x00;
   
    for(int i = 0; i < 32; i++)
    {
        if(bitRead(msg, i))
        {
            delayMicroseconds(m_baseDelay);
        }
        else
        {
            delayMicroseconds(3*m_baseDelay);
        }
        line_fall();
        delayMicroseconds(m_baseDelay*1.5);
        line_raise();
    }
   
    crc ^= msg&0xFF;
    crc ^= ((msg&0xFF00)>>8);
    crc ^= ((msg&0xFF0000)>>16);
    crc ^= ((msg&0xFF000000)>>24);
   
    for(int i = 0; i < 8; i++)
    {
        if(bitRead(crc, i))
        {
            delayMicroseconds(m_baseDelay);
        }
        else
        {
            delayMicroseconds(3*m_baseDelay);
        }

        line_fall();
        delayMicroseconds(m_baseDelay*1.5);
        line_raise();
    }
   
    line_fall();
}

int AmperkaLine::receive(unsigned long *msg)
{
    unsigned long tm;
    byte crc, rcrc;

    pinMode(m_pin, INPUT);
   
    do
    {
        while(!line_pin());
        tm = micros();
        while(line_pin());
    }
    while(!((micros() - tm) > 45*m_baseDelay && (micros() - tm) < 72*m_baseDelay));
   
    crc = 0;
   
    while(!line_pin());
    tm = micros();
   
    for(int i = 0; i < 32; i++)
    {
        while(line_pin());
     
        if(abs(micros() - tm) < 1.5*m_baseDelay)
        {
            bitSet((*msg), i);
        }
        else if(abs(micros() - tm) < 4*m_baseDelay)
        {
            bitClear((*msg), i);
        }
        else return 2;
     
        while(!line_pin());
        tm = micros();
    }
   
    crc ^= (*msg)&0xFF;
    crc ^= (((*msg)&0xFF00)>>8);
    crc ^= (((*msg)&0xFF0000)>>16);
    crc ^= (((*msg)&0xFF000000)>>24);
    rcrc = 0;
   
    for(int i = 0; i < 8; i++)
    {
        while(line_pin());
   
        if(abs(micros() - tm) < 1.5*m_baseDelay)
        {
            rcrc |= bit(i);
        }
        else if(abs(micros() - tm) < 4*m_baseDelay)
        {
            rcrc &= ~bit(i);
        }
        else return 2;
   
        while(!line_pin());
        tm = micros();
    }
   
    if(rcrc != crc) return 3;
   
    return 0;
}

