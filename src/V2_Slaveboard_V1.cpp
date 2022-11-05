#include <Arduino.h>
#include <Wire.h>
//#include <BitArray.h>

// 40 Pin Assignment
// Resistor Power
byte CR1 = 1;  // 1
byte CR2 = 0;  // 33
byte CR3 = 32; // 39
byte CR4 = 31; // 38
byte CR5 = 30; // 37
byte CR6 = 29; // 36
byte CR7 = 28; // 35

// Collunm power
// C = collum number, starting on left, used to power the collunms 1 at a time.
byte C1 = 8;
byte C2 = 9;
byte C3 = 10;
byte C4 = 11;
byte C5 = 7;
byte C6 = 6;
byte C7 = 5;

// Row data
//  R = Row Number, starting at top, used to read the rows when a collunm is powered
byte R1 = 14; // 22
byte R2 = 15; // 23
byte R3 = 16; // 24
byte R4 = 17; // 25
byte R5 = 18; // 26
byte R6 = 19; // 27

// 3 usable LEDS on board;

int Led1 = 26;
int Led2 = 25;
int Led3 = 24;

// transostor for i2c line
int Sdasig = 12;

/*

//48 Pin Assigment
//Resistor Power
byte CR1 = 1;   //1
byte CR2 = 0;  //33
byte CR3 = 40;  //39
byte CR4 = 39;  //38
byte CR5 = 38;  //37
byte CR6 = 37;  //36
byte CR7 = 36;  //35

//Collunm power
//C = collum number, starting on left, used to power the collunms 1 at a time.
byte C1 = 14;
byte C2 = 15;
byte C3 = 16;
byte C4 = 17;
byte C5 = 7;
byte C6 = 6;
byte C7 = 5;


//Row data
// R = Row Number, starting at top, used to read the rows when a collunm is powered
byte R1 = 22; //22
byte R2 = 23;  //23
byte R3 = 24;  //24
byte R4 = 25;  //25
byte R5 = 26;  //26
byte R6 = 27;  //27

//3 usable LEDS on board;

int Led1 = 34;
int Led2 = 33;
int Led3 = 32;

*/

byte assignmentArray[6][7] = {
    // array to hold assignment for possition, digital or analogue
    {1, 1, 1, 1, 1, 1, 2},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
};

// 1 button 2 analog 0 empty

byte analogChanges = 0; // merge these 2 later
byte digitalChanges = 0;
int changesSent = 0;

byte changed[6][7] = {
    // 1 or 0 has the value change from previous
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
};

int analogreads = 0;
int analogArray[6][7] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
};

int moduleID[6][7] = {
    // module ID array called for in initilization in the master
    {1024, 1024, 32, 1024, 512, 256, 1024},
    {0, 0, 0, 0, 1024, 0, 14},
    {0, 16, 0, 255, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
};

int keyAssignment[6][7] = {
    // byte value of the ascii chariter to be sent.
    {102, 103, 104, 105, 106, 107, 108},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
};

byte testRead = 0;

byte rowArray[6] = {R1, R2, R3, R4, R5, R6};     // put the rows in an array so the can be incremented through
byte colArray[7] = {C1, C2, C3, C4, C5, C6, C7}; // put the collunms in an array so the can be incremented through

int Row = 1;
int Col = 1;

int oldButtonArray[6][7] = {
    // previously sent buttons to the master
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
};

int dButtonArray[6][7] = {
    // new button positions and values.
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
};
bool sentdigital = false;
bool sentAnalog = false;

/*          collunms
r* byte 1 = 11,12,13,14,15,16,17
o* byte 2 = 21,22,23,24,25,26,27
w* byte 3 = 31,32,33,34,35,36,37
 * byte 4 = 41,42,43,44,45,46,47
 * byte 5 = 51,52,53,54,55,56,57
 * byte 6 = 61,62,63,64,65,66,67
 */
bool sendingLoad = false;
int analogread1 = 0;
int analogread2;

int LoadSize = 0;
bool sentLoadSize = false;
bool initilize = false;
byte SlaveID = 253;
bool firstContact = false;
byte moduleIDChunk = 0; // Module ID has to be sent in 3 parts due to 32 bit limit on i2c, this keeps track of how much has been sent

byte digitalRuns = 0;
byte analogRuns = 0;

bool sendingData = false;

bool confirmID = false;
int makeLinear(float y)
{
  float a = 102; // pot max
  float b = 47;  // input resistor
  float c = 47;  // ouput resistor

  y = y / 1024;

  if (y < 0.0001)
  {
    y = 0.0001;
  }

  return ((-(-pow(a, 2) * y - a * b * y + c * a - sqrt(pow((pow(a, 2) * y + a * b * y - c * a), 2) + 4 * pow(a, 2) * y * (c * a * y + c * b * y))) / (2 * pow(a, 2) * y)) * 1024);
}

byte ChangeWeight = 6;

void sendData()
{
  for (int row = 0; row < sizeof(rowArray); row++) // calulate digital load size here
  {
    for (int col = 0; col < sizeof(colArray); col++)
    {

      if (changed[row][col] == 2)
      {
        digitalWrite(Led2, HIGH);
        Wire.write(1); // sending what module it is
        Wire.write(SlaveID);

        Wire.write(highByte(dButtonArray[row][col]));
        Wire.write(lowByte(dButtonArray[row][col]));
        Wire.write(highByte(keyAssignment[row][col])); // send byte 1
        Wire.write(lowByte(keyAssignment[row][col]));  // send byte 2
        changed[row][col] = 0;

        return;
      }

      if (changed[row][col] == 3)
      {
        // Wire.write(1);
        // Wire.write(253);
        // Wire.write(10);
        Wire.write(2); // sending what module it is
        Wire.write(SlaveID);

        Wire.write(highByte(analogArray[row][col]));
        Wire.write(lowByte(analogArray[row][col]));

        Wire.write(0); // padding to make equal size as digital info
        Wire.write(0);
        assignmentArray[row][col] = 2;
        changed[row][col] = 0;
        return;
      }
    }
  }
}

// void sendAnalog()
// {
//   for (int row = 0; row < sizeof(rowArray); row++) // calulate digital load size here
//   {
//     for (int col = 0; col < sizeof(colArray); col++)
//     {
//       if (changed[row][col] == 3)
//       {
//         // Wire.write(1);
//         // Wire.write(253);
//         // Wire.write(10);
//         Wire.write(SlaveID);
//         Wire.write(highByte(analogArray[row][col]));
//         Wire.write(lowByte(analogArray[row][col]));
//         assignmentArray[row][col] = 2;
//         changed[row][col] = 0;
//         digitalWrite(Led2, HIGH);
//         break;
//       }
//     }
//   }
// }

void receiveEvent(int receiveSize)
{
  // initialize the slave with the new ID from the master and start the new wire session
  // also opens the bypass transistor to allow communication to the next slave in line.
  if (initilize == false)
  {
    SlaveID = Wire.read();
    Wire.end();
    Wire.begin(SlaveID);
    initilize = true;
    Sdasig = HIGH;
    digitalWrite(Led3, HIGH);
  }

  // prepare the load size
  //  if (initilize == true)
  //  {
  //    if (Wire.read() == 10)
  //    { // prepare load size
  //      sentLoadSize = false;
  //      digitalLoadSize = (digitalChanges * 5); // sends the 4 data points for each button change
  //      analogLoadSize = (analogChanges * 3);   // send the 3 datapoints for each analog change
  //    }
  //  }
}

void requestEvent()
{

  digitalWrite(Led1, LOW);

  if (initilize == true && confirmID == true)
  { // normal operation

    sendingLoad = true;

    if (sentLoadSize == false) // calculate the load size
    {
      LoadSize = 0;
      for (int row = 0; row < sizeof(rowArray); row++)
      {
        for (int col = 0; col < sizeof(colArray); col++)
        {
          // calulate digital load size here
          if (changed[row][col] == 1 && assignmentArray[row][col] == 1) // checks if it is a digital button and if the button has changed
          {
            changed[row][col] = 2;              // will be set back to 0 when the data is sent back to the master by the send button function
            LoadSize = LoadSize + ChangeWeight; // should fully reset the changed matrix
          }

          // calulate analog load size here
          if (changed[row][col] == 1 && assignmentArray[row][col] == 102) // checks if analog and if value has changed 102 = changed analog signal
          {

            changed[row][col] = 3;              // will be set back to 0 when the data is sent back to the master by the send button function
            LoadSize = LoadSize + ChangeWeight; // should fully reset the changed matrix
          }
        }
      }
    }

    if (sentLoadSize == true) // start sending the load
    {

      if (sentdigital == false)
      {
        // digitalWrite(Led1, HIGH);
        if (digitalRuns > 0 || LoadSize == 0)
        {

          digitalRuns--;
          LoadSize = 30; // refills the load size with 30 bytes and uses a run.
        }

        if (LoadSize > 0) // send load size of data.
        {
          sendData();
          LoadSize = LoadSize - ChangeWeight;
        }

        if (digitalRuns < 1 && LoadSize < 1)
        { // conditions if all data has been sent
          LoadSize = 0;
          digitalRuns = 0;
          sentdigital = true;
          // sentLoadSize = false; // reset flag for load sent // move back down to bottom after
          for (int row = 0; row < sizeof(rowArray); row++)
          {
            for (int col = 0; col < sizeof(colArray); col++)
            {
              oldButtonArray[row][col] = dButtonArray[row][col];
              dButtonArray[row][col] = 0;
            }
          }
          sentLoadSize = false;
          sendingLoad = false;
        }
      }
    }

    if (sentLoadSize == false) // Send load size and generate runs
    {
      sentdigital = false;
      Wire.write(LoadSize);
      //  digitalWrite(Led2, LOW);
      if (LoadSize > 0) // does not  continue with sending load if there is no data to send
      {
        sentLoadSize = true; // send the load size
        // digitalWrite(Led1, HIGH);
        while (LoadSize / 30 > 1) // start generating runs
        {
          digitalRuns++;
          LoadSize = LoadSize - 30;
        }
        // Wire.write(88); // for senging slave runs verbose
      }
    }
  }

  if (firstContact == true && confirmID == false) // confirm Slave ID
  {
    Wire.write(67);
    Wire.write(SlaveID);
    confirmID = true;
  }

  // initiates first contact and sends back data
  if (firstContact == false)
  { // when initilizing
    Wire.write(253);
    firstContact = true;
  }
}

void setup()
{

  pinMode(Sdasig, OUTPUT);
  digitalWrite(Sdasig, LOW);

  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);

  digitalWrite(Led1, LOW);
  digitalWrite(Led2, LOW);
  digitalWrite(Led3, LOW);

  pinMode(R1, INPUT);
  pinMode(R2, INPUT);
  pinMode(R3, INPUT);
  pinMode(R4, INPUT);
  pinMode(R5, INPUT);
  pinMode(R6, INPUT);

  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  pinMode(C4, OUTPUT);
  pinMode(C5, OUTPUT);
  pinMode(C6, OUTPUT);
  pinMode(C7, OUTPUT);

  pinMode(CR1, OUTPUT);
  pinMode(CR2, OUTPUT);
  pinMode(CR3, OUTPUT);
  pinMode(CR4, OUTPUT);
  pinMode(CR5, OUTPUT);
  pinMode(CR6, OUTPUT);
  pinMode(CR7, OUTPUT);

  digitalWrite(CR1, LOW);
  digitalWrite(CR2, LOW);
  digitalWrite(CR3, LOW);
  digitalWrite(CR4, LOW);
  digitalWrite(CR5, LOW);
  digitalWrite(CR6, LOW);
  digitalWrite(CR7, LOW);

  /*
  for (int i=0; i < sizeof(rowArray); i++  ){
      for (int j=0; j < sizeof(colArray); j++){
        digitalWrite(colArray[j], HIGH); // powers on the collunm for both button and analog read
        moduleID[i][j] = analogRead(rowArray[i]); // writes the analog value to the module ID array
        digitalWrite(colArray[j], LOW);  // de power the collunm to move onto the next one
        }
    }
  */

  Wire.begin(253); // all slaves start at 253 then get assigned a i2c ID by master
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  // need to put in module
}

void loop()
{
  digitalWrite(Led2, LOW);
  // loop through rows
  // loop through each collunm
  // turn collum on
  // is button or analog
  // is button on or off?
  // read collum
  // turn collum off

  if (initilize == true) // && sendingLoad == false)
  {

      for (int row = 0; row < sizeof(rowArray); row++)
      {
        for (int col = 0; col < sizeof(colArray); col++)
        {
          digitalWrite(colArray[col], HIGH); // powers on the collunm for both button and analog read

          if (assignmentArray[row][col] == 1 && changed[row][col] < 2)
          { // find if the component is a button

            if (digitalRead(rowArray[row]) == HIGH)
            { // reads the current row

              dButtonArray[row][col] = 1; // write that the button is pressed
            }
            else
            {

              dButtonArray[row][col] = 0; // else clear the button press data
            }

            if (dButtonArray[row][col] != oldButtonArray[row][col])
            {                        // checks if the key press has changed from the previous send
              changed[row][col] = 1; // enter the change into the changed matrix
                                     // will need to calulate the digital changes after
            }
            else
            {

              changed[row][col] = 0; // if it is the same the clear the change from the change matrix
            }
          }

          if (assignmentArray[row][col] == 2 || assignmentArray[row][col] == 102)
          { // check if it is a analog assignment

            analogreads = makeLinear(analogRead(rowArray[row])); // reads the current row
            if (analogArray[row][col] != analogreads)
            { // checks if the current value for the anallog device is diffrent from its previous value

              analogArray[row][col] = analogreads; // set the new analog value
              if (assignmentArray[row][col] == 2 && changed[row][col] < 2)  // checks if the analog device is already updated
              {

                assignmentArray[row][col] = 102; // sets the analog device to be un a updated state
                // analogChanges++; // add a analog change to be sent on the next data sent to the master
                changed[row][col] = 1;
              }

              // analogArray[i][j] = analogreads;
            }
          }

          digitalWrite(colArray[col], LOW); // de power the collunm to move onto the next one
        }
      }

  }
}
