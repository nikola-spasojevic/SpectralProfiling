#ifndef Project_Engine_Head_H
#define Project_Engine_Head_H

//=============================================================================
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <tchar.h>
#include <map>
#include <fstream>
#include <iostream>
using namespace std;
#include <Eigen/SVD>
#include "Eigen/Dense"
#include <H:\Visual Studio 2010\eigen\Eigen\src\SVD\JacobiSVD.h>
#include <Eigen/SparseCore>
using namespace Eigen;
#include "Linked_List.h"
#include <H:\Visual Studio 2010\nanoflann-1.1.6\include\nanoflann.hpp>
using namespace nanoflann;
//===================================================================
//
// Constants and Type Declarations
//
//===================================================================
#define TRAINING_DIRECTORY		L"C:\\Netflix\\Training_set\\*.txt"				//File Directory containg the TRAINING SET
#define TRAINING_FILE			L"C:\\Netflix\\Training_set\\%s"				//Read each individaul training file
#define FEATURE_FILE			L"C:\\Netflix\\Feature_file.txt"
#define TEST_PATH				L"C:\\Netflix\\%s"
#define PREDICTION_DIRECTORY	L"C:\\Netflix\\Prediction_file\\prediction.txt"	//File Directory for PREDICTION SET

#define RATINGS_MAX     5016404//100480508//42697									// Ratings		accountable within the training set (+1)
#define CUSTOMERS_MAX   404603//480190//38066										// Customers	accountable within the training set (+1)
#define MOVIES_MAX      1002//17771// 20											// Movies		accountable within the training set (+1)

#define Profile_Length	7

typedef unsigned char			BYTE;
typedef map<int, int>			Mapping_ID;										//Declare Map for customer ID's
typedef Mapping_ID::iterator	ID_iterator;									//Customer ID Map iterator
typedef Eigen::SparseMatrix<short int, RowMajor> Sparse_Mat;					// declares a row-major sparse matrix type of int
//===================================================================
//
// Data Structures
//
//===================================================================
struct Struct_Movie
{
	int		Movie_RateCount;	//Number of ratings per movie
	int		Movie_RateSum;		//Sum of ratings per movie
	double	Movie_RateAvg;		//Rating Average per movie
	double	Movie_PsAvg;		//Weighted average used to deal with small movie counts 
};

struct Struct_Customer
{
	int		Cust_ID;			//Customer ID
	int		Cust_RateCount;		//Number of ratings from specific customer
	int		Cust_RateSum;		//Rating sum from customer
	double	Cust_RateAvg;		//Rating Average of customer
};

struct Struct_Rating
{
	int		Cust_ID;			//Customer ID associated with each rating
	short	Movie_ID;			//Movie ID associated with each rating
	BYTE	Rating;				
	float	Cache;
};
//===================================================================
//
// Engine Class 
//
//===================================================================

class Project_Engine 
{
private:
    int						upRating_Count;									//Number of Ratings read from the Training Set                             
    Struct_Rating           Rating_spec[RATINGS_MAX];						//Specifications of the Rating
    Struct_Movie			Movie_spec[MOVIES_MAX];							//Movie Specifications Array
    Struct_Customer			Customer_spec[CUSTOMERS_MAX];					//Customer Specifications Array
    Mapping_ID				mapped_custID;                                  // Map for one time translation of ids to compact array index

    bool					ReadNumber(wchar_t* pwzBufferIn, int nLength, int &nPosition, wchar_t* pwzBufferOut);
    bool					Parse_Str_Int(wchar_t* pwzBuffer, int nLength, int &nPosition, int& nValue);
    bool					Parse_Str_Float(wchar_t* pwzBuffer, int nLength, int &nPosition, float& fValue);

	void					matrixMultiply(int rows1, int cols1, float **mat1, int rows2, int cols2, float **mat2, float **result);

public:
    Project_Engine(void);
    ~Project_Engine(void) { };

	void            Data_Integratrion();
	void            File_Processing	(wchar_t* pwzFile);

    void            Mapping();
    void            CalcFeatures();


	void			User_Profiling();
	void			Class_Sorting(Node** table);
	void			Voting_Algorithm(Node** table);
	void			Matlab_execution();
	void			kd_Tree(MatrixXf Profile_Vector);
	void			Qualify(wchar_t* pwzFile, MatrixXd rate_matrix, NodePtr Class_List, Sparse_Mat Movie_Matrix);
};

#endif