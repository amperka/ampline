const int rfpin = 5;


void rf_raise() {
  digitalWrite(rfpin, HIGH);
}

void rf_fall() {
  digitalWrite(rfpin, LOW);
}

bool rf_pin() {
  return digitalRead(rfpin);
}

void rf_putdw(unsigned long dw)
{
  byte crc, *dt = (byte*)dw;

  rf_raise();
  delay(20);
  rf_fall();
  delay(5);
  rf_raise();

  crc = 0x00;

  for(int i =0; i < 32; i++) {
    if(_BIT(dw, i)) {
      delay(5);
    } else {
      delay(10);
    }
    rf_fall();
    delay(3);
    rf_raise();
  }

  for(int i = 0; i < 8; i++) {
    if(_BIT(crc, i)) {
      delay(5);
    } else {
      delay(10);
    }
    rf_fall();
    delay(3);
    rf_raise();
  }

  rf_fall();
}

bool rf_getdw(unsigned long* dw) {
  unsigned long tm;

  while(!rf_pin());

  tm = millis();

  while(rf_pin());

  if(!(millis() - tm > 15 && millis() - tm < 25)) {
    return 1;
  }

  while(!rf_pin());
  tm = millis();

  for(int i = 0; i < 32; i++) {
    while(rf_pin());

    if(abs(millis() - tm) < 7) {
      *dw |= _BIT(0xFFFFFFFF, i);
    } else if(abs(millis() - tm) < 13) {
      *dw &= ~_BIT(0xFFFFFFFF, i);
    } else {
      return 2;
    }

    while(!rf_pin());
    tm = millis();
  }

  return 0;
}

