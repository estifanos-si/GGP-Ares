#include "utils/utils/hashing.hh"
#include "reasoner/substitution.hh"
#include "utils/gdl/variable.hh"
#include "utils/gdl/term.hh"
#include <string.h>
#include "utils/gdl/clause.hh"

namespace ares
{   
    std::size_t CharpHasher::operator()(const char* name) const{
        unsigned long hash = 5381;
        int c;
        while ( (c = *name++))
            hash = ((hash << 5) + hash) + c;
        return hash;
    }
    

    std::size_t SpVarHasher::operator() (const cnst_var_sptr& x) const{
        return x->hash();
    }
    bool SpVarEqual::operator()(const cnst_var_sptr& x, const cnst_var_sptr& y) const{
        return x.get() == y.get();
    }
    std::size_t ClauseHasher::operator() (const Clause* c) const{
        std::size_t hash = 0;
        if( c->head) hash_combine(hash, c->getHead());
        for (auto &&lit : c->body)
            hash_combine(hash, lit);
        
        return hash;
    }
    bool ClauseHasher::operator()(const Clause* c,const Clause* c2) const{
        if( c->body.size() != c2->body.size() || (c->head != c2->head)) return false;

        for (size_t i = 0; i < c->body.size(); i++)
            if( c->body[i] != c2->body[i]) 
                return false;
        return true;
    }
    bool StrEq::operator()(const char* s1, const char* s2) const{
        return strcasecmp(s1,s2) == 0;
    }
    
    std::size_t PoolKeyHasher::operator() (const PoolKey& k) const{
        std::size_t nHash = std::hash<bool>()(k.p);
        for (const cnst_term_sptr& t: *k.body)
            hash_combine(nHash, t);
        return nHash;
    }
    bool PoolKeyEqual::operator()(const PoolKey& k1, const PoolKey& k2) const{
        if( (k1.p != k2.p)  || (k1.body->size() != k2.body->size()) ) return false;
        for (size_t i = 0; i < k1.body->size(); i++)
            if( (*k1.body)[i] != (*k2.body)[i])
                return false;
        return true;
    }
} // namespace ares