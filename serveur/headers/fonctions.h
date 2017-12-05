#ifndef FONCTIONS_H
#define FONCTIONS_H

#include "types.h"

/*fonctions "constructeur" de nos types*/
voyage * creer_voyage(int pNumero_train, char* pVoyageille_depart, char* pVoyageille_arrive, horaire pHeure_depart, horaire pHeure_arrive, double pPrix, CodePromo pCode_promo);
horaire *creer_horaire(int pHeure, int pMinute);

/*fonctions qui manipulent TableauVoyage*/
TableauVoyage* ajouter_voyage(TableauVoyage *pTab_v, voyage* pVoyage);
TableauVoyage* remplir_tableau_voyage(TableauVoyage *pTableau_voyage);

/*fonctions d'usages*/
char** diviseur_chaine(char* pChaine, const char pDelimiteur);
void afficher_table(TableauVoyage *tableauVoyage);
void usage(char *p_nom_programme);
void erreur(char *p_fonction);
void recupere_mort_fils();

#endif