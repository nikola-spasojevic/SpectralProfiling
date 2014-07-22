#include "Project_Engine_Head.h"

/*****************************ReadNumber()*****************************/

//-------------------------------------------------------------------
//Traverses through the line of text, and when a number is discovered a 
//sequence is started. The corresponding numbers are then written to the
//output buffer.
//This includes: Movie ID, Customer ID and Date (YYYY-MM-DD).
//-------------------------------------------------------------------

/*****************************ReadNumber()*****************************/

bool Project_Engine::ReadNumber(wchar_t* Text_BufferIn, int Line_length, int &nPosition, wchar_t* TextOut_Buffer)
{
    int count = 0;
    int start = nPosition;
    wchar_t number = 0;

    // Find start of number
    while (start < Line_length)
    {
        number = Text_BufferIn[start];
        if ((number >= 48 && number <= 57) || (number == 45)) break;	//48<=wc=57 corresponds to numbers 0 - 9 in ASCII Table format
        start++;														//If the corresponding character is a number, break
    }

    nPosition = start;													//Each character of the corresponding number is then copied into the output buffer
    while (nPosition < Line_length && ((number >= 48 && number <= 57) || number == 69  || number == 101 || number == 45 || number == 46))
    {
        TextOut_Buffer[count++] = number;
        number = Text_BufferIn[++nPosition];
    }

    TextOut_Buffer[count] = 0;											// Null terminate and return
    return (count > 0);
}

/*****************************Parse_Str_Int()*****************************/

//-------------------------------------------------------------------
//Parsing a string input into an integer output. The ouput corresponds
//to the Movie ID, Customer ID and Rating.
//-------------------------------------------------------------------

/*****************************Parse_Str_Int()*****************************/

bool Project_Engine::Parse_Str_Int(wchar_t* Text_Buffer, int Line_length, int &nPosition, int& nValue)//Buffer,Length of line,Position,ReturnValue
{
    wchar_t pwzNumber[20];
    bool bResult = ReadNumber(Text_Buffer, Line_length, nPosition, pwzNumber);
    nValue = (bResult) ? _wtoi(pwzNumber) : 0;
    return bResult;
}

bool Project_Engine::Parse_Str_Float(wchar_t* Text_Buffer, int Line_length, int &nPosition, float& fValue)
{
    wchar_t pwzNumber[20];
    bool bResult = ReadNumber(Text_Buffer, Line_length, nPosition, pwzNumber);
    fValue = (bResult) ? (float)_wtof(pwzNumber) : 0;
    return false;
}
