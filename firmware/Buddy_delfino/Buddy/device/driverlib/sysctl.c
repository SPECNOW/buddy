//###########################################################################
//
// FILE:   sysctl.c
//
// TITLE:  C28x system control driver.
//
//###########################################################################
// $TI Release: F2837xS Support Library v3.01.00.00 $
// $Release Date: Mon May 22 15:44:59 CDT 2017 $
// $Copyright:
// Copyright (C) 2014-2017 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

#include "cputimer.h"
#include "sysctl.h"

//
// Define to isolate inline assembly
//
#define SYSCTL_CLRC_DBGM    __asm(" CLRC DBGM")

//*****************************************************************************
//
// SysCtl_delay()
//
//*****************************************************************************
__asm(" .def _SysCtl_delay\n"
      " .sect \".TI.ramfunc\"\n"
      " .global  _SysCtl_delay\n"
      "_SysCtl_delay:\n"
      " SUB    ACC,#1\n"
      " BF     _SysCtl_delay,GEQ\n"
      " LRETR\n");

//*****************************************************************************
//
// SysCtl_getClock()
//
//*****************************************************************************
uint32_t
SysCtl_getClock(uint32_t clockInHz)
{
    uint32_t temp;
    uint32_t oscSource;
    uint32_t clockOut;

    //
    // Don't proceed if an MCD failure is detected.
    //
    if(SysCtl_isMCDClockFailureDetected())
    {
        //
        // OSCCLKSRC2 failure detected. Returning the INTOSC1 rate. You need
        // to handle the MCD and clear the failure.
        //
        clockOut = SYSCTL_DEFAULT_OSC_FREQ;
    }
    else
    {
        //
        // If one of the internal oscillators is being used, start from the
        // known default frequency.  Otherwise, use clockInHz parameter.
        //
        oscSource = HWREG(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &
                    (uint32_t)SYSCTL_CLKSRCCTL1_OSCCLKSRCSEL_M;

        if((oscSource == ((uint32_t)SYSCTL_OSCSRC_OSC2 >> SYSCTL_OSCSRC_S)) ||
           (oscSource == ((uint32_t)SYSCTL_OSCSRC_OSC1 >> SYSCTL_OSCSRC_S)))
        {
            clockOut = SYSCTL_DEFAULT_OSC_FREQ;
        }
        else
        {
            clockOut = clockInHz;
        }

        //
        // If the PLL is enabled calculate its effect on the clock
        //
        if((HWREG(CLKCFG_BASE + SYSCTL_O_SYSPLLCTL1) &
            (SYSCTL_SYSPLLCTL1_PLLEN | SYSCTL_SYSPLLCTL1_PLLCLKEN)) == 3U)
        {
            //
            // Calculate portion from fractional multiplier
            //
            switch((HWREG(CLKCFG_BASE + SYSCTL_O_SYSPLLMULT) &
                    SYSCTL_SYSPLLMULT_FMULT_M) >> SYSCTL_SYSPLLMULT_FMULT_S)
            {
                case 0U:
                    // No fractional portion.
                    temp = 0U;
                    break;

                case 1U:
                    temp = clockInHz / 4U;
                    break;

                case 2U:
                    temp = clockInHz / 2U;
                    break;

                case 3U:
                    temp = (clockInHz * 3U) / 4U;
                    break;

                default:
                    // Not a valid value for the FMULT register.
                    temp = 0U;
                    break;
            }

            //
            // Calculate integer multiplier and fixed divide by 2
            //
            clockOut = clockOut * ((HWREG(CLKCFG_BASE + SYSCTL_O_SYSPLLMULT) &
                                      SYSCTL_SYSPLLMULT_IMULT_M) >>
                                     SYSCTL_SYSPLLMULT_IMULT_S);

            //
            // Add in fractional portion
            //
            clockOut += temp;
        }

        if((HWREG(CLKCFG_BASE + SYSCTL_O_SYSCLKDIVSEL) &
           SYSCTL_SYSCLKDIVSEL_PLLSYSCLKDIV_M) != 0U)
        {
            clockOut /= (2U * (HWREG(CLKCFG_BASE + SYSCTL_O_SYSCLKDIVSEL) &
                                SYSCTL_SYSCLKDIVSEL_PLLSYSCLKDIV_M));
        }
    }

    return(clockOut);
}

//*****************************************************************************
//
// SysCtl_getAuxClock()
//
//*****************************************************************************
uint32_t SysCtl_getAuxClock(uint32_t clockInHz)
{
    uint32_t temp = 0;

    if(((HWREG(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &
         SYSCTL_CLKSRCCTL1_OSCCLKSRCSEL_M) ==
        ((uint32_t)SYSCTL_OSCSRC_OSC2 >> SYSCTL_OSCSRC_S)) ||
       ((HWREG(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &
         SYSCTL_CLKSRCCTL1_OSCCLKSRCSEL_M) ==
        ((uint32_t)SYSCTL_OSCSRC_OSC1 >> SYSCTL_OSCSRC_S)))
    {
        // 10MHz Internal Clock
        clockInHz = SYSCTL_DEFAULT_OSC_FREQ;
    }

    //
    // If the PLL is enabled calculate its effect on the clock
    //
    if((HWREG(CLKCFG_BASE + SYSCTL_O_AUXPLLCTL1) &
        (SYSCTL_AUXPLLCTL1_PLLEN | SYSCTL_AUXPLLCTL1_PLLCLKEN)) == 3U)
    {
        //
        // Calculate portion from fractional multiplier
        //
        temp = (clockInHz * ((HWREG(CLKCFG_BASE + SYSCTL_O_AUXPLLMULT) &
                SYSCTL_AUXPLLMULT_FMULT_M) >> SYSCTL_AUXPLLMULT_FMULT_S))/4U;

        //
        // Calculate integer multiplier and fixed divide by 2
        //
        clockInHz = clockInHz * ((HWREG(CLKCFG_BASE + SYSCTL_O_AUXPLLMULT) &
                                  SYSCTL_AUXPLLMULT_IMULT_M) >>
                                 SYSCTL_AUXPLLMULT_IMULT_S);

        //
        // Add in fractional portion
        //
        clockInHz += temp;
    }

    clockInHz /= (1U << (HWREG(CLKCFG_BASE + SYSCTL_O_AUXCLKDIVSEL) &
                        SYSCTL_AUXCLKDIVSEL_AUXPLLDIV_M));

    return(clockInHz);
}

//*****************************************************************************
//
// SysCtl_setClock()
//
//*****************************************************************************
bool
SysCtl_setClock(uint32_t config)
{
    uint16_t divSel;
    uint16_t pllMult;
    bool status;
    uint16_t i, tempSCSR, tempWDCR, tempWDWCR, intStatus;

    //
    // Check the arguments.
    //
    ASSERT((config & SYSCTL_OSCSRC_M) != SYSCTL_OSCSRC_M); // 3 is not valid

    //
    // Don't proceed to the PLL initialization if an MCD failure is detected.
    //
    if(SysCtl_isMCDClockFailureDetected())
    {
        //
        // OSCCLKSRC2 failure detected. Returning false. You'll need to clear
        // the MCD error.
        //
        status = false;
    }
    else
    {
        //
        // Configure oscillator source
        //
        SysCtl_selectOscSource(config & SYSCTL_OSCSRC_M);

        //
        // Bypass PLL
        //
        EALLOW;
        HWREGH(CLKCFG_BASE + SYSCTL_O_SYSPLLCTL1) &=
            ~SYSCTL_SYSPLLCTL1_PLLCLKEN;
        EDIS;

        SysCtl_delay(3U);

        //
        // Configure PLL if enabled
        //
        EALLOW;
        if((config & SYSCTL_PLL_ENABLE) == SYSCTL_PLL_ENABLE)
        {
            if((HWREGH(DEVCFG_BASE + SYSCTL_O_SYSDBGCTL) &
                SYSCTL_SYSDBGCTL_BIT_0) != 0U)
            {
                //
                // The user can optionally insert handler code here. This will
                // only be executed if a watchdog reset occurred after a failed
                // system PLL initialization. See your device user's guide for
                // more information.
                //
                // If the application has a watchdog reset handler, this bit
                // should be checked to determine if the watchdog reset
                // occurred because of the PLL.
                //
                // No action here will continue with retrying the PLL as
                // normal.
                //
            }

            //
            // Set dividers to /1
            //
            HWREGH(CLKCFG_BASE + SYSCTL_O_SYSCLKDIVSEL) = 0;

            //
            // Get the PLL multiplier settings from config
            //
            pllMult  = (uint16_t)((config & SYSCTL_IMULT_M) <<
                                  SYSCTL_SYSPLLMULT_IMULT_S);

            pllMult |= (uint16_t)(((config & SYSCTL_FMULT_M) >>
                                   SYSCTL_FMULT_S) <<
                                  SYSCTL_SYSPLLMULT_FMULT_S);

            //
            // Lock the PLL five times. This helps ensure a successful start.
            // Five is the minimum recommended number. The user can increase
            // this number according to allotted system initialization time.
            //
            for(i = 0; i < 5; i++)
            {
                //
                // Turn off PLL
                //
                HWREGH(CLKCFG_BASE + SYSCTL_O_SYSPLLCTL1) &=
                    ~SYSCTL_SYSPLLCTL1_PLLEN;

                SysCtl_delay(3U);

                //
                // Write multiplier, which automatically turns on the PLL
                //
                HWREGH(CLKCFG_BASE + SYSCTL_O_SYSPLLMULT) |= pllMult;

                //
                // Wait for the SYSPLL lock counter
                //
                while((HWREGH(CLKCFG_BASE + SYSCTL_O_SYSPLLSTS) &
                       SYSCTL_SYSPLLSTS_LOCKS) == 0U)
                {
                    //
                    // Consider to servicing the watchdog using
                    // SysCtl_serviceWatchdog()
                    //
                }
            }
        }

        //
        // Configure Dividers. Set divider to produce slower output frequency
        // to limit current increase.
        //
        divSel = (uint16_t)(config & SYSCTL_SYSDIV_M) >> SYSCTL_SYSDIV_S;

        if(divSel != (126U / 2U))
        {
            HWREGH(CLKCFG_BASE + SYSCTL_O_SYSCLKDIVSEL) =
                (HWREGH(CLKCFG_BASE + SYSCTL_O_SYSCLKDIVSEL) &
                 ~(uint16_t)SYSCTL_SYSCLKDIVSEL_PLLSYSCLKDIV_M) | (divSel + 1U);
        }
        else
        {
            HWREGH(CLKCFG_BASE + SYSCTL_O_SYSCLKDIVSEL) =
                (HWREGH(CLKCFG_BASE + SYSCTL_O_SYSCLKDIVSEL) &
                 ~(uint16_t)SYSCTL_SYSCLKDIVSEL_PLLSYSCLKDIV_M) | divSel;
        }

        //
        //      *CAUTION*
        // It is recommended to use the following watchdog code to monitor the
        // PLLstartup sequence. If your application has already cleared the
        // watchdog SCRS[WDOVERRIDE] bit this cannot be done. It is recommended
        // not to clear this bit until after the PLL has been initiated.
        //

        //
        // Backup User Watchdog
        //
        tempSCSR = HWREGH(WD_BASE + SYSCTL_O_SCSR);
        tempWDCR = HWREGH(WD_BASE + SYSCTL_O_WDCR);
        tempWDWCR = HWREGH(WD_BASE + SYSCTL_O_WDWCR);

        //
        // Disable windowed functionality, reset counter
        //
        HWREGH(WD_BASE + SYSCTL_O_WDWCR) = 0x0U;
        SysCtl_serviceWatchdog();

        //
        // Disable global interrupts
        //
        intStatus = __disable_interrupts();

        //
        // Configure for watchdog reset and to run at max frequency
        //
        EALLOW;
        HWREGH(WD_BASE + SYSCTL_O_SCSR) = 0x0U;
        HWREGH(WD_BASE + SYSCTL_O_WDCR) = SYSCTL_WD_CHKBITS;

        //
        // This bit is reset only by power-on-reset (POR) and will not be
        // cleared by a WD reset
        //
        HWREGH(DEVCFG_BASE + SYSCTL_O_SYSDBGCTL) |= SYSCTL_SYSDBGCTL_BIT_0;

        //
        // Enable PLLSYSCLK is fed from system PLL clock
        //
        HWREGH(CLKCFG_BASE + SYSCTL_O_SYSPLLCTL1) |= SYSCTL_SYSPLLCTL1_PLLCLKEN;

        EDIS;

        //
        // Delay to ensure system is clocking from PLL prior to clearing status bit
        //
        SysCtl_delay(3U);

        //
        // Clear bit
        //
        EALLOW;
        HWREGH(DEVCFG_BASE + SYSCTL_O_SYSDBGCTL) &= ~SYSCTL_SYSDBGCTL_BIT_0;
        EDIS;

        //
        // Restore user watchdog, first resetting counter
        //
        SysCtl_serviceWatchdog();

        //
        // Set the KEY bits and make sure not to set the WDOVERRIDE bit
        //
        EALLOW;
        HWREGH(WD_BASE + SYSCTL_O_WDCR) = tempWDCR | SYSCTL_WD_CHKBITS;
        HWREGH(WD_BASE + SYSCTL_O_WDWCR) = tempWDWCR;
        HWREGH(WD_BASE + SYSCTL_O_SCSR) = tempSCSR & ~SYSCTL_SCSR_WDOVERRIDE;
        EDIS;

        //
        // Restore state of ST1[INTM]. This was set by the __disable_interrupts()
        // intrinsic previously.
        //
        if(!(intStatus & 0x1))
        {
            EINT;
        }

        //
        // Restore state of ST1[DBGM]. This was set by the __disable_interrupts()
        // intrinsic previously.
        //
        if(!(intStatus & 0x2))
        {
            SYSCTL_CLRC_DBGM;
        }

        //
        // ~200 PLLSYSCLK delay to allow voltage regulator to stabilize prior
        // to increasing entire system clock frequency.
        //
        SysCtl_delay(40U);

        //
        // Set the divider to user value
        //
        EALLOW;
        HWREGH(CLKCFG_BASE + SYSCTL_O_SYSCLKDIVSEL) =
            (HWREGH(CLKCFG_BASE + SYSCTL_O_SYSCLKDIVSEL) &
             ~SYSCTL_SYSCLKDIVSEL_PLLSYSCLKDIV_M) | divSel;
        EDIS;

        status = true;
    }

    return(status);
}

//*****************************************************************************
//
// SysCtl_setAuxClock()
//
//*****************************************************************************
void SysCtl_setAuxClock(uint32_t config)
{
    uint16_t pllMult;
    uint16_t i, counter = 0U, started = 0U;
    uint16_t t2TCR, t2TPR, t2TPRH, t2CLKCTL;
    uint32_t t2PRD;

    //
    // Check the arguments
    //
    ASSERT((config & SYSCTL_OSCSRC_M) != SYSCTL_OSCSRC_M); // 3 is not valid

    //
    // Bypass PLL
    //
    EALLOW;
    HWREGH(CLKCFG_BASE + SYSCTL_O_AUXPLLCTL1) &= ~SYSCTL_AUXPLLCTL1_PLLCLKEN;
    SysCtl_delay(3U);

    //
    // Configure oscillator source
    //
    switch (config & SYSCTL_OSCSRC_M)
    {
        case SYSCTL_AUXPLL_OSCSRC_OSC2:
            // Turn on INTOSC2OFF
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &=
                    ~(SYSCTL_CLKSRCCTL1_INTOSC2OFF);
            // Clk Src = INTOSC2
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL2) &=
                    ~(SYSCTL_CLKSRCCTL2_AUXOSCCLKSRCSEL_M);
            break;

        case SYSCTL_AUXPLL_OSCSRC_XTAL:
            // Turn on XTALOSC
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &=
                    ~(SYSCTL_CLKSRCCTL1_XTALOFF);
            // Clk Src = XTAL
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL2) &=
                    ~(SYSCTL_CLKSRCCTL2_AUXOSCCLKSRCSEL_M);
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL2) |=
                    1U << SYSCTL_CLKSRCCTL2_AUXOSCCLKSRCSEL_S;
            break;

        case SYSCTL_AUXPLL_OSCSRC_AUXCLKIN:
            // Clk Src = AUXCLKIN
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL2) &=
                    ~(SYSCTL_CLKSRCCTL2_AUXOSCCLKSRCSEL_M);
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL2) |=
                    (2U << SYSCTL_CLKSRCCTL2_AUXOSCCLKSRCSEL_S) &
                    SYSCTL_CLKSRCCTL2_AUXOSCCLKSRCSEL_M;
            break;
        default:
            // Do nothing. Not a valid clock source value.
            break;
    }
    EDIS;

    //
    // Configure PLL if enabled
    //
    EALLOW;
    if((config & SYSCTL_AUXPLL_ENABLE) == SYSCTL_AUXPLL_ENABLE)
    {
        //
        // Backup Timer 2 settings
        //
        t2CLKCTL = HWREGH(CPUSYS_BASE + SYSCTL_O_TMR2CLKCTL);
        t2TCR = HWREGH(CPUTIMER2_BASE + CPUTIMER_O_TCR);
        t2PRD = HWREG(CPUTIMER2_BASE + CPUTIMER_O_PRD);
        t2TPR = HWREGH(CPUTIMER2_BASE + CPUTIMER_O_TPR);
        t2TPRH = HWREGH(CPUTIMER2_BASE + CPUTIMER_O_TPRH);

        //
        // Configure Timer 2 for AUXPLL as source in known configuration
        // - Clock source to AUXPLL
        // - Clock divider to divide by 1
        // - Small period to detect overflow
        // - Interrupt disabled
        //
        EALLOW;
        HWREGH(CPUSYS_BASE + SYSCTL_O_TMR2CLKCTL) = 6U;

        HWREGH(CPUTIMER2_BASE + CPUTIMER_O_TCR) |= CPUTIMER_TCR_TSS;

        HWREG(CPUTIMER2_BASE + CPUTIMER_O_PRD) = 10U;
        HWREGH(CPUTIMER2_BASE + CPUTIMER_O_TPR) = 0U;
        HWREGH(CPUTIMER2_BASE + CPUTIMER_O_TPRH) = 0U;
        HWREGH(CPUTIMER2_BASE + CPUTIMER_O_TCR) &= ~CPUTIMER_TCR_TIE;

        //
        // Set AUX Divide by 8 to ensure that AUXPLLCLK <= SYSCLK / 2 while
        // using Timer 2
        //
        HWREGH(CLKCFG_BASE + SYSCTL_O_AUXCLKDIVSEL) = 0x3U;
        EDIS;

        //
        // Get the PLL multiplier settings from config
        //
        pllMult  = (uint16_t)((config & SYSCTL_IMULT_M) <<
                              SYSCTL_AUXPLLMULT_IMULT_S);
        pllMult |= (uint16_t)(((config & SYSCTL_FMULT_M) >> SYSCTL_FMULT_S) <<
                              SYSCTL_AUXPLLMULT_FMULT_S);

        //
        // Lock the PLL up to five times. CPU Timer 2 will monitor a successful
        // lock and break out of the loop earlier if detected.
        //
        while((counter < 5U) && (started == 0U))
        {
            EALLOW;
            //
            // Turn off AUXPLL and delay for it to power down.
            //
            HWREGH(CLKCFG_BASE + SYSCTL_O_AUXPLLCTL1) &=
                ~SYSCTL_AUXPLLCTL1_PLLEN;
            SysCtl_delay(3U);

            //
            // Set integer and fractional multiplier, which automatically turns
            // on the PLL
            //
            HWREGH(CLKCFG_BASE + SYSCTL_O_AUXPLLMULT) |= pllMult;

            //
            // Enable AUXPLL
            //
            HWREGH(CLKCFG_BASE + SYSCTL_O_AUXPLLCTL1) |=
                SYSCTL_AUXPLLCTL1_PLLEN;
            EDIS;

            //
            // Wait for the AUXPLL lock counter
            //
            while((HWREGH(CLKCFG_BASE + SYSCTL_O_AUXPLLSTS) &
                   SYSCTL_AUXPLLSTS_LOCKS) != 1U)
            {
                //
                // Consider to servicing the watchdog using
                // SysCtl_serviceWatchdog()
                //
            }

            //
            // Enable AUXPLLCLK to be fed from AUXPLL
            //
            EALLOW;
            HWREGH(CLKCFG_BASE + SYSCTL_O_AUXPLLCTL1) |=
                SYSCTL_AUXPLLCTL1_PLLCLKEN;
            SysCtl_delay(3U);

            //
            // CPU Timer 2 will now be setup to be clocked from AUXPLLCLK. This
            // is used to test that the PLL has successfully started.
            //
            // Reload and start the timer
            //
            HWREGH(CPUTIMER2_BASE + CPUTIMER_O_TCR) |= CPUTIMER_TCR_TRB;
            HWREGH(CPUTIMER2_BASE + CPUTIMER_O_TCR) &= ~CPUTIMER_TCR_TSS;

            //
            // Check to see timer is counting properly
            //
            for(i = 0U; i < 1000U; i++)
            {
                //
                // Check overflow flag
                //
                if((HWREGH(CPUTIMER2_BASE + CPUTIMER_O_TCR) &
                    CPUTIMER_TCR_TIF) != 0U)
                {
                    //
                    // Clear overflow flag
                    //
                    HWREGH(CPUTIMER2_BASE + CPUTIMER_O_TCR) |= CPUTIMER_TCR_TIF;

                    //
                    // Set flag to indicate PLL started and break out of
                    // for-loop
                    //
                    started = 1U;
                    break;
                }
            }

            //
            // Stop timer
            //
            HWREGH(CPUTIMER2_BASE + CPUTIMER_O_TCR) |= CPUTIMER_TCR_TSS;
            counter++;
            EDIS;
        }

        if(started == 0U)
        {
            //
            // AUX PLL may not have started. Reset multiplier to 0 (bypass
            // PLL).
            //
            EALLOW;
            HWREGH(CLKCFG_BASE + SYSCTL_O_AUXPLLMULT) = 0U;
            EDIS;

            //
            // The user should put some handler code here based on how this
            // condition should be handled in their application.
            //
            ESTOP0;
        }

        //
        // Restore Timer 2 configuration
        //
        EALLOW;
        HWREGH(CPUSYS_BASE + SYSCTL_O_TMR2CLKCTL) = t2CLKCTL;
        HWREGH(CPUTIMER2_BASE + CPUTIMER_O_TCR) = t2TCR;
        HWREG(CPUTIMER2_BASE + CPUTIMER_O_PRD) = t2PRD;
        HWREGH(CPUTIMER2_BASE + CPUTIMER_O_TPR) = t2TPR;
        HWREGH(CPUTIMER2_BASE + CPUTIMER_O_TPRH) = t2TPRH;

        //
        // Reload period value
        //
        HWREGH(CPUTIMER2_BASE + CPUTIMER_O_TCR) |= CPUTIMER_TCR_TRB;
    }

    //
    // Set divider to desired value
    //
    EALLOW;
    HWREGH(CLKCFG_BASE + SYSCTL_O_AUXCLKDIVSEL) =
        (uint16_t)(config & SYSCTL_SYSDIV_M) >> SYSCTL_SYSDIV_S;
    EDIS;
}

//*****************************************************************************
//
// SysCtl_selectOscSource()
//
//*****************************************************************************
void
SysCtl_selectOscSource(uint32_t oscSource)
{
    ASSERT((oscSource == SYSCTL_OSCSRC_OSC1) |
           (oscSource == SYSCTL_OSCSRC_OSC2) |
           (oscSource == SYSCTL_OSCSRC_XTAL));

    //
    // Select the specified source.
    //
    EALLOW;
    switch(oscSource)
    {
        case SYSCTL_OSCSRC_OSC2:
            // Turn on INTOSC2
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &=
                ~SYSCTL_CLKSRCCTL1_INTOSC2OFF;
            // Clk Src = INTOSC2
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &=
                ~SYSCTL_CLKSRCCTL1_OSCCLKSRCSEL_M;
            //Turn off XTALOSC
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) |=
                SYSCTL_CLKSRCCTL1_XTALOFF;
            break;

        case SYSCTL_OSCSRC_XTAL:
            //Turn on XTALOSC
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &=
                ~SYSCTL_CLKSRCCTL1_XTALOFF;
            // Clk Src = XTALOSC
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &=
                ~SYSCTL_CLKSRCCTL1_OSCCLKSRCSEL_M;
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) |=
                ((uint32_t)SYSCTL_OSCSRC_XTAL >> SYSCTL_OSCSRC_S);
            break;

        case SYSCTL_OSCSRC_OSC1:
            // Clk Src = INTOSC1
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) &=
                ~SYSCTL_CLKSRCCTL1_OSCCLKSRCSEL_M;
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) |=
                ((uint32_t)SYSCTL_OSCSRC_OSC1 >> SYSCTL_OSCSRC_S);
            //Turn off XTALOSC
            HWREGH(CLKCFG_BASE + SYSCTL_O_CLKSRCCTL1) |=
                SYSCTL_CLKSRCCTL1_XTALOFF;
            break;

        default:
            // Do nothing. Not a valid oscSource value.
            break;
    }
    EDIS;
}

//*****************************************************************************
//
// SysCtl_getLowSpeedClock()
//
//*****************************************************************************
uint32_t
SysCtl_getLowSpeedClock(uint32_t clockInHz)
{
    uint32_t clockOut;

    //
    // Get the main system clock
    //
    clockOut = SysCtl_getClock(clockInHz);

    //
    // Apply the divider to the main clock
    //
    if((HWREG(CLKCFG_BASE + SYSCTL_O_LOSPCP) &
        SYSCTL_LOSPCP_LSPCLKDIV_M) != 0U)
    {
        clockOut /= (2U * (HWREG(CLKCFG_BASE + SYSCTL_O_LOSPCP) &
                            SYSCTL_LOSPCP_LSPCLKDIV_M));
    }

    return(clockOut);
}

//*****************************************************************************
//
// SysCtl_getDeviceParametric()
//
//*****************************************************************************
uint16_t
SysCtl_getDeviceParametric(SysCtl_DeviceParametric parametric)
{
    uint16_t value;

    //
    // Get requested parametric value
    //
    switch(parametric)
    {
        case SYSCTL_DEVICE_QUAL:
            //
            // Qualification Status
            //
            value = ((HWREG(DEVCFG_BASE + SYSCTL_O_PARTIDL) &
                      SYSCTL_PARTIDL_QUAL_M) >> SYSCTL_PARTIDL_QUAL_S);
            break;

        case SYSCTL_DEVICE_PINCOUNT:
            //
            // Pin Count
            //
            value = ((HWREG(DEVCFG_BASE + SYSCTL_O_PARTIDL) &
                      SYSCTL_PARTIDL_PIN_COUNT_M) >>
                     SYSCTL_PARTIDL_PIN_COUNT_S);
            break;

        case SYSCTL_DEVICE_INSTASPIN:
            //
            // InstaSPIN Feature Set
            //
            value = ((HWREG(DEVCFG_BASE + SYSCTL_O_PARTIDL) &
                      SYSCTL_PARTIDL_INSTASPIN_M) >>
                     SYSCTL_PARTIDL_INSTASPIN_S);
            break;

        case SYSCTL_DEVICE_FLASH:
            //
            // Flash Size (KB)
            //
            value = ((HWREG(DEVCFG_BASE + SYSCTL_O_PARTIDL) &
                      SYSCTL_PARTIDL_FLASH_SIZE_M) >>
                     SYSCTL_PARTIDL_FLASH_SIZE_S);
            break;
        case SYSCTL_DEVICE_PARTID:
            //
            // PARTID Format Revision
            //
            value = ((HWREG(DEVCFG_BASE + SYSCTL_O_PARTIDL) &
                      SYSCTL_PARTIDL_PARTID_FORMAT_REVISION_M) >>
                     SYSCTL_PARTIDL_PARTID_FORMAT_REVISION_S);
            break;
        case SYSCTL_DEVICE_FAMILY:
            //
            // Device Family
            //
            value = ((HWREG(DEVCFG_BASE + SYSCTL_O_PARTIDH) &
                      SYSCTL_PARTIDH_FAMILY_M) >> SYSCTL_PARTIDH_FAMILY_S);
            break;

        case SYSCTL_DEVICE_PARTNO:
            //
            // Part Number
            //
            value = ((HWREG(DEVCFG_BASE + SYSCTL_O_PARTIDH) &
                      SYSCTL_PARTIDH_PARTNO_M) >> SYSCTL_PARTIDH_PARTNO_S);
            break;

        case SYSCTL_DEVICE_CLASSID:
            //
            // Class ID
            //
            value = ((HWREG(DEVCFG_BASE + SYSCTL_O_PARTIDH) &
                      SYSCTL_PARTIDH_DEVICE_CLASS_ID_M) >>
                     SYSCTL_PARTIDH_DEVICE_CLASS_ID_S);
            break;

        default:
            // Not a valid value for PARTID register
            value = 0U;
            break;
    }

    return(value);
}
