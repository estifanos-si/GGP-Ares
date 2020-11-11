#ifndef LITERAL_HH
#define LITERAL_hh
#include <string>
#include <vector>
#include "utils/gdl/term.hh"

namespace Ares
{
    class Literal
    {
    private:
        std::string name;
        bool positive;
        std::vector<Term*> body;
    public:
        Literal(const Literal& l) = delete;
        Literal& operator = (const Literal &l) = delete;
        Literal(std::string n,bool p):name(n),positive(p){}
        Literal(std::string n, bool p,uint arity):name(n),positive(p){ body.resize(arity);}
        explicit operator bool() {
            return positive;
        }
        uint getArity(){return body.size();}
        Literal* operator ()(Substitution sub,bool inPlace=false){
            Literal* instance = this;
            if(!inPlace)
                instance = new Literal(name,positive,getArity());
            
            
        }
        ~Literal();
    };
} // namespace Ares

#endif