// $Id$
/*
 * MIDIbox CV V2 Encoder functions
 *
 * ==========================================================================
 *
 *  Copyright (C) 2013 Thorsten Klose (tk@midibox.org)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 * 
 * ==========================================================================
 */

/////////////////////////////////////////////////////////////////////////////
// Include files
/////////////////////////////////////////////////////////////////////////////

#include <mios32.h>
#include <string.h>
#include <app.h>
#include <scs.h>
#include <MbCvEnvironment.h>

#include "mbcv_enc.h"
#include "tasks.h"
#include "mbcv_hwcfg.h"
#include "scs_config.h"


/////////////////////////////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Local variables
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Local prototypes
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Initialisation
/////////////////////////////////////////////////////////////////////////////
s32 MBCV_ENC_Init(u32 mode)
{
  return 0; // no error
}


/////////////////////////////////////////////////////////////////////////////
// Encoder functions
/////////////////////////////////////////////////////////////////////////////
s32 MBCV_ENC_NotifyChange(u32 enc, s32 incrementer)
{
#if 0
  DEBUG_MSG("[MBCV_ENC] ENC#%d %d\n", enc, incrementer);
#endif

  if( enc >= MBCV_ENC_NUM )
    return -1; // invalid encoder

  switch( enc ) {
  case 0: {
    SCS_ENC_MENU_NotifyChange(incrementer);
  } break;

  case 1: {
    DEBUG_MSG("[MBCV_ENC] ENC_GP1=%d (no function implemented yet...)\n", incrementer);
  } break;

  case 2: {
    DEBUG_MSG("[MBCV_ENC] ENC_GP2=%d (no function implemented yet...)\n", incrementer);
  } break;
  }

  return 0; // no error
}
