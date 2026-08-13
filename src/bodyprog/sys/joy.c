#include "game.h"

#include <psyq/libpad.h>

#include "bodyprog/math/math.h"
#include "bodyprog/screen/screen_data.h"
#include "bodyprog/screen/screen_draw.h"
#include "bodyprog/sys/joy.h"

void Joy_Init(void) // 0x8003441C
{
    PadInitDirect(&g_GameWork.rawController, g_Controller1);
    PadStartCom();
}

void Joy_ReadP1(void) // 0x80034450
{
    s_ControllerData* cont;

    cont = &g_GameWork.controllers[0];

    // NOTE: `memcpy` is close, reads `rawController` as two `s32`s, but doesn't give match.
    // memcpy(&cont->analogController, &g_GameWork.rawController, sizeof(s_AnalogController));

    *(s32*)&cont->analogController        = *(s32*)&g_GameWork.rawController;
    *(s32*)&cont->analogController.rightX = *(s32*)&g_GameWork.rawController.rightX;

    // Alternate
    // ((s32*)&cont->analogController)[0] = ((s32*)&g_GameWork.rawController)[0];
    // ((s32*)&cont->analogController)[1] = ((s32*)&g_GameWork.rawController)[1];
}

void Joy_Update(void) // 0x8003446C
{
    Joy_ReadP1();
    Joy_ControllerDataUpdate();
}

void Joy_ControllerDataUpdate(void) // 0x80034494
{
    #define CONTROLLER_COUNT             2
    #define PULSE_INITIAL_INTERVAL_TICKS (TICKS_PER_SECOND / 2)
    #define PULSE_INTERVAL_TICKS         (PULSE_INITIAL_INTERVAL_TICKS / 10)

    s_ControllerData* cont;
    s32               i;
    s32               prevBtnsHeld;
    s32               pulseTicks;
    s32               btnsPulsed;

    // Update controller button flags.
    for (i = CONTROLLER_COUNT, cont = g_Controller0; i > 0; i--, cont++)
    {
        prevBtnsHeld = cont->heldBtnFlags;

        // Update held button flags.
        if (cont->analogController.status == 0xFF)
        {
            cont->heldBtnFlags = ControllerFlag_None;
        }
        else
        {
            cont->heldBtnFlags = ~cont->analogController.digitalButtons & 0xFFFF;
        }

        // TODO: Demagic hex values.
        ControllerData_AnalogToDigital(cont, (*(u16*)&cont->analogController.status & 0x5300) == 0x5300);

        // Directional held flag sanitation? TODO: Find out what it's doing.
        cont->heldBtnFlags = cont->heldBtnFlags | (((cont->heldBtnFlags << 20) | (cont->heldBtnFlags << 8)) &
                                                (ControllerFlag_LStickUp | ControllerFlag_LStickRight | ControllerFlag_LStickDown | ControllerFlag_LStickLeft));

        // Clear up/down held flags if concurrent.
        if ((cont->heldBtnFlags & (ControllerFlag_LStickUp | ControllerFlag_LStickDown)) == (ControllerFlag_LStickUp | ControllerFlag_LStickDown))
        {
            cont->heldBtnFlags &= ~(ControllerFlag_LStickUp | ControllerFlag_LStickDown);
        }

        // Clear left/right held flags if concurrent.
        if ((cont->heldBtnFlags & (ControllerFlag_LStickRight | ControllerFlag_LStickLeft)) == (ControllerFlag_LStickRight | ControllerFlag_LStickLeft))
        {
            cont->heldBtnFlags = cont->heldBtnFlags & ~(ControllerFlag_LStickRight | ControllerFlag_LStickLeft);
        }

        // Update clicked and released button flags.
        cont->clickedBtnFlags  = ~prevBtnsHeld & cont->heldBtnFlags;
        cont->releasedBtnFlags =  prevBtnsHeld & ~cont->heldBtnFlags;

        // Update pulse ticks.
        pulseTicks = cont->pulseTicks;
        if (cont->heldBtnFlags != prevBtnsHeld)
        {
            pulseTicks = 0;
        }
        else
        {
            pulseTicks += g_VBlanks;
        }

        // Update pulsed button flags.
        if (pulseTicks >= PULSE_INITIAL_INTERVAL_TICKS)
        {
            cont->pulsedBtnFlags = cont->heldBtnFlags;
            pulseTicks          = PULSE_INITIAL_INTERVAL_TICKS - PULSE_INTERVAL_TICKS;
        }
        else
        {
            cont->pulsedBtnFlags = cont->clickedBtnFlags;
        }

        btnsPulsed             = cont->pulsedBtnFlags;
        cont->pulseTicks     = pulseTicks;
        cont->pulsedGuiBtnFlags = btnsPulsed;

        // Clear left/right pulse flags if concurrent.
        if ((btnsPulsed & (ControllerFlag_LStickRight | ControllerFlag_LStickLeft)) == (ControllerFlag_LStickRight | ControllerFlag_LStickLeft))
        {
            cont->pulsedGuiBtnFlags &= ~(ControllerFlag_LStickRight | ControllerFlag_LStickLeft);
        }

        // Clear up/down pulse flags if concurrent.
        if ((cont->pulsedGuiBtnFlags & (ControllerFlag_LStickUp | ControllerFlag_LStickDown)) == (ControllerFlag_LStickUp | ControllerFlag_LStickDown))
        {
            cont->pulsedGuiBtnFlags &= ~(ControllerFlag_LStickUp | ControllerFlag_LStickDown);
        }

        // Clear left/right pulse flags if up/down is concurrent.
        if ((cont->pulsedGuiBtnFlags & (ControllerFlag_LStickUp | ControllerFlag_LStickDown)))
        {
            cont->pulsedGuiBtnFlags &= ~(ControllerFlag_LStickRight | ControllerFlag_LStickLeft);
        }
    }
}

void ControllerData_AnalogToDigital(s_ControllerData* cont, bool arg1) // 0x80034670
{
    s32 dPadButtonFlags; // Used as analog value at first.
    s32 axisIdx;
    s32 processedInputFlags;
    s32 normalizedAnalogData;
    s32 xorShiftedRawAnalog;
    s32 heldButtonFlags;
    s32 signedRawAnalog;
    s32 negDirBitIdx;
    s32 posDirBitIdx;

    heldButtonFlags = cont->heldBtnFlags;

    if (arg1)
    {
        // Convert unsigned range to signed range.
        signedRawAnalog = *(u32*)&cont->analogController.rightX ^ 0x80808080;

        xorShiftedRawAnalog = signedRawAnalog;

        for (normalizedAnalogData = 0, axisIdx = 3;
             axisIdx >= 0;
             axisIdx--)
        {
            normalizedAnalogData <<= 8;
            dPadButtonFlags        = xorShiftedRawAnalog >> 24;
            xorShiftedRawAnalog  <<= 8;

            if (dPadButtonFlags < -STICK_DEADZONE)
            {
                normalizedAnalogData |= (dPadButtonFlags + STICK_DEADZONE) & 0xFF;
                negDirBitIdx         = 23 - (axisIdx & (1 << 0));
                heldButtonFlags      |= 1 << (negDirBitIdx - (axisIdx * 2));
            }
            else if (dPadButtonFlags >= STICK_DEADZONE)
            {
                normalizedAnalogData |= (dPadButtonFlags - (STICK_DEADZONE - 1)) & 0xFF;
                posDirBitIdx          = (axisIdx & 0x1) + 21;
                heldButtonFlags      |= 1 << (posDirBitIdx - ((axisIdx >> 1) * 4));
            }
        }

        cont->heldBtnFlags = heldButtonFlags;
    }
    else
    {
        signedRawAnalog      = 0;
        normalizedAnalogData = 0;
    }

    processedInputFlags       = normalizedAnalogData;
    cont->rawSticks.rawData_0 = signedRawAnalog;

    // TODO: Demagic remaining hex values.
    if (cont == g_Controller0)
    {
        if (!(processedInputFlags & 0xFF000000))
        {
            dPadButtonFlags = heldButtonFlags & (ControllerFlag_DpadUp | ControllerFlag_DpadDown);
            if (dPadButtonFlags == ControllerFlag_DpadDown)
            {
                normalizedAnalogData = processedInputFlags | 0x2D000000;
            }
            else if (dPadButtonFlags == ControllerFlag_DpadUp)
            {
                normalizedAnalogData = processedInputFlags | 0xD3000000;
            }
        }
        if (!(normalizedAnalogData & ControllerFlag_Sticks))
        {
            dPadButtonFlags = heldButtonFlags & (ControllerFlag_DpadRight | ControllerFlag_DpadLeft);
            if (dPadButtonFlags == ControllerFlag_DpadRight)
            {
                normalizedAnalogData |= 0x2D0000;
            }
            else if (dPadButtonFlags == ControllerFlag_DpadLeft)
            {
                normalizedAnalogData |= 0xD30000;
            }
        }
        if (!(processedInputFlags & 0xFF000000))
        {
            dPadButtonFlags = heldButtonFlags & (ControllerFlag_DpadUp | ControllerFlag_DpadDown);
            if (dPadButtonFlags == ControllerFlag_DpadDown)
            {
                processedInputFlags |= 0x20000000;
            }
            else if (dPadButtonFlags == ControllerFlag_DpadUp)
            {
                if (!(heldButtonFlags & g_GameWorkPtr->config.controllerConfig.run))
                {
                    processedInputFlags |= 0xE0000000;
                }
                else
                {
                    processedInputFlags |= 0xC0000000;
                }
            }
        }
        if (!(processedInputFlags & ControllerFlag_Sticks))
        {
            dPadButtonFlags = heldButtonFlags & (ControllerFlag_DpadRight | ControllerFlag_DpadLeft);
            if (dPadButtonFlags == ControllerFlag_DpadRight)
            {
                processedInputFlags |= ControllerFlag_RStickRight;
            }
            else if (dPadButtonFlags == ControllerFlag_DpadLeft)
            {
                processedInputFlags |= 0xE00000; // Masks right, down, left???
            }
        }
    }

    cont->normalizedSticks.rawData_0 = normalizedAnalogData;
    cont->field_28 = processedInputFlags;
}

bool func_8003483C(u16* arg0) // 0x8003483C
{
    if (g_Controller0->clickedBtnFlags & *(*arg0 + arg0))
    {
        *arg0 = *arg0 + 1;
    }
    else if (g_Controller0->clickedBtnFlags & (*(arg0 + 1)))
    {
        *arg0 = 2;
    }
    else if (g_Controller0->clickedBtnFlags & 0xFFFF)
    {
        *arg0 = 1;
    }

    if (*(*arg0 + arg0) == 0xFFFF)
    {
        *arg0 = 1;
        return true;
    }
    else
    {
        return false;
    }
}
