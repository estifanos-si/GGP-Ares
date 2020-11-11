#ifndef CONST_HH
#define CONST_HH

#include "utils/gdl/term.hh"

namespace Ares
{
    class Constant: public Term
    {
    public:
        Constant(std::string name):Term(name){}
        virtual Term* operator ()(Substitution &sub,bool inPlace=false){
            return this;
        }
        virtual bool isGround(){
            return true;
        }
    };
    
} // namespace Ares

#endif