#INCLUDE <P16F877A.INC>

RES_VECT  CODE    0x0000            ; processor reset vector
    GOTO    START                   ; go to beginning of program


MAIN_PROG CODE                      ; let linker place main program

START
    BSF STATUS, RB0
    CLRF TRISD
    BCF STATUS, RP0
    CLRF PORTD
    
MAIN
    BSF PORTD, 0
    CALL DELAY
    BCF PORTD, 0
    CALL DELAY
    GOTO MAIN
    
DELAY
    MOVLW 0xFF
    MOVWF 0x20
    MOVLW 0xFF
    MOVWF 0x21
    MOVLW 0x02
    MOVWF 0x22
    
LOOP1
    DECFSZ 0x20,1
    GOTO LOOP1
    DECFSZ 0x21,1
    GOTO LOOP1
    DECFSZ 0x22,1
    GOTO LOOP1
RETURN
    

    END