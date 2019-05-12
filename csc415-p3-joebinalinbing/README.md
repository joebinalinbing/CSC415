# CSC 415 - Project 3 - My Shell

## Student Name: Joe Binalinbing

## Student ID   : 918501338

## Build Instructions
##### 1. fgets reads users input

##### 2. if new line is detected replace with null.

##### 3.for background I set a flag so when true itll execute. I omit the '&' before executing parsing my tokens

##### 4. i execute parse tokens to store the arguments. For my pipe implementtion, I use arrays to hold the arguments before '|' and a second array to hold after it

##### 5. Process symbols and also commands to determine which function to execute.

##### 6. I globalized my flags so if one the flags are true itll direct to the specific execvp like redirection, pipe or normals commands

## Run Instructions

##### 1. cd to directory

##### 2. make

##### 3. ./myshell


## List Extra Credit comepleted

##### 1. Implement the shell so the current working directory is shown on the prompt

##### 2. Shows a list of previous history of commands by inputting history
