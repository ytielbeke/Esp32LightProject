<h1>The setup of a simple MQTT client connection in Android studio</h1>

First in **build.gradle (Project: ...)** add the following line in the repositories of the buildscript:

```
maven {
url "https://repo.eclipse.org/content/repositories/paho-releases/"
}
```

then in the *build.gradle (Module: app)** add in dependencies:

```java

implementation 'org.eclipse.paho:org.eclipse.paho.client.mqttv3:1.1.1'
implementation 'org.eclipse.paho:org.eclipse.paho.android.service:1.1.1'
```


