/*
 * memoryIntCode.x -- memory configuration for TC23x
 *
 * Copyright (C) 2001 - 2014 HighTec EDV-Systeme GmbH.
 *
 */
__USTACK_SIZE = 4K;
__ISTACK_SIZE = 256;
__HEAP_MIN = 4K;
__CSA_SIZE = 4K;
__TRICORE_DERIVATE_MEMORY_MAP__ = 0x2300;
/* the external RAM description */
__EXT_CODE_RAM_BEGIN = 0;
__EXT_CODE_RAM_SIZE = 0 ;
__EXT_DATA_RAM_BEGIN = 0;
__EXT_DATA_RAM_SIZE = 0;
__RAM_END = __EXT_DATA_RAM_BEGIN + __EXT_DATA_RAM_SIZE;
/* internal FLASH description */
__INT_CODE_FLASH_BEGIN = 0x80000000;
__INT_CODE_FLASH_SIZE = 2M;
__INT_DATA_FLASH_BEGIN = 0xaf000000;
__INT_DATA_FLASH_SIZE = 128K;
/* the internal ram description */
__INT_CODE_RAM_BEGIN = 0xc0000000;
__INT_CODE_RAM_SIZE = 8K;
__INT_DATA_RAM_BEGIN = 0xd0000000;
__INT_DATA_RAM_SIZE = 184K;
/* the pcp memory description */
__PCP_CODE_RAM_BEGIN = 0;
__PCP_CODE_RAM_SIZE = 0;
__PCP_DATA_RAM_BEGIN = 0;
__PCP_DATA_RAM_SIZE = 0;
MEMORY
{
  PMU_PFLASH0 (rx!p): org = 0x80000000, len = 2M
  PMU_DFLASH0 (w!xp): org = 0xaf000000, len = 128K
  PMI_SPRAM (rx!p): org = 0xc0000000, len = 8K
  DMI_LDRAM (w!xp): org = 0xd0000000, len = 184K
  PCP_PRAM (wp!x): org = 0, len = 0
  PCP_CMEM (rpx): org = 0, len = 0
}
REGION_ALIAS("DATA_MEM", DMI_LDRAM)
REGION_ALIAS("CODE_MEM", PMI_SPRAM)
REGION_ALIAS("SDATA_MEM", DMI_LDRAM)
REGION_ALIAS("BSS_MEM", DMI_LDRAM)
REGION_ALIAS("ZDATA_MEM", DMI_LDRAM)
REGION_ALIAS("CSA_MEM", DMI_LDRAM)
REGION_ALIAS("PCP_CODE", PCP_CMEM)
REGION_ALIAS("PCP_DATA", PCP_PRAM)
/* the symbol __TRICORE_DERIVATE_NAME__ will be defined in the crt0.S and is
 * tested here to confirm that this memory map and the startup file will
 * fit together
 */
_. = ASSERT ((__TRICORE_DERIVATE_MEMORY_MAP__ == __TRICORE_DERIVATE_NAME__), "Using wrong Memory Map. This Map is for TC23XX");
INSERT BEFORE .startup
