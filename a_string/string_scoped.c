










/* double number_double_from_string(const char * s)
{
	    char *endPtr;
    double r = strtod(s, &endPtr);
    
    if (endPtr == str)  r = 0;
	
	return r;
	
} */


/* char * ssz__f(float amount,int * is_error)
{
int len = snprintf(NULL, 0, "%f", amount);
char *result = malloc(len + 1);
if(result)
snprintf(result, len + 1, "%f", amount);
else
	*is_error = STA_TRUE;
return result;

} */

//ssz uuz 

#define ssz___t(func_name,name_t,string_format)  char * func_name(name_t amount,int * is_error) \
{ \
int len = snprintf(NULL, 0, string_format, amount); \
if (len < 0){* is_error = STA_TRUE; return 0;} \
char *result = (char*)malloc(len + 1); \
if(result){result[0]=0; \
snprintf(result, len + 1, string_format, amount);} \
else \
	*is_error = STA_TRUE; \
return result; \
}

typedef unsigned int uint_sys_t;
ssz___t(ssz__f,float,"%f")
ssz___t(ssz__F,float,"%f")
ssz___t(ssz__g,float,"%g")
ssz___t(ssz__G,float,"%G")

ssz___t(ssz__i,int,"%i")
ssz___t(ssz__u,unsigned int,"%u")
ssz___t(ssz__hex,unsigned int,"%h")
ssz___t(ssz__HEX,unsigned int,"%H")

/* specifier	Output	Example
d or i	Signed decimal integer	392
u	Unsigned decimal integer	7235
o	Unsigned octal	610
x	Unsigned hexadecimal integer	7fa
X	Unsigned hexadecimal integer (uppercase)	7FA
f	Decimal floating point, lowercase	392.65
F	Decimal floating point, uppercase	392.65
e	Scientific notation (mantissa/exponent), lowercase	3.9265e+2
E	Scientific notation (mantissa/exponent), uppercase	3.9265E+2
g	Use the shortest representation: %e or %f	392.65
G	Use the shortest representation: %E or %F	392.65
a	Hexadecimal floating point, lowercase	-0xc.90fep-2
A	Hexadecimal floating point, uppercase	-0XC.90FEP-2
c	Character	a
s	String of characters	sample
p	Pointer address	b8000000
n	Nothing printed.
The corresponding argument must be a pointer to a signed int.
The number of characters written so far is stored in the pointed location.	
%	A % followed by another % character will write a single % to the stream.	% */






//-----------------------


uint16_t * uuz__copy_malloc(const uint16_t * s,int * is_error)
{
	return uuz__left_malloc(s,wcslen((const wchar_t*)s),is_error);
}
uint16_t * uuz__left_malloc(const uint16_t * s , int cc,int * is_error)
{
	uint16_t * r = (uint16_t*) malloc((cc+1)*sizeof(uint16_t));
	if(r)
	{
		if(cc)
	wcsncpy(( wchar_t*)r,(const wchar_t*)s,cc);
	r[cc]=0;
	}
	else
		*is_error = 1;
	return r;
}

