#include "system.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_pio_regs.h"
#include "opencores_i2c_regs.h"
#include "opencores_i2c.h"
#include "sys/alt_stdio.h"
#include <sys/alt_irq.h>
#include "alt_types.h"
#include "sys/alt_sys_init.h"
#include "io.h"
#include <unistd.h>
#include<stdio.h>
#include<stdint.h>

#define adx_address 0x1D

#define power_data 0x2c             //configuration of power_mode and transfer
#define data_format 0x31            //configuration of data_format
#define INT_ENABLE 0x2E             //configuration of functions to generate interrupts
#define INT_map 0x2F                // configuration to send interrupt

#define Power_Register 0x2D
#define X_Axis_Register_DATAX0 0x32 // Hexadecimal address for the DATAX0 internal register.
#define X_Axis_Register_DATAX1 0x33 // Hexadecimal address for the DATAX1 internal register.
#define Y_Axis_Register_DATAY0 0x34 // Hexadecimal address for the DATAY0 internal register.
#define Y_Axis_Register_DATAY1 0x35 // Hexadecimal address for the DATAY1 internal register.
#define Z_Axis_Register_DATAZ0 0x36 // Hexadecimal address for the DATAZ0 internal register.
#define Z_Axis_Register_DATAZ1 0x37 // Hexadecimal address for the DATAZ1 internal register.

#define off_x 0x1E                  // Hexadecimal address for the offset_x internal register
#define off_y 0x1F                  // Hexadecimal address for the offset_y internal register
#define off_z 0x20                  // Hexadecimal address for the offset_z internal register


#define Threshold_Act 0x24          // Hexadecimal address for the HIGH-G detection register
#define act_inact_Ctl 0x27          // Hexadecimal address for the choice axes register
#define INT_SOURCE 0x30             // Hexadecimal address for register who ensure that the function has triggered


void send_data(alt_u32 reg, alt_u32 data);
alt_u8 read_data(alt_u32 reg);
void recup_data();
void init();
int print_val(int8_t val0,int8_t val1);
void aff(uint16_t nbr);
static void irqhandler_btn(void *context);
static void irqhandler_switch(void *context);
static void interrup(void *Context);
uint8_t detection(int x,int y,int z);


volatile int x=0.0,y=0.0,z=0.0;
volatile uint8_t detect=0;
volatile uint8_t THRESOLD_VAL=14;
volatile uint32_t time=5000;
volatile uint8_t cpt=0;
volatile uint8_t addr=0x00;
alt_u8 xdata0,xdata1 , ydata0,ydata1, zdata0,zdata1;


static void irqhandler_btn(void *context){
    alt_printf("INTERRUPT push_btn\n");
    cpt++;
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PUSH_BTN_BASE,0x01);
}

static void irqhandler_switch(void *context){

    uint8_t SW = 0;
    alt_printf("INTERRUPT switch\n");

    SW = IORD_ALTERA_AVALON_PIO_DATA(SWITCH_BASE);

    THRESOLD_VAL = SW*1+2;

    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWITCH_BASE,0b00001111);
}


static void interrup(void *Context){

    alt_printf("timer \n\r");

    recup_data();
    x=print_val(xdata0,xdata1);
	y=print_val(ydata0,ydata1);
	z=print_val(zdata0,zdata1);

	alt_printf("X = %x\n",x);
	alt_printf("y = %x\n",y);
	alt_printf("z = %x\n",z);

	if((cpt%3)==0){
        aff(x);
        alt_printf("x est affichee\n\r");
	}
	else if((cpt%3)==1){
        aff(y);
        alt_printf("y est affichee\n\r");
	}
	else{
        aff(z);
        alt_printf("z est affichee\n\r");
	}

    detect=detection(x,y,z);
    if (detect==1){
        addr=0x01;
        IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, addr);
        usleep(time);
    }
    else{
        addr=0x00;
    }
}

void send_data(alt_u32 reg, alt_u32 data){

    I2C_start(OPENCORES_I2C_0_BASE,adx_address , 0);

	I2C_write(OPENCORES_I2C_0_BASE,reg, 0);

	I2C_write(OPENCORES_I2C_0_BASE,data, 1);
}

alt_u8 read_data(alt_u32 reg){

    int8_t data;
    //write message
    I2C_start(OPENCORES_I2C_0_BASE,adx_address , 0);
	I2C_write(OPENCORES_I2C_0_BASE,reg, 0);

	I2C_start(OPENCORES_I2C_0_BASE,adx_address , 1);
	data = I2C_read(OPENCORES_I2C_0_BASE,1);

	return data;
}

void recup_data(){

    xdata0=read_data(X_Axis_Register_DATAX0);
    alt_printf("X0 = %x\n",X_Axis_Register_DATAX0);
    xdata1=read_data(X_Axis_Register_DATAX1);
    alt_printf("X1 = %x\n",X_Axis_Register_DATAX1);
    ydata0=read_data(Y_Axis_Register_DATAY0);
    ydata1=read_data(Y_Axis_Register_DATAY1);
    zdata0=read_data(Z_Axis_Register_DATAZ0);
    zdata1=read_data(Z_Axis_Register_DATAZ1);


}

void init(){
    send_data(power_data,0x0F);
    send_data(data_format,0x0B);
    send_data(Power_Register,0x08);
    send_data(INT_ENABLE,0x10);
    send_data(INT_map,0xEF);
    send_data(off_x,(int8_t)-1);
    send_data(off_y,(int8_t)-1);
    send_data(off_z,(int8_t)-11);
    send_data(Threshold_Act,0xA0);
}

uint8_t detection(int x,int y,int z){
  uint8_t detect=0;
  if (x > THRESOLD_VAL || y >THRESOLD_VAL || z >THRESOLD_VAL){
    detect=1;
  }
  else{
    detect=0;
  }
  return detect;
}

int print_val(int8_t val0,int8_t val1){
    int16_t res=0;
    int16_t val2=0;
    int16_t res_f=0;
    val2=(val1 & 0x1f)<<8;

    res = val2 | val0;

    if(res > 4095)          // to have negative values for 13 bits, 2^13=8096
    {
    res = res - 8096;
    }

    res_f=res*4;       //For a range of +-16g, we need to multiply the raw values by 0.0039, according to the datasheet
    return res_f;
}

void aff(uint16_t nbr){
	uint16_t i =0;
	uint16_t tab[4] = {0,0,0,0};
    uint16_t sign;

    if (nbr<0){
       sign=1;
       nbr=nbr*-1.0000;
    }
    else{
        sign =0;
    }
    IOWR_ALTERA_AVALON_PIO_DATA(SEG5_BASE, (nbr > 9) ? 1 : 0);


	while (nbr >= 10) {
		tab[i] = nbr % 10;
		nbr = nbr / 10;
		i = i+1;
	}
	tab[i] = nbr;

	IOWR_ALTERA_AVALON_PIO_DATA(SEG1_BASE,tab[0]);
	IOWR_ALTERA_AVALON_PIO_DATA(SEG2_BASE,tab[1]);
	IOWR_ALTERA_AVALON_PIO_DATA(SEG3_BASE,tab[2]);
	IOWR_ALTERA_AVALON_PIO_DATA(SEG4_BASE,tab[3]);
	IOWR_ALTERA_AVALON_PIO_DATA(SEG6_BASE,sign);
}
int main(){

    init();

    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PUSH_BTN_BASE, 0x01);
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PUSH_BTN_BASE, 0x01);

    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(SWITCH_BASE, 0b00001111);
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWITCH_BASE, 0b00001111);

/*
    alt_ic_isr_register(SWITCH_IRQ_INTERRUPT_CONTROLLER_ID, SWITCH_IRQ, irqhandler_switch, NULL, NULL);
    alt_ic_isr_register(PUSH_BTN_IRQ_INTERRUPT_CONTROLLER_ID, PUSH_BTN_IRQ, irqhandler_btn, NULL, NULL);
    alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID,TIMER_0_IRQ, interrup,NULL, NULL);
*/

    alt_irq_register(PUSH_BTN_IRQ, NULL, irqhandler_btn);
    alt_irq_register(SWITCH_IRQ, NULL, irqhandler_switch);
    alt_irq_register(TIMER_0_IRQ, NULL, interrup);

    // CHECK LE STATUS
	int status = IORD_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE);
	alt_printf("STATUS = %x\n",status);

	// CHECK LE CONTROL
	int init = IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE);
	alt_printf("init = %x\n",init);
	while(1){

	}
	return 0;
}
