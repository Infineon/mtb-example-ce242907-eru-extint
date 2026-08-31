/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSOC™ Control C1 MCU: ERU external interrupt example
*              for ModusToolbox.
*              External signal is fed to Event Request Unit (ERU) using one of the GPIO pins.
*              Service request interrupt is generated based on the falling edge of the external signal.
*              LED is toggled at every falling edge of the external signal inside the ERU service request interrupt.
*
* Related Document: See README.md
*
******************************************************************************
* (c) 2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*****************************************************************************/

#include <stdio.h>
#include "cybsp.h"
#include "cy_utils.h"
#include "cy_retarget_io.h"

/*******************************************************************************
* Defines
*******************************************************************************/

/* Define macro to enable/disable printing of debug messages */
#define ENABLE_DEBUG_PRINT              (0)

/* Define macro to set the loop count before printing debug messages */
#if ENABLE_DEBUG_PRINT
#define DEBUG_LOOP_COUNT_MAX                (1U)
#endif

#define INTERRUPT_PRIORITY_NODE_ID          IRQ3_IRQn
#define INTERRUPT_EVENT_PRIORITY            (3U)
#define ERU_EXTERNAL_EVENT_HANDLER          IRQ_Hdlr_3

/* Define and initialize the interrupt event count to zero */
static volatile uint32_t interrupt_event_count = 0;

/*******************************************************************************
* Function Name: ERU_EXTERNAL_EVENT_HANDLER
********************************************************************************
* Summary:
* This is the interrupt handler function for the ERU external interrupt.
*
* Parameters:
*  none
*
* Return:
*  void
*
*******************************************************************************/
void ERU_EXTERNAL_EVENT_HANDLER(void)
{
    /*Toggle the LED*/
    Cy_GPIO_ToggleOutput(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN);
    interrupt_event_count++;
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function.
*  1. It demonstrates the usage of the Event Request Unit(ERU).
*  2. ERU consists of Event Trigger logic (ETL), which is configured to trigger an event during the falling edge of an external signal.
*  3. Trigger output from ETL is directed to one of the Output Gating Unit (OGU), which is configured to generate a service request.
*  4. For every falling edge of the external signal, LED is toggled inside the interrupt service request.
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/

int main(void)
{
    cy_rslt_t result;

    #if ENABLE_DEBUG_PRINT
    /* Assign false to disable printing of debug messages*/
    static volatile bool debug_printf = true;
    #endif

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_HW);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    #if ENABLE_DEBUG_PRINT
    printf("Initialization done\r\n");
    #endif

    /*Set Priority for IRQ*/
    NVIC_SetPriority(INTERRUPT_PRIORITY_NODE_ID,INTERRUPT_EVENT_PRIORITY);

    /*Enable the Interrupt*/
    NVIC_EnableIRQ(INTERRUPT_PRIORITY_NODE_ID);

    /*Infinite loop */
    while(1)
    {
        #if ENABLE_DEBUG_PRINT
        if(debug_printf && (interrupt_event_count == DEBUG_LOOP_COUNT_MAX))
        {
            debug_printf = false;
            /* Print message after the loop has run DEBUG_LOOP_COUNT_MAX times */
            printf("LED is toggled inside the interrupt service request, for every falling edge of the external signal\r\n");
        }
        #endif
    }
}

/* [] END OF FILE */
