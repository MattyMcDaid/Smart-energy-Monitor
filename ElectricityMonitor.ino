#include <ESP8266Firebase.h>

#include <dummy.h>
#include <EmonLib.h> // Include EmonLib for energy monitoring
#include <ESP8266WiFi.h> // Include ESP8266WiFi library

EnergyMonitor emon1; // Create an instance of EnergyMonitor

const float voltage = 230.0; // Define the voltage of the system (should match your actual mains voltage)
float energyKWh = 0; // Variable to store energy in kWh
unsigned long lastMillis = 0; // Variable to store the last time the energy was updated

void setup() {
  Serial.begin(115200); // Start serial communication at 115200 baud rate

  // Initialize CT sensor (pin, calibration factor). The calibration factor needs fine-tuning
  emon1.current(A0, 111.1); // Assuming 111.1 is the calibration factor for your CT sensor
}

void loop() {
  double Irms = emon1.calcIrms(1480); // Calculate RMS current. The 1480 samples could be adjusted for your application
  double powerWatts = Irms * voltage; // Calculate power in watts

  unsigned long currentMillis = millis(); // Get the current time
  if (currentMillis - lastMillis >= 1000) { // Check if a second has passed
    double energyIncrement = (powerWatts / 1000.0) * ((currentMillis - lastMillis) / 3600000.0); // Calculate the energy increment in kWh
    energyKWh += energyIncrement; // Update the total energy
    lastMillis = currentMillis; // Update lastMillis to the current time
  }

  // Print power and energy values to the Serial Monitor
  Serial.print("Power (W): ");
  Serial.print(powerWatts);
  Serial.print("\tEnergy (KWh): "); 
  Serial.println(energyKWh); // Changed from print to println for a new line

  delay(1000); // Delay for a second before the next loop iteration
}
