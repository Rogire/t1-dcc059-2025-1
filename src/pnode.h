#ifndef PNODE_H
#define PNODE_H

// CRIADO PELO GRUPO - Igor Correa Trifilio Campos 202365092AB

template <typename V>
class pnode
{
public:
    pnode(V v) : value(v), prox(nullptr) 
    {
        value = v;
    }
    
    pnode() {}
    void setVal(V value) { this->value = value; };
    void setProx(pnode *p) { this->prox = p; };
    V getVal() { return this->value; };
    pnode *getProx() { return this->prox; };

private:
    V value;
    pnode *prox;
};

#endif // PNODE_H
