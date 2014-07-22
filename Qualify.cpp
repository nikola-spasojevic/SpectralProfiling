#include "Project_Engine_Head.h"
#include "engine.h"
#include "Linked_List.h"

//
// ProcessTest
// - Load a sample set in the following format
//
//   <Movie1Id>:
//   <CustomerId>
//   <CustomerId>
//   ...
//   <Movie2Id>:
//   <CustomerId>
//
// - And write results:
//
//   <Movie1Id>:
//   <Rating>
//   <Rating>
//   ...
void Project_Engine::Qualify(wchar_t* pwzFile, MatrixXd rate_matrix, NodePtr Class_List, Sparse_Mat Movie_Matrix)
{
    FILE *streamIn, *streamOut;
    wchar_t pwzBuffer[1000];
    int custId, movieId, pos = 0, counter = 0;
    double predict_rating, err, rmse = 0;
	double sq = 0;
    bool bMovieRow;

    wsprintf(pwzBuffer, TEST_PATH, pwzFile);
    wprintf(L"\n\nProcessing test: %s\n", pwzBuffer);

    if (_wfopen_s(&streamIn, pwzBuffer, L"r") != 0) return;
    if (_wfopen_s(&streamOut, PREDICTION_DIRECTORY, L"w") != 0) return;

    fgetws(pwzBuffer, 1000, streamIn);
    while ( !feof( streamIn ) )
    {
        bMovieRow = false;
        for (int i=0; i<(int)wcslen(pwzBuffer); i++)
        {
            bMovieRow |= (pwzBuffer[i] == 58); 
        }

        pos = 0;
        if (bMovieRow)
        {
            Parse_Str_Int(pwzBuffer, (int)wcslen(pwzBuffer), pos, movieId);

            // Write same row to results
            fputws(pwzBuffer,streamOut); 
        }
        else
        {
            Parse_Str_Int(pwzBuffer, (int)wcslen(pwzBuffer), pos, custId);
            custId = mapped_custID[custId];

			// Predict rating and calc error
			predict_rating = rate_matrix(returnClass(custId-1,Class_List),movieId);//Rating Prediction Algorithm!!!!
			err = (Movie_Matrix.coeff(custId-1,movieId-1) - predict_rating);
            sq += err*err;
			counter++;

            // Write predicted value
            swprintf(pwzBuffer,1000,L"%5.3f\n",predict_rating);//Print predicted value!!!
            fputws(pwzBuffer,streamOut);
        }

        //wprintf(L"Got Line: %d %d %d \n", movieId, custId, rating);
        fgetws(pwzBuffer, 1000, streamIn);
    }

	rmse = sqrt(sq/counter);
    // Cleanup
    fclose( streamIn );
    fclose( streamOut );
}