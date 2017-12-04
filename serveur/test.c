#include <stdio.h>
#include <stdlib.h>
#include "fonctions.h"


int main(){
    TableauVoyage *tmpTableau=NULL;
    /* TODO :: Ajouter le tableau tmpTableau*/
    tmpTableau=remplir_tableau_voyage(tmpTableau);
    afficher_table(tmpTableau);

}


