#include <stdio.h>
#include <stdlib.h>
//Util pour traiter les lignes du fichier
#include <string.h>
#include <assert.h>

// Structure pour les codes promos
typedef enum CodePromo CodePromo;
enum CodePromo {
	REDUC,SUPPL,DEFAULT
};

// Structure de l'heure de voyage
typedef struct {
	int heure;
	int minute;
}horaire;

// Structure du voyage
typedef struct{
	int numero_train;
	char* ville_depart;
	char* ville_arrive;
	horaire heure_depart;
	horaire heure_arrive;
	double prix;
	CodePromo code_promo;

}voyage;


// Tableau contenant la liste des voyages
struct TableauVoyage{
    voyage v;
    struct TableauVoyage* suivant;
};
typedef struct TableauVoyage TableauVoyage;





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
    voyage * tmpVoyage= pVoyage;
    tmpNouveauVoyage->v = *tmpVoyage;
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
 * @return {char**} Liste de liste separé par le delimiteur
 */
char** diviseur_chaine(char* pChaine, const char pDelimiteur)
{
    char** tmpResultat     = 0;
    size_t tmpCompteur     = 0;
    char* tmpChaine        = pChaine;
    char* tmpLigneCourante       = 0;
    char tmpDelimiteur[2];
    tmpDelimiteur[0] = pDelimiteur;
    tmpDelimiteur[1] = 0;

    while (*tmpChaine)
    {
        if (pDelimiteur == *tmpChaine)
        {
            tmpCompteur++;
            tmpLigneCourante = tmpChaine;
        }
        tmpChaine++;
    }

    tmpCompteur += tmpLigneCourante < (pChaine + strlen(pChaine) - 1);

    tmpCompteur++;

    tmpResultat = malloc(sizeof(char*) * tmpCompteur);

    if (tmpResultat)
    {
        size_t tmpId  = 0;
        char* token = strtok(pChaine, tmpDelimiteur);

        while (token)
        {
            assert(tmpId < tmpCompteur);
            *(tmpResultat + tmpId++) = strdup(token);
            token = strtok(0, tmpDelimiteur);
        }
        assert(tmpId == tmpCompteur - 1);
        *(tmpResultat + tmpId) = 0;
    }

    return tmpResultat;
}




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


    tmp_fichier = fopen("Trains.txt", "r");
    if (tmp_fichier == NULL)
	exit(EXIT_FAILURE);
    /*On parcours toutes les lignes du fichier*/
    while ((tmpTailleLigneCourante = getline(&tmpLigne, &tmpLongueur, tmp_fichier)) != -1) {
	printf("%s", tmpLigne);
	/*On decoupe la chaine de caractere en tableau*/
	tmpLigneCourante=diviseur_chaine(tmpLigne,';');
	/*Si on a une ligne correcte non null*/
	if(tmpLigneCourante){		    
	    
            /*On separe l'heure et les minutes pour l'heure de depart et l'heure d'arrivé car on recupere une chaine du type hh:MM*/
	    tmpTableauHoraireDepart = diviseur_chaine(tmpLigneCourante[2],':');
	    tmpTableauHoraireArrive = diviseur_chaine(tmpLigneCourante[3],':');
	    /*On crée le type horaire pour l'horaire d'arrivé et de départ*/
	    horaire* tmpHoraireDepart = creer_horaire(atoi(tmpTableauHoraireDepart[0]),atoi(tmpTableauHoraireDepart[1]));
	    horaire* tmpHoraireArrivee = creer_horaire(atoi(tmpTableauHoraireArrive[0]),atoi(tmpTableauHoraireArrive[1]));
		
	    /*On crée le code promo associé*/
	    enum CodePromo tmpCodePromo;
	    if(tmpLigneCourante[5]==(char*)"REDUC")
		tmpCodePromo=REDUC;
	    else if(tmpLigneCourante[5]==(char*)"SUPPL")
			tmpCodePromo=SUPPL;
	    else
		tmpCodePromo=DEFAULT;	  
	    /*On crée un voyage et on l'ajoute au tableau*/
	    voyage* tmpVoyage = creer_voyage(tmpCompteur+1,tmpLigneCourante[0],tmpLigneCourante[1],*tmpHoraireDepart,*tmpHoraireArrivee,atof(tmpLigneCourante[4]),tmpCodePromo);
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


void afficher_table(TableauVoyage *tableauVoyage){
    if(tableauVoyage==NULL)
        puts("tableau vide");
    else {
        printf("num : %d\n",tableauVoyage->v.numero_train);
        while(tableauVoyage->suivant!=NULL){
            printf("num : %d\n",tableauVoyage->v.numero_train);
            tableauVoyage=tableauVoyage->suivant;
        }
    }
}


int main(){
	TableauVoyage *tmpTableau=NULL;
	/* TODO :: Ajouter le tableau tmpTableau*/
	tmpTableau=remplir_tableau_voyage(tmpTableau);
	afficher_table(tmpTableau);

}


