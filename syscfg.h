#ifndef SYSCFG_H
#define SYSCFG_H

typedef struct
{
	const char* entry; // section:keyword
	const char* value;
}ini_t;

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

typedef struct syscfg syscfg_t;

syscfg_t* syscfg_new(char* path, ini_t* def_cfg, unsigned size);
syscfg_t* syscfg_open(const char* path);
int syscfg_update(syscfg_t* me, ini_t* def_cfg, unsigned size);
int syscfg_duplicate(syscfg_t* me, const char* path);
int syscfg_save_entry(syscfg_t* me, const char* entry, const char* value);
int syscfg_retrieve_entry(syscfg_t* me, const char* entry, const char** value);
void syscfg_free(syscfg_t* me);


#endif