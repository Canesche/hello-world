#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (9, 10);
byte addresses[][6] = {"0"};

struct package
{
  int id=1;
  float temperature = 18.3;
  char  text[100] = "Maiquim aqui!!!!";
};

typedef struct package Package;
Package data;

void setup()
{
  Serial.begin(115200);//115200
  delay(1000);
  myRadio.begin();  
  myRadio.setChannel(98); // configura o canal
  myRadio.setPALevel(RF24_PA_MIN);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe(addresses[0]);
  myRadio.openReadingPipe(1,addresses[0]);

  myRadio.startListening();
   
  //myRadio.openWritingPipe( addresses[0]);
  delay(1000);
}

void loop(){

  // enviando
  data.id = data.id + 1;
  data.temperature = data.temperature+0.1;

  if(Serial.available()){
    
    char text1[100];
    String tmp = Serial.readString();
    Serial.print("Enviei: ");
    Serial.println(tmp);
    char p[100];
    tmp.toCharArray(p, 100);
    for(int i = 0; i < 100; i++)
      data.text[i] = p[i];
    myRadio.write(&data, sizeof(data));
  }

  myRadio.startListening();
  delay(500);
  // recebendo
  if (myRadio.available()){
    while (myRadio.available()){
      myRadio.read( &data, sizeof(data) );
    }
    Serial.print("Recebi: ");
    Serial.println(data.text);
    
  }
  myRadio.stopListening();
  
  
  delay(500);

}
