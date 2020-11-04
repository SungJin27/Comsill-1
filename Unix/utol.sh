#!/bin/bash

if [ $# -eq 0 ]
then
        for file in * 
        do
        
                for (( i=1; i<=${#file}; i++ ))
                do
		
		char=$(echo $file | cut -c "$i")
					
		if [[ "$char" =~ [a-z] ]]
		then
		chan=$(echo $char | tr 'a-z' 'A-Z')
		name="${name} ${chan}"		
		
		elif [[ "$char" =~ [A-Z] ]]
		then
		chan=$(echo $char | tr 'A-Z' 'a-z')
		name="${name}${chan}"
		
		else
		name="${name} ${char}"
		fi	
                done
		
		name=$(echo $name | tr -d '[:space:]' )
		mv "$file" "$name"
       		name=""
	done

	echo "working directory:"
	echo | pwd

else
	cd $1
        if [ $? == 0 ]
        then
	for file in *
	do
		for (( i=1; i<=${#file}; i++ ))
		do
 
                char=$(echo $file | cut -c "$i")
 
                if [[ "$char" =~ [a-z] ]]
                then
                chan=$(echo $char | tr 'a-z' 'A-Z')
                name="${name} ${chan}"
 
                elif [[ "$char" =~ [A-Z] ]]
                then
                chan=$(echo $char | tr 'A-Z' 'a-z')
                name="${name}${chan}"
  
	        else
                name="${name} ${char}"
                fi
                done
 
                name=$(echo $name | tr -d '[:space:]' )
		mv "$file" "$name"
                name=""
	done
	
	echo "working directory:"
	echo | pwd
	
	else
	echo "there is no such directory."
	fi

fi
