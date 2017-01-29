Name:		Yin-Li (Emily) Chow
UCID:		10103742
Tut: 		T01
Partner:	Yue Chen (10065082, T03)

How to run + connect:
- compile in terminal using: gcc -o server server.c
- in one terminal, run using: ./server <port#>
- in another terminal, run using: nc localhost <port#>
  Please note that this port number must match the other port number
- After being prompted for a password, enter: a
- Use 'help' to get commands


Supported commands:
pwd 		returns current working directory
cd <dir>	changes current working directory to <dir>
ls 		lists the contents of the current working directory
cat <file> 	returns contents of the file
help 		prints a list of commands
encourage me 	provides a motivational sentence to the user
beep 		makes computer beep (supposedly)
who 		returns current user (if possible)
off 		terminates the program
