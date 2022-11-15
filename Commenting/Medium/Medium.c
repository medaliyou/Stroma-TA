#include "main.h"                  

#define BOOT_FLAG_ADDRESS           0x08004000U // Internal Flash Address of BOOT_FLAG
#define APP_ADDRESS                 0x08008000U // Internal Flash Application Start Address
#define TIMEOUT_VALUE               SystemCoreClock/4 // Timeout value = SystemCoreClock/4

#define ACK     0x06U  // ACK Sequence
#define NACK    0x16U  // NACK Sequence

static UART_HandleTypeDef huart;  // UART HAL Handle
static uint8_t RX_Buffer[32]; // UART RX buffer
typedef enum
{
    ERASE = 0x43,
    WRITE = 0x31,
    CHECK = 0x51,
    JUMP  = 0xA1,
} COMMANDS; // Definito of type COMMANDS (enumeration)

/* Private Functions Prototypes */
static void Jump2App(void);
static void Boot_Init(void);
static void Transmit_ACK(UART_HandleTypeDef *huart);
static void Transmit_NACK(UART_HandleTypeDef *huart);
static uint8_t Check_Checksum(uint8_t *pBuffer, uint32_t len);
static void Erase(void);
static void Write(void);
static void Check(void);

/* main function (entrypoint) */
int main(void)
{
    /* Setup System Clocks */
    Clk_Update();
    /* Init GIOPs and UART */
    Boot_Init();
    /* Send ACK via UART */
    Transmit_ACK(&huart);
    /* Try to Receive two bytes visa UART to intercept regular application BOOT */
    if(HAL_UART_Rx(&huart, RX_Buffer, 2, TIMEOUT_VALUE) == HAL_UART_TIMEOUT)
    {
      /* Receiving was failed */
      /* Indicate NACK */
        Transmit_NACK(&huart);
        /* boot to application */
        Jump2App();
    }
    /* Received successfully, make sure we have received only one ACK */

    if(Check_Checksum(RX_Buffer, 2) != 1 || RX_Buffer[0] != ACK)
    {
        /* Indicate NACK */

        Transmit_NACK(&huart);
        /* boot to application */
        Jump2App();
    }
    
	for(;;)
	{
        /* Wait for reception */
        while(HAL_UART_Rx(&huart, RX_Buffer, 2, TIMEOUT_VALUE) == HAL_UART_TIMEOUT);
        /* if we didn't receive valid 2 bytes */
        if(Check_Checksum(RX_Buffer, 2) != 1)
        {
          /* send NACK and wait again */
            Transmit_NACK(&huart);
        }
        else
        {
            /* if have received valid 2 bytes */
            /* switch over the 1 received byte */
            switch(RX_Buffer[0])
            {
                case ERASE:
                    Transmit_ACK(&huart); // ACK ERASE command 
                    Erase();  // process ERASE command 
                    break;
                case WRITE:
                    Transmit_ACK(&huart);  // ACK WRITE command 
                    Write();// process WRITE command 
                    break;
                case CHECK:
                    Transmit_ACK(&huart);// ACK CHECK command 
                    Check();// process CHECK command 
                    break;
                case JUMP:
                    Transmit_ACK(&huart);// ACK JUMP command 
                    Jump2App();// process JUMP command 
                    break;
                default: 
                    Transmit_NACK(&huart); // NACK wrong command 
                    break;
            }
        }
	}
    
  for(;;); // Error Handler
	return 0;
}

/* Jump from bootloader (or primary application ) to main application */
static void Jump2App(void)
{
    if (((*(__IO uint32_t*)APP_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
    {
        /* disable all Interrupts */
        __disable_irq();
        /* Get the address of the reset-handler of main application */
        uint32_t jump_address = *(__IO uint32_t *)(APP_ADDRESS + 4);
        /* Set Stack Pointer */
        __set_MSP(*(__IO uint32_t *)APP_ADDRESS);
        /* Create function pointer for application's firmware reset-handler */
        void (*pmain_app)(void) = (void (*)(void))(jump_address);
        /* call reset-handler of main application or "JUMP" */
        pmain_app();
    }
    
}

/**
* Setup GPIOs A2 and A3 for UART RX/TX 
* Setup USART2 IP
* Enable USART2 
**/
static void Boot_Init(void)
{
    GPIO_InitTypeDef gpio_uart; // define gpio_uart as GPIO Init struct
    /* configure pins */
    gpio_uart.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    gpio_uart.Mode = GPIO_MODE_AF_PP;
    gpio_uart.Pull = GPIO_PULL_NONE;
    gpio_uart.Speed = GPIO_SPEED_LOW;
    gpio_uart.Alternate = GPIO_AF7_USART2;
    /* enable GPIOA clk */
    HAL_RCC_GPIOA_CLK_ENABLE();
    /* init pins */
    HAL_GPIO_Init(GPIOA, &gpio_uart);
    
    huart.Init.BaudRate = 115200;
    huart.Init.Mode = HAL_UART_MODE_TX_RX;
    huart.Init.OverSampling = HAL_UART_OVERSAMPLING_16;
    huart.Init.Parity = HAL_UART_PARITY_NONE;
    huart.Init.StopBits = HAL_UART_STOP_1;
    huart.Init.WordLength = HAL_UART_WORD8;
    huart.Instance = USART2;
    
    HAL_RCC_USART2_CLK_ENABLE();
    HAL_UART_Init(&huart);
}

/**
 * @brief Transmit An ACK message over UART (Sequence {ACK, ACK})
 * 
 * @param handle 
 */
static void Transmit_ACK(UART_HandleTypeDef *handle)
{
    uint8_t msg[2] = {ACK, ACK};
    
    HAL_UART_Tx(handle, msg, 2);
}

/**
 * @brief Transmit An NACK message over UART (Sequence {NACK, NACK})
 * 
 * @param handle 
 */
static void Transmit_NACK(UART_HandleTypeDef *handle)
{
    uint8_t msg[2] = {NACK, NACK};
    
    HAL_UART_Tx(handle, msg, 2);
}

/**
 * @brief Calculate the cumulative XOR Checksum and Check if it is not zero
 * 
 * @param pBuffer pointer to a buffer of type uint8_t
 * @param len length of the buffer pointed to by pBuffer
 * @return uint8_t Status 0 : On Success 
 */
static uint8_t Check_Checksum(uint8_t *pBuffer, uint32_t len)
{
    uint8_t initial = 0xFF; // Initial XOR value
    uint8_t result = 0x7F; // not useful
    
    result = initial ^ *pBuffer++; // Initialize the result with the XOR checksum of the first byte
    len--;  // decement, already XORed the 1st Byte
    while(len--)
    {
        result ^= *pBuffer++; // XOR the remaining Bytes
    }
    
    result ^= 0xFF;
    
    if(result == 0x00)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Erase N sectors on internal Flash 
 * 
 */
static void Erase(void)
{
    Flash_EraseInitTypeDef flashEraseConfig;
    uint32_t sectorError;
    
    /* Try to receive and validate 3 bytes otherwise indicate NACK */
    while(HAL_UART_Rx(&huart, RX_Buffer, 3, TIMEOUT_VALUE) == HAL_UART_TIMEOUT);

    if(Check_Checksum(RX_Buffer, 3) != 1)
    {
        Transmit_NACK(&huart);
        return;
    }
    /* Don't allow received 1st byte with value 0xFF */
    if(RX_Buffer[0] == 0xFF)
    {
        Transmit_NACK(&huart);
    }
    else
    {
      /* Define flash erase config */

        flashEraseConfig.TypeErase = HAL_FLASH_TYPEERASE_SECTOR;
        flashEraseConfig.NbSectors = RX_Buffer[0];
        flashEraseConfig.Sector = RX_Buffer[1];

        HAL_Flash_Unlock(); // unlock flash 
        HAL_Flash_Erase(&flashEraseConfig, &sectorError); // erase flash 
        HAL_Flash_Lock(); // lock flash again
        
        Transmit_ACK(&huart); // indicate Successfull erase 
    }
}

/**
 * @brief 
 * 
 */
static void Write(void)
{
    uint8_t numBytes;
    uint32_t startingAddress = 0;
    uint8_t i;
    /* Try to receive and validate 5 bytes, if successfull ACK, otherwise indicate NACK */

    while(HAL_UART_Rx(&huart, RX_Buffer, 5, TIMEOUT_VALUE) == HAL_UART_TIMEOUT);

    if(Check_Checksum(RX_Buffer, 5) != 1)
    {
        Transmit_NACK(&huart);
        return;
    }
    else
    {
        Transmit_ACK(&huart);
    }
    // construct write address
    startingAddress = RX_Buffer[0] + (RX_Buffer[1] << 8) 
                    + (RX_Buffer[2] << 16) + (RX_Buffer[3] << 24);
    /* Try to receive 2 bytes */

    while(HAL_UART_Rx(&huart, RX_Buffer, 2, TIMEOUT_VALUE) == HAL_UART_TIMEOUT);
    numBytes = RX_Buffer[0]; // set number of bytes to be received 
    /* Try to receive numBytes+1 bytes */

    while(HAL_UART_Rx(&huart, RX_Buffer, numBytes+1, TIMEOUT_VALUE) == HAL_UART_TIMEOUT);
    
    if(Check_Checksum(RX_Buffer, 5) != 1) // validate checksum for the first 5 bytes
    {
        Transmit_NACK(&huart);
        return;
    }

    i = 0;
    HAL_Flash_Unlock(); // unlock flash 
    while(numBytes--)
    {
        HAL_Flash_Program(FLASH_TYPEPROGRAM_BYTE, startingAddress, RX_Buffer[i]); // write byte 
        startingAddress++;
        i++; 
    }
    HAL_Flash_Lock(); // lock flash again 
    Transmit_ACK(&huart); // indicate successfull write 
}

/**
 * @brief 
 * 
 */
static void Check(void)
{
    uint32_t startingAddress = 0;
    uint32_t endingAddress = 0;
    uint32_t address;
    uint32_t *data;
    uint32_t crcResult;
    /* Try to receive and validate 5 bytes, if successfull ACK, otherwise indicate NACK */

    while(HAL_UART_Rx(&huart, RX_Buffer, 5, TIMEOUT_VALUE) == HAL_UART_TIMEOUT);
    
    if(Check_Checksum(RX_Buffer, 5) != 1)
    {
        Transmit_NACK(&huart);
        return;
    }
    else
    {
        Transmit_ACK(&huart);
    }
    
    startingAddress = RX_Buffer[0] + (RX_Buffer[1] << 8)  // construct starting address 
                    + (RX_Buffer[2] << 16) + (RX_Buffer[3] << 24);
    
    /* Try to receive and validate 5 bytes, if successfull ACK, otherwise indicate NACK */

    while(HAL_UART_Rx(&huart, RX_Buffer, 5, TIMEOUT_VALUE) == HAL_UART_TIMEOUT);

    if(Check_Checksum(RX_Buffer, 5) != 1)
    {
        Transmit_NACK(&huart);
        return;
    }
    else
    {
        Transmit_ACK(&huart);
    }
    
    endingAddress = RX_Buffer[0] + (RX_Buffer[1] << 8) // construct ending address 
                    + (RX_Buffer[2] << 16) + (RX_Buffer[3] << 24);
    
    HAL_RCC_CRC_CLK_ENABLE(); // Enable HW CRC IP Clock
    data = (uint32_t *)((__IO uint32_t*) startingAddress); // CRC deals with words (32 bits)
    for(address = startingAddress; address < endingAddress; address += 4)
    {
        data = (uint32_t *)((__IO uint32_t*) address);
        crcResult = HAL_CRC_Accumulate(data, 1); // calculate Cumulated CRC 
    }
    
    HAL_RCC_CRC_CLK_DISABLE();  // Disable HW CRC IP Clock
    if(crcResult == 0x00) // either all values are zero , or CRC is appended to the data
    {
        Transmit_ACK(&huart);
    }
    else
    {
        Transmit_NACK(&huart);
    }
    
    Jump2App(); // jump to app
}