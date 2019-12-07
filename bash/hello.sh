#!/bin/bash

#nameArr=("gabi" "dani" "yossi")
read -p "Plese type names:" nameArr

for name in ${nameArr[@]};do
#for name in "$@";do
	if [ "${name}" == "gabi" ];then
		echo "Shalom ${name}"
	elif [ "${name}" != "dani" ];then
		echo "Hola ${name}"
	else
		echo "Hello ${name}"
	fi
done
