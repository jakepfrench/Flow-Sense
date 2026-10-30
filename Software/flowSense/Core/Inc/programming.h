#ifndef PROGRAMMING_H_
#define PROGRAMMING_H_

#include <stdint.h>


#define PROGRAM_BUFFER_SIZE 256

typedef enum
{
    PROGRAM_START,        // Wait for 'p'
    PROGRAM_SELECT_ICON,  // Wait for icon ID (0-4)
    PROGRAM_RECEIVE,      // Receive 256-byte blocks
	PROGRAM_DEMO,		  // Demo
    PROGRAM_STOP
} ProgrammingState;



typedef enum
{
    ICON_SPLASH_ADD       = 0x000000,  // 128 KiB	0

    ICON_FLOW_ON_ADD      = 0x020000,  // 64 KiB	1
    ICON_FLOW_OFF_ADD     = 0x030000,  // 64 KiB	2
    ICON_TEMP_ADD         = 0x040000,  // 64 KiB	3

    ICON_STATUS_10PC_ADD  = 0x050000,  // 64 KiB	4
    ICON_STATUS_20PC_ADD  = 0x060000,  // 64 KiB	5
    ICON_STATUS_30PC_ADD  = 0x070000,  // 64 KiB	6
    ICON_STATUS_40PC_ADD  = 0x080000,  // 64 KiB	7
    ICON_STATUS_50PC_ADD  = 0x090000,  // 64 KiB	8
    ICON_STATUS_60PC_ADD  = 0x0A0000,  // 64 KiB	9
    ICON_STATUS_70PC_ADD  = 0x0B0000,  // 64 KiB	10
    ICON_STATUS_80PC_ADD  = 0x0C0000,  // 64 KiB	11
    ICON_STATUS_90PC_ADD  = 0x0D0000,  // 64 KiB	12
    ICON_STATUS_100PC_ADD = 0x0E0000,  // 64 KiB	13

    ICON_HELP_ADD         = 0x0F0000,  // 64 KiB	14
	ICON_FLASH_ADD        = 0x100000,  // 64 KiB	15
	ICON_SLEEP_ADD        = 0x110000,  // 64 KiB	16

} IconAdd;




typedef struct
{
    uint8_t id;
    IconAdd iconAddress;
    const char *iconName;
    uint16_t width;
    uint16_t height;
    uint16_t x;
    uint16_t y;
} IconConfig;

extern IconConfig IconConfigscreens[17];
extern IconConfig current_icon;




extern volatile ProgrammingState programming_state;
extern volatile uint8_t demo_mode;
extern volatile uint8_t programming_requested;
extern volatile uint8_t program_buffer_ready;
extern volatile uint16_t program_buffer_count;
extern uint8_t program_buffer[PROGRAM_BUFFER_SIZE];

void Programming_Process(uint8_t* Buf, uint32_t *Len);
void Programming_Task(void);




#endif /* PROGRAMMING_H_ */
