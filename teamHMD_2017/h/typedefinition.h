/*
 * typedefinition.h
 *
 *  Created on: 2017. 6. 26.
 *      Author: ghbae
 */

#ifndef TYPEDEFINITION_H_
#define TYPEDEFINITION_H_

/*******************************************************************************
 * Include
 *******************************************************************************/

/*******************************************************************************
 * Constant
 *******************************************************************************/
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef ENABLE
#define ENABLE 1
#endif

#ifndef DISABLE
#define DISABLE 0
#endif

/*******************************************************************************
 * Define & MACRO
 *******************************************************************************/

/*******************************************************************************
 * Type Definition
 *******************************************************************************/
typedef unsigned char   bool;
typedef unsigned char   uint8;
typedef signed   char   sint8;
typedef unsigned short  uint16;
typedef signed   short  sint16;
typedef unsigned long   uint32;
typedef signed   long   sint32;
typedef float           float32;
typedef double          float64;

typedef volatile uint8  vuint8;
typedef volatile sint8  vsint8;
typedef volatile uint16 vuint16;
typedef volatile sint16 vsint16;
typedef volatile uint32 vuint32;
typedef volatile sint32 vsint32;

typedef int pixel;
typedef double edge;

/*******************************************************************************
 * Member
 *******************************************************************************/

/*******************************************************************************
 * Member Functions
 *******************************************************************************/

#endif /* TYPEDEFINITION_H_ */
