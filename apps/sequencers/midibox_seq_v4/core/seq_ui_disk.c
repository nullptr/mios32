// $Id$
/*
 * Metronome configuration page
 *
 * ==========================================================================
 *
 *  Copyright (C) 2008 Thorsten Klose (tk@midibox.org)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 * 
 * ==========================================================================
 */

/////////////////////////////////////////////////////////////////////////////
// Include files
/////////////////////////////////////////////////////////////////////////////

#include <mios32.h>

#include "seq_lcd.h"
#include "seq_ui.h"
#include "tasks.h"

#include "seq_file.h"


/////////////////////////////////////////////////////////////////////////////
// Local definitions
/////////////////////////////////////////////////////////////////////////////

#define NUM_OF_ITEMS           2
#define ITEM_BACKUP            0
#define ITEM_ENABLE_MSD        1
#define ITEM_INFO              2


/////////////////////////////////////////////////////////////////////////////
// Local LED handler function
/////////////////////////////////////////////////////////////////////////////
static s32 LED_Handler(u16 *gp_leds)
{
  if( ui_cursor_flash ) // if flashing flag active: no LED flag set
    return 0;

  switch( ui_selected_item ) {
    case ITEM_BACKUP: *gp_leds |= 0x0003; break;
    case ITEM_ENABLE_MSD: *gp_leds |= 0x0300; break;
    case ITEM_INFO: *gp_leds |= 0x3000; break;
  }

  return 0; // no error
}


/////////////////////////////////////////////////////////////////////////////
// Local encoder callback function
// Should return:
//   1 if value has been changed
//   0 if value hasn't been changed
//  -1 if invalid or unsupported encoder
/////////////////////////////////////////////////////////////////////////////
static s32 Encoder_Handler(seq_ui_encoder_t encoder, s32 incrementer)
{
  switch( encoder ) {
    case SEQ_UI_ENCODER_GP1:
    case SEQ_UI_ENCODER_GP2:
      ui_selected_item = ITEM_BACKUP;
      break;

    case SEQ_UI_ENCODER_GP4:
    case SEQ_UI_ENCODER_GP5:
    case SEQ_UI_ENCODER_GP6:
    case SEQ_UI_ENCODER_GP7:
    case SEQ_UI_ENCODER_GP8:
      return -1; // not used (yet)

    case SEQ_UI_ENCODER_GP9:
    case SEQ_UI_ENCODER_GP10:
      ui_selected_item = ITEM_ENABLE_MSD;
      break;

    case SEQ_UI_ENCODER_GP11:
    case SEQ_UI_ENCODER_GP12:
      return -1; // not used (yet)

    case SEQ_UI_ENCODER_GP13:
    case SEQ_UI_ENCODER_GP14:
      ui_selected_item = ITEM_INFO;
      break;

    case SEQ_UI_ENCODER_GP15:
    case SEQ_UI_ENCODER_GP16:
      return -1; // not used (yet)
  }

  // for GP encoders and Datawheel
  switch( ui_selected_item ) {
    case ITEM_BACKUP: {
      seq_ui_backup_req = 1; // backup handled by low-priority task in app.c
      // messages print in seq_ui.c so long request is active
      return 1;
    };

    case ITEM_ENABLE_MSD:
      TASK_MSD_EnableSet((incrementer > 0) ? 1 : 0);
      SEQ_UI_Msg(SEQ_UI_MSG_USER, 1000, "Mass Storage via USB", (incrementer > 0) ? "enabled!" : "disabled!");

      return 1;

    case ITEM_INFO: {
      s32 status;

      MUTEX_SDCARD_TAKE;
      status = SEQ_FILE_PrintSDCardInfos();
      MUTEX_SDCARD_GIVE;

      if( status < 0 )
	SEQ_UI_SDCardErrMsg(2000, status);
      else
	SEQ_UI_Msg(SEQ_UI_MSG_USER, 1000, "Open MIOS Terminal", "to display Info!");
      return 1;
    }
  }

  return -1; // invalid or unsupported encoder
}


/////////////////////////////////////////////////////////////////////////////
// Local button callback function
// Should return:
//   1 if value has been changed
//   0 if value hasn't been changed
//  -1 if invalid or unsupported button
/////////////////////////////////////////////////////////////////////////////
static s32 Button_Handler(seq_ui_button_t button, s32 depressed)
{
  if( depressed ) return 0; // ignore when button depressed

  // GP button selects preset
  if( button <= SEQ_UI_BUTTON_GP16 ) {
    s32 incrementer = 0;

    // GP9/10 toggles MSD enable/disable
    if( button == SEQ_UI_BUTTON_GP9 || button == SEQ_UI_BUTTON_GP10 )
      incrementer = (TASK_MSD_EnableGet() > 0) ? -1 : 1;

    // re-use encoder function
    return Encoder_Handler(button, incrementer);
  }

  switch( button ) {
    case SEQ_UI_BUTTON_Select:
    case SEQ_UI_BUTTON_Right:
      if( ++ui_selected_item >= NUM_OF_ITEMS )
	ui_selected_item = 0;

      return 1; // value always changed

    case SEQ_UI_BUTTON_Left:
      if( ui_selected_item == 0 )
	ui_selected_item = NUM_OF_ITEMS-1;

      return 1; // value always changed

    case SEQ_UI_BUTTON_Up:
      return Encoder_Handler(SEQ_UI_ENCODER_Datawheel, 1);

    case SEQ_UI_BUTTON_Down:
      return Encoder_Handler(SEQ_UI_ENCODER_Datawheel, -1);
  }

  return -1; // invalid or unsupported button
}


/////////////////////////////////////////////////////////////////////////////
// Local Display Handler function
// IN: <high_prio>: if set, a high-priority LCD update is requested
/////////////////////////////////////////////////////////////////////////////
static s32 LCD_Handler(u8 high_prio)
{
  if( high_prio )
    return 0; // there are no high-priority updates

  // layout:
  // 00000000001111111111222222222233333333330000000000111111111122222222223333333333
  // 01234567890123456789012345678901234567890123456789012345678901234567890123456789
  // <--------------------------------------><-------------------------------------->
  //   Create                                  MSD USB           Send SD Info        
  //   Backup                                 disabled           to Terminal         

  //                                           MSD USB (UMRW)
  //                                           enabled


  ///////////////////////////////////////////////////////////////////////////
  SEQ_LCD_CursorSet(0, 0);
  SEQ_LCD_PrintString("  Create");
  SEQ_LCD_PrintSpaces(32);

  SEQ_LCD_PrintString("  MSD USB ");
  if( TASK_MSD_EnableGet() == 1 ) {
    char str[5];
    TASK_MSD_FlagStrGet(str);
    SEQ_LCD_PrintFormattedString("(%s)", str);
  } else {
    SEQ_LCD_PrintSpaces(6);
  }
  SEQ_LCD_PrintString("    Send SD Info        ");

  ///////////////////////////////////////////////////////////////////////////
  SEQ_LCD_CursorSet(0, 1);
  if( ui_selected_item == ITEM_BACKUP && ui_cursor_flash ) {
    SEQ_LCD_PrintSpaces(8);
  } else {
    SEQ_LCD_PrintString("  Backup");
  }

  SEQ_LCD_PrintSpaces(32);

  ///////////////////////////////////////////////////////////////////////////

  if( ui_selected_item == ITEM_ENABLE_MSD && ui_cursor_flash ) {
    SEQ_LCD_PrintSpaces(14);
  } else {
    s32 status = TASK_MSD_EnableGet();
    switch( status ) {
      case 0:  SEQ_LCD_PrintString(" disabled     "); break;
      case 1:  SEQ_LCD_PrintString("  enabled     "); break;
      default: SEQ_LCD_PrintString(" Not Emulated!"); break;
    }
  }
  SEQ_LCD_PrintSpaces(6);


  ///////////////////////////////////////////////////////////////////////////
  if( ui_selected_item == ITEM_INFO && ui_cursor_flash ) {
    SEQ_LCD_PrintSpaces(11);
  } else {
    SEQ_LCD_PrintString("to Terminal");
  }
  SEQ_LCD_PrintSpaces(9);

  return 0; // no error
}


/////////////////////////////////////////////////////////////////////////////
// Initialisation
/////////////////////////////////////////////////////////////////////////////
s32 SEQ_UI_DISK_Init(u32 mode)
{
  // install callback routines
  SEQ_UI_InstallButtonCallback(Button_Handler);
  SEQ_UI_InstallEncoderCallback(Encoder_Handler);
  SEQ_UI_InstallLEDCallback(LED_Handler);
  SEQ_UI_InstallLCDCallback(LCD_Handler);

  return 0; // no error
}