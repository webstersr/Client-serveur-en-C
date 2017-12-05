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


void afficher_table(TableauVoyage *tableauVoyage);
TableauVoyage* remplir_tableau_voyage(TableauVoyage *pTableau_voyage);
voyage * creer_voyage(int pNumero_train, char* pVoyageille_depart, char* pVoyageille_arrive, horaire pHeure_depart, horaire pHeure_arrive, double pPrix, CodePromo pCode_promo);
horaire *creer_horaire(int pHeure, int pMinute);
TableauVoyage* ajouter_voyage(TableauVoyage *pTab_v, voyage* pVoyage);
char** diviseur_chaine(char* pChaine, const char pDelimiteur);
TableauVoyage* remplir_tableau_voyage(TableauVoyage *pTableau_voyage);


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
    tmp_fichier = fopen("Trains.txt", "r");
    if (tmp_fichier == NULL){
        fprintf(stderr,"ERREUR OUVERTURE IMPOSSIBLE DU FICHIER");
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
            tmpTableauHoraireDepart = diviseur_chaine(tmpLigneCourante[3],':');
            tmpTableauHoraireArrive = diviseur_chaine(tmpLigneCourante[4],':');
            /*On crée le type horaire pour l'horaire d'arrivé et de départ*/
            
            // printf("heure depart test : %s, %s\n", tmpTableauHoraireDepart[0], tmpTableauHoraireDepart[1]);
            // printf("heure arrivé test : %s, %s\n", tmpTableauHoraireArrive[0], tmpTableauHoraireArrive[1]);

            tmpHoraireDepart = creer_horaire(atoi(tmpTableauHoraireDepart[0]),atoi(tmpTableauHoraireDepart[1]));
            tmpHoraireArrivee = creer_horaire(atoi(tmpTableauHoraireArrive[0]),atoi(tmpTableauHoraireArrive[1]));
            
            int tmpNumeroTrain = atoi(tmpLigneCourante[0]);
            
            /*On crée le code promo associé*/
            if(tmpLigneCourante[5]==(char*)"REDUC")
                tmpCodePromo=REDUC;
            else if(tmpLigneCourante[5]==(char*)"SUPPL")
                tmpCodePromo=SUPPL;
            else
                tmpCodePromo=DEFAULT;
            /*On crée un voyage et on l'ajoute au tableau*/
            tmpVoyage=creer_voyage(tmpNumeroTrain,tmpLigneCourante[1], tmpLigneCourante[2], *tmpHoraireDepart,*tmpHoraireArrivee,atof(tmpLigneCourante[5]),tmpCodePromo);
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

/**
* Fonction qui crée et initialise un voyage
* @param	{int} pNumero_train, numéro du train
* @param	{char*} pVoyageille_depart, ville de départ
* @param	{char*} pVoyageille_arrive, ville d'arrivee
* @param	{horaire} pHeure_depart, horaire de départ
* @param	{horaire} pHeure_arrive, horaire d'arrivee
* @param	{double} pPrix, prix du trajet
* @param	{enum CodePromo} pCode_promo, code promo (REDUC, SUPPL, DEFAULT)
* @return	Le voyage crée et initialisé 
*/
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

/**
* Fonction qui crée et initialise un horaire
* @param	{int} pHeure, heure de l'horaire
* @param	{int} pMinute, minute de l'horaire
* @return	L'horaire crée
*/
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

/**
* Fonction qui ajoute un voyage à la fin de le tableau des voyages
* @param	{TableauVoyage} *pTab_v, le tableau des voyages où l'on ajoute
* @param	{voyage} pVoyage, le voyage à ajouter
* @return	le tableau des voyages avec le voyage ajouté en fin
*/
TableauVoyage* ajouter_voyage(TableauVoyage *pTab_v, voyage* pVoyage){
    TableauVoyage *tmpNouveauVoyage = (TableauVoyage*)malloc(sizeof(TableauVoyage));
    /*Sauvegarde de notre table*/
    TableauVoyage *tmpSauvegarde=pTab_v;
    /*On ajoute le voyage à le tableau*/
    tmpNouveauVoyage->v = *pVoyage;
    tmpNouveauVoyage->suivant = NULL;
    /*Si le tableau est vide*/
    if(pTab_v==NULL) {
    	/*On retourne le tableau avec le voyage ajouter*/
        return tmpNouveauVoyage;
    }
    /*Sinon on parcourt le tableau*/
    else {
    	/*Parcours du tableau*/
        while(pTab_v->suivant!=NULL)
            pTab_v=pTab_v->suivant;
        /*On ajoute notre voyage à fin de le tableau*/
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


/**
* Fonction qui affiche le tableau des voyages passées en paramètre
* @param	{TableauVoyage*} tableauVoyage, le tableau à afficher
* @return	void
*/
void afficher_table(TableauVoyage *tableauVoyage){
	/*Si le tableau est vide on dit que le tableau est vide*/
    if(tableauVoyage==NULL)
        puts("tableau vide");
    else {
    	/*On affiche le premier voyage*/
        printf("num : %d\n",tableauVoyage->v.numero_train);
        /*On parcourt le tableau*/
        while(tableauVoyage->suivant!=NULL){
        	tableauVoyage=tableauVoyage->suivant;
        	/*On affiche le voyage courant*/
            printf("num : %d\n",tableauVoyage->v.numero_train);
            
        }
    }
}

/**
* Fonction qui affiche un horaire sous la forme hh:mm
* @param	{horaire} pH, horaire à afficher
* @return	void
*/
void afficher_horaire(horaire pH){
    if(pH.heure<10) {
    	/*cas h:m*/
        if(pH.minute<10)
            fprintf(stdout,"0%d:0%d\n",pH.heure,pH.minute);
       	/*cas h:mm*/
        else
            fprintf(stdout,"0%d:%d\n",pH.heure,pH.minute);
    }	
    else {
    	/*cas hh:m*/
        if(pH.minute<10)
            fprintf(stdout,"%d:0%d\n",pH.heure,pH.minute);
        /*cas hh:mm*/
        else
            fprintf(stdout,"%d:%d\n",pH.heure,pH.minute);
    }
}

void afficher_table_cfSAMSE(TableauVoyage * tmpTableauRechercheParDest)
{
    if(tmpTableauRechercheParDest==NULL)
    {
        printf("Tableau Vide\n");
    }
    else{
        while(tmpTableauRechercheParDest->suivant!=NULL)
        {
            printf("Affichage Voyage retourné par rechercherTrainParDest() \n => %d, %s, %s, %d, %d, %d, %d, %f \n\n",tmpTableauRechercheParDest->v.numero_train, tmpTableauRechercheParDest->v.ville_depart, tmpTableauRechercheParDest->v.ville_arrive, tmpTableauRechercheParDest->v.heure_depart.heure, tmpTableauRechercheParDest->v.heure_depart.minute, tmpTableauRechercheParDest->v.heure_arrive.heure, tmpTableauRechercheParDest->v.heure_arrive.minute, tmpTableauRechercheParDest->v.prix);
            tmpTableauRechercheParDest = tmpTableauRechercheParDest->suivant;
        }
    }

}

void afficher_voyage(voyage *tmpVoyage)
{
    if(tmpVoyage!=NULL)
    {
        printf("Affichage Voyage retourné par rechercherTrainParDepart() \n => %d, %s, %s, %d, %d, %d, %d, %f \n\n",tmpVoyage->numero_train, tmpVoyage->ville_depart, tmpVoyage->ville_arrive, tmpVoyage->heure_depart.heure, tmpVoyage->heure_depart.minute, tmpVoyage->heure_arrive.heure, tmpVoyage->heure_arrive.minute, tmpVoyage->prix);
    }
}

/**
 * La fonction recherche un voyage parmis les voyages contenu dans pTableau
 * @param {char*} pVille_depart :
 * @param {char*} pVille_arrive : 
 * @param {char*} pHeure_depart : Delimiteur à appliquer sur la chaine
 * @param {TableauVoyage*} pTableau :
 * @return {voyage} struct voyage du voyage en question ou voyage NULL
 */
voyage* rechercherTrainParHoraireDepart(char* pVille_depart, char* pVille_arrive, char* pHeure_depart, TableauVoyage *pTableau)
{
    char ** tmpTableauHoraireDepart;
    
    /*On separe l'heure et les minutes pour l'heure de depart et l'heure d'arrivé car on recupere une chaine du type hh:MM*/
    tmpTableauHoraireDepart = diviseur_chaine(pHeure_depart,':');

    /*On cast les minutes et les heures en int*/
    int tmpHeure = atoi(tmpTableauHoraireDepart[0]);
    int tmpMinute = atoi(tmpTableauHoraireDepart[1]);
    while(pTableau->suivant!=NULL)
    {
        /* Si train trouve a la bonne heure */
        if(strcmp(pTableau->v.ville_depart, pVille_depart) == 0 &&
           strcmp(pTableau->v.ville_arrive, pVille_arrive) == 0 &&
           pTableau->v.heure_depart.heure == tmpHeure &&
           pTableau->v.heure_depart.minute == tmpMinute)
        {
            printf("\nTrain trouvé\n\n");
            free(*(tmpTableauHoraireDepart));
            printf("\napres le train trouvé %s\n\n", pTableau->v.ville_depart);
            return &pTableau->v;
        }
        /* Si train trouve le plus proche, apres l'heure indique */
        else if(strcmp(pTableau->v.ville_depart, pVille_depart) == 0 &&
                strcmp(pTableau->v.ville_arrive, pVille_arrive) == 0 &&
                pTableau->v.heure_depart.heure >= tmpHeure &&
                pTableau->v.heure_depart.minute >= tmpMinute)
        {
            printf("\nTrain plus proche trouvé\n\n");
            free(*(tmpTableauHoraireDepart));
            printf("\napres le train trouvé %s\n\n", pTableau->v.ville_depart);
            return &pTableau->v;
        }
        /* Sinon passer au suivant */
        else
        {
            pTableau = pTableau->suivant;
        }
    }
    printf("\nAucun Train trouvé\n\n");
    free(*(tmpTableauHoraireDepart));
    return NULL;
    // return pTableau->v;
}

/**
 * La fonction recherche tout voyage d'un point a un autre parmis les voyages contenu dans pTableau
 * @param {char*} pVille_depart :
 * @param {char*} pVille_arrive :
 * @param {TableauVoyage*} pTableau :
 * @return {TableauVoyage*} tableau contenant les voyages en question
 */
TableauVoyage* rechercherTrainsParDestionation(char* pVille_depart, char* pVille_arrive, TableauVoyage *pTableau)
{
    TableauVoyage *tmpTableau=NULL;

    while(pTableau->suivant!=NULL)
    {
        /* Si train avec meme depart et meme dest*/
        if(strcmp(pTableau->v.ville_depart, pVille_depart) == 0 &&
           strcmp(pTableau->v.ville_arrive, pVille_arrive) == 0)
        {
            printf("\nTrain trouvé\n\n");
            tmpTableau = ajouter_voyage(tmpTableau, &pTableau->v);
        }
        /* Sinon passer au suivant */
        pTableau = pTableau->suivant;
    }
    return tmpTableau;
}

/*
TableauVoyage* rechercherTrainsParTrancheHoraires(char* pVille_depart, char* pVille_arrive, char* pHeure_depart_min, char* pHeure_depart_max)
{
}

TableauVoyage* filterTrajets(char* pFiltre, voyage[] pListeVoyage)
{
 // pFiltre char * = PRIX ou DURE
}
*/



int main(){
    TableauVoyage *tmpTableau=NULL;
    /* TODO :: Ajouter le tableau tmpTableau*/
    tmpTableau=remplir_tableau_voyage(tmpTableau);
    afficher_table(tmpTableau);

    /* Test fct rechercherTrainParHoraireDepart*/
    printf("\nTest fct rechercherTrainParHoraireDepart :\n");
    
    char *tmpHeure = "6:44"; // allocation d'une heure test en memoire pour eviter 'erreur Bus 10'
    char *str = malloc(strlen(tmpHeure));
    strcpy(str, tmpHeure);
    
    voyage *tmpVoyage = rechercherTrainParHoraireDepart("Valence", "Grenoble", str, tmpTableau);
    afficher_voyage(tmpVoyage);

    /* Test fct rechercherTrainsParDestionation*/
    printf("\nTest fct rechercherTrainsParDestionation :\n");

    TableauVoyage *tmpTableauRechercheParDest = rechercherTrainsParDestionation("Valence", "Gr", tmpTableau);
    afficher_table_cfSAMSE(tmpTableauRechercheParDest);


}

