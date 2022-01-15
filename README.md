# autotrigger_DSLR
Device to automaticly take a number of long exposure pictures designed for astrophotography. 
Works with any DSLR that has a remote trigger connector and a bulb exposure mode

## How to use
The device uses a 2.5mm headphoe jack to simulate an remote trigger. It is powered my USB. Uses SSD1306 OLED Display as Display. With 4 buttons you can change the exposure time and the number of Pictures to take. The controller is an ESP8266 (ESP-12F module), so wifi capabilities could be implemented, **but are not yet**.

1. Plug the USB into any USB Power source (Powerbank/Phone Charger/...) to power the device. Power consumtion is only 50mA so any Power source should work. Make sure you have a Power source that doesn't switch off because not enough current is detected.
2. Plug the headphone jack into the external/remote trigger socket of the camera.
3. Turn on the camera and change the settings to bulb mode for long exposures, if you want short exposures (under 1s) adjust exposure time at the camera accordingly.
4. Change exposure time on the autotrigger with the UP and DOWN buttons. (1s - 300s, range can be changed in code)
5. Press NEXT to select number of Exposures.
6. Change number of exposures to how much exposures you want UP and DOWN buttons. (1 - 100, range can be changed in code)
7. Press START to start the configured Series of Exposures.
8. While one Session is runnig, some info is displayed (exposure number, time of current exposure, total time)
9. Wait until session is finished or press NEXT to abort an session (current exposue will be finished)

## Building

### Ordering PCB and Parts

Order the PCB from a manufacturer you like, i use [JLCPCB](https://jlcpcb.com/ "JLCPCB"), by uploading the prepared gerber files.

Partslist:
| Part  | Value  | Package  | Comment  | Link  |
| ------------ | ------------ | ------------ | ------------ | ------------ |
| C1  | 10uF  | 0805  |   | https://www.lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_FH-Guangdong-Fenghua-Advanced-Tech-0805X106M160NT_C2832123.html  |
| C2  | 10uF  | 0805  |   | https://www.lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_FH-Guangdong-Fenghua-Advanced-Tech-0805X106M160NT_C2832123.html  |
| C3  | 100nF  | 0805  |   | https://www.lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_FH-BK-Guangdong-Fenghua-Bangke-Elec-0805B104J500NT_C2830722.html  |
| C4  | 10uF  | 0805  |   | https://www.lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_FH-Guangdong-Fenghua-Advanced-Tech-0805X106M160NT_C2832123.html  |
| C5  | 100nF  | 0805  |   | https://www.lcsc.com/product-detail/Multilayer-Ceramic-Capacitors-MLCC-SMD-SMT_FH-BK-Guangdong-Fenghua-Bangke-Elec-0805B104J500NT_C2830722.html  |
| D1  | 1N5819  | SOD-123  |   | https://www.lcsc.com/product-detail/Schottky-Barrier-Diodes-SBD_ZHIDE-1N5819W_C2683619.html  |
| D2  | 1N5819  | SOD-123   |   | https://www.lcsc.com/product-detail/Schottky-Barrier-Diodes-SBD_ZHIDE-1N5819W_C2683619.html  |
| F1  | 500mA  | 1812  | Polyfuse  | https://www.lcsc.com/product-detail/Resettable-Fuses_TECHFUSE-mSMD050-15V_C70110.html  |
| J1  |   | 5x2.54mm  | Pin Header Female  | https://www.lcsc.com/product-detail/Female-Headers_Ckmtw-Shenzhen-Cankemeng-B-2200S05P-A120_C124414.html  |
| J2  | SSD1306  | SSD1306  | Display, look for pinout *GND VCC SCL SDA* , 4Pin, White | https://www.aliexpress.com/item/32638662748.html |
| K1  | G6K  | G6K-2F  | Omron Relay  | https://www.lcsc.com/product-detail/Signal-Relays_Omron-Electronics-G6K-2F-Y-TR-DC5_C47190.html  |
| Q1  | MosFET  |   | N-Channel  | https://www.lcsc.com/product-detail/MOSFETs_Sinopower-Semicon-SM2326NSANC-TRG_C368561.html  |
| R1  | 10k  | 0805  |   | https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_TyoHM-RMC080510K1-N_C269742.html  |
| R2  | 10k  | 0805  |   | https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_TyoHM-RMC080510K1-N_C269742.html  |
| R3  | 1k  | 0805  |   | https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_TyoHM-RMC08051K5-N_C269727.html  |
| R4  | 10k  | 0805  |   | https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_TyoHM-RMC080510K1-N_C269742.html  |
| R5  | 5k  | 0805  |   | https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_Viking-Tech-ARG05DTC5001_C2828783.html  |
| R6  | 5k  | 0805  |   | https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_Viking-Tech-ARG05DTC5001_C2828783.html  |
| R7  | 5k  | 0805  |   | https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_Viking-Tech-ARG05DTC5001_C2828783.html  |
| R8  | 5k  | 0805  |   | https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_Viking-Tech-ARG05DTC5001_C2828783.html  |
| R9  | 1k  | 0805  |   | https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_TyoHM-RMC08051K5-N_C269727.html  |
| R10  | 10k  | 0805  |   | https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_TyoHM-RMC080510K1-N_C269742.html  |
| R11  | 1k  | 0805  |   | https://www.lcsc.com/product-detail/Chip-Resistor-Surface-Mount_TyoHM-RMC08051K5-N_C269727.html  |
| SW1  | pushbutton  | SPST SMD  |   | https://www.lcsc.com/product-detail/Tactile-Switches_XKB-Connectivity-TS-1101-C-W_C318938.html  |
| SW2  | pushbutton  | SPST SMD  |   | https://www.lcsc.com/product-detail/Tactile-Switches_XKB-Connectivity-TS-1101-C-W_C318938.html  |
| SW3  | pushbutton  | THT 6x6x10mm  |   | https://www.lcsc.com/product-detail/Tactile-Switches_G-Switch-GT-TC096A-H100-L1_C963363.html  |
| SW4  | pushbutton  | THT 6mm_H9.5mm  |   | https://www.lcsc.com/product-detail/Tactile-Switches_G-Switch-GT-TC096A-H100-L1_C963363.html  |
| SW5  | pushbutton  | THT 6mm_H9.5mm  |   | https://www.lcsc.com/product-detail/Tactile-Switches_G-Switch-GT-TC096A-H100-L1_C963363.html  |
| SW6  | pushbutton  | THT 6mm_H9.5mm  |   | https://www.lcsc.com/product-detail/Tactile-Switches_G-Switch-GT-TC096A-H100-L1_C963363.html  |
| U1  | ESP8266  | ESP-12F  |   | https://www.lcsc.com/product-detail/WiFi-Modules_Ai-Thinker-ESP-12F-ESP8266MOD_C82891.html  |
| U2  | AMS1117-3.3  | SOT-223-3  |   | https://www.lcsc.com/product-detail/Linear-Voltage-Regulators-LDO_PUOLOP-AMS1117-3-3_C351784.html  |







## Configuration and Programming

## Todo
- Mounting holes PCB need adjusdment, false measurement
