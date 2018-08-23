#ifndef CONST_H
#define CONST_H


#define DATA_PORT 20730   // Audio data transmissin port
#define CTRL_PORT 30730   // Control packets transmission port
#define PSIZE 512         // Packet size
#define FSIZE 131072      // Sender's FIFO size
#define RTIME 250         // Retransmission time
#define BSIZE 65536       // Receiver's buffer size
#define UI_PORT 10730
#define SENDER_NAME "Nienazwany Nadajnik"
#define LOOKUP "ZERO_SEVEN_COME_IN"
#define REPLY "BOREWICZ_HERE"
#define REXMIT "LOUDER_PLEASE"
#define DISCOVER_ADDR "255.255.255.255"
#define MAIN_CONTROLLER_BUF 100
#define STDIN_FD 0
#define MAX_TTL 255

#endif
