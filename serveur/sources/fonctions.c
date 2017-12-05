#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonctions.h"
#include <sys/wait.h>
#include <errno.h>

/**
 * La méthode lis le fichier, et remplis le tableau passé en parametre
 * @param {TableauVoyage} pTableau_voyage : tableau à remplir
 * @return {int} renvoie -1 en cas d'erreur, 1 si reussi
 */
TableauVoyage* remplir_tableau_voyage(TableauVoyage *pTableau_voyage){
    FILE * tmp_fichier;
    char * tmpLigne = NULL;
    size_t tmpLongueur = 0;
    ssize_t tmpTailleLigneCourante;
    int tmpCompteur = 0;
    char** tmpLigneCourante;
    char ** tmpTableauHoraireDepart;
    char ** tmpTableauHoraireArrive;
    voyage* tmpVoyage;
    horaire* tmpHoraireDepart;
    horaire* tmpHoraireArrivee;
    enum CodePromo tmpCodePromo;
    
/*On ouvre le fichier*/
    #ifdef TEST
    tmp_fichier = fopen("../Trains.txt", "r");
    #else
    tmp_fichier = fopen("../sources/Trains.txt", "r");
    #endif
    if (tmp_fichier == NULL){
        fprintf(stderr,"ERREUR OUVERTURE IMPOSSIBLE DU FICHIER\n");
	exit(EXIT_FAILURE);
    }
    
    /*On parcours toutes les lignes du fichier*/
    while ((tmpTailleLigneCourante = getline(&tmpLigne, &tmpLongueur, tmp_fichier)) != -1) {
	printf("%s", tmpLigne);
	/*On decoupe la chaine de caractere en tableau*/
	tmpLigneCourante=diviseur_chaine(tmpLigne,';');
	/*Si on a une ligne correcte non null*/
	if(*tmpLigneCourante){		    
	    
            /*On separe l'heure et les minutes pour l'heure de depart et l'heure d'arrivé car on recupere une chaine du type hh:MM*/
	    tmpTableauHoraireDepart = diviseur_chaine(tmpLigneCourante[2],':');
	    tmpTableauHoraireArrive = diviseur_chaine(tmpLigneCourante[3],':');
	    /*On crée le type horaire pour l'horaire d'arrivé et de départ*/
	    tmpHoraireDepart = creer_horaire(atoi(tmpTableauHoraireDepart[0]),atoi(tmpTableauHoraireDepart[1]));
	    tmpHoraireArrivee = creer_horaire(atoi(tmpTableauHoraireArrive[0]),atoi(tmpTableauHoraireArrive[1]));
		
	    /*On crée le code promo associé*/
	    if(tmpLigneCourante[5]==(char*)"REDUC")
		tmpCodePromo=REDUC;
	    else if(tmpLigneCourante[5]==(char*)"SUPPL")
                tmpCodePromo=SUPPL;
	    else
		tmpCodePromo=DEFAULT;	  
            /*On crée un voyage et on l'ajoute au tableau*/
            tmpVoyage=creer_voyage(tmpCompteur+1,tmpLigneCourante[0],tmpLigneCourante[1],*tmpHoraireDepart,*tmpHoraireArrivee,atof(tmpLigneCourante[4]),tmpCodePromo);
	    printf("%s\n\n",tmpVoyage->ville_depart);
	    /* On ajoute le voyage au tableau */
	    pTableau_voyage = ajouter_voyage(pTableau_voyage,tmpVoyage);
	    free(*(tmpLigneCourante));
	    tmpCompteur++;
	    /*printf("%d // %s---%s---%s---%s---%s---%s\n\n", tmpCompteur+1,tmpLigneCourante[0],tmpLigneCourante[1],tmpLigneCourante[2],tmpLigneCourante[3],tmpLigneCourante[4],tmpLigneCourante[5]);*/	
	}
    }
    return pTableau_voyage;
}

voyage * creer_voyage(int pNumero_train, char* pVoyageille_depart, char* pVoyageille_arrive, horaire pHeure_depart, horaire pHeure_arrive, double pPrix, CodePromo pCode_promo) {
    voyage *tmpVoyage = (voyage*)malloc(sizeof(voyage));
    /*Remplissage des données du voyage*/
    tmpVoyage->numero_train = pNumero_train;
    tmpVoyage->ville_depart = pVoyageille_depart;
    tmpVoyage->ville_arrive = pVoyageille_arrive;
    tmpVoyage->heure_depart.heure = pHeure_depart.heure;
    tmpVoyage->heure_depart.minute = pHeure_depart.minute;
    tmpVoyage->heure_arrive.heure = pHeure_arrive.heure;
    tmpVoyage->heure_arrive.minute = pHeure_arrive.minute;
    tmpVoyage->heure_arrive = pHeure_arrive;
    tmpVoyage->prix = pPrix;
    tmpVoyage->code_promo = pCode_promo;
    return tmpVoyage;
}
horaire *creer_horaire(int pHeure, int pMinute){
    horaire *h=(horaire *)malloc(sizeof(horaire));
    /*Verification format heure*/
    if(pHeure<0 || pHeure>24 || pMinute<0 || pMinute>60){
        fprintf(stderr,"ERREUR FORMAT HORAIRE\n");
        exit(-1);
    }
    /*Remplissage des données*/
    h->heure=pHeure;
    h->minute=pMinute;
    return h;
}


TableauVoyage* ajouter_voyage(TableauVoyage *pTab_v, voyage* pVoyage){
    TableauVoyage *tmpNouveauVoyage = (TableauVoyage*)malloc(sizeof(TableauVoyage));
    TableauVoyage *tmpSauvegarde=pTab_v;
    tmpNouveauVoyage->v = *pVoyage;
    tmpNouveauVoyage->suivant = NULL;
    if(pTab_v==NULL) {
        return tmpNouveauVoyage;
    }
    else {
        while(pTab_v->suivant!=NULL)    
            pTab_v=pTab_v->suivant;
        pTab_v->suivant = tmpNouveauVoyage;
    }
    return tmpSauvegarde;
}



/**
 * La méthode découpe la chaine de caractere à partir d'un délimiteur passé en parametre
 * @param {char*} pChaine : Chaine de caractere à traiter
 * @param {char} pDelimiteur : Delimiteur à appliquer sur la chaine
 * @return {char**} tableau de chaine de caracteres separé par le delimiteur et se finissant par (char*)0
 */
char** diviseur_chaine(char* pChaine, const char pDelimiteur)
{
    char** tmpResultat = 0;
    size_t tmpCompteur = 0;
    char* tmpChaine = pChaine;
    char* tmpLigneCourante = 0;
    char tmpDelimiteur[2];
    size_t tmpId;
    char* token;
    tmpDelimiteur[0] = pDelimiteur;
    tmpDelimiteur[1] = 0;

    /*On compte le nombre de chaine séparé par un délimiteur*/
    while (*tmpChaine)
    {
        if (pDelimiteur == *tmpChaine)
        {
            tmpCompteur++;
            tmpLigneCourante = tmpChaine;
        }
        tmpChaine++;
    }

    /*Si le pointeur se trouve dans la chaine il reste un mot à prendre en compte*/
    tmpCompteur += tmpLigneCourante < (pChaine + strlen(pChaine) - 1);
    /*Puis on ajoute un pour le mot 0*/
    tmpCompteur++;
    /*On alloue la mémoire au tableau resultat celon le nombre de mots*/
    tmpResultat = malloc(sizeof(char*) * tmpCompteur);

    /*MAYBE USELESS if (tmpResultat) ##############################################################
      {*/
    tmpId  = 0;
    token = strtok(pChaine, tmpDelimiteur);

    /*tant qu'on a des mots on remplit notre tableau*/
    while (token)
    {
        *(tmpResultat + tmpId++) = strdup(token);
        token = strtok(0, tmpDelimiteur);
    }
    /*on ajoute le mot 0*/
    *(tmpResultat + tmpId) = 0;
    /*}#############################################################################################*/
   
    return tmpResultat;
}



void afficher_table(TableauVoyage *tableauVoyage){
    if(tableauVoyage==NULL)
        puts("tableau vide");
    else {
        printf("num : %d\n",tableauVoyage->v.numero_train);
        while(tableauVoyage->suivant!=NULL){
            tableauVoyage=tableauVoyage->suivant;
            printf("num : %d\n",tableauVoyage->v.numero_train);
        }
    }
}

void recupere_mort_fils(){
	wait(NULL);
}

void erreur(char *p_fonction){
	perror(p_fonction);
	exit(errno);
}

void usage(char *p_nom_programme){
	fprintf(stderr,"%s prend 1 paramètre, c'est le numéro de port que prendra la socket du serveur\n", p_nom_programme);
	exit(-1);
}
