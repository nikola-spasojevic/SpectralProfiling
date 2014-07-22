#include "Project_Engine_Head.h"
#include "engine.h"
#include "Linked_List.h"
#include <math.h> 

void Project_Engine::Matlab_execution()
{
	Engine	*ep;
	mxArray *cust_array					= mxCreateDoubleMatrix(CUSTOMERS_MAX-1,1,mxREAL);
	mxArray *mov_avg_array				= mxCreateDoubleMatrix(MOVIES_MAX-1,1,mxREAL);
	mxArray *mov_count_array			= mxCreateDoubleMatrix(MOVIES_MAX-1,1,mxREAL);
	double	*p_rating_count				= mxGetPr(cust_array);
	double	*p_movie_avg				= mxGetPr(mov_avg_array);
	double	*p_mov_count				= mxGetPr(mov_count_array);
	int		i,j,counter;
	float	Current_movie,Current_cust;
	intHashTable*	movie_average		= new intHashTable(9);
	intHashTable*	user_average		= new intHashTable(9);

//======================USER AVERAGE DISTRIBUTION======================
	for (i=1;i<CUSTOMERS_MAX;i++){
		//Round up each customer rating average to the nearest 0.5 between 1 and 5
		Current_cust = floor((2*Customer_spec[i].Cust_RateAvg)+0.5)/2;
		for (j=0;j<9;j++){
			if (Current_cust<(1+0.5*j)){
					//According to the users rounded average, the corresponding id is inserted into a specific rating list 
					user_average->insert(i,Current_cust,j);
					break;
			}
		}
	}

mxArray *custrating_array	= mxCreateDoubleMatrix(9,1,mxREAL);
mxArray *custcount_array	= mxCreateDoubleMatrix(9,1,mxREAL);
double *pcustx				= mxGetPr(custrating_array);
double *pcusty				= mxGetPr(custcount_array);
Node** user_table			= user_average->getTable();

for (j=0;j<9;j++){
		counter = 0;
		pcustx[j] = 1+0.5*j;
		while(user_table[j]!=NULL){
			counter++;
			user_table[j] = user_table[j]->Next;
		}
		pcusty[j] = counter;
	}
memcpy((void*)mxGetPr(custrating_array),	(void*)pcustx, (9)*sizeof(float));
memcpy((void*)mxGetPr(custcount_array),		(void*)pcusty, (9)*sizeof(float));
//======================USER AVERAGE DISTRIBUTION======================

//======================MOVIE AVERAGE DISTRIBUTION======================
	for (i=1;i<MOVIES_MAX;i++){
		Current_movie = floor((2*Movie_spec[i].Movie_RateAvg)+0.5)/2;
		for (j=0;j<9;j++){
			if (Current_movie<(1+0.5*j)){
					movie_average->insert(i,Current_movie,j);
					break;
				}
		}
	}

mxArray *mrating_array	= mxCreateDoubleMatrix(9,1,mxREAL);
mxArray *mcount_array	= mxCreateDoubleMatrix(9,1,mxREAL);
double *px				= mxGetPr(mrating_array);
double *py				= mxGetPr(mcount_array);
Node** movie_table		= movie_average->getTable();

for (j=0;j<9;j++){
		counter = 0;
		px[j] = 1+0.5*j;
		while(movie_table[j]!=NULL){
			counter++;
			movie_table[j] = movie_table[j]->Next;
		}
		py[j] = counter;
	}
memcpy((void*)mxGetPr(mrating_array),	(void*)px, (9)*sizeof(float));
memcpy((void*)mxGetPr(mcount_array),	(void*)py, (9)*sizeof(float));
//======================MOVIE AVERAGE DISTRIBUTION======================	

	if (!(ep = engOpen("\0")))
		cerr << "Cant start MATLAB engine" << endl;

	for (i=1;i<CUSTOMERS_MAX;i++){
		p_rating_count[i-1]	=	Customer_spec[i].Cust_RateCount;		
	 };

	for (i=1;i<MOVIES_MAX;i++){
		p_movie_avg[i-1]		=	Movie_spec[i].Movie_RateAvg;
		p_mov_count[i-1]		=	Movie_spec[i].Movie_RateCount;
	 };

memcpy((void*)mxGetPr(cust_array),		(void*)p_rating_count,	(CUSTOMERS_MAX-1)*sizeof(float));
memcpy((void*)mxGetPr(mov_avg_array),	(void*)p_movie_avg,		(MOVIES_MAX-1)*sizeof(float));
memcpy((void*)mxGetPr(mov_count_array), (void*)p_mov_count,		(MOVIES_MAX-1)*sizeof(float));

engPutVariable(ep,"rating_count",cust_array);
engPutVariable(ep,"movie_avg"	,mov_avg_array);
engPutVariable(ep,"mov_count"	,mov_count_array);

engEvalString(ep, "figure(1);");
engEvalString(ep, "B=sort(rating_count);");
engEvalString(ep, "numberOfBins = 100;[counts, binValues] = hist(B, numberOfBins);normalizedCounts = 100 * counts / sum(counts);");
engEvalString(ep, "bar(sort(binValues),normalizedCounts,'r');set(gca,'XTick', []);");
engEvalString(ep, "grid on;");
engEvalString(ep, "title('Customer Rating Count Distribution')");
engEvalString(ep, "ylabel('Rating Count [%]')");

engEvalString(ep, "figure(2);");
engEvalString(ep, "B=sort(movie_avg);plot(B,'-.r*');");
engEvalString(ep, "grid on;");
engEvalString(ep, "title('Movie Average Distribution')");
engEvalString(ep, "xlabel('Movie ID')");
engEvalString(ep, "ylabel('Rating Average')");

engEvalString(ep, "figure(3);");
engEvalString(ep, "C=sort(mov_count);");
engEvalString(ep, "numberOfBins = 100;[counts, binValues] = hist(C, numberOfBins);normalizedCounts = 100 * counts / sum(counts);");
engEvalString(ep, "bar(sort(binValues),normalizedCounts);");
engEvalString(ep, "grid on;");
engEvalString(ep, "title('Movie Rating Count Distribution')");
engEvalString(ep, "ylabel('Rating Count [%]')");

engPutVariable(ep,"x",mrating_array);
engPutVariable(ep,"y",mcount_array);

engPutVariable(ep,"custx",custrating_array);
engPutVariable(ep,"custy",custcount_array);

engEvalString(ep, "figure(4);");
engEvalString(ep, "bar(x,y/sum(y));");
engEvalString(ep, "grid on;");
engEvalString(ep, "title('Average Rating Distribution of Movies')");
engEvalString(ep, "xlabel('Rating')");
engEvalString(ep, "ylabel('Density')");
engEvalString(ep, "hold on");
engEvalString(ep, "plot(custx,custy/sum(custy),'--r','LineWidth',2);");
engEvalString(ep, "hold off");

string s;
cout << "Press Any Key to exit MATLAB and continue the program:" << endl;
getline(cin, s);
mxDestroyArray(cust_array);
mxDestroyArray(mov_avg_array);
mxDestroyArray(mov_count_array);
engEvalString(ep,"close;");
}