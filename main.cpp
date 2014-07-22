#include "Project_Engine_Head.h"

int _tmain(int argc, _TCHAR* argv[])
{
    Project_Engine* project_engine = new Project_Engine();	//Initialization

    project_engine->Data_Integratrion();					//Open directory containing TRAINING_FILE and update variables with Movie-User data into Rating_spec[] array.
    
	project_engine->Mapping();								//Process the training data and Map customer id's to a compact array
    
	project_engine->Matlab_execution();						//Generate graphs corresponding to user/movie rating specifications

	project_engine->User_Profiling();						//Implement Profile Vector Algorithm

	wprintf(L"\nTerminate Program\n");
    getchar();
    
    return 0;
}