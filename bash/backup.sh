#!/bin/bash

BACKUP_DIR="mybackup"
SCRIPT_NAME=`basename "$0"`

mkdir -p ${BACKUP_DIR}
cp ${SCRIPT_NAME} ${BACKUP_DIR}/${SCRIPT_NAME}.bak
