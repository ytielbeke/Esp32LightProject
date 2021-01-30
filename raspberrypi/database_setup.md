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

