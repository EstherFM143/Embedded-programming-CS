/* C Standard library */
#include <stdio.h>
#include <math.h>

/* XDCtools files */
#include <xdc/std.h>

#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/I2C.h>

/* Board Header files */
#include "Board.h"
#include "string.h"

#define STACKSIZE 2048
Char uartTaskStack[STACKSIZE];
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CCC26XX.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/sysbios/knl/Semaphore.h>
#include "Board.h"
#include "mpu9250.h"
#include <stdbool.h>
#include<stdio.h>
#include "Board.h"
#include "buzzer.h"

#define STACKSIZE 2048
Char taskStack[STACKSIZE];

// Buzzer pin configuration
static PIN_Handle hBuzzer;
static PIN_State sBuzzer;
PIN_Config cBuzzer[] = {
  Board_BUZZER | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
  PIN_TERMINATE
};

// MPU power pin global variables
static PIN_Handle hMpuPin;
static PIN_State  MpuPinState;
// Button and LED pin configuration
static PIN_Handle buttonHandle;
static PIN_State buttonState;
static PIN_Handle ledHandle;
static PIN_State ledState;

// for program states
enum state { WAITING=1, DATA_READY };
enum state programState = WAITING;

// Sleep/reading states for FSM (finite state machine)
int sleeping_state = 1;
int reading_state = 0;

static Semaphore_Handle sensorSem;   // Semaphore for sensor task

// Global variable for ambient light
double ambientLight = -1000.0;

// pins (button and LED) RTOS-variables and configuration
PIN_Config buttonConfig[] = {
   Board_BUTTON0  | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
   PIN_TERMINATE
};

PIN_Config ledConfig[] = {
   Board_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
   PIN_TERMINATE
};

// FSM states
typedef enum {
    STATE_SLEEPING,
    STATE_READING
} ButtonState;

ButtonState currentState = STATE_SLEEPING;

// Button interrupt callback to change state
void buttonFxn(PIN_Handle handle, PIN_Id pinId) {
    uint_t pinValue = PIN_getOutputValue(Board_LED1);

    pinValue = !pinValue;
    PIN_setOutputValue(ledHandle, Board_LED1, pinValue);

    if (currentState == STATE_SLEEPING) {
        currentState = STATE_READING;
        sleeping_state = 0;
        reading_state = 1;
    } else if (currentState == STATE_READING) {
        currentState = STATE_SLEEPING;
        sleeping_state = 1;
        reading_state = 0;
    }

}

// MPU power pin configuration
static PIN_Config MpuPinConfig[] = {
    Board_MPU_POWER  | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

// MPU uses its own I2C interface
static const I2CCC26XX_I2CPinCfg i2cMPUCfg = {
    .pinSDA = Board_I2C0_SDA1,
    .pinSCL = Board_I2C0_SCL1
};

// message buffer and counter
char message[100];  // Array to store symbols and line breaks
int Count = 0;      // Counter for the current position in the array

// Melody sequence
int melody[] = {262, 294, 330, 349, 392, 440, 494, 523}; //freauency in Hz
int noteDurations[] = {4, 4, 4, 4, 4, 4, 4, 4}; // 4 = quarter note

// Function to play music
void playMusic(PIN_Handle hBuzzer) {
    int noteCount = sizeof(melody) / sizeof(melody[0]);

    int i;
    for (i = 0; i < noteCount; i++) {
        int duration = 1000000 / noteDurations[i]; // Duration in microseconds

        // Set the buzzer frequency
        buzzerOpen(hBuzzer);
        buzzerSetFrequency(melody[i]);

        // Play the note for 90% of the duration
        Task_sleep(duration * 0.9 / Clock_tickPeriod);

        // Pause briefly between notes
        buzzerClose();
        Task_sleep(duration * 0.1 / Clock_tickPeriod);
    }
}

// task to read MPU sensor data
void sensorFxn(UArg arg0, UArg arg1) {
    float ax = 0, ay = 0, az = 0;
    float gx = 0, gy = 0, gz = 0;
    uint32_t aika = 0; // Timestamp in milliseconds

    // Error thresholds for motion detection
    float axThreshold = 3.5;
    float ayThreshold = 2.5;
    float azThreshold = 5.0;
    float gxThreshold = 150.0;

    I2C_Handle i2cMPU;
    I2C_Params i2cMPUParams;

    I2C_Params_init(&i2cMPUParams);
    i2cMPUParams.bitRate = I2C_400kHz;
    i2cMPUParams.custom = (uintptr_t)&i2cMPUCfg;

    // MPU power on
    PIN_setOutputValue(hMpuPin, Board_MPU_POWER, Board_MPU_POWER_ON);
    Task_sleep(100000 / Clock_tickPeriod); // Wait 100ms for MPU sensor to power up
    System_printf("MPU9250: Power ON\n");

    // Open I2C
    i2cMPU = I2C_open(Board_I2C, &i2cMPUParams);
    if (i2cMPU == NULL) {
        System_abort("Error Initializing I2CMPU\n");
    }

    // MPU setup and calibration
    System_printf("MPU9250: Setup and calibration...\n");
    mpu9250_setup(&i2cMPU);
    System_printf("MPU9250: Setup and calibration OK\n");

    while (1) {
        if (reading_state) {
            char str[100];
            mpu9250_get_data(&i2cMPU, &ax, &ay, &az, &gx, &gy, &gz);

            // Print sensor data
            sprintf(str, "%u, %6.2f, %6.2f, %6.2f, %2.0f, %2.0f, %2.0f\n", aika, ax, ay, az, gx, gy, gz);
            //System_printf(str);

            // Increment timestamp
            aika += 10;

            // Check signals and append to message
            bool signal_detected = false;

            if (az > 2) {   // Ensure enough space for '.' + '\r\n'
                    message[Count++] = '.';
                    Count %= 10;   // Null-terminate the string
                    signal_detected = true;
            }
            if (ay > 1.2) { // Ensure enough space for '-' + '\r\n'
                    message[Count++] = '-';
                    Count %= 10;  // Null-terminate the string
                    signal_detected = true;
            }
            if (gx > 90) {  // Ensure enough space for ' ' + '\r\n'
                    message[Count++] = ' '; // Null-terminate the string
                    Count %= 10;
                    signal_detected = true;
            }

            // Handle signal detected state
            if (signal_detected) {
                System_printf("Message:%s\n", message);
                System_flush();

                    // Check thresholds for motion detection
                if (fabs(ax) > axThreshold || fabs(ay) > ayThreshold || fabs(az) > azThreshold) {
                    System_printf("Motion Detected! Acceleration exceeds threshold: ax=%.2f, ay=%.2f, az=%.2f\n", ax, ay, az);

                    //Turn on LED or take another action
                    PIN_setOutputValue(ledHandle, Board_LED1, 1); // Turn LED on
                    Task_sleep(500000 / Clock_tickPeriod);       // Wait 500ms
                    PIN_setOutputValue(ledHandle, Board_LED1, 0); // Turn LED off
                    Semaphore_post(sensorSem); // Notify UART task
                }

                 if (fabs(gx) > gxThreshold) {
                    System_printf("Gyroscope Motion exceeded threshold!", gx);

                    // Trigger music as a warning
                    playMusic(hBuzzer); // Call playMusic function to alert user
                }

                    // Debounce delay: Wait for 500ms before reading the next signal
                    Task_sleep(1000000 / Clock_tickPeriod);  // 500ms delay

                    // Explicitly reset the signal_detected flag
                    signal_detected = false;
            } else {
                Task_sleep(100000 / Clock_tickPeriod);  // Normal 100ms delay
            }

            // for SOS signal
            if (150 <= gz) {
                System_printf("SOS\n");
                playMusic(hBuzzer); // Play SOS melody
                Task_sleep(1000000 / Clock_tickPeriod);
            }

            System_flush();
        }
    }
}


// Task to manage UART communication
void uartTaskFxn(UArg arg0, UArg arg1) {
    // Initialize UART with 9600 baud, 8 data bits, no parity, 1 stop bit

    UART_Handle uart;
    UART_Params uartParams;

    // Initialize UART communication
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_TEXT;
    uartParams.readDataMode = UART_DATA_TEXT;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.readMode = UART_MODE_BLOCKING;
    uartParams.baudRate = 9600; // 9600 baud rate
    uartParams.dataLength = UART_LEN_8; // 8 data bits
    uartParams.parityType = UART_PAR_NONE; // No parity
    uartParams.stopBits = UART_STOP_ONE; // 1 stop bit

    uart = UART_open(Board_UART0, &uartParams);
    if (uart == NULL) {
        System_abort("Error opening the UART");
    }
    int i = 0;
    char str[10];
    while(1){
        if(sleeping_state){
            // Trigger music when in sleep state
            if (i == 0) { // Play music once
                playMusic(hBuzzer);
                i = 1; // Prevent repeating
            }
            // Send '0' in sleep state
            sprintf(str, "0\r\n\0");
            UART_write(uart, str, 4);
            Char rxBuffer;
            UART_read(uart, &rxBuffer, 1);
            System_printf("%c", rxBuffer);
            if(rxBuffer == '-'){
                  buzzerOpen(hBuzzer);
                  buzzerSetFrequency(3000);
                  Task_sleep(50000 / Clock_tickPeriod);
                  buzzerClose();

            }
            else if(rxBuffer == '.'){
                  buzzerOpen(hBuzzer);
                  buzzerSetFrequency(2000);
                  Task_sleep(10000 / Clock_tickPeriod);
                  buzzerClose();

            }
            else if(rxBuffer == ' '){
                  Task_sleep(10000 / Clock_tickPeriod);
            }


        }
        else if(message[i]!='\0'){
            // send message in reading state
            sprintf(str, "%c\r\n\0", message[i]);
            UART_write(uart, str, 4);
            message[i] = '\0';
            i++;
            i %= 10;
        }
        Task_sleep(1000000 / Clock_tickPeriod);
    }
}

Int main(void) {
    // Task variables
    Task_Handle uartTaskHandle;
    Task_Params uartTaskParams;
    Task_Handle task;
    Task_Params taskParams;

    // initialize hardware components
    Board_initGeneral();
    Board_initI2C();
    Board_initUART();

    // Initialize Semaphore
    Semaphore_Params semParams;
    Semaphore_Params_init(&semParams);
    sensorSem = Semaphore_create(0, &semParams, NULL); // Start as locked
    if (sensorSem == NULL) {
        System_abort("Semaphore creation failed!");
    }

    //initialize pins
    hMpuPin = PIN_open(&MpuPinState, MpuPinConfig);
    if (hMpuPin == NULL) {
        System_abort("Pin open failed!");
    }
    buttonHandle = PIN_open(&buttonState, buttonConfig);
               if(!buttonHandle) {
                  System_abort("Error initializing button pins\n");
               }
               ledHandle = PIN_open(&ledState, ledConfig);
               if(!ledHandle) {
                  System_abort("Error initializing LED pins\n");
               }

               if (PIN_registerIntCb(buttonHandle, &buttonFxn) != 0) {
                  System_abort("Error registering button callback function");
               }

    // create sensor task
    Task_Params_init(&taskParams);
    taskParams.stackSize = STACKSIZE;
    taskParams.stack = &taskStack;
    task = Task_create((Task_FuncPtr)sensorFxn, &taskParams, NULL);
    if (task == NULL) {
        System_abort("Task create failed!");
    }

    // Create UART task
    Task_Params_init(&uartTaskParams);
    uartTaskParams.stackSize = STACKSIZE;
    uartTaskParams.stack = &uartTaskStack;
    uartTaskParams.priority = 2;
    uartTaskHandle = Task_create(uartTaskFxn, &uartTaskParams, NULL);
    if (uartTaskHandle == NULL) {
        System_abort("Task create failed!");
    }
    hBuzzer = PIN_open(&sBuzzer, cBuzzer);
    if (hBuzzer == NULL) {
    System_abort("Pin open failed!");
    }

    System_printf("Hello World\n");
    System_flush();

    BIOS_start();   // Start the TI-RTOS scheduler

    return (0);

}

