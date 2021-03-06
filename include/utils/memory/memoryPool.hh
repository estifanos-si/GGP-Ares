#ifndef MEMORY_POOL_HH
#define MEMORY_POOL_HH
#include "utils/threading/locks.hh"

#include <stdlib.h>
#include <sys/types.h>

#include <iostream>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace ares
{
    template <class T>
    struct _Body;
    class MemCache;
    class Term;
    class structured_term;
    class Constant;
    class Variable;
    class Function;
    class Atom;
    class Or;
    class Clause;

    /**************************************************/
    /**
     * Typedef shared_ptr for the different managed objects.
     */
    typedef _Body<const Term> Body;
    typedef Body ClauseBody;
    /**************************************************/
    /**
     * Containers of shared_ptrs of terms and literals
     */
    typedef std::vector<Term*> term_container;
    typedef std::vector<Atom*> lit_container;

    typedef std::vector<const Term*> cnst_term_container;
    typedef std::vector<const Atom*> cnst_lit_container;

    typedef u_char arity_t;
    typedef std::unordered_map<arity_t, std::pair<uint, std::vector<void*>>>
        arity_pool_map;

    /**************************************************/

    enum pool_type {
        sterm_pool_t = 0,
        clause_pool_t,
        body_pool_t,
        container_pool_t
    };

    /**
     * A pool of structured_terms and clasuses.
     */
    class MemoryPool
    {
     private:
        // Ctor
        MemoryPool(std::size_t st_terms, std::size_t clause_s,
                   std::vector<std::pair<arity_t, uint>> arities);

        MemoryPool(const MemoryPool&) = delete;
        MemoryPool& operator=(const MemoryPool&) = delete;
        MemoryPool(const MemoryPool&&) = delete;
        MemoryPool& operator=(const MemoryPool&&) = delete;

     public:
        inline static MemoryPool& create(
            std::size_t st_terms, std::size_t clause_s,
            std::vector<std::pair<arity_t, uint>> arities)
        {
            static MemoryPool mem(st_terms, clause_s, arities);
            return mem;
        }

        MemCache* getCache() { return memCache; }
        void init_pools(std::size_t st_terms, std::size_t clause_s,
                        std::vector<std::pair<arity_t, uint>> arities);
        /**
         * Initializes the structured_term, Clause, Body pools.
         */
        template <class T>
        inline void init_pool(std::vector<void*>& pool, std::size_t n)
        {
            pool.resize(n);
            for (size_t i = 0; i < n; i++) pool[i] = calloc(1, sizeof(T));
        }
        /**
         * Initializes container_pool
         */
        inline void init_pool(std::vector<std::pair<arity_t, uint>>& arities)
        {
            for (auto&& pair : arities) {
                auto& ar = pair.first;
                auto& per_arity_amount = pair.second;

                if (ar == 0)
                    continue;
                ar_gfactor(ar) = 1;
                ar_pool(ar).resize(per_arity_amount);
                // Allocate per_arity_amounts of T objects 0 initialized
                for (size_t k = 0; k < per_arity_amount; k++) {
                    term_container* t = new term_container();
                    t->resize(ar);
                    ar_pool(ar)[k] = t;
                }
            }
        }

        /**
         * @param type could be sterm_pool_t,clause_pool_t and
         * @returns a structured_term from the free function pool.
         * @returns a Clause from the free Clause pool.
         * respectively.
         */
        void* allocate(pool_type type);
        /**
         * @returns a container<cnst_term_sptr>* or container<cnst_lit_sptr>*
         * from the free pool
         */
        void* allocate(arity_t arity);

        inline uint& ar_gfactor(arity_t& ar)
        {
            return container_pool[ar].first;
        }

        inline std::vector<void*>& ar_pool(arity_t ar)
        {
            return container_pool[ar].second;
        }
        /**
         * increase the capacity of structured_term, Clause, Body pools.
         */
        inline void grow(std::vector<void*>& pool, pool_type t)
        {
            pool.reserve(gfactor[t]);
            for (size_t i = 0; i < gfactor[t]; i++)
                pool.push_back(calloc(1, pool_element_size[t]));
            gfactor[t] <<= 1;
        }
        /**
         * Increase the capacity  container_pool
         */
        inline void grow(std::vector<void*>& pool, uint& gfactor, arity_t ar)
        {
            if (gfactor == 0)
                ++gfactor;
            pool.reserve(gfactor);
            for (size_t i = 0; i < gfactor; i++) {
                term_container* l = static_cast<term_container*>(
                    calloc(1, sizeof(term_container)));
                l->resize(ar);
                pool.push_back(l);
            }
            gfactor <<= 1;
        }
        /**
         * Return @param st a structured_term back to the free pool to be
         * reused.
         */
        void deallocate(structured_term* st);
        /**
         * Return @param c,a Clause ptr, back to the free pool to be reused.
         */
        void deallocate(Clause* c);
        /**
         * Return @param _body back to the free pool to be reused.
         */
        template <class T>
        void deallocate(_Body<T>* _body)
        {
            std::lock_guard<SpinLock> lk(slock[body_pool_t]);
            body_pool.push_back(_body);
        }

        /**
         * Return @param vec back to the free pool to be reused.
         */
        void deallocate(cnst_term_container* vec);
        void deallocate(cnst_lit_container* vec);

        inline std::size_t capacity(pool_type t, arity_t arity = 0)
        {
            switch (t) {
                case sterm_pool_t:
                    return st_term_pool.size();
                case clause_pool_t:
                    return clause_pool.size();
                case body_pool_t:
                    return body_pool.size();
                case container_pool_t:
                    return ar_pool(arity).size();

                default:
                    break;
            }
            return -1;
        }
        ~MemoryPool();
        const static lit_container* EMPTY_CONTAINER;

     private:
        inline void* allocate(std::vector<void*>& pool)
        {
            void* el = pool.back();
            pool.pop_back();
            return el;
        }

        /**
         * Data
         */
     private:
        static MemCache* memCache;
        uint pool_element_size[3];
        uint gfactor[3] = {1, 1, 1};

        /**
         * Most operations are either popping (while allocation) or
         * pushing(deallocatio) so they wouldn't take much time so spin lock
         * seems suffices.
         * TODO: if neccessary change lock granularity from 1 lock per
         * arity_pool_map to 1 lock per arity_pool_map[arity].
         */
        SpinLock slock[4];
        /**
         * Apparently std::vector<> is faster than std::list even for alot of
         * insertions/removals!
         */

        /**
         * These are basically collections of "shells" that can hold a name and
         * a body*. The separation b/n term and body is b/c only MemCache is
         * allowed to create terms/literals but other user code can still create
         * a PoolKey{name,body} to index the expression pool. That way only one
         * instance of a term exists.
         */
        std::vector<void*> st_term_pool;
        std::vector<void*> clause_pool;
        /**
         * This is also collections of "shells/wrappers"  around an actuall
         * container that can hold shared_ptr of Terms.
         */
        std::vector<void*> body_pool;
        /**
         * This is a collection of containers (probably std::vectors) to hold
         * shared_ptr of Terms. Each is a mapping of an arity to a pool of
         * containers with that arity
         */
        arity_pool_map container_pool;  // for body of Literals, Fns and Clauses

        std::vector<std::vector<void*>*> POOLS;
    };

    /**
     * TODO: write an allocator for the pool. This can be used with
     * stl-containers and shared_ptr.
     */
    class Allocator
    {
     private:
        /* data */
     public:
        Allocator(/* args */) {}
        ~Allocator() {}
    };
}  // namespace ares

#endif