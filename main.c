#include <stdio.h>
#include "bsp/board.h"
#include "pico/stdlib.h"
#include "tusb.h"
#include "usb_descriptors.h"

void handle_button_press(uint gpio, uint32_t events);
void register_gpio_button(uint gpio);

bool led_state = false;

int main() 
{
  stdio_init_all();

  register_gpio_button(16);
  register_gpio_button(17);

  gpio_init(25);
  gpio_set_dir(25, GPIO_OUT);

  tusb_init();

  while(1) 
  {
    tud_task(); // device task
  }
    
  return 0;
}

void handle_button_press(uint gpio, uint32_t events) {
  led_state = !led_state;

  gpio_put(25, led_state);

  uint8_t key = 0x04;

  tud_hid_keyboard_report(0, 0, &key);

}

void register_gpio_button(uint gpio) {
  gpio_init(gpio);
  gpio_set_dir(gpio, GPIO_IN);
  gpio_pull_up(gpio);
  
  gpio_set_irq_enabled_with_callback(gpio, GPIO_IRQ_EDGE_FALL, true, &handle_button_press);
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT)
  {
    // Set keyboard LED e.g Capslock, Numlock etc...
    if (report_id == REPORT_ID_KEYBOARD)
    {
      // bufsize should be (at least) 1
      if ( bufsize < 1 ) return;

      uint8_t const kbd_leds = buffer[0];

      if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
      {
        // Capslock On: disable blink, turn led on
        blink_interval_ms = 0;
        board_led_write(true);
      }else
      {
        // Caplocks Off: back to normal blink
        board_led_write(false);
        blink_interval_ms = BLINK_MOUNTED;
      }
    }
  }
}