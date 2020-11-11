#include "utils/memory/body.hh"
#include "utils/utils/hashing.hh"
#include "utils/gdl/gdl.hh"
#include "utils/memory/memCache.hh"
#include "utils/memory/namer.hh"
#include "utils/utils/cfg.hh"
/**
 * 
 * MOCK CLASSES
 * 
 */

namespace ares
{
    
    struct VariantSubstitution : public Substitution
    {
        virtual bool isRenaming() const { return true;}
    };
    std::string toString(const Term* t){
        if( is_var(t) ){
            return "?x$" + std::to_string(t->get_name());
        }
        if( is_const(t)){
            return "C$" + std::to_string(t->get_name());
        }
        structured_term *st = (structured_term*)t;
        std::string s("(");
        if(not (*st) ) s.append("not ( ");
        std::string name = is_fn(t) ? "F" : "P";
        s.append(name + std::to_string(st->get_name()));
        for (auto &t : st->getBody()){
            s.append(" " + toString(t.get()));
        }
        if(not (*st) ) s.append(" )");
        s.append(")");
        return s;
    }
    
    class Ares
    {
    private:
        /* data */
    public:
        Ares(/* args */) {}
        ~Ares() {}
        static MemCache* memCache;
        static MemoryPool* mempool;
    };
    MemCache* Ares::memCache = nullptr;
    MemoryPool* Ares::mempool = nullptr;
    
    //Namer static
    std::unordered_map<ushort, std::string> Namer::vIdName;
    std::unordered_map<std::string, ushort> Namer::vNameId;
    
    std::unordered_map<ushort, std::string> Namer::idName;
    std::unordered_map<std::string, ushort> Namer::nameId;
    
    //Initialize static members of term
    cnst_term_sptr     Term::null_term_sptr(nullptr);
    cnst_lit_sptr     Term::null_literal_sptr(nullptr);
    
    template<class T>
    MemoryPool* _Body<T>::mempool =nullptr;
}; // namespace ares