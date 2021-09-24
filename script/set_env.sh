#!/usr/bin/env bash

# get FreeRTOS related directories
CURRENT_DIR="$(dirname ${0})"
FREERTOS_REL_DIR="${CURRENT_DIR}/../FreeRTOS"

FREERTOS_DIR="$(realpath ${FREERTOS_REL_DIR})"
FREERTOS_KERNEL_DIR="${FREERTOS_DIR}/FreeRTOS/Source"

# check and set env
if [ -R FREERTOS_DIR_ENV ]
then
    echo "FREERTOS_DIR_ENV has been set!"
else
    $(grep "export FREERTOS_DIR_ENV*" ~/.bashrc)
    if [ $? -eq 0 ]
    then
        $(sed -i 's/export FREERTOS_DIR_ENV=.*/export FREERTOS_DIR_ENV='"$(echo ${FREERTOS_DIR} | sed 's_/_\\/_g')"'/g' ~/.bashrc)
    else
        echo "export FREERTOS_DIR_ENV=${FREERTOS_DIR}" >> ~/.bashrc
    fi
    echo "FREERTOS_DIR_ENV writed into ~/.bashrc"
fi

if [ -R FREERTOS_KERNEL_DIR_ENV ]
then
    echo "FREERTOS_KERNEL_DIR_ENV has been set!"
else
    $(grep "export FREERTOS_KERNEL_DIR_ENV*" ~/.bashrc)
    if [ $? -eq 0 ]
    then
        $(sed -i 's/export FREERTOS_KERNEL_DIR_ENV=.*/export FREERTOS_KERNEL_DIR_ENV='"$(echo ${FREERTOS_KERNEL_DIR} | sed 's_/_\\/_g')"'/g' ~/.bashrc)
    else
        echo "export FREERTOS_KERNEL_DIR_ENV=${FREERTOS_KERNEL_DIR}" >> ~/.bashrc
    fi
    echo "FREERTOS_KERNEL_DIR_ENV writed into ~/.bashrc"
fi

$(source ~/.bashrc)

