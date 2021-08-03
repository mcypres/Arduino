
/*	
If ReadSerialPortData( port, @inp, 14 )
			
	If AddElement( *this\sample() )
		*this\sample()\range 		= Asc(Mid(inp,  1))  ; RANGE
		*this\sample()\digits		= Mid( inp, 2,  5 )  ; DIGITS
		*this\sample()\function 	= Asc(Mid(inp,  7 )) ; FUNCTION
		*this\sample()\status		= Asc(Mid(inp,  8 )) ; STATUS
		
		*this\sample()\option[0]	= Asc(Mid(inp,  9 )) ; OPTION 1
		*this\sample()\option[1]	= Asc(Mid(inp, 10 )) ; OPTION 2
		*this\sample()\option[2]	= Asc(Mid(inp, 11 )) ; OPTION 2
	EndIf
			
EndIf*/

I believe the last two digits are the “end of packet” limiter (CRLF) however I currently cannot test this, I wrote the code a long time ago and I forgot to comment on this slight detail, but as you can see I’ve defined the CRLF contants, so it must be there.
A list of constants:

Here are some constants from my UT61e (unpublished) library…

#FUNCTION_VOLTAGE 		= %0111011
#FUNCTION_AUTO_CURRENT_UA 	= %0111101
#FUNCTION_AUTO_CURRENT_MA 	= %0111111
#FUNCTION_CURRENT_22A		= %0110000
#FUNCTION_CURRENT_MANUAL_A	= %0111001
#FUNCTION_OHMS			= %0110011 
#FUNCTION_CONTINUITY		= %0110101
#FUNCTION_DIODE			= %0110001 
#FUNCTION_FREQUENCY		= %0110010
#FUNCTION_CAPACITANCE		= %0110110
#FUNCTION_TEMPERATURE		= %0110100
#FUNCTION_ADP			= %0111110

#RANGE_ONE			= %0110000 //; Example: 22.000nF
#RANGE_TWO			= %0110001 //; Example: 220.00nF
#RANGE_THREE			= %0110010 //; Example: 2.2000µF
#RANGE_FOUR			= %0110011 //; Example: 22.000µF
#RANGE_FIVE			= %0110100 //; Example: 220.00µF
#RANGE_SIX			= %0110101 //; Example: 2.2000mF
#RANGE_SEVEN			= %0110110 //; Example: 22.000mF	
#RANGE_EIGHT			= %0110111 //; Example: 220.00mF

#DIGIT_0			= %0110000
#DIGIT_1			= %0110001
#DIGIT_2			= %0110010
#DIGIT_3			= %0110011
#DIGIT_4			= %0110100
#DIGIT_5			= %0110101
#DIGIT_6			= %0110110 
#DIGIT_7			= %0110111 
#DIGIT_8			= %0111000
#DIGIT_9			= %0111001

#STATUS_OL			= 1 < < 0
#STATUS_BATT			= 1 << 1
#STATUS_SIGN			= 1 << 2
#STATUS_JUDGE			= 1 << 3

#OPTION1_RMR			= 1 << 0
#OPTION1_REL			= 1 << 1
#OPTION1_MIN			= 1 << 2
#OPTION1_MAX			= 1 << 3

#OPTION2_0			= 1 << 0
#OPTION2_PMIN			= 1 << 1
#OPTION2_PMAX			= 1 << 2
#OPTION2_UL			= 1 << 3

#OPTION3_VAHZ			= 1 << 0
#OPTION3_AUTO			= 1 << 1
#OPTION3_AC			= 1 << 2
#OPTION3_DC			= 1 << 3

#UT_CR				= %0001101
#UT_LF				= %0001010
	
#FUNCTION_VOLTAGE 		= %0111011
#FUNCTION_AUTO_CURRENT_UA 	= %0111101
#FUNCTION_AUTO_CURRENT_MA 	= %0111111
#FUNCTION_CURRENT_22A		= %0110000
#FUNCTION_CURRENT_MANUAL_A	= %0111001
#FUNCTION_OHMS			= %0110011 
#FUNCTION_CONTINUITY		= %0110101
#FUNCTION_DIODE			= %0110001 
#FUNCTION_FREQUENCY		= %0110010
#FUNCTION_CAPACITANCE		= %0110110
#FUNCTION_TEMPERATURE		= %0110100
#FUNCTION_ADP			= %0111110
 
#RANGE_ONE			= %0110000 ; Example: 22.000nF
#RANGE_TWO			= %0110001 ; Example: 220.00nF
#RANGE_THREE			= %0110010 ; Example: 2.2000µF
#RANGE_FOUR			= %0110011 ; Example: 22.000µF
#RANGE_FIVE			= %0110100 ; Example: 220.00µF
#RANGE_SIX			= %0110101 ; Example: 2.2000mF
#RANGE_SEVEN			= %0110110 ; Example: 22.000mF	
#RANGE_EIGHT			= %0110111 ; Example: 220.00mF
 
#DIGIT_0			= %0110000
#DIGIT_1			= %0110001
#DIGIT_2			= %0110010
#DIGIT_3			= %0110011
#DIGIT_4			= %0110100
#DIGIT_5			= %0110101
#DIGIT_6			= %0110110 
#DIGIT_7			= %0110111 
#DIGIT_8			= %0111000
#DIGIT_9			= %0111001
 
#STATUS_OL			= 1 < < 0
#STATUS_BATT			= 1 << 1
#STATUS_SIGN			= 1 << 2
#STATUS_JUDGE			= 1 << 3
 
#OPTION1_RMR			= 1 << 0
#OPTION1_REL			= 1 << 1
#OPTION1_MIN			= 1 << 2
#OPTION1_MAX			= 1 << 3
 
#OPTION2_0			= 1 << 0
#OPTION2_PMIN			= 1 << 1
#OPTION2_PMAX			= 1 << 2
#OPTION2_UL			= 1 << 3
 
#OPTION3_VAHZ			= 1 << 0
#OPTION3_AUTO			= 1 << 1
#OPTION3_AC			= 1 << 2
#OPTION3_DC			= 1 << 3
 
#UT_CR				= %0001101
#UT_LF				= %0001010
