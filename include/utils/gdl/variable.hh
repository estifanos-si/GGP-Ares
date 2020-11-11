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
                return sub[this];
            
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

    //Used in Substitutions to index using a variable
    struct VarHasher
    {
        std::size_t operator() (const Variable* x) const{
            return std::hash<std::string>()(x->getName());
        }
    };
    
} // namespace Ares

#endif // VARIABLE_HH
