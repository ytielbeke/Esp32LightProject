<H1>Fuctions you can use</H1>

First add the mqtt.java to your file:

```JAVA

 mqtt mqtt = new mqtt(getApplicationContext());
 ```
 
**Callback function:** 
 
 ```JAVA
 
 mqtt.setCallback(new MqttCallbackExtended(){
 
 ```
 You get four posible actions.
 
  **Publish function:**
 
 ```JAVA
 
 mqtt.publish( <topic> , <payload> );
 
 ```
 
 
 
