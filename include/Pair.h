#ifndef PAIR_H
#define PAIR_H 

template<class M, class N>
struct Pair {
    M first;
    N second;
    
    Pair() {}

    Pair(M v1, N v2): first(v1), second(v2) {}
};

#endif