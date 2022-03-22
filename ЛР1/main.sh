#!/bin/bash

function cat_research() 
{
	cd  $1 
	echo "Directory: "$(pwd) 
	ls -la | grep -v '^d' | tail -n+2 | sort -k2 -r | awk {'print "    file " $10 " - " $2 " reference"'} 
	for D in $(find -maxdepth 1 -type d | tail -n+2 |sort)
	do 
		cd $D
		echo "-----------------------------------------------"
		echo "Directory: "$(pwd)
		
		ls -la | grep -v '^d' | tail -n+2 | sort -k2 -r | awk {'print "    file " $10 " - " $2 " reference"'} 
		
		for file in $(pwd)/*  
		do
			if [ -d "$file" ] 
			then
			echo "-----------------------------------------------"
	   		cat_research $file 
        		cd ..
			fi
		done
		cd ..

	done
}

IFS=$'\n'
if [ -n "$1" ]
then
	if [ $# -ne 1 ]; then
		"Too many parameters" 2>> errors
		 exit 1
	elif ! [[ -d $1 && -r $1 ]]; then 
		"Not directory or does not exist OR no read permission" 2>> errors
		exit 2
	else
		cat_research $1
	fi
else
	"To run you must specify the directory name" 2>> errors
	exit 3
fi
