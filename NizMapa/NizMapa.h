//
// Created by dzena on 26/01/2021.
//

#ifndef ASP_VJEZBA7_NIZMAPA_H
#define ASP_VJEZBA7_NIZMAPA_H

#include "../Mapa/Mapa.h"
#include <stdexcept>
#include "../Par/Par.h"

template <typename TipKljuc, typename TipVrijednost>
class MapaIterator;

template <typename TipKljuc, typename TipVrijednost>
class NizMapa : public Mapa<TipKljuc, TipVrijednost> {
    int brEl, maxBrEl;
    Par<TipKljuc, TipVrijednost> **par;
    bool jeLiUMapi(TipKljuc index) const {
        for(int i = 0; i < brEl; i++)
            if(par[i]->kljuc == index)
                return true;
        return false;
    }
    int dajLokacijuUMapi(TipKljuc index) const {
        for(int i = 0; i < brEl; i++)
            if(par[i]->kljuc == index)
                return i;
        return -1;
    }
public:
    NizMapa() : brEl(0), maxBrEl(10), par(new Par<TipKljuc, TipVrijednost>*[maxBrEl]{}) {}

    friend class MapaIterator<TipKljuc, TipVrijednost>;

    ~NizMapa() {
        for(int i = 0; i < brEl; ++i)
            delete par[i];
        delete[] par;
    }

    NizMapa(const NizMapa &m) : brEl(m.brEl), maxBrEl(m.maxBrEl), par(new Par<TipKljuc, TipVrijednost>*[maxBrEl]{}) {
        for(int i = 0; i < brEl; i++)
            par[i] = new Par<TipKljuc, TipVrijednost>(*m.par[i]);
    }

    NizMapa &operator =(const NizMapa &m) {
        if(&m == this) return *this;
        for(int i = 0; i < brEl; ++i)
            delete par[i];
        delete[] par;
        brEl = m.brEl; maxBrEl = m.maxBrEl;
        par = new Par<TipKljuc, TipVrijednost>*[maxBrEl]{};
        for(int i = 0; i < brEl; i++)
            par[i] = new Par<TipKljuc, TipVrijednost>(*m.par[i]);
        return *this;
    }

    TipVrijednost operator[](TipKljuc index) const override {
        int i = dajLokacijuUMapi(index);
        if(i != -1) return par[i]->vrijednost;
        return TipVrijednost();
    }

    TipVrijednost &operator[](TipKljuc index) override {
        if(jeLiUMapi(index)) {
            int i = dajLokacijuUMapi(index);
            return par[i]->vrijednost;
        }
        else {
            if(brEl == maxBrEl) {
                maxBrEl *= 2;
                auto **temp = new Par<TipKljuc, TipVrijednost>*[maxBrEl]{};
                for(int i = 0; i < brEl; i++)
                    temp[i] = par[i];
                temp[brEl] = new Par<TipKljuc, TipVrijednost>{index, TipVrijednost()};
                delete[] par;
                par = temp;
            }
            else {
                par[brEl] = new Par<TipKljuc, TipVrijednost>{index, TipVrijednost()};
            }
            brEl++;
            return par[brEl - 1]->vrijednost;
        }
    }

    int brojElemenata() const override {
        return brEl;
    }

    void obrisi() override {
        for(int i = 0; i < brEl; ++i)
            delete par[i];
        maxBrEl = 10;
        brEl = 0;
        delete[] par;
        par = new Par<TipKljuc, TipVrijednost>*[maxBrEl]{};
    }

    void obrisi(const TipKljuc &k) override {
        if(!jeLiUMapi(k))
            throw std::domain_error("Ne postoji vrijednost sa tim kljucem");
        int j = dajLokacijuUMapi(k);
        delete par[j];
        for(int i = j; i < brEl - 1; i++)
            par[i] = par[i + 1];
        par[brEl] = nullptr;
        brEl--;
    }
};

template <typename TipKljuc, typename TipVrijednost>
class MapaIterator {
    Par<TipKljuc, TipVrijednost> **p;
    int brEl;
    int trenutniEl;
    void test() const {
        if(brEl == 0)
            throw std::domain_error("Mapa je prazna");
    }
public:
    MapaIterator(const NizMapa<TipKljuc, TipVrijednost> &map) : p(map.par), brEl(map.brEl), trenutniEl(0) {}

    TipKljuc trenutniKljuc() const {
        test();
        return p[trenutniEl]->kljuc;
    }

    TipVrijednost trenutnaVrijednost() const {
        test();
        return p[trenutniEl]->vrijednost;
    }

    void prethodni() {
        test();
        if(trenutniEl == 0)
            throw std::domain_error("Nema prethodnog elementa");
        trenutniEl--;
    }

    void sljedeci() {
        test();
        if(trenutniEl == brEl - 1)
            throw std::domain_error("Nema sljedeceg elementa");
        trenutniEl++;
    }

    void pocetak() {
        test();
        trenutniEl = 0;
    }

    void kraj() {
        test();
        trenutniEl = brEl - 1;
    }
};

#endif //ASP_VJEZBA7_NIZMAPA_H
