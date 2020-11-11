#ifndef ARES_HH
#define ARES_HH

#include <thread>
#include <chrono> 
#include <fstream>  
#include "reasoner/suffixRenamer.hh"
#include "reasoner/reasoner.hh"
#include "utils/gdl/gdlParser/gdlParser.hh"
#include "utils/game/game.hh"
#include <iostream>
#include "utils/utils/cfg.hh"
#include "utils/memory/memoryPool.hh"

namespace ares
{
    class Ares
    {
    private:
        /* data */
    public:
        Ares(/* args */){}
        ~Ares(){}

        static ExpressionPool* exprpool;
        static MemoryPool* mempool;
    };
} // namespace ares

#endif  