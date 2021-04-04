<h1>The setup of a simple MQTT client connection in Android studio</h1>

First in **build.gradle (Project: ...)** add the following line in the repositories of the buildscript:

```
maven {
url "https://repo.eclipse.org/content/repositories/paho-releases/"
}
```

then in the **build.gradle (Module: app)** add in dependencies:

```java

    compile 'org.eclipse.paho:org.eclipse.paho.client.mqttv3:1.1.0'
    compile 'org.eclipse.paho:org.eclipse.paho.android.service:1.1.1'

    implementation 'androidx.localbroadcastmanager:localbroadcastmanager:1.0.0'
```


Then add to the **AndroidManifest.xml** the following:

```JAVA

    <uses-permission android:name="android.permission.WAKE_LOCK" />
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
    <uses-permission android:name="android.permission.INTERNET" />
    <uses-permission android:name="android.permission.READ_PHONE_STATE" />
    
```

And in the application of **AndroidManifest.xml** add the following service:

```JAVA

<service android:name="org.eclipse.paho.android.service.MqttService"/>

```

This was the setup for the mqqt, now add the Mqtt.java file.
