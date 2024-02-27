/***************************************************************************/
/* Author       : Omar Wael                                                */
/* Version      : V0.0.0                                                   */
/*  Date        : Oct 31, 2023                                             */
/*  Description : Driver Functions Implementation                          */
/*  Features    : Register Definition                                      */
/***************************************************************************/

/***************************************************************************/
/*                  File Guard Will Call On Time in .c File                */
/***************************************************************************/
#ifndef MCAL_MNVIC_MNVIC_PRIVATE_H_
#define MCAL_MNVIC_MNVIC_PRIVATE_H_

/*********************************** REGISTERS AND ADDRESSES ************************************/

/***************************************************************************/
/*                      Base Address Of NVIC Register                      */
/***************************************************************************/
#define MNVIC_BASE_ADDR					 (0xE000E100)


/**************************************************************************/
/* Register Definition :-                                                 */
/* Features    : Private File Can`t Be Edit By User                       */
/* Description :-                                                         */
/*               - #define :      YES                                     */
/*               - union   :      NO                                      */
/*               - struct  :      YES                                     */
/**************************************************************************/

/********************** NVIC REGISTERS ***************************/
typedef struct {

	u32 ISER[8];
	u32 Reserved2[24];
	u32 ICER[8];
	u32 Reserved3[24];
	u32 ISPR[8];
	u32 Reserved4[24];
	u32 ICPR[8];
	u32 Reserved5[24];
	u32 IABR[8];
	u32 Reserved6[56];
	u8 IPR[60];
	u32 Reserved7[240];
	u32 STIR;

}MNVIC_Mem_Map_t;

/******************* Pointer to struct ***************/
#define NVIC					((volatile MNVIC_Mem_Map_t*)MNVIC_BASE_ADDR)

#define SCB_AIRCR				*((volatile u32*)(0xE000ED0C))

#define VECTKEY					(0x05FA0000)

#endif /* MCAL_MNVIC_MNVIC_PRIVATE_H_ */
