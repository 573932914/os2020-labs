.text
.globl  start, _start
start:
_start:
        //movl 2f422f312f382f302f332f302f392f362f36, 0xB8000
        movl $0x2f652f68, 0xB8000   //he
        movl $0x2f6c2f6c, 0xB8004   //ll
        movl $0x2f772f6f, 0xB8008   //ow
        movl $0x2f722f6f, 0xB800c   //or
        movl $0x2f642f6c, 0xB8010   //ld
        movl $0x2f502f2c, 0xB8014   //,P
        movl $0x2f312f42, 0xB8018   //B1
        movl $0x2f302f38, 0xB801c   //80
        movl $0x2f302f33, 0xB8020   //30
        movl $0x2f362f39, 0xB8024   //96
        movl $0x2f5f2f36, 0xB8028   //6_
        movl $0x2f612f77, 0xB802c   //wa
        movl $0x2f672f6e, 0xB8030   //ng
        movl $0x2f682f73, 0xB8034   //sh
        movl $0x2f6e2f61, 0xB8038   //an
        movl $0x2f67, 0xB803c   //g
        .align  4
multiboot_header:
        .long   0x1BADB002
        .long   0x00000001
        .long   -(0x1BADB002 + 0x00000001)
loop:   hlt
    jmp loop
     
