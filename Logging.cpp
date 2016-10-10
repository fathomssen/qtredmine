#include "Logging.h"

static int __debug_indent__ = 0;
static bool __debug_newline__ = false;

int
getLoggingIndent()
{
    return __debug_indent__;
}

bool
getLoggingNewline()
{
    return __debug_newline__;
}

void
increaseLoggingIndent()
{
    ++__debug_indent__;
    __debug_newline__ = false;
}

void
decreaseLoggingIndent()
{
    --__debug_indent__;
    __debug_newline__ = true;
}
