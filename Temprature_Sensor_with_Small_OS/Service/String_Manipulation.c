/*- INCLUDES ----------------------------------------------*/
#include"Data_types.h"
#include<math.h>
/*- LOCAL MACROS ------------------------------------------*/
/*- LOCAL Dataypes ----------------------------------------*/
/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
/*- GLOBAL STATIC VARIABLES -------------------------------*/
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/

/*- APIs IMPLEMENTATION -----------------------------------*/
char_t* TransformIntToString(long long int Number)
{
	static char_t Int_String[100]={0};
    uint8_t digits=0;
    while(Number/(long long int)pow(10,digits++));
    digits--;
    uint8_t index=0;
    while (Number) 
    { 
        Int_String[(digits-1)-index] = (Number%10) + '0'; 
        Number= Number/10; 
        index++;
    } 
	Int_String[index]='\0';
    return Int_String;
}