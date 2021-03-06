/*
 * arch/arm/mach-at91rm9200/at91sam9260.c
 *
 *  Copyright (C) 2006 SAN People
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include <linux/module.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/arch/at91sam9260.h>
#include <asm/arch/at91_pmc.h>
#include <asm/arch/at91_rstc.h>

#include "generic.h"
#include "clock.h"

static struct map_desc at91sam9260_io_desc[] __initdata = {
	{
		.virtual	= AT91_VA_BASE_SYS,
		.pfn		= __phys_to_pfn(AT91_BASE_SYS),
		.length		= SZ_16K,
		.type		= MT_DEVICE,
	}, {
		.virtual	= AT91_IO_VIRT_BASE - AT91SAM9260_SRAM0_SIZE,
		.pfn		= __phys_to_pfn(AT91SAM9260_SRAM0_BASE),
		.length		= AT91SAM9260_SRAM0_SIZE,
		.type		= MT_DEVICE,
	}, {
		.virtual	= AT91_IO_VIRT_BASE - AT91SAM9260_SRAM0_SIZE - AT91SAM9260_SRAM1_SIZE,
		.pfn		= __phys_to_pfn(AT91SAM9260_SRAM1_BASE),
		.length		= AT91SAM9260_SRAM1_SIZE,
		.type		= MT_DEVICE,
	},
};

/* --------------------------------------------------------------------
 *  Clocks
 * -------------------------------------------------------------------- */

/*
 * The peripheral clocks.
 */
static struct clk pioA_clk = {
	.name		= "pioA_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_PIOA,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk pioB_clk = {
	.name		= "pioB_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_PIOB,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk pioC_clk = {
	.name		= "pioC_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_PIOC,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk adc_clk = {
	.name		= "adc_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_ADC,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk usart0_clk = {
	.name		= "usart0_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_US0,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk usart1_clk = {
	.name		= "usart1_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_US1,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk usart2_clk = {
	.name		= "usart2_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_US2,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk mmc_clk = {
	.name		= "mci_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_MCI,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk udc_clk = {
	.name		= "udc_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_UDP,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk twi_clk = {
	.name		= "twi_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_TWI,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk spi0_clk = {
	.name		= "spi0_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_SPI0,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk spi1_clk = {
	.name		= "spi1_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_SPI1,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk ohci_clk = {
	.name		= "ohci_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_UHP,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk ether_clk = {
	.name		= "ether_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_EMAC,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk isi_clk = {
	.name		= "isi_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_ISI,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk usart3_clk = {
	.name		= "usart3_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_US3,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk usart4_clk = {
	.name		= "usart4_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_US4,
	.type		= CLK_TYPE_PERIPHERAL,
};
static struct clk usart5_clk = {
	.name		= "usart5_clk",
	.pmc_mask	= 1 << AT91SAM9260_ID_US5,
	.type		= CLK_TYPE_PERIPHERAL,
};

static struct clk *periph_clocks[] __initdata = {
	&pioA_clk,
	&pioB_clk,
	&pioC_clk,
	&adc_clk,
	&usart0_clk,
	&usart1_clk,
	&usart2_clk,
	&mmc_clk,
	&udc_clk,
	&twi_clk,
	&spi0_clk,
	&spi1_clk,
	// ssc
	// tc0 .. tc2
	&ohci_clk,
	&ether_clk,
	&isi_clk,
	&usart3_clk,
	&usart4_clk,
	&usart5_clk,
	// tc3 .. tc5
	// irq0 .. irq2
};

/*
 * The two programmable clocks.
 * You must configure pin multiplexing to bring these signals out.
 */
static struct clk pck0 = {
	.name		= "pck0",
	.pmc_mask	= AT91_PMC_PCK0,
	.type		= CLK_TYPE_PROGRAMMABLE,
	.id		= 0,
};
static struct clk pck1 = {
	.name		= "pck1",
	.pmc_mask	= AT91_PMC_PCK1,
	.type		= CLK_TYPE_PROGRAMMABLE,
	.id		= 1,
};

static void __init at91sam9260_register_clocks(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(periph_clocks); i++)
		clk_register(periph_clocks[i]);

	clk_register(&pck0);
	clk_register(&pck1);
}

/* --------------------------------------------------------------------
 *  GPIO
 * -------------------------------------------------------------------- */

static struct at91_gpio_bank at91sam9260_gpio[] = {
	{
		.id		= AT91SAM9260_ID_PIOA,
		.offset		= AT91_PIOA,
		.clock		= &pioA_clk,
	}, {
		.id		= AT91SAM9260_ID_PIOB,
		.offset		= AT91_PIOB,
		.clock		= &pioB_clk,
	}, {
		.id		= AT91SAM9260_ID_PIOC,
		.offset		= AT91_PIOC,
		.clock		= &pioC_clk,
	}
};

static void at91sam9260_reset(void)
{
	at91_sys_write(AT91_RSTC_CR, (0xA5 << 24) | AT91_RSTC_PROCRST | AT91_RSTC_PERRST);
}


/* --------------------------------------------------------------------
 *  AT91SAM9260 processor initialization
 * -------------------------------------------------------------------- */

void __init at91sam9260_initialize(unsigned long main_clock)
{
	/* Map peripherals */
	iotable_init(at91sam9260_io_desc, ARRAY_SIZE(at91sam9260_io_desc));

	at91_arch_reset = at91sam9260_reset;
	at91_extern_irq = (1 << AT91SAM9260_ID_IRQ0) | (1 << AT91SAM9260_ID_IRQ1)
			| (1 << AT91SAM9260_ID_IRQ2);

	/* Init clock subsystem */
	at91_clock_init(main_clock);

	/* Register the processor-specific clocks */
	at91sam9260_register_clocks();

	/* Register GPIO subsystem */
	at91_gpio_init(at91sam9260_gpio, 3);
}

/* --------------------------------------------------------------------
 *  Interrupt initialization
 * -------------------------------------------------------------------- */

/*
 * The default interrupt priority levels (0 = lowest, 7 = highest).
 */
static unsigned int at91sam9260_default_irq_priority[NR_AIC_IRQS] __initdata = {
	7,	/* Advanced Interrupt Controller */
	7,	/* System Peripherals */
	0,	/* Parallel IO Controller A */
	0,	/* Parallel IO Controller B */
	0,	/* Parallel IO Controller C */
	0,	/* Analog-to-Digital Converter */
	6,	/* USART 0 */
	6,	/* USART 1 */
	6,	/* USART 2 */
	0,	/* Multimedia Card Interface */
	4,	/* USB Device Port */
	0,	/* Two-Wire Interface */
	6,	/* Serial Peripheral Interface 0 */
	6,	/* Serial Peripheral Interface 1 */
	5,	/* Serial Synchronous Controller */
	0,
	0,
	0,	/* Timer Counter 0 */
	0,	/* Timer Counter 1 */
	0,	/* Timer Counter 2 */
	3,	/* USB Host port */
	3,	/* Ethernet */
	0,	/* Image Sensor Interface */
	6,	/* USART 3 */
	6,	/* USART 4 */
	6,	/* USART 5 */
	0,	/* Timer Counter 3 */
	0,	/* Timer Counter 4 */
	0,	/* Timer Counter 5 */
	0,	/* Advanced Interrupt Controller */
	0,	/* Advanced Interrupt Controller */
	0,	/* Advanced Interrupt Controller */
};

void __init at91sam9260_init_interrupts(unsigned int priority[NR_AIC_IRQS])
{
	if (!priority)
		priority = at91sam9260_default_irq_priority;

	/* Initialize the AIC interrupt controller */
	at91_aic_init(priority);

	/* Enable GPIO interrupts */
	at91_gpio_irq_setup();
}
