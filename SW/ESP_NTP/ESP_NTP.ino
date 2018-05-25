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

struct secodsWithMillis
{
  unsigned long seconds;
  int           millisec;
};

struct color
{
  byte r;
  byte g;
  byte b;
  byte w;
};

secodsWithMillis timeDifference;

color colores[10];

int millisInicio;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();



  colores[0] = {  0,  0,  0};
  colores[1] = {  0,  0,255};
  colores[2] = {  0,255,  0};
  colores[3] = {  0,255,255};
  colores[4] = {255,  0,  0};
  colores[5] = {255,  0,255};
  colores[6] = {255,255,  0};
  colores[7] = {255,255,255};
  colores[8] = {128,  0,128};
  colores[9] = {255,165,  0};

  Serial.print ("colores {");
  Serial.print (colores[9].r); Serial.print (", ");
  Serial.print (colores[9].g); Serial.print (", ");
  Serial.print (colores[9].b); Serial.println ("}");

  pixels.begin();

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    for (int i=0; i<8; i++)
    {
      for (int j=0; j<5; j++)
      {
        pixels.setPixelColor(j, pixels.Color(colores[i].r, colores[i].g, colores[i].b));
        pixels.show();
        delay (10);
      }
    }
    Serial.print(".");
  }
  Serial.println("");

  for (int j=0; j<5; j++)
  {
    pixels.setPixelColor(j, pixels.Color(0,0,0));
  }
  pixels.show();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

  //secondsSice1900 = get_NTP_seconds();
  //deltaMiliseconds = millis();

  secodsWithMillis extTime;




  delay (1000);
  extTime.seconds = 0;
  int i = 0;   
  while (extTime.seconds == 0)
  {
    for (int j=0; j<5; j++)
    {
      if (i==j) pixels.setPixelColor(i, pixels.Color(100,25,125));
      else      pixels.setPixelColor(j, pixels.Color(0,0,0));
    }
    pixels.show();
    i++;
    i = i%5;

    extTime = setLocalTime();
    millisInicio = millis();

  }
  
  printTime(extTime);
  Serial.println ();

  
  delay (1000);
  for (int j=0; j<5; j++)
    pixels.setPixelColor(j, pixels.Color(0,0,0));
  pixels.show();  
}

void loop()
{
  int actualSecond;
  secodsWithMillis extTime;
  secodsWithMillis extTime2;


  extTime2 = getLocalTime();
  int colorIndex;
  static int colorIndexAnt;

  colorIndex = extTime2.seconds % 10;
  for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(colores[colorIndex].r,
                                             colores[colorIndex].g,
                                             colores[colorIndex].b));

  if (colorIndex != colorIndexAnt)
  {
    Serial.print (int((millis()- millisInicio)/1000));
    Serial.print (" s\t\t");
    printTime(extTime2);
    Serial.println ();
    
    colorIndexAnt = colorIndex;
  }

  pixels.show();
  delay(50); 
  
}

/***************************************************************************
   This function sets the parameters for calculating the actual time from 
   the milis() function
***************************************************************************/
void printTime(secodsWithMillis extTime)
{
  String timeText;
  timeText = String (extTime.millisec);
  if      (extTime.millisec ==  0) timeText = "000";
  else if (extTime.millisec <  10) timeText =  "00" + timeText;
  else if (extTime.millisec < 100) timeText =   "0" + timeText;

  timeText = String (extTime.seconds) + "." + timeText;
  
  Serial.print (timeText);
}


/***************************************************************************
   This function retuns an struct with seconds since 1990, and milliseconds
   so it can be sincronized with the millis() function
***************************************************************************/
secodsWithMillis get_NTP_seconds()
{
  secodsWithMillis millisSince1990;
  int cb = 0;
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


    millisSince1990.seconds  = highWord << 16 | lowWord;
    millisSince1990.millisec = packetBuffer[44]*1000/256;

     
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.print(millisSince1990.seconds);
    Serial.print(".");
    Serial.print(millisSince1990.millisec);
    Serial.println();
    
    return millisSince1990;
  }
}

/***************************************************************************
   This function sets the parameters for calculating the actual time from 
   the milis() function
***************************************************************************/
secodsWithMillis setLocalTime()
{
  
  secodsWithMillis extTime;
  unsigned long milliseconds;
  
  extTime = get_NTP_seconds();

  milliseconds = millis();
  timeDifference.millisec = extTime.millisec - milliseconds%1000;
  timeDifference.seconds  = extTime.seconds  - milliseconds/1000;
  if (timeDifference.millisec < 0)
  {
    timeDifference.millisec += 1000;
    timeDifference.seconds  -= 1;
  }
  return timeDifference;

}


/***************************************************************************
   This function retuns the time calculated fron the internal 
   milis() function
***************************************************************************/
secodsWithMillis getLocalTime()
{
  secodsWithMillis extTime;
  unsigned long milliseconds;

  milliseconds = millis();
  extTime.millisec = timeDifference.millisec + (milliseconds%1000);
  extTime.seconds  = timeDifference.seconds  + milliseconds/1000;
  if (extTime.millisec > 999)
  {
    extTime.millisec -= 1000;
    extTime.seconds  += 1;
  }

  return extTime;
  
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

