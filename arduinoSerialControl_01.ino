/* ****************************************************

arduinoVBserialcontrol
Arduino controlled by VisualBasic
www.martyncurrey.com

Some of the below code is not very elegant but it should be easy to see what is happening.

 Pins
 2 
 3 
 4 LED
 5 LED
 6 LED
 7 LED
 8 LED
 9 RGB LED RED PIN
 10 RGB LED GREEN PIN
 11 RGB LED BLUE PIN
 12 LED
 14 LED - LEFT
 15 LED - DOWN
 16 LED - UP
 17 LED - RIGHT
 18 LED - OK

Commands
The expected commands are

Pin HIGH / LOW
<P001ON> - P001 = Pin 1. ON = HIGH
<P001OF> - P001 = Pin 1. OF = LOW

RGB LED / Slider
<Trrrgggbbb> - T is the slider command. rrr is the decimal value 0-255 for the red LED
                                        ggg is the decimal value 0-255 for the green LED
                                        bbb is the decimal value 0-255 for the blue LED

Navigation
<NUON> - N = navigation. U for UP. ON for HIGH
<NUOF> - N = navigation. U for UP. OF for LOW
U = up. D = down. L = left. R = right. K = OK/select


Commands
<CMD01> - red / green flash
<CMD02> - LED chaser
<CMD03> - RGB LED cycle colours

Other.
<HELLO>
<FON>
<FOF>
<DON>
<DOF>

*/ ///////////////////////////////////////////////////////


boolean debug = true;

// length of command is 20 chrs
// if you need longer commands then increase the size of numChars
const byte numChars = 20;
char receivedChars[numChars];

boolean newData = false;

unsigned int red = 255;
unsigned int green = 255;
unsigned int blue = 255;

byte UPpin = 16;
byte DOWNpin = 15;
byte LEFTpin = 14;
byte RIGHTpin = 17;
byte OKpin = 18;

boolean feedback = true;


void setup() 
{
     pinMode(12, OUTPUT); 
     pinMode(8, OUTPUT); 
     pinMode(7, OUTPUT); 
     pinMode(6, OUTPUT); 
     pinMode(5, OUTPUT); 
     pinMode(4, OUTPUT); 

     pinMode(11, OUTPUT);
     pinMode(10, OUTPUT);
     pinMode(9, OUTPUT);
     
     // Common anode RGB lED, 255 is off.
     // If using a common cathod RGB lED then remove the next 3 lines.
     analogWrite(9, red);
     analogWrite(10, green);
     analogWrite(11, blue);  
            
     pinMode(14, OUTPUT); 
     pinMode(15, OUTPUT);      
     pinMode(16, OUTPUT);      
     pinMode(17, OUTPUT); 
     pinMode(18, OUTPUT);   
     
     Serial.begin(9600);
     sendStartMessage();
}

void loop() 
{
     if (Serial.available() > 0)     {  recvWithStartEndMarkers(); }
     if (newData) { parseData(); }
}     



/*********************
* sends a start up message over serial.
* Assumes serial is connected
* 
* Global:
*       debug
* Local:
* 
*/   
void sendStartMessage()
{
     
     Serial.println(" "); 
     Serial.println("arduinoVBserialControl Ver 1.0"); 
     Serial.println(" "); 
     Serial.println("DON = debug on"); 
     Serial.println("DOF = debug off"); 
     Serial.println("START to reset"); 
     Serial.println(" "); 
     
     if (debug) { Serial.println("Debug is on"); }
           else { Serial.println("Debug is off"); }
    Serial.println(" "); 
}




/*********************
* Checks receivedChars[] for commands
* 
* Global:
*  receivedChars[]
*  newData;
* 
* Local:
* 
*/      
void parseData()
{  
        newData = false;    
        if (debug) { Serial.println( receivedChars ); }    



        // HELLO
        // If the Arduino receives "HELLO" it sends "HELLO" back
        // This is used by the VB program to show it is connected
        if (strcmp(receivedChars, "HELLO")  == 0)
        {
            Serial.println("HELLO");
            
            //Blink the LED on pin 13 to show "HELLO" was received
            digitalWrite(13,HIGH); 
            delay(100);
            digitalWrite(13,LOW); 
        }

        if (strcmp(receivedChars, "START")  == 0)
        {
            sendStartMessage();
        }


        // PIN
        // P001ON - P for pin. 001 is the button number. ON = on
        // P001OF - P for pin. 001 is the button number. OF = off
        if (receivedChars[0] == 'P'  )  
        {
             int tmp = convertToNumber( 1 ); 
             if ( receivedChars[4] == 'O' && receivedChars[5] == 'N' ) { digitalWrite(tmp,HIGH); }
             if ( receivedChars[4] == 'O' && receivedChars[5] == 'F' ) { digitalWrite(tmp,LOW);  }
        } // PIN



        // RGB SLIDER
        // Trrrgggbbb
        if  ( receivedChars[0] == 'T' )
        {  
            // For a common anode RGB LED take the value away from 255.
            // If using a common cathod RGB led then use:
            // red = convertToNumber( 1 );  
            // green  = convertToNumber( 4 );
            // blue = convertToNumber( 7 );  
            
            red = 255 - convertToNumber( 1 );  
            green  = 255 - convertToNumber( 4 );
            blue = 255 - convertToNumber( 7 );  
            analogWrite(9, red);
            analogWrite(10, green);
            analogWrite(11, blue);            
        }  // RGB SLIDER       
        
        
        
        // NAVIGATION
        // NUON - N = navigation. U for UP. ON for on
        // NUOF - N = navigation. U for UP. OF for off
        if ( receivedChars[0] == 'N' )  
        {
              if ( receivedChars[1] == 'U' )   // UP
              { 
                  if ( receivedChars[2] == 'O'  && receivedChars[3] == 'N'  ) { digitalWrite(UPpin,HIGH); }
                  if ( receivedChars[2] == 'O'  && receivedChars[3] == 'F'  ) { digitalWrite(UPpin,LOW); }
              }
  
              if ( receivedChars[1] == 'D' )   // UP
              {
                  if ( receivedChars[2] == 'O'  && receivedChars[3] == 'N'  ) { digitalWrite(DOWNpin,HIGH); }
                  if ( receivedChars[2] == 'O'  && receivedChars[3] == 'F'  ) { digitalWrite(DOWNpin,LOW); }
              }
  
              if ( receivedChars[1] == 'L' )   // UP
              {
                  if ( receivedChars[2] == 'O'  && receivedChars[3] == 'N'  ) { digitalWrite(LEFTpin,HIGH); }
                  if ( receivedChars[2] == 'O'  && receivedChars[3] == 'F'  ) { digitalWrite(LEFTpin,LOW); }              
                
              }
              
              if ( receivedChars[1] == 'R' )   // UP
              {
                  if ( receivedChars[2] == 'O'  && receivedChars[3] == 'N'  ) { digitalWrite(RIGHTpin,HIGH); }
                  if ( receivedChars[2] == 'O'  && receivedChars[3] == 'F'  ) { digitalWrite(RIGHTpin,LOW); }              
              }
  
              if ( receivedChars[1] == 'K' )   // UP
              {
                   if ( receivedChars[2] == 'O'  && receivedChars[3] == 'K'  ) 
                   {   digitalWrite(OKpin,HIGH); 
                       delay(75);
                       digitalWrite(OKpin,LOW);
                   }              
              }
            
        } // NAVIGATION



        //COMMAND
        //CMD01
        //CMD02
        //CMD03
        // do not really need 3 characters. A single "C" would do.
        if ( receivedChars[0] == 'C'  && receivedChars[1] == 'M'  && receivedChars[2] == 'D' )  
        {        
              
              if ( receivedChars[4] == '1')
              {           
                    for ( byte Count = 1; Count <= 15; Count++) 
                    {
                        digitalWrite(4,HIGH); delay(100); digitalWrite(4,LOW); 
                        digitalWrite(12,HIGH); delay(100); digitalWrite(12,LOW); 
                    }
                    if (feedback) { sendOK(1); }
             }     
                      
          
              if ( receivedChars[4] == '2')
              { 
                    for ( byte Count = 1; Count <= 5; Count++) 
                    {
                        digitalWrite(4,HIGH);  delay(100); digitalWrite(4,LOW); 
                        digitalWrite(5,HIGH);  delay(100); digitalWrite(5,LOW); 
                        digitalWrite(6,HIGH);  delay(100); digitalWrite(6,LOW); 
                        digitalWrite(7,HIGH);  delay(100); digitalWrite(7,LOW); 
                        digitalWrite(8,HIGH);  delay(100); digitalWrite(8,LOW); 
                        digitalWrite(12,HIGH); delay(100); digitalWrite(12,LOW); 
                    }
                    if (feedback) { sendOK(2); }
              }
        
        
              if ( receivedChars[4] == '3')
              {   
                  
                  red = 255; green = 255; blue = 255;
                  for(int red = 255; red >0;    red--)        {  analogWrite(9, red);     delay (5);   } 
                  for(int red = 0;   red < 256; red++)        {  analogWrite(9, red);     delay (5);   } 
                  for(int green = 255; green >0;    green--)  {  analogWrite(10, green);  delay (5);   } 
                  for(int green = 0;   green < 256; green++)  {  analogWrite(10, green);  delay (5);   } 
                  for(int blue = 255; blue >0;    blue--)     {  analogWrite(11, blue);   delay (5);   } 
                  for(int blue = 0;   blue < 256; blue++)     {  analogWrite(11, blue);   delay (5);   } 
                  if (feedback) { sendOK(3); }
              }        

        }  // COMMAND




        // Typed commands -----------------------------------------------------------------------------
        
        if (strcmp(receivedChars, "DON")  == 0)  
        {
              if ( receivedChars[1] =='O'  && receivedChars[2] =='F' )  {  debug = false;  Serial.println("Debug is off"); }
        }
        if (strcmp(receivedChars, "DOF")  == 0)  
        {
              if ( receivedChars[1] =='O'  && receivedChars[2] =='F' )  {  debug = false;  Serial.println("Debug is off"); }
        }
        

        // ACKNOWLEDGMENT
        // FON
        // FOF
        if  ( receivedChars[0] == 'F' )
        {
           if ( receivedChars[1] =='O'  && receivedChars[2] =='N' )  {  feedback = true;    }
           if ( receivedChars[1] =='O'  && receivedChars[2] =='F' )  {  feedback = false;    }
           
           if (feedback) { Serial.println("acknowledgment is on");  } 
           else          { Serial.println("acknowledgment is off"); } 
        }  // FEEDBACK  
        
       
        
        if  ( receivedChars[0] == 'D' )
        {
              if ( receivedChars[1] =='O'  && receivedChars[2] =='N' )  {  debug = true;   Serial.println("Debug is on"); }
              if ( receivedChars[1] =='O'  && receivedChars[2] =='F' )  {  debug = false;  Serial.println("Debug is off"); }
        }
        
        
        
}






/*********************
* Takes seial input and looks for data between a start and end marker.
* 
* Global:
*  Updates receivedChars[] with the received data
* 
* Local:
* 
*/ 
void recvWithStartEndMarkers() 
{
    
// function recvWithStartEndMarkers by Robin2 of the Arduino forums
// See  http://forum.arduino.cc/index.php?topic=288234.0

     static boolean recvInProgress = false;
     static byte ndx = 0;
     char startMarker = '<';
     char endMarker = '>';

     char rc;

     if (Serial.available() > 0) 
     {
          rc = Serial.read();

          if (recvInProgress == true) 
          {
               if (rc != endMarker) 
               {
                    receivedChars[ndx] = rc;
                    ndx++;
                    if (ndx >= numChars) { ndx = numChars - 1; }
               }
               else 
               {
                     receivedChars[ndx] = '\0'; // terminate the string
                     recvInProgress = false;
                     ndx = 0;
                     newData = true;
               }
          }

          else if (rc == startMarker) { recvInProgress = true; }
     }

}





/*********************
* converts 3 ascii characters to a numeric value
* 
* Global:
*  Expects receivedChars[] to contain the ascii characters
* 
* Local:
*  startPos is the position of the first character
* 
* 
*/

int convertToNumber( byte startPos)
{
  unsigned int tmp = 0;
  tmp = (receivedChars[startPos]-48) * 100;
  tmp = tmp + (receivedChars[startPos+1]-48) * 10;
  tmp = tmp + receivedChars[startPos+2]-48;  
  return tmp;
}



void sendOK(int val)
{
  // The 3 command buttons wait for the OK signal
  Serial.print("OK");Serial.println(val);
}





