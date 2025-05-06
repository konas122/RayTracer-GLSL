#ifndef __PROGRESS_H__
#define __PROGRESS_H__

#include "thread/spin_lock.h"

class Progress {
public:
    Progress(size_t total, size_t step = 1);
    void update(size_t count);

private:
    size_t total, current;
    int percent, last_percent, step;
    SpinLock spin_lock;
};

#endif
