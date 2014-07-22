#ifndef Linked_List_H
#define Linked_List_H
#include "stdlib.h"

//===================================================================
//
// Constants and Type Declarations
//
//===================================================================
#define			Cust_classes	500		//Partition of users into K disjoint classes
#define			Mov_classes		50			//Partition of movies into K' disjoint classes

struct Node
{
	int			Cust_ID;
	float		Eucl_Norm;
	Node*		Next;
};

typedef Node*	NodePtr;

//======================FUNCTION PROTOTYPES======================
void			addToList		(int id, float norm, Node* &hdList);
void			deleteFromList	(int id, Node* &hdList);
int				returnClass		(int id, Node* &hdList);

//======================HASH TABLE======================
class intHashTable{
private:
	int			size;
	Node**		table;										//create an array of pointers
public:
				intHashTable(int size);						// construct a new hash table with size elements
				~intHashTable();							// delete the memory for all internal components
	void		insert(int id,float norm, int class_number);// insert num into the hash table, no effect if num is already in table
	void		remove(int num);							// remove num from the hash table, no effect if not in table
	int			lookup(int num);							// return 1 if num is already in table, 0 otherwise
	void		print(void);								// print the elements of the hash table to the screen
	Node**		getTable();									// returns the table, i.e the array of lists
};
//======================================================

#endif 