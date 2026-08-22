#ifndef _COMPAT_FIXES_H_
#define _COMPAT_FIXES_H_
#include <linux/timer.h>
#ifndef from_timer
#define from_timer(var, callback_timer, timer_fieldname) \
    container_of(callback_timer, typeof(*var), timer_fieldname)
#endif
#ifndef del_timer_sync
#define del_timer_sync(timer) timer_delete_sync(timer)
#endif
#endif
