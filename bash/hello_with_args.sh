#!/bin/bash

while [[ $# -gt 0 ]]; do
	name=$1
	if [ "${name}" == "gabi" ];then
		echo "Shalom ${name}"
	elif [ "${name}" != "dani" ];then
		echo "Hola ${name}"
	else
		echo "Hello ${name}"
	fi
	shift
done
