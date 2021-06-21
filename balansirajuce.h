#ifndef BALANSIRAJUCE_H
#define BALANSIRAJUCE_H
#include <string>
#include <ostream>
#include <vector>
using namespace std;
template<class T>
class BalansirajuceStablo {

    struct Cvor {
        T vrijednost;
        unsigned int brojPotomaka;
        Cvor *roditelj, *lijevoDijete, *desnoDijete;
        Cvor(T vrijednost, Cvor *roditelj = nullptr, Cvor *lijevoDijete = nullptr, Cvor *desnoDijete = nullptr);

        Cvor(const Cvor &);

        Cvor(Cvor &&) noexcept ;
    };
    Cvor* korijen;
    unsigned int brojElemenata;

    void izgradiPodstablo(Cvor *, Cvor *);
    template<class T1>
    friend void inorderUTok(ostream &, typename BalansirajuceStablo<T1>::Cvor *);
    void preorderIspis(ostream&, Cvor*);
    void postorderIspis(ostream&, Cvor*);
    Cvor* nadjiRoditelja(const T&, typename BalansirajuceStablo<T>::Cvor*);
    void azurirajPutanjuDoKorijena(Cvor*, int);
    void balansiraj(Cvor*);
    void popuniSortiraniNiz(vector<T>&, Cvor*);
    Cvor* napraviStabloIzSortiranogNiza(const vector<T>&, int, int);

    void unistiStablo(Cvor *);
public:
    BalansirajuceStablo();
    BalansirajuceStablo(const BalansirajuceStablo<T>&);
    BalansirajuceStablo(BalansirajuceStablo<T>&&) noexcept ;

    BalansirajuceStablo<T>& operator=(const BalansirajuceStablo<T>&);
    BalansirajuceStablo<T>& operator=(BalansirajuceStablo<T>&&) noexcept;

    void dodaj(const T& element);
    void izbrisi(const T &element);

    void ispisiUTokInorder(ostream &);

    void ispisiPreorder(ostream &);
    void ispisiPostorder(ostream &);

    Cvor *nadji(const T &);

    template<class T1>
    friend ostream &operator<<(ostream &, const BalansirajuceStablo<T1> &);

    ~BalansirajuceStablo() {
        unistiStablo(korijen);
        brojElemenata = 0;
        korijen = nullptr;
    }
};


#endif // BALANSIRAJUCE_H
