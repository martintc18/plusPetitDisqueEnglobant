#include "fonction_megiddo.h"

/*
A chaque point est associé une équation linéaire ==> demi plan
dans un premier temps on regarde les points 2 à 2 pour éliminer des
fonctions.
*/
/*Nombre de point paire*/
/*dans un premier temps, le problème est contraint sur l'axe des abscisses y=0*/



/*
Dans cette fonction on tri le tableau de points donné:
on enlève les redondances au niveau des équations
*/
int triAbscisse(POINT tab[],int longueur){
	int i,j,k,compteur,doublon,c;
	POINT *resultat=malloc(longueur*sizeof(POINT));
	compteur=0;
	doublon=0;
	for(i=0;i<longueur;i+=2){//on prend les points 2 par 2
			if(tab[i].x==tab[i+1].x){//si ils ont la même abscisse
				if(auCarre(tab[i].y)<=auCarre(tab[i+1].y)){
					resultat[compteur]= tab[i+1];
					i-=1;
					compteur+=1;
					//on supprime le plus proche de (x,0)
				}
				else{
					resultat[compteur]=tab[i];
					compteur+=1;
					
				}
				doublon+=1;

			}else{
				resultat[compteur]=tab[i];
				resultat[compteur+1]=tab[i+1];
				compteur+=2;
				
			}
		//printf("%d doublon\n",doublon);


	}
	POINT *resultat_without_zero=malloc((longueur-doublon)*sizeof(POINT));
	c=0;
	for (j = 0; j < longueur; j++){
		if((resultat[j].x==resultat[j+1].x)&&(resultat[j].y==resultat[j+1].y)){
			j+=1;
		}
		resultat_without_zero[c]=resultat[j];
		c+=1;
	}
	for(k=0;k<longueur-doublon;k++){
		tab[k]=resultat_without_zero[k];
	}
	return longueur-doublon;

}

/*
Calcul des valeurs critiques:
revient à calculer l'intersection des médiatrice [ai;ai+1] avec l'axe y=ordonne
si nombre de point impaire: on ne traite pas le dernier à cette occurence
pour avoir le point selon une droite d'équation y!=0, on a simplement à prendre l'abscisse à y=0
et y ajouter ordonne*B; B étant le coeff directeur de la médiatrice
*/

DOUBLET* tableauValeurCritique(POINT tab[],int longueur,int ordonne){
	int i,j;
	double valeur_en_zero,valeur;
	double epsilon=0.00000001;
	double coeff_directeur;
	j=0;
		
	//if(longueur%2==0){
		DOUBLET *critique=malloc((longueur/2)*sizeof(DOUBLET));

		for(i=0;i<longueur;i+=2){
			valeur_en_zero=(auCarre(tab[i+1].x)-auCarre(tab[i].x)+auCarre(tab[i+1].y)-auCarre(tab[i].y))/(epsilon+2*((tab[i+1].x)-(tab[i].x)));
			coeff_directeur=(((tab[i+1].y)-(tab[i].y))/((tab[i].x)-(tab[i+1].x)));
			valeur=valeur_en_zero+ (coeff_directeur*ordonne);
			critique[j].mediatrice=valeur;
			critique[j].a=tab[i];
			critique[j].b=tab[i+1];
			j+=1;
		}
		return critique;
}

/*
Calcul la position de x* la solution par rapport à xm
*/

int calculPositionSolutionX(POINT tab[],int longueur,double mediane){
	int i;
	double max[2];
	POINT *xm=malloc(sizeof(POINT));
	xm->x=mediane;
	xm->y=0;
	max[0]=0;// xm<x*
	max[1]=0;// xm>x*
	for(i=0;i<longueur;i++){
		if((distance(*xm,tab[i])>=max[0])||(distance(*xm,tab[i])>=max[1])){

			if((tab[i].x)<(xm->x)){

				max[0]=distance(*xm,tab[i]);
			}else{

				max[1]=distance(*xm,tab[i]);
			}

		}

	}
	if(max[0]>max[1]){

		return INFERIEUR;

	}
	else if(max[0]<max[1]){

		return SUPERIEUR;

	}
	else{

		return EGAL;
	}

}


POINT* calculDemiPlanY(POINT tab[],int longueur,double ordonne){
	int i;
	int fin=0;
	int compteur=1;
	int nbpoints=2;
	POINT *enveloppeConvexe=malloc(longueur*sizeof(POINT));
	for(i=0;i<longueur;i++){//on centre les valeurs sur yc
		tab[i].y=tab[i].y-ordonne;
	}

	quickSortPointY(tab,0,longueur-1);
	enveloppeConvexe[0]=tab[0];
	for(i=1;i<longueur;i++){
		if(tab[i].y==tab[0].y){
			enveloppeConvexe[i]=tab[i];
			nbpoints+=1;
		}
		else{
			break;
		}
	}
	for(i=longueur-2;i>0;i--){
		if(tab[i].y==tab[longueur-1].y){
			enveloppeConvexe[nbpoints]=tab[i];
			nbpoints+=1;
		}
		else{
			break;
		}
	enveloppeConvexe[nbpoints+1]=tab[longueur-1];
	enveloppeConvexe[0]=tab[0];
	}

return enveloppeConvexe;
}
/*
on regarde les xcritique > ou < à xm
on peut enlever 1/2 de ces points
*/

int pruningContraint(POINT point[],int longueur,int ordonne){
	int i,j,compteur,solution,taille,impair;
	double med;
	compteur=0;
	impair=0;
	/*Tableau de départ*/
	printf("tabeau de point de départ\n");
	afficherTableauPoint(point,longueur);

	/*Trie de doublets de même abscisse*/
	taille=triAbscisse(point,longueur);
	printf("abscisse trié: \n");
	afficherTableauPoint(point,taille);

	/*Calcul des valeurs critiques*/
	if(taille%2!=0){
		taille-=1;
		impair=1;
	}
	
	//printf("bug ICI\n");
	DOUBLET* doublet=malloc(taille/2*sizeof(DOUBLET));
	doublet=tableauValeurCritique(point,taille,ordonne);
	printf("Calcul des valeurs critiques: \n");
	afficherTableauDoublet(doublet,taille/2);

	/*Trie des valeurs pour trouver la médiane + gestion des cas impairs*/
	med=triMediatrice(doublet,0,taille/2-1);
	printf("tableau de doublets trié pour trouver la médiane\n");
	afficherTableauDoublet(doublet,taille/2);
	
	printf("valeur médiane: %lf\n",med );


	/*Création d'un POINT médian (xm,0)*/
	POINT *xm=malloc(sizeof(POINT));
	xm->x=med;
	xm->y=0;

	/*Calcul de la position de x*
	=2 fin
	=1 xm<x*
	=0 xm>x*
	 */
	solution=calculPositionSolutionX(point,taille,xm->x);
	if(solution==1){
		printf("xm<x*\n");
	}
	else{
		printf("xm>x*\n");
	}

	/*élagage de 1/4 des points*/
	POINT *result=malloc(taille*sizeof(POINT));//resultat
	POINT *elim=malloc(taille*sizeof(POINT));//point à éliminer
	int count=0;
	for(i=0;i<taille/2;i++){
		if(solution==1){
			if(doublet[i].mediatrice<=(xm->x)){
				elim[count]=*distanceInf(*xm,&doublet[i].a,&doublet[i].b);
				count+=1;
			}
		}
		else if(solution==0){

			if(doublet[i].mediatrice>=(xm->x)){
				elim[count]=*distanceInf(*xm,&doublet[i].a,&doublet[i].b);
				count+=1;
			}

		}
		else{
			exit(EXIT_SUCCESS);
		}
	}
	printf("count= %d\n",count );
	printf("valeurs à éliminer:\n");
	quickSortPointX(elim,0,count-1);
	afficherTableauPoint(elim,count);
	quickSortPointX(point,0,taille-1);
	if(impair==1){
		taille+=1;
	}
	/*Problème de suppression si 2 valeurs avec la même abscisse*/

	for(j=0;j<taille;j++){
		if((point[j].x==elim[compteur].x)&&(point[j].y==elim[compteur].y)){
			compteur++;

		}else{
			result[j-compteur]=point[j];
		}
			
	}
	for(j=0;j<taille-compteur;j++){
		point[j]=result[j];
	}
	// free(doublet);
	// free(xm);
	// free(result);
	// free(elim);
	return taille-compteur;		
}


/*Calcul l'angle avec la droite x=0 */

DOUBLET* calculAngleCritique(POINT tab[],int longueur){
	int i;
	double angle;
	
	POINT *milieu=malloc(sizeof(POINT));
	POINT *xcritique=malloc(sizeof(POINT));
	DOUBLET *critique=malloc(partiEntiere(longueur/2)*sizeof(DOUBLET));

	critique=tableauValeurCritique(tab,longueur,0);

	for(i=0;i<partiEntiere(longueur/2);i++){
		xcritique->x=critique[i].mediatrice;
		xcritique->y=0;
		milieu->x=((critique[i].a.x)+(critique[i].b.x))/2;
		milieu->y=((critique[i].a.y)+(critique[i].b.y))/2;
		angle=atan2((milieu->y),((milieu->x)-(xcritique->x)));
		if(angle>(PI/2)){
			angle=angle -PI;
		}
		else if(angle<(-PI/2)){
			angle=angle +PI;
		}
		if (critique[i].a.x!=critique[i].b.x){
			critique[i].angle=angle;
		}
		else{
			critique[i].angle=0;
		}
		

	}
	return critique;

}


POINT* intersectionLigne(DOUBLET tab[],int longueur){
	int i;
	double angleMedian;
	int taille=partiEntiere(longueur/2);
	POINT *intersection=malloc(taille*sizeof(POINT));
	angleMedian=triAngle(tab,0,longueur-1);
	printf("angle médian:%lf\n",angleMedian );
	calculDroite(tab,longueur);
	for(i=0;i<taille;i++){
		if(estParallele(tab[i],tab[longueur-i-1])!=1){
			intersection[i]=*calculIntersection(tab[i],tab[longueur-i-1]);
			//printf("point %d | doublet %d et %d | x=%lf | y=%lf\n",i,i,longueur-i-1,intersection[i].x,intersection[i].y );
		}
	}
	//printf("taille tableau: %d\n",i);
	return intersection;
}

