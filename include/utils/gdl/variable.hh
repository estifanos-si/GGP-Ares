#ifndef VARIABLE_HH
#define VARIABLE_HH

#include "utils/gdl/term.hh"

namespace Ares
{
    class Variable : public Term
    {

    public:
        Variable(std::string name):Term(name)
        {
        }
        /**
         * Apply Substitution sub on this variable, return its replacement if bound 
         * otherwise return self.
         */
        virtual Term* operator ()(Substitution &sub,bool inPlace=false){
            if( sub.contains(this) )
                return this;

            return this;
        }
        
        virtual bool isGround(){
            return false;
        }
        //Used for resolving hash collisions in Substitutions.
        bool operator ==(const Variable &y)const{
            return y.name == name;
        }
    };
    
} // namespace Ares

#endif // VARIABLE_HH
