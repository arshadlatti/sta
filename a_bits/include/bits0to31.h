

#define A_BIT_0   0x1
#define A_BIT_1   0x2
#define A_BIT_2   0x4
#define A_BIT_3   0x8
#define A_BIT_4   0x10
#define A_BIT_5   0x20
#define A_BIT_6   0x40
#define A_BIT_7   0x80
#define A_BIT_8   0x100
#define A_BIT_9   0x200
#define A_BIT_10   0x400
#define A_BIT_11   0x800
#define A_BIT_12   0x1000
#define A_BIT_13   0x2000
#define A_BIT_14   0x4000
#define A_BIT_15   0x8000
#define A_BIT_16   0x10000
#define A_BIT_17   0x20000
#define A_BIT_18   0x40000
#define A_BIT_19   0x80000
#define A_BIT_20   0x100000
#define A_BIT_21   0x200000
#define A_BIT_22   0x400000
#define A_BIT_23   0x800000
#define A_BIT_24   0x1000000
#define A_BIT_25   0x2000000
#define A_BIT_26   0x4000000
#define A_BIT_27   0x8000000
#define A_BIT_28   0x10000000
#define A_BIT_29   0x20000000
#define A_BIT_30   0x40000000
#define A_BIT_31   0x80000000


#define A_MASK_BIT_0    0xFFFFFFFE
#define A_MASK_BIT_1    0xFFFFFFFD
#define A_MASK_BIT_2    0xFFFFFFFB
#define A_MASK_BIT_3    0xFFFFFFF7
#define A_MASK_BIT_4    0xFFFFFFEF
#define A_MASK_BIT_5    0xFFFFFFDF
#define A_MASK_BIT_6    0xFFFFFFBF
#define A_MASK_BIT_7    0xFFFFFF7F
#define A_MASK_BIT_8    0xFFFFFEFF
#define A_MASK_BIT_9    0xFFFFFDFF
#define A_MASK_BIT_10   0xFFFFFBFF
#define A_MASK_BIT_11   0xFFFFF7FF
#define A_MASK_BIT_12   0xFFFFEFFF
#define A_MASK_BIT_13   0xFFFFDFFF
#define A_MASK_BIT_14   0xFFFFBFFF
#define A_MASK_BIT_15   0xFFFF7FFF
#define A_MASK_BIT_16   0xFFFEFFFF
#define A_MASK_BIT_17   0xFFFDFFFF
#define A_MASK_BIT_18   0xFFFBFFFF
#define A_MASK_BIT_19   0xFFF7FFFF
#define A_MASK_BIT_20   0xFFEFFFFF
#define A_MASK_BIT_21   0xFFDFFFFF
#define A_MASK_BIT_22   0xFFBFFFFF
#define A_MASK_BIT_23   0xFF7FFFFF
#define A_MASK_BIT_24   0xFEFFFFFF
#define A_MASK_BIT_25   0xFDFFFFFF
#define A_MASK_BIT_26   0xFBFFFFFF
#define A_MASK_BIT_27   0xF7FFFFFF
#define A_MASK_BIT_28   0xEFFFFFFF
#define A_MASK_BIT_29   0xDFFFFFFF
#define A_MASK_BIT_30   0xBFFFFFFF
#define A_MASK_BIT_31   0x7fffffff

 //BIT_ mask       or  eax, 0x80000000 (set)  -> and eax, 0x7fffffff  mask(reset)
 // and if(a & BIT_31) //test 
 
