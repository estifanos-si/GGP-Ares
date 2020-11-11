#ifndef SUBST_HH
#define SUBST_HH

#include <unordered_map>
#include "utils/gdl/variable.hh"
#include <functional>

namespace Ares
{
    typedef std::unordered_map<Variable*,Term*,VarHasher> Mapping;
    class Substitution
    {
        
    private:
        Mapping mappping;
        
    public:

        bool bind(Variable* x, Term* t){
            //This variable has already been bound!
            if(mappping.find(x) != mappping.end()) return false;
            mappping[x] = t;
            return true;
        }

        //Overload the indexing operator, to get the underlying mapping        
        Term* operator[](Variable* x){
            return mappping[x];
        }

        bool contains(Variable* x){
            return mappping.find(x) != mappping.end();
        }

        //Overload the += operator for substitution composition.
        //θ:= {X1/s1,...,Xm/sm}
        //σ:= {Y1/t1,...,Yn/tn}
        //θσ= {X1/s1σ,...,Xm/smσ,Y1/t1,...,Yk/tn} 
        //k<=n, where Each Yi distinct from Xj
        void operator +=(Substitution& sub){
            for (auto& it : this->mappping)
            {   
                Variable* x = it.first;
                Term* term = it.second;
                //An inplace substitution.
                mappping[x] = (*term)(sub,true);
            }
            //Add new mappings not present in this substitution.
            for (auto &it : sub.mappping)
                if(this->mappping.find(it.first) == this->mappping.end())
                    this->mappping[it.first] = it.second;
        }   
    };

} // namespace Ares
#endif 
