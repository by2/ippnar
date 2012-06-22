/* Name: main.c
 * Project: MOTOR DRIVER
 * Author: Abhishek Kumar
 * Creation Date: 2010-03-31
 */

#define F_OSC 12000000		           /* oscillator-frequency in Hz */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"        /* This is also an example for using debug macros */

uchar speedFlag = 0;        /* If 1 go Normal; If 0 go Slow */
uchar outMotors = 0x00;     /* Output Directly through PORTC to motors */
uchar ledStatus = 0x00;     /* Start with both OFF */
uchar arb = 0;

static uchar bytesRemaining;
static uchar temp[12] = "";
/* ------------------------------------------------------------------------- */
/* ----------------------------- Motor  Driver ----------------------------- */
/* ------------------------------------------------------------------------- */

void driveRover(){

  uchar outData = ledStatus | outMotors;

  if(speedFlag == 0){
    PORTC = outData;
  }else{
    if(arb%2 == 0)
      PORTC = outData;
    else
      PORTC = ledStatus;
    arb--;
  }

}

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

/* usbFunctionRead is not used! */
uchar usbFunctionRead(uchar *data, uchar len)
{
    uchar i;
    if(len > bytesRemaining)                // len is max chunk size
        len = bytesRemaining;               // send an incomplete chunk
    bytesRemaining -= len;
	

    for(i = 0; i < len; i++)
        data[i] = temp[i]; // copy the data to the buffer
	
    return len;                             // return real chunk size
}
/* usbFunctionRead is not used! */


usbMsgLen_t usbFunctionSetup(uchar setupData[8])
{
  usbRequest_t    *rq = (void *)setupData;
  uchar control,speed,lMotor,rMotor;
	
  control = rq->bRequest & 0x1f;
  ledStatus = rq->bRequest & 0x60;
  ledStatus = ledStatus >> 1;

  speed = control & 0x10;
  lMotor = control & 0x0c;
  rMotor = control & 0x03;

  if(lMotor != 0x0c || rMotor != 0x03 ) { 
    outMotors = lMotor | rMotor;
  }

  if(speed == 0x10){
    speedFlag = 1;        /* Half Speed */
  }else{
    speedFlag = 0;        /* Full Speed */
  }

  return 0;               /* default for not implemented requests: return no data back to host */
}

/* ------------------------------------------------------------------------- */

int main(void)
{
  uchar   i;

  DDRC = 0xff;

  usbInit();
  usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
  i = 0;
  while(--i){             /* fake USB disconnect for > 250 ms */
    _delay_ms(1);
  }
  usbDeviceConnect();
	
  sei();
  for(;;){                /* main event loop */
    usbPoll();
    driveRover();
  }
  return 0;
}

/* ------------------------------------------------------------------------- */
