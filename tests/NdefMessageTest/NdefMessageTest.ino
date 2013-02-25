#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>
#include <Ndef.h>
#include <ArduinoUnit.h>

TestSuite suite;

// Custom Assertion
void assertNoLeak(Test& __test__, void (*callback)())
{
  int start = freeMemory();
  (*callback)();
  int end = freeMemory();
  assertEquals(0, (start - end));
}

void assertBytesEqual(Test& __test__, const uint8_t* expected, const uint8_t* actual, int size) {
  for (int i = 0; i < size; i++) {
    Serial.print("> ");Serial.print(expected[i]);Serial.print(" ");Serial.println(actual[i]);
    assertEquals(expected[i], actual[i]);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("\n");
  Serial.println(F("========="));
  Serial.println(freeMemory());
  Serial.println(F("========="));
}

test(memoryKludgeEnd)
{
  // TODO ensure the output matches start
  Serial.println(F("========="));
  Serial.print("End ");Serial.println(freeMemory());
  Serial.println(F("========="));
}

test(messageDelete)
{
  int start = freeMemory();

  NdefMessage* m1 = new NdefMessage();
  m1->addTextRecord("Foo");
  delete m1;

  int end = freeMemory();
//  Serial.print("Start ");Serial.println(start);
//  Serial.print("End ");Serial.println(end);
  assertEquals(0, (start-end));
}


test(assign)
{
  int start = freeMemory();
  
  NdefMessage* m1 = new NdefMessage();
  m1->addTextRecord("We the People of the United States, in Order to form a more perfect Union...");
  
  NdefMessage* m2 = new NdefMessage();
  
  *m2 = *m1;
  
  NdefRecord r1 = m1->get(0);
  NdefRecord r2 = m2->get(0);
  
  assertEquals(r1.getTnf(), r2.getTnf());
  assertEquals(r1.getTypeLength(), r2.getTypeLength());
  assertEquals(r1.getPayloadLength(), r2.getPayloadLength());
  assertEquals(r1.getIdLength(), r2.getIdLength());

  uint8_t* p1 = r1.getPayload();
  uint8_t* p2 = r2.getPayload();  
  int size = r1.getPayloadLength();
  assertBytesEqual(__test__, p1, p2, size);
  free(p1);
  free(p2);

  delete m2;
  delete m1;
  
  int end = freeMemory();
  assertEquals(0, (start-end));
}

test(assign2)
{
  int start = freeMemory();
  
  NdefMessage m1 = NdefMessage();
  m1.addTextRecord("We the People of the United States, in Order to form a more perfect Union...");
  
  NdefMessage m2 = NdefMessage();
  
  m2 = m1;
  
  NdefRecord r1 = m1.get(0);
  NdefRecord r2 = m2.get(0);
  
  assertEquals(r1.getTnf(), r2.getTnf());
  assertEquals(r1.getTypeLength(), r2.getTypeLength());
  assertEquals(r1.getPayloadLength(), r2.getPayloadLength());
  assertEquals(r1.getIdLength(), r2.getIdLength());

  // TODO check type

  uint8_t* p1 = r1.getPayload();
  uint8_t* p2 = r2.getPayload();  
  int size = r1.getPayloadLength();
  assertBytesEqual(__test__, p1, p2, size);
  free(p1);
  free(p2);

  int end = freeMemory();
  //assertEquals(0, (start-end));
}

// TODO fix this
// NdefMessage copy constructor and assignment
test(assign3)
{
  int start = freeMemory();
  
  NdefMessage* m1 = new NdefMessage();
  m1->addTextRecord("We the People of the United States, in Order to form a more perfect Union...");
  
  NdefMessage* m2 = new NdefMessage();
  
  *m2 = *m1;
  
  delete m1;
  
  NdefRecord r2 = m2->get(0);
  
  assertEquals(TNF_WELL_KNOWN, r2.getTnf());
  assertEquals(1, r2.getTypeLength());
  assertEquals(79, r2.getPayloadLength());
  assertEquals(0, r2.getIdLength());
  
  String s = "We the People of the United States, in Order to form a more perfect Union...";
  byte payload[s.length() + 1];
  s.getBytes(payload, sizeof(payload));

  uint8_t* p2 = r2.getPayload();  
  int size = r2.getPayloadLength();
  assertBytesEqual(__test__, payload, p2+3, s.length());
  free(p2);

  delete m2;
  
  int end = freeMemory();
  assertEquals(0, (start-end));
}

  
test(memoryKludgeStart)
{
  Serial.println(F("---------"));
  Serial.print("Start ");Serial.println(freeMemory());
  Serial.println(F("---------"));
}

void loop() {
  suite.run();  
}