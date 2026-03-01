/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private define ------------------------------------------------------------*/
#define 	Stanby_Time 			20*64
#define 	DAC_BUFFER_SIZE			2000

typedef enum
{
	Stop=0,
	Playing,
	Endfile
}PlayerStatus;

typedef struct
{
	PlayerStatus	Status;
	FIL				File;
}AudioPlayer;

typedef struct
{
	uint8_t			Started;
	uint32_t		Start_time;

}Timer;

typedef struct
{
	uint32_t 	Stanby;
	uint8_t 	Slider;
	uint8_t 	Menu;
	uint8_t 	Play;
	uint8_t 	Shuffle;
	uint8_t 	DOEN;
	uint8_t 	DOTime;
	uint8_t 	DO;
	uint8_t 	OVL;
	uint8_t 	PH;
	uint8_t 	TR;
}seti;

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac2;
RNG_HandleTypeDef hrng;
SD_HandleTypeDef hsd;
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart2;

int 			cnth=0,cntv=0;
char 			Loaded_Images[32];
BYTE 			Dac_Buffer[DAC_BUFFER_SIZE];
BYTE 			Dac_Temp_Buffer[DAC_BUFFER_SIZE];
AudioPlayer 	Player;
BYTE			DAC_Status;
seti				Setting;

uint32_t input;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_DAC_Init(void);
static void MX_RNG_Init(void);
static void MX_TIM2_Init(void);

static void SRAM_Precharge(void);
static void SRAM_AutoRefresh(void);
static void SRAM_RowBankActive(unsigned int ad);
static void SRAM_Write(unsigned int ad,unsigned int data,char mode);
//static unsigned int SRAM_Read(unsigned int ad,char mode);
static void SRAM_RegisterSet();
static void SRAM_BurstStop(void);
static void SRAM_Init(void);
static void LCD_Hsync(char de,int bank);
static void LCD_Show(char sector);
static void Slide_Show(char a,char b);
static void LCD_Display(char state);
static void LCD_Init();
static void PicToRam(const TCHAR* path);
char ReadState(char Last);
uint16_t len(uint8_t *string);
FRESULT PlayStart(const TCHAR* path,DWORD seek);
FRESULT PlayUpdate();
void PlayStop();
void Load_Setting(const TCHAR* path);

/* USER CODE BEGIN PFP */

//##########################################################################
//##########################################################################

int main(void)
{
	FATFS mynewdiskFatFs;
	uint8_t ShowNumber;
	uint8_t Standby=0;
	uint8_t LastState;
	uint8_t State;
	Timer StanbyTimer;

/* MCU Configuration--------------------------------------------------------*/
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	/* Configure the system clock */
	SystemClock_Config();
	HAL_Delay(100);
	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_SDIO_SD_Init();
	MX_USART2_UART_Init();
	MX_DAC_Init();
	MX_RNG_Init();
	MX_TIM2_Init();
	MX_FATFS_Init();
/*------------------------------------------------------------------------- */

	LCD_Init();
	SRAM_Init();


	if(f_mount(&mynewdiskFatFs, SDPath, 1)==FR_OK)
	{
		PicToRam("/theme.thm");
		Load_Setting("/Setting.cnf");
	}

	LCD_Display(1);

	while (1)
	{
		if(Standby==1)            //################ StandBy ########################
		{
			SRAM_Precharge();
			SRAM_AutoRefresh();
			if(HAL_GPIO_ReadPin(GPIOC,IN_PH_Pin)==Setting.PH)
			{
				LCD_Display(1);
				Standby=2;
			}

		}
		else if(Standby==2)      //################ Return From StandBy ########################
		{
			if(Setting.Menu==1)
			{
				LCD_Show(10);
				State=ReadState(LastState);
				if(State!=LastState)
				{
					Standby=0;
				}
			}
			else
			{
				Standby=0;
			}

		}
		else					//################ Normal Mode ########################
		{
			State=ReadState(LastState);
			if(State==LastState)
			{
				if(StanbyTimer.Started==1)
				{
					if( HAL_GetTick()- StanbyTimer.Start_time > Setting.Stanby)
					{
						Standby=1;
						StanbyTimer.Started=0;
						StanbyTimer.Start_time=0;
						PlayStop();
						LCD_Display(0);
					}
				}
				else
				{
					StanbyTimer.Started=1;
					StanbyTimer.Start_time=HAL_GetTick();
				}

				LCD_Show(State);

			}
			else
			{
				StanbyTimer.Started=0;
				StanbyTimer.Start_time=0;
				if(Setting.Slider==1) Slide_Show(LastState, State);
				LastState=State;
			}

		}

	}

}

//##########################################################################
//##########################################################################

void Load_Setting(const TCHAR* path)
{
	UINT wbytes=0;
	BYTE buffer[11];
	FIL MyFile;

	if(f_open(&MyFile,path, FA_READ)==FR_OK)
	{
		f_lseek(&MyFile,10);
		f_read(&MyFile,buffer,11,&wbytes);
		Setting.Stanby=buffer[0]*1000;
		Setting.Slider=buffer[1];
		Setting.Menu=buffer[2];
		Setting.Play=buffer[3];
		Setting.Shuffle=buffer[4];
		Setting.DOEN=buffer[5];
		Setting.DOTime=buffer[6];
		Setting.DO=buffer[7];
		Setting.OVL=buffer[8];
		Setting.PH=buffer[9];
		Setting.TR=buffer[10];
	}
	else
	{
		Setting.DO=1;
		Setting.OVL=1;
		Setting.PH=1;
		Setting.TR=1;
		Setting.DOEN=0;
		Setting.DOTime=20;
		Setting.Menu=1;
		Setting.Play=3;
		Setting.Shuffle=1;
		Setting.Slider=1;
		Setting.Stanby=20000;
	}
}

FRESULT PlayUpdate()
{
	FRESULT re;
	UINT wbytes;

	if(Player.Status==Playing)
	{
		re=f_read(&Player.File,(char*)Dac_Temp_Buffer,DAC_BUFFER_SIZE,&wbytes);
		if(wbytes!=0)
		{
			for(int i=1;i<DAC_BUFFER_SIZE;i+=2)
						 *(Dac_Temp_Buffer+i) = *(Dac_Temp_Buffer+i)^0x80;
		}
		else
		{
			PlayStop();
			Player.Status=Endfile;
		}

		DAC_Status=0;

	}
}

void PlayStop()
{
	HAL_DAC_Stop_DMA(&hdac, DAC1_CHANNEL_2);
	HAL_TIM_Base_Stop(&htim2);
	Player.Status=Stop;
}

FRESULT PlayStart(const TCHAR* path,DWORD seek)
{
	FRESULT re;
	UINT wbytes;

	HAL_DAC_Stop_DMA(&hdac, DAC1_CHANNEL_2);
	HAL_TIM_Base_Stop(&htim2);
	Player.Status=Stop;

	re=f_open(&Player.File,path,FA_READ);
	if(re==FR_OK)
	{
		f_lseek(&Player.File, 46+seek);
		re=f_read(&Player.File,(char*)Dac_Buffer,DAC_BUFFER_SIZE,&wbytes);
		for(int i=1;i<DAC_BUFFER_SIZE;i+=2)
			 *(Dac_Buffer+i) = *(Dac_Buffer+i)^0x80;
		re=f_read(&Player.File,(char*)Dac_Temp_Buffer,DAC_BUFFER_SIZE,&wbytes);
		for(int i=1;i<DAC_BUFFER_SIZE;i+=2)
			*(Dac_Temp_Buffer+i) = *(Dac_Temp_Buffer+i)^0x80;

		HAL_DAC_Start_DMA(&hdac, DAC1_CHANNEL_2,(char*)Dac_Buffer, DAC_BUFFER_SIZE/2, DAC_ALIGN_12B_L);
		HAL_TIM_Base_Start(&htim2);
		Player.Status=Playing;
		DAC_Status=0;
	}

	return re;
}


uint16_t len(uint8_t *string)
{
	uint16_t i=0;
	while( string[i] != '\0')
		i++;

	return i;
}
char ReadState(char Last)
{
	char State=0;

	input=(GPIOD->IDR) & 0xFF80;

	switch(input)
	{
		case 0x8C80:    //P
			State=14;
			break;
		case 0xAF80:    //R
			State=15;
			break;
		case 0xC780:    //L
			State=16;
			break;
		case 0x8380:    //B
			State=17;
			break;
		case 0x1980:    //-4
			State=18;
			break;
		case 0x3080:    //-3
			State=19;
			break;
		case 0x2480:    //-2
			State=20;
			break;
		case 0x7980:    //-1
			State=21;
			break;
		case 0xC080:    //00
			State=22;
			break;
		case 0xF980:    //01
			State=23;
			break;
		case 0xA480:    //02
			State=24;
			break;
		case 0xB080:    //03
			State=25;
			break;
		case 0x9980:    //04
			State=26;
			break;
		case 0x9280:    //05
			State=27;
			break;
		case 0x8280:    //06
			State=28;
			break;
		case 0xF880:    //07
			State=29;
			break;
		case 0x8080:    //08
			State=30;
			break;
		case 0x9080:    //09
			State=31;
			break;
		case 0xC000:    //10
			State=32;
			break;
		case 0xF900:    //11
			State=33;
			break;
		case 0xA400:    //12
			State=34;
			break;
		case 0xB000:    //13
			State=35;
			break;
		case 0x9900:    //14
			State=36;
			break;
		case 0x9200:    //15
			State=37;
			break;
		case 0x8200:    //16
			State=38;
			break;
		case 0xF800:    //17
			State=39;
			break;
		case 0x8000:    //18
			State=40;
			break;
		case 0x9000:    //19
			State=41;
			break;
		default:
			State=Last;
			break;
	}
	return State;
}

static void SRAM_Precharge(void)
{
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOF, RAM_CS_Pin|RAM_RAS_Pin|RAM_WE_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOF, RAM_CAS_Pin|RAM_A10_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=0xC0052002;
	GPIOC->BSRR=0xC0052002;
	GPIOC->BSRR=0xC0052002;
	GPIOB->BSRR=0x400;
	GPIOB->BSRR=0x400;
	GPIOB->BSRR=0x400;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOF, RAM_CS_Pin , GPIO_PIN_SET);
	GPIOC->BSRR=0x40008000;
	GPIOC->BSRR=0x40008000;
	GPIOC->BSRR=0x40008000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
	GPIOC->BSRR=0x40000000;
	GPIOC->BSRR=0x40000000;
	GPIOC->BSRR=0x40000000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
	GPIOC->BSRR=0x40000000;
	GPIOC->BSRR=0x40000000;
	GPIOC->BSRR=0x40000000;
}
static void SRAM_AutoRefresh(void)
{
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOF, RAM_CS_Pin|RAM_RAS_Pin|RAM_CAS_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOF, RAM_WE_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=0xC0032004;
	GPIOC->BSRR=0xC0032004;
	GPIOC->BSRR=0xC0032004;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOF, RAM_CS_Pin , GPIO_PIN_SET);
	GPIOC->BSRR=0x40008000;
	GPIOC->BSRR=0x40008000;
	GPIOC->BSRR=0x40008000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
	GPIOC->BSRR=0x40000000;
	GPIOC->BSRR=0x40000000;
	GPIOC->BSRR=0x40000000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
	GPIOC->BSRR=0x40000000;
	GPIOC->BSRR=0x40000000;
	GPIOC->BSRR=0x40000000;
}
static void SRAM_RowBankActive(unsigned int ad)
{
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOF, RAM_CS_Pin|RAM_RAS_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOF, RAM_WE_Pin|RAM_CAS_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=0xC0012006;
	GPIOC->BSRR=0xC0012006;
	GPIOC->BSRR=0xC0012006;

	GPIOA->BSRR=0xFF0000;
	GPIOB->BSRR=0xFF200000;
	GPIOA->BSRR=(ad & 0xFF);
	GPIOA->BSRR=(ad & 0xFF);
	GPIOB->BSRR=(ad & 0xFF20);
	GPIOB->BSRR=(ad & 0xFF20);
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOF, RAM_CS_Pin , GPIO_PIN_SET);
	GPIOC->BSRR=0x40008000;
	GPIOC->BSRR=0x40008000;
	GPIOC->BSRR=0x40008000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
	GPIOC->BSRR=0x40000000;
	GPIOC->BSRR=0x40000000;
	GPIOC->BSRR=0x40000000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
	GPIOC->BSRR=0x40000000;
	GPIOC->BSRR=0x40000000;
	GPIOC->BSRR=0x40000000;
}
static void SRAM_Write(unsigned int ad,unsigned int data,char mode)
{
	if(mode==0)
	{
		HAL_GPIO_WritePin(GPIOC, RAM_CLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, RAM_CS_Pin|RAM_CAS_Pin|RAM_WE_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, RAM_RAS_Pin, GPIO_PIN_SET);
		//GPIOF->BSRR=0x1B0004;
		GPIOA->BSRR=0xFF0000;
		GPIOB->BSRR=0xFF200000;
		GPIOA->BSRR=(ad & 0xFF);
		GPIOB->BSRR=(ad & 0xFF20)|(1<<10);
		GPIOE->MODER=0x55555555;
		GPIOE->ODR=data;
		HAL_GPIO_WritePin(GPIOC, RAM_CLK_Pin, GPIO_PIN_SET);
		//GPIOF->BSRR=1;
		HAL_GPIO_WritePin(GPIOC, RAM_CLK_Pin, GPIO_PIN_RESET);
		//GPIOF->BSRR=(1<<16);
		HAL_GPIO_WritePin(GPIOC, RAM_CS_Pin , GPIO_PIN_SET);
		//GPIOF->BSRR=1;
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, RAM_CLK_Pin, GPIO_PIN_RESET);
		//GPIOF->BSRR=(1<<16);
		GPIOE->ODR=data;
		HAL_GPIO_WritePin(GPIOC, RAM_CLK_Pin, GPIO_PIN_SET);
		//GPIOF->BSRR=1;
	}

}
/*
static unsigned int SRAM_Read(unsigned int ad,char mode)
{
	unsigned int data=0;
	if(mode==0)
	{
		//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(GPIOF, RAM_CS_Pin|RAM_CAS_Pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(GPIOF, RAM_RAS_Pin|RAM_WE_Pin, GPIO_PIN_SET);
		GPIOF->BSRR=0xB0014;
		GPIOE->ODR=ad|(1<<10);
		GPIOG->MODER=0;
		//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
		GPIOF->BSRR=1;
		//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(GPIOF, RAM_CS_Pin , GPIO_PIN_SET);
		GPIOF->BSRR=0x10002;
		//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
		GPIOF->BSRR=1;
		//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
		GPIOF->BSRR=(1<<16);
		//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
		GPIOF->BSRR=1;
		data=GPIOG->IDR;
		//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
		GPIOF->BSRR=(1<<16);
	}
	else
	{
		//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
		GPIOF->BSRR=1;
		data=GPIOG->IDR;
		//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
		GPIOF->BSRR=(1<<16);
	}
	return data;
}

*/
static void SRAM_RegisterSet()
{
	HAL_GPIO_WritePin(GPIOC, RAM_CLK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, RAM_CS_Pin|RAM_RAS_Pin|RAM_WE_Pin|RAM_CAS_Pin, GPIO_PIN_RESET);
	GPIOA->BSRR=0xFF0000;
	GPIOB->BSRR=0xFF000020;
	GPIOA->BSRR=0x07;
	HAL_GPIO_WritePin(GPIOC, RAM_CLK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, RAM_CLK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, RAM_CS_Pin|RAM_RAS_Pin , GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, RAM_CLK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, RAM_CLK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, RAM_CLK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, RAM_CLK_Pin, GPIO_PIN_RESET);
}
static void SRAM_BurstStop(void)
{
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOF, RAM_CS_Pin|RAM_WE_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOF, RAM_RAS_Pin|RAM_CAS_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=0xC0042003;
	GPIOC->BSRR=0xC0042003;
	GPIOC->BSRR=0xC0042003;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOF, RAM_CS_Pin , GPIO_PIN_SET);
	GPIOC->BSRR=0x40008000;
	GPIOC->BSRR=0x40008000;
	GPIOC->BSRR=0x40008000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
	GPIOC->BSRR=0x40000000;
	GPIOC->BSRR=0x40000000;
	GPIOC->BSRR=0x40000000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	GPIOC->BSRR=0x4000;
	//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
	GPIOC->BSRR=0x40000000;
	GPIOC->BSRR=0x40000000;
	GPIOC->BSRR=0x40000000;
}
static void SRAM_Init(void)
{
	HAL_GPIO_WritePin(GPIOC, RAM_CKE_Pin, GPIO_PIN_SET);
	SRAM_Precharge();
	SRAM_AutoRefresh();
	SRAM_RegisterSet();
}

static void PicToRam(const TCHAR* path)
{

	UINT wbytes=0;
	BYTE buffer[960];
	WORD color=0;
	FIL MyFile;
	int ad,bank;

	if(f_open(&MyFile,path, FA_READ)==FR_OK)
	{
		f_lseek(&MyFile,10);
		f_read(&MyFile,Loaded_Images,32,&wbytes);

		/* Load Logo Picture ------------------------------------------ */
		ad=0;
		bank=0;
		for(int j=0;j<272;j++)
		{
			f_read(&MyFile,buffer,960,&wbytes);
			color=( (buffer[0] << 8) | buffer[1] );
			SRAM_RowBankActive(j+ad);
			SRAM_Write(bank,color,0);
			for(int c=1;c<480;c++)
			{
				color=( (buffer[c*2] << 8) | buffer[c*2+1] );
				SRAM_Write(bank,color,1);
			}

			SRAM_BurstStop();
			SRAM_Precharge();
		}

		/* Load other Pictures ------------------------------------------ */
		for(int c=0;c<32;c++)
		{
			ad=(c+10)*272;
			bank=ad & 0x6000;
			if(Loaded_Images[c]==1)
			{
				for(int j=0;j<272;j++)
				{
					f_read(&MyFile,buffer,960,&wbytes);
					color=( (buffer[0] << 8) | buffer[1] );
					SRAM_RowBankActive(j+ad);
					SRAM_Write(bank,color,0);
					for(int c=1;c<480;c++)
					{
						color=( (buffer[c*2] << 8) | buffer[c*2+1] );
						SRAM_Write(bank,color,1);
					}

					SRAM_BurstStop();
					SRAM_Precharge();
				}
			}
			/*
			else
			{
				color=0xFFFF;
				for(int j=0;j<272;j++)
				{
					for(int a=0;a<4;a++)
					{
						if(a==0)
						{
							SRAM_RowBankActive(j+ad);
							SRAM_Write(bank,color,0);
							for(int c=1;c<120;c++)
							{
								SRAM_Write(bank,color,1);
							}
						}
						else
						{
							for(int c=0;c<120;c++)
							{
								SRAM_Write(bank,color,1);
							}
						}
					}
					SRAM_BurstStop();
					SRAM_Precharge();
				}

			}
			*/

		}

		f_close(&MyFile);
	}


}

static void LCD_Display(char state)
{
	HAL_GPIO_WritePin(GPIOA,LCD_DIS_Pin,state);
	HAL_GPIO_WritePin(GPIOA,LCD_BLK_Pin, state);
}
static void LCD_Init()
{
	HAL_GPIO_WritePin(GPIOA,LCD_HSY_Pin|LCD_CLK_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,LCD_VSY_Pin|LCD_DE_Pin, GPIO_PIN_RESET);
}
static void LCD_Hsync(char de,int bank)
{
	//HAL_GPIO_WritePin(GPIOB,LCD_HSY_Pin, GPIO_PIN_RESET);
	GPIOA->BSRR=(1<<24);
	//HAL_GPIO_WritePin(GPIOB,LCD_CLK_Pin, GPIO_PIN_RESET);
	GPIOA->BSRR=(1<<26);
	//HAL_GPIO_WritePin(GPIOB,LCD_CLK_Pin, GPIO_PIN_SET);
	GPIOA->BSRR=(1<<10);
	//HAL_GPIO_WritePin(GPIOB,LCD_HSY_Pin, GPIO_PIN_SET);
	GPIOA->BSRR=(1<<8);
	cnth=0;
	while(cnth<45)   //39
	{
		//HAL_GPIO_WritePin(GPIOB,LCD_CLK_Pin, GPIO_PIN_RESET);
		GPIOA->BSRR=(1<<26);
		//HAL_GPIO_WritePin(GPIOB,LCD_CLK_Pin, GPIO_PIN_SET);
		GPIOA->BSRR=(1<<10);
		cnth++;
	}

	if(de==1)
	{
		//########################## SRAM_Read #########################################################
		//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(GPIOF, RAM_CS_Pin|RAM_CAS_Pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(GPIOF, RAM_RAS_Pin|RAM_WE_Pin, GPIO_PIN_SET);
		GPIOC->BSRR=0xC0022005;
		GPIOC->BSRR=0xC0022005;
		GPIOC->BSRR=0xC0022005;
		GPIOA->BSRR=0xFF0000;
		GPIOB->BSRR=0xFF200000;
		GPIOB->BSRR=bank;
		GPIOE->MODER=0;
		//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
		GPIOC->BSRR=0x4000;
		GPIOC->BSRR=0x4000;
		GPIOC->BSRR=0x4000;
		//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(GPIOF, RAM_CS_Pin , GPIO_PIN_SET);
		GPIOC->BSRR=0x40008000;
		GPIOC->BSRR=0x40008000;
		GPIOC->BSRR=0x40008000;
		//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_SET);
		//GPIOC->BSRR=0x4000;
		//GPIOC->BSRR=0x4000;
		//GPIOC->BSRR=0x4000;
		//HAL_GPIO_WritePin(GPIOF, RAM_CLK_Pin, GPIO_PIN_RESET);
		//GPIOC->BSRR=0x40000000;
		//GPIOC->BSRR=0x40000000;
		//GPIOC->BSRR=0x40000000;
		//###############################################################################################
		//HAL_GPIO_WritePin(GPIOB,LCD_DE_Pin, de);
		GPIOC->BSRR=(1<<6);

		while(cnth<525)
		{
			GPIOA->BSRR=(1<<10);						//set LCD_CLK
			GPIOC->BSRR=0x4000;							//set SRAM_CLK
			GPIOA->BSRR=(1<<10);						//set LCD_CLK
			GPIOC->BSRR=0x4000;							//set SRAM_CLK
			GPIOA->BSRR=(1<<10);						//set LCD_CLK
			GPIOC->BSRR=0x4000;							//set SRAM_CLK

			GPIOA->BSRR=(1<<26); 						//reset SRAM_CLK
			GPIOC->BSRR=0x40000000; 					//reset LCD_CLK
			GPIOA->BSRR=(1<<26); 						//reset SRAM_CLK
			GPIOC->BSRR=0x40000000; 					//reset LCD_CLK
			GPIOA->BSRR=(1<<26); 						//reset SRAM_CLK
			GPIOC->BSRR=0x40000000; 					//reset LCD_CLK
			cnth++;
		}
		//HAL_GPIO_WritePin(GPIOB,LCD_DE_Pin, 0);
		GPIOC->BSRR=(1<<22);

	}
	else
	{
		while(cnth<525)
		{
			//HAL_GPIO_WritePin(GPIOB,LCD_CLK_Pin, GPIO_PIN_RESET);
			GPIOA->BSRR=(1<<26);
			//HAL_GPIO_WritePin(GPIOB,LCD_CLK_Pin, GPIO_PIN_SET);
			GPIOA->BSRR=(1<<10);
			cnth++;
		}

	}

	//while(cnth<525)
	//{
	//	//HAL_GPIO_WritePin(GPIOB,LCD_CLK_Pin, GPIO_PIN_RESET);
	//	GPIOA->BSRR=(1<<26);
	//	//HAL_GPIO_WritePin(GPIOB,LCD_CLK_Pin, GPIO_PIN_SET);
	//	GPIOA->BSRR=(1<<10);
	//	cnth++;
	//}

}
static void LCD_Show(char sector)
{
	int row;
	int bank;
	row=sector*272;
	bank=row & 0x6000;
	if(DAC_Status) PlayUpdate();

	//HAL_GPIO_WritePin(GPIOB,LCD_VSY_Pin, GPIO_PIN_RESET);
	GPIOC->BSRR=(1<<23);
	LCD_Hsync(0,bank);
	//HAL_GPIO_WritePin(GPIOB,LCD_VSY_Pin, GPIO_PIN_SET);
	GPIOC->BSRR=(1<<7);
	cntv=0;
	while(cntv<8)			//vertical back porch7
	{
		LCD_Hsync(0,bank);
		cntv++;
	}

	cntv=0;
	while(cntv<272)			//vertical data
	{
		SRAM_RowBankActive(cntv+row);
		LCD_Hsync(1,bank);
		cntv++;
		SRAM_BurstStop();
		SRAM_Precharge();
		SRAM_AutoRefresh();
		//SRAM_RegisterSet();
	}

	while(cntv<288)			//vertical front porch8
	{
		LCD_Hsync(0,bank);
		cntv++;
	}

}

static void Slide_Show(char a,char b)
{
	int a_row,a_bank,b_row,b_bank;

	a_row=a*272;
	a_bank=a_row & 0x6000;

	b_row=b*272;
	b_bank=b_row & 0x6000;

	for(int i=0;i<34;i++)
	{
		if(DAC_Status) PlayUpdate();

		//HAL_GPIO_WritePin(GPIOB,LCD_VSY_Pin, GPIO_PIN_RESET);
		GPIOC->BSRR=(1<<23);
		LCD_Hsync(0,0);
		//HAL_GPIO_WritePin(GPIOB,LCD_VSY_Pin, GPIO_PIN_SET);
		GPIOC->BSRR=(1<<7);
		cntv=0;
		while(cntv<16)			//vertical back porch7
		{
			LCD_Hsync(0,0);
			cntv++;
		}

		if(a>b)
		{
			for(int m=i*8;m<272;m++)		//vertical data
			{

				SRAM_RowBankActive(m+a_row);
				LCD_Hsync(1,a_bank);
				SRAM_BurstStop();
				SRAM_Precharge();
				SRAM_AutoRefresh();
				//SRAM_RegisterSet();
			}
			for(int n=0;n<i*8;n++)		//vertical data
			{

				SRAM_RowBankActive(n+b_row);
				LCD_Hsync(1,b_bank);
				SRAM_BurstStop();
				SRAM_Precharge();
				SRAM_AutoRefresh();
				//SRAM_RegisterSet();
			}
		}
		else
		{
			for(int m=272-i*8;m<272;m++)		//vertical data
			{

				SRAM_RowBankActive(m+b_row);
				LCD_Hsync(1,b_bank);
				SRAM_BurstStop();
				SRAM_Precharge();
				SRAM_AutoRefresh();
				//SRAM_RegisterSet();
			}
			for(int n=0;n<272-i*8;n++)		//vertical data
			{

				SRAM_RowBankActive(n+a_row);
				LCD_Hsync(1,a_bank);
				SRAM_BurstStop();
				SRAM_Precharge();
				SRAM_AutoRefresh();
				//SRAM_RegisterSet();
			}
		}


		//while(cntv<288)			//vertical front porch8
		//{
		//	LCD_Hsync(0,bank);
		//	cntv++;
		//}

	}

}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */
  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT2 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

static void MX_SDIO_SD_Init(void)
{

  /* USER CODE BEGIN SDIO_Init 0 */

  /* USER CODE END SDIO_Init 0 */

  /* USER CODE BEGIN SDIO_Init 1 */

  /* USER CODE END SDIO_Init 1 */
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 1;
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */

}

static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 21;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 121;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

static void MX_DMA_Init(void)
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, RAM_CKE_Pin|RAM_CLK_Pin|RAM_CS_Pin|RAM_RAS_Pin
                          |RAM_CAS_Pin|RAM_WE_Pin|LCD_DE_Pin|LCD_VSY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RAM_A0_Pin|RAM_A1_Pin|RAM_A2_Pin|RAM_A3_Pin
                          |RAM_A4_Pin|RAM_A6_Pin|RAM_A7_Pin|LCD_HSY_Pin
                          |LCD_DIS_Pin|LCD_CLK_Pin|LCD_BLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RAM_A10_Pin|RAM_A11_Pin|RAM_A12_Pin|RAM_A13_Pin
                          |RAM_A14_Pin|RAM_A15_Pin|RAM_A5_Pin|RAM_A8_Pin
                          |RAM_A9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE2 PE3 PE4 PE5
                           PE6 PE7 PE8 PE9
                           PE10 PE11 PE12 PE13
                           PE14 PE15 PE0 PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : RAM_CKE_Pin RAM_CLK_Pin RAM_CS_Pin */
  GPIO_InitStruct.Pin = RAM_CKE_Pin|RAM_CLK_Pin|RAM_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : RAM_RAS_Pin RAM_CAS_Pin RAM_WE_Pin LCD_DE_Pin
                           LCD_VSY_Pin */
  GPIO_InitStruct.Pin = RAM_RAS_Pin|RAM_CAS_Pin|RAM_WE_Pin|LCD_DE_Pin
                          |LCD_VSY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : IN_TR_Pin IN_PH_Pin IN_UP_Pin */
  GPIO_InitStruct.Pin = IN_TR_Pin|IN_PH_Pin|IN_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : RAM_A0_Pin RAM_A1_Pin RAM_A2_Pin RAM_A3_Pin
                           RAM_A4_Pin RAM_A6_Pin RAM_A7_Pin LCD_HSY_Pin
                           LCD_DIS_Pin LCD_CLK_Pin */
  GPIO_InitStruct.Pin = RAM_A0_Pin|RAM_A1_Pin|RAM_A2_Pin|RAM_A3_Pin
                          |RAM_A4_Pin|RAM_A6_Pin|RAM_A7_Pin|LCD_HSY_Pin
                          |LCD_DIS_Pin|LCD_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : IN_DN_Pin IN_OVL_Pin IN_DO_Pin */
  GPIO_InitStruct.Pin = IN_DN_Pin|IN_OVL_Pin|IN_DO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : RAM_A10_Pin RAM_A11_Pin RAM_A12_Pin RAM_A13_Pin
                           RAM_A14_Pin RAM_A15_Pin RAM_A5_Pin RAM_A8_Pin
                           RAM_A9_Pin */
  GPIO_InitStruct.Pin = RAM_A10_Pin|RAM_A11_Pin|RAM_A12_Pin|RAM_A13_Pin
                          |RAM_A14_Pin|RAM_A15_Pin|RAM_A5_Pin|RAM_A8_Pin
                          |RAM_A9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : IN_A_Pin IN_B_Pin IN_C_Pin IN_D_Pin
                           IN_E_Pin IN_F_Pin IN_G_Pin IN_10_Pin */
  GPIO_InitStruct.Pin = IN_A_Pin|IN_B_Pin|IN_C_Pin|IN_D_Pin
                          |IN_E_Pin|IN_F_Pin|IN_G_Pin|IN_10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : IN___Pin */
  GPIO_InitStruct.Pin = IN___Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(IN___GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_BLK_Pin */
  GPIO_InitStruct.Pin = LCD_BLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_BLK_GPIO_Port, &GPIO_InitStruct);

}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
