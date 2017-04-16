//  File:   prefetch_cache.h
//  Author: Yuri Panchul

#ifndef PREFETCH_CACHE_H
#define PREFETCH_CACHE_H

#include <sys/attribs.h>

#include "types.h"

#define FLASH_FREQUENCY (30 * 1000 * 1000)

__longramfunc__ void prefetch_cache_backup (void);
void prefetch_cache_report (bool dump_cache_lines);

#endif
