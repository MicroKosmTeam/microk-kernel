[bits 64]

%macro pushall 0
push rdi
push rsi

push rax
push rbx
push rcx
push rdx

push r8
push r9
push r10
push r11
push r12
push r13
push r14
push r15

%endmacro

%macro exitisr 0
	o64 iret
%endmacro

%macro popall 0

pop r15
pop r14
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8

pop rdx
pop rcx
pop rbx
pop rax

pop rsi
pop rdi

add rsp, 16

%endmacro

; ISR macros
%macro isr_null_stub 1
isr_stub_%+%1:

pushall
popall

exitisr

%endmacro

%macro isr_err_stub 1
isr_stub_%+%1:

push %1

pushall
mov rdi, rsp
call exceptionHandler
mov rsp, rax
popall

exitisr

%endmacro

%macro isr_no_err_stub 1
isr_stub_%+%1:

push 0
push %1

pushall
mov rdi, rsp
call exceptionHandler
mov rsp, rax
popall

exitisr

%endmacro

%macro isr_timer_stub 1
isr_stub_%+%1:

push 0
push %1

pushall

mov rdi, rsp
call timerHandler 
mov rsp, rax

popall

exitisr

%endmacro

%macro isr_spurious_stub 1
isr_stub_%+%1:

push 0
push %1

pushall
mov rdi, rsp
call spuriousHandler 
mov rsp, rax
popall

exitisr

%endmacro

%macro isr_syscall_stub 1
isr_stub_%+%1:
push 0
push %1

pushall

mov rdi, rsp
call syscallHandler
mov rsp, rax

popall

exitisr

%endmacro

; Handling functions
extern exceptionHandler
extern timerHandler
extern spuriousHandler
extern syscallHandler

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31
isr_timer_stub  32
isr_null_stub   33
isr_null_stub   34
isr_null_stub   35
isr_null_stub   36
isr_null_stub   37
isr_null_stub   38
isr_spurious_stub  39
isr_null_stub   40
isr_null_stub   41
isr_null_stub   42
isr_null_stub   43
isr_null_stub   44
isr_null_stub   45
isr_null_stub   46
isr_null_stub   47
isr_null_stub   48
isr_null_stub   49
isr_null_stub   50
isr_null_stub   51
isr_null_stub   52
isr_null_stub   53
isr_null_stub   54
isr_null_stub   55
isr_null_stub   56
isr_null_stub   57
isr_null_stub   58
isr_null_stub   59
isr_null_stub   60
isr_null_stub   61
isr_null_stub   62
isr_null_stub   63
isr_null_stub   64
isr_null_stub   65
isr_null_stub   66
isr_null_stub   67
isr_null_stub   68
isr_null_stub   69
isr_null_stub   70
isr_null_stub   71
isr_null_stub   72
isr_null_stub   73
isr_null_stub   74
isr_null_stub   75
isr_null_stub   76
isr_null_stub   77
isr_null_stub   78
isr_null_stub   79
isr_null_stub   80
isr_null_stub   81
isr_null_stub   82
isr_null_stub   83
isr_null_stub   84
isr_null_stub   85
isr_null_stub   86
isr_null_stub   87
isr_null_stub   88
isr_null_stub   89
isr_null_stub   90
isr_null_stub   91
isr_null_stub   92
isr_null_stub   93
isr_null_stub   94
isr_null_stub   95
isr_null_stub   96
isr_null_stub   97
isr_null_stub   98
isr_null_stub   99
isr_null_stub   100
isr_null_stub   101
isr_null_stub   102
isr_null_stub   103
isr_null_stub   104
isr_null_stub   105
isr_null_stub   106
isr_null_stub   107
isr_null_stub   108
isr_null_stub   109
isr_null_stub   110
isr_null_stub   111
isr_null_stub   112
isr_null_stub   113
isr_null_stub   114
isr_null_stub   115
isr_null_stub   116
isr_null_stub   117
isr_null_stub   118
isr_null_stub   119
isr_null_stub   120
isr_null_stub   121
isr_null_stub   122
isr_null_stub   123
isr_null_stub   124
isr_null_stub   125
isr_null_stub   126
isr_null_stub   127
isr_null_stub   128
isr_null_stub   129
isr_null_stub   130
isr_null_stub   131
isr_null_stub   132
isr_null_stub   133
isr_null_stub   134
isr_null_stub   135
isr_null_stub   136
isr_null_stub   137
isr_null_stub   138
isr_null_stub   139
isr_null_stub   140
isr_null_stub   141
isr_null_stub   142
isr_null_stub   143
isr_null_stub   144
isr_null_stub   145
isr_null_stub   146
isr_null_stub   147
isr_null_stub   148
isr_null_stub   149
isr_null_stub   150
isr_null_stub   151
isr_null_stub   152
isr_null_stub   153
isr_null_stub   154
isr_null_stub   155
isr_null_stub   156
isr_null_stub   157
isr_null_stub   158
isr_null_stub   159
isr_null_stub   160
isr_null_stub   161
isr_null_stub   162
isr_null_stub   163
isr_null_stub   164
isr_null_stub   165
isr_null_stub   166
isr_null_stub   167
isr_null_stub   168
isr_null_stub   169
isr_null_stub   170
isr_null_stub   171
isr_null_stub   172
isr_null_stub   173
isr_null_stub   174
isr_null_stub   175
isr_null_stub   176
isr_null_stub   177
isr_null_stub   178
isr_null_stub   179
isr_null_stub   180
isr_null_stub   181
isr_null_stub   182
isr_null_stub   183
isr_null_stub   184
isr_null_stub   185
isr_null_stub   186
isr_null_stub   187
isr_null_stub   188
isr_null_stub   189
isr_null_stub   190
isr_null_stub   191
isr_null_stub   192
isr_null_stub   193
isr_null_stub   194
isr_null_stub   195
isr_null_stub   196
isr_null_stub   197
isr_null_stub   198
isr_null_stub   199
isr_null_stub   200
isr_null_stub   201
isr_null_stub   202
isr_null_stub   203
isr_null_stub   204
isr_null_stub   205
isr_null_stub   206
isr_null_stub   207
isr_null_stub   208
isr_null_stub   209
isr_null_stub   210
isr_null_stub   211
isr_null_stub   212
isr_null_stub   213
isr_null_stub   214
isr_null_stub   215
isr_null_stub   216
isr_null_stub   217
isr_null_stub   218
isr_null_stub   219
isr_null_stub   220
isr_null_stub   221
isr_null_stub   222
isr_null_stub   223
isr_null_stub   224
isr_null_stub   225
isr_null_stub   226
isr_null_stub   227
isr_null_stub   228
isr_null_stub   229
isr_null_stub   230
isr_null_stub   231
isr_null_stub   232
isr_null_stub   233
isr_null_stub   234
isr_null_stub   235
isr_null_stub   236
isr_null_stub   237
isr_null_stub   238
isr_null_stub   239
isr_null_stub   240
isr_null_stub   241
isr_null_stub   242
isr_null_stub   243
isr_null_stub   244
isr_null_stub   245
isr_null_stub   246
isr_null_stub   247
isr_null_stub   248
isr_null_stub   249
isr_null_stub   250
isr_null_stub   251
isr_null_stub   252
isr_null_stub   253
isr_syscall_stub 254
isr_null_stub   255

; The isr stub table
global isrStubTable
isrStubTable:
%assign i 0
%rep    256
dq isr_stub_%+i
%assign i i+1
%endrep
