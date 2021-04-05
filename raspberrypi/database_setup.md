<H2>Setup database</H2>

```
sudo apt update
sudo apt upgrade
sudo apt install mariadb-server
```

Then remove some settings and add stuff:

edit as you want

```
sudo mysql_secure_installation
```

<H3>Create User and Database</H3>

Open MariaDB as root:
```
sudo mysql -u root -p
```

Create a database

```
CREATE DATABASE sensors;
```

Create a user

```
CREATE USER 'username'@'localhost' IDENTIFIED BY 'Password';
```

give the user permisions to edit all the tables;

```
GRANT ALL PRIVILEGES ON sensors.* TO 'username'@'localhost';
```

# network access:
edit the mysql configure file to change the access.
```
sudo nano /etc/mysql/mariadb.conf.d/50-server.cnf
```
 
**change the bind-address to 0.0.0.0**
