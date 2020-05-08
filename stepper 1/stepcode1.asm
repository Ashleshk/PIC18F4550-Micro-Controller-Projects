MOV A, #66H    ; LOAD THE STEP SEQUENCE
                       		     
      	BACK :
     		MOV P1, A    			; LOAD SEQUENCE TO PORT
    		RL A        ; CHANGE SEQUENCE ROTATE CLOCKWISE
     		ACALL DELAY    		; WAIT FOR IT
			ACALL DELAY
			ACALL DELAY
     		SJMP BACK    		; NOW KEEP GOING
                       	      
     	DELAY :
     		MOV R2, #100
                      	     
      		H1 :
      		MOV R3, #255
                           
     		 H2 :
     		DJNZ R3, H2
     		DJNZ R2, H1
      		RET
     
                           	END   
