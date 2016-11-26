/***************************************************************************************************   
  ****  About the Project : 
  ****  [Flash memory - program memory ] : Maximum : 32,256 bytes | used : 6,906 bytes  | (24%) used
  ****  [Dynamic memory - part of Ram]   : Maximum : 2,048 bytes  | used : 748 bytes    | (36%) used
  ****  This code for the third arduino that controls ARM motors, motor for the 3D cams plate and light.  
  ****  Received message <----> *spin,bind,gripper,light,cam_motor$
****************************************************************************************************/
//[hash defines]
#define msgLength 5    //Determine the length of received message. 

#define light A0       //Light on pin A0.

//[variables]
char msg=' ';
String message=" ";
int index=0 , commaPosition =0;
int data[msgLength];

//struct that represents motor pins.
struct motor{
  int right;
  int left;
  int pwm;
};
motor arm1 = {2, 4, 3};             //spin motor  <--> right pin [pin2], left pin [pin4], PWM [pin3]  
motor arm2 = {6, 7, 5};             //bind motor  <--> right pin [pin6], left pin [pin7], PWM [pin5]
motor arm3 = {8, 10, 9};            //gripper     <--> right pin [pin8], left pin [pin10], PWM [pin9]
motor plate = {12, 13, 11};         //plate motor <--> right pin [pin12], left pin [pin13], PWM [pin11]
/***************************************************************************************************/
void setup()
{
  //Serial.begin(9600);               //opens serial port, sets data rate to 9600 bps
  pinsDeclaration();                  
}
/***************************************************************************************************/
void loop()
{  if (Serial.available() > 0)
   { //serial();
     spin(data[0]);
     bind(data[1]);
     gripper(data[2]);
     light_1(data[3]);
     //plate_move(data[4]);
   }
}
/***************************************************************************************************/

/***************************************************************************************************/
void serial()
{ 
    /* Receive data from serial && Convert string data separated by commas to List of Integers.*/
    
    msg = Serial.read();
    // ping to Know third arduino.
    if(msg == 'p')
    {Serial.println("arm");}
    // start to receive real message.
    else if (msg == '*')
    {
      message = Serial.readStringUntil('$');
      Serial.println(message);
      for (index = 0; index < msgLength; index++)
      {
           commaPosition = message.indexOf(',');
           //commaPosition will be -1 if no comma is found.
           if (commaPosition != -1 )
           { 
              data[index] = message.substring(0, commaPosition).toInt();
              message = message.substring(commaPosition + 1, message.length());
           }
           else 
           {
              if (message.length() > 0 )
                data[index] = message.toInt();
           }
      }
    }
 }
 /***************************************************************************************************/
 void spin(int pwm)
 {
    if(pwm > 0 && pwm <= 255 )
   {
     digitalWrite(arm1.right , HIGH);  
     digitalWrite(arm1.left , LOW);  
     analogWrite(arm1.pwm , pwm);
     Serial.print("arm spin right : ");
     Serial.println(pwm);
   }
   else if(pwm < 0 && pwm >= -255 )
   {
     digitalWrite(arm1.right , LOW);  
     digitalWrite(arm1.left , HIGH);  
     analogWrite(arm1.pwm , -1*pwm);
     Serial.print("arm spin left : ");
     Serial.println(pwm);
   }
   else
   {
     digitalWrite(arm1.right , LOW);  
     digitalWrite(arm1.left , LOW);  
     analogWrite(arm1.pwm , 0);
     Serial.print("spining stopped : ");
     Serial.println(pwm);  
   } 
 }
 /***************************************************************************************************/
 void bind(int pwm)
 {
   if(pwm > 0 && pwm <= 255 )
   {
     digitalWrite(arm2.right , HIGH);  
     digitalWrite(arm2.left , LOW);  
     analogWrite(arm2.pwm , pwm);
     Serial.print("arm bind forward : ");
     Serial.println(pwm);
   }
   else if(pwm < 0 && pwm >= -255 )
   {
     digitalWrite(arm2.right , LOW);  
     digitalWrite(arm2.left , HIGH);  
     analogWrite(arm2.pwm , -1*pwm);
     Serial.print("arm bind backward : ");
     Serial.println(pwm);
   }
   else
   {
     digitalWrite(arm2.right , LOW);  
     digitalWrite(arm2.left , LOW);  
     analogWrite(arm2.pwm , 0);
     Serial.print("bind stopped : ");
     Serial.println(pwm);  
   } 
 }
 /***************************************************************************************************/
 void gripper(int enable)
 {  // [1] <---> open, [-1] <---> close
    if (enable == 1 )
   {
     digitalWrite(arm3.right , HIGH);  
     digitalWrite(arm3.left , LOW);  
     analogWrite(arm3.pwm , 150);
     Serial.println("gripper open ");
   }
   else if (enable == -1)
   {
     digitalWrite(arm3.right , LOW);  
     digitalWrite(arm3.left , HIGH);  
     analogWrite(arm3.pwm , 150);
     Serial.println("gripper close ");
   }
   else
   {
     digitalWrite(arm3.right , LOW);  
     digitalWrite(arm3.left , LOW);  
     analogWrite(arm3.pwm , 0);
     Serial.println("arm3 stopped ");
   } 
   
 }
 /***************************************************************************************************/
 void plate_move(int enable)
 {  // [1] <---> move up , [-1] <---> move down
     if (enable == 1 )
   {
     digitalWrite(plate.right , HIGH);  
     digitalWrite(plate.left , LOW);  
     analogWrite(plate.pwm , 80);
     Serial.println("plate moving up ");
   }
   else if (enable == -1)
   {
     digitalWrite(plate.right , LOW);  
     digitalWrite(plate.left , HIGH);  
     analogWrite(plate.pwm , 80);
     Serial.println("plate moving down ");
   }
   else
   {
     digitalWrite(plate.right , LOW);  
     digitalWrite(plate.left , LOW);  
     analogWrite(plate.pwm , 0);
     Serial.println("plate stopped ");
   }    
 }
/***************************************************************************************************/
void light_1(int enable)
{  // [1] <----> Light ON
   if(enable == 1)
   {   
      digitalWrite(light, HIGH);
      Serial.println("Light ON");
   }
   else
   {
      digitalWrite(light, LOW);
      Serial.println("Light OFF");
   }
}
