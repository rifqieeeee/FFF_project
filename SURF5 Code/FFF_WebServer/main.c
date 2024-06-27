/**
 ******************************************************************************
 * @file    WZTOE/WZTOE_WebServer/main.c
 * @author  WIZnet
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2018 WIZnet</center></h2>
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "wizchip_conf.h"
#include "dhcp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @addtogroup W7500x_StdPeriph_Examples
 * @{
 */

/** @addtogroup WZTOE_WebServer
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DATA_BUF_SIZE 2048
#define BUFFER_SIZE 100

/* Public variables ---------------------------------------------------------*/
char data_arduino[BUFFER_SIZE]; // Public string variable to store Arduino data
int CH4, Temperature, Smoke;
char CH4_str[BUFFER_SIZE], Temperature_str[BUFFER_SIZE], Smoke_str[BUFFER_SIZE];

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
uint8_t test_buf[DATA_BUF_SIZE];
wiz_NetInfo gWIZNETINFO;
static char buffer[BUFFER_SIZE];
static int buffer_index = 0;
static int storing = 0;

/* Private function prototypes -----------------------------------------------*/
static void UART_Config(void);
static void GPIO_Config(void);
static void DUALTIMER_Config(void);
static void Network_Config(void);
void dhcp_assign(void);
void dhcp_update(void);
void dhcp_conflict(void);
int32_t WebServer(uint8_t sn, uint8_t* buf, uint16_t port);
void delay(__IO uint32_t milliseconds);
void TimingDelay_Decrement(void);
static void UART_Config(void);
void parse_buffer(char *buffer);


/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
    uint32_t ret;
    uint8_t dhcp_retry = 0;

    SystemInit();

    UART_Config();

    /* SysTick_Config */
    SysTick_Config((GetSystemClock() / 1000));

    /* Set WZ_100US Register */
    setTIC100US((GetSystemClock() / 10000));

    GPIO_Config();
    DUALTIMER_Config();

    printf("W7500x Standard Peripheral Library version : %d.%d.%d\r\n", __W7500X_STDPERIPH_VERSION_MAIN, __W7500X_STDPERIPH_VERSION_SUB1, __W7500X_STDPERIPH_VERSION_SUB2);

    printf("SourceClock : %d\r\n", (int) GetSourceClock());
    printf("SystemClock : %d\r\n", (int) GetSystemClock());

    /* Initialize PHY */
#ifdef W7500
    printf("PHY Init : %s\r\n", PHY_Init(GPIOB, GPIO_Pin_15, GPIO_Pin_14) == SET ? "Success" : "Fail");
#elif defined (W7500P)
    printf("PHY Init : %s\r\n", PHY_Init(GPIOB, GPIO_Pin_14, GPIO_Pin_15) == SET ? "Success" : "Fail");
#endif

    /* Check Link */
    printf("Link : %s\r\n", PHY_GetLinkStatus() == PHY_LINK_ON ? "On" : "Off");

    /* Network information setting before DHCP operation. Set only MAC. */
    Network_Config();

    /* DHCP Process */
    DHCP_init(0, test_buf);
    reg_dhcp_cbfunc(dhcp_assign, dhcp_update, dhcp_conflict);
    if (gWIZNETINFO.dhcp == NETINFO_DHCP) {       // DHCP
        printf("Start DHCP\r\n");
        while (1) {
            ret = DHCP_run();

            if (ret == DHCP_IP_LEASED) {
                printf("DHCP Success\r\n");
                break;
            }
            else if (ret == DHCP_FAILED) {
                dhcp_retry++;
            }

            if (dhcp_retry > 3) {
                printf("DHCP Fail\r\n");
                break;
            }
        }
    }

    /* Network information setting after DHCP operation.
     * Displays the network information allocated by DHCP. */
    Network_Config();

    printf("System Loop Start\r\n");

    while (1) {
        ret = WebServer(1, test_buf, 80);
        if (ret < 0) {
            printf("WebServer encountered an error, restarting...\r\n");
            close(1); // Close the socket in case of error
            delay(2000); // Add a delay before attempting to restart
            // Reinitialize the socket
            if ((ret = socket(1, Sn_MR_TCP, 80, 0x00)) != 1) {
                printf("Socket reinitialization failed\r\n");
                while (1); // If socket reinitialization fails, halt the system
            }
        }
    }
	
	return 0;
}

/**
 * @brief  Configures the UART Peripheral.
 * @note
 * @param  None
 * @retval None
 */

void remove_spaces(char* source) {
    char* i = source;
    char* j = source;
    while(*j != '\0') {
        if(!isspace(*j)) {
            *i = *j;
            i++;
        }
        j++;
    }
    *i = '\0';
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

    // UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RTS_CTS;

    // Disable hardware flow control for UART0
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;

    UART_Init(UART0, &UART_InitStructure);
    UART_Cmd(UART0, ENABLE);
}

/**
 * @brief  Configures the GPIO Peripheral.
 * @note   GPIO pin configures for ADC
 * @param  None
 * @retval None
 */
static void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Direction = GPIO_Direction_IN;
    GPIO_InitStructure.GPIO_Pad = GPIO_Pad_Default;
    GPIO_InitStructure.GPIO_AF = PAD_AF0;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
 * @brief  Configures the DUALTIMER Peripheral.
 * @note
 * @param  None
 * @retval None
 */
static void DUALTIMER_Config(void)
{
    DUALTIMER_InitTypDef DUALTIMER_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    DUALTIMER_InitStructure.Timer_Load = GetSystemClock() / 1; //1s
    DUALTIMER_InitStructure.Timer_Prescaler = DUALTIMER_Prescaler_1;
    DUALTIMER_InitStructure.Timer_Wrapping = DUALTIMER_Periodic;
    DUALTIMER_InitStructure.Timer_Repetition = DUALTIMER_Wrapping;
    DUALTIMER_InitStructure.Timer_Size = DUALTIMER_Size_32;
    DUALTIMER_Init(DUALTIMER0_0, &DUALTIMER_InitStructure);

    DUALTIMER_ITConfig(DUALTIMER0_0, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = DUALTIMER0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DUALTIMER_Cmd(DUALTIMER0_0, ENABLE);
}

/**
 * @brief  Configures the Network Information.
 * @note
 * @param  None
 * @retval None
 */
static void Network_Config(void)
{
    uint8_t mac_addr[6] = { 0x00, 0x08, 0xDC, 0x01, 0x02, 0x03 };

    memcpy(gWIZNETINFO.mac, mac_addr, 6);
    gWIZNETINFO.dhcp = NETINFO_DHCP;

    ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);

    printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", gWIZNETINFO.mac[0], gWIZNETINFO.mac[1], gWIZNETINFO.mac[2], gWIZNETINFO.mac[3], gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
    printf("IP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
    printf("GW: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0], gWIZNETINFO.gw[1], gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
    printf("SN: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1], gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);
    printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0], gWIZNETINFO.dns[1], gWIZNETINFO.dns[2], gWIZNETINFO.dns[3]);
}

/**
 * @brief  The call back function of ip assign.
 * @note
 * @param  None
 * @retval None
 */
void dhcp_assign(void)
{
    getIPfromDHCP(gWIZNETINFO.ip);
    getGWfromDHCP(gWIZNETINFO.gw);
    getSNfromDHCP(gWIZNETINFO.sn);
    getDNSfromDHCP(gWIZNETINFO.dns);

    ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);
}

/**
 * @brief  The call back function of ip update.
 * @note
 * @param  None
 * @retval None
 */
void dhcp_update(void)
{
    ;
}

/**
 * @brief  The call back function of ip conflict.
 * @note
 * @param  None
 * @retval None
 */
void dhcp_conflict(void)
{
    ;
}

/**
 * @brief  WebServer example function.
 * @note
 * @param  sn: Socket number to use.
 * @param  buf: The buffer the socket will use.
 * @param  port: Socket port number to use.
 * @retval Success or Fail of configuration functions
 */
int32_t WebServer(uint8_t sn, uint8_t* buf, uint16_t port)
{
    uint8_t i;
    uint8_t adcChannelOffset = 2;
    int32_t ret;
    uint16_t size = 0;
    uint8_t destip[4];
    uint16_t destport;
    uint8_t data_buf[128] = { '\0', };

    // Wait until data is received
    while (UART_GetFlagStatus(UART0, UART_FLAG_RXFE) == SET);

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
        
        // Later in your code, parse and assign values correctly
        if (received_char == '&') {
            storing = 0;
            buffer[buffer_index] = '\0'; // Null-terminate the buffer
            strcpy(data_arduino, buffer); // Copy buffer content to data_arduino

            // Parse data_arduino based on commas
            char *token;
            int count = 0;
            token = strtok(data_arduino, ",");
            while (token != NULL && count < 3) {
                switch (count) {
                    case 0:
                        printf("CH4: %s\r\n", token);
                        strcpy(CH4_str, token); // Store CH4 data as string
                        break;
                    case 1:
                        printf("Temperature: %s\r\n", token);
                        strcpy(Temperature_str, token); // Store Temperature data as string
                        break;
                    case 2:
                        printf("Smoke: %s\r\n", token);
                        strcpy(Smoke_str, token); // Store Smoke data as string
                        break;
                    default:
                        break;
                }
                token = strtok(NULL, ",");
                count++;
            }

            // Convert string data to integer
            remove_spaces(CH4_str);
            CH4 = atoi(CH4_str);
            remove_spaces(Temperature_str);
            Temperature = atoi(Temperature_str);
            remove_spaces(Smoke_str);
            Smoke = atoi(Smoke_str);
        }
 
    }

    switch (getSn_SR(sn))
    {
        case SOCK_ESTABLISHED:
        if (getSn_IR(sn) & Sn_IR_CON) {
            getSn_DIPR(sn, destip);
            destport = getSn_DPORT(sn);
            // printf("%d:Connected - %d.%d.%d.%d : %d\r\n", sn, destip[0], destip[1], destip[2], destip[3], destport);
            setSn_IR(sn, Sn_IR_CON);
        }

        if ((size = getSn_RX_RSR(sn)) > 0) {
            if (size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;
            ret = recv(sn, buf, size);
            if (ret <= 0) return ret;
            // printf("%s", buf);

            const char *responseHeader = "HTTP/1.1 200 OK\r\n"
                                         "Content-Type: text/html\r\n"
                                         "Connection: close\r\n"
                                         "Refresh: 5\r\n"
                                         "\r\n";
            const char *responseBodyStart = "<!DOCTYPE HTML>\r\n"
                                            "<html>\r\n"
                                            "<head>\r\n"
                                            "<style>\r\n"
                                            "body { background-color: #add8e6; }\r\n"
                                            "table { font-family: Arial, sans-serif; border-collapse: collapse; width: 80%; margin: auto; background-color: white; }\r\n"
                                            "td, th { border: 1px solid #dddddd; text-align: center; padding: 12px; font-size: 20px; }\r\n"
                                            "th { background-color: #4CAF50; color: white; font-size: 22px; }\r\n"
                                            ".center { text-align: center; }\r\n"
                                            ".description { font-size: 18px; color: #555; text-align: center; margin: 20px; }\r\n"
                                            ".title { font-size: 40px; color: black; }\r\n"
                                            ".footer { text-align: center; font-size: 16px; color: #555; margin-top: 20px; }\r\n"
                                            "</style>\r\n"
                                            "</head>\r\n"
                                            "<body>\r\n"
                                            "<div class=\"center\">\r\n"
                                            "<h2 class=\"title\">FFF Project Real-time Monitoring</h2>\r\n"
                                            "<p class=\"description\">The <strong>FFF Project</strong> is an advanced system designed to monitor critical parameters on livestock farms. This includes tracking <strong>methane gas levels</strong>, detecting <strong>significant temperature changes</strong> above threshold limits, and identifying the presence of <strong>smoke</strong> that may result from electrical short circuits or discarded cigarette butts.</p>\r\n"
                                            "<p class=\"description\">By providing <strong>early detection</strong> of potential fire hazards, the FFF Project ensures timely interventions and enhances the safety and security of livestock farms. This continuous monitoring system helps prevent fires, thereby protecting livestock, workers, and the farm's infrastructure from potentially devastating incidents.</p>\r\n"
                                            "</div>\r\n"
                                            "<table>\r\n"
                                            "<tr><th>ADC Channel</th><th>Value</th><th>Status</th></tr>\r\n";

            const char *responseBodyEnd = "</table>\r\n"
                                          "<div class=\"footer\">FFF Project created by: Rifqi Abdillah</div>\r\n"
                                          "</body>\r\n"
                                          "</html>\r\n";

            ret = send(sn, responseHeader, strlen(responseHeader));
            if (ret < 0) {
                close(sn);
                return ret;
            }

            ret = send(sn, responseBodyStart, strlen(responseBodyStart));
            if (ret < 0) {
                close(sn);
                return ret;
            }

            for (i = 0; i < 4; i++) {
                if (i == 0) {
                    sprintf(data_buf, "<tr><td class=\"center\">%s</td><td class=\"center\">%d</td><td class=\"center\">%s</td></tr>\r\n", "CH4 Gas", CH4, (CH4 > 50) ? "<span style=\"color:green;\">Safe</span>" : "<span style=\"color:red;\">Danger</span>");
                }

                if (i == 1) {
                    sprintf(data_buf, "<tr><td class=\"center\">%s</td><td class=\"center\">%d</td><td class=\"center\">%s</td></tr>\r\n", "Temperature", Temperature, (Temperature < 40) ? "<span style=\"color:green;\">Safe</span>" : "<span style=\"color:red;\">Danger</span>");
                }

                if (i == 2) {
                    sprintf(data_buf, "<tr><td class=\"center\">%s</td><td class=\"center\">%d</td><td class=\"center\">%s</td></tr>\r\n", "Smoke Gas", Smoke, (Smoke > 50) ? "<span style=\"color:green;\">Safe</span>" : "<span style=\"color:red;\">Danger</span>");
                }

                if (i == 3) {
                    sprintf(data_buf, "<tr><td class=\"center\">%s</td><td class=\"center\">%d</td><td class=\"center\">%s</td></tr>\r\n", "Goat Counter", 2, (2 == 2) ? "<span style=\"color:green;\">Safe</span>" : "<span style=\"color:red;\">Danger</span>");
                }
                ret = send(sn, data_buf, strlen(data_buf));
                if (ret < 0) {
                    close(sn);
                    return ret;
                }

                memset(data_buf, '\0', sizeof(data_buf));
}


            ret = send(sn, responseBodyEnd, strlen(responseBodyEnd));
            if (ret < 0) {
                close(sn);
                return ret;
            }

            disconnect(sn);
        }
        break;

        case SOCK_CLOSE_WAIT:
        if ((ret = disconnect(sn)) != SOCK_OK) {
            close(sn);
            // printf("%d:Socket Closed\r\n", sn);
        }
        break;

        case SOCK_INIT:
        // printf("%d:Listen, Web server, port [%d]\r\n", sn, port);
        if ((ret = listen(sn)) != SOCK_OK) return ret;
        break;

        case SOCK_CLOSED:
        if ((ret = socket(sn, Sn_MR_TCP, port, 0x00)) != sn) return ret;
        // printf("%d:Socket Opened\r\n", sn);
        break;

        default:
        break;
    }
    return 1;
}


/**
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in milliseconds.
 * @retval None
 */
void delay(__IO uint32_t milliseconds)
{
    TimingDelay = milliseconds;

    while (TimingDelay != 0)
        ;
}

/**
 * @brief  Decrements the TimingDelay variable.
 * @param  None
 * @retval None
 */
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00) {
        TimingDelay--;
    }
}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
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

/******************** (C) COPYRIGHT WIZnet *****END OF FILE********************/
