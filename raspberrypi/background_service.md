<H3>To setup an service</H3>

```
sudo vi /lib/systemd/system/NameofYourService.service
```


***Then add:***

Edit the ***WorkingDirectory*** and the ***ExecStart*** locations.

```
[Unit]
Description=Example python App running on Ubuntu

[Service]
WorkingDirectory=/home/app
ExecStart=/usr/bin/python3 /home/app/app.py
Restart=always
# Restart service after 10 seconds if the dotnet service crashes:
RestartSec=10
KillSignal=SIGINT
SyslogIdentifier=Nameofyourapp

[Install]
WantedBy=multi-user.target
```

<H3>Start the service</H3>

```
sudo systemctl enable NameofYourService.service
sudo systemctl start NameofYourService.service
```

If you want to stop:
```
sudo systemctl stop NameofYourService.service
sudo systemctl disable NameofYourService.service
```

<H4>To see the LOG</H4>

```
sudo journalctl -fu NameofYourService.service
```


