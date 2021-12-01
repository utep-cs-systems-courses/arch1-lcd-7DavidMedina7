	.arch msp430g2553
	.p2align 1,0
	.text

	.extern period		;global variable that will keep track of the current period
	.extern song_state	;global variable that will keep track of the current song state
	.global play_cool_song	;making reference to the global method play_cool_song

jt:
	.word option0 		;jt[0]
	.word option1 		;jt[1]
	.word option2 		;jt[2]


play_cool_song:
	cmp #4, &song_state 	;song_state - 4 doesn't borrow if s > 3
	jhs default 		;jmp if s > 3

	;; index into jt
	mov &song_state, r12
	add r12, r12 		;r12=2*song_state
	mov jt(r12), r0 	;jmp jt[s]
	
	
	;; switch table options are in same order as in source

option0:
	mov #1000, &period	;period = 1000
	mov #1, &song_state	;song_state = 1
	jmp end			;break

option1:
	mov #1500, &period	;period = 1500
	mov #2, &song_state	;song_state = 2
	jmp end			;break

option2:
	mov #2000, &period	;period = 2000
	mov #0, &song_state	;song_state = 0
	jmp end 		;break

default:
	mov #1000, &period
	mov #1, &song_state 	;no break

end:	
	mov &period, r12
	call #buzzer_set_period ;calling buzzer_set_period with period as a param.
	pop r0 			;return
