#ifndef AMPERKA_LINE_H
#define AMPERKA_LINE_H

class AmperkaLine
{
public:
  AmperkaLine (
               int pin,
               word baseDelay = 100,
               byte msgLen = 8,
               byte crcLen = 2,
               word polynom = 0,
               word timeout = 100
              );

  bool send(const void *msg);
  bool receive(void *msg);

protected:
  int m_pin;
  word m_baseDelay;
  byte m_msgLen;
  byte m_crcLen;
  word m_polynom;
};

#endif

