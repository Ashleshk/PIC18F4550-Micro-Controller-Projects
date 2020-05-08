ORG 0000H
 			LJMP MAIN
ORG 0100H
MAIN:   MOV A,#00H
        MOV P2 , A
        ACALL DELAY
              
         
        MOV P2,#0FFH
        ACALL DELAY
		SJMP MAIN
                   
DELAY:
               
		MOV R2 ,#10H
	UP:     MOV R3,#0FFH
  			AGN:DJNZ R3,AGN
     			DJNZ R2, UP
            
      			RET
				END
