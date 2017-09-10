/********************
- www.geekstips.com
- Arduino Time Sync from NTP Server using ESP8266 WiFi module 
- Arduino code example
 ********************/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <Adafruit_NeoPixel.h>
// Which pin on the ESP8266 is connected to the NeoPixels?
#define PIN            4
// How many NeoPixels are attached to the ESP8266?
#define NUMPIXELS      5
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


char ssid[] = "Rwlan55";
char pass[] = "1395513955";



//char ssid[] = "*************";  //  your network SSID (name)
//char pass[] = "********";       // your network password


unsigned int localPort = 2390;      // local port to listen for UDP packets

/* Don't hardwire the IP address or we won't get the benefits of the pool.
 *  Lookup the IP address for the host name instead */
//IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;


unsigned long secondsSice1900;
unsigned long deltaMiliseconds;

struct secodsWithMillis
{
  unsigned long seconds;
  int           milliseconds;
};


void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

  //secondsSice1900 = get_NTP_seconds();
  //deltaMiliseconds = millis();  
}

void loop()
{
  int actualSecond;
  secodsWithMillis millisSince1990;
 
  
  while (1)
  {
    millisSince1990 = get_NTP_seconds();
    delay (800 + random (400));
    Serial.println ("   ***   ");
  }




  
  actualSecond = (millis()-deltaMiliseconds)/1000;  // calcula los segundos desde la lectura de NTP
  actualSecond = actualSecond + secondsSice1900;    // suma los segundos en aquel momento
  actualSecond = actualSecond % 10;                 // divide módulo 10

   switch (actualSecond) {
    case 0:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(0,0,0));
      break;
    case 1:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(0,0,255));
      break;
    case 2:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(0,255,0));
      break;
    case 3:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(0,255,255));
      break; 
    case 4:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(255,0,0));
      break;
    case 5:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(255,0,255));
      break;
    case 6:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(255,255,0));
      break;
    case 7:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(255,255,255));
      break;
    case 8:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(128,0,128));
      break;
    case 9:
      for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(255,165,0));
      break;              
  }
  pixels.show();
  delay(50); 
}



secodsWithMillis get_NTP_seconds()
{
  secodsWithMillis millisSince1990;
  int cb;
  int startQueringTime;
  int endQueringTime;

  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP); 
    
  startQueringTime = millis();
  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available

  while (!cb)
  {
    cb = udp.parsePacket();
    if (cb) break;
    
    endQueringTime = millis();
    if ((endQueringTime - startQueringTime) > 2000) break;
  }
  endQueringTime = millis();

  if (!cb) {
    Serial.println("no packet received");
  }
  else {
    Serial.print("packets receibed after ");
    Serial.print(endQueringTime - startQueringTime);
    Serial.println(" ms");

    Serial.print("packet received, length=");
    Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):


    
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    int milisSince1900 = packetBuffer[44]*1000/256;

    millisSince1990.seconds = highWord << 16 | lowWord;
    millisSince1990.milliseconds = packetBuffer[44]*1000/256;

    
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.print(secsSince1900);
    Serial.print(".");
    Serial.print(milisSince1900);
    Serial.println();
    
    

    //unsigned long retardo;
    //retardo = (255 - packetBuffer[44]) * 3906; // espera al comienzo del próximo segundo
    //delayMicroseconds (retardo);

    return millisSince1990;
  }
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address)
{
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

