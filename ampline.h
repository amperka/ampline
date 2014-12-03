#ifndef AMPERKA_LINE_H
#define AMPERKA_LINE_H

class AmperkaLine
{
public:
  AmperkaLine (
               int pin,
               unsigned long baseDelay = 1000,
               byte msgLen = 8,
               byte crcLen = 2,
               word polynom = 0,
               word timeout = 100
              );

  void send(unsigned long msg);
  int receive(unsigned long *msg);

private:
  void line_raise(void);
  void line_fall(void);
  bool line_pin(void);

protected:
  int m_pin;
  unsigned long m_baseDelay;
  byte m_msgLen;
  byte m_crcLen;
  word m_polynom;
  unsigned long m_timeout;
};

#endif

