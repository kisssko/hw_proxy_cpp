
#include <stdint.h>
// #include <stdlib.h>
#include <stdio.h>

#include "hw_proxy.hpp"

#define UART0_BASE		0x08000000

#define UART_RDATA		0x00000010
#define UART_WDATA		0x00000020

static void mtdcr(uintptr_t a, uint32_t v);
static uint32_t mfdcr(uintptr_t a);

// int printf(const char*,...);
// void puts(const char*);

typedef uint32_t dcr_reg_t;

template <typename U, hw_proxy_access A = RDWR>
using DCR_PROXY			= HW_PROXY<dcr_reg_t, U, A, mfdcr, mtdcr>;
using DCR_PROXY32_NONE	= DCR_PROXY<uint32_t, NONE>;
using DCR_PROXY32_RD	= DCR_PROXY<uint32_t, RD>;
using DCR_PROXY32_WR	= DCR_PROXY<uint32_t, WR>;
using DCR_PROXY32_RDWR	= DCR_PROXY<uint32_t, RDWR>;
using DCR_PROXY8		= DCR_PROXY<uint8_t,  RDWR>;

typedef struct // Just example
{
	DCR_PROXY32_NONE	__unused1[4];
	DCR_PROXY32_RDWR	RDATA;
	DCR_PROXY32_NONE	__unused2[3];
	DCR_PROXY32_RDWR	WDATA;
	DCR_PROXY32_RDWR	CONTROL;
	DCR_PROXY32_RDWR	COUNT;
	DCR_PROXY32_RD		STATUS;
} UART_REGS;

uint32_t		 dcr_example_val = 0;
DCR_PROXY8		*UART0			 =  (DCR_PROXY8*) UART0_BASE;
UART_REGS		&UART0_REGS 	 = *(UART_REGS*)  UART0_BASE;

static void mtdcr(uintptr_t a, uint32_t v)
{
	printf("%s: Writing %08X to 0x%016X\n", __FUNCTION__, v, a);
}

static uint32_t mfdcr(uintptr_t a)
{
	printf("%s: Reading %08X from 0x%016X\n",
		__FUNCTION__, dcr_example_val, a);
	return dcr_example_val;
}

int main(int argc, char **argv)
{
	uint32_t v;
	printf("sizeof(DCR_PROXY32_RDWR) == %u\n", sizeof(DCR_PROXY32_RDWR));
	printf("sizeof(DCR_PROXY8) == %u\n",       sizeof(DCR_PROXY8));
    // Structure based demo
    dcr_example_val  = 0x5A5A5A5A;
	puts("-----");
	puts("structure test:");
	UART0_REGS.WDATA = 0x12345678;
	v = UART0_REGS.RDATA;
	printf("Value 0x%08X was read\n", v);
	printf("Value 0x%08X was read\n", UART0_REGS.RDATA());
	printf("Value 0x%08X was read\n", (uint32_t)UART0_REGS.RDATA);
	// Array based demo
	puts("-----");
	puts("array test:");
	dcr_example_val  ^= ~0U;
	UART0[UART_WDATA] = 0x87654321;
	v = UART0[UART_RDATA];
	printf("Value 0x%08X was read\n", v);
	printf("Value 0x%08X was read\n", UART0[UART_RDATA]());
	printf("Value 0x%08X was read\n", (uint32_t)UART0[UART_RDATA]);
	v = UART0_REGS.RDATA + UART0_REGS.RDATA;
	printf("Value 0x%08X was read\n", v);
	puts("-----");
	UART0_REGS.STATUS = ~0;
	puts("-----");
	++UART0_REGS.COUNT;
	UART0_REGS.COUNT++;
	puts("-----");
	UART0_REGS.COUNT   +=  4;
	UART0_REGS.CONTROL |=  0x000000FF;
	UART0_REGS.CONTROL &= ~0xFF000000;
	puts("-----");
	UART0_REGS.CONTROL = UART0_REGS.STATUS;

	return 0;
}

