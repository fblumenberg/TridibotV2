#pragma once

#define CMD_FORWARD 'F'
#define CMD_BACKWARD 'B'
#define CMD_LEFT 'L'
#define CMD_RIGHT 'R'
#define CMD_STAND 'S'
#define CMD_CENTER 'C'
#define CMD_SETSPEED 'V'
#define CMD_SETBRIGHTNES 'b'

extern void setupMovement();
extern void handleCommand(char variable, String arg);