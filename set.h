/*
 *  SkipList.h
 *
 *  Jean Goulet 2015-10-26
 *
 */

#pragma once

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <random>
#include <chrono>
//#include "/Users/goulet/CPPutil/divers.h"

using std::string;
using std::cout;
using std::endl;
using std::vector;

template <typename T,typename compare=std::less<T> >
class set{
private:
	struct cellule{
		T CONTENU;
        vector<cellule*> PREC,SUIV;
		cellule(T=T(),cellule* =nullptr,cellule* =nullptr);
		};
	size_t NBNIV;
	size_t SIZE;
	cellule AVANT,APRES;
	
	void initialiser();
    bool plusPetit(const T&,cellule*)const;
    bool plusPetit(cellule*,const T&)const;
	bool egaux(const T&,cellule*)const;
    size_t tirer_couches()const;
public:
    class iterator;

	set();
	~set();
	set(const set&);
	const set& operator=(const set&);
	
	size_t size()const;
	bool empty()const;
    
	size_t count(const T&)const;
    iterator find(const T&);
    iterator lower_bound(const T&);
    
	iterator insert(const T&);
    iterator insert(iterator,const T&);
	size_t erase(const T&);
	iterator erase(iterator);
	void clear();
	
	iterator begin()const;
	iterator end()const;
		
	void afficher(string="")const;
};

/////////////////////////////////////////////////
// cellule

template <typename T,typename compare >
set<T,compare>::cellule::cellule(T t,cellule* pr,cellule* su){
    CONTENU=t;
    PREC.push_back(pr);
    SUIV.push_back(su);
}


/////////////////////////////////////////////////
// iterator

template <typename T,typename compare >
class set<T,compare>::iterator{
private:
    cellule* POINTEUR;
public:
    friend class set<T,compare>;
    iterator(cellule*p=nullptr):POINTEUR(p){}
    const T& operator*()const{return POINTEUR->CONTENU;}
    iterator operator++();     //++i
    iterator operator++(int);  //i++
    iterator operator--();     //--i
    iterator operator--(int);  //i--
    bool operator==(const iterator&i2)const{return POINTEUR==i2.POINTEUR;}
    bool operator!=(const iterator&i2)const{return POINTEUR!=i2.POINTEUR;}
};


template <typename T,typename compare >
typename set<T,compare>::iterator set<T,compare>::iterator::operator++(){
    POINTEUR=POINTEUR->SUIV[0];
    return *this;
}

template <typename T,typename compare >
typename set<T,compare>::iterator set<T,compare>::iterator::operator++(int){
    iterator r(POINTEUR);
    POINTEUR=POINTEUR->SUIV[0];
    return r;
}

template <typename T,typename compare >
typename set<T,compare>::iterator set<T,compare>::iterator::operator--(){
    POINTEUR=POINTEUR->PREC[0];
    return *this;
}

template <typename T,typename compare >
typename set<T,compare>::iterator set<T,compare>::iterator::operator--(int){
    iterator r(POINTEUR);
    POINTEUR=POINTEUR->PREC[0];
    return r;
}

/////////////////////////////////////////////////
// set
// fonctions privees

template <typename T,typename compare >
void set<T,compare>::initialiser(){
    using namespace std;
	NBNIV=1;
	SIZE=0;
    AVANT.PREC.clear();
    AVANT.SUIV.clear();
    APRES.PREC.clear();
    APRES.SUIV.clear();
    AVANT.SUIV.push_back(&APRES);
    APRES.PREC.push_back(&AVANT);
}

template <typename T,typename compare >
bool set<T,compare>::plusPetit(const T& t,cellule* p)const{
    //verifie que t < contenu de *p
    static compare plus_petit_que;
    if(p->SUIV.size()==0)return true;  //p est la fin
    if(p->PREC.size()==0)return false; //p avant le debut
    return plus_petit_que(t,p->CONTENU);   //sinon utiliser le comparateur
}

template <typename T,typename compare >
bool set<T,compare>::plusPetit(cellule* p,const T& t)const{
    //verifie que contenu de *p < t
    static compare plus_petit_que;
    if(p->SUIV.size()==0)return false; //p est la fin
    if(p->PREC.size()==0)return true;  //p est avant le debut
    return plus_petit_que(p->CONTENU,t);   //sinon utiliser le comparateur
}

template <typename T,typename compare >
bool set<T,compare>::egaux(const T& t,cellule* p)const{
    //verifie que t == *p
    static compare plus_petit_que;
    if(p->SUIV.size()==0)return false; //p est la fin
    if(p->PREC.size()==0)return false; //p est avant le debut
    if(plus_petit_que(*p->CONTENU,t))return false;   //vient avant
    if(plus_petit_que(t,*p->CONTENU))return false;   //vient apres
    return true;  //ni plus petit, ni plus grand
	}

template <typename T,typename compare >
size_t set<T,compare>::tirer_couches()const{
    //tirer au hasard le nombre de couches dans [1 et NBNIV+1]
    static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::minstd_rand0 generator(static_cast<unsigned int>(seed));
    size_t i=1;
    auto g=generator();
    for(;i<=NBNIV;++i){
        if(g%2==0)break;
        g/=2;
    }
    return i;
}


/////////////////////////////////////////////////
// set
// fonctions publiques

template <typename T,typename compare >
set<T,compare>::set(){
	initialiser();
	}

template <typename T,typename compare >
set<T,compare>::~set(){
	clear();
	}

template <typename T,typename compare >
set<T,compare>::set(const set& src){
	initialiser();
	*this=src;
	}


template <typename T,typename compare >
size_t set<T,compare>::size()const{
	return SIZE;
}

template <typename T,typename compare >
bool set<T,compare>::empty()const{
	return SIZE==0;
}

template <typename T,typename compare >
size_t set<T,compare>::count(const T& t)const{
    auto it=find(t);
    if(it==end())return 0;
    else return 1;
	}

template <typename T,typename compare >
typename set<T,compare>::iterator set<T,compare>::find(const T& t){
    auto it=lower_bound(t);
    if(plusPetit(t,it.POINTEUR))return end();
    return it;
}

template <typename T,typename compare >
typename set<T,compare>::iterator set<T,compare>::insert(const T& t){
    iterator it=lower_bound(t);
    if(plusPetit(t,it.POINTEUR))it=insert(it,t);
    return it;
}

template <typename T,typename compare >
typename set<T,compare>::iterator set<T,compare>::insert(iterator it,const T& t){
    //verifier que l'on est a la bonne place
    iterator av=it;--av;
    cellule* pa=av.POINTEUR;
    cellule* pi=it.POINTEUR;
    cellule* p;
    size_t i;
    if(plusPetit(t,pa) || plusPetit(pi,t))return insert(t);
    size_t nbc=tirer_couches();
    if(nbc>NBNIV){
        //ajouter une nouvelle couche
        AVANT.SUIV.push_back(&APRES);
        APRES.PREC.push_back(&AVANT);
        ++NBNIV;
        }
    //inserer la nouvelle cellule entre pa et pi
    pi->PREC[0]=pa->SUIV[0]=p=new cellule(t,pa,pi);
    ++SIZE;
    //creer le bon nombre d'entrees dans la nouvelle cellule
    p->PREC.resize(nbc);
    p->SUIV.resize(nbc);
    //inserer la nouvelle cellule dans les couches
    for(i=1;i<nbc;++i){
        //avancer au besoin pi vers le prochain
        while(pi->SUIV.size()==i)pi=pi->SUIV.back();
        pa=pi->PREC[i];
        pa->SUIV[i]=p;
        pi->PREC[i]=p;
        p->SUIV[i]=pi;
        p->PREC[i]=pa;
        }
    it.POINTEUR=p;
    return it;
}

template <typename T,typename compare >
size_t set<T,compare>::erase(const T& t){
    iterator it=find(t);
    if(it==end())return 0;
    erase(it);
    return 1;
	}


/////////////////////////////////////////////////
// iteration

template <typename T,typename compare >
typename set<T,compare>::iterator set<T,compare>::begin()const{
    return iterator(AVANT.SUIV[0]);
}

template <typename T,typename compare >
typename set<T,compare>::iterator set<T,compare>::end()const{
    iterator it(APRES.PREC[0]->SUIV[0]);
    return it;
}

/////////////////////////////////////////////////
// afficher

template <typename T,typename compare >
void set<T,compare>::afficher(string st)const{
    using namespace std;
    cout<<endl;
	cout<<"------------------------------------------"<<st<<endl;
    
    string une_clef,lignes,clefs;
    T elem;
    cellule *p;
    lignes=to_string(SIZE)+" elements";
	for(size_t nb=NBNIV;nb>0;){
        string clefs=" ";
        nb--;
		//afficher toute la couche nb
        cout<<lignes<<endl;
        lignes="|";
        for(auto it=begin();it!=end();++it){
            if(it.POINTEUR->SUIV.size()>nb){
                p=it.POINTEUR;
                elem=p->CONTENU;
                une_clef="---"+to_string(elem);
                clefs+=une_clef.substr(une_clef.size()-4,4);
                lignes+="   |";
                }
            else{
                clefs+="----";
                lignes+="    ";
                }
            }
        clefs+="--";
        cout<<clefs<<endl;
        lignes+="  |";
		}
    cout<<"------------------------------------------"<<st<<endl;
	cout<<endl<<endl;
	}


////////////////////////////////////////////////////////////

#include "set2.h"
