#ifndef SNTP_CFG_H
#define SNTP_CFG_H

#define SNTP_CFG_PATH "sntp.ini"

ini_t* sntp_cfg_get_def(void);
unsigned sntp_cfg_get_size(void);

#endif