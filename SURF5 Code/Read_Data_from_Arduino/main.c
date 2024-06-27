#include "main.h"
#include <stdio.h>

/** @addtogroup W7500x_StdPeriph_Examples
 * @{
 */

/** @addtogroup UART_HardwareFlowControl
 * @{
 */

#define BUFFER_SIZE 100

char data_arduino[BUFFER_SIZE];
int CH4, Temperature, Smoke;

static char buffer[BUFFER_SIZE];
static int buffer_index = 0;
static int storing = 0;

static void UART_Config(void);

int main(void)
{
    SystemInit();

    UART_Config();

    printf("W7500x Standard Peripheral Library version : %d.%d.%d\r\n", __W7500X_STDPERIPH_VERSION_MAIN, __W7500X_STDPERIPH_VERSION_SUB1, __W7500X_STDPERIPH_VERSION_SUB2);

    printf("SourceClock : %d\r\n", (int) GetSourceClock());
    printf("SystemClock : %d\r\n", (int) GetSystemClock());

    printf("System Loop Start\r\n");

    while (1) {
        if (UART_GetFlagStatus(UART0, UART_FLAG_RXFE) == RESET) {
            char received_char = UART_ReceiveData(UART0);

            if (storing == 1 && received_char != '&' && buffer_index < BUFFER_SIZE - 1) {
                if (received_char != '\n') {
                    buffer[buffer_index++] = received_char;
                }
            }

            if (received_char == '$') {
                storing = 1;
                buffer_index = 0;
            }
            
            if (received_char == '&') {
                storing = 0;
                buffer[buffer_index] = '\0';
                strcpy(data_arduino, buffer);

                char *token;
                int count = 0;
                token = strtok(data_arduino, ",");
                while (token != NULL && count < 3) {
                    switch (count) {
                        case 0:
                            printf("CH4: %s\r\n", token);
                            strcpy(CH4, token);
                            break;
                        case 1:
                            printf("Temperature: %s\r\n", token);
                            strcpy(Temperature, token);
                            break;
                        case 2:
                            printf("Smoke: %s\r\n", token);
                            strcpy(Smoke, token);
                            break;
                        default:
                            break;
                    }
                    token = strtok(NULL, ",");
                    count++;
                }
            } 
        }
    }
	
	return 0;
}

static void UART_Config(void)
{
    UART_InitTypeDef UART_InitStructure;

    UART_StructInit(&UART_InitStructure);

#if defined (USE_WIZWIKI_W7500_EVAL)
    UART_Init(UART1, &UART_InitStructure);
    UART_Cmd(UART1, ENABLE);
#else
    S_UART_Init(115200);
    S_UART_Cmd(ENABLE);
#endif

    UART_StructInit(&UART_InitStructure);

    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;

    UART_Init(UART0, &UART_InitStructure);
    UART_Cmd(UART0, ENABLE);
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
    while (1)
    {
    }
}
#endif

/**
 * @}
 */

/**
 * @}
 */
