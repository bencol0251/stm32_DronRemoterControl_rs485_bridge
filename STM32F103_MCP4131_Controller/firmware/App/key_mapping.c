#include "key_mapping.h"
#include "app_config.h"

uint8_t KeyMap_U1(uint8_t key)
{
    switch (key)
    {
    case 1u: return 51u;
    case 2u: return 205u;
    default: return MCP4131_INITIAL_U1;
    }
}

uint8_t KeyMap_U2(uint8_t key)
{
    switch (key)
    {
    case 3u: return 51u;
    case 4u: return 205u;
    default: return MCP4131_INITIAL_U2;
    }
}

uint8_t KeyMap_U3(uint8_t key)
{
    switch (key)
    {
    case 5u: return 51u;
    case 6u: return 205u;
    default: return MCP4131_INITIAL_U3;
    }
}

uint8_t KeyMap_U4(uint8_t key)
{
    switch (key)
    {
    case 7u: return 51u;
    case 8u: return 205u;
    default: return MCP4131_INITIAL_U4;
    }
}

uint8_t KeyMap_U5(uint8_t key)
{
    switch (key)
    {
    case 32u: return 0u;
    case 33u: return 23u;
    case 34u: return 42u;
    case 35u: return 59u;
    case 36u: return 73u;
    case 37u: return 85u;
    case 38u: return 96u;
    case 39u: return 158u;
    case 40u: return 185u;
    case 41u: return 200u;
    case 42u: return 210u;
    case 43u: return 217u;
    default: return MCP4131_INITIAL_U5;
    }
}
