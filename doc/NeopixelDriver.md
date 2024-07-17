
- SET_LED - nastavla HIGH, LOW vrednosti glede na vhodne registre (r0,1,2) - rgb barve
- SET0 - nastavi LOW stanje (64% periode high)
- SET1 - nstavi HIGH stanje (32% periode high)
- INIT_TC4 - inicializira Timer Counter 4 
- RESET - preklopi v nizko stanje za vec kot 100 mikro sec (NeoPixel dirver reset)
- INIT_IO - Inicializacija PIO krmilnika
- DIM_BLUE - zmanjsuje vrednsot regsitra modre barve (255 -> 0)

##### Main
```
.global _main
/* main program */
_main:
      .equ PIOA_BASE, 0xFFFFF400    /* Zacetek registrov za vrata C - PIOC */
      .equ PIOB_BASE, 0xFFFFF600    /* Zacetek registrov za vrata B - PIOB */
      .equ PIOC_BASE, 0xFFFFF800    /* Zacetek registrov za vrata C - PIOC */
      .equ PIO_PER,   0x00	        /* Odmiki... */
      .equ PIO_PDR,   0x04
      .equ PIO_OER,   0x10
      .equ PIO_CODR,  0x34
      .equ PIO_SODR,  0x30
      .equ PIO_ASR,   0x70
      .equ PIO_BSR,   0x74
      
      .equ PMC_BASE, 	0xFFFFFC00	/* Power Management Controller */
      .equ PMC_PCER, 	0x10  	  	/* Peripheral Clock Enable Register */
      
      .equ TC0_BASE, 	0xFFFA0000	/* TC0 Channel Base Address */
      .equ TC3_BASE, 	0xFFFDC000	/* TC3 Channel Base Address */
      .equ TC4_BASE, 	0xFFFDC040	/* TC4 Channel Base Address */
      .equ TC_CCR,  	0x00  		  /* TC0 Channel Control Register */
      .equ TC_CMR, 	  0x04	  	  /* TC0 Channel Mode Register*/
      .equ TC_CV,    	0x10	    	/* TC0 Counter Value */
      .equ TC_RA,    	0x14	    	/* TC0 Register A */
      .equ TC_RB,    	0x18	     	/* TC0 Register B */
      .equ TC_RC,    	0x1C	    	/* TC0 Register C */
      .equ TC_SR,    	0x20	    	/* TC0 Status Register */
      .equ TC_IER,   	0x24	    	/* TC0 Interrupt Enable Register*/
      .equ TC_IDR,   	0x28		    /* TC0 Interrupt Disable Register */
      .equ TC_IMR,  	0x2C	    	/* TC0 Interrupt Mask Register */

/* user code here */

      bl INIT_IO
      bl INIT_TC4
      
      mov r0, #0b11111111   @ Green 
      mov r1, #0b11111111   @ Red
      mov r2, #0b11111111   @ Blue
      
      
      
      
set_all:
      mov r0, #0b11111111   @ Green 
      mov r1, #0b00000000   @ Red
      mov r2, #0b00000000   @ Blue
      
      @mov r0, #0
      @mov r1, #0
      @mov r2, #0
      
      bl SET_LED
      b set_all
      bl RESET

/* end user code */
```
##### SET_LED
```
SET_LED: 
      stmfd r13!, {r3-r4, lr} 
      ldr r3, =TC0_BASE
      
      @ r0 = Green
      @ r1 = Red
      @ r2 = Blue
      
/* -------- GREEN -------- */     
loop1:ldr r4, [r3, #TC_SR]        /* LOAD - Status registr */
      tst r4, #0b10000            /* test CPCS - bit4 (1 perioda) */
      beq loop1
      tst r0, #1
      bleq SET0
      blne SET1
      movs r0, r0, lsr #1
      bne loop1 
  
/* -------- RED -------- */       
loop2:ldr r4, [r3, #TC_SR]        /* LOAD - Status registr */
      tst r4, #0b10000            /* test CPCS - bit4 (1 perioda) */
      beq loop2
      tst r1, #1
      bleq SET0
      blne SET1
      movs r1, r1, lsr #1
      bne loop2
      
      mov r5, #8
/* -------- BLUE -------- */       
loop3:ldr r4, [r3, #TC_SR]        /* LOAD - Status registr */
      tst r4, #0b10000            /* test CPCS - bit4 (1 perioda) */
      beq loop3
      tst r2, #1
      bleq SET0
      blne SET1
      movs r2, r2, lsr #1
      bne loop3

      ldmfd r13!, {r3-r4, pc} 
```
##### SET_0
```
SET0: 
      stmfd r13!, {r0-r1, lr}
      ldr r0, =TC0_BASE
      
      @mov r1, #0b010              /* Enable TC (CLKEN = 1) bit0 */
      @str r1, [r0, #TC_CCR]       /* Start TC  (SWTRG = 1) bit2 */
      
      mov r1, #9          /* 30 * 0.32 (64%) = 9.6 */              
      str r1, [r0, #TC_RA] 
      
      mov r1, #0b100              /* Enable TC (CLKEN = 1) bit0 */
      str r1, [r0, #TC_CCR]       /* Start TC  (SWTRG = 1) bit2 */
      
      ldmfd r13!, {r0-r1, pc}
```
##### SET_1
```
SET1: 
      stmfd r13!, {r0-r1, lr}
      ldr r0, =TC0_BASE
      
      @mov r1, #0b010              /* Enable TC (CLKEN = 1) bit0 */
      @str r1, [r0, #TC_CCR]       /* Start TC  (SWTRG = 1) bit2 */
      
      mov r1, #21         /* 30 * 0.64 (64%) = 19 */              
      str r1, [r0, #TC_RA]
      
      mov r1, #0b100              /* Enable TC (CLKEN = 1) bit0 */
      str r1, [r0, #TC_CCR]       /* Start TC  (SWTRG = 1) bit2 */
      
      
      ldmfd r13!, {r0-r1, pc}
```
##### INIT_TC4
```
INIT_TC4: 
      stmfd r13!, {r0-r1, lr}
      
      ldr r0, =PMC_BASE
      mov r1, #1 << 17            /* Enable clock signal na TC0 */ 
			str r1, [r0, #PMC_PCER]	    
  
      ldr r0, =TC0_BASE           /* WAVE -> Wave mode (bit 15 = 1)   */      
      mov r1, #0b110 << 13        /* WAVSEL -> nacin (0 do RC)(bit 13,14 = 10) */
      add r1, r1, #0b000          /* TCCLKS -> TIMER_CLOCK4 (MCK/128) */
                                   /* TCCLKS -> TIMER_CLOCK1 (MCK/2) */
                                   
      /* ACPC - bit 18,19 (#0b11 << 18) --> RC compare action (toggle - 11) */
      /* ACPA - bit 16,17 (#0b11 << 16) --> RA compare action (toggle - 11) */
      @add r1, r1, #0b1111 << 16     /* RC, RA compare action (toggle - 11) */
      add r1, r1, #0b0110 << 16     /* RC, RA compare action (toggle - 11) */
      
      @add r1, r1, #0b1 << 6        /* Disable clock, trigger comapre RC */
      
      @add r1, r1, #0b01 << 18    /* RC compare action (set - HIGH) */
      @add r1, r1, #0b10 << 18    /* RC compare action (clear - LOW) */
      str r1, [r0, #TC_CMR]      

      ldr r1, =30              /* meja RC registra 65535 = max */
      str r1, [r0, #TC_RC]        /* RC value = 375 */
      
      mov r1, #0b101              /* Enable TC (CLKEN = 1) bit0 */
      str r1, [r0, #TC_CCR]       /* Start TC  (SWTRG = 1) bit2 */
      
      ldmfd r13!, {r0-r1, pc}
```
##### INIT_IO
```
INIT_IO: 
			stmfd r13!, {r0-r1, lr}  
      
      ldr r0, =PIOA_BASE
      mov r1, #1 << 26         
			str r1, [r0, #PIO_PDR]	/* Prikljucek A26 krmili Peripheral device (TC) */ 
      str r1, [r0, #PIO_ASR]	/* Upravaljanje naprave A (TIOA0) */   
      
      ldmfd r13!, {r0-r1, pc}
      
```
##### RESET
```
RESET: 
			stmfd r13!, {r0-r2, lr}  
      ldr r0, =PIOA_BASE
      mov r1, #1 << 26 
              
			str r1, [r0, #PIO_PER]	/* Prikljucek A26 krmili Peripheral device (TC) */ 
      str r1, [r0, #PIO_OER]	/* Prikljucek A26 krmili Peripheral device (TC) */ 
      str r1, [r0, #PIO_CODR]	/* Prikljucek A26 krmili Peripheral device (TC) */
  
      @ldr r2, =4800  
      @bl DELAY          @ nizko stanje > 50 mikro s
     
			@str r1, [r0, #PIO_PDR]	/* Prikljucek A26 krmili Peripheral device (TC) */ 
      @str r1, [r0, #PIO_ASR]	/* Upravaljanje naprave A (TIOA0) */   
      
      ldmfd r13!, {r0-r2, pc}
```
##### DIM_BLUE
```
DIM_BLUE:
      stmfd r13!, {r5, lr}
      ldr r2, =100000
      bl DELAY
      cmp r4, #0b11111111
      addlo r4, r4, #1
      moveq r4, #0
      mov r2, r4

      ldmfd r13!, {r5, pc}
```

