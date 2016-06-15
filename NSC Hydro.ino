//************************************************************************************/
//
//   Growtek Hydroponics Controller v.1.0
//   Written and Debugged by Nick J. Penna
//   Copyright 2015-2016 Nick J. Penna
//     E-mail:  kaniick702@gmail.com
//     Twitter: Kaniick
//
//************************************************************************************/
//
//   This Program is currently under active Development as of June 2016
//
//************************************************************************************/
//
//   Known Bugs/Issues:
//     - It may be possible to set a date that does not exist (ie. FEB 31, 2011)
//     - Occasionally when returing from ScreenSaver, minor Time and Date Bar issues
//     - The level of automation may make you lazy
//     - If you spot an error or bug, please let me know!
//     -
//     -
//************************************************************************************/
//
// LEGAL DISCLAIMER:
//   Growtek Hydroponics Controller v.1.0, Copyright 2015-2016 Nick J Penna.
//   I'm providing this program as free software with the sole intent on furthering
//   DIY Aquarium Lighting, but WITHOUT ANY WARRANTY; without even the implied warranty
//   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. You may modify and/or use
//   it under the terms of the GNU General Public License as published by the Free
//   Software Foundation version 3 of the License, or (at your option) any later
//   version.  However if you intend on using the program (either in its entirety or any
//   part of it) in any way, shape, or form for PROFIT, you MUST contact me and obtain
//   my EXPRESS WRITTEN CONSENT (contact information is provided above).
//   VIOLATORS WILL BE PROSECUTED TO THE FULLEST EXTENT OF THE LAW.
//
//************************************************************************************/

// ***************
// *  Libraries  *
// ***************
// The Libraries needed for this system
#include <DHT.h>                          // Temperature and Humidity Sensor Library
#include <OneWire.h>                      // OneWire Library - Used to com data over 1 wire - DUH
#include <DallasTemperature.h>            // DS18B20 Temperature Sensor Library
#include <Wire.h>                         // A Library for wires ... Cool huh?
#include "RTClib.h"                       // RTC Library - It's used to keep track of the time
#include <SD.h>							  // SD Card library - used to write/read and access the SD card breakout

// ***************
// *  Variables  *
// ***************
// DHT Temperature & Humidity Sensors
#define DHTPIN4 10                        // what pin DHT sensor 4 is connected to
#define DHTPIN3 11                        // what pin DHT sensor 3 is connected to
#define DHTPIN2 12                        // what pin DHT sensor 2 is connected to
#define DHTPIN1 13                        // what pin DHT sensor 1 is connected to
// DS18B20 Temp Sensors
#define ONE_WIRE_BUS 2                    // DS18B20 Data wire is plugged into pin 3
// SD Card Chipset for output; Note that even if it's not used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library functions will not work.
const int chipSelect = 53;                // Arduino Mega Chipset selected, used later on in the code

float tempC, tempC1, tempF1, hum1, tempC2, tempF2, hum2;	// this lines just solves problems
float tempC3, tempF3, hum3, tempC4, tempF4, hum4;  		  	// this lines just solves problems
float p1, p2, p3, pf1, pf2, pf3;					     // this lines just solves problems
int ID = 1;								  // Data logged entry Identification Number (Entry number)

// Uncomment whatever type you're using. I am using DHT22 in this application, a little more expensive
// but it is more accurate. So check this section for your Application.
//#define DHTTYPE DHT11                   // DHT 11
#define DHTTYPE DHT22                     // DHT 22 (AM2302)
//#define DHTTYPE DHT21                   // DHT 21 (AM2301)

//DHT Temp-Hum Sensors
DHT dht1(DHTPIN1, DHTTYPE);               // set the variable DHT to dht - this shows sensor 1
DHT dht2(DHTPIN2, DHTTYPE);               // set the variable DHT to dht - this shows sensor 2
DHT dht3(DHTPIN3, DHTTYPE);               // set the variable DHT to dht - this shows sensor 3
DHT dht4(DHTPIN4, DHTTYPE);               // set the variable DHT to dht - this shows sensor 4
// DS18B20 Temp Sensors
OneWire oneWire(ONE_WIRE_BUS);            // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);      // Pass our oneWire reference to Dallas Temperature.

// **************************************
// *  HEX Address of Temperature Probes *
// **************************************
// Assign the addresses of your 1-Wire DS18B20 temp sensors.
// You retrieve the HEX Codes of the Sensors and Assign them to the Probe Variable.
byte Thermo1[8] = { 0x28, 0x89, 0x1C, 0x08, 0x00, 0x00, 0x80, 0xAF };		// DS18B20 Probe 1 HEX ID
byte Thermo2[8] = { 0x28, 0x4C, 0x05, 0x08, 0x00, 0x00, 0x80, 0x54 };		// DS18B20 Probe 2 HEX ID
byte Thermo3[8] = { 0x28, 0xC6, 0x29, 0x08, 0x00, 0x00, 0x80, 0xA8 };		// DS18B20 Probe 3 HEX ID
//byte Thermo4[8] = { 0x28, 0x89, 0x1C, 0x08, 0x00, 0x00, 0x80, 0xAF };		// DS18B20 Probe 1 HEX ID
//byte Thermo5[8] = { 0x28, 0x4C, 0x05, 0x08, 0x00, 0x00, 0x80, 0x54 };		// DS18B20 Probe 2 HEX ID
//byte Thermo6[8] = { 0x28, 0xC6, 0x29, 0x08, 0x00, 0x00, 0x80, 0xA8 };		// DS18B20 Probe 3 HEX ID
//byte Thermo7[8] = { 0x28, 0x89, 0x1C, 0x08, 0x00, 0x00, 0x80, 0xAF };		// DS18B20 Probe 1 HEX ID
//byte Thermo8[8] = { 0x28, 0x4C, 0x05, 0x08, 0x00, 0x00, 0x80, 0x54 };		// DS18B20 Probe 2 HEX ID

// RTC Module
RTC_DS1307 RTC;                           // Set variable RTC_DS1307 to RTC

// ********************
// *  Hardware Setup  *
// ********************
void setup() {                            // Set up the Hardware
  delay(500);
// Starting the Serial Monitor
  Serial.begin(9600);                     // Set baud rate for the hardware serial port_0 to 9600
  Serial.println("System Ready");    	  // Display a Message in Serial monitor so we know all is well
// SD Card
  Serial.print("Initializing SD card...");		// Initializing SD Card notification prompt
  pinMode(10, OUTPUT);                    		// Note that even if it's not used as the CS pin, the hardware SS pin
                                          		// (10 on most Arduino boards, 53 on the Mega) must be left as an output
                                          		// or the SD library functions will not work.
  if (!SD.begin(chipSelect)) {          		// Start SD Card, the chipSelect is set in the variable above ^^ depending on which arduino is used.
    Serial.println("Initialization failed, or the SD card is not present.");   // Notified if Intialization has failed, there is a connection or format error if so
      return;
  }
    Serial.println("Initialization Complete");  // if SD card was sucessful the initialization is complete
	PrintHeader();

// DS18B20 Temp Sensors
  sensors.begin();                       	// Start up the library
  sensors.setResolution(Thermo1, 10);    	// set the resolution to 10 bit (Should be good enough?)
  sensors.setResolution(Thermo2, 10);    	// set the resolution to 10 bit (Should be good enough?)
  sensors.setResolution(Thermo3, 10);    	// set the resolution to 10 bit (Should be good enough?)
  //sensors.setResolution(Thermo4, 10);    	// set the resolution to 10 bit (Should be good enough?)
  //sensors.setResolution(Thermo5, 10);    	// set the resolution to 10 bit (Should be good enough?)
  //sensors.setResolution(Thermo6, 10);    	// set the resolution to 10 bit (Should be good enough?)
  //sensors.setResolution(Thermo7, 10);    	// set the resolution to 10 bit (Should be good enough?)
  //sensors.setResolution(Thermo8, 10);    	// set the resolution to 10 bit (Should be good enough?)
//DHT Temp-Hum Sensors
  dht1.begin();                             // start dht1
  dht2.begin();                             // start dht2
  dht3.begin();                             // start dht3
  dht4.begin();                             // start dht4

// Lets Start Grabbing some Data
  Getdht1Data();							// Starts the function to grab data from DHT1
  Getdht2Data();							// Starts the function to grab data from DHT2
  Getdht3Data();							// Starts the function to grab data from DHT3
  Getdht4Data();							// Starts the function to grab data from DHT4

// RTC Module - It Keeps track of the time
  Wire.begin();                             // start wire
  RTC.begin();                              // start RTC ::tick-tick::

// Lets Get the Current Time
  Getcurrenttime();							// Gets the current Time
// Report RTC
  if (! RTC.isrunning()) {					// Checks to see if RTC is running
    Serial.println("RTC is NOT running");	// if it's not running an error will display on the Serial Monitor
  }
// following line sets the RTC to the date & time this sketch was compiled
    // This line sets the RTC with an explicit date & time, for example to set
    // June 12, 2016 at 8:51:00 PM you would call:
    //rtc.adjust(DateTime(2016, 6, 12, 20, 51, 0));
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!!!!!!!!! IMPORTANT NOTICE !!!!!!!!!!!!!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! MAKE SURE YOU SET THE CORRECT TIME BEFORE COMPILING CODE !!!
    RTC.adjust(DateTime(2016, 6, 12, 22, 22, 0));		// This is the current time (This has to be set manual upon uploading sketch)

    // Start DS18B20 Function
    Getds18b20data();
}

// *****************************************
// *  Printes Header Data to SD Card File  *
// *****************************************
void PrintHeader() // prints header to SD card
{
   File datafile = SD.open("tdata1.csv", FILE_WRITE);
   if (datafile)
   {
      String header = "ID, Date, Time, Sensor, DHT-1 Temp C, DHT-1 Temp F, DHT-1 Humidity (%), Sensor, DHT-2 Temp C, DHT-2 Temp F, DHT-2 Humidity (%), Probe #, P-1 Temp C, P-1 Temp F, Probe #, P-2 Temp C, P-2 Temp F, Probe #, P-3 Temp C, P-3 Temp F";
      datafile.println(header);
      datafile.close();
   }
}

// *****************
// *  System Loop  *
// *****************
// Start of the System, this loops through the entire process (including delays)
void loop()
{                             // here we go ... Weeeeeeeeee ... This is gonna be Fun!

  // Real Time Clock, This should be configured above in the setup section look for the Important section
  delay(1500);
  Getcurrenttime();						  // Grabs the Current Date and Time

  // Notes about Sensor and timing
  // Reading temperature or humidity takes about 250 milliseconds! We Delayed it to 1/2 second
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  Getdht1Data();						  // Grabs data from the DHT1 Sensor (°C, °F, and Humidity) || If it's unable to connect it will display and error
  delay(500);							  // delayed 500 ms = 1/2 second
  Getdht2Data();						  // Grabs data from the DHT2 Sensor (°C, °F, and Humidity) || If it's unable to connect it will display and error
  delay(500);							  // delayed 500 ms = 1/2 second
  Getdht3Data();						  // Grabs data from the DHT3 Sensor (°C, °F, and Humidity) || If it's unable to connect it will display and error
  delay(500);							  // delayed 500 ms = 1/2 second
  Getdht4Data();						  // Grabs data from the DHT4 Sensor (°C, °F, and Humidity) || If it's unable to connect it will display and error

  // Data from DS18B20 Sensors
  delay(1000);
  Serial.print("Number of Devices found on bus = ");
  Serial.println(sensors.getDeviceCount());
  delay(3000);
  Serial.print("Getting Temperature Probe Data...");
  Serial.println();
  Getds18b20data();
  delay(500);							  // delayed 500 ms = 1/2 second

  // Data is important to our system, so lets Log the data so we can analyze it later at our computers.
  PrinttoSD();							  // This Logs the Data, its currently structured for a .CSV file type || If it's unable to connect it will display and error

// Print to Serial Monitor
// Currently only used for debugging purposes, will eventually need this to happen in order to pass to other devices in the future.
// PrintonSerialMonitor();    // Uncomment this line in order to print to the serial monitor all the Sensor Data
}


// ****************************
// *   Grab DHT Sensor Data   *
// ****************************
void Getdht1Data() // this function gets Temperature (°C & °F)as well as Humidity % values from the DHT 1 sensor
   {
      hum1 = dht1.readHumidity();                              		// Humidity Values from sensor
      tempC1 = dht1.readTemperature();                         		// Temperature in °C from sensor
      tempF1 = (tempC1*9)/5+32;                                		// Temperature in °F from sensor
       if (isnan(hum1) || isnan(tempC1) || isnan(tempF1)) {   		// See if any values are Nan
         Serial.println("Failed to read from DHT Sensor 1");  		// if values are Nan then display Error in Serial Monitor
       }
   }

void Getdht2Data() // this function gets Temperature (°C & °F)as well as Humidity % values from the DHT 2 sensor
   {
      hum2 = dht2.readHumidity();                           		// Humidity Values from sensor
      tempC2 = dht2.readTemperature();                      		// Temperature in °C from sensor
      tempF2 = (tempC2*9)/5+32;                             	 	// Temperature in °F from sensor
       if (isnan(hum2) || isnan(tempC2) || isnan(tempF2)) {   		// See if any values are Nan
         Serial.println("Failed to read from DHT Sensor 2");  		// if values are Nan then display Error in Serial Monitor
       }
   }

 void Getdht3Data() // this function gets Temperature (°C & °F)as well as Humidity % values from the DHT 3 sensor
    {
      hum3 = dht3.readHumidity();                           		// Humidity Values from sensor
      tempC3 = dht3.readTemperature();                      		// Temperature in °C from sensor
      tempF3 = (tempC3*9)/5+32;                                		// Temperature in °F from sensor
        if (isnan(hum3) || isnan(tempC3) || isnan(tempF3)) {   		// See if any values are Nan
          Serial.println("Failed to read from DHT Sensor 3");  		// if values are Nan then display Error in Serial Monitor
        }
    }

  void Getdht4Data() // this function gets Temperature (°C & °F)as well as Humidity % values from the DHT 4 sensor
     {
       hum4 = dht4.readHumidity();                              	// Humidity Values from sensor
       tempC4 = dht4.readTemperature();                         	// Temperature in °C from sensor
       tempF4 = (tempC4*9)/5+32;                                	// Temperature in °F from sensor
         if (isnan(hum4) || isnan(tempC4) || isnan(tempF4)) {   	// See if any values are Nan
           Serial.println("Failed to read from DHT Sensor 4");  	// if values are Nan then display Error in Serial Monitor
         }
     }

// ***********************************
// *  DS18B20 Sensor Value function  *
// ***********************************
  float sensorValue (byte deviceAddress[]){
    tempC = sensors.getTempC (deviceAddress);
    return tempC;
  }

// ******************************
// *  Grab DS18B20 Sensor Data  *
// ******************************
void Getds18b20data(){
  sensors.requestTemperatures();  // call readings from the addresses
  p1 = sensorValue(Thermo1);
  p2 = sensorValue(Thermo2);
  p3 = sensorValue(Thermo3);
  //p4 = sensorValue(Thermo4);
  //p5 = sensorValue(Thermo5);
  //p6 = sensorValue(Thermo6);
  //p7 = sensorValue(Thermo7);
  //p8 = sensorValue(Thermo8);
  pf1 = sensorValue(Thermo1)*9/5+32;
  pf2 = sensorValue(Thermo2)*9/5+32;
  pf3 = sensorValue(Thermo3)*9/5+32;
  //pf4 = sensorValue(Thermo4)*9/5+32;
  //pf5 = sensorValue(Thermo5)*9/5+32;
  //pf6 = sensorValue(Thermo6)*9/5+32;
  //pf7 = sensorValue(Thermo7)*9/5+32;
  //pf8 = sensorValue(Thermo8)*9/5+32;
}

// ******************************
// *  Grab Current Date & Time  *
// ******************************
void Getcurrenttime() // this function gets Temperature (°C & °F)as well as Humidity % values from the DHT 4 sensor
    {
     // Display the Current Time, Date, Month, Year
       DateTime now = RTC.now();
       Serial.print(now.year(), DEC);								// Prints the Current Year
       Serial.print('/');
       Serial.print(now.month(), DEC);								// Prints the Current Month
       Serial.print('/');
       Serial.print(now.day(), DEC);								// Prints the Current Day
       Serial.print(' ');
       Serial.print(now.hour(), DEC);								// Prints the Current Hour
       Serial.print(':');
       Serial.print(now.minute(), DEC);								// Prints the Current Minute
       Serial.print(':');
       Serial.print(now.second(), DEC);								// Prints the Current Second
       Serial.println();
    }

// ************************************
// *  Stores Data to .CSV on SD Card  *
// ************************************
void PrinttoSD()  // this function saves the data to the SD card
{
  DateTime now = RTC.now();											// Gives the time fuction
  File datafile = SD.open("tdata1.csv", FILE_WRITE);					// Writes to the file specified in the " ",
      if(datafile)													// The structure is set up as a .csv file (Comma Seperated Value)
      {
        datafile.print(ID);											// Entry Idenification Number (helps with tracking information Logged)
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print(now.month(), DEC);							// Logs current Month
        datafile.print("/");										// Date seperator
        datafile.print(now.day(), DEC);								// Logs current Day of the Week
        datafile.print("/");										// Date seperator
        datafile.print(now.year(), DEC);							// Logs current Year
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print(now.hour(), DEC);							// Logs current Hour
        datafile.print(":");										// Time seperator
        datafile.print(now.minute(), DEC);							// Logs current Minute
        datafile.print(":");										// Time seperator
        datafile.print(now.second(), DEC);							// Logs current Second
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print("DHT-1");									// Sensor name (IE: DHT sensor #1)
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print(tempC1, 3);        							// Logs the temperature in celsius (°C) for DHT1
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print(tempF1, 3);        							// Logs the temperature in fahrenheit (°F) for DHT1
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print(hum1, 3);        							// Logs the Humidity percentage for DHT1
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print("DHT-2");       								// Sensor name (IE: DHT sensor #2)
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print(tempC2, 3);        							// Logs the temperature in celsius (°C) for DHT2
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print(tempF2, 3);        							// Logs the temperature in fahrenheit (°F) for DHT2
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print(hum2, 3);        							// Logs the Humidity percentage for DHT2
        datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print("DHT-3");       								// Sensor name (IE: DHT sensor #3)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print(tempC3, 3);        							// Logs the temperature in celsius (°C) for DHT3
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print(tempF3, 3);        							// Logs the temperature in fahrenheit (°F) for DHT3
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print(hum3, 3);        							// Logs the Humidity percentage for DHT3
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print("DHT-4");       								// Sensor name (IE: DHT sensor #4)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print(tempC4, 3);        							// Logs the temperature in celsius (°C) for DHT4
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print(tempF4, 3);        							// Logs the temperature in fahrenheit (°F) for DHT4
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print(hum4, 3);        							// Logs the Humidity percentage for DHT4
        //datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print("P-1");										// Logs DS18B20 Sensor Name under the correct column (used for referance)
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print(p1);										// Logs DS18B20 Sensor Probe 1 Data in celsius (°C)
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print(pf1);										// Logs DS18B20 Sensor Probe 1 Data in fahrenheit (°F)
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print("P-2");										// Logs DS18B20 Sensor Name under the correct column (used for referance)
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print(p2);										// Logs DS18B20 Probe 2 Sensor Data in celsius (°C)
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print(pf2);										// Logs DS18B20 Sensor Probe 2 Data (in fahrenheit (°F)
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print("P-3");										// Logs DS18B20 Sensor Name under the correct column (used for referance)
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print(p3);										// Logs DS18B20 Sensor Probe 3 Data in celsius (°C)
        datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.print(pf3);										// Logs DS18B20 Sensor Probe 3 Data in fahrenheit (°F)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print("P-4");										// Logs DS18B20 Sensor Name under the correct column (used for referance)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print(p4);										// Logs DS18B20 Sensor Probe 1 Data in celsius (°C)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print(pf4);										// Logs DS18B20 Sensor Probe 1 Data in fahrenheit (°F)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print("P-5");										// Logs DS18B20 Sensor Name under the correct column (used for referance)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print(p5);										// Logs DS18B20 Probe 2 Sensor Data in celsius (°C)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print(pf5);										// Logs DS18B20 Sensor Probe 2 Data (in fahrenheit (°F)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print("P-6");										// Logs DS18B20 Sensor Name under the correct column (used for referance)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print(p6);										// Logs DS18B20 Sensor Probe 3 Data in celsius (°C)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print(pf6);										// Logs DS18B20 Sensor Probe 3 Data in fahrenheit (°F)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print("P-7");										// Logs DS18B20 Sensor Name under the correct column (used for referance)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print(p7);										// Logs DS18B20 Probe 2 Sensor Data in celsius (°C)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print(pf7);										// Logs DS18B20 Sensor Probe 2 Data (in fahrenheit (°F)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print("P-8");										// Logs DS18B20 Sensor Name under the correct column (used for referance)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print(p8);										// Logs DS18B20 Sensor Probe 3 Data in celsius (°C)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        //datafile.print(pf8);										// Logs DS18B20 Sensor Probe 3 Data in fahrenheit (°F)
        //datafile.print(",");										// comma to seperate value (moves to next column)
        datafile.println();
        datafile.close();
      }
      else
        Serial.println("SD card error.");
}

void PrintonSerialMonitor()  // this function saves the data to the SD card
{

// *******************************
// *   Debug on Serial Monitor   *
// *******************************
// Comment the lines of the sensors that you would NOT like to have the values print to the Serial Monitor
// this section is under development as new sensors are being added.
    Serial.println();
    Serial.print("DHT1 Information");
    Serial.println();
    Serial.print("Humidity: ");
    Serial.print(hum1);
    Serial.print(" %\t");                 // percentage sign
    Serial.println();
    Serial.print("Temperature (°C): ");
    Serial.print(tempC1);
    Serial.print("°C");                 // Degrees C sign
    Serial.println();
    Serial.print("Temperature (°F): ");
    Serial.print(tempF1);
    Serial.print("°F");                 // Degrees F sign
    Serial.println();
    delay(4000);
    Serial.println();
    Serial.print("DHT2 Information");
    Serial.println();
    Serial.print("Humidity: ");
    Serial.print(hum2);
    Serial.print(" %\t");                 // percentage sign
    Serial.println();
    Serial.print("Temperature (°C): ");
    Serial.print(tempC2);
    Serial.print("°C");                 // Degrees C sign
    Serial.println();
    Serial.print("Temperature (°F): ");
    Serial.print(tempF2);
    Serial.print("°F");                 // Degrees F sign
    Serial.println();
    delay(4000);
    Serial.println();
    Serial.print("DHT3 Information");
    Serial.println();
    Serial.print("Humidity: ");
    Serial.print(hum3);
    Serial.print(" %\t");                 // percentage sign
    Serial.println();
    Serial.print("Temperature (°C): ");
    Serial.print(tempC3);
    Serial.print("°C");                 // Degrees C sign
    Serial.println();
    Serial.print("Temperature (°F): ");
    Serial.print(tempF3);
    Serial.print("°F");                 // Degrees F sign
    Serial.println();
    delay(4000);
    Serial.println();
    Serial.print("DHT4 Information");
    Serial.println();
    Serial.print("Humidity: ");
    Serial.print(hum4);
    Serial.print(" %\t");                 // percentage sign
    Serial.println();
    Serial.print("Temperature (°C): ");
    Serial.print(tempC4);
    Serial.print("°C");                 // Degrees C sign
    Serial.println();
    Serial.print("Temperature (°F): ");
    Serial.print(tempF4);
    Serial.print("°F");                 // Degrees F sign
    Serial.println();
    delay(4000);
  // Display the Data for the DS18B20 Temp Sensors (In a group, for mine I will display upto 8)
  // Number of Devices connected
    Serial.print("Number of Devices found on bus = ");
    Serial.println(sensors.getDeviceCount());
    Serial.print("Getting temperatures... ");
    Serial.println();
    delay(1000);
    sensors.requestTemperatures();
    Serial.print("Temperature °C = ");
  	Serial.print(p1);
  	Serial.println();
  	Serial.print("Temperature °C = ");
  	Serial.println(p2);
  	Serial.print("Temperature °C = ");
  	Serial.println(p3);
  	Serial.print("Temperature °F = ");
  	Serial.print(pf1);
    Serial.print("Temperature °F = ");
    Serial.print(pf2);
    Serial.print("Temperature °F = ");
    Serial.print(pf3);
  	Serial.println();
    delay(4000);
}
