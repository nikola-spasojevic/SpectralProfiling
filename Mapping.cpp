#include "Project_Engine_Head.h"

/*****************************Mapping()*****************************/

//-------------------------------------------------------------------
//This function traverses through the newly updated ratings array and 
//assigns specifications to each according Movie and Customer.
//This includes: Rating count, Rating Sum and Rating Average of each
//individual movie and customer.
//-------------------------------------------------------------------

//-------------------------------------------------------------------
//The implementation of the map class allows us to use an associative
//array in order to assign different ID's to each customer. 
//-------------------------------------------------------------------

/*****************************Mapping()*****************************/

void Project_Engine::Mapping()
{
    int i, new_cust_ID;
    ID_iterator itr;

    //Traverse through the TRAINING SET and process each row
    for (i=0; i<upRating_Count; i++)
    {
        Struct_Rating* rating = Rating_spec + i;						//access each rating from the list

	//Increment Movie Specifications
        Movie_spec[rating->Movie_ID].Movie_RateCount++;					//Movie_spec[0] is undefined, no movie with ID = 0
        Movie_spec[rating->Movie_ID].Movie_RateSum += rating->Rating;	

//-------------------------------------------------------------------
// Mapping
//-------------------------------------------------------------------

		//Update customer list by using mapped_custID to re-number id's to array indexes
        itr = mapped_custID.find(rating->Cust_ID);						//Returns an Iterator to the element with a key equivalent to CustId

        if (itr == mapped_custID.end())									//If the element is not found, itr is returned as  map::end	
        {
            new_cust_ID = 1 + (int)mapped_custID.size();

            mapped_custID[rating->Cust_ID] = new_cust_ID;				// Reserve new id and add lookup

            Customer_spec[new_cust_ID].Cust_ID = rating->Cust_ID;		// Store off old sparse id for later

            Customer_spec[new_cust_ID].Cust_RateCount = 0;				//Initialise Customer_spec components
            Customer_spec[new_cust_ID].Cust_RateSum = 0;
        }
        else
        {
            new_cust_ID = itr->second;									//If found, assign customer ID to newly assigned array ID 
        }

        rating->Cust_ID = new_cust_ID;									//Swap sparse ID list for ordered array list
		
		//Increment Customer Specifications
        Customer_spec[new_cust_ID].Cust_RateCount++;					//Customer_spec[0] is undefined, no customer with ID = 0
        Customer_spec[new_cust_ID].Cust_RateSum += rating->Rating;
    }

	//Calculate Movie Average of each movie
	wprintf(L"\nMovie Averages Listing:\n");
    for (i=1; i<MOVIES_MAX; i++)
    {
        Struct_Movie* movie = Movie_spec+i;
        movie->Movie_RateAvg = movie->Movie_RateSum / (1.0 * movie->Movie_RateCount);
		movie->Movie_PsAvg = (3.23 * 25 + movie->Movie_RateSum) / (25.0 + movie->Movie_RateCount);
		cout<<movie->Movie_RateAvg <<endl;								//Output Movie Rating Average
    }

	//Calculate Customer Average of each movie	
	for (i=1; i<CUSTOMERS_MAX; i++)
    {
        Struct_Customer* customer = Customer_spec+i;
		customer->Cust_RateAvg = customer->Cust_RateSum /(1.0 * customer->Cust_RateCount);
		//cout<<customer->Cust_RateAvg <<endl;							//Output Customer Rating Average
    }

}