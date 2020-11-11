#ifndef FUNCTION_HH
#define FUNCTION_HH

#include "utils/gdl/term.hh"

namespace Ares
{
    //This represents gdl functions
    class Function:public Term
    {

    private:
        std::vector<Term*> body;

    public:
        Function(std::string n):Term(n){}
        Function(std::string n,uint arity):Term(n){ body.resize(arity);}
        
        uint getArity(){
            return body.size();
        }
        bool setArity(uint arity){
            if(body.size() > 0 ) return false;

            body.resize(arity);
            return true;
        }

        virtual bool isGround(){
            bool ground = true;
            for (Term* arg : body)
                ground &= arg->isGround();
            
            return ground;
        }
        /**
         * Create an instance of this function do 
         * either in place modifications or by creating
         * a new term and modifying that.
         */
        virtual Term* operator ()(Substitution &sub,bool inPlace=false){
            Function* instance = this;
            if(!inPlace)
                instance  = new Function(this->name,getArity());

            for (size_t i = 0; i < this->getArity(); i++)
            {
                Term* arg = this->body[i];
                instance->body[i] = (*arg)(sub,inPlace);
            }
            return instance;
        }

        ~Function();
    };
    Function::~Function(){

    }
} // namespace Ares


#endif