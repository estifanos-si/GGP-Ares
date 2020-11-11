#ifndef SUBST_HH
#define SUBST_HH

#include <unordered_map>
#include <functional>

namespace Ares
{
    class Variable;
    class Term;
    //Used in Substitutions to index using a variable
    struct VarHasher
    {
        std::size_t operator() (const Variable* x) const;
    };

    typedef std::unordered_map<Variable*,Term*,VarHasher> Mapping;

    class Substitution
    {

    private:
        Mapping mappping;

    public:
        bool bind(Variable* x, Term* t);
        //Overload the indexing operator, to get the underlying mapping        
        Term* operator[](Variable* x);

        bool contains(Variable* x);

        //Overload the += operator for substitution composition.
        //θ:= {X1/s1,...,Xm/sm}
        //σ:= {Y1/t1,...,Yn/tn}
        //θσ= {X1/s1σ,...,Xm/smσ,Y1/t1,...,Yk/tn} 
        //k<=n, where Each Yi distinct from Xj
        void operator +=(Substitution& sub);
    };

} // namespace Ares
#endif 
