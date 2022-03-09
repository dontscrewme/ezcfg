#include <stdio.h>
#include "syscfg.h"
#include "sntp_cfg.h"

typedef struct
{
    char* path;
    ini_t* (*def)(void);
    unsigned (*size)(void);
}cfg_map_t;

cfg_map_t cfg_map[] =
{
    {SNTP_CFG_PATH, sntp_cfg_get_def, sntp_cfg_get_size},
};

int main(int argc, char** argv)
{
    unsigned i = 0;
    for(; i<COUNT_OF(cfg_map); i++)
    {
        syscfg_t* cfg = syscfg_open(cfg_map[i].path, cfg_map[i].def(), cfg_map[i].size());
        if (NULL == cfg)
        {
            continue;
        }

        syscfg_update(cfg, cfg_map[i].def(), cfg_map[i].size());
        syscfg_free(cfg);
    }

    return 0;
}
