ORG 0000H
	MOV DPTR ,#1000
	MOV P1,#00H
	MOV P2,#00H
	MOV R1,#00H
	MOV R0,#00H
	
	
	L1: MOV A , R0
		MOVC A ,@A+DPTR
		INC R0
		CJNE R0,#0AH,L1
		MOV R0,#00H
		SETB P1.0
		MOV P0,A
		ACALL DELAY
		CLR P1.0
		
		MOV A ,R1
		MOVC A, @A+DPTR
		INC R1
		CJNE R1,#0AH,L1
		MOV R1,#00H
		SETB P1.1
		MOV P0,A
		ACALL DELAY
		CLR P1.1
		SJMP $	
			
DELAY:  MOV R1,#10H
      		TOP:    MOV R0,#90H
      		UPP:    MOV R2,#0FFH
      		AGN:    DJNZ R2,AGN
                      		DJNZ R0,UPP
                    		DJNZ R1,TOP
                    		RET

ORG 1000H
     		DB 0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H

END