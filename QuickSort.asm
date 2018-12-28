fun [320] 0		;prompt and get size of array
mov dx [1199]	;size of the array (Should be less than 50 to be safe)
mov bx 650		;start of the array
mov cx bx
add cx dx
sub cx 1
fun [410] 0
fun [185] 2		;Fill the array
bx	
dx
fun [165] 2		;Print the array
bx
dx
fun [485] 0		;returned to 1199: 1=bubble, 2=quick
mov ax [1199]
cmp ax 1
je [40]			;bubble sort
fun [205] 2		;Quick sort the array
bx
cx
jmp [45]		;to print
fun [100] 2		;the bubble sort
bx
dx
fun [165] 2		;Print again
bx
dx
halt
















































;Bubble sort function @ 100 (array, size)
mov cx 1
mov bx [99]
mov ax [98]
add ax bx
mov [97] ax	;end of the array
cmp cx [98]	;condition of outer loop. [97]=end of array
jae [145]
mov ax [97]	;=end of array
sub ax cx	;where inner loop stops. not checking sorted indecies
mov [96] ax	;store in memory
mov bx [99]	;start of array
cmp bx [96]	;condition of inner loop. while(i<MAX)
jae [141]	; break
mov ax [bx]
cmp ax [bx+1]	;for swap
jbe [137]	;continue (if < or == then already in order)
mov dx bx
add dx 1
fun [150] 2
bx
dx
add bx 1	;++ of inner loop
jmp [120]
add cx 1	;++ of outer loop
jmp [109]
ret [1199]


;swap function @150 (array, size)
mov bx [149]
mov ax [bx]
mov bx [148]
mov dx [bx]
mov [bx] ax
mov bx [149]
mov [bx] dx
ret [1199]


;print function @165 (array, size)
mov bx [164]
mov cx bx
add cx [163]
mov ax [bx]
put
add bx 1
cmp bx cx
jb [170]
ret [1199]





;fillArray function @185 (array, size)
mov bx [184]
mov cx 0
get
mov [bx] ax
add bx 1
add cx 1
cmp cx [183]
jb [189]
ret [1199]



;quicksort function @205
mov ax [204]       ;first
mov bx [203]          ;last
cmp ax bx
jae [234]          ;end sort
fun [240] 2      ;partition function (F)
ax
[203]
mov cx [315]    ;[350] contains new pivot as return from partition function
sub cx 1
fun [205] 2        ;recursive call (A)
ax
cx
add cx 2
fun [205] 2
cx
bx
ret [1199]



;find pivot @240
mov bx [239]
mov dx [bx]
mov ax bx
add ax 1
mov cx [238]
cmp ax cx		;while high > low	;(20pdf)=248
jae [283]		;break low >= high	;(37pdf)
cmp ax cx		;(22pdf) =251
ja [262]		;break low > high
mov bx ax		
cmp dx [bx]		
jb [262]		;break pivot < list[low]
add ax 1		;low++
jmp [251]		;continue loop
cmp ax cx		;(26pdf) =262
ja [273]		;break low > high
mov bx cx
cmp dx [bx]
jae [273]		;break pivot > list[high]
sub cx 1
jmp [262]		;continue loop
cmp ax cx		;(30pdf) =273
jae [248]		;break low >= high (continue the outer while)
fun [150] 2		;swapFunction(ax, cx)
ax
cx
jmp [248]		;continue the outer while;  end first while
cmp cx [239]	;(37pdf) @283
jbe [296]		;break high <= first
mov bx cx
cmp dx [bx]
ja [296]		;break pivot > list[high]
sub cx 1
jmp [283]
mov bx cx
cmp dx [bx]		;(41pdf) =296
jbe [308]		;break pivot <= list[high]
fun [150] 2		;swapFunction ([F-1], cx)
[239]
cx
mov ax cx
ret [315]		;return high
mov ax [239]	;(46pdf)=308
ret [315]		;return first







;promt func @320 ()
mov ax 65
ext
put
mov ax 114
ext
put
mov ax 114
ext
put
mov ax 97
ext
put
mov ax 121
ext
put
mov ax 32
ext
put
mov ax 83
ext
put
mov ax 105
ext
put
mov ax 122
ext
put
mov ax 101
ext
put
mov ax 63
ext
put
mov ax 32
ext
put
mov ax 40
ext
put
mov ax 51
ext
put
mov ax 45
ext
put
mov ax 53
ext
put
mov ax 48
ext
put
mov ax 41
ext
put
mov ax 10
ext
put
get
cmp ax 3
jb [392]
cmp ax 50
ja [392]
ret [1199]


;fill promt @ 410
mov ax 10
ext
put
mov ax 69
ext
put
mov ax 110
ext
put
mov ax 116
ext
put
mov ax 101
ext
put
mov ax 114
ext
put
mov ax 32
ext
put
mov ax 105
ext
put
mov ax 110
ext
put
mov ax 116
ext
put
mov ax 111
ext
put
mov ax 32
ext
put
mov ax 97
ext
put
mov ax 114
ext
put
mov ax 114
ext
put
mov ax 97
ext
put
mov ax 121
ext
put
ret [1199]




;bubble or quick prompt @ 485
mov ax 67
ext
put
mov ax 104
ext
put
mov ax 111
ext
put
mov ax 111
ext
put
mov ax 115
ext
put
mov ax 101
ext
put
mov ax 58
ext
put
mov ax 10
ext
put
mov ax 49
ext
put
mov ax 58
ext
put
mov ax 32
ext
put
mov ax 66
ext
put
mov ax 117
ext
put
mov ax 98
ext
put
mov ax 98
ext
put
mov ax 108
ext
put
mov ax 101
ext
put
mov ax 10
ext
put
mov ax 50
ext
put
mov ax 58
ext
put
mov ax 32
ext
put
mov ax 81
ext
put
mov ax 117
ext
put
mov ax 105
ext
put
mov ax 99
ext
put
mov ax 107
ext
put
mov ax 115
ext
put
mov ax 111
ext
put
mov ax 114
ext
put
mov ax 116
ext
put
mov ax 10
ext
put
get
cmp ax 1
je [620]
cmp ax 2
je [620]
jmp [609]
ret [1199]