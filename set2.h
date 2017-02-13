//
//  set2.h
//  SkipList
//
//  Jean Goulet 2015-10-29
//  Bastien Chapusot	15 149 590
//  Eddy Ouedraogo	14 178 490
//  2015-11-05

#ifndef SkipList_set2_h
#define SkipList_set2_h

template <typename T,typename compare >
typename set<T,compare>::iterator set<T,compare>::lower_bound(const T& t){
    iterator it;
    static compare plus_petit_que;
    
    size_t niv = NBNIV-1;
    for(it.POINTEUR=AVANT.SUIV[niv]; it!= end();) {
        if(plusPetit(it.POINTEUR,t)) {
            it.POINTEUR = it.POINTEUR->SUIV[niv];
        } else {
            if(!plusPetit(t,it.POINTEUR)) {
                break;
            } else {
                niv--;
            }
        }
    }

    return it;
}

template <typename T,typename compare >
typename set<T,compare>::iterator set<T,compare>::erase(iterator it){
	int nivASuppr = it.POINTEUR->SUIV.size();
	cellule* tmpPrec;
	cellule* tmpSuiv;

	for(int i=nivASuppr-1; i>=0; --i) {
		tmpPrec = it.POINTEUR->PREC[i];
		tmpSuiv = it.POINTEUR->SUIV[i];
		// on isole la cellule a supprimer
		tmpPrec->SUIV[i] = tmpSuiv;
		tmpSuiv->PREC[i] = tmpPrec;

		// doit-on supprimer un niveau?
		if(tmpPrec->PREC[i] == nullptr && tmpSuiv->SUIV[i] == nullptr) {
			AVANT.SUIV.pop_back();
			APRES.PREC.pop_back();
			NBNIV--;
		}
	}
	delete it.POINTEUR;
	SIZE--;

    return iterator(tmpSuiv);
}

template <typename T,typename compare >
const set<T,compare>& set<T,compare>::operator=(const set& src){
	// Copie de la taille, du maillage de niveaux et des cellules
	clear();
	
	for(iterator it=src.begin(); it!=src.end(); ++it) {
		insert(it.POINTEUR->CONTENU);
	}

    return *this;
}

template <typename T,typename compare >
void set<T,compare>::clear(){
	for(cellule* tmp=AVANT.SUIV[0]; tmp->SUIV[0]!=nullptr; ) {
		tmp = tmp->SUIV[0];		
		delete tmp->PREC[0];	
	}

	initialiser();
	
}


#endif
