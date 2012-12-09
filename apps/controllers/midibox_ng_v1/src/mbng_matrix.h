// $Id$
/*
 * Scan Matrix access functions for MIDIbox NG
 *
 * ==========================================================================
 *
 *  Copyright (C) 2012 Thorsten Klose (tk@midibox.org)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 * 
 * ==========================================================================
 */

#ifndef _MBNG_MATRIX_H
#define _MBNG_MATRIX_H

#include "mbng_event.h"

/////////////////////////////////////////////////////////////////////////////
// global definitions
/////////////////////////////////////////////////////////////////////////////

#define MBNG_MATRIX_DOUT_LEDRING_PATTERNS 4

/////////////////////////////////////////////////////////////////////////////
// Type definitions
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

extern s32 MBNG_MATRIX_Init(u32 mode);
extern s32 MBNG_MATRIX_PrepareCol(void);
extern s32 MBNG_MATRIX_GetRow(void);
extern s32 MBNG_MATRIX_ButtonHandler(void);

extern s32 MBNG_MATRIX_DIN_NotifyReceivedValue(mbng_event_item_t *item, u16 value);
extern s32 MBNG_MATRIX_DOUT_NotifyReceivedValue(mbng_event_item_t *item, u16 value);

/////////////////////////////////////////////////////////////////////////////
// Exported variables
/////////////////////////////////////////////////////////////////////////////


#endif /* _MBNG_MATRIX_H */