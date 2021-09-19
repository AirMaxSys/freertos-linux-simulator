###Brief
This repository is a simulator of FreeRTOS in Posix/Linux(Windows WSL).
### Clone repository
```
git clone https://github.com/AirMaxSys/freertos-linux-simulator.git --recurse-submodules
```
If repo had been clone:
```
git clone https://github.com/AirMaxSys/freertos-linux-simulator.git
git submodules update --init --recursive
```
It will take a few minutes cause FreeRTOS-Plus submodules.
###Bug
1 The FreeRTOS CLI funtion is abnormal when I tested in WSL2. The stdin alaways has character input makes fgets()/getchar()/fread()/read() cannot work. 

