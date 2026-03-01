








//2str
//int
#define ssz_i(i) ssz__(ssz__i(i,&dh_is_error))
//uint
#define ssz_u(i) ssz__(ssz__u(i,&dh_is_error))
//hex
#define ssz_hex(i) ssz__(ssz__hex(i,&dh_is_error))
#define ssz_HEX(i) ssz__(ssz__HEX(i,&dh_is_error))
//double
#define ssz_d
//.count
#define ssz_d_
//float
#define ssz_f(f) ssz__(ssz__f(f,&dh_is_error))
#define ssz_F(f) ssz__(ssz__F(f,&dh_is_error))
//float without extra zeros
#define ssz_g(f) ssz__(ssz__g(f,&dh_is_error))
#define ssz_G(f) ssz__(ssz__G(f,&dh_is_error))

//.count
#define ssz_f_
//64bit int uint
#define ssz_i64
#define ssz_u64




#define char_i(i)
#define assci_at(s,i)
//system time and date
#define ssz_time
#define ssz_date


//basic like

#define ssz_left(s,n) ssz__(LMR(s,0,n,0,&dh_is_error))
#define ssz_right(s,n) ssz__(LMR(s,0,n,1,&dh_is_error))
#define ssz_mid(s,m,n) ssz__(LMR(s,m,n,0,&dh_is_error))
#define ssz_mids(s,m) ssz__(LMR(s,0,strlen(s)-m,1,&dh_is_error))

// space/fill/set(c,n) upper lower  
//len = strlen work fine 


#define ssz___def(func_name,name_t,string_format)  char * func_name(name_t amount,int * is_error);
ssz___def(ssz__f,float,"%f")
ssz___def(ssz__F,float,"%f")
ssz___def(ssz__g,float,"%g")
ssz___def(ssz__G,float,"%G")

ssz___def(ssz__i,int,"%i")
ssz___def(ssz__u,unsigned int,"%u")
ssz___def(ssz__hex,unsigned int,"%h")
ssz___def(ssz__HEX,unsigned int,"%H")



//----------------


uint16_t * uuz__copy_malloc(const uint16_t * s,int * is_error);
uint16_t * uuz__left_malloc(const uint16_t * s , int cc,int * is_error);
#define uuz_1(s)  uuz__(uuz__copy_malloc(s,&dh_is_error))


///------
/*
ttz_ -> ssz and uuz 

*/
