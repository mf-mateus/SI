|======== Projecto ========|
	#==== RASPBERRY ====#
		~Fresh install~
			write ect/wpa_supplicant.cnf (network) (do it via Unix VM)
			enable ssh
			apt get update
			apt get upgrade
			
			// Install Mosquito		https://randomnerdtutorials.com/how-to-install-mosquitto-broker-on-raspberry-pi/
				sudo apt install -y mosquitto mosquitto-clients
				sudo systemctl enable mosquitto.service
			
			// Install Node	Red		https://nodered.org/docs/hardware/raspberrypi
				bash <(curl -sL https://raw.githubusercontent.com/node-red/raspbian-deb-package/master/resources/update-nodejs-and-nodered)	
				cd ~/.node-red						~
				npm i node-red-dashboard

				
			// Run node Red
				node-red-start
				http://192.168.137.40:1880
				
			// Edit /etc/rc.local
				sudo nano /etc/rc.local
				su - pi -c "screen -dm -S teste /home/pi/startup"
				
			// Create startup file
				nano startup
					
					node-red-start