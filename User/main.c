
#include <string.h>
#include "debug.h"
//#include "ina226.h"
#include "uart.h"
#include "KAIGUAN.h"
//#include "TJCScreen.h"
//#include "WS2818.h"
/* EEPROM DATA ADDRESS Length Definition */
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#define Address_8bit  0
#define Address_16bit  1

/* Global Variable */
u16 RxBuffer1[6] = {0};                                             /* USART2 Using */
volatile u8 RxCnt1 = 0;
float Bus_V[8] = {0};
float current[8] = {0};
float power[8] = {0};
int COM_Mode[8] = {0};
int time = 0;
int key = 0;
int old_key = 0;
char isLongTouch = 0;
int alert_flag = 0;

double current_para = 0.00017297;
double Bus_V_para = 0.00120795;

void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));


/* EERPOM DATA ADDRESS Length Selection */
#define Address_Lenth   Address_8bit
//#define Address_Lenth   Address_16bit

/* Global Variable */


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
/*
 * 5A  5V
 *  计算公式：
 *  电流LSB = 预计最大电流 / 2 ^ 15       0.0002

    校准寄存器=0.00512/电流LSB/0.002      12800  3200
    测量电流 = 电流寄存器值*电流LSB = 并联电压寄存器值*校准寄存器值/2048
 *  总线电压 = 总线电压寄存器值*1.25
 *  功率 = 功率寄存器值*25*电流LSB=电流*总线电压
 * */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%lu\r\n", SystemCoreClock);
    printf("ChipID:%08lx\r\n", DBGMCU_GetCHIPID());
    kaiguan_Init();

    USART2_Init();
    int led = 0;

    while (1)
    {
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, led);
        Delay_Ms(100);
        //电压监测
//        led = (led + 1) % 2;

        uint16_t t;
        if (RxBuffer1[0])
        {
            switch (RxBuffer1[0])
            {
                case 1:
                    switch (RxBuffer1[1]) {
                        case 1:
                            led = (led + 1) % 2;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
            memset(RxBuffer1, 0, sizeof(RxBuffer1));
            RxCnt1 = 0;
        }

    }

}

void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        RxBuffer1[RxCnt1++] = USART_ReceiveData(USART2);
        USART_SendData(USART1, RxBuffer1[RxCnt1 - 1]);
//        if (RxCnt1 == 6) {
//            RxCnt1 = 0;
//        }
    }
}



#pragma clang diagnostic pop