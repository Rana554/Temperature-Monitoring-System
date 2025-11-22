1. Temperature Monitoring System: Use ADC, LL drivers, and clock configuration to build a 
system that measures the surrounding temperature using an analog temperature sensor.

Behavior :
    1. The temperature sensor continuously outputs an analog voltage that varies with 
       temperature. 
   2. The ADC in the microcontroller converts this analog voltage into a digital value. 
   3. The program calculates the corresponding temperature based on the sensor’s 
      characteristics. 
   4. The system keeps updating the temperature reading in real-time. 
   5. If the temperature exceeds a certain threshold, an action can be triggered — for 
      example: 
            When the temperature exceeds 40°C, an LED turns ON to indicate overheating. 
            The LED turns OFF again when the temperature falls below 40°C.

Sensor Characteristics : 
Measures temperature from 0°C to 150°C 
Gives 10 mV for each 1°C 
Example: 25°C = 0.25V 
Output range about 0V–1.5V 
