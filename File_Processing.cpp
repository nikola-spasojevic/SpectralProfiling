#include "Project_Engine_Head.h"

/*****************************File_Processing()*****************************/

//-------------------------------------------------------------------
//By reading in each individual text file from the training directory
//we aim to process each of them by initially reading in the 
//movie id (first line), followed by the ratings placed by each 
//customer.
//-------------------------------------------------------------------

//-------------------------------------------------------------------
//The files are loaded in the following format:
//   <MovieId>:
//   <CustomerId>,<Rating>
//   <CustomerId>,<Rating>
//   ...
//-------------------------------------------------------------------

/*****************************File_Processing()*****************************/

void Project_Engine::File_Processing(wchar_t* File_data)
{
    FILE *Movie_File;
    wchar_t Text_Buffer[1000];
    wsprintf(Text_Buffer,TRAINING_FILE,File_data);							//Writes formatted data to the specified buffer.
	
	int custId, movieId, rating, pos = 0;
    
    //wprintf(L"Processing file: %ls \n", Text_Buffer);

    if (_wfopen_s(&Movie_File, Text_Buffer, L"r") != 0) return;

	//First row is the Movie ID
    fgetws(Text_Buffer, 1000, Movie_File);								

    Parse_Str_Int(Text_Buffer, (int)wcslen(Text_Buffer), pos, movieId);		//Update movieId	//Get wide string length

    Movie_spec[movieId].Movie_RateCount = 0;
    Movie_spec[movieId].Movie_RateSum = 0;

    //The remaining rows consist of customer ID's and their corresponding ratings
    fgetws(Text_Buffer, 1000, Movie_File);									//Get new line: Customer_ID,Rating,YYYY-MM-DD
    while (!feof(Movie_File))
    {
        pos = 0;
        Parse_Str_Int(Text_Buffer, (int)wcslen(Text_Buffer), pos, custId);	//Convert from string to integer - Customer_ID
        Parse_Str_Int(Text_Buffer, (int)wcslen(Text_Buffer), pos, rating);	//Convert from string to integer - Rating
        
        Rating_spec[upRating_Count].Movie_ID = (short)movieId;
        Rating_spec[upRating_Count].Cust_ID = custId;
        Rating_spec[upRating_Count].Rating = (BYTE)rating;	
        Rating_spec[upRating_Count].Cache = 0;
        upRating_Count++;													//Current number of loaded ratings

        fgetws(Text_Buffer, 1000, Movie_File);								//Get new line: Customer_ID,Rating,YYYY-MM-DD
    }

    fclose(Movie_File);
}