#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  uint8_t st = 0;

  //Gets the configuration from the given timer
  if(timer_get_conf(timer,&st) != 0){
    return EXIT_FAILURE;
  }

  //Prints the configuration
  if(timer_display_conf(timer, st, field) != 0){
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {

  //Changes the frequency of the timer
  if(timer_set_frequency(timer, freq) != 0){
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int(timer_test_int)(uint8_t time) {

  if(time == 0){
    return EXIT_FAILURE;
  }
  
  int ipc_status;
  message msg;
  int r = 0;
  uint8_t bit_no;

  //subscribes the interrupt
  timer_subscribe_int(&bit_no);

  //sets the bit mask
  uint8_t irq_set = BIT(bit_no);

 while(time) { 
    /*Get a request message. */

     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
         continue;
     }
     if (is_ipc_notify(ipc_status)) { /* received notification */
         switch (_ENDPOINT_P(msg.m_source)) {
             case HARDWARE: /* hardware interrupt notification */				
                 if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                       timer_int_handler();
                       if(counter % 60 == 0){
                          timer_print_elapsed_time();
                          time--;
                       }
                 }
                 break;
             default:
                 break; /* no other notifications expected: do nothing */	
         }
     } else { /* received a standard message, not a notification */
         /* no standard messages expected: do nothing */
     }
 }

//unsubscribes the interrupt
 timer_unsubscribe_int();


  return EXIT_SUCCESS;
}
