// $Id: tasks.h 1869 2013-11-25 20:33:23Z tk $
/*
 * Header file for tasks which have to be serviced
 *
 * ==========================================================================
 *
 *  Copyright (C) 2011 Thorsten Klose (tk@midibox.org)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 *
 * ==========================================================================
 */

#ifndef _TASKS_H
#define _TASKS_H

#ifndef MIOS32_FAMILY_EMULATION
# include <FreeRTOS.h>
# include <portmacro.h>
# include <task.h>
# include <queue.h>
# include <semphr.h>
#endif


/////////////////////////////////////////////////////////////////////////////
// Global definitions
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

// this mutex should be used by all tasks which are accessing the SD Card
#ifdef MIOS32_FAMILY_EMULATION
  extern void TASKS_SDCardSemaphoreTake(void);
  extern void TASKS_SDCardSemaphoreGive(void);
# define MUTEX_SDCARD_TAKE { TASKS_SDCardSemaphoreTake(); }
# define MUTEX_SDCARD_GIVE { TASKS_SDCardSemaphoreGive(); }
#else
  extern xSemaphoreHandle xSDCardSemaphore;
# define MUTEX_SDCARD_TAKE { while( xSemaphoreTakeRecursive(xSDCardSemaphore, (portTickType)1) != pdTRUE ); }
# define MUTEX_SDCARD_GIVE { xSemaphoreGiveRecursive(xSDCardSemaphore); }
#endif

// MIDI IN handler
#ifdef MIOS32_FAMILY_EMULATION
  extern void TASKS_MIDIINSemaphoreTake(void);
  extern void TASKS_MIDIINSemaphoreGive(void);
# define MUTEX_MIDIIN_TAKE { TASKS_MIDIINSemaphoreTake(); }
# define MUTEX_MIDIIN_GIVE { TASKS_MIDIINSemaphoreGive(); }
#else
  extern xSemaphoreHandle xMIDIINSemaphore;
# define MUTEX_MIDIIN_TAKE { while( xSemaphoreTakeRecursive(xMIDIINSemaphore, (portTickType)1) != pdTRUE ); }
# define MUTEX_MIDIIN_GIVE { xSemaphoreGiveRecursive(xMIDIINSemaphore); }
#endif


// MIDI OUT handler
#ifdef MIOS32_FAMILY_EMULATION
  extern void TASKS_MIDIOUTSemaphoreTake(void);
  extern void TASKS_MIDIOUTSemaphoreGive(void);
# define MUTEX_MIDIOUT_TAKE { TASKS_MIDIOUTSemaphoreTake(); }
# define MUTEX_MIDIOUT_GIVE { TASKS_MIDIOUTSemaphoreGive(); }
#else
  extern xSemaphoreHandle xMIDIOUTSemaphore;
# define MUTEX_MIDIOUT_TAKE { if( xMIDIOUTSemaphore ) while( xSemaphoreTakeRecursive(xMIDIOUTSemaphore, (portTickType)1) != pdTRUE ); }
# define MUTEX_MIDIOUT_GIVE { if( xMIDIOUTSemaphore ) xSemaphoreGiveRecursive(xMIDIOUTSemaphore); }
#endif


// Digital Out serialization
#ifdef MIOS32_FAMILY_EMULATION
extern void TASKS_DigitalOutSemaphoreTake(void);
  extern void TASKS_DigitalOutSemaphoreGive(void);
# define MUTEX_DIGITALOUT_TAKE { TASKS_DigitalOutSemaphoreTake(); }
# define MUTEX_DIGITALOUT_GIVE { TASKS_DigitalOutSemaphoreGive(); }
#else
extern xSemaphoreHandle xDigitalOutSemaphore;
# define MUTEX_DIGITALOUT_TAKE { if( xDigitalOutSemaphore ) while( xSemaphoreTakeRecursive(xDigitalOutSemaphore, (portTickType)1) != pdTRUE ); }
# define MUTEX_DIGITALOUT_GIVE { if( xDigitalOutSemaphore ) xSemaphoreGiveRecursive(xDigitalOutSemaphore); }
#endif


extern s32 TASK_MSD_EnableSet(u8 enable);
extern s32 TASK_MSD_EnableGet();
extern s32 TASK_MSD_FlagStrGet(char str[5]);


/////////////////////////////////////////////////////////////////////////////
// Export global variables
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif /* _TASKS_H */