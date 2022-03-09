#include "syscfg.h"

static ini_t def[] = 
{
    {"info", "NULL"},
    {"info:server", "andy.com"},
};

ini_t* sntp_cfg_get_def(void)
{
    return &def[0];
}

unsigned sntp_cfg_get_size(void)
{
    return COUNT_OF(def);
}
