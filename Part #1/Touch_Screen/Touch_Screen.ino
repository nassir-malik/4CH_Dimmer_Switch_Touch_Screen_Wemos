#include <Nextion.h>
#include <EEPROM.h>
#define USE_SERIAL  Serial

SoftwareSerial nextion(12, 13);//TX->D6, RX->D7 (Wemos Di mini)
Nextion myNextion(nextion, 9600);


int selectedDimmer;
int dimmerlvl1; int dimmerlvl2; int dimmerlvl3; int dimmerlvl4;
struct dimmers {int   dimmerlvl1; int   dimmerlvl2; int   dimmerlvl3; int   dimmerlvl4; };
int outVal = 0;
int dimminglevel=0;
//###########################       Setup   ###########################
void setup() {
  USE_SERIAL.begin(115200); 
  EEPROM.begin(512);
} 
//###########################       Loop   ###########################
void loop() {
    String message = myNextion.listen(); //check for message
  if (message != "") { // if an input recieved from touch screen process it
    int str_len = message.length() + 1;
    char char_array[str_len];
    message.toCharArray(char_array, str_len);
    int switchid = (int) strtol(&char_array[5], 0, 16);
    Serial.println(message);
    processTouchScreenCommand(switchid);
  }


}

//###########################       Supported functions  ###########################

 void savetoeeprom() {
//int dimmingLevel = myNextion.getComponentValue("h0");
Serial.print("Writtening dim level!=>");
Serial.print(dimminglevel);
Serial.println(selectedDimmer);
switch (selectedDimmer) {
    case 1:
      dimmerlvl1=dimminglevel;
      break;
    case 2:
      dimmerlvl2=dimminglevel;
      break;
    case 3:
      dimmerlvl3=dimminglevel;
      break;
    case 4:
      dimmerlvl4=dimminglevel;
      break;
}

  
  Serial.println("Writtening thermostate data to EEPROM!");
  //Data to store.
dimmers customVar ={
       dimmerlvl1,
       dimmerlvl2,
       dimmerlvl3,
       dimmerlvl4,
};
  EEPROM.put(0, customVar);
  EEPROM.commit();
}
//######################################################
void readfromeeprom() {
  dimmers customVar; //Variable to store custom object read from EEPROM.
  EEPROM.get( 0, customVar );

  dimmerlvl1 = customVar.dimmerlvl1;
  dimmerlvl2 = customVar.dimmerlvl2;
  dimmerlvl3 = customVar.dimmerlvl3;
  dimmerlvl4 = customVar.dimmerlvl4;


  Serial.println("Reading dimmer data from EEPROM!");
  Serial.print( "dimmer1=>" );
  Serial.println( dimmerlvl1 );
  Serial.print( "dimmer2=>" );
  Serial.println( dimmerlvl2);
  Serial.print( "dimmer3=>" );
  Serial.println( dimmerlvl3 ); 
  Serial.print( "dimmer4=>" );
  Serial.println( dimmerlvl4 );  


}

//######################################################
int booltoint(bool value){
  if(value==false){
    return 0;
  }else{
    return 1;
  }
}
//######################################################
char* processcommand(String comm, int value){
  char* commandbuffer;
  commandbuffer = (char *)malloc(sizeof(comm)); 
  sprintf(commandbuffer, comm.c_str(), value);
  Serial.println(commandbuffer);
  return commandbuffer;
}
//######################################################

void processTouchScreenCommand(int switchid){
  
switch (switchid) {
    case 1:
        dimminglevel = myNextion.getComponentValue("h0");
        savetoeeprom();
        readfromeeprom();
        break;
    case 2:
        selectedDimmer=1;
        Serial.println("Button 1 pressed");
        myNextion.sendCommand(processcommand("h0.val=%d", dimmerlvl1));
        myNextion.sendCommand(processcommand("n0.val=%d", dimmerlvl1));
        myNextion.sendCommand("ref h0");
        myNextion.sendCommand("ref n0");
        break;
    case 3:
        selectedDimmer=2;
        Serial.println("Button 2 pressed");
        myNextion.sendCommand(processcommand("h0.val=%d", dimmerlvl2));
        myNextion.sendCommand(processcommand("n0.val=%d", dimmerlvl2));
        myNextion.sendCommand("ref h0");
        myNextion.sendCommand("ref n0");
        break;
    case 4:
        selectedDimmer=3;
        Serial.println("Button 3 pressed");
        myNextion.sendCommand(processcommand("h0.val=%d", dimmerlvl3));
        myNextion.sendCommand(processcommand("n0.val=%d", dimmerlvl3));
        myNextion.sendCommand("ref h0");
        myNextion.sendCommand("ref n0");
        break;
    case 5:
        selectedDimmer=4;
        Serial.println("Button 4 pressed");
        myNextion.sendCommand(processcommand("h0.val=%d", dimmerlvl4));
        myNextion.sendCommand(processcommand("n0.val=%d", dimmerlvl4));
        myNextion.sendCommand("ref h0");
        myNextion.sendCommand("ref n0");
        break;
   default:
        Serial.println("non");
        break;
}
}
