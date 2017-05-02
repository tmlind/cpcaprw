/*
 * Read and write CPCAP PMIC registers on Android devices
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <asm/errno.h>
#include <sys/ioctl.h>

/*
 * Copied from Motorola mapphone Linux kernel include/linux/spi/cpcap.h
 */
enum cpcap_reg {
	CPCAP_REG_START,        /* Start of CPCAP registers. */

	CPCAP_REG_INT1 = CPCAP_REG_START, /* Interrupt 1 */
	CPCAP_REG_INT2,		/* Interrupt 2 */
	CPCAP_REG_INT3,		/* Interrupt 3 */
	CPCAP_REG_INT4,		/* Interrupt 4 */
	CPCAP_REG_INTM1,	/* Interrupt Mask 1 */
	CPCAP_REG_INTM2,	/* Interrupt Mask 2 */
	CPCAP_REG_INTM3,	/* Interrupt Mask 3 */
	CPCAP_REG_INTM4,	/* Interrupt Mask 4 */
	CPCAP_REG_INTS1,	/* Interrupt Sense 1 */
	CPCAP_REG_INTS2,	/* Interrupt Sense 2 */
	CPCAP_REG_INTS3,	/* Interrupt Sense 3 */
	CPCAP_REG_INTS4,	/* Interrupt Sense 4 */
	CPCAP_REG_ASSIGN1,	/* Resource Assignment 1 */
	CPCAP_REG_ASSIGN2,	/* Resource Assignment 2 */
	CPCAP_REG_ASSIGN3,	/* Resource Assignment 3 */
	CPCAP_REG_ASSIGN4,	/* Resource Assignment 4 */
	CPCAP_REG_ASSIGN5,	/* Resource Assignment 5 */
	CPCAP_REG_ASSIGN6,	/* Resource Assignment 6 */
	CPCAP_REG_VERSC1,	/* Version Control 1 */
	CPCAP_REG_VERSC2,	/* Version Control 2 */

	CPCAP_REG_MI1,		/* Macro Interrupt 1 */
	CPCAP_REG_MIM1,		/* Macro Interrupt Mask 1 */
	CPCAP_REG_MI2,		/* Macro Interrupt 2 */
	CPCAP_REG_MIM2,		/* Macro Interrupt Mask 2 */
	CPCAP_REG_UCC1,		/* UC Control 1 */
	CPCAP_REG_UCC2,		/* UC Control 2 */
	CPCAP_REG_PC1,		/* Power Cut 1 */
	CPCAP_REG_PC2,		/* Power Cut 2 */
	CPCAP_REG_BPEOL,	/* BP and EOL */
	CPCAP_REG_PGC,		/* Power Gate and Control */
	CPCAP_REG_MT1,		/* Memory Transfer 1 */
	CPCAP_REG_MT2,		/* Memory Transfer 2 */
	CPCAP_REG_MT3,		/* Memory Transfer 3 */
	CPCAP_REG_PF,		/* Print Format */

	CPCAP_REG_SCC,		/* System Clock Control */
	CPCAP_REG_SW1,		/* Stop Watch 1 */
	CPCAP_REG_SW2,		/* Stop Watch 2 */
	CPCAP_REG_UCTM,		/* UC Turbo Mode */
	CPCAP_REG_TOD1,		/* Time of Day 1 */
	CPCAP_REG_TOD2,		/* Time of Day 2 */
	CPCAP_REG_TODA1,	/* Time of Day Alarm 1 */
	CPCAP_REG_TODA2,	/* Time of Day Alarm 2 */
	CPCAP_REG_DAY,		/* Day */
	CPCAP_REG_DAYA,		/* Day Alarm */
	CPCAP_REG_VAL1,		/* Validity 1 */
	CPCAP_REG_VAL2,		/* Validity 2 */

	CPCAP_REG_SDVSPLL,	/* Switcher DVS and PLL */
	CPCAP_REG_SI2CC1,	/* Switcher I2C Control 1 */
	CPCAP_REG_Si2CC2,	/* Switcher I2C Control 2 */
	CPCAP_REG_S1C1,	        /* Switcher 1 Control 1 */
	CPCAP_REG_S1C2,	        /* Switcher 1 Control 2 */
	CPCAP_REG_S2C1,	        /* Switcher 2 Control 1 */
	CPCAP_REG_S2C2,	        /* Switcher 2 Control 2 */
	CPCAP_REG_S3C,	        /* Switcher 3 Control */
	CPCAP_REG_S4C1,	        /* Switcher 4 Control 1 */
	CPCAP_REG_S4C2,	        /* Switcher 4 Control 2 */
	CPCAP_REG_S5C,	        /* Switcher 5 Control */
	CPCAP_REG_S6C,	        /* Switcher 6 Control */
	CPCAP_REG_VCAMC,	/* VCAM Control */
	CPCAP_REG_VCSIC,	/* VCSI Control */
	CPCAP_REG_VDACC,	/* VDAC Control */
	CPCAP_REG_VDIGC,	/* VDIG Control */
	CPCAP_REG_VFUSEC,	/* VFUSE Control */
	CPCAP_REG_VHVIOC,	/* VHVIO Control */
	CPCAP_REG_VSDIOC,	/* VSDIO Control */
	CPCAP_REG_VPLLC,	/* VPLL Control */
	CPCAP_REG_VRF1C,	/* VRF1 Control */
	CPCAP_REG_VRF2C,	/* VRF2 Control */
	CPCAP_REG_VRFREFC,	/* VRFREF Control */
	CPCAP_REG_VWLAN1C,	/* VWLAN1 Control */
	CPCAP_REG_VWLAN2C,	/* VWLAN2 Control */
	CPCAP_REG_VSIMC,	/* VSIM Control */
	CPCAP_REG_VVIBC,	/* VVIB Control */
	CPCAP_REG_VUSBC,	/* VUSB Control */
	CPCAP_REG_VUSBINT1C,	/* VUSBINT1 Control */
	CPCAP_REG_VUSBINT2C,	/* VUSBINT2 Control */
	CPCAP_REG_URT,		/* Useroff Regulator Trigger */
	CPCAP_REG_URM1,		/* Useroff Regulator Mask 1 */
	CPCAP_REG_URM2,		/* Useroff Regulator Mask 2 */

	CPCAP_REG_VAUDIOC,	/* VAUDIO Control */
	CPCAP_REG_CC,		/* Codec Control */
	CPCAP_REG_CDI,		/* Codec Digital Interface */
	CPCAP_REG_SDAC,		/* Stereo DAC */
	CPCAP_REG_SDACDI,	/* Stereo DAC Digital Interface */
	CPCAP_REG_TXI,		/* TX Inputs */
	CPCAP_REG_TXMP,		/* TX MIC PGA's */
	CPCAP_REG_RXOA,		/* RX Output Amplifiers */
	CPCAP_REG_RXVC,		/* RX Volume Control */
	CPCAP_REG_RXCOA,	/* RX Codec to Output Amps */
	CPCAP_REG_RXSDOA,	/* RX Stereo DAC to Output Amps */
	CPCAP_REG_RXEPOA,	/* RX External PGA to Output Amps */
	CPCAP_REG_RXLL,		/* RX Low Latency */
	CPCAP_REG_A2LA,		/* A2 Loudspeaker Amplifier */
	CPCAP_REG_MIPIS1,	/* MIPI Slimbus 1 */
	CPCAP_REG_MIPIS2,	/* MIPI Slimbus 2 */
	CPCAP_REG_MIPIS3,	/* MIPI Slimbus 3. */
	CPCAP_REG_LVAB,		/* LMR Volume and A4 Balanced. */

	CPCAP_REG_CCC1,		/* Coulomb Counter Control 1 */
	CPCAP_REG_CRM,		/* Charger and Reverse Mode */
	CPCAP_REG_CCCC2,	/* Coincell and Coulomb Ctr Ctrl 2 */
	CPCAP_REG_CCS1,		/* Coulomb Counter Sample 1 */
	CPCAP_REG_CCS2,		/* Coulomb Counter Sample 2 */
	CPCAP_REG_CCA1,		/* Coulomb Counter Accumulator 1 */
	CPCAP_REG_CCA2,		/* Coulomb Counter Accumulator 2 */
	CPCAP_REG_CCM,		/* Coulomb Counter Mode */
	CPCAP_REG_CCO,		/* Coulomb Counter Offset */
	CPCAP_REG_CCI,		/* Coulomb Counter Integrator */

	CPCAP_REG_ADCC1,	/* A/D Converter Configuration 1 */
	CPCAP_REG_ADCC2,	/* A/D Converter Configuration 2 */
	CPCAP_REG_ADCD0,	/* A/D Converter Data 0 */
	CPCAP_REG_ADCD1,	/* A/D Converter Data 1 */
	CPCAP_REG_ADCD2,	/* A/D Converter Data 2 */
	CPCAP_REG_ADCD3,	/* A/D Converter Data 3 */
	CPCAP_REG_ADCD4,	/* A/D Converter Data 4 */
	CPCAP_REG_ADCD5,	/* A/D Converter Data 5 */
	CPCAP_REG_ADCD6,	/* A/D Converter Data 6 */
	CPCAP_REG_ADCD7,	/* A/D Converter Data 7 */
	CPCAP_REG_ADCAL1,	/* A/D Converter Calibration 1 */
	CPCAP_REG_ADCAL2,	/* A/D Converter Calibration 2 */

	CPCAP_REG_USBC1,	/* USB Control 1 */
	CPCAP_REG_USBC2,	/* USB Control 2 */
	CPCAP_REG_USBC3,	/* USB Control 3 */
	CPCAP_REG_UVIDL,	/* ULPI Vendor ID Low */
	CPCAP_REG_UVIDH,	/* ULPI Vendor ID High */
	CPCAP_REG_UPIDL,	/* ULPI Product ID Low */
	CPCAP_REG_UPIDH,	/* ULPI Product ID High */
	CPCAP_REG_UFC1,		/* ULPI Function Control 1 */
	CPCAP_REG_UFC2,		/* ULPI Function Control 2 */
	CPCAP_REG_UFC3,		/* ULPI Function Control 3 */
	CPCAP_REG_UIC1,		/* ULPI Interface Control 1 */
	CPCAP_REG_UIC2,		/* ULPI Interface Control 2 */
	CPCAP_REG_UIC3,		/* ULPI Interface Control 3 */
	CPCAP_REG_USBOTG1,	/* USB OTG Control 1 */
	CPCAP_REG_USBOTG2,	/* USB OTG Control 2 */
	CPCAP_REG_USBOTG3,	/* USB OTG Control 3 */
	CPCAP_REG_UIER1,	/* USB Interrupt Enable Rising 1 */
	CPCAP_REG_UIER2,	/* USB Interrupt Enable Rising 2 */
	CPCAP_REG_UIER3,	/* USB Interrupt Enable Rising 3 */
	CPCAP_REG_UIEF1,	/* USB Interrupt Enable Falling 1 */
	CPCAP_REG_UIEF2,	/* USB Interrupt Enable Falling 1 */
	CPCAP_REG_UIEF3,	/* USB Interrupt Enable Falling 1 */
	CPCAP_REG_UIS,		/* USB Interrupt Status */
	CPCAP_REG_UIL,		/* USB Interrupt Latch */
	CPCAP_REG_USBD,		/* USB Debug */
	CPCAP_REG_SCR1,		/* Scratch 1 */
	CPCAP_REG_SCR2,		/* Scratch 2 */
	CPCAP_REG_SCR3,		/* Scratch 3 */
	CPCAP_REG_VMC,		/* Video Mux Control */
	CPCAP_REG_OWDC,		/* One Wire Device Control */
	CPCAP_REG_GPIO0,	/* GPIO 0 Control */
	CPCAP_REG_GPIO1,	/* GPIO 1 Control */
	CPCAP_REG_GPIO2,	/* GPIO 2 Control */
	CPCAP_REG_GPIO3,	/* GPIO 3 Control */
	CPCAP_REG_GPIO4,	/* GPIO 4 Control */
	CPCAP_REG_GPIO5,	/* GPIO 5 Control */
	CPCAP_REG_GPIO6,	/* GPIO 6 Control */

	CPCAP_REG_MDLC,		/* Main Display Lighting Control */
	CPCAP_REG_KLC,		/* Keypad Lighting Control */
	CPCAP_REG_ADLC,		/* Aux Display Lighting Control */
	CPCAP_REG_REDC,		/* Red Triode Control */
	CPCAP_REG_GREENC,	/* Green Triode Control */
	CPCAP_REG_BLUEC,	/* Blue Triode Control */
	CPCAP_REG_CFC,		/* Camera Flash Control */
	CPCAP_REG_ABC,		/* Adaptive Boost Control */
	CPCAP_REG_BLEDC,	/* Bluetooth LED Control */
	CPCAP_REG_CLEDC,	/* Camera Privacy LED Control */

	CPCAP_REG_OW1C,		/* One Wire 1 Command */
	CPCAP_REG_OW1D,		/* One Wire 1 Data */
	CPCAP_REG_OW1I,		/* One Wire 1 Interrupt */
	CPCAP_REG_OW1IE,	/* One Wire 1 Interrupt Enable */
	CPCAP_REG_OW1,		/* One Wire 1 Control */
	CPCAP_REG_OW2C,		/* One Wire 2 Command */
	CPCAP_REG_OW2D,		/* One Wire 2 Data */
	CPCAP_REG_OW2I,		/* One Wire 2 Interrupt */
	CPCAP_REG_OW2IE,	/* One Wire 2 Interrupt Enable */
	CPCAP_REG_OW2,		/* One Wire 2 Control */
	CPCAP_REG_OW3C,		/* One Wire 3 Command */
	CPCAP_REG_OW3D,		/* One Wire 3 Data */
	CPCAP_REG_OW3I,		/* One Wire 3 Interrupt */
	CPCAP_REG_OW3IE,	/* One Wire 3 Interrupt Enable */
	CPCAP_REG_OW3,		/* One Wire 3 Control */
	CPCAP_REG_GCAIC,	/* GCAI Clock Control */
	CPCAP_REG_GCAIM,	/* GCAI GPIO Mode */
	CPCAP_REG_LGDIR,	/* LMR GCAI GPIO Direction */
	CPCAP_REG_LGPU,		/* LMR GCAI GPIO Pull-up */
	CPCAP_REG_LGPIN,	/* LMR GCAI GPIO Pin */
	CPCAP_REG_LGMASK,	/* LMR GCAI GPIO Mask */
	CPCAP_REG_LDEB,		/* LMR Debounce Settings */
	CPCAP_REG_LGDET,	/* LMR GCAI Detach Detect */
	CPCAP_REG_LMISC,	/* LMR Misc Bits */
	CPCAP_REG_LMACE,	/* LMR Mace IC Support */
	CPCAP_REG_TEST,         /* Test */
	CPCAP_REG_ST_TEST1,     /* ST Test1 */
	CPCAP_REG_ST_TEST2,     /* ST Test2 */
	CPCAP_REG_END = CPCAP_REG_ST_TEST2, /* End of CPCAP registers. */

	CPCAP_REG_MAX		/* The largest valid register value. */
	= CPCAP_REG_END,

	CPCAP_REG_SIZE = CPCAP_REG_MAX + 1,
	CPCAP_REG_UNUSED = CPCAP_REG_MAX + 2,
};

#define CPCAP_NUM_REG_CPCAP (CPCAP_REG_END - CPCAP_REG_START + 1)
#define CONFIG_EMU_UART_DEBUG

/*
 * Copied from drivers/mfd/cpcap-regacc.c in Motorola mapphone Linux
 * kernel tree
 */
static const struct {
	unsigned short address;         /* Address of the register */
	unsigned short constant_mask;	/* Constant modifiability mask */
	unsigned short rbw_mask;	/* Read-before-write mask */
} register_info_tbl[CPCAP_NUM_REG_CPCAP] = {
	[CPCAP_REG_INT1]      = {0, 0x0004, 0x0000},
	[CPCAP_REG_INT2]      = {1, 0x0000, 0x0000},
	[CPCAP_REG_INT3]      = {2, 0x0000, 0x0000},
	[CPCAP_REG_INT4]      = {3, 0xFC00, 0x0000},
	[CPCAP_REG_INTM1]     = {4, 0x0004, 0xFFFF},
	[CPCAP_REG_INTM2]     = {5, 0x0000, 0xFFFF},
	[CPCAP_REG_INTM3]     = {6, 0x0000, 0xFFFF},
	[CPCAP_REG_INTM4]     = {7, 0xFC00, 0xFFFF},
	[CPCAP_REG_INTS1]     = {8, 0xFFFF, 0xFFFF},
	[CPCAP_REG_INTS2]     = {9, 0xFFFF, 0xFFFF},
	[CPCAP_REG_INTS3]     = {10, 0xFFFF, 0xFFFF},
	[CPCAP_REG_INTS4]     = {11, 0xFFFF, 0xFFFF},
	[CPCAP_REG_ASSIGN1]   = {12, 0x80F8, 0xFFFF},
	[CPCAP_REG_ASSIGN2]   = {13, 0x0000, 0xFFFF},
	[CPCAP_REG_ASSIGN3]   = {14, 0x0004, 0xFFFF},
	[CPCAP_REG_ASSIGN4]   = {15, 0x0068, 0xFFFF},
	[CPCAP_REG_ASSIGN5]   = {16, 0x0000, 0xFFFF},
	[CPCAP_REG_ASSIGN6]   = {17, 0xFC00, 0xFFFF},
	[CPCAP_REG_VERSC1]    = {18, 0xFFFF, 0xFFFF},
	[CPCAP_REG_VERSC2]    = {19, 0xFFFF, 0xFFFF},
	[CPCAP_REG_MI1]       = {128, 0x0000, 0x0000},
	[CPCAP_REG_MIM1]      = {129, 0x0000, 0xFFFF},
	[CPCAP_REG_MI2]       = {130, 0x0000, 0xFFFF},
	[CPCAP_REG_MIM2]      = {131, 0xFFFF, 0xFFFF},
	[CPCAP_REG_UCC1]      = {132, 0xF000, 0xFFFF},
	[CPCAP_REG_UCC2]      = {133, 0xFC00, 0xFFFF},
	[CPCAP_REG_PC1]       = {135, 0xFC00, 0xFFFF},
	[CPCAP_REG_PC2]       = {136, 0xFC00, 0xFFFF},
	[CPCAP_REG_BPEOL]     = {137, 0xFE00, 0xFFFF},
	[CPCAP_REG_PGC]       = {138, 0xFE00, 0xFFFF},
	[CPCAP_REG_MT1]       = {139, 0x0000, 0x0000},
	[CPCAP_REG_MT2]       = {140, 0x0000, 0x0000},
	[CPCAP_REG_MT3]       = {141, 0x0000, 0x0000},
	[CPCAP_REG_PF]        = {142, 0x0000, 0xFFFF},
	[CPCAP_REG_SCC]       = {256, 0xFF00, 0xFFFF},
	[CPCAP_REG_SW1]       = {257, 0xFFFF, 0xFFFF},
	[CPCAP_REG_SW2]       = {258, 0xFC7F, 0xFFFF},
	[CPCAP_REG_UCTM]      = {259, 0xFFFE, 0xFFFF},
	[CPCAP_REG_TOD1]      = {260, 0xFF00, 0xFFFF},
	[CPCAP_REG_TOD2]      = {261, 0xFE00, 0xFFFF},
	[CPCAP_REG_TODA1]     = {262, 0xFF00, 0xFFFF},
	[CPCAP_REG_TODA2]     = {263, 0xFE00, 0xFFFF},
	[CPCAP_REG_DAY]       = {264, 0x8000, 0xFFFF},
	[CPCAP_REG_DAYA]      = {265, 0x8000, 0xFFFF},
	[CPCAP_REG_VAL1]      = {266, 0x0000, 0xFFFF},
	[CPCAP_REG_VAL2]      = {267, 0x0000, 0xFFFF},
	[CPCAP_REG_SDVSPLL]   = {384, 0x2488, 0xFFFF},
	[CPCAP_REG_SI2CC1]    = {385, 0x8000, 0xFFFF},
	[CPCAP_REG_Si2CC2]    = {386, 0xFF00, 0xFFFF},
	[CPCAP_REG_S1C1]      = {387, 0x9080, 0xFFFF},
	[CPCAP_REG_S1C2]      = {388, 0x8080, 0xFFFF},
	[CPCAP_REG_S2C1]      = {389, 0x9080, 0xFFFF},
	[CPCAP_REG_S2C2]      = {390, 0x8080, 0xFFFF},
	[CPCAP_REG_S3C]       = {391, 0xFA84, 0xFFFF},
	[CPCAP_REG_S4C1]      = {392, 0x9080, 0xFFFF},
	[CPCAP_REG_S4C2]      = {393, 0x8080, 0xFFFF},
	[CPCAP_REG_S5C]       = {394, 0xFFD7, 0xFFFF},
	[CPCAP_REG_S6C]       = {395, 0xFFF4, 0xFFFF},
	[CPCAP_REG_VCAMC]     = {396, 0xFF48, 0xFFFF},
	[CPCAP_REG_VCSIC]     = {397, 0xFFA8, 0xFFFF},
	[CPCAP_REG_VDACC]     = {398, 0xFF48, 0xFFFF},
	[CPCAP_REG_VDIGC]     = {399, 0xFF48, 0xFFFF},
	[CPCAP_REG_VFUSEC]    = {400, 0xFF50, 0xFFFF},
	[CPCAP_REG_VHVIOC]    = {401, 0xFFE8, 0xFFFF},
	[CPCAP_REG_VSDIOC]    = {402, 0xFF40, 0xFFFF},
	[CPCAP_REG_VPLLC]     = {403, 0xFFA4, 0xFFFF},
	[CPCAP_REG_VRF1C]     = {404, 0xFF50, 0xFFFF},
	[CPCAP_REG_VRF2C]     = {405, 0xFFD4, 0xFFFF},
	[CPCAP_REG_VRFREFC]   = {406, 0xFFD4, 0xFFFF},
	[CPCAP_REG_VWLAN1C]   = {407, 0xFFA8, 0xFFFF},
	[CPCAP_REG_VWLAN2C]   = {408, 0xFD32, 0xFFFF},
	[CPCAP_REG_VSIMC]     = {409, 0xE154, 0xFFFF},
	[CPCAP_REG_VVIBC]     = {410, 0xFFF2, 0xFFFF},
#ifdef CONFIG_EMU_UART_DEBUG
	[CPCAP_REG_VUSBC]     = {411, 0xFFFF, 0xFFFF},
#else
	[CPCAP_REG_VUSBC]     = {411, 0xFEA2, 0xFFFF},
#endif
	[CPCAP_REG_VUSBINT1C] = {412, 0xFFD4, 0xFFFF},
	[CPCAP_REG_VUSBINT2C] = {413, 0xFFD4, 0xFFFF},
	[CPCAP_REG_URT]       = {414, 0xFFFE, 0xFFFF},
	[CPCAP_REG_URM1]      = {415, 0x0000, 0xFFFF},
	[CPCAP_REG_URM2]      = {416, 0xFC00, 0xFFFF},
	[CPCAP_REG_VAUDIOC]   = {512, 0xFF88, 0xFFFF},
	[CPCAP_REG_CC]        = {513, 0x0000, 0xFEDF},
	[CPCAP_REG_CDI]       = {514, 0x4000, 0xFFFF},
	[CPCAP_REG_SDAC]      = {515, 0xF000, 0xFCFF},
	[CPCAP_REG_SDACDI]    = {516, 0xC000, 0xFFFF},
	[CPCAP_REG_TXI]       = {517, 0x0000, 0xFFFF},
	[CPCAP_REG_TXMP]      = {518, 0xF000, 0xFFFF},
	[CPCAP_REG_RXOA]      = {519, 0xF800, 0xFFFF},
	[CPCAP_REG_RXVC]      = {520, 0x00C3, 0xFFFF},
	[CPCAP_REG_RXCOA]     = {521, 0xF800, 0xFFFF},
	[CPCAP_REG_RXSDOA]    = {522, 0xE000, 0xFFFF},
	[CPCAP_REG_RXEPOA]    = {523, 0x8000, 0xFFFF},
	[CPCAP_REG_RXLL]      = {524, 0x0000, 0xFFFF},
	[CPCAP_REG_A2LA]      = {525, 0xFF00, 0xFFFF},
	[CPCAP_REG_MIPIS1]    = {526, 0x0000, 0xFFFF},
	[CPCAP_REG_MIPIS2]    = {527, 0xFF00, 0xFFFF},
	[CPCAP_REG_MIPIS3]    = {528, 0xFFFC, 0xFFFF},
	[CPCAP_REG_LVAB]      = {529, 0xFFFC, 0xFFFF},
	[CPCAP_REG_CCC1]      = {640, 0xFFF0, 0xFFFF},
	[CPCAP_REG_CRM]       = {641, 0xC000, 0xFFFF},
	[CPCAP_REG_CCCC2]     = {642, 0xFFC0, 0xFFFF},
	[CPCAP_REG_CCS1]      = {643, 0x0000, 0xFFFF},
	[CPCAP_REG_CCS2]      = {644, 0xFF00, 0xFFFF},
	[CPCAP_REG_CCA1]      = {645, 0x0000, 0xFFFF},
	[CPCAP_REG_CCA2]      = {646, 0x0000, 0xFFFF},
	[CPCAP_REG_CCM]       = {647, 0xFC00, 0xFFFF},
	[CPCAP_REG_CCO]       = {648, 0xFC00, 0xFFFF},
	[CPCAP_REG_CCI]       = {649, 0xC000, 0xFFFF},
	[CPCAP_REG_ADCC1]     = {768, 0x0000, 0xFFFF},
	[CPCAP_REG_ADCC2]     = {769, 0x0080, 0xFFFF},
	[CPCAP_REG_ADCD0]     = {770, 0xFFFF, 0xFFFF},
	[CPCAP_REG_ADCD1]     = {771, 0xFFFF, 0xFFFF},
	[CPCAP_REG_ADCD2]     = {772, 0xFFFF, 0xFFFF},
	[CPCAP_REG_ADCD3]     = {773, 0xFFFF, 0xFFFF},
	[CPCAP_REG_ADCD4]     = {774, 0xFFFF, 0xFFFF},
	[CPCAP_REG_ADCD5]     = {775, 0xFFFF, 0xFFFF},
	[CPCAP_REG_ADCD6]     = {776, 0xFFFF, 0xFFFF},
	[CPCAP_REG_ADCD7]     = {777, 0xFFFF, 0xFFFF},
	[CPCAP_REG_ADCAL1]    = {778, 0xFFFF, 0xFFFF},
	[CPCAP_REG_ADCAL2]    = {779, 0xFFFF, 0xFFFF},
	[CPCAP_REG_USBC1]     = {896, 0x0000, 0xFFFF},
#ifdef CONFIG_EMU_UART_DEBUG
	[CPCAP_REG_USBC2]     = {897, 0x0F07, 0xFFFF},
#else
	[CPCAP_REG_USBC2]     = {897, 0x0000, 0xFFFF},
#endif
	[CPCAP_REG_USBC3]     = {898, 0x8200, 0xFFFF},
	[CPCAP_REG_UVIDL]     = {899, 0xFFFF, 0xFFFF},
	[CPCAP_REG_UVIDH]     = {900, 0xFFFF, 0xFFFF},
	[CPCAP_REG_UPIDL]     = {901, 0xFFFF, 0xFFFF},
	[CPCAP_REG_UPIDH]     = {902, 0xFFFF, 0xFFFF},
	[CPCAP_REG_UFC1]      = {903, 0xFF80, 0xFFFF},
	[CPCAP_REG_UFC2]      = {904, 0xFF80, 0xFFFF},
	[CPCAP_REG_UFC3]      = {905, 0xFF80, 0xFFFF},
	[CPCAP_REG_UIC1]      = {906, 0xFF64, 0xFFFF},
	[CPCAP_REG_UIC2]      = {907, 0xFF64, 0xFFFF},
	[CPCAP_REG_UIC3]      = {908, 0xFF64, 0xFFFF},
	[CPCAP_REG_USBOTG1]   = {909, 0xFFC0, 0xFFFF},
	[CPCAP_REG_USBOTG2]   = {910, 0xFFC0, 0xFFFF},
	[CPCAP_REG_USBOTG3]   = {911, 0xFFC0, 0xFFFF},
	[CPCAP_REG_UIER1]     = {912, 0xFFE0, 0xFFFF},
	[CPCAP_REG_UIER2]     = {913, 0xFFE0, 0xFFFF},
	[CPCAP_REG_UIER3]     = {914, 0xFFE0, 0xFFFF},
	[CPCAP_REG_UIEF1]     = {915, 0xFFE0, 0xFFFF},
	[CPCAP_REG_UIEF2]     = {916, 0xFFE0, 0xFFFF},
	[CPCAP_REG_UIEF3]     = {917, 0xFFE0, 0xFFFF},
	[CPCAP_REG_UIS]       = {918, 0xFFFF, 0xFFFF},
	[CPCAP_REG_UIL]       = {919, 0xFFFF, 0xFFFF},
	[CPCAP_REG_USBD]      = {920, 0xFFFF, 0xFFFF},
	[CPCAP_REG_SCR1]      = {921, 0xFF00, 0xFFFF},
	[CPCAP_REG_SCR2]      = {922, 0xFF00, 0xFFFF},
	[CPCAP_REG_SCR3]      = {923, 0xFF00, 0xFFFF},
	[CPCAP_REG_VMC]       = {939, 0xFFFE, 0xFFFF},
	[CPCAP_REG_OWDC]      = {940, 0xFFFC, 0xFFFF},
	[CPCAP_REG_GPIO0]     = {941, 0x0D11, 0x3FFF},
	[CPCAP_REG_GPIO1]     = {943, 0x0D11, 0x3FFF},
	[CPCAP_REG_GPIO2]     = {945, 0x0D11, 0x3FFF},
	[CPCAP_REG_GPIO3]     = {947, 0x0D11, 0x3FFF},
	[CPCAP_REG_GPIO4]     = {949, 0x0D11, 0x3FFF},
	[CPCAP_REG_GPIO5]     = {951, 0x0C11, 0x3FFF},
	[CPCAP_REG_GPIO6]     = {953, 0x0C11, 0x3FFF},
	[CPCAP_REG_MDLC]      = {1024, 0x0000, 0xFFFF},
	[CPCAP_REG_KLC]       = {1025, 0x8000, 0xFFFF},
	[CPCAP_REG_ADLC]      = {1026, 0x8000, 0xFFFF},
	[CPCAP_REG_REDC]      = {1027, 0xFC00, 0xFFFF},
	[CPCAP_REG_GREENC]    = {1028, 0xFC00, 0xFFFF},
	[CPCAP_REG_BLUEC]     = {1029, 0xFC00, 0xFFFF},
	[CPCAP_REG_CFC]       = {1030, 0xF000, 0xFFFF},
	[CPCAP_REG_ABC]       = {1031, 0xFFC3, 0xFFFF},
	[CPCAP_REG_BLEDC]     = {1032, 0xFC00, 0xFFFF},
	[CPCAP_REG_CLEDC]     = {1033, 0xFC00, 0xFFFF},
	[CPCAP_REG_OW1C]      = {1152, 0xFF00, 0xFFFF},
	[CPCAP_REG_OW1D]      = {1153, 0xFF00, 0xFFFF},
	[CPCAP_REG_OW1I]      = {1154, 0xFFFF, 0xFFFF},
	[CPCAP_REG_OW1IE]     = {1155, 0xFF00, 0xFFFF},
	[CPCAP_REG_OW1]       = {1157, 0xFF00, 0xFFFF},
	[CPCAP_REG_OW2C]      = {1160, 0xFF00, 0xFFFF},
	[CPCAP_REG_OW2D]      = {1161, 0xFF00, 0xFFFF},
	[CPCAP_REG_OW2I]      = {1162, 0xFFFF, 0xFFFF},
	[CPCAP_REG_OW2IE]     = {1163, 0xFF00, 0xFFFF},
	[CPCAP_REG_OW2]       = {1165, 0xFF00, 0xFFFF},
	[CPCAP_REG_OW3C]      = {1168, 0xFF00, 0xFFFF},
	[CPCAP_REG_OW3D]      = {1169, 0xFF00, 0xFFFF},
	[CPCAP_REG_OW3I]      = {1170, 0xFF00, 0xFFFF},
	[CPCAP_REG_OW3IE]     = {1171, 0xFF00, 0xFFFF},
	[CPCAP_REG_OW3]       = {1173, 0xFF00, 0xFFFF},
	[CPCAP_REG_GCAIC]     = {1174, 0xFF00, 0xFFFF},
	[CPCAP_REG_GCAIM]     = {1175, 0xFF00, 0xFFFF},
	[CPCAP_REG_LGDIR]     = {1176, 0xFFE0, 0xFFFF},
	[CPCAP_REG_LGPU]      = {1177, 0xFFE0, 0xFFFF},
	[CPCAP_REG_LGPIN]     = {1178, 0xFF00, 0xFFFF},
	[CPCAP_REG_LGMASK]    = {1179, 0xFFE0, 0xFFFF},
	[CPCAP_REG_LDEB]      = {1180, 0xFF00, 0xFFFF},
	[CPCAP_REG_LGDET]     = {1181, 0xFF00, 0xFFFF},
	[CPCAP_REG_LMISC]     = {1182, 0xFF07, 0xFFFF},
	[CPCAP_REG_LMACE]     = {1183, 0xFFF8, 0xFFFF},
	[CPCAP_REG_TEST]      = {7936, 0x0000, 0xFFFF},
	[CPCAP_REG_ST_TEST1]  = {8002, 0x0000, 0xFFFF},
	[CPCAP_REG_ST_TEST2]  = {8006, 0xFFFC, 0xFFFF},
};

enum {
	CPCAP_IOCTL_NUM_TEST__START,
	CPCAP_IOCTL_NUM_TEST_READ_REG,
	CPCAP_IOCTL_NUM_TEST_WRITE_REG,
	CPCAP_IOCTL_NUM_TEST__END,
};

#define CPCAP_IOCTL_TEST_READ_REG \
	_IOWR(0, CPCAP_IOCTL_NUM_TEST_READ_REG, struct cpcap_regacc*)
#define CPCAP_IOCTL_TEST_WRITE_REG \
	_IOWR(0, CPCAP_IOCTL_NUM_TEST_WRITE_REG, struct cpcap_regacc*)

/*
 * Note that Motorola mapphone Linux kernel uses a register index
 * for reg instead of regmap address offset.
 */
struct cpcap_regacc {
	unsigned short reg;
	unsigned short value;
	unsigned short mask;
};

static int cpcap_init_regwrite(struct cpcap_regacc *reg, int offset)
{
	int i;

	reg->reg = 0;
	reg->value = 0;
	reg->mask = 0;

	for (i = 0; i < CPCAP_NUM_REG_CPCAP; i++)
		if ((register_info_tbl[i].address * 4) == offset) {
			reg->mask = register_info_tbl[i].rbw_mask;
			reg->reg = i;
			return i;
		}

	return -EINVAL;
}

/*
 * See drivers/mfd/cpcap-regacc.c in Motorola mapphone Linux kernel tree
 * for details of the ioctl handling.
 */
static int cpcap_read(int fd, int reg_offset, int *val)
{
	struct cpcap_regacc reg;
	int index, error;

	index = cpcap_init_regwrite(&reg, reg_offset);
	if (index < 0)
		return index;

	reg.value = 0;

	error = ioctl(fd, CPCAP_IOCTL_TEST_READ_REG, &reg);
	if (error < 0) {
		fprintf(stderr, "read ioctl failed: %i\n", error);

		return error;
	}

	printf("CPCAP register%i 0x%04x=0x%04x\n",
	       index, reg_offset, reg.value);

	*val = reg.value;

	return 0;
}

static int cpcap_write(int fd, int value, int reg_offset)
{
	struct cpcap_regacc reg;
	int index, error, tmp;

	index = cpcap_init_regwrite(&reg, reg_offset);
	if (index < 0)
		return index;

	reg.value = value;

	/* For write to work, see README for chcon usage */
	error = ioctl(fd, CPCAP_IOCTL_TEST_WRITE_REG, &reg);
	if (error < 0) {
		fprintf(stderr, "write ioctl failed: %i\n", error);

		return error;
	}

	error = cpcap_read(fd, reg_offset, &tmp);
	if (error < 0)
		return error;

	return 0;
}

int main(int argc, char *argv[])
{
	const char *file_name = "/dev/cpcap";
	const char delimiters[] = "=";
	char *running, *token;
	int offset, val = -1;
	int error, fd;

	if (argc < 2) {
		printf("usage: %s offset[=value]\n", argv[0]);
		printf("\nNote that offsets are not contiguous.\n");
		return -EINVAL;
	}

	running = malloc(strlen(argv[1] + 1));
	strncpy(running, argv[1], strlen(argv[1]));

	token = strsep(&running, delimiters);
	if (token) {
		offset = strtol(token, NULL, 16);

		token = strsep(&running, delimiters);
		if (token)
			val = strtol(token, NULL, 16);
	} else {
		offset = strtol(argv[1], NULL, 16);
	}

	fd = open(file_name, O_RDWR);
	if (fd == -1) {
		fprintf(stderr, "Could not open %s: %i\n",
			file_name, fd);
		error = fd;
		return fd;
	}

	if (val < 0)
		error = cpcap_read(fd, offset, &val);
	else
		error = cpcap_write(fd, val, offset);
	if (error)
		goto close;

close:
	free(running);
	close(fd);

	return error;
}
