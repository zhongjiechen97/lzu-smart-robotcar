/* Functions for various sensor types */
#include <dht.h>
#include <utility.h>

dht DHT;

float microsecondsToCm(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per cm.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

long Pinging(int pin) {
  long duration, range;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pin, INPUT);
  duration = pulseIn(pin, HIGH);

  // convert the time into meters
  range = microsecondsToCm(duration);
  
  return(range);
}

std::pair<float, float> readDHT22(uint8_t dht22_pin)
{
  // Serial.print(dht22_pin);
  // READ DATA
  // Serial.print("DHT22, \t");
  // uint32_t start = micros();
  int chk = DHT.read22(dht22_pin);
  // uint32_t stop = micros();

/*   switch (chk)
  {
  case DHTLIB_OK:
    Serial.print("OK,\t");
    break;
  case DHTLIB_ERROR_CHECKSUM:
    Serial.print("Checksum error,\t");
    break;
  case DHTLIB_ERROR_TIMEOUT:
    Serial.print("Time out error,\t");
    break;
  case DHTLIB_ERROR_CONNECT:
    Serial.print("Connect error,\t");
    break;
  case DHTLIB_ERROR_ACK_L:
    Serial.print("Ack Low error,\t");
    break;
  case DHTLIB_ERROR_ACK_H:
    Serial.print("Ack High error,\t");
    break;
  default:
    Serial.print("Unknown error,\t");
    break;
  } */
  // DISPLAY DATA
  // Serial.print(DHT.humidity, 1);//小数点后取1位
  // Serial.print(",\t");
  // Serial.print(DHT.temperature, 1);
  // Serial.print(",\t");
  // Serial.print(stop - start);
  // Serial.println();
  std::pair<float,float> dht_pair(DHT.humidity,DHT.temperature);
  return dht_pair;
}
