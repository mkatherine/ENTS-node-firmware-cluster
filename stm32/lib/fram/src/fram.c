#include "fram.h"

#include "fram_def.h"

// #define FRAM_FM24CL16B
// #define FRAM_MB85RC1MT

#if defined(FRAM_FM24CL16B) && defined(FRAM_MB85RC1MT)
#error Only one FRAM chip can be enabled
#elif defined(FRAM_FM24CL16B)
#include "fm24cl16b.h"
const FramInterfaceType FramInterface = {.WritePtr = Fm24cl16bWrite,
                                         .ReadPtr = Fm24cl16bRead,
                                         .size = fm24cl16b_size};
#elif defined(FRAM_MB85RC1MT)
#include "mb85rc1mt.h"
const FramInterfaceType FramInterface = {.WritePtr = Mb85rc1mtWrite,
                                         .ReadPtr = Mb85rc1mtRead,
                                         .size = mb85rc1mt_size};
#else
#error No FRAM chip enabled
#endif

FramStatus FramWrite(FramAddr addr, const uint8_t *data, size_t len)
{
  // check size
  if (addr + len > FramSize())
  {
    return FRAM_OUT_OF_RANGE;
  }

  return FramInterface.WritePtr(addr, data, len);
}

FramStatus FramRead(FramAddr addr, size_t len, uint8_t *data)
{
  // check size
  if (addr + len > FramSize())
  {
    return FRAM_OUT_OF_RANGE;
  }

  return FramInterface.ReadPtr(addr, len, data);
}

FramAddr FramSize(void) { return FramInterface.size; }

HAL_StatusTypeDef ConfigureSettings(configuration c)
{
  HAL_StatusTypeDef status = HAL_OK;

  // TODO(GSOC student) implement user config write

  return status;
}

configuration ReadSettings(void)
{
  configuration c;

  // TODO(GSOC student) implement user config read

  return c;
}

// only call FramDump() with input arg linesize=16
FramStatus FramDump(uint32_t linesize, uint8_t displayformat)
{
  FramStatus returnstatus = FRAM_OK;
  // DUMP FRAM
  APP_PRINTF("\r\n\r\nDumping FRAM...\r\n\r\n");
  APP_PRINTF("Lines with only junk data (i.e. 1 2 3 4 5...) are omitted. Runtime ~18 seconds.\r\n\r\n");
  if (displayformat == DUMP_FRAM_HEX)
  {
    APP_PRINTF("Memory values in HEX.\r\n\r\n");
  }
  else if (displayformat == DUMP_FRAM_DECIMAL)
  {
    APP_PRINTF("Memory values in DECIMAL.\r\n\r\n");
  }
  // print column headers, each display line is 16 bytes
  APP_PRINTF("               ");
  for (int i = 0; i < 16; i++)
  {
    if (displayformat == DUMP_FRAM_HEX)
    {
      APP_PRINTF("+%1X ", i);
    }
    else if (displayformat == DUMP_FRAM_DECIMAL)
    {
      APP_PRINTF("+%2d ", i);
    }
  }
  APP_PRINTF("\r\n");
  // FRAM chip is 1<<17 bytes long (131072).
  // linesize should be a factor of 1<<17
  uint8_t framdumpbuffer[linesize];
  FramStatus framdumpstatus;
  uint8_t printcurrentline;
  for (int line = 0; line < ((1 << 17) / linesize); line++)
  {
    framdumpstatus = FramRead(line * linesize, linesize, framdumpbuffer);
    if (framdumpstatus != FRAM_OK)
    {
      APP_PRINTF("\r\n%5X (%6d) FramRead error: %d at iter %d of %d, continuing...", line * linesize, line * linesize, framdumpstatus, line, ((1 << 17) / linesize) - 1);
      returnstatus = framdumpstatus;
    }
    else
    {
      printcurrentline = 1;
      // if the current line of FRAM consists of junk (i.e. sequential 1 2 3 4 5 1 2 3 4 5 ...) then omit
      for (int offset = 0; offset < (linesize - 5); offset++)
      {
        if (framdumpbuffer[offset] != framdumpbuffer[offset + 5])
        {
          // found non-junk data
          printcurrentline = 0;
          break;
        }
      }

      if (printcurrentline == 0)
      {
        APP_PRINTF("\r\n%5X (%6d) ", line * linesize, line * linesize);
        for (int offset = 0; offset < linesize; offset++)
        {
          if (displayformat == DUMP_FRAM_HEX)
          {
            APP_PRINTF("%02X ", framdumpbuffer[offset]);
          }
          else if (displayformat == DUMP_FRAM_DECIMAL)
          {
            APP_PRINTF("%3d ", framdumpbuffer[offset]);
          }
        }
      }
    }
  }
  APP_PRINTF("\r\n\r\nDone dumping FRAM.\r\n\r\n");

  return returnstatus;
}
