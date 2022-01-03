/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif
#include <stdio.h>
#include <inttypes.h>
#include <cstring>
#include "systick.h"
#include "LpcUart.h"
#include "esp8266_socket.h"
#include "bmp2.h"
#include <cr_section_macros.h>
#include "I2C.h"
#include "MQTTClient.h"
#define TICKRATE_HZ1 1000
#include <atomic>
static volatile std::atomic_int counter;
#define SSID	    "Nallukka"
#define PASSWORD    "password123"
#define BROKER_IP   "otso.hopto.org"
#define BROKER_PORT  1883
#define TEST_INPUT_PIN          0	/* GPIO pin number mapped to PININT */
#define TEST_INPUT_PORT         0	/* GPIO port number mapped to PININT */
#define TEST_INPUT_PIN_PORT     0
#define TEST_INPUT_PIN_BIT      0
#define PININT_INDEX   0	/* PININT index used for GPIO mapping */
#define PININT_IRQ_HANDLER  PIN_INT0_IRQHandler	/* GPIO interrupt IRQ function name */
#define PININT_NVIC_NAME    PIN_INT0_IRQn	/* GPIO interrupt NVIC interrupt name */

static volatile uint32_t systicks;
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
	systicks++;
	if(counter > 0) counter--;
}

uint32_t get_ticks(void)
{
	return systicks;
}

#ifdef __cplusplus
}
#endif

void Sleep(int ms)
{
	counter = ms;
	while(counter > 0) {
		__WFI();
	}
}

/* this function is required by the modbus library */
uint32_t millis() {
	return systicks;
}


void messageArrived(MessageData* data)
{
	printf("Message arrived on topic %.*s: %.*s\r\n", data->topicName->lenstring.len, data->topicName->lenstring.data,
		data->message->payloadlen, (char *)data->message->payload);
}

void PININT_IRQ_HANDLER(void)
{
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(PININT_INDEX));
	Board_LED_Toggle(0);
}

int main(void) {
	SystemCoreClockUpdate();
	Board_Init();
	Board_LED_Set(0, false);

	/* Initialize PININT driver */
		Chip_PININT_Init(LPC_GPIO_PIN_INT);

		/* Set pin back to GPIO (on some boards may have been changed to something
		   else by Board_Init()) */
		Chip_IOCON_PinMuxSet(LPC_IOCON, TEST_INPUT_PIN_PORT, TEST_INPUT_PIN_BIT,
							 (IOCON_DIGMODE_EN | IOCON_MODE_INACT) );

		/* Configure GPIO pin as input */
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, TEST_INPUT_PORT, TEST_INPUT_PIN);

		/* Enable PININT clock */
		Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PININT);

		/* Reset the PININT block */
		Chip_SYSCTL_PeriphReset(RESET_PININT);

		/* Configure interrupt channel for the GPIO pin in INMUX block */
		Chip_INMUX_PinIntSel(PININT_INDEX, TEST_INPUT_PORT, TEST_INPUT_PIN);

		/* Configure channel interrupt as edge sensitive and falling edge interrupt */
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(PININT_INDEX));
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(PININT_INDEX));
		Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(PININT_INDEX));

		/* Enable interrupt in the NVIC */
		NVIC_ClearPendingIRQ(PININT_NVIC_NAME);
		NVIC_EnableIRQ(PININT_NVIC_NAME);

		/* Spin in a loop here.  All the work is done in ISR. */
		while (1) {}

#if 0
	LpcPinMap none = {-1, -1}; // unused pin has negative values in it
	LpcPinMap txpin = { 0, 18 }; // transmit pin that goes to debugger's UART->USB converter
	LpcPinMap rxpin = { 0, 13 }; // receive pin that goes to debugger's UART->USB converter
	LpcUartConfig cfg = { LPC_USART0, 115200, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1, false, txpin, rxpin, none, none };
	LpcUart dbgu(cfg);
#endif
	/* Set up SWO to PIO1_2 */
	Chip_SWM_MovablePortPinAssign(SWM_SWO_O, 1, 2); // Needed for SWO printf

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / 1000);

	printf("\r\nBoot\r\n");

    /* The sysTick counter only has 24 bits of precision, so it will
	   overflow quickly with a fast core clock. You can alter the
	   sysTick divider to generate slower sysTick clock rates. */
	Chip_Clock_SetSysTickClockDiv(1);

	/* A SysTick divider is present that scales the sysTick rate down
	   from the core clock. Using the SystemCoreClock variable as a
	   rate reference for the SysTick_Config() function won't work,
	   so get the sysTick rate by calling Chip_Clock_GetSysTickClockRate() */
	uint32_t sysTickRate = Chip_Clock_GetSysTickClockRate();

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(sysTickRate / TICKRATE_HZ1);
	I2C_config cfg;
    I2C i2c(cfg);
    uint8_t tx[1];
    uint8_t rx[1];
    bmp2_config *conf;
    bmp2_dev *dev;
    bmp2_set_config(conf, dev);
    bmp2_init(dev);

    // Force the counter to be placed into memory
/*    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
    	char str[32];
    	tx[0] = 0x01;
    	i2c.transaction(0x48, tx, 1, rx, 1);
    	__WFI();
    	Sleep(1);
    	tx[0] = 0x00;
    	i2c.transaction(0x48, tx, 1, rx, 1);
    	printf(str,"Temp: %d\n", static_cast<signed char>(rx[0]));
    	ITM_write(str);
    	Sleep(100);
        i++ ;
    }
    return 0 ;
*/

	/* connect to mqtt broker, subscribe to a topic, send and receive messages regularly every 1 sec
	MQTTClient client;
	Network network;
	unsigned char sendbuf[80], readbuf[80];
	int rc = 0,
		count = 0;
	MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

	NetworkInit(&network,SSID,PASSWORD);
	MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

	char* address = (char *)BROKER_IP;
	if ((rc = NetworkConnect(&network, address, BROKER_PORT)) != 0)
		printf("Return code from network connect is %d\r\n", rc);


	connectData.MQTTVersion = 3;
	connectData.clientID.cstring = (char *)"esp_test";

	if ((rc = MQTTConnect(&client, &connectData)) != 0)
		printf("Return code from MQTT connect is %d\r\n", rc);
	else
		printf("MQTT Connected\r\n");

	if ((rc = MQTTSubscribe(&client, "heartrate", QOS2, messageArrived)) != 0)
		printf("Return code from MQTT subscribe is %d\r\n", rc);
	volatile static int i = 0;
	uint32_t sec = 0;
	while (true)
	{
		printf("\r\ntesti\r\n");
		// send one message per second
	if(get_ticks() / 1000 != sec) {
			MQTTMessage message;
			char payload[32];
			sec = get_ticks() / 1000;
			++count;
			    	i2c.read(0xAF, rx, 1);
			    	__WFI();
			    	Sleep(1);
			    	printf(payload,"%d\n");
			    	Sleep(100);
			    	i++;
			message.qos = QOS1;
			message.retained = 0;
			message.payload = payload;
			printf(payload, "message number %d", count);
			message.payloadlen = strlen(payload);

			if ((rc = MQTTPublish(&client, "heartrate", &message)) != 0)
				printf("Return code from MQTT publish is %d\r\n", rc);
	}

		// run MQTT for 100 ms
		if ((rc = MQTTYield(&client, 100)) != 0)
			printf("Return code from yield is %d\r\n", rc);
	}

	while(true) {
		// "Dummy" NOP to allow source level single
		// stepping of tight while() loop
		__asm volatile ("nop");
	}
	/* do not return */
	return 0 ;
}
