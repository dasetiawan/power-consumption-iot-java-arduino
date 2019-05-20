## Electric Power Consumption Monitor using Arduino, Java, and Flutter
  This project is an IoT system that reads electric power consumption of a company. The data of the power consumption is available in the Thingsboard cloud platform for the public to view using our flutter app, which the source code is in the lib folder.
  
### Arduino Server Device
  The server device retreives data from sensors (In this case, the data is hard coded in the server device and is using dummy data) and sends it to the client device using Bluetooth Low Power (BLE) connection.

### Arduino Client Device
  The client device retreives data from the server device through BLE connection. The client device is directly connected to the gateway through serial port. So the data is forwarded to the gateway through it.
  
### Gateway PC (Coded with Java)
  Using java serial reader, the gateway scans for incoming data from the serial port conencted to the client device. The retreived data is sent to the Thingsboard API using Http JSON request.
