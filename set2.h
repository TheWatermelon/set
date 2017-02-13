//
//  set2.h
//  WBT
//
//  Jean Goulet on 2015-11-13.
//  Copyleft (c) 2015 UdeS
//
//	Bastien Chapusot	15 149 590
//	Francis Trepanier	15 102 504
//

#ifndef WBT_set2_h
#define WBT_set2_h

#include <iostream>

///////////////////////////////////////////////////////////////////////////
// lower_bound

template <typename TYPE>
typename set<TYPE>::iterator set<TYPE>::lower_bound(const TYPE& c)const{
    //localise le premier element non inferieur a c
    if(SIZE == 0) return end();
    
    noeud* doigt=RACINE;
    while(1) {
    	if(c < doigt->CONTENU) {
    		if(doigt->GAUCHE == nullptr) {
    			return iterator(doigt);
    		} else {
    			doigt = doigt->GAUCHE;
    		}
    	} else if(!(doigt->CONTENU < c)) {
    		return iterator(doigt);
    	} else {
    		if(doigt->DROITE == nullptr) {
    			return end();
    		} else {
    			doigt = doigt->DROITE;
    		}
    	}
    }
    return end();
}

///////////////////////////////////////////////////////////////////////////
// fonctions supplementaires de la class iterator

template <typename TYPE>
void set<TYPE>::iterator::avancer(const noeud*& p){
    //AVANCER le pointeur p vers le prochain noeud en inordre
    //cette fonction fait automatiquement exception
    //si on avance au-dela de la fin
    if(p->DROITE == nullptr) {
    	while(p != p->PARENT->GAUCHE) {
    		p = p->PARENT;
    	}
    	p = p->PARENT;
    } else {
    	p = p->DROITE;
    	while(p->GAUCHE != nullptr) {
    		p = p->GAUCHE;
    	}
    }
}

template <typename TYPE>
void set<TYPE>::iterator::reculer(const noeud*& p){
    //RECULER le pointeur p vers le noeud precedent en inordre
    //cette fonction fait automatiquement exception
    //si on recule du debut
	if(p->GAUCHE == nullptr) {
    	while(p != p->PARENT->DROITE) {
    		p = p->PARENT;
    	}
    	p = p->PARENT;
    } else {
    	p = p->GAUCHE;
    	while(p->DROITE != nullptr) {
    		p = p->DROITE;
    	}
    }
}


///////////////////////////////////////////////////////////////////////////
// equilibre


template <typename TYPE>
void set<TYPE>::transferer_vers_la_droite(noeud*& p){
	if(p->GAUCHE == nullptr) return;
	if(p->GAUCHE->POIDS < 4) return;
	size_t tmpPoidsDroite = (p->DROITE == nullptr) ? 0 : p->DROITE->POIDS;
	size_t tmpPoidsGauche = p->GAUCHE->POIDS;
	if(tmpPoidsGauche > 3*tmpPoidsDroite) {
		if(p->GAUCHE->GAUCHE != nullptr || p->GAUCHE->DROITE != nullptr) {
			tmpPoidsDroite = (p->GAUCHE->DROITE == nullptr) ? 0 : p->GAUCHE->DROITE->POIDS;
			tmpPoidsGauche = (p->GAUCHE->GAUCHE == nullptr) ? 0 : p->GAUCHE->GAUCHE->POIDS;
			if(tmpPoidsGauche < tmpPoidsDroite) {
				rotation_droite_gauche(p->GAUCHE);
			}
		}
		rotation_gauche_droite(p);
	}
}

template <typename TYPE>
void set<TYPE>::transferer_vers_la_gauche(noeud*& p){
	if(p->DROITE == nullptr) return;
	if(p->DROITE->POIDS < 4) return;
	size_t tmpPoidsGauche = (p->GAUCHE == nullptr) ? 0 : p->GAUCHE->POIDS;
	size_t tmpPoidsDroite = p->DROITE->POIDS;
	if(tmpPoidsDroite > 3*tmpPoidsGauche) {
		if(p->DROITE->GAUCHE != nullptr || p->DROITE->DROITE != nullptr) {
			tmpPoidsGauche = (p->DROITE->GAUCHE == nullptr) ? 0 : p->DROITE->GAUCHE->POIDS;
			tmpPoidsDroite = (p->DROITE->DROITE == nullptr) ? 0 : p->DROITE->DROITE->POIDS;
			if(tmpPoidsGauche < tmpPoidsDroite) {
				rotation_gauche_droite(p->DROITE);
			}
		}
		rotation_droite_gauche(p);
	}
}

template <typename TYPE>
void set<TYPE>::rotation_gauche_droite(noeud*& p){
	if(p->GAUCHE == nullptr) return;
	// MAJ du poids
	size_t tmpPoids = p->GAUCHE->POIDS;
	p->GAUCHE->POIDS = p->POIDS;
	p->POIDS = tmpPoids;

	p->GAUCHE->PARENT = p->PARENT;
	p->PARENT = p->GAUCHE;
	p->GAUCHE = p->PARENT->DROITE;
	if(p->PARENT->DROITE != nullptr)
		p->PARENT->DROITE->PARENT = p;
	p->PARENT->DROITE = p;
	
	if(p->PARENT->PARENT->GAUCHE == p) {
		p->PARENT->PARENT->GAUCHE = p->PARENT;
	} else if (p->PARENT->PARENT->DROITE == p) {
		p->PARENT->PARENT->DROITE = p->PARENT;
	}

	if(p == RACINE) {
		RACINE = p->PARENT;
	}
}

template <typename TYPE>
void set<TYPE>::rotation_droite_gauche(noeud*& p){
	if(p->DROITE == nullptr) return;
	// MAJ du poids
	size_t tmpPoids = p->DROITE->POIDS;
	p->DROITE->POIDS = p->POIDS;
	p->POIDS = tmpPoids;

	p->DROITE->PARENT = p->PARENT;
	p->PARENT = p->DROITE;
	p->DROITE = p->PARENT->GAUCHE;
	if(p->PARENT->GAUCHE != nullptr)
		p->PARENT->GAUCHE->PARENT = p;
	p->PARENT->GAUCHE = p;

	if(p->PARENT->PARENT->GAUCHE == p) {
		p->PARENT->PARENT->GAUCHE = p->PARENT;
	} else if (p->PARENT->PARENT->DROITE == p) {
		p->PARENT->PARENT->DROITE = p->PARENT;
	}

	if(p == RACINE) {
		RACINE = p->PARENT;
	}
}



#endif
