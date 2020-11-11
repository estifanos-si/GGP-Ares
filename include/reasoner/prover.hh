#ifndef PROVER_HH
#define PROVER_HH

#include "reasoner/substitution.hh"
#include "utils/gdl/gdl.hh"
#include "utils/game/match.hh"
#include "utils/utils/exceptions.hh"
#include "utils/gdl/gdlParser/gdlParser.hh"
#include "utils/memory/memCache.hh"
#include "reasoner/cache.hh"
#include "reasoner/unifier.hh"
#include "utils/utils/cfg.hh"
#include "utils/threading/threading.hh"

namespace ares
{ 
    class Prover
    {
    private:
        //ctor
        Prover(const KnowledgeBase* _kb=nullptr)
        :kb(_kb)
        {
            if( cfg.proverThreads > 0 ) proverPool = new ThreadPool(new LoadBalancerRR(cfg.proverThreads));
        };


        Prover(const Prover&)=delete;
        Prover& operator=(const Prover&)=delete;
        Prover(const Prover&&)=delete;
        Prover& operator=(const Prover&&)=delete;
    /**
     * Methods
     */
    public:
        /**
         * The singleton prover.
         */
        static Prover& create(const KnowledgeBase* _kb=nullptr){
            static Prover prover(_kb);
            return prover;     //singleton
        }

        inline void setKb(const KnowledgeBase* kb_){ kb=kb_; }
        /**
         * Carry out backward chaining. Search the sld-tree for a successful refutation.
         * Using kb + context as a combined knowledgebase.
         * @param query.context : The current state determined by true and does
         * @param query.cb is called with an answer each time a successful refutation is derived.
         */
        void compute(Query& query);  
        ~Prover(){ delete proverPool; }
    private:
        /**
         * Extension of compute(Query& query), needed for recursion.
         */ 
        void compute(Query query,const bool isLookup);
        /**
         *
         */
        void compute(cnst_lit_sptr lit,Query q,CallBack* cb,const bool lookup=false);
        /**
         * Iterate over all of the new solutions in `it` and resolve against lit
         * @param it a list of new solutions for lit
         */
        void lookup(AnsIterator& it, Query& query,cnst_lit_sptr& lit);
        /**
         * Carry out a single (normal) SLD-Resolution Step
         * Where the selected literal in goal is positive.
         */
        Clause* resolve(const cnst_lit_sptr& lit, const Clause& c,SuffixRenamer&);
        /**
         * Carry out a single SLDNF-Resolution Step
         * Where the selected literal in goal is negative.
         */
        void computeNegation(Query& query);
        /**
         * The distinct relation.
         */
        void computeDistinct(Query& query);
        inline bool contextual(const State* context, const cnst_lit_sptr& g) const{
            return context and (  g->get_name() == Namer::DOES  or  g->get_name() == Namer::TRUE) ;
        }

    /**
     * Data
     */
    private:
        const KnowledgeBase* kb;
        ThreadPool* proverPool = nullptr;     //Used for the initial query, and subequent searches.   

        friend class ClauseCB;
    }; 
} // namespace ares
#endif