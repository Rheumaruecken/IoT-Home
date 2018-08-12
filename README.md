# IoT-Home
RasPi@home
#
Hausautomation mit einem RasPi
#
-Wetterstation
#
-Brandmeldeanlage
#
-Alarmanlage
#
#
1 Hardware
#
1.1 EDV
#
1.1.1 Computer
#
      Raspberry 3B
      #
      32GB microSD
#
1.1.2 Router
#
      GL-AR300M Mini Smart Router
      UMTS-Stick
#
1.2 Sensoren
#
1.2.1 Wetterstation
1.2.2 Brandmeldeanlage
#
	Rauchmelder (MC145012) mit ESP-01 erweitert
	- umgebaute Standard-Rauchmelder
#
1.2.3 Alarmanlage
#
	Nottaster mit ESP-01
	- umgebaute Push-Buttons
#
2 Software
#
2.1 Raspi
#
2.1.1 Linux
#
      Download von Raspian
      Entpacken auf microSD
      
      sudo apt-get update
      sudo apt-get dist-upgrade
#
2.1.2 PHP7
#
      sudo apt-get install php-fpm
#
2.1.2 nginx mit SSL (Port 443)
#
      sudo apt-get install nginx
      sudo /etc/init.d/nginx start
      
      // Enable PHP in NGINX
      cd /etc/nginx
      sudo nano sites-enabled/default
	      index index.html index.htm;
	      index index.php index.html index.htm;
            
            sudo openssl dhparam -out dh2048.pem 2048
            sudo nano /etc/nginx/sites-enabled/default
                  ******************************************************
                  server {
                        listen 80;
                        return 301 https://$host$request_uri;
                  }
                  server {
                  listen 443 ssl;
                  server_name YOUR_DOMAIN_NAME;

                  add_header Strict-Transport-Security max-age=31536000;
                  ssl_protocols TLSv1 TLSv1.1 TLSv1.2;
                  ssl_ciphers ECDH+AESGCM:DH+AESGCM:ECDH+AES256:DH+AES256:ECDH+AES128:DH+AES:ECDH+3DES:DH+3DES:RSA+AESGCM:RSA+AES:RSA+3DES:!aNULL:!MD5:!DSS;
                  ssl_buffer_size 8k;
                  ssl_prefer_server_ciphers on;
                  ssl_session_cache shared:SSL:30m;
                  ssl_session_timeout 30m;

                  ssl_certificate          /etc/nginx/crt.crt;
                  ssl_certificate_key      /etc/nginx/key.key;

                  ssl_stapling on;
                  resolver 8.8.8.8;
                  ssl_stapling_verify on;

                  ssl_dhparam  /etc/nginx/dh2048.pem;
                  ssl_trusted_certificate  /etc/nginx/ca.crt;

                  location / {
                        proxy_pass http://YOUR_RPI_IP:YOUR_MOTION_STREAM_PORT;
                  }

            }
            ******************************************************
            YOUR_DOMAIN_NAME -> Your SSL secured domain name
            YOUR_RPI_IP -> Your Raspberry Pi IP address
            YOUR_MOTION_STREAM_PORT -> The port on your Raspberry Pi that you set Motion to stream to
            ******************************************************
#
	sudo service nginx reload
#
2.1.3 mariaDB
#
      sudo apt-get install mariadb-server mariadb-client
#
2.1.4 PHPmyAdmin
#
      sudo bash
      apt-get install phpmyadmin
#
2.2 Programm
#
2.2.1 Wetterstation
#
2.2.2 Brandmeldeanlage
#
	ESP-01f.py
	ESP8266-Feuer.ino
#
2.2.3 Alarmanlage
#
	bme280.py
	ESP-01.py
	ESP8266-Dash.ino
#
