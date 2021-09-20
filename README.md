### Brief
This repository is a simulator of FreeRTOS in Posix/Linux. It can run in WSL.
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
### Usage
- Set FreeRTOS and FreeRTOS-Kernel PATH. You can use `bash ./script/set_env.sh` to set `FREERTOS_DIR_ENV and FREERTOS_KERNEL_DIR_ENV` to enverment variable, or pass those path value to Makefile.
- Compile source code using `make` if you don't want FreeRTOS trace on you can use `make TRACE_ON_ENTER=0 COVERAGE_TEST=1`
- Run `./build/posix_demo`
### Bug
- [Fixed]The FreeRTOS CLI funtion is abnormal when I tested in WSL2. The stdin alaways has character input makes fgets()/getchar()/fread()/read() cannot work.     
See FreeRTOS Linux simulator document of [known issues](https://www.freertos.org/FreeRTOS-simulator-for-Linux.html#known_issues)

