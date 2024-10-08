#include <lcom/lcf.h>

#include "i8042.h"
#include "kbd.h"

uint8_t scancode = 0;
int KBD_hook_id = 0;
uint32_t counter_KBD = 0;

void (kbc_ih)() {
    if(kbd_read_scancode() != 0){
        scancode = 0;
        printf("Error in the scancode reading \n");
    }

}

int (kbd_read_status)(uint8_t *status){

    if(util_sys_inb(KBC_ST_REG, status) != 0){
        return EXIT_FAILURE;
    }
    #ifdef LAB3
    counter_KBD++;
    #endif
        
    return EXIT_SUCCESS;
}

int (kbd_read_scancode)(){
    uint8_t value = 0;
    uint8_t status = 0; 
    int tries = 20; //avoids the program to block

    while( tries ) {

        if(kbd_read_status(&status) != 0){
            return EXIT_FAILURE;
        }

        /* loop while 8042 input buffer is not empty */
        if(status & (KBC_PARITY | KBC_TIMEOUT | KBC_AUX)){
            return EXIT_FAILURE;
        }

        if(status & KBC_OBF) {
            if(util_sys_inb(KBC_OUT_BUF, &value) != 0){
                return EXIT_FAILURE;
            }

            #ifdef LAB3
            counter_KBD++;
            #endif

            scancode = value;
            return EXIT_SUCCESS;
        }

        tickdelay(micros_to_ticks(DELAY_US));// e.g. tickdelay()
        tries--; 
    }

    return EXIT_FAILURE;
    
}

int (kbd_subscribe_int)(uint8_t *bit_no){
    *bit_no = IRQ_KEYBOARD; 
    KBD_hook_id = *bit_no;

    if(sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &KBD_hook_id) != 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int (kbd_unsubscribe_int)(){
    if(sys_irqrmpolicy(&KBD_hook_id) != 0){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(kbd_activate)(){
    uint8_t command = KBC_RD_CM_B;

    if(kbd_write_command(&command,KBC_CMD_REG) != 0){
        return EXIT_FAILURE;
    }

    if(util_sys_inb(KBC_OUT_BUF, &command) != 0){
        return EXIT_FAILURE;
    }

    #ifdef LAB3
    counter_KBD++;
    #endif

    uint8_t command2 = KBC_WRT_CM_B;
    if(kbd_write_command(&command2, KBC_ST_REG)!= 0){
        return EXIT_FAILURE;
    }

    command |= KBD_ENABLE;


    if(kbd_write_command(&command, KBC_OUT_BUF) != 0){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}



int(kbd_write_command)(uint8_t *command, int port){
    uint8_t status = 0;
    int tries = 20;

    while( tries ) {
        if(util_sys_inb(KBC_ST_REG, &status) != 0){
            return EXIT_FAILURE;
        }
        #ifdef LAB3
        counter_KBD++;
        #endif
        /* loop while 8042 input buffer is not empty */
        if(status & (KBC_PARITY | KBC_TIMEOUT | KBC_AUX)){
            return EXIT_FAILURE;
        }
        if( (status & KBC_IBF) == 0 ) {
            sys_outb(port, *command);
            return EXIT_SUCCESS;
        }
        tickdelay(micros_to_ticks(DELAY_US)); // e.g. tickdelay()
        tries--;
    }

    return EXIT_FAILURE;
}
