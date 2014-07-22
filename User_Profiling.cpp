#include "Project_Engine_Head.h"

/*****************************User_Profiling()*****************************/

//-------------------------------------------------------------------
//By implementing the Singular Value Decomposition of the Customer-Movie
//observation matrix, we are able to compute the normalised Left Singular Vectors.
//The Sparse Movie Matrix is compressed and offers high performance and 
//low memory usage. It implements a Compressed Row Storage scheme.
//-------------------------------------------------------------------

/*****************************User_Profiling()*****************************/

void Project_Engine::User_Profiling()
{	
	int				i;
	float			nlize = sqrt(float (CUSTOMERS_MAX) );								

	MatrixXf		Profile_Vector	(CUSTOMERS_MAX-1,MOVIES_MAX-1);						//Decalre N Profile Vectors	(1 x F)					
	Sparse_Mat		Movie_Matrix	(CUSTOMERS_MAX-1,MOVIES_MAX-1);						//Declare Movie Matrix		(N x F)
	Movie_Matrix.reserve(VectorXi::Constant(CUSTOMERS_MAX,Profile_Length));				//Reserve memory for a MAX_CUSTOMERS number of vectors (of size Profile_Length)	
	
		for (i=0; i<upRating_Count; i++){
			if (Rating_spec[i].Rating>2)
				Movie_Matrix.insert(Rating_spec[i].Cust_ID-1,Rating_spec[i].Movie_ID-1) = 1;//Update Observation Matrix (if a user has rated a movie, a value of 1 is assigned. 0 otherwise)
			else
				Movie_Matrix.insert(Rating_spec[i].Cust_ID-1,Rating_spec[i].Movie_ID-1) = -1;
	}

	Movie_Matrix.makeCompressed();														//suppresses the remaining empty space and transforms the matrix into a compressed row storage.
	JacobiSVD<MatrixXf> svd(Movie_Matrix, ComputeThinU | ComputeThinV); 				//Two-sided Jacobi SVD decomposition of a rectangular matrix.

	Profile_Vector = svd.matrixU();
	Profile_Vector.conservativeResize(CUSTOMERS_MAX-1,Profile_Length);					//Resizing the Profile Vectors according to the predefined Profile Space Dimension 
	Profile_Vector = nlize*Profile_Vector;												//Normalised Left Singular Value Vectors are the columns of the U matrix
	

	cout << "Movie_Matrix singular values are:" << endl << svd.singularValues() << endl;
	cout<< "U matrix is of size:"	<<	endl;
	cout<< svd.matrixU().rows()		<<	"x"	<<	svd.matrixU().cols()	<<	endl;
	cout<< "V matrix is of size:"	<<	endl;
	cout<< svd.matrixV().rows()		<<	"x"	<<	svd.matrixV().cols()	<<	endl;

	this->kd_Tree(Profile_Vector);
}