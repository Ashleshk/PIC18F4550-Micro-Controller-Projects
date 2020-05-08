ORG 0000H
 	             		LJMP MAIN
     		ORG 0100H
                       	    
 	     	MAIN:   MOV A ,#00H      ;INITAILIZE
      		HERE:   MOV P2,A 
                     			INC A 
                      		CJNE A,#0FFH,HERE
                       	                      
      		NXT:    MOV P2, A
                      		DEC A 
                      		CJNE A , #00H ,NXT
                    		SJMP HERE
                      		      		END
