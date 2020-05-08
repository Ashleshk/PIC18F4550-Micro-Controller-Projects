ORG 0000H
              		LJMP MAIN
     		ORG 0030H
                             
      	 	MAIN:  MOV R3,#3
       		UP:    	MOV  P2,#03H
                      		ACALL DELAY
                      		MOV P2, #06H
                      		ACALL DELAY
                      		MOV P2,#0CH
                      		ACALL DELAY
                     		MOV P2, #09H
                      		ACALL DELAY
                      		DJNZ R3 , UP
							SJMP $
                      	                     
      		DELAY:  MOV R1,#10H
      		TOP:    MOV R0,#90H
      		UPP:    MOV R2,#0FFH
      		AGN:    DJNZ R2,AGN
                     		DJNZ R0,UPP
                      		DJNZ R1,TOP
                     		RET
                      	     END


