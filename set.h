//
//  set.h
//  set
//
//  Jean Goulet  2015-11-13.
//  Modification 2015-11-26
//  Copyleft (c) 2015 UdeS
//

#ifndef set_set_h
#define set_set_h

#include <vector>
#include <string>
#include <cassert>
#include <cmath>

template <typename TYPE>
class set{
public:
    class iterator;
    friend class iterator;
private:
    struct noeud{
        TYPE CONTENU;
        noeud *PARENT,*GAUCHE,*DROITE;
        size_t POIDS;
        noeud(const TYPE& c,noeud* PAR,noeud*GAU=nullptr,noeud*DRO=nullptr):CONTENU(c),PARENT(PAR),GAUCHE(GAU),DROITE(DRO),POIDS(1){}
        noeud():PARENT(nullptr),GAUCHE(nullptr),DROITE(nullptr),POIDS(0){}
        ~noeud(){PARENT=GAUCHE=DROITE=nullptr;}
    };
    noeud *RACINE;
    const noeud *DEBUT;
    noeud APRES;
    size_t SIZE;
    //fonctions privees
    void initialiser();
    void vider(noeud*&);
    void copier(noeud*,noeud*&,noeud*);
    
    bool insert(const TYPE&,noeud*&,iterator&);
    bool ajoute_gauche(const TYPE&,noeud*&,iterator&);
    bool ajoute_droite(const TYPE&,noeud*&,iterator&);
    
    bool erase(const TYPE&,noeud*&,noeud*&);
    bool enleve_gauche(const TYPE&,noeud*&,noeud*&);
    bool enleve_droite(const TYPE&,noeud*&,noeud*&);
    bool eliminer(noeud*&,noeud*&);
    
    void transferer_vers_la_droite(noeud*&);
    void transferer_vers_la_gauche(noeud*&);
    void rotation_gauche_droite(noeud*&);
    void rotation_droite_gauche(noeud*&);
    
    void afficher(set<TYPE>::noeud*,int,std::vector<std::string>&,double&,int&)const;
    void afficher_barres(std::vector<std::string>&,int)const;
public:
    set();
    ~set(){clear();}
    set(const set&);
    set& operator=(const set&);
    void swap(set&);
    
    size_t size()const{return SIZE;}
    bool empty()const{return SIZE==0;}
    void clear();
    
    iterator find(const TYPE&)const;
    iterator lower_bound(const TYPE&)const;
    std::pair<iterator,bool> insert(const TYPE&);
    size_t erase(const TYPE&);
    iterator erase(iterator);
    
    //fonction d'iteration
    iterator begin()const{return iterator(DEBUT);}
    iterator end()const{return iterator(APRES.DROITE);}
    
    //fonction de mise au point
    void afficher()const;
};

template <typename TYPE>
class set<TYPE>::iterator{
private:
    const noeud* POINTEUR;
    static void avancer(const noeud*&);
    static void reculer(const noeud*&);
    friend class set<TYPE>;
public:
    iterator(const noeud*p=nullptr):POINTEUR(p){}
    iterator(const iterator&)=default;
    iterator& operator=(const iterator&)=default;
    const TYPE& operator*()const{return POINTEUR->CONTENU;}
    const TYPE* operator->()const{return &(POINTEUR->CONTENU);}
    iterator& operator++(){avancer(POINTEUR);return *this;}                         //++i
    iterator operator++(int){iterator copie(*this);avancer(POINTEUR);return copie;} //i++
    iterator& operator--(){reculer(POINTEUR);return *this;}                         //--i
    iterator operator--(int){iterator copie(*this);reculer(POINTEUR);return copie;} //i--
    bool operator==(const iterator& dr)const{return POINTEUR==dr.POINTEUR;}
    bool operator!=(const iterator& dr)const{return POINTEUR!=dr.POINTEUR;}
};


///////////////////////////////////////////////////////////////////////////
// fonctions privees

template <typename TYPE>
void set<TYPE>::initialiser(){
    DEBUT=RACINE=nullptr;
    SIZE=0;
    APRES.PARENT=nullptr;
    APRES.GAUCHE=nullptr;
    APRES.DROITE=&APRES;
}

template <typename TYPE>
void set<TYPE>::vider(noeud*& p){
    if(p==nullptr)return;
    vider(p->GAUCHE);
    vider(p->DROITE);
    delete p;
}

template <typename TYPE>
void set<TYPE>::copier(noeud* source,noeud*& dest,noeud* parent){
    if(source==nullptr)return;
    dest=new noeud(*source);
    dest->PARENT=parent;
    dest->POIDS=source->POIDS;
    copier(source->DROITE,dest->DROITE,dest);
    copier(source->GAUCHE,dest->GAUCHE,dest);
}


///////////////////////////////////////////////////////////////////////////
// insertion


template <typename TYPE>
bool set<TYPE>::insert(const TYPE& c,noeud*& p,iterator& r){
    if(c<p->CONTENU){
        if(ajoute_gauche(c,p,r)){
            ++(p->POIDS);
            transferer_vers_la_droite(p);
            return true;
        }
    }
    else if(p->CONTENU<c){
        if(ajoute_droite(c,p,r)){
            ++(p->POIDS);
            transferer_vers_la_gauche(p);
            return true;
        }
    }
    else r=iterator(p);
    return false;
}

template <typename TYPE>
bool set<TYPE>::ajoute_gauche(const TYPE& c,noeud*& p,iterator& r){
    if(p->GAUCHE==nullptr){  //nouvelle feuille
        p->GAUCHE=new noeud(c,p);
        ++SIZE;
        r=iterator(p->GAUCHE);
        if(p==DEBUT)DEBUT=p->GAUCHE;  //nouveau premier element
        return true;
    }
    else                    //ajout general a gauche
        return insert(c,p->GAUCHE,r);
}

template <typename TYPE>
bool set<TYPE>::ajoute_droite(const TYPE& c,noeud*& p,iterator& r){
    if(p->DROITE==nullptr){  //nouvelle feuille
        p->DROITE=new noeud(c,p);
        ++SIZE;
        r=iterator(p->DROITE);
        return true;
    }
    else  //ajout general a droite
        return insert(c,p->DROITE,r);
}

///////////////////////////////////////////////////////////////////////////
// elimination

template <typename TYPE>
bool set<TYPE>::erase(const TYPE& c,noeud*& p,noeud*& aRemplacer){
    if(c<p->CONTENU)
        return enleve_gauche(c,p,aRemplacer);
    else if(p->CONTENU<c)
        return enleve_droite(c,p,aRemplacer);
    else{
        aRemplacer=p;
        return enleve_gauche(c,p,aRemplacer);
    }
}

template <typename TYPE>
bool set<TYPE>::enleve_gauche(const TYPE& c,noeud*& p,noeud*& aRemplacer){
    if(p->GAUCHE==nullptr)
        return eliminer(p,aRemplacer);
    else if(erase(c,p->GAUCHE,aRemplacer)){
        --(p->POIDS);
        transferer_vers_la_gauche(p);
        return true;
        }
    else
        return false;
}

template <typename TYPE>
bool set<TYPE>::enleve_droite(const TYPE& c,noeud*& p,noeud*& aRemplacer){
    if(p->DROITE==nullptr)
        return eliminer(p,aRemplacer);
    else if(erase(c,p->DROITE,aRemplacer)){
        --(p->POIDS);
        transferer_vers_la_droite(p);
        return true;
    }
    else
        return false;
}

template <typename TYPE>
bool set<TYPE>::eliminer(noeud*& p,noeud*& aRemplacer){
    if(aRemplacer!=nullptr){
        bool x=DEBUT==p;
        if(x)iterator::avancer(DEBUT);
        aRemplacer->CONTENU=p->CONTENU;
        aRemplacer=p;
        if(p->GAUCHE==nullptr)p=p->DROITE;else p=p->GAUCHE;
        if(p!=nullptr)p->PARENT=aRemplacer->PARENT;
        --SIZE;
        delete aRemplacer;
        aRemplacer=nullptr;
        return true;
        }
    else
        return false;
}



///////////////////////////////////////////////////////////////////////////
// fonctions publiques

template <typename TYPE>
set<TYPE>::set(){
    initialiser();
}

template <typename TYPE>
set<TYPE>::set(const set& source):set(){
    copier(source.RACINE,RACINE,nullptr);
    SIZE=source.size();
}

template <typename TYPE>
set<TYPE>& set<TYPE>::operator=(const set& source){
    if(this==&source)return *this;
    set<TYPE> copie(source);
    this->swap(copie);
    return *this;
}

template <typename TYPE>
void set<TYPE>::swap(set& source){
    std::swap(SIZE,source.SIZE);
    std::swap(RACINE,source.RACINE);
    std::swap(DEBUT,source.DEBUT);
    std::swap(APRES.GAUCHE,source.APRES.GAUCHE);
}

template <typename TYPE>
void set<TYPE>::clear(){
    vider(RACINE);
    initialiser();
}

template <typename TYPE>
typename set<TYPE>::iterator set<TYPE>::find(const TYPE& c)const{
    iterator retour=lower_bound(c);
    if(retour==end() || c<*retour)return end();
    return retour;
}

template <typename TYPE>
std::pair<typename set<TYPE>::iterator,bool> set<TYPE>::insert(const TYPE& c){
    if(SIZE==0){  //arbre vide
        DEBUT=APRES.GAUCHE=RACINE=new noeud(c,&APRES);
        SIZE=1;
        return std::make_pair(begin(),true);
        }
    iterator retour;
    bool valeur=insert(c,RACINE,retour);
    APRES.GAUCHE=RACINE;
    return std::make_pair(retour,valeur);
}

template <typename TYPE>
size_t set<TYPE>::erase(const TYPE& c){
    if(SIZE!=0){
        noeud* aRemplacer=nullptr;
        if(erase(c,RACINE,aRemplacer)){
            APRES.GAUCHE=RACINE;
            if(RACINE!=nullptr)RACINE->PARENT=APRES.DROITE;
            return 1;
        }
    }
    return 0;
}

template <typename TYPE>
typename set<TYPE>::iterator set<TYPE>::erase(iterator i){
    assert(i!=end());
    TYPE c=*i;
    erase(c);
    return lower_bound(c);
}


///////////////////////////////////////////////////////////////////////////
// fonctions de mise au point
template <typename TYPE>
void set<TYPE>::afficher()const{
    using namespace std;
    double total=0.;
    int max=0;
    cout<<"-------------------------------"<<endl;
    vector<string> barres;
    barres.push_back("    ");
    afficher(RACINE,1,barres,total,max);
    total=total/SIZE;
    cout<<SIZE<<" element";if(SIZE>1)cout<<"s";cout<<endl;
    cout<<"log("<<SIZE<<"): "<<log2(double(SIZE))<<endl;
    cout<<"hauteur moyenne: "<<total<<endl;
    cout<<"hauteur maximale: "<<max<<endl;
    cout<<"-------------------------------"<<endl;
}


template <typename TYPE>
void set<TYPE>::afficher(set<TYPE>::noeud* p,int niveau,std::vector<std::string>& barres,double& total,int& max)const{
    using namespace std;
    if(p==0)return;
    total+=niveau;
    if(niveau>max)max=niveau;
    if(niveau>=barres.size())barres.push_back("    ");
    
    afficher(p->DROITE,niveau+1,barres,total,max);
    
    //si on est un enfant de gauche arreter les barres a ce niveau
    if(p->PARENT!=0 && p->PARENT->GAUCHE==p)barres[niveau-1]="    ";
    
    //cout<<niveau;
    afficher_barres(barres,niveau);
    cout<<"--->";
    cout<<p->CONTENU<<" ("<<p->POIDS;
    //cout<<", "<<p;
    //cout<<", par="<<p->PARENT;
    //cout<<", gau="<<p->GAUCHE;;
    //cout<<", dro="<<p->DROITE;
    cout<<")"<<endl;
    
    //si on est un enfant de droite barre a mon niveau
    if(p->PARENT->DROITE==p)barres[niveau-1]="   |";
    
    //si on a un enfant a gauche mettre des barres apres
    if(p->GAUCHE!=0)barres[niveau]="   |";
    else barres[niveau]="    ";
    
    //cout<<niveau;
    afficher_barres(barres,niveau+1);
    cout<<endl;
    
    afficher(p->GAUCHE,niveau+1,barres,total,max);
}

template <typename TYPE>
void set<TYPE>::afficher_barres(std::vector<std::string>& barres,int n)const{
    for(int i=0;i<n;++i)std::cout<<barres[i];
}

#include "set2.h"

#endif
