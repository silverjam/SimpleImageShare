#ifndef SISLIBRARY_H
#define SISLIBRARY_H

#include "commandsink.h"
#include "siscommands.h"
#include "datapool.h"

#define VERIFY(cond) \
{ \
    bool ok = cond; \
    Q_ASSERT(ok); \
}

#endif // SISLIBRARY_H
