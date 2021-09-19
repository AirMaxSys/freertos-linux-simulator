#!/bin/bash

CURRENT_DIR="$(dirname ${0})"
FREERTOS_REL_DIR="${CURRENT_DIR}/../FreeRTOS"
FREERTOS_DIR="$(realpath ${FREERTOS_REL_DIR})"
FREERTOS_KERNEL_DIR="${FREERTOS_DIR}/FreeRTOS/Source"

echo "export FREERTOS_DIR_ENV=${FREERTOS_DIR}" >> ~/.bashrc
echo "export FREERTOS_KERNEL_DIR_ENV=${FREERTOS_KERNEL_DIR}" >> ~/.bashrc

source ~/.bashrc

