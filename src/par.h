#ifndef PAR_H
#define PAR_H

// CRIADO PELO GRUPO - Igor Correa Trifilio Campos 202365092AB
/*
Alunos:
Igor Correa Trifilio Campos
Gustavo Duarte Fernandes de Jesus
Enzo Araújo Pinheiro
Gabriel Toledo Gonçalves Barreto

Repositório: https://github.com/Rogire/t1-dcc059-2025-1
*/
template <typename K, typename V>
class par
{
public:
    par(K k, V v) : key(k), value(v) {}
    par() {}

    K &getKey() { return key; };
    V &getValue() { return value; };

    void setKey(K k) { key = k; };
    void setValue(V v) { value = v; };

private:
    K key;
    V value;
};

#endif // PAR_H
