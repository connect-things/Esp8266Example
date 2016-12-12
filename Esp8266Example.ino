/**
 * @file ESP8266Example.cpp
 * @brief The implementation of class ESP8266. 
 * @author Francisco Calaça<chicocx@gmail.com> 
 * @date 2016.12
 * 
 * @par Copyright:
 * Copyright (c) 2016 Francisco Calaça. \n\n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version. \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * 
 * Use the lib https://github.com/connect-things/ConnectThings_ESP8266
 * You can download in Arduino Library Manager
 */


#include <SoftwareSerial.h>
#include <ConnectThings_ESP8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//Connect the data pin of Termometer (DS18B20) in Arduino pin 5
#define ONE_WIRE_BUS 5

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;


//rx-tx
//Arduino rx - 8 -> tx ESP 8266
//Arduino tx - 9 -> rx ESP 8266
SoftwareSerial uart = SoftwareSerial(8,9);
Wifi wifi = Wifi(uart);

String ssid = "IoTcel";
String pwd = "12345678";



void setup() {
  Serial.begin(9600);
  connectTermometer();
  connectWifi();
}

void connectWifi(){
  if(wifi.connect(ssid,pwd)){
    Serial.println("Wifi connected");
  }else{
    Serial.println("Error in connect");
  }
 
}

void connectTermometer(){
  Serial.println("Searching sensores DS18B20...");
  Serial.print("Found sensors: ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" sensores.");
  if (!sensors.getAddress(sensor1, 0)) {
    Serial.println("Sensors not found !");    
  }
  Serial.println();
  Serial.println();

}

void loop() {
  sensors.requestTemperatures();
  float temp = sensors.getTempC(sensor1);
  String data = "temp=" + String(temp);
  char toSend[data.length()];
  data.toCharArray(toSend, data.length());

  Serial.println(toSend);

  //change 192.168.43.79 to real ip of the server
  //The server was implemented in the project
  //https://github.com/connect-things/server-esp8266
  wifi.openTcpSocket("192.168.43.79", 8080);
  wifi.sendTcpData(toSend);
  wifi.closeTcpSocket();
  delay(2000);
  
}
