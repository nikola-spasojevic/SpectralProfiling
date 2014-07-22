#include "Project_Engine_Head.h"
#include "engine.h"
#include "Linked_List.h"

/*****************************Class_Sorting()*****************************/

//-------------------------------------------------------------------
//Generating a Statistical Model where Users and Movies are sorted into 
//partitioned classes that hold statistically identical elements.
//This is accomplished by calculating the Euclidean Distance of each pair
//of Profile Vectors and sorting them accordingly to their corresponding class.
//The execution of the User and Movie Sorting Algorithms is based 
//upon the implementaion of Linked List arrays (Hash Tables), where a
//user or movie is sorted into a class by being added into a specific Linked 
//List.
//-------------------------------------------------------------------

/*****************************Class_Sorting()*****************************/

void Project_Engine::Class_Sorting(Node** table)	
{
	int				i,j,counter;
	float			min_mov,max_mov,mov_ref,Current_mov;
	MatrixXf		R_Matrix(Cust_classes,Mov_classes); 
	VectorXf		Mov(MOVIES_MAX-1);						//Vector containing corresponding Profile Vector Norm values 
	NodePtr			Norm_List	= NULL;											//Declare Customer linked list
	NodePtr			Movie_List	= NULL;											//Declare Movie linked list	
	intHashTable*	movie_hash_table	= new intHashTable(Mov_classes);		//Declare Movie Hash Table
	Engine*			ep;

	/*The basis of similarities is set upon the Squared Euclidean distance between Profile Vectors*/

	if (!(ep = engOpen("\0"))){
		cerr << "Cant start MATLAB engine" << endl;
	}

/*****************************USER MATLAB*****************************/	
	mxArray *x_array	= mxCreateDoubleMatrix(CUSTOMERS_MAX-1,1,mxREAL);
	mxArray *y_array	= mxCreateDoubleMatrix(CUSTOMERS_MAX-1,1,mxREAL);
	double *px			= mxGetPr(x_array);
	double *py			= mxGetPr(y_array);
	NodePtr temp		= NULL;

	counter = 0;
		for (j=0;j<Cust_classes;j++){
			temp = table[j];
			while(temp!=NULL){
				px[counter] = j;
				py[counter] = temp->Eucl_Norm;
				temp = temp->Next;
				counter++;
		}
	}
	memcpy((void*)mxGetPr(x_array), (void*)px, (CUSTOMERS_MAX-1)*sizeof(float));
	memcpy((void*)mxGetPr(y_array), (void*)py, (CUSTOMERS_MAX-1)*sizeof(float));

/*****************************MOVIE CLASSING ALGORITHM*****************************/
	for (i=0;i<MOVIES_MAX-1;i++){
			addToList(i+1, (float) Movie_spec[i+1].Movie_RateCount, Movie_List);
			Mov(i) = Movie_spec[i+1].Movie_RateCount;
	}

	min_mov = Mov.minCoeff();
	max_mov = Mov.maxCoeff();
	mov_ref = (max_mov-min_mov)/Mov_classes;

	for (i=0;i<MOVIES_MAX-1;i++){
		Current_mov = Mov(i);
		for (j=1;j<=Mov_classes;j++){
			if (Current_mov<(j*mov_ref+min_mov)){
					movie_hash_table->insert(i+1,Current_mov,j-1);
					break;
			}
		}
	}

/*****************************MOVIE MATLAB*****************************/	
mxArray *movx_array	= mxCreateDoubleMatrix(MOVIES_MAX-1,1,mxREAL);
mxArray *movy_array	= mxCreateDoubleMatrix(MOVIES_MAX-1,1,mxREAL);
double *pmovx			= mxGetPr(movx_array);
double *pmovy			= mxGetPr(movy_array);
Node** movtable			= movie_hash_table->getTable();

counter = 0;
for (j=0;j<Mov_classes;j++){	
		while(movtable[j]!=NULL){
			pmovx[counter] = j;
			pmovy[counter] = movtable[j]->Eucl_Norm;
			movtable[j] = movtable[j]->Next;
			counter++;
		}
}
memcpy((void*)mxGetPr(movx_array), (void*)pmovx, (MOVIES_MAX-1)*sizeof(float));
memcpy((void*)mxGetPr(movy_array), (void*)pmovy, (MOVIES_MAX-1)*sizeof(float));

/*****************************MATLAB EXECUTION*****************************/	
engPutVariable(ep,"x",x_array);
engPutVariable(ep,"y",y_array);
engPutVariable(ep,"movx",movx_array);
engPutVariable(ep,"movy",movy_array);
engEvalString(ep, "figure(1);");
engEvalString(ep, "plot(x,y,'*');");
engEvalString(ep, "grid on;");
engEvalString(ep, "title('Profile Vector Classing Algorithm')");
engEvalString(ep, "xlabel('Customer Class')");
engEvalString(ep, "ylabel('Euclidean Distance from referrence point')");
engEvalString(ep, "figure(2);");
engEvalString(ep, "plot(movx,movy,'*');");
engEvalString(ep, "title('Movie Rating Classing Algorithm')");
engEvalString(ep, "xlabel('Movie Class')");
engEvalString(ep, "ylabel('Number of Ratings (Popularity)')");

string s;
cout << "Press Any Key to exit MATLAB and continue the program:" << endl;
getline(cin, s);
mxDestroyArray(x_array);
mxDestroyArray(y_array);
engEvalString(ep,"close;");

movie_hash_table->~intHashTable();
}