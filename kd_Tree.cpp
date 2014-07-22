#include "Project_Engine_Head.h"
#include "engine.h"
#include "Linked_List.h"
#include <algorithm>    // std::sort

/*****************************kd_Tree()*****************************/

//-------------------------------------------------------------------

//-------------------------------------------------------------------

/*****************************kd_Tree()*****************************/

void Project_Engine::kd_Tree(MatrixXf Profile_Vector)	
{
	VectorXf		Nrm					(CUSTOMERS_MAX-1),Mov(MOVIES_MAX-1);//Vector containing the corresponding Profile Vector Norm values 
	NodePtr			Norm_List			= NULL;								//Declare Customer linked list
	intHashTable*	int_hash_table		= new intHashTable(Cust_classes);	//Declare Customer Hash Table
	Node**			table;
	MatrixXf		Dummy_Matrix		(CUSTOMERS_MAX-1,Profile_Length+1);
	MatrixXf		Dummy_Vector		(CUSTOMERS_MAX-1,1);
	
	size_t			i;
	int				j;
	float			max_norm,radius;
	vector<float>	query_pt(Profile_Length);								//Querry point used as point of reference for kd Tree search algorithm
	
	/*****************************MATRIX SPECIFICATION*****************************/
	for (j=0;j<CUSTOMERS_MAX-1;j++){
			addToList(j, Profile_Vector.row(j).squaredNorm(), Norm_List);	//Update Customer Linked List with User id's
			Nrm(j) = Profile_Vector.row(j).squaredNorm();
	}
	max_norm		= Nrm.maxCoeff();
	radius			= max_norm;												//Boundary of the entire system within the Profile Space

	for (int l=0;l<CUSTOMERS_MAX-1;l++){
		Dummy_Vector(l) =  l;												//Dummy Vector containing user id's (from 0 to CUSTOMERS_MAX-2)
	}
	Dummy_Matrix.block(0,0,CUSTOMERS_MAX-1,Profile_Length);					//Specify Dummy Vector to contain Profile Vector coordinates
	Dummy_Matrix.col(Profile_Length).tail(CUSTOMERS_MAX-1) = Dummy_Vector;	//include column in Dummy Matrix corresponding to respected user id's

	/*****************************KD-TREE ALGORITHM IMPLEMENTATION*****************************/
	typedef KDTreeEigenMatrixAdaptor< MatrixXf>  my_kd_tree_t;				//Define kd-tree
	const size_t max_leaf	= 10;											//Specify number of leaf nodes
	const size_t dim		= Profile_Length;								//Specify the dimension of the Profile Space
	int jk					= Profile_Vector.rows();						//variable used to update the number of rows specified in Profile_Vector

	/*****************************K NEAREST NEIGHBOR SEARCH*****************************/
	size_t				num_results = (CUSTOMERS_MAX-1)/Cust_classes;			//Define number of returned Nodes
	vector<size_t>		ret_indexes(num_results);							//Vector containg the indexes of the returned results
	vector<float>		out_dists_sqr(num_results);							//Vector containing the distances of each node from the querry point
	int					k=0;

	nanoflann::KNNResultSet<float> resultSet(num_results);					//Generate Result Set

	while (Norm_List != NULL){												//Implement the KNN Search while there are still elemts in the List			

		my_kd_tree_t	profile_index(dim, Profile_Vector, max_leaf );		//Implement updated Profile Vector Matrix//REIMPLEMENT THE TREE THROUGH THE LOOP!!!!!!!!!!!!!!!!!!
		profile_index.index->buildIndex();

		for (i=0;i<Profile_Length;i++) {
				query_pt[i] = Profile_Vector.row(0)[i];							//Assign Querry Point as first row of profile vector matrix
		}

		resultSet.init(&ret_indexes[0], &out_dists_sqr[0] );					//Initialise Result Set
		profile_index.index->findNeighbors(resultSet, &query_pt[0], nanoflann::SearchParams(10));	//Find set of nearest neighbors to vec[0:dim-1]. 

		std::cout << "knnSearch(nn="<<num_results<<"): \n";
		/*
		for (size_t i=0;i<num_results;i++) {
			std::cout << "ret_index["<<i<<"]=" << ret_indexes[i] << " out_dist_sqr=" << out_dists_sqr[i] << endl;
		}*/
	
		sort(ret_indexes.begin(),	ret_indexes.end(),	 greater<int>());		//Sort returned Profile Vector indexes in descending order
		sort(out_dists_sqr.begin(), out_dists_sqr.end(), greater<int>());

		for (size_t i=0;i<num_results;i++) {
			int_hash_table->insert(Dummy_Matrix.row(ret_indexes[i])[Profile_Length], out_dists_sqr[i], k);
			deleteFromList(Dummy_Matrix.row(ret_indexes[i])[Profile_Length],Norm_List);//Remove all points associated with Querry Point
		
			for(j = ret_indexes[i];j<jk-1;j++){									//Indexes only go up to MAX-1 (since we copy the next one into the current)
				Profile_Vector.row(j)	= Profile_Vector.row(j+1);
				Dummy_Matrix.row(j)		= Dummy_Matrix.row(j+1);
			}
		}

		jk = jk-num_results;
			if (jk<num_results){//Compensate for the left over values
				num_results = jk;
				ret_indexes.resize(num_results);
				out_dists_sqr.resize(num_results);
			}

		k++;
	
		Profile_Vector.conservativeResize	(jk,Profile_Length);
		Dummy_Matrix.conservativeResize		(jk,Profile_Length+1);
	}

	table		= int_hash_table->getTable();

	this->Class_Sorting(table);
	this->Voting_Algorithm(table);
}