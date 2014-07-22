#include "Project_Engine_Head.h"
#include "engine.h"
#include "Linked_List.h"

/*****************************Voting_Algorithm()*****************************/

//-------------------------------------------------------------------

//-------------------------------------------------------------------

/*****************************Voting_Algorithm()*****************************/

void Project_Engine::Voting_Algorithm(Node** table){

int						i,j,current_cust,counter;
Sparse_Mat				Movie_Matrix(CUSTOMERS_MAX-1,MOVIES_MAX-1);		//Declare Movie Matrix		(N x F)
double					sum;
MatrixXd				rate_matrix(Cust_classes,MOVIES_MAX);
NodePtr					p;
NodePtr					Class_List	= NULL;	

	Movie_Matrix.reserve(VectorXi::Constant(CUSTOMERS_MAX,Profile_Length));//Reserve memory for a MAX_CUSTOMERS number of vectors (of size 4)		
	for (i=0; i<upRating_Count; i++){ 
			Movie_Matrix.insert(Rating_spec[i].Cust_ID-1,Rating_spec[i].Movie_ID-1) = Rating_spec[i].Rating;
		}
	Movie_Matrix.makeCompressed();	

	for (j=0;j<Cust_classes;j++){
		for (i=1;i<MOVIES_MAX;i++){	
			sum		= 0;
			counter = 0;
			p = table[j];
				while(p!=NULL){
					current_cust = p->Cust_ID;	//0 - 38065
					addToList(current_cust, j, Class_List);				//List showing with which class each user is associated with
						if (Movie_Matrix.coeff(current_cust,i-1)!=0){	//if (Movie_Matrix.coeff(current_cust-1,i-1)!=0){
							sum += Movie_Matrix.coeff(current_cust,i-1);//sum += Movie_Matrix.coeff(current_cust-1,i-1);
							counter++;
						}
					p = p->Next;
				}
				sum = sum/counter;
				if ((sum == 0) || (counter == 0))
					sum = 3.9;
				rate_matrix(j,i) = sum;
		}
	}

	this->Qualify(L"probe.txt", rate_matrix, Class_List, Movie_Matrix);
}