/*
 *  HID driver for acer devices
 *
 *  Copyright (c) 2016 Brady McDonough
 *  Modified from Simon Wörner's hid-acer <https://github.com/SWW13/hid-acer>
 */

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */

#include <linux/device.h>
#include <linux/hid.h>
#include <linux/module.h>

#include "acer-hids.h"

/* Some Acer keyboards have an issue where they report an excessive max usages value
 * The kyboard targeted in this fix identifies itself as 06CB:73F4
 *
 * At boot this module inspects the report descriptor for a specific error and changes the values so that they will parse properly
 */

#define ACER_KBD_RDESC_SIZE	188
#define ACER_KBD_RDESC_CHECK_POS	(173 * sizeof(__u8))
#define ACER_KBD_RDESC_CHECK_DATA	0x2AFFFF150026FFFF
#define ACER_KBD_RDESC_FIX_POS1		ACER_KBD_RDESC_CHECK_POS + 2
#define ACER_KBD_RDESC_FIX_POS2		ACER_KBD_RDESC_CHECK_POS + 7

static __u8 *acer_kbd_report_fixup(struct hid_device *hdev, __u8 *rdesc, unsigned int *rsize){

  /* Check that the descriptor size matches what we expect */
  if (*rsize == ACER_KBD_RDESC_SIZE) {
    __u64 check = be64_to_cpu(*(__be64 *)(rdesc + ACER_KBD_RDESC_CHECK_POS));
    
    /* check for invalid max usages */
    if (check == ACER_KBD_RDESC_CHECK_DATA) {
      hid_info(hdev, "fixing up acer keyboard report descriptor\n");
      
      /* change max values to 0xFF00 */
      rdesc[ACER_KBD_RDESC_FIX_POS1] = 0x00;
      rdesc[ACER_KBD_RDESC_FIX_POS2] = 0x00;
    }
  }
  
  return rdesc;
}

static const struct hid_device_id acer_devices[] = {
  { HID_USB_DEVICE(USB_VENDOR_ID_ACER_SYNAPTICS,
		   USB_VENDOR_ID_ACER_SYNAPTICS_TP_73F4) },
  { }
};
MODULE_DEVICE_TABLE(hid, acer_devices);

static struct hid_driver acer_driver = {
  .name = "acer",
  .id_table = acer_devices,
  .report_fixup = acer_kbd_report_fixup,
};
module_hid_driver(acer_driver);

MODULE_AUTHOR("Brady McDonough");
MODULE_LICENSE("GPL");
