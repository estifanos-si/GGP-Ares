#ifndef VARRENAMER_HH
#define VARRENAMER_HH

#include "reasoner/substitution.hh"
#include "utils/gdl/variable.hh"

namespace Ares
{
    class VarRenamer
    {
    public:
        VarRenamer(const VarRenamer& vr) = delete;
        VarRenamer& operator =(const VarRenamer& vr) = delete;
        
        VarRenamer(/* args */){};
        virtual Context* rename(Context& context) = 0;
        ~VarRenamer();
    };
} // namespace Ares

#endif