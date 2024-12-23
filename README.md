# interfaces
A small console app to print IP addresses for all network interfaces.
This app is for Linux, this shows all interface information in a nice
table.

Example:

(jcartwright@2403-4800-25af-b00--3) 192.168.1.2 Documents  $ ./netinfo 
+-----------------+----------------------+---------------------------+------------------+
| Interface Name  | IP Address           | MAC Address               | Netmask          |
+-----------------+----------------------+---------------------------+------------------+
| lo              | 127.0.0.1            | 00:00:00:00:00:00        | 255.0.0.0        |
| eno1            | 192.168.1.2          | FC:34:97:A5:BC:7E        | 255.255.255.0    |
+-----------------+----------------------+---------------------------+------------------+
#END EXAMPLE.

Compile: gcc interf.c -o netinfo
