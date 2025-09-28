.include "xc.inc"

.text                       ;BP (put the following data in ROM(program memory))

; This is a library, thus it can *not* contain a _main function: the C file will
; define main().  However, we will need a .global statement to make available ASM
; functions to C code.
; All functions utilized outside of this file will need to have a leading 
; underscore (_) and be included in a comment delimited list below.
.global _example_public_function, _second_public_function
    
.global _write_0, _write_1, _wait_100us, _wait_1ms

;Sends a 0 bit by generating a short high pulse and then a longer low pulse
_write_0:
    inc LATA                 
    repeat #3               
    nop                     
    
    clr LATA                
    repeat #4               
    nop                     
    
    return                   
; Sends a 1 bit by generating a longer high pulse and then by a short low pulse
_write_1:
    inc LATA                 
    repeat #10               
    nop 
    
    clr LATA   
    
    return


_wait_100us:
    push   w8
    mov    #0x20E, w8  
    loop_100us:
	dec    w8, w8
	bra    nz, loop_100us  
	pop    w8
	return
_wait_1ms:
    push   w8
    mov    #0x2710, w8  
    loop_1ms:
	dec    w8, w8
	bra    nz, loop_1ms  
	pop    w8
	return
    




