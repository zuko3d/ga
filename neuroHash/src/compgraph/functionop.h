#ifndef FUNCTIONOP_H
#define FUNCTIONOP_H

#include "computationnode.h"

#include <functional>

class FunctionOp
{
public:
    FunctionOp();

protected:
    std::function<numeric_t(numeric_t)> func_;
};

#endif // FUNCTIONOP_H
