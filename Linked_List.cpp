#include "Linked_List.h"

/*****************************addToList()*****************************/

//-------------------------------------------------------------------
//Assign Dynamic Memory to new node and update values. The new node is 
//then inserted at the head of the list.
//-------------------------------------------------------------------

/*****************************addToList()*****************************/
void addToList(int id, float norm, Node* &head){
	NodePtr newNode	   = (Node*)malloc(sizeof(Node));//sizeof(NodePtr)
	newNode->Cust_ID   = id;											
	newNode->Eucl_Norm = norm;
	newNode->Next	   = head;
	head = newNode;		
}

/*****************************deleteFromList()*****************************/
void deleteFromList(int id,Node* &head){

	bool found = 0;
	NodePtr oldPtr		= (Node*)malloc(sizeof(Node));
	NodePtr searchPtr	= (Node*)malloc(sizeof(Node));
	NodePtr lastPtr	= (Node*)malloc(sizeof(Node));
	NodePtr current		= head;// first will have the node after deletion

	if (head == NULL)
		return;
	else if (head->Cust_ID == id){
		oldPtr = head;
		head = head->Next;
		free (oldPtr);
	}
	else {
		found = 0;
		searchPtr = head;
		lastPtr = head;
		while ((searchPtr != NULL) && (! found)){
			if (searchPtr->Cust_ID == id){
				found = 1;
				lastPtr->Next = searchPtr->Next;
				free (searchPtr);
			}
			else{
				lastPtr = searchPtr;
				searchPtr = searchPtr->Next;
			}
		}
	}
}

/*****************************returnClass()*****************************/
int returnClass(int id, Node* &head){
	
	bool found = 0;
	NodePtr searchPtr	= (Node*)malloc(sizeof(Node));

	if (head == NULL)
		return 0;
	else if (head->Cust_ID == id){
		return head->Eucl_Norm;
	}
	else{
		found = 0;
		searchPtr = head;
		while ((searchPtr != NULL) && (! found)){
			if (searchPtr->Cust_ID == id){
				found = 1;
				return searchPtr->Eucl_Norm;
			}
			else{
				searchPtr = searchPtr->Next;
			}
		}
	}
}
/*****************************intHashTable*****************************/

//-------------------------------------------------------------------
//construct a new hash table with class_size elements
//-------------------------------------------------------------------

/*****************************intHashTable*****************************/
intHashTable::intHashTable(int class_size){	
  size = class_size;
  table = new Node*[size];
  for ( int i = 0; i < size; i++ ){
	  table[i] = NULL;
  }
}

intHashTable::~intHashTable(){
   for(int i=0; i<size; i++){
      Node* temp = table[i];
	  while(temp != NULL){
         Node* next = temp->Next;
         delete temp;
         temp = next;
      }
   }
   size = 0;
   delete[] table;
}

void intHashTable::insert(int id,float norm,int class_number){  
	NodePtr runner = table[class_number];
	addToList(id, norm, table[class_number]);
}

Node** intHashTable::getTable(){
	return table;
}

