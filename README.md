# Temperature & humidity Detector
Detecting temperature and humidity using DHT22 and send the data to database using POST request. THis is the arduino part from my GDP project called 'Forest Fire Detection Using Wireless Sensor Network Optimized by Solar Power'.

### Component used in this project:
1. Arduino Mega
2. DHT22 temperature sensor
3. LCD
4. 10K Potentiometer
5. GSM module (SIM900A)
6. Buzzer
7. 3 LED (Green,Yellow,Red)

### How it is operated?
Arduino will act as a controller to the DHT22 sensor and send the temperature and humidity value of the surrounding to GSM module using serial connection. The GSM module later send the data to the database in VPS that i set up through GPRS connection. When the temperature reaches certain threshold, Arduino will instruct GSM module to send SMS notification to the preconfigured number. It also turn on red LED and turn on the buzzer as an alert to people nearby. 

###### Circuit diagram
![Diagram](https://github.com/JohanBrainz/Arduino_TemperatureHumidity_Sensor/blob/master/Circuit%20Diagram.png "Circuit Diagram")
