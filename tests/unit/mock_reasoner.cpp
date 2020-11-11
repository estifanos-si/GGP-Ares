#include "mock_reasoner.hh"

namespace ares{
    /**
     * Generate a random game tree with random branching factor and depth.
     */
    void MockReasoner::initGameTree(){   
        typedef std::function<void(std::reference_wrapper<Node>,uint)> Fn;
        auto* root = new Node(new State);
        game.max_branching = (rand() % 4 ) + 5;
        game.max_depth = (rand() % 5 ) + 4;
        Fn dfsConstruct = [&](Node& node,uint depth){
            game.addNode(&node);
            uint bf = (rand() % game.max_branching );
            //Assign terminal states a value;
            if( depth == 0 || bf==0){ node.value = (rand() % 2) * 100; return;}
            //Create the transition table
            for (size_t i = 0; i < bf; i++)
                node.add(new Action(),new Node(new State));
            for (auto &&[act, child] : node.get())
                dfsConstruct(*child.get(), depth-1);  
        };

        dfsConstruct(*root, game.max_depth);
    }

    const Roles& MockReasoner::roles(){
        static Roles roles{0};
        return roles;
    }
    /**
     * @returns the initial state
     */
    const State& MockReasoner::init(){
        return *game.init();
    }
    /**
     * @param state the current state
     * @param action an ordered (by role order) list of moves taken by roles. 
     *  i.e action[i] is the move taken by role[i].
     * @returns the next state following from state by taking action.
     */
    State* MockReasoner::next(const State& state,const Action& action){
        State* nxt = game[&state][&action].getState();
        auto* clone = new State;
        game.addEqui(clone,nxt);
        return clone;
    }
    /**
     * What are the legal moves in the state @param state
     */
    Moves* MockReasoner::moves(const State& state,const Role& role,bool rand){
        throw "NOT IMPLEMENTED YET! (NOT NECCESSARY FOR MONTE CARLO TESTS!)";
        return nullptr; //so the compiler doesn't complain
    }
    /**
     * Is @param state a terminal state?
     */
    bool MockReasoner::terminal(const State& state){
        return game[&state].terminal();
    }

    /**
     * @returns the reward associated with this role in the @param state.
     */
    float MockReasoner::reward(Role& role, const State* state){
        return game[state].value;
    }
    

    /**
     * Some helper functions.
     */
    
    ushort MockReasoner::roleIndex(ushort name)const{
        return 0;
    }
        /**
     * Get a random move.
     */
    move_sptr MockReasoner::randMove(const State& state,const Role& role){
        throw "NOT IMPLEMENTED YET! (NOT NECCESSARY FOR MONTE CARLO TESTS!)";
        return nullptr; //so the compiler doesn't complain   
    }
    /**
     * Get a random action, i.e <move_1,...,move_n> , where move_i is taken by role i.
     */
    Action* MockReasoner::randAction(const State& state){
        auto& node = game[&state];
        uint i =  rand() % node.get().size();
        uint j =0;
        for (auto &&[action, c] : node.get())
            if( j == i ){
                auto* selected = new Action;
                node.addEqui(selected, action);
                return selected;
            }
            else j++;
        return nullptr;
    }

    
    /**
     * Get all possible actions from this state,
     * an action = <move_1,...,move_n> , where move_i is taken by role i.
     */
    std::vector<uAction>* MockReasoner::actions(const State& state){
        auto* as = new std::vector<uAction>();
        auto& node = game[&state];
        for (auto &&[act, c] : node.get()){
            auto* clone = new Action();
            node.addEqui(clone, act);
            as->push_back(uAction(clone));
        }
        return as;
    }
    
    void MockReasoner::reset(Game* kb){
    }
};