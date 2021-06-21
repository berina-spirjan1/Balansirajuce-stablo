#ifndef BALANSIRAJUCE_CPP
#define BALANSIRAJUCE_CPP
#include "balansirajuce.h"
#include <iostream>
#include <vector>


template<class T>
BalansirajuceStablo<T>::Cvor::Cvor(T vrijednost, BalansirajuceStablo::Cvor *roditelj, BalansirajuceStablo::Cvor *lijevoDijete, BalansirajuceStablo::Cvor *desnoDijete):vrijednost(vrijednost),
                                                                                                                                                                       roditelj(roditelj),
                                                                                                                                                                       lijevoDijete(lijevoDijete),
                                                                                                                                                                       desnoDijete(desnoDijete),
                                                                                                                                                                       brojPotomaka(0) {}

template<class T>
BalansirajuceStablo<T>::Cvor::Cvor(const BalansirajuceStablo::Cvor &cvor) {
    this->vrijednost = cvor.vrijednost;
}

template<class T>
BalansirajuceStablo<T>::Cvor::Cvor(BalansirajuceStablo::Cvor &&cvor) noexcept {
    this->vrijednost = cvor.vrijednost;
    this->roditelj = cvor.roditelj;
    this->lijevoDijete = cvor.lijevoDijete;
    this->desnoDijete = cvor.desnoDijete;

    cvor.roditelj = nullptr;
    cvor.lijevoDijete = nullptr;
    cvor.desnoDijete = nullptr;
}


template<class T>
BalansirajuceStablo<T>::BalansirajuceStablo() {
    this->brojElemenata = 0;
    this->korijen = nullptr;
}

template<class T>
BalansirajuceStablo<T>::BalansirajuceStablo(const BalansirajuceStablo<T> &stablo) {
    this->brojElemenata = stablo.brojElemenata;
    Cvor *korijenStabla = stablo.korijen;
    if (korijenStabla) { // Ako postoji element u korijenu stabla, onda treba kopirati elemente stabla u novo stablo
        this->korijen = new Cvor(korijenStabla->vrijednost);
        izgradiPodstablo(this->korijen, korijenStabla);
    }
}

template<class T>
void BalansirajuceStablo<T>::izgradiPodstablo(BalansirajuceStablo::Cvor *destinacija, BalansirajuceStablo::Cvor *izvor) {
    if (izvor) {
        if (izvor->lijevoDijete) {
            auto *lijevoDijete = new Cvor(izvor->lijevoDijete);
            destinacija->lijevoDijete = lijevoDijete;
            lijevoDijete->roditelj = destinacija;
            izgradiPodstablo(lijevoDijete, izvor->lijevoDijete);
        }
        if (izvor->desnoDijete) {
            auto *desnoDijete = new Cvor(izvor->desnoDijete);
            destinacija->desnoDijete = desnoDijete;
            desnoDijete->roditelj = destinacija;
            izgradiPodstablo(desnoDijete, izvor->desnoDijete);
        }
    }
}

template<class T>
void inorderUTok(std::ostream &tok, typename BalansirajuceStablo<T>::Cvor *cvor) {
    if (cvor) {
        inorderUTok<T>(tok, (typename BalansirajuceStablo<T>::Cvor *) cvor->lijevoDijete);
        tok << cvor->vrijednost << " ";
        inorderUTok<T>(tok, cvor->desnoDijete);
    }
}

template<class T>
std::ostream &operator<<(std::ostream &tok, const BalansirajuceStablo<T> &stablo) {
    typename BalansirajuceStablo<T>::Cvor *korijen = stablo.korijen;
    inorderUTok<T>(tok, korijen);
    return tok;
}

template<class T>
void BalansirajuceStablo<T>::dodaj(const T &element) {
    Cvor *roditelj = nadjiRoditelja(element, korijen);
    if (roditelj) {
        if (roditelj->vrijednost == element) {
            // Ne radi nista, ne treba dodavati isti element
        } else {
            Cvor *cvor = new Cvor(element);
            if (roditelj->vrijednost > element) {
                roditelj->lijevoDijete = cvor;
            } else {
                roditelj->desnoDijete = cvor;
            }
            cvor->roditelj = roditelj;
            brojElemenata++;
            azurirajPutanjuDoKorijena(cvor, 1);
        }
    } else {
        Cvor *cvor = new Cvor(element);
        korijen = cvor;
        brojElemenata++;
        azurirajPutanjuDoKorijena(cvor, 1);
    }

}

template<class T>
typename BalansirajuceStablo<T>::Cvor *BalansirajuceStablo<T>::nadjiRoditelja(const T &element, typename BalansirajuceStablo<T>::Cvor *roditelj) {
    if (roditelj) {
        if (element < roditelj->vrijednost) {
            if (roditelj->lijevoDijete) {
                return nadjiRoditelja(element, roditelj->lijevoDijete);
            } else {
                return roditelj;
            }
        } else if (roditelj->vrijednost == element) {
            return roditelj;
        } else {
            if (roditelj->desnoDijete) {
                return nadjiRoditelja(element, roditelj->desnoDijete);
            } else {
                return roditelj;
            }
        }
    } else {
        return nullptr;
    }
}

template<class T>
void BalansirajuceStablo<T>::izbrisi(const T &element) {
    // Prvo trebamo pronaci element koji se treba obrisati, tj. pokazivac na cvor koji sadrzi ovu vrijednost
    Cvor *zaIzbrisati = nadjiRoditelja(element, korijen);
    if (zaIzbrisati) {
        // postoji onaj koji se zeli izbrisati
        if (!zaIzbrisati->lijevoDijete && !zaIzbrisati->desnoDijete) {
            // Ukoliko cvor nema djece, on je list
            // tada se samo treba izbaciti iz stabla i azurirati njegov roditelj
            Cvor *roditelj = zaIzbrisati->roditelj;
            if (roditelj) {// ukoliko roditelj postoji, veza prema izbrisanom djetetu se ukida
                if (roditelj->lijevoDijete == zaIzbrisati) {
                    roditelj->lijevoDijete = nullptr;
                } else {
                    roditelj->desnoDijete = nullptr;
                }
            }
            if (korijen == zaIzbrisati) {// u slucaju da je korijen stabla onaj koji se treba izbrisati, azuriramo polje klase
                korijen = nullptr;
            }
            // zbog mehanizma balansiranja, trebamo azurirati broj djece za cvorove koje se nalaze od ovog izbrisanog cvora pa do korijena
            // to znaci da cemo svakom roditelju na ovom putu oduzeti jednog potomka
            azurirajPutanjuDoKorijena(zaIzbrisati, -1);
            delete zaIzbrisati;
            brojElemenata--;
        } else if (zaIzbrisati->lijevoDijete) {
            // kada brisemo neki element u stablu koji nije list
            // onda trebamo naci nasljednika koji ce zamijeniti obrisani cvor na tom mjestu
            // da bi stablo ostalo u ispravnom stanju
            // nasljednik u ovo slucaju je najveci cvor u lijevom podstablu
            Cvor *nasljednik = zaIzbrisati->lijevoDijete;
            while (nasljednik->desnoDijete) {// idemo na desno dijete, sve dok postoji put
                nasljednik = nasljednik->desnoDijete;
            }
            azurirajPutanjuDoKorijena(nasljednik, -1);
            // trebamo sada azurirati put do korijena, al od nasljednika, zato sto on ide na mjesto obrisanog cvora
            // razlog tome je sto logicki mi nismo obrisali cvor koji stoji u unutrasnjosti stabla, nego smo nasli zamjenu tom cvoru
            // a tehnicki smo obrisali cvor koji je bio na zamjenikovom/nasljednikovom mjestu
            // i zbog toga azuriranje krece od nasljednika pa na gore
            if (!nasljednik->lijevoDijete && !nasljednik->desnoDijete) {
                // Ukoliko nasljednik ima djecu, treba azurirati
                Cvor *roditeljNasljednika = nasljednik->roditelj;
                if (roditeljNasljednika->lijevoDijete == nasljednik) {
                    roditeljNasljednika->lijevoDijete = nullptr;
                } else {
                    roditeljNasljednika->desnoDijete = nullptr;
                }
                nasljednik->roditelj = zaIzbrisati->roditelj;
                if (zaIzbrisati->roditelj) {
                    if (zaIzbrisati->roditelj->lijevoDijete == zaIzbrisati) {
                        zaIzbrisati->roditelj->lijevoDijete = nasljednik;
                    } else {
                        zaIzbrisati->roditelj->desnoDijete = nasljednik;
                    }
                }
                if (zaIzbrisati == korijen) {
                    korijen = nasljednik;
                }
                if (zaIzbrisati->desnoDijete) {
                    zaIzbrisati->desnoDijete->roditelj = nasljednik;
                }
                if (zaIzbrisati->lijevoDijete) {
                    zaIzbrisati->lijevoDijete->roditelj = nasljednik;
                }
                nasljednik->desnoDijete = zaIzbrisati->desnoDijete;
                nasljednik->lijevoDijete = zaIzbrisati->lijevoDijete;
                nasljednik->brojPotomaka = zaIzbrisati->brojPotomaka;
                delete zaIzbrisati;
                brojElemenata--;
            } else if (!nasljednik->desnoDijete) {
                Cvor *zamjena = nasljednik->lijevoDijete;
                Cvor *roditeljNasljednika = nasljednik->roditelj;
                Cvor *lijevoDijeteRoditeljaNasljednika = roditeljNasljednika->lijevoDijete;
                if (nasljednik->roditelj->lijevoDijete == nasljednik) {
                    nasljednik->roditelj->lijevoDijete = zamjena;
                } else {
                    nasljednik->roditelj->desnoDijete = zamjena;
                }

                if (lijevoDijeteRoditeljaNasljednika == nasljednik) {
                    roditeljNasljednika->lijevoDijete = nullptr;
                } else {
                    roditeljNasljednika->desnoDijete = nullptr;
                }

                nasljednik->roditelj = zaIzbrisati->roditelj;
                if (zaIzbrisati->roditelj->lijevoDijete == zaIzbrisati) {
                    zaIzbrisati->roditelj->lijevoDijete = nasljednik;
                } else {
                    zaIzbrisati->roditelj->desnoDijete = nasljednik;
                }
                if (zaIzbrisati == korijen) {
                    korijen = nasljednik;
                }
                nasljednik->desnoDijete = zaIzbrisati->desnoDijete;
                nasljednik->brojPotomaka = zaIzbrisati->brojPotomaka;
                delete zaIzbrisati;
                brojElemenata--;
            }
        } else {
            Cvor *nasljednik = zaIzbrisati->desnoDijete;
            while (nasljednik->lijevoDijete) {
                nasljednik = nasljednik->lijevoDijete;
            }
            // ovo radimo zbog azuriranja broja potomaka za sve cvorove na putu do korijena od cvora koji se tehnicki brise sa mjesta u stablu
            azurirajPutanjuDoKorijena(nasljednik, -1);
            if (!nasljednik->lijevoDijete && !nasljednik->desnoDijete) {
                Cvor *roditeljNasljednika = nasljednik->roditelj;
                if (roditeljNasljednika->lijevoDijete == roditeljNasljednika) {
                    roditeljNasljednika->lijevoDijete = nullptr;
                } else {
                    roditeljNasljednika->desnoDijete = nullptr;
                }
                nasljednik->roditelj = zaIzbrisati->roditelj;
                if (zaIzbrisati->roditelj) {
                    if (zaIzbrisati->roditelj->lijevoDijete == zaIzbrisati) {
                        zaIzbrisati->roditelj->lijevoDijete = nasljednik;
                    } else {
                        zaIzbrisati->roditelj->desnoDijete = nasljednik;
                    }
                }
                if (zaIzbrisati == korijen) {
                    korijen = nasljednik;
                }
                nasljednik->desnoDijete = zaIzbrisati->desnoDijete;
                nasljednik->lijevoDijete = zaIzbrisati->lijevoDijete;
                nasljednik->brojPotomaka = zaIzbrisati->brojPotomaka;

                delete zaIzbrisati;
                brojElemenata--;
            } else if (!nasljednik->lijevoDijete) {
                Cvor *zamjena = nasljednik->desnoDijete;
                zamjena->roditelj = nasljednik->roditelj;
                if (nasljednik->roditelj->lijevoDijete == nasljednik) {
                    nasljednik->roditelj->lijevoDijete = zamjena;
                } else {
                    nasljednik->roditelj->desnoDijete = zamjena;
                }
                Cvor *roditeljNasljednika = nasljednik->roditelj;
                if (roditeljNasljednika->lijevoDijete == roditeljNasljednika) {
                    roditeljNasljednika->lijevoDijete = nullptr;
                } else {
                    roditeljNasljednika->desnoDijete = nullptr;
                }
                nasljednik->roditelj = zaIzbrisati->roditelj;
                if (zaIzbrisati->roditelj->lijevoDijete == zaIzbrisati) {
                    zaIzbrisati->roditelj->lijevoDijete = nasljednik;
                } else {
                    zaIzbrisati->roditelj->desnoDijete = nasljednik;
                }
                if (zaIzbrisati == korijen) {
                    korijen = nasljednik;
                }
                nasljednik->desnoDijete = zaIzbrisati->desnoDijete;
                nasljednik->lijevoDijete = zaIzbrisati->lijevoDijete;
                nasljednik->brojPotomaka = zaIzbrisati->brojPotomaka;
                delete zaIzbrisati;
                brojElemenata--;
            }
        }
    }
}

template<class T>
void BalansirajuceStablo<T>::azurirajPutanjuDoKorijena(BalansirajuceStablo::Cvor *cvor, int povecanje) {
    int brojNivoa = 0;
    Cvor *pocetni = cvor;
    Cvor *temp = cvor;
    while (temp->roditelj) {
        temp = temp->roditelj;
        brojNivoa++;
    }
    // prebrojimo koliko ukupno ima nivoa u stablu, to cemo koristiti da znamo do kad mozemo da radimo balansiranje
    // po postavci treba da se radi balansiranje na nivou 3 ili vise
    Cvor *balanser = nullptr;

    while (cvor->roditelj) {
        // ovom petljom azuriramo broj potomaka
        // ukoliko se dodaje clan, onda je povecanje 1
        // ukoliko se brise clan, onda je povecanje -1
        // mijenja se broj potomaka za sva cvorove do korijena do mjesta brisanja/dodavanja
        cvor->roditelj->brojPotomaka += povecanje;
        cvor = cvor->roditelj;
        if (brojNivoa >= 3) {// ovaj dio koda sluzi za pamcenje cvora koji je korijen podstabla koje treba balansirati
            if (cvor->desnoDijete && cvor->lijevoDijete) {
                Cvor *lijevoDijete = cvor->lijevoDijete;
                Cvor *desnoDijete = cvor->desnoDijete;
                // ukoliko je jedno podstablo ima barem duplo vise elemenata od drugog podstabla
                if (lijevoDijete->brojPotomaka + 1 > 2 * (desnoDijete->brojPotomaka + 1)
                || desnoDijete->brojPotomaka + 1 > 2 * (lijevoDijete->brojPotomaka + 1)) {
                    balanser = cvor;
                }
            }
        }
        // kada zavrsimo to, prelazimo na nivo iznad/ispod
        brojNivoa--;
    }
    if (balanser && balanser != pocetni) {// ukoliko se nadje cvor koji ispunjava uslove za balansiranje
        // tada balansiramo to podstablo
        balansiraj(balanser);
    }
}

template<class T>
void BalansirajuceStablo<T>::balansiraj(Cvor *balanser) {
    // prvi korak pri balansiranju je formiranje sortiranog niza od podstabla koje se balansira
    std::vector<T> sortirani;
    popuniSortiraniNiz(sortirani, balanser);
    int velicina = sortirani.size();
    // nakon toga kreiramo balansirano stablo iz sortiranog niza
    Cvor* cvor = napraviStabloIzSortiranogNiza(sortirani, 0, velicina - 1);
    if (balanser == korijen) {
        // ukoliko je cvor korijen, onda treba unistiti kompletno stablo, ali to se nece desavati zbog
        // ogranicenja koje ima u postavci da se balansira tek na 3. nivou, ali je ipak ostavljeno da se moze balansirati
        // ukoliko dodje do promjene u postavci
        unistiStablo(korijen);
        korijen = cvor;
    } else {
        // kada umecemo balansirano podstablo, trebamo korijen tog podstabla povezati sa roditeljem
        // s kojim je prethodni korijen bio povezan
        Cvor* roditelj = balanser->roditelj;
        cvor->roditelj = roditelj;
        // ukoliko je korijen podstabla bio lijevo dijete
        // povezati ga kao lijevo dijete
        if (roditelj->lijevoDijete == balanser) {
            roditelj->lijevoDijete = cvor;
        } else {// u suprotnom kao desno
            roditelj->desnoDijete = cvor;
        }
        // posto smo kreirali novo balansirano podstablo, staro nebalansirano podstablo unistavamo
        unistiStablo(balanser);
    }
}

template<class T>
void BalansirajuceStablo<T>::popuniSortiraniNiz(std::vector<T> &sortiraniNiz, BalansirajuceStablo::Cvor *cvor) {
    if (cvor) {// ima istu strukturu kao i inorder ispisivanje, ali ovdje radimo inorder dodavanje clanova u niz
        if (cvor->lijevoDijete) {
            popuniSortiraniNiz(sortiraniNiz, cvor->lijevoDijete);// posjetimo lijevo dijete
        }
        sortiraniNiz.push_back(cvor->vrijednost);// dodatmo roditelja u niz
        if (cvor->desnoDijete) {
            popuniSortiraniNiz(sortiraniNiz, cvor->desnoDijete);// posjetimo desno dijete
        }
    }
}

template<class T>
typename BalansirajuceStablo<T>::Cvor * BalansirajuceStablo<T>::napraviStabloIzSortiranogNiza(const std::vector<T> &sortiraniNiz, int lijevaGranica, int desnaGranica) {
    // ukoliko nam se desi da granice nisu uredu, znaci da smo taj dio niza zavrsili
    if (lijevaGranica > desnaGranica)
        return nullptr;
    // ukoliko se lijeva i desna granica poklapaju, tada treba kreirati jedan cvor
    // ovo je specijalni slucaj, jer se onda ovaj dio niza vise ne moze poloviti
    if (lijevaGranica == desnaGranica)
        return new Cvor(sortiraniNiz[lijevaGranica]);

    // kao sto je kod binarne pretrage, i ovdje polovimo niz na dva dijela i kreiramo cvor od srednjeg clana dijela niza
    int sredina = lijevaGranica + (desnaGranica - lijevaGranica + 1) / 2;
    Cvor* cvor = new Cvor(sortiraniNiz[sredina]);
    cvor->brojPotomaka = desnaGranica - lijevaGranica;
    // azuriramo broj potomaka, jer znamo koliko ima elemenata okolo ovog clana niza
    // Sada imamo dvije polovice ovog dijela niza i trebamo rekurzivno pozvati ovu metodu da isto uradit sa tim dijelovima niza
    // Prvo uzimamo lijevi dio koji predstavlja elemente do sredine dijela niza
    cvor->lijevoDijete = napraviStabloIzSortiranogNiza(sortiraniNiz, lijevaGranica, sredina - 1);
    if (cvor->lijevoDijete) {
        cvor->lijevoDijete->roditelj = cvor;
    }
    // pri tome smo taj novi cvor povezali sa trenutnim kad se rekurzija odmota
    // nakon toga, isto radimo za desno dijete
    cvor->desnoDijete = napraviStabloIzSortiranogNiza(sortiraniNiz, sredina + 1, desnaGranica);
    if (cvor->desnoDijete) {
        cvor->desnoDijete->roditelj = cvor;
    }
    return cvor;// vracamo srednji cvor koji u prvom pozivu ove metode, ustvari, vraca korijen balansiranog podstabla koje stavljamo u nase stablo
}

template<class T>
void BalansirajuceStablo<T>::unistiStablo(BalansirajuceStablo::Cvor *cvor) {
    if (cvor) {
        // Ova metoda radi postoder brisanje elemenata
        // razlog tome je sto se prvo trebaju obrisati sva djeca, pa tek onda roditelj
        unistiStablo(cvor->lijevoDijete);// rekurzivni poziv za lijevo dijete
        unistiStablo(cvor->desnoDijete);
        delete cvor;
    }
}

template<class T>
BalansirajuceStablo<T>::BalansirajuceStablo(BalansirajuceStablo<T> &&stablo) noexcept {
    this->brojElemenata = stablo.brojElemenata;
    this->korijen = stablo.korijen;
    stablo.korijen = nullptr;
    stablo.brojElemenata = 0;
}

template<class T>
BalansirajuceStablo<T> &BalansirajuceStablo<T>::operator=(const BalansirajuceStablo<T> &stablo) {
    if (&stablo != this) {
        unistiStablo(korijen);
        brojElemenata = 0;
        BalansirajuceStablo<T> temp(stablo);
        this->korijen = temp.korijen;
        this->brojElemenata = temp.brojElemenata;
        temp.korijen = nullptr;
        temp.brojElemenata = 0;
    }
    return *this;
}

template<class T>
BalansirajuceStablo<T> &BalansirajuceStablo<T>::operator=(BalansirajuceStablo<T> && stablo)  noexcept {
    if(this != &stablo) {
        unistiStablo(korijen);
        brojElemenata = stablo.brojElemenata;
        korijen = stablo.korijen;
        stablo.korijen = nullptr;
        stablo.brojElemenata = 0;
    }
    return *this;
}

template<class T>
void BalansirajuceStablo<T>::ispisiUTokInorder(std::ostream &tok) {
    inorderUTok(tok, korijen);
}

template<class T>
void BalansirajuceStablo<T>::ispisiPreorder(std::ostream &tok) {
    preorderIspis(tok, korijen);
}

template<class T>
void BalansirajuceStablo<T>::ispisiPostorder(std::ostream &tok) {
    postorderIspis(tok, korijen);
}

template<class T>
void BalansirajuceStablo<T>::preorderIspis(std::ostream &tok, BalansirajuceStablo::Cvor *cvor) {
    if (cvor) {
        tok << cvor->vrijednost << " ";
        preorderIspis(tok, cvor->lijevoDijete);
        preorderIspis(tok, cvor->desnoDijete);
    }
}

template<class T>
void BalansirajuceStablo<T>::postorderIspis(std::ostream &tok, BalansirajuceStablo::Cvor *cvor) {
    if (cvor) {
        postorderIspis(tok, cvor->lijevoDijete);
        postorderIspis(tok, cvor->desnoDijete);
        tok << cvor->vrijednost << " ";
    }
}

template<class T>
typename BalansirajuceStablo<T>::Cvor *BalansirajuceStablo<T>::nadji(const T &x) {
    auto *element = nadjiRoditelja(x);
    if (element && element->vrijednost == x) {
        return element;
    }
    return nullptr;
}


#endif // BALANSIRAJUCE_CPP
