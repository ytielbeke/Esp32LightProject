<H2>Write data to MariaDB with python</H2>

Install mysql-connector-pyton:

```
python -m pip install mysql-connector-python
```

***Open a file to use the python data from:***

first add the libary:

```
import mysql.connector
```

then connect to the Database:

```
mysql.connector.connect(
  host="192.168.2.190",
  user="mqtt_handler",
  password="MQTT"
)
```
