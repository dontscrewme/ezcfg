#include <stdarg.h>
#include "syscfg.h"
#include "iniparser.h"

static int default_error_callback(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    int ret = vfprintf(stderr, format, argptr);
    va_end(argptr);

    return ret;
}

static int (*syscfg_error_set)(const char*, ...) = &default_error_callback;
void syscfg_set_error_callback(int (*error_callback)(const char*, ...))
{	
    if (error_callback)
    {
	    syscfg_error_set = error_callback;
    }
}

struct syscfg
{
    const char* path;
    dictionary* dictionary;
};

static syscfg_t* syscfg_new(char* path)
{
    syscfg_t* me = malloc(sizeof(syscfg_t));
    if (NULL == me)
    {
        syscfg_error_set("%s: malloc failed\n", __FUNCTION__);
        return NULL;
    }

    me->path = path;
    me->dictionary = iniparser_load(me->path);
    if (NULL == me->dictionary)
    {
        free(me);
        return NULL;
    }

    return me;
}

syscfg_t* syscfg_open(char* path, ini_t* def, unsigned size)
{
    if (access(path, F_OK) == 0)
    {
	    return syscfg_new(path);
    }

    FILE* ini_file = fopen(path, "w");
    if (NULL == ini_file)
    {
        return NULL;
    }

    syscfg_t* me = syscfg_new(path);
    
    unsigned int i = 0;
    for (; i<size; i++)
    {		
        iniparser_set(me->dictionary, def[i].entry, def[i].value);
    }
	
    iniparser_dump_ini(me->dictionary, ini_file);
    fclose(ini_file);

    return me;
}

int syscfg_update(syscfg_t* me, ini_t* def, unsigned size)
{
    dictionary* new_me = iniparser_load(me->path);
    if (NULL == new_me)
    {
        return -1;
    }
    
    unsigned int i = 0;
    for (; i<size; i++)
    {
	    const char* value = iniparser_getstring(me->dictionary, def[i].entry, def[i].value);
	    iniparser_set(new_me, def[i].entry, value);
    }

    FILE* ini_file = fopen(me->path, "w");
    if (NULL == ini_file)
    {
        return -1;
    }

    iniparser_dump_ini(new_me, ini_file);
    fclose(ini_file);
    
    iniparser_freedict(me->dictionary);
    me->dictionary = new_me;

    return 0;
}

int syscfg_duplicate(syscfg_t* me, const char* path)
{
    FILE* new_file = fopen(path, "w");
    if (NULL == new_file)
    {
	    syscfg_error_set("%s: CANNOT create %s for write\n", __FUNCTION__, path);
	    return -1;	
    }
	
    iniparser_dump_ini(me->dictionary, new_file);
    fclose(new_file);

    return 0;
}

int syscfg_save_entry(syscfg_t* me, const char* entry, const char* value)
{
    if ((iniparser_find_entry(me->dictionary, entry) == -1)
     || (iniparser_set(me->dictionary, entry, value) == -1))
    {
        return -1;
    }

    FILE* ini_file = fopen(me->path, "w");
    if (NULL == ini_file)
    {
        syscfg_error_set("%s: CANNOT create %s for write\n", __FUNCTION__, me->path);
        return -1;
    }

    iniparser_dump_ini(me->dictionary, ini_file);
    fclose(ini_file);

    return 0;
}

int syscfg_retrieve_entry(syscfg_t* me, const char* entry, const char** value)
{
    if (iniparser_find_entry(me->dictionary, entry) == -1)
    {
        return -1;
    }

    *value = iniparser_getstring(me->dictionary, entry, NULL);
    if (NULL == *value)
    {
        return -1;
    }
    
    return 0;
}

void syscfg_free(syscfg_t* me)
{
    if (NULL != me)
    {
        iniparser_freedict(me->dictionary);
        free(me);
    }
}
