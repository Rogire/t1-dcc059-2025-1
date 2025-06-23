#ifndef PAR_H
#define PAR_H

//CRIADO PELO GRUPO - Igor Correa Trifilio Campos 202365092AB

template <typename K, typename V>
class par
{
public:
    par(K k, V v) : key(k), value(v) {}
    par() {}

    K getKey() { return key; };
    V getValue() { return value; };

    void setKey(K k) { key = k; };
    void setValue(V v) { value = v; };

private:
    K key;
    V value;
};

#endif // PAR_H
