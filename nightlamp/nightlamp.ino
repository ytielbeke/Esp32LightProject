#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

#define PIN 27
#define nr_leds 116
#define delaytime 1000
#define set_brightness 10

const char* ssid = "raspberry";
const char* password = "Wachtwoord1!";

const char* mqtt_server = "192.168.4.1";

WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(nr_leds, PIN, NEO_GRB + NEO_KHZ800);

void black();
void number(int where, int number, int r,int g, int b);
void set_light(int r, int g, int b, int brightness);
void print_time(int thousend, int hounderd, int tens, int ones);


int thousend, hounderd, tens, ones;
int old_thousend, old_hounderd, old_tens, old_ones;
int light;

void setup() {
  Serial.begin(115200);
  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pixels.setBrightness(set_brightness);

  old_thousend = 11;
  old_hounderd = 11;
  old_tens = 11;
  old_ones = 11;

  light = 0;
}

void loop() {
  if (!client.connected()) {
   reconnect();
  }

  client.loop();
}

void setup_wifi(){
  delay (10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("nightlamp/time");
      client.subscribe("nightlamp/light");
      client.subscribe("nightlamp/brightness");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  //Serial.println(messageTemp);
  //String S_topic = String(topic);

  if(String(topic).equals("nightlamp/time")){
    
    int msg = messageTemp.toInt();

    thousend = (msg / 1000) % 10;
    hounderd = (msg / 100) % 10;
    tens = (msg / 10) % 10;
    ones = msg % 10;
    
    if(light == !1){
      print_time(thousend,hounderd,tens,ones);
    }

    /*Serial.println("-------------------------------------------------");

    Serial.println(hounderd);
    Serial.println(tens);
    Serial.println(ones);
    Serial.println(messageTemp);

    Serial.println("-------------------------------------------------");
    */
  }

  if(String(topic).equals("nightlamp/light")){
    String message = messageTemp;
    long int msg = message.toInt();
    Serial.println();
    Serial.println(msg);
    if(msg == 0){
      black();
      light = 0;
      old_thousend = 11;
      old_hounderd = 11;
      old_tens = 11;
      old_ones = 11;
      print_time(thousend,hounderd,tens,ones);
    }else{
      black();
      light = 1;
      int help =(((msg / 100000000000) % 10)*100);
      //int brightness = (((msg / 100000000000) % 10)*100)+(((msg / 10000000000) % 10) * 10)+((msg / 1000000000) % 10);
      int red = (((msg / 100000000) % 10)*100)+(((msg / 10000000) % 10) * 10)+((msg / 1000000) % 10);
      int green = (((msg / 100000) % 10)*100)+(((msg / 10000) % 10) * 10)+((msg / 1000) % 10);
      int blue = (((msg / 100) % 10)*100)+(((msg / 10) % 10) * 10)+(msg % 10);

      Serial.println("-------------------------------------------------");
      Serial.println(help);
      //Serial.println(brightness);
      Serial.println(red);
      Serial.println(green);
      Serial.println(blue);
      Serial.println("-------------------------------------------------");
      
      for(int l = 0; l < 166; l++){
        pixels.setPixelColor(l, pixels.Color(red,green,blue));
      }
      
      pixels.show();
    }
  }

  if(String(topic).equals("nightlamp/brightness")){
    String message = messageTemp;
    int msg = message.toInt();

    pixels.setBrightness(msg);
    pixels.show();
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}

void print_time(int thousend, int hounderd, int tens, int ones){
  
    if(old_thousend != thousend){
      if(thousend == 1){
        number(1,1,136,0,255);
      }else{
        number(1,11,136,0,255);
      }
    }
    if(old_hounderd != hounderd){
      number(2,11,136,0,255);
      number(2,hounderd,136,0,255);
    }
    if(old_tens != tens){
      number(3,11,136,0,0);
      number(3,tens,136,0,0);
    }
    if(old_ones != ones){
      number(4,11,136,0,0);
      number(4,ones,255,0,0);
      pixels.show();
    }
    
    old_thousend = thousend;
    old_hounderd = hounderd;
    old_tens = tens;
    old_ones = ones;
}

void black(){
    for(int l = 0; l < 166; l++){
      pixels.setPixelColor(l, pixels.Color(0,0,0));
    }
    pixels.show();
}

void set_light(int r, int g, int b, int brightness){
    float factor = 1 / brightness;
    for(int l = 0; l < 166; l++){
      //pixels.setPixelColor(l, pixels.Color(0,0,0));
      pixels.setPixelColor(l, pixels.Color(r,g,b));
    }
    pixels.setBrightness(brightness);
    pixels.show();
}

void number(int where, int number, int r,int g, int b){
  
    //  21    23  24  25      27  28  29      31  32  33
    //  47    45      43      41      39      37      35
    //  52    54  55  56      58  59  60      62  63  64
    //  79    77      75      73      71      69      67
    //  83    85  86  87      89  90  91      93  94  95     
  
    switch(where){
      case 1:
         //  20  21  22
         //  48      46
         //  51  52  53
         //  80      78
         //  82  83  84

        switch(number){
          case 0:
            pixels.setPixelColor(20, pixels.Color(r,g,b));
            pixels.setPixelColor(21, pixels.Color(r,g,b));
            pixels.setPixelColor(22, pixels.Color(r,g,b));
            pixels.setPixelColor(46, pixels.Color(r,g,b));
            pixels.setPixelColor(48, pixels.Color(r,g,b));
            pixels.setPixelColor(51, pixels.Color(r,g,b));
            pixels.setPixelColor(53, pixels.Color(r,g,b));
            pixels.setPixelColor(78, pixels.Color(r,g,b));
            pixels.setPixelColor(80, pixels.Color(r,g,b));
            pixels.setPixelColor(82, pixels.Color(r,g,b));
            pixels.setPixelColor(83, pixels.Color(r,g,b));
            pixels.setPixelColor(84, pixels.Color(r,g,b));
            break;
          case 1:
            pixels.setPixelColor(20, pixels.Color(r,g,b));
            pixels.setPixelColor(48, pixels.Color(r,g,b));
            pixels.setPixelColor(51, pixels.Color(r,g,b));
            pixels.setPixelColor(80, pixels.Color(r,g,b));
            pixels.setPixelColor(82, pixels.Color(r,g,b));
            break;
          case 2:
            pixels.setPixelColor(20, pixels.Color(r,g,b));
            pixels.setPixelColor(21, pixels.Color(r,g,b));
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(46, pixels.Color(r,g,b));
            pixels.setPixelColor(51, pixels.Color(r,g,b));
            pixels.setPixelColor(52, pixels.Color(r,g,b));
            pixels.setPixelColor(53, pixels.Color(r,g,b));
            pixels.setPixelColor(80, pixels.Color(r,g,b));
            pixels.setPixelColor(82, pixels.Color(r,g,b));
            pixels.setPixelColor(83, pixels.Color(r,g,b));
            pixels.setPixelColor(84, pixels.Color(r,g,b));
            break;
          case 3:
            pixels.setPixelColor(20, pixels.Color(r,g,b));
            pixels.setPixelColor(21, pixels.Color(r,g,b));
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(46, pixels.Color(r,g,b));
            pixels.setPixelColor(51, pixels.Color(r,g,b));
            pixels.setPixelColor(52, pixels.Color(r,g,b));
            pixels.setPixelColor(53, pixels.Color(r,g,b));
            pixels.setPixelColor(78, pixels.Color(r,g,b));
            pixels.setPixelColor(82, pixels.Color(r,g,b));
            pixels.setPixelColor(83, pixels.Color(r,g,b));
            pixels.setPixelColor(84, pixels.Color(r,g,b));
            break;
          case 4:
            pixels.setPixelColor(20, pixels.Color(r,g,b));
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(46, pixels.Color(r,g,b));
            pixels.setPixelColor(48, pixels.Color(r,g,b));
            pixels.setPixelColor(51, pixels.Color(r,g,b));
            pixels.setPixelColor(52, pixels.Color(r,g,b));
            pixels.setPixelColor(53, pixels.Color(r,g,b));
            pixels.setPixelColor(78, pixels.Color(r,g,b));
            pixels.setPixelColor(84, pixels.Color(r,g,b));
            break;
          case 5:
            pixels.setPixelColor(20, pixels.Color(r,g,b));
            pixels.setPixelColor(21, pixels.Color(r,g,b));
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(48, pixels.Color(r,g,b));
            pixels.setPixelColor(51, pixels.Color(r,g,b));
            pixels.setPixelColor(52, pixels.Color(r,g,b));
            pixels.setPixelColor(53, pixels.Color(r,g,b));
            pixels.setPixelColor(78, pixels.Color(r,g,b));
            pixels.setPixelColor(82, pixels.Color(r,g,b));
            pixels.setPixelColor(83, pixels.Color(r,g,b));
            pixels.setPixelColor(84, pixels.Color(r,g,b));
            break;
          case 6:
            pixels.setPixelColor(20, pixels.Color(r,g,b));
            pixels.setPixelColor(21, pixels.Color(r,g,b));
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(48, pixels.Color(r,g,b));
            pixels.setPixelColor(51, pixels.Color(r,g,b));
            pixels.setPixelColor(52, pixels.Color(r,g,b));
            pixels.setPixelColor(53, pixels.Color(r,g,b));
            pixels.setPixelColor(78, pixels.Color(r,g,b));
            pixels.setPixelColor(80, pixels.Color(r,g,b));
            pixels.setPixelColor(82, pixels.Color(r,g,b));
            pixels.setPixelColor(83, pixels.Color(r,g,b));
            pixels.setPixelColor(84, pixels.Color(r,g,b));
            break;
          case 7:
            pixels.setPixelColor(20, pixels.Color(r,g,b));
            pixels.setPixelColor(21, pixels.Color(r,g,b));
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(46, pixels.Color(r,g,b));
            pixels.setPixelColor(53, pixels.Color(r,g,b));
            pixels.setPixelColor(78, pixels.Color(r,g,b));
            pixels.setPixelColor(84, pixels.Color(r,g,b));
            break;
          case 8:
            pixels.setPixelColor(20, pixels.Color(r,g,b));
            pixels.setPixelColor(21, pixels.Color(r,g,b));
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(46, pixels.Color(r,g,b));
            pixels.setPixelColor(48, pixels.Color(r,g,b));
            pixels.setPixelColor(51, pixels.Color(r,g,b));
            pixels.setPixelColor(52, pixels.Color(r,g,b));
            pixels.setPixelColor(53, pixels.Color(r,g,b));
            pixels.setPixelColor(78, pixels.Color(r,g,b));
            pixels.setPixelColor(80, pixels.Color(r,g,b));
            pixels.setPixelColor(82, pixels.Color(r,g,b));
            pixels.setPixelColor(83, pixels.Color(r,g,b));
            pixels.setPixelColor(84, pixels.Color(r,g,b));
            break;
          case 9:
            pixels.setPixelColor(20, pixels.Color(r,g,b));
            pixels.setPixelColor(21, pixels.Color(r,g,b));
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(46, pixels.Color(r,g,b));
            pixels.setPixelColor(48, pixels.Color(r,g,b));
            pixels.setPixelColor(51, pixels.Color(r,g,b));
            pixels.setPixelColor(52, pixels.Color(r,g,b));
            pixels.setPixelColor(53, pixels.Color(r,g,b));
            pixels.setPixelColor(78, pixels.Color(r,g,b));
            pixels.setPixelColor(82, pixels.Color(r,g,b));
            pixels.setPixelColor(83, pixels.Color(r,g,b));
            pixels.setPixelColor(84, pixels.Color(r,g,b));
            break;
          case 11:
            pixels.setPixelColor(20, pixels.Color(0,0,0));
            pixels.setPixelColor(21, pixels.Color(0,0,0));
            pixels.setPixelColor(23, pixels.Color(0,0,0));
            pixels.setPixelColor(46, pixels.Color(0,0,0));
            pixels.setPixelColor(48, pixels.Color(0,0,0));
            pixels.setPixelColor(51, pixels.Color(0,0,0));
            pixels.setPixelColor(52, pixels.Color(0,0,0));
            pixels.setPixelColor(53, pixels.Color(0,0,0));
            pixels.setPixelColor(78, pixels.Color(0,0,0));
            pixels.setPixelColor(80, pixels.Color(0,0,0));
            pixels.setPixelColor(82, pixels.Color(0,0,0));
            pixels.setPixelColor(83, pixels.Color(0,0,0));
            pixels.setPixelColor(84, pixels.Color(0,0,0));
            break;
            
          default:
            pixels.setPixelColor(20, pixels.Color(r,g,b));
            pixels.show();
            delay(delaytime);
            pixels.setPixelColor(21, pixels.Color(r,g,b));
            pixels.show();
            delay(delaytime);
            pixels.setPixelColor(22, pixels.Color(r,g,b));
            pixels.show();
            delay(delaytime);
            pixels.setPixelColor(48, pixels.Color(r,g,b));
            pixels.show();
            delay(delaytime);
            pixels.setPixelColor(46, pixels.Color(r,g,b));
            pixels.show();
            delay(delaytime);
            pixels.setPixelColor(51, pixels.Color(r,g,b));
            pixels.show();
            delay(delaytime);
            pixels.setPixelColor(52, pixels.Color(r,g,b));
            pixels.show();
            delay(delaytime);
            pixels.setPixelColor(53, pixels.Color(r,g,b));
            pixels.show();
            delay(delaytime);
            pixels.setPixelColor(80, pixels.Color(r,g,b));
            pixels.show();
            delay(delaytime);
            pixels.setPixelColor(78, pixels.Color(r,g,b));
            pixels.show();
            delay(delaytime);
            pixels.setPixelColor(82, pixels.Color(r,g,b));
            pixels.show();
            delay(delaytime);
            pixels.setPixelColor(83, pixels.Color(r,g,b));
            pixels.show();
            delay(delaytime);
            pixels.setPixelColor(84, pixels.Color(r,g,b));
            
            break;
        }
        break;
      case 2:
        //  21    23  24  25      27  28  29      31  32  33
        //  47    45      43      41      39      37      35
        //  52    54  55  56      58  59  60      62  63  64
        //  79    77      75      73      71      69      67
        //  83    85  86  87      89  90  91      93  94  95  
        switch(number){
          case 0:
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(24, pixels.Color(r,g,b));
            pixels.setPixelColor(25, pixels.Color(r,g,b));
            pixels.setPixelColor(45, pixels.Color(r,g,b));
            pixels.setPixelColor(43, pixels.Color(r,g,b));
            pixels.setPixelColor(54, pixels.Color(r,g,b));
            pixels.setPixelColor(56, pixels.Color(r,g,b));
            pixels.setPixelColor(77, pixels.Color(r,g,b));
            pixels.setPixelColor(75, pixels.Color(r,g,b));
            pixels.setPixelColor(85, pixels.Color(r,g,b));
            pixels.setPixelColor(86, pixels.Color(r,g,b));
            pixels.setPixelColor(87, pixels.Color(r,g,b));
            break;
          case 1:
            pixels.setPixelColor(24, pixels.Color(r,g,b));
            pixels.setPixelColor(44, pixels.Color(r,g,b));
            pixels.setPixelColor(55, pixels.Color(r,g,b));
            pixels.setPixelColor(76, pixels.Color(r,g,b));
            pixels.setPixelColor(86, pixels.Color(r,g,b));
            break;
          case 2:
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(24, pixels.Color(r,g,b));
            pixels.setPixelColor(25, pixels.Color(r,g,b));
            pixels.setPixelColor(43, pixels.Color(r,g,b));
            pixels.setPixelColor(54, pixels.Color(r,g,b));
            pixels.setPixelColor(55, pixels.Color(r,g,b));
            pixels.setPixelColor(56, pixels.Color(r,g,b));
            pixels.setPixelColor(77, pixels.Color(r,g,b));
            pixels.setPixelColor(85, pixels.Color(r,g,b));
            pixels.setPixelColor(86, pixels.Color(r,g,b));
            pixels.setPixelColor(87, pixels.Color(r,g,b));
            break;
          case 3:
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(24, pixels.Color(r,g,b));
            pixels.setPixelColor(25, pixels.Color(r,g,b));
            pixels.setPixelColor(43, pixels.Color(r,g,b));
            pixels.setPixelColor(54, pixels.Color(r,g,b));
            pixels.setPixelColor(55, pixels.Color(r,g,b));
            pixels.setPixelColor(56, pixels.Color(r,g,b));
            pixels.setPixelColor(75, pixels.Color(r,g,b));
            pixels.setPixelColor(85, pixels.Color(r,g,b));
            pixels.setPixelColor(86, pixels.Color(r,g,b));
            pixels.setPixelColor(87, pixels.Color(r,g,b));
            break;
          case 4:
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(25, pixels.Color(r,g,b));
            pixels.setPixelColor(45, pixels.Color(r,g,b));
            pixels.setPixelColor(43, pixels.Color(r,g,b));
            pixels.setPixelColor(54, pixels.Color(r,g,b));
            pixels.setPixelColor(55, pixels.Color(r,g,b));
            pixels.setPixelColor(56, pixels.Color(r,g,b));
            pixels.setPixelColor(75, pixels.Color(r,g,b));
            pixels.setPixelColor(87, pixels.Color(r,g,b));
            break;
          case 5:
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(24, pixels.Color(r,g,b));
            pixels.setPixelColor(25, pixels.Color(r,g,b));
            pixels.setPixelColor(45, pixels.Color(r,g,b));
            pixels.setPixelColor(54, pixels.Color(r,g,b));
            pixels.setPixelColor(55, pixels.Color(r,g,b));
            pixels.setPixelColor(56, pixels.Color(r,g,b));
            pixels.setPixelColor(75, pixels.Color(r,g,b));
            pixels.setPixelColor(85, pixels.Color(r,g,b));
            pixels.setPixelColor(86, pixels.Color(r,g,b));
            pixels.setPixelColor(87, pixels.Color(r,g,b));
            break;
          case 6:
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(24, pixels.Color(r,g,b));
            pixels.setPixelColor(25, pixels.Color(r,g,b));
            pixels.setPixelColor(45, pixels.Color(r,g,b));
            pixels.setPixelColor(54, pixels.Color(r,g,b));
            pixels.setPixelColor(55, pixels.Color(r,g,b));
            pixels.setPixelColor(56, pixels.Color(r,g,b));
            pixels.setPixelColor(77, pixels.Color(r,g,b));
            pixels.setPixelColor(75, pixels.Color(r,g,b));
            pixels.setPixelColor(85, pixels.Color(r,g,b));
            pixels.setPixelColor(86, pixels.Color(r,g,b));
            pixels.setPixelColor(87, pixels.Color(r,g,b));
            break;
          case 7:
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(24, pixels.Color(r,g,b));
            pixels.setPixelColor(25, pixels.Color(r,g,b));
            pixels.setPixelColor(43, pixels.Color(r,g,b));
            pixels.setPixelColor(56, pixels.Color(r,g,b));
            pixels.setPixelColor(75, pixels.Color(r,g,b));
            pixels.setPixelColor(87, pixels.Color(r,g,b));
            break;
          case 8:
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(24, pixels.Color(r,g,b));
            pixels.setPixelColor(25, pixels.Color(r,g,b));
            pixels.setPixelColor(45, pixels.Color(r,g,b));
            pixels.setPixelColor(43, pixels.Color(r,g,b));
            pixels.setPixelColor(54, pixels.Color(r,g,b));
            pixels.setPixelColor(55, pixels.Color(r,g,b));
            pixels.setPixelColor(56, pixels.Color(r,g,b));
            pixels.setPixelColor(77, pixels.Color(r,g,b));
            pixels.setPixelColor(75, pixels.Color(r,g,b));
            pixels.setPixelColor(85, pixels.Color(r,g,b));
            pixels.setPixelColor(86, pixels.Color(r,g,b));
            pixels.setPixelColor(87, pixels.Color(r,g,b));
            break;
          case 9:
            pixels.setPixelColor(23, pixels.Color(r,g,b));
            pixels.setPixelColor(24, pixels.Color(r,g,b));
            pixels.setPixelColor(25, pixels.Color(r,g,b));
            pixels.setPixelColor(45, pixels.Color(r,g,b));
            pixels.setPixelColor(43, pixels.Color(r,g,b));
            pixels.setPixelColor(54, pixels.Color(r,g,b));
            pixels.setPixelColor(55, pixels.Color(r,g,b));
            pixels.setPixelColor(56, pixels.Color(r,g,b));
            pixels.setPixelColor(75, pixels.Color(r,g,b));
            pixels.setPixelColor(85, pixels.Color(r,g,b));
            pixels.setPixelColor(86, pixels.Color(r,g,b));
            pixels.setPixelColor(87, pixels.Color(r,g,b));
            break;
          case 11:
            pixels.setPixelColor(23, pixels.Color(0,0,0));
            pixels.setPixelColor(24, pixels.Color(0,0,0));
            pixels.setPixelColor(25, pixels.Color(0,0,0));
            pixels.setPixelColor(45, pixels.Color(0,0,0));
            pixels.setPixelColor(44, pixels.Color(0,0,0));
            pixels.setPixelColor(43, pixels.Color(0,0,0));
            pixels.setPixelColor(54, pixels.Color(0,0,0));
            pixels.setPixelColor(55, pixels.Color(0,0,0));
            pixels.setPixelColor(56, pixels.Color(0,0,0));
            pixels.setPixelColor(77, pixels.Color(0,0,0));
            pixels.setPixelColor(76, pixels.Color(0,0,0));
            pixels.setPixelColor(75, pixels.Color(0,0,0));
            pixels.setPixelColor(85, pixels.Color(0,0,0));
            pixels.setPixelColor(86, pixels.Color(0,0,0));
            pixels.setPixelColor(87, pixels.Color(0,0,0));
            break;
        }
        break;
      case 3:
        //  21    23  24  25      27  28  29      31  32  33
        //  47    45      43      41      39      37      35
        //  52    54  55  56      58  59  60      62  63  64
        //  79    77      75      73      71      69      67
        //  83    85  86  87      89  90  91      93  94  95  
        switch(number){
          case 0:
            pixels.setPixelColor(27, pixels.Color(r,g,b));
            pixels.setPixelColor(28, pixels.Color(r,g,b));
            pixels.setPixelColor(29, pixels.Color(r,g,b));
            pixels.setPixelColor(41, pixels.Color(r,g,b));
            pixels.setPixelColor(39, pixels.Color(r,g,b));
            pixels.setPixelColor(58, pixels.Color(r,g,b));
            pixels.setPixelColor(60, pixels.Color(r,g,b));
            pixels.setPixelColor(73, pixels.Color(r,g,b));
            pixels.setPixelColor(71, pixels.Color(r,g,b));
            pixels.setPixelColor(89, pixels.Color(r,g,b));
            pixels.setPixelColor(90, pixels.Color(r,g,b));
            pixels.setPixelColor(91, pixels.Color(r,g,b));
            break;
          case 1:
            pixels.setPixelColor(28, pixels.Color(r,g,b));
            pixels.setPixelColor(40, pixels.Color(r,g,b));
            pixels.setPixelColor(59, pixels.Color(r,g,b));
            pixels.setPixelColor(72, pixels.Color(r,g,b));
            pixels.setPixelColor(90, pixels.Color(r,g,b));
            break;
          case 2:
            pixels.setPixelColor(27, pixels.Color(r,g,b));
            pixels.setPixelColor(28, pixels.Color(r,g,b));
            pixels.setPixelColor(29, pixels.Color(r,g,b));
            pixels.setPixelColor(39, pixels.Color(r,g,b));
            pixels.setPixelColor(58, pixels.Color(r,g,b));
            pixels.setPixelColor(59, pixels.Color(r,g,b));
            pixels.setPixelColor(60, pixels.Color(r,g,b));
            pixels.setPixelColor(73, pixels.Color(r,g,b));
            pixels.setPixelColor(89, pixels.Color(r,g,b));
            pixels.setPixelColor(90, pixels.Color(r,g,b));
            pixels.setPixelColor(91, pixels.Color(r,g,b));
            break;
          case 3:
            pixels.setPixelColor(27, pixels.Color(r,g,b));
            pixels.setPixelColor(28, pixels.Color(r,g,b));
            pixels.setPixelColor(29, pixels.Color(r,g,b));
            pixels.setPixelColor(39, pixels.Color(r,g,b));
            pixels.setPixelColor(58, pixels.Color(r,g,b));
            pixels.setPixelColor(59, pixels.Color(r,g,b));
            pixels.setPixelColor(60, pixels.Color(r,g,b));
            pixels.setPixelColor(71, pixels.Color(r,g,b));
            pixels.setPixelColor(89, pixels.Color(r,g,b));
            pixels.setPixelColor(90, pixels.Color(r,g,b));
            pixels.setPixelColor(91, pixels.Color(r,g,b));
            break;
          case 4:
            pixels.setPixelColor(27, pixels.Color(r,g,b));
            pixels.setPixelColor(29, pixels.Color(r,g,b));
            pixels.setPixelColor(41, pixels.Color(r,g,b));
            pixels.setPixelColor(39, pixels.Color(r,g,b));
            pixels.setPixelColor(58, pixels.Color(r,g,b));
            pixels.setPixelColor(59, pixels.Color(r,g,b));
            pixels.setPixelColor(60, pixels.Color(r,g,b));
            pixels.setPixelColor(71, pixels.Color(r,g,b));
            pixels.setPixelColor(91, pixels.Color(r,g,b));
            break;
          case 5:
            pixels.setPixelColor(27, pixels.Color(r,g,b));
            pixels.setPixelColor(28, pixels.Color(r,g,b));
            pixels.setPixelColor(29, pixels.Color(r,g,b));
            pixels.setPixelColor(41, pixels.Color(r,g,b));
            pixels.setPixelColor(58, pixels.Color(r,g,b));
            pixels.setPixelColor(59, pixels.Color(r,g,b));
            pixels.setPixelColor(60, pixels.Color(r,g,b));
            pixels.setPixelColor(71, pixels.Color(r,g,b));
            pixels.setPixelColor(89, pixels.Color(r,g,b));
            pixels.setPixelColor(90, pixels.Color(r,g,b));
            pixels.setPixelColor(91, pixels.Color(r,g,b));
            break;
          case 6:
            pixels.setPixelColor(27, pixels.Color(r,g,b));
            pixels.setPixelColor(28, pixels.Color(r,g,b));
            pixels.setPixelColor(29, pixels.Color(r,g,b));
            pixels.setPixelColor(41, pixels.Color(r,g,b));
            pixels.setPixelColor(58, pixels.Color(r,g,b));
            pixels.setPixelColor(59, pixels.Color(r,g,b));
            pixels.setPixelColor(60, pixels.Color(r,g,b));
            pixels.setPixelColor(73, pixels.Color(r,g,b));
            pixels.setPixelColor(71, pixels.Color(r,g,b));
            pixels.setPixelColor(89, pixels.Color(r,g,b));
            pixels.setPixelColor(90, pixels.Color(r,g,b));
            pixels.setPixelColor(91, pixels.Color(r,g,b));
            break;
          case 7:
            pixels.setPixelColor(27, pixels.Color(r,g,b));
            pixels.setPixelColor(28, pixels.Color(r,g,b));
            pixels.setPixelColor(29, pixels.Color(r,g,b));
            pixels.setPixelColor(39, pixels.Color(r,g,b));
            pixels.setPixelColor(60, pixels.Color(r,g,b));
            pixels.setPixelColor(71, pixels.Color(r,g,b));
            pixels.setPixelColor(91, pixels.Color(r,g,b));
            break;
          case 8:
            pixels.setPixelColor(27, pixels.Color(r,g,b));
            pixels.setPixelColor(28, pixels.Color(r,g,b));
            pixels.setPixelColor(29, pixels.Color(r,g,b));
            pixels.setPixelColor(41, pixels.Color(r,g,b));
            pixels.setPixelColor(39, pixels.Color(r,g,b));
            pixels.setPixelColor(58, pixels.Color(r,g,b));
            pixels.setPixelColor(59, pixels.Color(r,g,b));
            pixels.setPixelColor(60, pixels.Color(r,g,b));
            pixels.setPixelColor(73, pixels.Color(r,g,b));
            pixels.setPixelColor(71, pixels.Color(r,g,b));
            pixels.setPixelColor(89, pixels.Color(r,g,b));
            pixels.setPixelColor(90, pixels.Color(r,g,b));
            pixels.setPixelColor(91, pixels.Color(r,g,b));
            break;
          case 9:
            pixels.setPixelColor(27, pixels.Color(r,g,b));
            pixels.setPixelColor(28, pixels.Color(r,g,b));
            pixels.setPixelColor(29, pixels.Color(r,g,b));
            pixels.setPixelColor(41, pixels.Color(r,g,b));
            pixels.setPixelColor(39, pixels.Color(r,g,b));
            pixels.setPixelColor(58, pixels.Color(r,g,b));
            pixels.setPixelColor(59, pixels.Color(r,g,b));
            pixels.setPixelColor(60, pixels.Color(r,g,b));
            pixels.setPixelColor(71, pixels.Color(r,g,b));
            pixels.setPixelColor(89, pixels.Color(r,g,b));
            pixels.setPixelColor(90, pixels.Color(r,g,b));
            pixels.setPixelColor(91, pixels.Color(r,g,b));
            break;
          case 11:
            pixels.setPixelColor(27, pixels.Color(0,0,0));
            pixels.setPixelColor(28, pixels.Color(0,0,0));
            pixels.setPixelColor(29, pixels.Color(0,0,0));
            pixels.setPixelColor(41, pixels.Color(0,0,0));
            pixels.setPixelColor(40, pixels.Color(0,0,0));
            pixels.setPixelColor(39, pixels.Color(0,0,0));
            pixels.setPixelColor(58, pixels.Color(0,0,0));
            pixels.setPixelColor(59, pixels.Color(0,0,0));
            pixels.setPixelColor(60, pixels.Color(0,0,0));
            pixels.setPixelColor(73, pixels.Color(0,0,0));
            pixels.setPixelColor(72, pixels.Color(0,0,0));
            pixels.setPixelColor(71, pixels.Color(0,0,0));
            pixels.setPixelColor(89, pixels.Color(0,0,0));
            pixels.setPixelColor(90, pixels.Color(0,0,0));
            pixels.setPixelColor(91, pixels.Color(0,0,0));
            break;
        }
        break;
      case 4:
        //  21    23  24  25      27  28  29      31  32  33
        //  47    45      43      41      39      37      35
        //  52    54  55  56      58  59  60      62  63  64
        //  79    77      75      73      71      69      67
        //  83    85  86  87      89  90  91      93  94  95 
        switch(number){
          case 0:
            pixels.setPixelColor(31, pixels.Color(r,g,b));
            pixels.setPixelColor(32, pixels.Color(r,g,b));
            pixels.setPixelColor(33, pixels.Color(r,g,b));
            pixels.setPixelColor(37, pixels.Color(r,g,b));
            pixels.setPixelColor(35, pixels.Color(r,g,b));
            pixels.setPixelColor(62, pixels.Color(r,g,b));
            pixels.setPixelColor(64, pixels.Color(r,g,b));
            pixels.setPixelColor(69, pixels.Color(r,g,b));
            pixels.setPixelColor(67, pixels.Color(r,g,b));
            pixels.setPixelColor(93, pixels.Color(r,g,b));
            pixels.setPixelColor(94, pixels.Color(r,g,b));
            pixels.setPixelColor(95, pixels.Color(r,g,b));
            break;
          case 1:
            pixels.setPixelColor(32, pixels.Color(r,g,b));
            pixels.setPixelColor(36, pixels.Color(r,g,b));
            pixels.setPixelColor(63, pixels.Color(r,g,b));
            pixels.setPixelColor(68, pixels.Color(r,g,b));
            pixels.setPixelColor(94, pixels.Color(r,g,b));
            break;
          case 2:
            pixels.setPixelColor(31, pixels.Color(r,g,b));
            pixels.setPixelColor(32, pixels.Color(r,g,b));
            pixels.setPixelColor(33, pixels.Color(r,g,b));
            pixels.setPixelColor(35, pixels.Color(r,g,b));
            pixels.setPixelColor(62, pixels.Color(r,g,b));
            pixels.setPixelColor(63, pixels.Color(r,g,b));
            pixels.setPixelColor(64, pixels.Color(r,g,b));
            pixels.setPixelColor(69, pixels.Color(r,g,b));
            pixels.setPixelColor(93, pixels.Color(r,g,b));
            pixels.setPixelColor(94, pixels.Color(r,g,b));
            pixels.setPixelColor(95, pixels.Color(r,g,b));
            break;
         case 3:
            pixels.setPixelColor(31, pixels.Color(r,g,b));
            pixels.setPixelColor(32, pixels.Color(r,g,b));
            pixels.setPixelColor(33, pixels.Color(r,g,b));
            pixels.setPixelColor(35, pixels.Color(r,g,b));
            pixels.setPixelColor(62, pixels.Color(r,g,b));
            pixels.setPixelColor(63, pixels.Color(r,g,b));
            pixels.setPixelColor(64, pixels.Color(r,g,b));
            pixels.setPixelColor(67, pixels.Color(r,g,b));
            pixels.setPixelColor(93, pixels.Color(r,g,b));
            pixels.setPixelColor(94, pixels.Color(r,g,b));
            pixels.setPixelColor(95, pixels.Color(r,g,b));
            break;
          case 4:
            pixels.setPixelColor(31, pixels.Color(r,g,b));
            pixels.setPixelColor(33, pixels.Color(r,g,b));
            pixels.setPixelColor(37, pixels.Color(r,g,b));
            pixels.setPixelColor(35, pixels.Color(r,g,b));
            pixels.setPixelColor(62, pixels.Color(r,g,b));
            pixels.setPixelColor(63, pixels.Color(r,g,b));
            pixels.setPixelColor(64, pixels.Color(r,g,b));
            pixels.setPixelColor(67, pixels.Color(r,g,b));
            pixels.setPixelColor(95, pixels.Color(r,g,b));
            break;
          case 5:
            pixels.setPixelColor(31, pixels.Color(r,g,b));
            pixels.setPixelColor(32, pixels.Color(r,g,b));
            pixels.setPixelColor(33, pixels.Color(r,g,b));
            pixels.setPixelColor(37, pixels.Color(r,g,b));
            pixels.setPixelColor(62, pixels.Color(r,g,b));
            pixels.setPixelColor(63, pixels.Color(r,g,b));
            pixels.setPixelColor(64, pixels.Color(r,g,b));
            pixels.setPixelColor(67, pixels.Color(r,g,b));
            pixels.setPixelColor(93, pixels.Color(r,g,b));
            pixels.setPixelColor(94, pixels.Color(r,g,b));
            pixels.setPixelColor(95, pixels.Color(r,g,b));
            break;
          case 6:
            pixels.setPixelColor(31, pixels.Color(r,g,b));
            pixels.setPixelColor(32, pixels.Color(r,g,b));
            pixels.setPixelColor(33, pixels.Color(r,g,b));
            pixels.setPixelColor(37, pixels.Color(r,g,b));
            pixels.setPixelColor(62, pixels.Color(r,g,b));
            pixels.setPixelColor(63, pixels.Color(r,g,b));
            pixels.setPixelColor(64, pixels.Color(r,g,b));
            pixels.setPixelColor(69, pixels.Color(r,g,b));
            pixels.setPixelColor(67, pixels.Color(r,g,b));
            pixels.setPixelColor(93, pixels.Color(r,g,b));
            pixels.setPixelColor(94, pixels.Color(r,g,b));
            pixels.setPixelColor(95, pixels.Color(r,g,b));
            break;
          case 7:
            pixels.setPixelColor(31, pixels.Color(r,g,b));
            pixels.setPixelColor(32, pixels.Color(r,g,b));
            pixels.setPixelColor(33, pixels.Color(r,g,b));
            pixels.setPixelColor(35, pixels.Color(r,g,b));
            pixels.setPixelColor(64, pixels.Color(r,g,b));
            pixels.setPixelColor(67, pixels.Color(r,g,b));
            pixels.setPixelColor(95, pixels.Color(r,g,b));
            break;
          case 8:
            pixels.setPixelColor(31, pixels.Color(r,g,b));
            pixels.setPixelColor(32, pixels.Color(r,g,b));
            pixels.setPixelColor(33, pixels.Color(r,g,b));
            pixels.setPixelColor(37, pixels.Color(r,g,b));
            pixels.setPixelColor(35, pixels.Color(r,g,b));
            pixels.setPixelColor(62, pixels.Color(r,g,b));
            pixels.setPixelColor(63, pixels.Color(r,g,b));
            pixels.setPixelColor(64, pixels.Color(r,g,b));
            pixels.setPixelColor(69, pixels.Color(r,g,b));
            pixels.setPixelColor(67, pixels.Color(r,g,b));
            pixels.setPixelColor(93, pixels.Color(r,g,b));
            pixels.setPixelColor(94, pixels.Color(r,g,b));
            pixels.setPixelColor(95, pixels.Color(r,g,b));
            break;
          case 9:
            pixels.setPixelColor(31, pixels.Color(r,g,b));
            pixels.setPixelColor(32, pixels.Color(r,g,b));
            pixels.setPixelColor(33, pixels.Color(r,g,b));
            pixels.setPixelColor(37, pixels.Color(r,g,b));
            pixels.setPixelColor(35, pixels.Color(r,g,b));
            pixels.setPixelColor(62, pixels.Color(r,g,b));
            pixels.setPixelColor(63, pixels.Color(r,g,b));
            pixels.setPixelColor(64, pixels.Color(r,g,b));
            pixels.setPixelColor(67, pixels.Color(r,g,b));
            pixels.setPixelColor(93, pixels.Color(r,g,b));
            pixels.setPixelColor(94, pixels.Color(r,g,b));
            pixels.setPixelColor(95, pixels.Color(r,g,b));
            break;
          case 11:
            pixels.setPixelColor(31, pixels.Color(0,0,0));
            pixels.setPixelColor(32, pixels.Color(0,0,0));
            pixels.setPixelColor(33, pixels.Color(0,0,0));
            pixels.setPixelColor(37, pixels.Color(0,0,0));
            pixels.setPixelColor(36, pixels.Color(0,0,0));
            pixels.setPixelColor(35, pixels.Color(0,0,0));
            pixels.setPixelColor(62, pixels.Color(0,0,0));
            pixels.setPixelColor(63, pixels.Color(0,0,0));
            pixels.setPixelColor(64, pixels.Color(0,0,0));
            pixels.setPixelColor(69, pixels.Color(0,0,0));
            pixels.setPixelColor(68, pixels.Color(0,0,0));
            pixels.setPixelColor(67, pixels.Color(0,0,0));
            pixels.setPixelColor(93, pixels.Color(0,0,0));
            pixels.setPixelColor(94, pixels.Color(0,0,0));
            pixels.setPixelColor(95, pixels.Color(0,0,0));
            break;
        }
        break;
    }
}
