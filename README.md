![image](https://github.com/rifqieeeee/FFF_project/assets/52254229/c913a719-798e-46b1-b437-60c1332ab0ee)# FFF Project (Flame-Free Farm)

Link : https://maker.wiznet.io/rifqieeeee/contest/fff%2Dproject%2Dflame%2Dfree%2Dfarm/

## Problem

![Problem](https://hackster.imgix.net/uploads/attachments/1715688/2_rtvJrhr829.jpg?auto=compress%2Cformat&w=1280&h=960&fit=max)
Livestock farming is a crucial sector in the economy, providing food sources such as meat and milk, along with other by-products for human consumption. However, livestock farms also face a high risk of fire, which can be triggered by various factors. Common causes of farm fires include electrical short circuits and discarded cigarette butts by workers. This risk is further exacerbated by the presence of methane gas (CH4). The manure from livestock, especially cattle and goats, contains high levels of organic matter that, when decomposed by microorganisms through anaerobic fermentation, produces methane as a primary by-product. Methane is a highly flammable gas that can form explosive mixtures in the air if it accumulates in sufficient concentrations. This situation makes livestock farms a hazardous environment prone to fires, posing threats not only to the safety of the animals and workers but also potentially causing significant economic losses and environmental damage. Therefore, an effective early fire detection system is necessary to proactively monitor and manage this risk.

## Solutions

![Solutions](https://hackster.imgix.net/uploads/attachments/1723051/flame_free_farm_OAZ3uf1EFT.jpg?auto=compress%2Cformat&w=1280&h=960&fit=max)
We need to implement serious preventive measures to address this issue. To tackle this problem, the FFF Project has been developed. The FFF Project is a system designed to monitor methane gas levels, significant temperature changes above threshold limits, and the presence of smoke that may arise from electrical short circuits or discarded cigarette butts. This comprehensive monitoring system aims to provide early detection of potential fire hazards, ensuring timely interventions and enhancing the safety and security of livestock farms. By continuously tracking these critical parameters, the FFF Project helps prevent fires, thereby protecting livestock, workers, and the farm's infrastructure from potentially devastating incidents.

![Additional](https://hackster.imgix.net/uploads/attachments/1722912/4_RiYLKaRJdX.jpg?auto=compress%2Cformat&w=1280&h=960&fit=max)
Additionally, to enhance the security of the livestock, a goat counter feature has been incorporated. Many farmers, including my father, have expressed concerns about goat theft. Therefore, this feature has been added to the FFF Project as a supplementary tool to count the number of goats in the pen. This helps in ensuring the goats' safety and provides peace of mind to the farmers by enabling them to monitor and verify the presence of all their livestock at all times.

## How it Works

![How it Works](https://hackster.imgix.net/uploads/attachments/1723062/flame_free_farm_(1)_K5LGgeJPq5.jpg?auto=compress%2Cformat&w=740&h=555&fit=max)
This system is divided into two subsystems:

### Monitoring Subsystem
The monitoring subsystem includes environmental monitoring and a goat counter. This subsystem utilizes an Arduino Mega along with several necessary sensors:
- **MQ4 sensor**: for methane gas detection
- **SHT40 sensor**: for temperature and humidity monitoring
- **Smoke sensor**: for smoke detection
- **Grove Vision AI v2**: for goat counting

### Web Server Subsystem
The web server subsystem allows for remote monitoring without the need to be physically present. This subsystem is built using the SURF 5 board for the monitoring process. The SURF 5 board will be connected via WLAN cable and linked to the internet. SURF 5 will then function as a web server where users can monitor the results of the monitoring carried out by the first subsystem. Communication between the two subsystems is conducted using serial communication, ensuring that data from the monitoring process can be transmitted to the web server for remote monitoring.

## Hardware Components

- **WIZnet SURF 5 Board**
- **WIZnet WIZPoE-P1**
- **DFRobot Fermion: MEMS Smoke Gas Detection Sensor**
- **DFRobot Fermion: SHT40 Temperature & Humidity Sensor**
- **MQ4 Sensor**
- **Grove Vision AI Module V2**
- **Arduino Mega 2560**
- **0.96" OLED 64x128 Display Module**
- **Routers**
- **Lan cable**
- **USB type A cable**
- **Buzzer**

## Software Apps and Online Services

- **Microsoft VS Code**
- **WIZnet W7500 ISP Tool**
- **Hercules SETUP utility**
- **Arduino IDE**
- **Autodesk EAGLE**
- **Solidworks**
- **Ultimaker Cura**

## Hand Tools and Fabrication Machines

- **Soldering iron**
- **Solder tin**
- **3D Printer**
- **Cutter**
- **Screws and Hexnut**

## Step by Step

1. Testing OLED 0.96 Inch Screens
2. Sensor Testing and OLED Display Integration
3. Integrating Arduino with SURF 5 Boards for Data Transmission
4. Designing an Accurate Goat Counting System
5. Building Effective Web Monitoring Solutions
6. Designing and Fabricating Custom Printed Circuit Boards
7. Designing Functional 3D-Printed Casings
8. System Testing

---

By implementing the FFF Project, we aim to enhance the safety and security of livestock farms, providing early fire detection and preventing goat theft, ultimately protecting the livestock, workers, and farm infrastructure.
