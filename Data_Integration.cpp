#include "Project_Engine_Head.h"

/*****************************Data_Integratrion()*****************************/

//-------------------------------------------------------------------
// Loop through all of the files in the training directory - TRAINING SET
//-------------------------------------------------------------------

/*****************************Data_Integratrion()*****************************/

void Project_Engine::Data_Integratrion()
{
    WIN32_FIND_DATA Training_Data;
    HANDLE hFind;
    bool bContinue = true;
    int count = 0; // TEST

    hFind = FindFirstFile(TRAINING_DIRECTORY, &Training_Data);	
   // if (hFind == INVALID_HANDLE_VALUE) return;
    
    while (bContinue)
	{			
        this->File_Processing(Training_Data.cFileName);	//"this" keyword is a pointer to the current instance of the class in a function.
		printf("%s", TRAINING_FILE);
		bContinue = (FindNextFile(hFind, &Training_Data) != 0);

        if (++count > 1000) break; // count used to test algorithm on X movies
    } 

    FindClose(hFind);
}