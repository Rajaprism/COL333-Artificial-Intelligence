
kernel:     file format elf32-i386


Disassembly of section .text:

00100000 <multiboot_header>:
  100000:	02 b0 ad 1b 00 00    	add    0x1bad(%eax),%dh
  100006:	00 00                	add    %al,(%eax)
  100008:	fe 4f 52             	decb   0x52(%edi)
  10000b:	e4                   	.byte 0xe4

0010000c <_start>:

# Entering xv6 on boot processor, with paging off.
.globl entry
entry:
  # Set up the stack pointer.
  movl $(stack + KSTACKSIZE), %esp
  10000c:	bc e0 3c 10 00       	mov    $0x103ce0,%esp

  # Jump to main(), and switch to executing at
  # high addresses. The indirect call is needed because
  # the assembler produces a PC-relative instruction
  # for a direct jump.
  mov $main, %eax
  100011:	b8 80 06 10 00       	mov    $0x100680,%eax
  jmp *%eax
  100016:	ff e0                	jmp    *%eax
  100018:	66 90                	xchg   %ax,%ax
  10001a:	66 90                	xchg   %ax,%ax
  10001c:	66 90                	xchg   %ax,%ax
  10001e:	66 90                	xchg   %ax,%ax

00100020 <printint>:
static void consputc(int);
static int panicked = 0;

static void
printint(int xx, int base, int sign)
{
  100020:	55                   	push   %ebp
  100021:	89 e5                	mov    %esp,%ebp
  100023:	57                   	push   %edi
  100024:	56                   	push   %esi
  100025:	53                   	push   %ebx
  100026:	89 d3                	mov    %edx,%ebx
  100028:	83 ec 2c             	sub    $0x2c,%esp
  static char digits[] = "0123456789abcdef";
  char buf[16];
  int i;
  uint x;

  if(sign && (sign = xx < 0))
  10002b:	85 c0                	test   %eax,%eax
  10002d:	79 05                	jns    100034 <printint+0x14>
  10002f:	83 e1 01             	and    $0x1,%ecx
  100032:	75 6c                	jne    1000a0 <printint+0x80>
    x = -xx;
  else
    x = xx;
  100034:	c7 45 d4 00 00 00 00 	movl   $0x0,-0x2c(%ebp)
  10003b:	89 c1                	mov    %eax,%ecx

  i = 0;
  10003d:	31 f6                	xor    %esi,%esi
  10003f:	90                   	nop
  do{
    buf[i++] = digits[x % base];
  100040:	89 c8                	mov    %ecx,%eax
  100042:	31 d2                	xor    %edx,%edx
  100044:	89 f7                	mov    %esi,%edi
  100046:	f7 f3                	div    %ebx
  100048:	8d 76 01             	lea    0x1(%esi),%esi
  10004b:	0f b6 92 0c 1e 10 00 	movzbl 0x101e0c(%edx),%edx
  100052:	88 54 35 d7          	mov    %dl,-0x29(%ebp,%esi,1)
  }while((x /= base) != 0);
  100056:	89 ca                	mov    %ecx,%edx
  100058:	89 c1                	mov    %eax,%ecx
  10005a:	39 da                	cmp    %ebx,%edx
  10005c:	73 e2                	jae    100040 <printint+0x20>

  if(sign)
  10005e:	8b 45 d4             	mov    -0x2c(%ebp),%eax
  100061:	85 c0                	test   %eax,%eax
  100063:	74 07                	je     10006c <printint+0x4c>
    buf[i++] = '-';
  100065:	c6 44 35 d8 2d       	movb   $0x2d,-0x28(%ebp,%esi,1)

  while(--i >= 0)
  10006a:	89 f7                	mov    %esi,%edi
  10006c:	8d 5d d8             	lea    -0x28(%ebp),%ebx
  10006f:	01 df                	add    %ebx,%edi
  100071:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
    consputc(buf[i]);
  100078:	0f be 07             	movsbl (%edi),%eax
consputc(int c)
{
  if(c == BACKSPACE){
    uartputc('\b'); uartputc(' '); uartputc('\b');
  } else
    uartputc(c);
  10007b:	83 ec 0c             	sub    $0xc,%esp
  10007e:	50                   	push   %eax
  10007f:	e8 5c 0a 00 00       	call   100ae0 <uartputc>
  while(--i >= 0)
  100084:	89 f8                	mov    %edi,%eax
  100086:	83 c4 10             	add    $0x10,%esp
  100089:	83 ef 01             	sub    $0x1,%edi
  10008c:	39 d8                	cmp    %ebx,%eax
  10008e:	75 e8                	jne    100078 <printint+0x58>
}
  100090:	8d 65 f4             	lea    -0xc(%ebp),%esp
  100093:	5b                   	pop    %ebx
  100094:	5e                   	pop    %esi
  100095:	5f                   	pop    %edi
  100096:	5d                   	pop    %ebp
  100097:	c3                   	ret
  100098:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  10009f:	90                   	nop
    x = -xx;
  1000a0:	f7 d8                	neg    %eax
  if(sign && (sign = xx < 0))
  1000a2:	c7 45 d4 01 00 00 00 	movl   $0x1,-0x2c(%ebp)
    x = -xx;
  1000a9:	89 c1                	mov    %eax,%ecx
  1000ab:	eb 90                	jmp    10003d <printint+0x1d>
  1000ad:	8d 76 00             	lea    0x0(%esi),%esi

001000b0 <cprintf>:
{
  1000b0:	55                   	push   %ebp
  1000b1:	89 e5                	mov    %esp,%ebp
  1000b3:	57                   	push   %edi
  1000b4:	56                   	push   %esi
  1000b5:	53                   	push   %ebx
  1000b6:	83 ec 1c             	sub    $0x1c,%esp
  if (fmt == 0)
  1000b9:	8b 75 08             	mov    0x8(%ebp),%esi
  1000bc:	85 f6                	test   %esi,%esi
  1000be:	74 7d                	je     10013d <cprintf+0x8d>
  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
  1000c0:	0f b6 06             	movzbl (%esi),%eax
  1000c3:	85 c0                	test   %eax,%eax
  1000c5:	74 76                	je     10013d <cprintf+0x8d>
  argp = (uint*)(void*)(&fmt + 1);
  1000c7:	8d 4d 0c             	lea    0xc(%ebp),%ecx
  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
  1000ca:	31 db                	xor    %ebx,%ebx
  1000cc:	eb 4d                	jmp    10011b <cprintf+0x6b>
  1000ce:	66 90                	xchg   %ax,%ax
    c = fmt[++i] & 0xff;
  1000d0:	83 c3 01             	add    $0x1,%ebx
  1000d3:	0f b6 3c 1e          	movzbl (%esi,%ebx,1),%edi
    if(c == 0)
  1000d7:	85 ff                	test   %edi,%edi
  1000d9:	74 62                	je     10013d <cprintf+0x8d>
    switch(c){
  1000db:	83 ff 70             	cmp    $0x70,%edi
  1000de:	0f 84 ec 00 00 00    	je     1001d0 <cprintf+0x120>
  1000e4:	7f 62                	jg     100148 <cprintf+0x98>
  1000e6:	83 ff 25             	cmp    $0x25,%edi
  1000e9:	0f 84 c1 00 00 00    	je     1001b0 <cprintf+0x100>
  1000ef:	83 ff 64             	cmp    $0x64,%edi
  1000f2:	75 5e                	jne    100152 <cprintf+0xa2>
      printint(*argp++, 10, 1);
  1000f4:	8b 01                	mov    (%ecx),%eax
  1000f6:	8d 79 04             	lea    0x4(%ecx),%edi
  1000f9:	ba 0a 00 00 00       	mov    $0xa,%edx
  1000fe:	b9 01 00 00 00       	mov    $0x1,%ecx
  100103:	e8 18 ff ff ff       	call   100020 <printint>
  100108:	89 f9                	mov    %edi,%ecx
  10010a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
  100110:	83 c3 01             	add    $0x1,%ebx
  100113:	0f b6 04 1e          	movzbl (%esi,%ebx,1),%eax
  100117:	85 c0                	test   %eax,%eax
  100119:	74 22                	je     10013d <cprintf+0x8d>
    if(c != '%'){
  10011b:	83 f8 25             	cmp    $0x25,%eax
  10011e:	74 b0                	je     1000d0 <cprintf+0x20>
    uartputc(c);
  100120:	83 ec 0c             	sub    $0xc,%esp
  100123:	89 4d e4             	mov    %ecx,-0x1c(%ebp)
  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
  100126:	83 c3 01             	add    $0x1,%ebx
    uartputc(c);
  100129:	50                   	push   %eax
  10012a:	e8 b1 09 00 00       	call   100ae0 <uartputc>
  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
  10012f:	0f b6 04 1e          	movzbl (%esi,%ebx,1),%eax
      continue;
  100133:	8b 4d e4             	mov    -0x1c(%ebp),%ecx
  100136:	83 c4 10             	add    $0x10,%esp
  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
  100139:	85 c0                	test   %eax,%eax
  10013b:	75 de                	jne    10011b <cprintf+0x6b>
}
  10013d:	8d 65 f4             	lea    -0xc(%ebp),%esp
  100140:	5b                   	pop    %ebx
  100141:	5e                   	pop    %esi
  100142:	5f                   	pop    %edi
  100143:	5d                   	pop    %ebp
  100144:	c3                   	ret
  100145:	8d 76 00             	lea    0x0(%esi),%esi
    switch(c){
  100148:	83 ff 73             	cmp    $0x73,%edi
  10014b:	74 23                	je     100170 <cprintf+0xc0>
  10014d:	83 ff 78             	cmp    $0x78,%edi
  100150:	74 7e                	je     1001d0 <cprintf+0x120>
    uartputc(c);
  100152:	83 ec 0c             	sub    $0xc,%esp
  100155:	89 4d e4             	mov    %ecx,-0x1c(%ebp)
  100158:	6a 25                	push   $0x25
  10015a:	e8 81 09 00 00       	call   100ae0 <uartputc>
  10015f:	89 3c 24             	mov    %edi,(%esp)
  100162:	e8 79 09 00 00       	call   100ae0 <uartputc>
  100167:	83 c4 10             	add    $0x10,%esp
  10016a:	8b 4d e4             	mov    -0x1c(%ebp),%ecx
  10016d:	eb a1                	jmp    100110 <cprintf+0x60>
  10016f:	90                   	nop
      if((s = (char*)*argp++) == 0)
  100170:	8b 11                	mov    (%ecx),%edx
  100172:	8d 41 04             	lea    0x4(%ecx),%eax
  100175:	89 45 e4             	mov    %eax,-0x1c(%ebp)
  100178:	85 d2                	test   %edx,%edx
  10017a:	74 74                	je     1001f0 <cprintf+0x140>
      for(; *s; s++)
  10017c:	0f be 02             	movsbl (%edx),%eax
      if((s = (char*)*argp++) == 0)
  10017f:	8b 4d e4             	mov    -0x1c(%ebp),%ecx
  100182:	89 d7                	mov    %edx,%edi
      for(; *s; s++)
  100184:	84 c0                	test   %al,%al
  100186:	74 88                	je     100110 <cprintf+0x60>
  100188:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  10018f:	90                   	nop
    uartputc(c);
  100190:	83 ec 0c             	sub    $0xc,%esp
      for(; *s; s++)
  100193:	83 c7 01             	add    $0x1,%edi
    uartputc(c);
  100196:	50                   	push   %eax
  100197:	e8 44 09 00 00       	call   100ae0 <uartputc>
      for(; *s; s++)
  10019c:	0f be 07             	movsbl (%edi),%eax
  10019f:	83 c4 10             	add    $0x10,%esp
  1001a2:	84 c0                	test   %al,%al
  1001a4:	75 ea                	jne    100190 <cprintf+0xe0>
  1001a6:	eb c2                	jmp    10016a <cprintf+0xba>
  1001a8:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  1001af:	90                   	nop
    uartputc(c);
  1001b0:	83 ec 0c             	sub    $0xc,%esp
  1001b3:	89 4d e4             	mov    %ecx,-0x1c(%ebp)
  1001b6:	6a 25                	push   $0x25
  1001b8:	e8 23 09 00 00       	call   100ae0 <uartputc>
}
  1001bd:	8b 4d e4             	mov    -0x1c(%ebp),%ecx
  1001c0:	83 c4 10             	add    $0x10,%esp
  1001c3:	e9 48 ff ff ff       	jmp    100110 <cprintf+0x60>
  1001c8:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  1001cf:	90                   	nop
      printint(*argp++, 16, 0);
  1001d0:	8b 01                	mov    (%ecx),%eax
  1001d2:	8d 79 04             	lea    0x4(%ecx),%edi
  1001d5:	ba 10 00 00 00       	mov    $0x10,%edx
  1001da:	31 c9                	xor    %ecx,%ecx
  1001dc:	e8 3f fe ff ff       	call   100020 <printint>
  1001e1:	89 f9                	mov    %edi,%ecx
      break;
  1001e3:	e9 28 ff ff ff       	jmp    100110 <cprintf+0x60>
  1001e8:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  1001ef:	90                   	nop
  1001f0:	b8 28 00 00 00       	mov    $0x28,%eax
        s = "(null)";
  1001f5:	bf 70 1d 10 00       	mov    $0x101d70,%edi
  1001fa:	eb 94                	jmp    100190 <cprintf+0xe0>
  1001fc:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi

00100200 <halt>:
{
  100200:	55                   	push   %ebp
  100201:	89 e5                	mov    %esp,%ebp
  100203:	83 ec 10             	sub    $0x10,%esp
  cprintf("Bye COL%d!\n\0", 331);
  100206:	68 4b 01 00 00       	push   $0x14b
  10020b:	68 fc 1d 10 00       	push   $0x101dfc
  100210:	e8 9b fe ff ff       	call   1000b0 <cprintf>
}

static inline void
outw(ushort port, ushort data)
{
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  100215:	b8 00 20 00 00       	mov    $0x2000,%eax
  10021a:	ba 02 06 00 00       	mov    $0x602,%edx
  10021f:	66 ef                	out    %ax,(%dx)
  100221:	ba 02 b0 ff ff       	mov    $0xffffb002,%edx
  100226:	66 ef                	out    %ax,(%dx)
}
  100228:	83 c4 10             	add    $0x10,%esp
  for(;;);
  10022b:	eb fe                	jmp    10022b <halt+0x2b>
  10022d:	8d 76 00             	lea    0x0(%esi),%esi

00100230 <panic>:
{
  100230:	55                   	push   %ebp
  100231:	89 e5                	mov    %esp,%ebp
  100233:	56                   	push   %esi
  100234:	53                   	push   %ebx
  100235:	83 ec 30             	sub    $0x30,%esp
}

static inline void
cli(void)
{
  asm volatile("cli");
  100238:	fa                   	cli
  cprintf("lapicid %d: panic: ", lapicid());
  100239:	e8 f2 03 00 00       	call   100630 <lapicid>
  10023e:	83 ec 08             	sub    $0x8,%esp
  getcallerpcs(&s, pcs);
  100241:	8d 5d d0             	lea    -0x30(%ebp),%ebx
  100244:	8d 75 f8             	lea    -0x8(%ebp),%esi
  cprintf("lapicid %d: panic: ", lapicid());
  100247:	50                   	push   %eax
  100248:	68 77 1d 10 00       	push   $0x101d77
  10024d:	e8 5e fe ff ff       	call   1000b0 <cprintf>
  cprintf(s);
  100252:	58                   	pop    %eax
  100253:	ff 75 08             	push   0x8(%ebp)
  100256:	e8 55 fe ff ff       	call   1000b0 <cprintf>
  cprintf("\n");
  10025b:	c7 04 24 b7 1d 10 00 	movl   $0x101db7,(%esp)
  100262:	e8 49 fe ff ff       	call   1000b0 <cprintf>
  getcallerpcs(&s, pcs);
  100267:	8d 45 08             	lea    0x8(%ebp),%eax
  10026a:	5a                   	pop    %edx
  10026b:	59                   	pop    %ecx
  10026c:	53                   	push   %ebx
  10026d:	50                   	push   %eax
  10026e:	e8 5d 0b 00 00       	call   100dd0 <getcallerpcs>
  for(i=0; i<10; i++)
  100273:	83 c4 10             	add    $0x10,%esp
  100276:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  10027d:	8d 76 00             	lea    0x0(%esi),%esi
    cprintf(" %p", pcs[i]);
  100280:	83 ec 08             	sub    $0x8,%esp
  100283:	ff 33                	push   (%ebx)
  for(i=0; i<10; i++)
  100285:	83 c3 04             	add    $0x4,%ebx
    cprintf(" %p", pcs[i]);
  100288:	68 8b 1d 10 00       	push   $0x101d8b
  10028d:	e8 1e fe ff ff       	call   1000b0 <cprintf>
  for(i=0; i<10; i++)
  100292:	83 c4 10             	add    $0x10,%esp
  100295:	39 f3                	cmp    %esi,%ebx
  100297:	75 e7                	jne    100280 <panic+0x50>
  halt();
  100299:	e8 62 ff ff ff       	call   100200 <halt>
  10029e:	66 90                	xchg   %ax,%ax

001002a0 <consoleintr>:

#define C(x)  ((x)-'@')  // Control-x

void
consoleintr(int (*getc)(void))
{
  1002a0:	55                   	push   %ebp
  1002a1:	89 e5                	mov    %esp,%ebp
  1002a3:	53                   	push   %ebx
  1002a4:	83 ec 14             	sub    $0x14,%esp
  1002a7:	8b 5d 08             	mov    0x8(%ebp),%ebx
  int c;

  while((c = getc()) >= 0){
  1002aa:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
  1002b0:	ff d3                	call   *%ebx
  1002b2:	85 c0                	test   %eax,%eax
  1002b4:	0f 88 9b 00 00 00    	js     100355 <consoleintr+0xb5>
    switch(c){
  1002ba:	83 f8 15             	cmp    $0x15,%eax
  1002bd:	0f 84 9d 00 00 00    	je     100360 <consoleintr+0xc0>
  1002c3:	83 f8 7f             	cmp    $0x7f,%eax
  1002c6:	0f 84 04 01 00 00    	je     1003d0 <consoleintr+0x130>
  1002cc:	83 f8 08             	cmp    $0x8,%eax
  1002cf:	0f 84 fb 00 00 00    	je     1003d0 <consoleintr+0x130>
        input.e--;
        consputc(BACKSPACE);
      }
      break;
    default:
      if(c != 0 && input.e-input.r < INPUT_BUF){
  1002d5:	85 c0                	test   %eax,%eax
  1002d7:	74 d7                	je     1002b0 <consoleintr+0x10>
  1002d9:	8b 15 88 24 10 00    	mov    0x102488,%edx
  1002df:	89 d1                	mov    %edx,%ecx
  1002e1:	2b 0d 80 24 10 00    	sub    0x102480,%ecx
  1002e7:	83 f9 7f             	cmp    $0x7f,%ecx
  1002ea:	77 c4                	ja     1002b0 <consoleintr+0x10>
        c = (c == '\r') ? '\n' : c;
        input.buf[input.e++ % INPUT_BUF] = c;
  1002ec:	8d 4a 01             	lea    0x1(%edx),%ecx
  1002ef:	83 e2 7f             	and    $0x7f,%edx
  1002f2:	89 0d 88 24 10 00    	mov    %ecx,0x102488
        c = (c == '\r') ? '\n' : c;
  1002f8:	83 f8 0d             	cmp    $0xd,%eax
  1002fb:	0f 84 12 01 00 00    	je     100413 <consoleintr+0x173>
        input.buf[input.e++ % INPUT_BUF] = c;
  100301:	88 82 00 24 10 00    	mov    %al,0x102400(%edx)
  if(c == BACKSPACE){
  100307:	3d 00 01 00 00       	cmp    $0x100,%eax
  10030c:	0f 85 24 01 00 00    	jne    100436 <consoleintr+0x196>
    uartputc('\b'); uartputc(' '); uartputc('\b');
  100312:	83 ec 0c             	sub    $0xc,%esp
  100315:	6a 08                	push   $0x8
  100317:	e8 c4 07 00 00       	call   100ae0 <uartputc>
  10031c:	c7 04 24 20 00 00 00 	movl   $0x20,(%esp)
  100323:	e8 b8 07 00 00       	call   100ae0 <uartputc>
  100328:	c7 04 24 08 00 00 00 	movl   $0x8,(%esp)
  10032f:	e8 ac 07 00 00       	call   100ae0 <uartputc>
  100334:	83 c4 10             	add    $0x10,%esp
        consputc(c);
        if(c == '\n' || c == C('D') || input.e == input.r+INPUT_BUF){
  100337:	a1 80 24 10 00       	mov    0x102480,%eax
  10033c:	83 e8 80             	sub    $0xffffff80,%eax
  10033f:	39 05 88 24 10 00    	cmp    %eax,0x102488
  100345:	0f 84 e1 00 00 00    	je     10042c <consoleintr+0x18c>
  while((c = getc()) >= 0){
  10034b:	ff d3                	call   *%ebx
  10034d:	85 c0                	test   %eax,%eax
  10034f:	0f 89 65 ff ff ff    	jns    1002ba <consoleintr+0x1a>
        }
      }
      break;
    }
  }
  100355:	8b 5d fc             	mov    -0x4(%ebp),%ebx
  100358:	c9                   	leave
  100359:	c3                   	ret
  10035a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
      while(input.e != input.w &&
  100360:	a1 88 24 10 00       	mov    0x102488,%eax
  100365:	39 05 84 24 10 00    	cmp    %eax,0x102484
  10036b:	75 46                	jne    1003b3 <consoleintr+0x113>
  10036d:	e9 3e ff ff ff       	jmp    1002b0 <consoleintr+0x10>
  100372:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
    uartputc('\b'); uartputc(' '); uartputc('\b');
  100378:	83 ec 0c             	sub    $0xc,%esp
        input.e--;
  10037b:	a3 88 24 10 00       	mov    %eax,0x102488
    uartputc('\b'); uartputc(' '); uartputc('\b');
  100380:	6a 08                	push   $0x8
  100382:	e8 59 07 00 00       	call   100ae0 <uartputc>
  100387:	c7 04 24 20 00 00 00 	movl   $0x20,(%esp)
  10038e:	e8 4d 07 00 00       	call   100ae0 <uartputc>
  100393:	c7 04 24 08 00 00 00 	movl   $0x8,(%esp)
  10039a:	e8 41 07 00 00       	call   100ae0 <uartputc>
      while(input.e != input.w &&
  10039f:	a1 88 24 10 00       	mov    0x102488,%eax
  1003a4:	83 c4 10             	add    $0x10,%esp
  1003a7:	3b 05 84 24 10 00    	cmp    0x102484,%eax
  1003ad:	0f 84 fd fe ff ff    	je     1002b0 <consoleintr+0x10>
            input.buf[(input.e-1) % INPUT_BUF] != '\n'){
  1003b3:	83 e8 01             	sub    $0x1,%eax
  1003b6:	89 c2                	mov    %eax,%edx
  1003b8:	83 e2 7f             	and    $0x7f,%edx
      while(input.e != input.w &&
  1003bb:	80 ba 00 24 10 00 0a 	cmpb   $0xa,0x102400(%edx)
  1003c2:	75 b4                	jne    100378 <consoleintr+0xd8>
  1003c4:	e9 e7 fe ff ff       	jmp    1002b0 <consoleintr+0x10>
  1003c9:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
      if(input.e != input.w){
  1003d0:	a1 88 24 10 00       	mov    0x102488,%eax
  1003d5:	3b 05 84 24 10 00    	cmp    0x102484,%eax
  1003db:	0f 84 cf fe ff ff    	je     1002b0 <consoleintr+0x10>
    uartputc('\b'); uartputc(' '); uartputc('\b');
  1003e1:	83 ec 0c             	sub    $0xc,%esp
        input.e--;
  1003e4:	83 e8 01             	sub    $0x1,%eax
    uartputc('\b'); uartputc(' '); uartputc('\b');
  1003e7:	6a 08                	push   $0x8
        input.e--;
  1003e9:	a3 88 24 10 00       	mov    %eax,0x102488
    uartputc('\b'); uartputc(' '); uartputc('\b');
  1003ee:	e8 ed 06 00 00       	call   100ae0 <uartputc>
  1003f3:	c7 04 24 20 00 00 00 	movl   $0x20,(%esp)
  1003fa:	e8 e1 06 00 00       	call   100ae0 <uartputc>
  1003ff:	c7 04 24 08 00 00 00 	movl   $0x8,(%esp)
  100406:	e8 d5 06 00 00       	call   100ae0 <uartputc>
  10040b:	83 c4 10             	add    $0x10,%esp
  10040e:	e9 9d fe ff ff       	jmp    1002b0 <consoleintr+0x10>
    uartputc(c);
  100413:	83 ec 0c             	sub    $0xc,%esp
        input.buf[input.e++ % INPUT_BUF] = c;
  100416:	c6 82 00 24 10 00 0a 	movb   $0xa,0x102400(%edx)
    uartputc(c);
  10041d:	6a 0a                	push   $0xa
  10041f:	e8 bc 06 00 00       	call   100ae0 <uartputc>
          input.w = input.e;
  100424:	a1 88 24 10 00       	mov    0x102488,%eax
  100429:	83 c4 10             	add    $0x10,%esp
  10042c:	a3 84 24 10 00       	mov    %eax,0x102484
  100431:	e9 7a fe ff ff       	jmp    1002b0 <consoleintr+0x10>
    uartputc(c);
  100436:	83 ec 0c             	sub    $0xc,%esp
  100439:	89 45 f4             	mov    %eax,-0xc(%ebp)
  10043c:	50                   	push   %eax
  10043d:	e8 9e 06 00 00       	call   100ae0 <uartputc>
        if(c == '\n' || c == C('D') || input.e == input.r+INPUT_BUF){
  100442:	8b 45 f4             	mov    -0xc(%ebp),%eax
  100445:	83 c4 10             	add    $0x10,%esp
  100448:	83 f8 0a             	cmp    $0xa,%eax
  10044b:	74 09                	je     100456 <consoleintr+0x1b6>
  10044d:	83 f8 04             	cmp    $0x4,%eax
  100450:	0f 85 e1 fe ff ff    	jne    100337 <consoleintr+0x97>
          input.w = input.e;
  100456:	a1 88 24 10 00       	mov    0x102488,%eax
  10045b:	eb cf                	jmp    10042c <consoleintr+0x18c>
  10045d:	66 90                	xchg   %ax,%ax
  10045f:	90                   	nop

00100460 <ioapicinit>:
  ioapic->data = data;
}

void
ioapicinit(void)
{
  100460:	55                   	push   %ebp
  100461:	89 e5                	mov    %esp,%ebp
  100463:	56                   	push   %esi
  100464:	53                   	push   %ebx
  int i, id, maxintr;

  ioapic = (volatile struct ioapic*)IOAPIC;
  100465:	c7 05 8c 24 10 00 00 	movl   $0xfec00000,0x10248c
  10046c:	00 c0 fe 
  ioapic->reg = reg;
  10046f:	c7 05 00 00 c0 fe 01 	movl   $0x1,0xfec00000
  100476:	00 00 00 
  return ioapic->data;
  100479:	8b 15 8c 24 10 00    	mov    0x10248c,%edx
  10047f:	8b 72 10             	mov    0x10(%edx),%esi
  ioapic->reg = reg;
  100482:	c7 02 00 00 00 00    	movl   $0x0,(%edx)
  return ioapic->data;
  100488:	8b 1d 8c 24 10 00    	mov    0x10248c,%ebx
  maxintr = (ioapicread(REG_VER) >> 16) & 0xFF;
  id = ioapicread(REG_ID) >> 24;
  if(id != ioapicid)
  10048e:	0f b6 15 94 24 10 00 	movzbl 0x102494,%edx
  maxintr = (ioapicread(REG_VER) >> 16) & 0xFF;
  100495:	c1 ee 10             	shr    $0x10,%esi
  100498:	89 f0                	mov    %esi,%eax
  10049a:	0f b6 f0             	movzbl %al,%esi
  return ioapic->data;
  10049d:	8b 43 10             	mov    0x10(%ebx),%eax
  id = ioapicread(REG_ID) >> 24;
  1004a0:	c1 e8 18             	shr    $0x18,%eax
  if(id != ioapicid)
  1004a3:	39 c2                	cmp    %eax,%edx
  1004a5:	74 16                	je     1004bd <ioapicinit+0x5d>
    cprintf("ioapicinit: id isn't equal to ioapicid; not a MP\n");
  1004a7:	83 ec 0c             	sub    $0xc,%esp
  1004aa:	68 20 1e 10 00       	push   $0x101e20
  1004af:	e8 fc fb ff ff       	call   1000b0 <cprintf>
  ioapic->reg = reg;
  1004b4:	8b 1d 8c 24 10 00    	mov    0x10248c,%ebx
  1004ba:	83 c4 10             	add    $0x10,%esp
{
  1004bd:	ba 10 00 00 00       	mov    $0x10,%edx
  1004c2:	31 c0                	xor    %eax,%eax
  1004c4:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  ioapic->reg = reg;
  1004c8:	89 13                	mov    %edx,(%ebx)
  1004ca:	8d 48 20             	lea    0x20(%eax),%ecx
  ioapic->data = data;
  1004cd:	8b 1d 8c 24 10 00    	mov    0x10248c,%ebx

  // Mark all interrupts edge-triggered, active high, disabled,
  // and not routed to any CPUs.
  for(i = 0; i <= maxintr; i++){
  1004d3:	83 c0 01             	add    $0x1,%eax
  1004d6:	81 c9 00 00 01 00    	or     $0x10000,%ecx
  ioapic->data = data;
  1004dc:	89 4b 10             	mov    %ecx,0x10(%ebx)
  ioapic->reg = reg;
  1004df:	8d 4a 01             	lea    0x1(%edx),%ecx
  for(i = 0; i <= maxintr; i++){
  1004e2:	83 c2 02             	add    $0x2,%edx
  ioapic->reg = reg;
  1004e5:	89 0b                	mov    %ecx,(%ebx)
  ioapic->data = data;
  1004e7:	8b 1d 8c 24 10 00    	mov    0x10248c,%ebx
  1004ed:	c7 43 10 00 00 00 00 	movl   $0x0,0x10(%ebx)
  for(i = 0; i <= maxintr; i++){
  1004f4:	39 c6                	cmp    %eax,%esi
  1004f6:	7d d0                	jge    1004c8 <ioapicinit+0x68>
    ioapicwrite(REG_TABLE+2*i, INT_DISABLED | (T_IRQ0 + i));
    ioapicwrite(REG_TABLE+2*i+1, 0);
  }
}
  1004f8:	8d 65 f8             	lea    -0x8(%ebp),%esp
  1004fb:	5b                   	pop    %ebx
  1004fc:	5e                   	pop    %esi
  1004fd:	5d                   	pop    %ebp
  1004fe:	c3                   	ret
  1004ff:	90                   	nop

00100500 <ioapicenable>:

void
ioapicenable(int irq, int cpunum)
{
  100500:	55                   	push   %ebp
  ioapic->reg = reg;
  100501:	8b 0d 8c 24 10 00    	mov    0x10248c,%ecx
{
  100507:	89 e5                	mov    %esp,%ebp
  100509:	8b 45 08             	mov    0x8(%ebp),%eax
  // Mark interrupt edge-triggered, active high,
  // enabled, and routed to the given cpunum,
  // which happens to be that cpu's APIC ID.
  ioapicwrite(REG_TABLE+2*irq, T_IRQ0 + irq);
  10050c:	8d 50 20             	lea    0x20(%eax),%edx
  10050f:	8d 44 00 10          	lea    0x10(%eax,%eax,1),%eax
  ioapic->reg = reg;
  100513:	89 01                	mov    %eax,(%ecx)
  ioapic->data = data;
  100515:	8b 0d 8c 24 10 00    	mov    0x10248c,%ecx
  ioapicwrite(REG_TABLE+2*irq+1, cpunum << 24);
  10051b:	83 c0 01             	add    $0x1,%eax
  ioapic->data = data;
  10051e:	89 51 10             	mov    %edx,0x10(%ecx)
  ioapicwrite(REG_TABLE+2*irq+1, cpunum << 24);
  100521:	8b 55 0c             	mov    0xc(%ebp),%edx
  ioapic->reg = reg;
  100524:	89 01                	mov    %eax,(%ecx)
  ioapic->data = data;
  100526:	a1 8c 24 10 00       	mov    0x10248c,%eax
  ioapicwrite(REG_TABLE+2*irq+1, cpunum << 24);
  10052b:	c1 e2 18             	shl    $0x18,%edx
  ioapic->data = data;
  10052e:	89 50 10             	mov    %edx,0x10(%eax)
}
  100531:	5d                   	pop    %ebp
  100532:	c3                   	ret
  100533:	66 90                	xchg   %ax,%ax
  100535:	66 90                	xchg   %ax,%ax
  100537:	66 90                	xchg   %ax,%ax
  100539:	66 90                	xchg   %ax,%ax
  10053b:	66 90                	xchg   %ax,%ax
  10053d:	66 90                	xchg   %ax,%ax
  10053f:	90                   	nop

00100540 <lapicinit>:
}

void
lapicinit(void)
{
  if(!lapic)
  100540:	a1 90 24 10 00       	mov    0x102490,%eax
  100545:	85 c0                	test   %eax,%eax
  100547:	0f 84 c3 00 00 00    	je     100610 <lapicinit+0xd0>
  lapic[index] = value;
  10054d:	c7 80 f0 00 00 00 3f 	movl   $0x13f,0xf0(%eax)
  100554:	01 00 00 
  lapic[ID];  // wait for write to finish, by reading
  100557:	8b 50 20             	mov    0x20(%eax),%edx
  lapic[index] = value;
  10055a:	c7 80 e0 03 00 00 0b 	movl   $0xb,0x3e0(%eax)
  100561:	00 00 00 
  lapic[ID];  // wait for write to finish, by reading
  100564:	8b 50 20             	mov    0x20(%eax),%edx
  lapic[index] = value;
  100567:	c7 80 20 03 00 00 20 	movl   $0x20020,0x320(%eax)
  10056e:	00 02 00 
  lapic[ID];  // wait for write to finish, by reading
  100571:	8b 50 20             	mov    0x20(%eax),%edx
  lapic[index] = value;
  100574:	c7 80 80 03 00 00 80 	movl   $0x989680,0x380(%eax)
  10057b:	96 98 00 
  lapic[ID];  // wait for write to finish, by reading
  10057e:	8b 50 20             	mov    0x20(%eax),%edx
  lapic[index] = value;
  100581:	c7 80 50 03 00 00 00 	movl   $0x10000,0x350(%eax)
  100588:	00 01 00 
  lapic[ID];  // wait for write to finish, by reading
  10058b:	8b 50 20             	mov    0x20(%eax),%edx
  lapic[index] = value;
  10058e:	c7 80 60 03 00 00 00 	movl   $0x10000,0x360(%eax)
  100595:	00 01 00 
  lapic[ID];  // wait for write to finish, by reading
  100598:	8b 50 20             	mov    0x20(%eax),%edx
  lapicw(LINT0, MASKED);
  lapicw(LINT1, MASKED);

  // Disable performance counter overflow interrupts
  // on machines that provide that interrupt entry.
  if(((lapic[VER]>>16) & 0xFF) >= 4)
  10059b:	8b 50 30             	mov    0x30(%eax),%edx
  10059e:	81 e2 00 00 fc 00    	and    $0xfc0000,%edx
  1005a4:	75 72                	jne    100618 <lapicinit+0xd8>
  lapic[index] = value;
  1005a6:	c7 80 70 03 00 00 33 	movl   $0x33,0x370(%eax)
  1005ad:	00 00 00 
  lapic[ID];  // wait for write to finish, by reading
  1005b0:	8b 50 20             	mov    0x20(%eax),%edx
  lapic[index] = value;
  1005b3:	c7 80 80 02 00 00 00 	movl   $0x0,0x280(%eax)
  1005ba:	00 00 00 
  lapic[ID];  // wait for write to finish, by reading
  1005bd:	8b 50 20             	mov    0x20(%eax),%edx
  lapic[index] = value;
  1005c0:	c7 80 80 02 00 00 00 	movl   $0x0,0x280(%eax)
  1005c7:	00 00 00 
  lapic[ID];  // wait for write to finish, by reading
  1005ca:	8b 50 20             	mov    0x20(%eax),%edx
  lapic[index] = value;
  1005cd:	c7 80 b0 00 00 00 00 	movl   $0x0,0xb0(%eax)
  1005d4:	00 00 00 
  lapic[ID];  // wait for write to finish, by reading
  1005d7:	8b 50 20             	mov    0x20(%eax),%edx
  lapic[index] = value;
  1005da:	c7 80 10 03 00 00 00 	movl   $0x0,0x310(%eax)
  1005e1:	00 00 00 
  lapic[ID];  // wait for write to finish, by reading
  1005e4:	8b 50 20             	mov    0x20(%eax),%edx
  lapic[index] = value;
  1005e7:	c7 80 00 03 00 00 00 	movl   $0x88500,0x300(%eax)
  1005ee:	85 08 00 
  lapic[ID];  // wait for write to finish, by reading
  1005f1:	8b 50 20             	mov    0x20(%eax),%edx
  lapicw(EOI, 0);

  // Send an Init Level De-Assert to synchronise arbitration ID's.
  lapicw(ICRHI, 0);
  lapicw(ICRLO, BCAST | INIT | LEVEL);
  while(lapic[ICRLO] & DELIVS)
  1005f4:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  1005f8:	8b 90 00 03 00 00    	mov    0x300(%eax),%edx
  1005fe:	80 e6 10             	and    $0x10,%dh
  100601:	75 f5                	jne    1005f8 <lapicinit+0xb8>
  lapic[index] = value;
  100603:	c7 80 80 00 00 00 00 	movl   $0x0,0x80(%eax)
  10060a:	00 00 00 
  lapic[ID];  // wait for write to finish, by reading
  10060d:	8b 40 20             	mov    0x20(%eax),%eax
    ;

  // Enable interrupts on the APIC (but not on the processor).
  lapicw(TPR, 0);
}
  100610:	c3                   	ret
  100611:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  lapic[index] = value;
  100618:	c7 80 40 03 00 00 00 	movl   $0x10000,0x340(%eax)
  10061f:	00 01 00 
  lapic[ID];  // wait for write to finish, by reading
  100622:	8b 50 20             	mov    0x20(%eax),%edx
}
  100625:	e9 7c ff ff ff       	jmp    1005a6 <lapicinit+0x66>
  10062a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi

00100630 <lapicid>:

int
lapicid(void)
{
  if (!lapic)
  100630:	a1 90 24 10 00       	mov    0x102490,%eax
  100635:	85 c0                	test   %eax,%eax
  100637:	74 07                	je     100640 <lapicid+0x10>
    return 0;
  return lapic[ID] >> 24;
  100639:	8b 40 20             	mov    0x20(%eax),%eax
  10063c:	c1 e8 18             	shr    $0x18,%eax
  10063f:	c3                   	ret
    return 0;
  100640:	31 c0                	xor    %eax,%eax
}
  100642:	c3                   	ret
  100643:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  10064a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi

00100650 <lapiceoi>:

// Acknowledge interrupt.
void
lapiceoi(void)
{
  if(lapic)
  100650:	a1 90 24 10 00       	mov    0x102490,%eax
  100655:	85 c0                	test   %eax,%eax
  100657:	74 0d                	je     100666 <lapiceoi+0x16>
  lapic[index] = value;
  100659:	c7 80 b0 00 00 00 00 	movl   $0x0,0xb0(%eax)
  100660:	00 00 00 
  lapic[ID];  // wait for write to finish, by reading
  100663:	8b 40 20             	mov    0x20(%eax),%eax
    lapicw(EOI, 0);
}
  100666:	c3                   	ret
  100667:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  10066e:	66 90                	xchg   %ax,%ax

00100670 <microdelay>:
// Spin for a given number of microseconds.
// On real hardware would want to tune this dynamically.
void
microdelay(int us)
{
  100670:	c3                   	ret
  100671:	66 90                	xchg   %ax,%ax
  100673:	66 90                	xchg   %ax,%ax
  100675:	66 90                	xchg   %ax,%ax
  100677:	66 90                	xchg   %ax,%ax
  100679:	66 90                	xchg   %ax,%ax
  10067b:	66 90                	xchg   %ax,%ax
  10067d:	66 90                	xchg   %ax,%ax
  10067f:	90                   	nop

00100680 <main>:
// Bootstrap processor starts running C code here.
// Allocate a real stack and switch to it, first
// doing some setup required for memory allocator to work.
int
main(void)
{
  100680:	55                   	push   %ebp
  100681:	89 e5                	mov    %esp,%ebp
  100683:	83 e4 f0             	and    $0xfffffff0,%esp
  mpinit();        // detect other processors
  100686:	e8 f5 00 00 00       	call   100780 <mpinit>
  lapicinit();     // interrupt controller
  10068b:	e8 b0 fe ff ff       	call   100540 <lapicinit>
  picinit();       // disable pic
  100690:	e8 0b 03 00 00       	call   1009a0 <picinit>
  ioapicinit();    // another interrupt controller
  100695:	e8 c6 fd ff ff       	call   100460 <ioapicinit>
  uartinit();      // serial port
  10069a:	e8 51 03 00 00       	call   1009f0 <uartinit>
  mouseinit();      // mouse init
  10069f:	e8 1c 15 00 00       	call   101bc0 <mouseinit>
  tvinit();         // trap vectors
  1006a4:	e8 b7 07 00 00       	call   100e60 <tvinit>
  idtinit();       // load idt register
  1006a9:	e8 32 09 00 00       	call   100fe0 <idtinit>


static inline void
sti(void)
{
  asm volatile("sti");
  1006ae:	fb                   	sti
  1006af:	90                   	nop
}

static inline void
wfi(void)
{
  asm volatile("hlt");
  1006b0:	f4                   	hlt
  sti();
  for(;;)
  1006b1:	eb fd                	jmp    1006b0 <main+0x30>
  1006b3:	66 90                	xchg   %ax,%ax
  1006b5:	66 90                	xchg   %ax,%ax
  1006b7:	66 90                	xchg   %ax,%ax
  1006b9:	66 90                	xchg   %ax,%ax
  1006bb:	66 90                	xchg   %ax,%ax
  1006bd:	66 90                	xchg   %ax,%ax
  1006bf:	90                   	nop

001006c0 <mpsearch1>:
}

// Look for an MP structure in the len bytes at addr.
static struct mp*
mpsearch1(uint a, int len)
{
  1006c0:	55                   	push   %ebp
  1006c1:	89 e5                	mov    %esp,%ebp
  1006c3:	56                   	push   %esi
  uchar *e, *p, *addr;

  // addr = P2V(a);
  addr = (uchar*) a;
  e = addr+len;
  1006c4:	8d 34 10             	lea    (%eax,%edx,1),%esi
{
  1006c7:	53                   	push   %ebx
  for(p = addr; p < e; p += sizeof(struct mp))
  1006c8:	39 f0                	cmp    %esi,%eax
  1006ca:	0f 83 a0 00 00 00    	jae    100770 <mpsearch1+0xb0>
  1006d0:	89 c3                	mov    %eax,%ebx
  1006d2:	eb 0f                	jmp    1006e3 <mpsearch1+0x23>
  1006d4:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  1006d8:	83 c3 10             	add    $0x10,%ebx
  1006db:	39 f3                	cmp    %esi,%ebx
  1006dd:	0f 83 8d 00 00 00    	jae    100770 <mpsearch1+0xb0>
    if(memcmp(p, "_MP_", 4) == 0 && sum(p, sizeof(struct mp)) == 0)
  1006e3:	83 ec 04             	sub    $0x4,%esp
  1006e6:	6a 04                	push   $0x4
  1006e8:	68 8f 1d 10 00       	push   $0x101d8f
  1006ed:	53                   	push   %ebx
  1006ee:	e8 8d 04 00 00       	call   100b80 <memcmp>
  1006f3:	83 c4 10             	add    $0x10,%esp
  1006f6:	85 c0                	test   %eax,%eax
  1006f8:	75 de                	jne    1006d8 <mpsearch1+0x18>
    sum += addr[i];
  1006fa:	66 0f 6f 0b          	movdqa (%ebx),%xmm1
  1006fe:	66 0f ef ed          	pxor   %xmm5,%xmm5
  100702:	66 0f 6f 13          	movdqa (%ebx),%xmm2
  100706:	66 0f ef e4          	pxor   %xmm4,%xmm4
  10070a:	66 0f 60 cd          	punpcklbw %xmm5,%xmm1
  10070e:	66 0f 68 d5          	punpckhbw %xmm5,%xmm2
  100712:	66 0f 6f d9          	movdqa %xmm1,%xmm3
  100716:	66 0f 6f c1          	movdqa %xmm1,%xmm0
  10071a:	66 0f 6f ca          	movdqa %xmm2,%xmm1
  10071e:	66 0f 61 dc          	punpcklwd %xmm4,%xmm3
  100722:	66 0f 69 c4          	punpckhwd %xmm4,%xmm0
  100726:	66 0f 61 cc          	punpcklwd %xmm4,%xmm1
  10072a:	66 0f fe c3          	paddd  %xmm3,%xmm0
  10072e:	66 0f 69 d4          	punpckhwd %xmm4,%xmm2
  100732:	66 0f fe c1          	paddd  %xmm1,%xmm0
  100736:	66 0f fe c2          	paddd  %xmm2,%xmm0
  for(i=0; i<len; i++)
  10073a:	66 0f 6f c8          	movdqa %xmm0,%xmm1
  10073e:	66 0f 73 d9 08       	psrldq $0x8,%xmm1
  100743:	66 0f fe c1          	paddd  %xmm1,%xmm0
  return sum;
  100747:	66 0f 6f c8          	movdqa %xmm0,%xmm1
  10074b:	66 0f 73 d9 04       	psrldq $0x4,%xmm1
  100750:	66 0f fe c1          	paddd  %xmm1,%xmm0
  100754:	66 0f 7e c0          	movd   %xmm0,%eax
    if(memcmp(p, "_MP_", 4) == 0 && sum(p, sizeof(struct mp)) == 0)
  100758:	84 c0                	test   %al,%al
  10075a:	0f 85 78 ff ff ff    	jne    1006d8 <mpsearch1+0x18>
      return (struct mp*)p;
  return 0;
}
  100760:	8d 65 f8             	lea    -0x8(%ebp),%esp
  100763:	89 d8                	mov    %ebx,%eax
  100765:	5b                   	pop    %ebx
  100766:	5e                   	pop    %esi
  100767:	5d                   	pop    %ebp
  100768:	c3                   	ret
  100769:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  100770:	8d 65 f8             	lea    -0x8(%ebp),%esp
  return 0;
  100773:	31 db                	xor    %ebx,%ebx
}
  100775:	89 d8                	mov    %ebx,%eax
  100777:	5b                   	pop    %ebx
  100778:	5e                   	pop    %esi
  100779:	5d                   	pop    %ebp
  10077a:	c3                   	ret
  10077b:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  10077f:	90                   	nop

00100780 <mpinit>:
  return conf;
}

void
mpinit(void)
{
  100780:	55                   	push   %ebp
  100781:	89 e5                	mov    %esp,%ebp
  100783:	57                   	push   %edi
  100784:	56                   	push   %esi
  100785:	53                   	push   %ebx
  100786:	83 ec 1c             	sub    $0x1c,%esp
  if((p = ((bda[0x0F]<<8)| bda[0x0E]) << 4)){
  100789:	0f b6 05 0f 04 00 00 	movzbl 0x40f,%eax
  100790:	0f b6 15 0e 04 00 00 	movzbl 0x40e,%edx
  100797:	c1 e0 08             	shl    $0x8,%eax
  10079a:	09 d0                	or     %edx,%eax
  10079c:	c1 e0 04             	shl    $0x4,%eax
  10079f:	75 1b                	jne    1007bc <mpinit+0x3c>
    p = ((bda[0x14]<<8)|bda[0x13])*1024;
  1007a1:	0f b6 05 14 04 00 00 	movzbl 0x414,%eax
  1007a8:	0f b6 15 13 04 00 00 	movzbl 0x413,%edx
  1007af:	c1 e0 08             	shl    $0x8,%eax
  1007b2:	09 d0                	or     %edx,%eax
  1007b4:	c1 e0 0a             	shl    $0xa,%eax
    if((mp = mpsearch1(p-1024, 1024)))
  1007b7:	2d 00 04 00 00       	sub    $0x400,%eax
    if((mp = mpsearch1(p, 1024)))
  1007bc:	ba 00 04 00 00       	mov    $0x400,%edx
  1007c1:	e8 fa fe ff ff       	call   1006c0 <mpsearch1>
  1007c6:	89 c6                	mov    %eax,%esi
  1007c8:	85 c0                	test   %eax,%eax
  1007ca:	0f 84 28 01 00 00    	je     1008f8 <mpinit+0x178>
  if((mp = mpsearch()) == 0 || mp->physaddr == 0)
  1007d0:	8b 5e 04             	mov    0x4(%esi),%ebx
  1007d3:	85 db                	test   %ebx,%ebx
  1007d5:	0f 84 0d 01 00 00    	je     1008e8 <mpinit+0x168>
  if(memcmp(conf, "PCMP", 4) != 0)
  1007db:	83 ec 04             	sub    $0x4,%esp
  1007de:	6a 04                	push   $0x4
  1007e0:	68 94 1d 10 00       	push   $0x101d94
  1007e5:	53                   	push   %ebx
  1007e6:	e8 95 03 00 00       	call   100b80 <memcmp>
  1007eb:	83 c4 10             	add    $0x10,%esp
  1007ee:	85 c0                	test   %eax,%eax
  1007f0:	0f 85 f2 00 00 00    	jne    1008e8 <mpinit+0x168>
  if(conf->version != 1 && conf->version != 4)
  1007f6:	0f b6 43 06          	movzbl 0x6(%ebx),%eax
  1007fa:	3c 01                	cmp    $0x1,%al
  1007fc:	74 08                	je     100806 <mpinit+0x86>
  1007fe:	3c 04                	cmp    $0x4,%al
  100800:	0f 85 e2 00 00 00    	jne    1008e8 <mpinit+0x168>
  if(sum((uchar*)conf, conf->length) != 0)
  100806:	0f b7 53 04          	movzwl 0x4(%ebx),%edx
  for(i=0; i<len; i++)
  10080a:	31 c9                	xor    %ecx,%ecx
  10080c:	66 85 d2             	test   %dx,%dx
  10080f:	74 26                	je     100837 <mpinit+0xb7>
  sum = 0;
  100811:	89 75 e4             	mov    %esi,-0x1c(%ebp)
  100814:	0f b7 ca             	movzwl %dx,%ecx
  100817:	89 d8                	mov    %ebx,%eax
  100819:	31 d2                	xor    %edx,%edx
  10081b:	8d 3c 0b             	lea    (%ebx,%ecx,1),%edi
  10081e:	66 90                	xchg   %ax,%ax
    sum += addr[i];
  100820:	0f b6 30             	movzbl (%eax),%esi
  for(i=0; i<len; i++)
  100823:	83 c0 01             	add    $0x1,%eax
    sum += addr[i];
  100826:	01 f2                	add    %esi,%edx
  for(i=0; i<len; i++)
  100828:	39 c7                	cmp    %eax,%edi
  10082a:	75 f4                	jne    100820 <mpinit+0xa0>
  if(sum((uchar*)conf, conf->length) != 0)
  10082c:	8b 75 e4             	mov    -0x1c(%ebp),%esi
  10082f:	84 d2                	test   %dl,%dl
  100831:	0f 85 b1 00 00 00    	jne    1008e8 <mpinit+0x168>
  struct mpioapic *ioapic;

  if((conf = mpconfig(&mp)) == 0)
    panic("Expect to run on an SMP");
  ismp = 1;
  lapic = (uint*)conf->lapicaddr;
  100837:	8b 43 24             	mov    0x24(%ebx),%eax
  10083a:	a3 90 24 10 00       	mov    %eax,0x102490
  for(p=(uchar*)(conf+1), e=(uchar*)conf+conf->length; p<e; ){
  10083f:	8d 43 2c             	lea    0x2c(%ebx),%eax
  100842:	01 cb                	add    %ecx,%ebx
  ismp = 1;
  100844:	b9 01 00 00 00       	mov    $0x1,%ecx
  100849:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  for(p=(uchar*)(conf+1), e=(uchar*)conf+conf->length; p<e; ){
  100850:	39 d8                	cmp    %ebx,%eax
  100852:	73 15                	jae    100869 <mpinit+0xe9>
    switch(*p){
  100854:	0f b6 10             	movzbl (%eax),%edx
  100857:	80 fa 02             	cmp    $0x2,%dl
  10085a:	74 74                	je     1008d0 <mpinit+0x150>
  10085c:	77 62                	ja     1008c0 <mpinit+0x140>
  10085e:	84 d2                	test   %dl,%dl
  100860:	74 36                	je     100898 <mpinit+0x118>
      p += sizeof(struct mpioapic);
      continue;
    case MPBUS:
    case MPIOINTR:
    case MPLINTR:
      p += 8;
  100862:	83 c0 08             	add    $0x8,%eax
  for(p=(uchar*)(conf+1), e=(uchar*)conf+conf->length; p<e; ){
  100865:	39 d8                	cmp    %ebx,%eax
  100867:	72 eb                	jb     100854 <mpinit+0xd4>
    default:
      ismp = 0;
      break;
    }
  }
  if(!ismp)
  100869:	85 c9                	test   %ecx,%ecx
  10086b:	0f 84 1c 01 00 00    	je     10098d <mpinit+0x20d>
    panic("Didn't find a suitable machine");

  if(mp->imcrp){
  100871:	80 7e 0c 00          	cmpb   $0x0,0xc(%esi)
  100875:	74 15                	je     10088c <mpinit+0x10c>
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  100877:	b8 70 00 00 00       	mov    $0x70,%eax
  10087c:	ba 22 00 00 00       	mov    $0x22,%edx
  100881:	ee                   	out    %al,(%dx)
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  100882:	ba 23 00 00 00       	mov    $0x23,%edx
  100887:	ec                   	in     (%dx),%al
    // Bochs doesn't support IMCR, so this doesn't run on Bochs.
    // But it would on real hardware.
    outb(0x22, 0x70);   // Select IMCR
    outb(0x23, inb(0x23) | 1);  // Mask external interrupts.
  100888:	83 c8 01             	or     $0x1,%eax
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  10088b:	ee                   	out    %al,(%dx)
  }
}
  10088c:	8d 65 f4             	lea    -0xc(%ebp),%esp
  10088f:	5b                   	pop    %ebx
  100890:	5e                   	pop    %esi
  100891:	5f                   	pop    %edi
  100892:	5d                   	pop    %ebp
  100893:	c3                   	ret
  100894:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
      if(ncpu < NCPU) {
  100898:	8b 3d 98 24 10 00    	mov    0x102498,%edi
  10089e:	83 ff 07             	cmp    $0x7,%edi
  1008a1:	7f 13                	jg     1008b6 <mpinit+0x136>
        cpus[ncpu].apicid = proc->apicid;  // apicid may differ from ncpu
  1008a3:	0f b6 50 01          	movzbl 0x1(%eax),%edx
        ncpu++;
  1008a7:	83 c7 01             	add    $0x1,%edi
        cpus[ncpu].apicid = proc->apicid;  // apicid may differ from ncpu
  1008aa:	88 97 9b 24 10 00    	mov    %dl,0x10249b(%edi)
        ncpu++;
  1008b0:	89 3d 98 24 10 00    	mov    %edi,0x102498
      p += sizeof(struct mpproc);
  1008b6:	83 c0 14             	add    $0x14,%eax
      continue;
  1008b9:	eb 95                	jmp    100850 <mpinit+0xd0>
  1008bb:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  1008bf:	90                   	nop
    switch(*p){
  1008c0:	83 ea 03             	sub    $0x3,%edx
  1008c3:	80 fa 01             	cmp    $0x1,%dl
  1008c6:	76 9a                	jbe    100862 <mpinit+0xe2>
  1008c8:	31 c9                	xor    %ecx,%ecx
  1008ca:	eb 84                	jmp    100850 <mpinit+0xd0>
  1008cc:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
      ioapicid = ioapic->apicno;
  1008d0:	0f b6 50 01          	movzbl 0x1(%eax),%edx
      p += sizeof(struct mpioapic);
  1008d4:	83 c0 08             	add    $0x8,%eax
      ioapicid = ioapic->apicno;
  1008d7:	88 15 94 24 10 00    	mov    %dl,0x102494
      continue;
  1008dd:	e9 6e ff ff ff       	jmp    100850 <mpinit+0xd0>
  1008e2:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
    panic("Expect to run on an SMP");
  1008e8:	83 ec 0c             	sub    $0xc,%esp
  1008eb:	68 99 1d 10 00       	push   $0x101d99
  1008f0:	e8 3b f9 ff ff       	call   100230 <panic>
  1008f5:	8d 76 00             	lea    0x0(%esi),%esi
{
  1008f8:	be 00 00 0f 00       	mov    $0xf0000,%esi
  1008fd:	eb 0c                	jmp    10090b <mpinit+0x18b>
  1008ff:	90                   	nop
  for(p = addr; p < e; p += sizeof(struct mp))
  100900:	83 c6 10             	add    $0x10,%esi
  100903:	81 fe 00 00 10 00    	cmp    $0x100000,%esi
  100909:	74 dd                	je     1008e8 <mpinit+0x168>
    if(memcmp(p, "_MP_", 4) == 0 && sum(p, sizeof(struct mp)) == 0)
  10090b:	83 ec 04             	sub    $0x4,%esp
  10090e:	6a 04                	push   $0x4
  100910:	68 8f 1d 10 00       	push   $0x101d8f
  100915:	56                   	push   %esi
  100916:	e8 65 02 00 00       	call   100b80 <memcmp>
  10091b:	83 c4 10             	add    $0x10,%esp
  10091e:	85 c0                	test   %eax,%eax
  100920:	75 de                	jne    100900 <mpinit+0x180>
    sum += addr[i];
  100922:	66 0f 6f 0e          	movdqa (%esi),%xmm1
  100926:	66 0f ef ed          	pxor   %xmm5,%xmm5
  10092a:	66 0f 6f 16          	movdqa (%esi),%xmm2
  10092e:	66 0f ef e4          	pxor   %xmm4,%xmm4
  100932:	66 0f 60 cd          	punpcklbw %xmm5,%xmm1
  100936:	66 0f 68 d5          	punpckhbw %xmm5,%xmm2
  10093a:	66 0f 6f d9          	movdqa %xmm1,%xmm3
  10093e:	66 0f 6f c1          	movdqa %xmm1,%xmm0
  100942:	66 0f 6f ca          	movdqa %xmm2,%xmm1
  100946:	66 0f 61 dc          	punpcklwd %xmm4,%xmm3
  10094a:	66 0f 69 c4          	punpckhwd %xmm4,%xmm0
  10094e:	66 0f 61 cc          	punpcklwd %xmm4,%xmm1
  100952:	66 0f fe c3          	paddd  %xmm3,%xmm0
  100956:	66 0f 69 d4          	punpckhwd %xmm4,%xmm2
  10095a:	66 0f fe c1          	paddd  %xmm1,%xmm0
  10095e:	66 0f fe c2          	paddd  %xmm2,%xmm0
  for(i=0; i<len; i++)
  100962:	66 0f 6f c8          	movdqa %xmm0,%xmm1
  100966:	66 0f 73 d9 08       	psrldq $0x8,%xmm1
  10096b:	66 0f fe c1          	paddd  %xmm1,%xmm0
  return sum;
  10096f:	66 0f 6f c8          	movdqa %xmm0,%xmm1
  100973:	66 0f 73 d9 04       	psrldq $0x4,%xmm1
  100978:	66 0f fe c1          	paddd  %xmm1,%xmm0
  10097c:	66 0f 7e c0          	movd   %xmm0,%eax
    if(memcmp(p, "_MP_", 4) == 0 && sum(p, sizeof(struct mp)) == 0)
  100980:	84 c0                	test   %al,%al
  100982:	0f 85 78 ff ff ff    	jne    100900 <mpinit+0x180>
  100988:	e9 43 fe ff ff       	jmp    1007d0 <mpinit+0x50>
    panic("Didn't find a suitable machine");
  10098d:	83 ec 0c             	sub    $0xc,%esp
  100990:	68 54 1e 10 00       	push   $0x101e54
  100995:	e8 96 f8 ff ff       	call   100230 <panic>
  10099a:	66 90                	xchg   %ax,%ax
  10099c:	66 90                	xchg   %ax,%ax
  10099e:	66 90                	xchg   %ax,%ax

001009a0 <picinit>:
  1009a0:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  1009a5:	ba 21 00 00 00       	mov    $0x21,%edx
  1009aa:	ee                   	out    %al,(%dx)
  1009ab:	ba a1 00 00 00       	mov    $0xa1,%edx
  1009b0:	ee                   	out    %al,(%dx)
picinit(void)
{
  // mask all interrupts
  outb(IO_PIC1+1, 0xFF);
  outb(IO_PIC2+1, 0xFF);
}
  1009b1:	c3                   	ret
  1009b2:	66 90                	xchg   %ax,%ax
  1009b4:	66 90                	xchg   %ax,%ax
  1009b6:	66 90                	xchg   %ax,%ax
  1009b8:	66 90                	xchg   %ax,%ax
  1009ba:	66 90                	xchg   %ax,%ax
  1009bc:	66 90                	xchg   %ax,%ax
  1009be:	66 90                	xchg   %ax,%ax

001009c0 <uartgetc>:


static int
uartgetc(void)
{
  if(!uart)
  1009c0:	a1 a4 24 10 00       	mov    0x1024a4,%eax
  1009c5:	85 c0                	test   %eax,%eax
  1009c7:	74 17                	je     1009e0 <uartgetc+0x20>
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  1009c9:	ba fd 03 00 00       	mov    $0x3fd,%edx
  1009ce:	ec                   	in     (%dx),%al
    return -1;
  if(!(inb(COM1+5) & 0x01))
  1009cf:	a8 01                	test   $0x1,%al
  1009d1:	74 0d                	je     1009e0 <uartgetc+0x20>
  1009d3:	ba f8 03 00 00       	mov    $0x3f8,%edx
  1009d8:	ec                   	in     (%dx),%al
    return -1;
  return inb(COM1+0);
  1009d9:	0f b6 c0             	movzbl %al,%eax
  1009dc:	c3                   	ret
  1009dd:	8d 76 00             	lea    0x0(%esi),%esi
    return -1;
  1009e0:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
}
  1009e5:	c3                   	ret
  1009e6:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  1009ed:	8d 76 00             	lea    0x0(%esi),%esi

001009f0 <uartinit>:
{
  1009f0:	55                   	push   %ebp
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  1009f1:	31 c9                	xor    %ecx,%ecx
  1009f3:	89 c8                	mov    %ecx,%eax
  1009f5:	89 e5                	mov    %esp,%ebp
  1009f7:	57                   	push   %edi
  1009f8:	bf fa 03 00 00       	mov    $0x3fa,%edi
  1009fd:	56                   	push   %esi
  1009fe:	89 fa                	mov    %edi,%edx
  100a00:	53                   	push   %ebx
  100a01:	83 ec 0c             	sub    $0xc,%esp
  100a04:	ee                   	out    %al,(%dx)
  100a05:	be fb 03 00 00       	mov    $0x3fb,%esi
  100a0a:	b8 80 ff ff ff       	mov    $0xffffff80,%eax
  100a0f:	89 f2                	mov    %esi,%edx
  100a11:	ee                   	out    %al,(%dx)
  100a12:	b8 0c 00 00 00       	mov    $0xc,%eax
  100a17:	ba f8 03 00 00       	mov    $0x3f8,%edx
  100a1c:	ee                   	out    %al,(%dx)
  100a1d:	bb f9 03 00 00       	mov    $0x3f9,%ebx
  100a22:	89 c8                	mov    %ecx,%eax
  100a24:	89 da                	mov    %ebx,%edx
  100a26:	ee                   	out    %al,(%dx)
  100a27:	b8 03 00 00 00       	mov    $0x3,%eax
  100a2c:	89 f2                	mov    %esi,%edx
  100a2e:	ee                   	out    %al,(%dx)
  100a2f:	ba fc 03 00 00       	mov    $0x3fc,%edx
  100a34:	89 c8                	mov    %ecx,%eax
  100a36:	ee                   	out    %al,(%dx)
  100a37:	b8 01 00 00 00       	mov    $0x1,%eax
  100a3c:	89 da                	mov    %ebx,%edx
  100a3e:	ee                   	out    %al,(%dx)
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  100a3f:	ba fd 03 00 00       	mov    $0x3fd,%edx
  100a44:	ec                   	in     (%dx),%al
  if(inb(COM1+5) == 0xFF)
  100a45:	3c ff                	cmp    $0xff,%al
  100a47:	74 3d                	je     100a86 <uartinit+0x96>
  uart = 1;
  100a49:	c7 05 a4 24 10 00 01 	movl   $0x1,0x1024a4
  100a50:	00 00 00 
  100a53:	89 fa                	mov    %edi,%edx
  100a55:	ec                   	in     (%dx),%al
  100a56:	ba f8 03 00 00       	mov    $0x3f8,%edx
  100a5b:	ec                   	in     (%dx),%al
  ioapicenable(IRQ_COM1, 0);
  100a5c:	83 ec 08             	sub    $0x8,%esp
  for(p="xv6...\n"; *p; p++)
  100a5f:	be b1 1d 10 00       	mov    $0x101db1,%esi
  100a64:	bf 78 00 00 00       	mov    $0x78,%edi
  ioapicenable(IRQ_COM1, 0);
  100a69:	6a 00                	push   $0x0
  100a6b:	6a 04                	push   $0x4
  100a6d:	e8 8e fa ff ff       	call   100500 <ioapicenable>
  if(!uart)
  100a72:	a1 a4 24 10 00       	mov    0x1024a4,%eax
  100a77:	83 c4 10             	add    $0x10,%esp
  100a7a:	85 c0                	test   %eax,%eax
  100a7c:	75 10                	jne    100a8e <uartinit+0x9e>
  for(p="xv6...\n"; *p; p++)
  100a7e:	83 c6 01             	add    $0x1,%esi
  100a81:	80 3e 00             	cmpb   $0x0,(%esi)
  100a84:	75 f8                	jne    100a7e <uartinit+0x8e>
}
  100a86:	8d 65 f4             	lea    -0xc(%ebp),%esp
  100a89:	5b                   	pop    %ebx
  100a8a:	5e                   	pop    %esi
  100a8b:	5f                   	pop    %edi
  100a8c:	5d                   	pop    %ebp
  100a8d:	c3                   	ret
  100a8e:	bb fd 03 00 00       	mov    $0x3fd,%ebx
  100a93:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  100a97:	90                   	nop
  100a98:	89 da                	mov    %ebx,%edx
  100a9a:	ec                   	in     (%dx),%al
  for(i = 0; i < 128 && !(inb(COM1+5) & 0x20); i++);
  100a9b:	a8 20                	test   $0x20,%al
  100a9d:	75 15                	jne    100ab4 <uartinit+0xc4>
  100a9f:	b9 80 00 00 00       	mov    $0x80,%ecx
  100aa4:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  100aa8:	83 e9 01             	sub    $0x1,%ecx
  100aab:	74 07                	je     100ab4 <uartinit+0xc4>
  100aad:	89 da                	mov    %ebx,%edx
  100aaf:	ec                   	in     (%dx),%al
  100ab0:	a8 20                	test   $0x20,%al
  100ab2:	74 f4                	je     100aa8 <uartinit+0xb8>
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  100ab4:	ba f8 03 00 00       	mov    $0x3f8,%edx
  100ab9:	89 f8                	mov    %edi,%eax
  100abb:	ee                   	out    %al,(%dx)
  for(p="xv6...\n"; *p; p++)
  100abc:	0f b6 7e 01          	movzbl 0x1(%esi),%edi
  100ac0:	83 c6 01             	add    $0x1,%esi
  100ac3:	89 f8                	mov    %edi,%eax
  100ac5:	84 c0                	test   %al,%al
  100ac7:	75 cf                	jne    100a98 <uartinit+0xa8>
}
  100ac9:	8d 65 f4             	lea    -0xc(%ebp),%esp
  100acc:	5b                   	pop    %ebx
  100acd:	5e                   	pop    %esi
  100ace:	5f                   	pop    %edi
  100acf:	5d                   	pop    %ebp
  100ad0:	c3                   	ret
  100ad1:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  100ad8:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  100adf:	90                   	nop

00100ae0 <uartputc>:
  if(!uart)
  100ae0:	a1 a4 24 10 00       	mov    0x1024a4,%eax
  100ae5:	85 c0                	test   %eax,%eax
  100ae7:	74 2f                	je     100b18 <uartputc+0x38>
{
  100ae9:	55                   	push   %ebp
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  100aea:	ba fd 03 00 00       	mov    $0x3fd,%edx
  100aef:	89 e5                	mov    %esp,%ebp
  100af1:	ec                   	in     (%dx),%al
  for(i = 0; i < 128 && !(inb(COM1+5) & 0x20); i++);
  100af2:	a8 20                	test   $0x20,%al
  100af4:	75 14                	jne    100b0a <uartputc+0x2a>
  100af6:	b9 80 00 00 00       	mov    $0x80,%ecx
  100afb:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  100aff:	90                   	nop
  100b00:	83 e9 01             	sub    $0x1,%ecx
  100b03:	74 05                	je     100b0a <uartputc+0x2a>
  100b05:	ec                   	in     (%dx),%al
  100b06:	a8 20                	test   $0x20,%al
  100b08:	74 f6                	je     100b00 <uartputc+0x20>
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  100b0a:	8b 45 08             	mov    0x8(%ebp),%eax
  100b0d:	ba f8 03 00 00       	mov    $0x3f8,%edx
  100b12:	ee                   	out    %al,(%dx)
}
  100b13:	5d                   	pop    %ebp
  100b14:	c3                   	ret
  100b15:	8d 76 00             	lea    0x0(%esi),%esi
  100b18:	c3                   	ret
  100b19:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi

00100b20 <uartintr>:

void
uartintr(void)
{
  100b20:	55                   	push   %ebp
  100b21:	89 e5                	mov    %esp,%ebp
  100b23:	83 ec 14             	sub    $0x14,%esp
  consoleintr(uartgetc);
  100b26:	68 c0 09 10 00       	push   $0x1009c0
  100b2b:	e8 70 f7 ff ff       	call   1002a0 <consoleintr>
  100b30:	83 c4 10             	add    $0x10,%esp
  100b33:	c9                   	leave
  100b34:	c3                   	ret
  100b35:	66 90                	xchg   %ax,%ax
  100b37:	66 90                	xchg   %ax,%ax
  100b39:	66 90                	xchg   %ax,%ax
  100b3b:	66 90                	xchg   %ax,%ax
  100b3d:	66 90                	xchg   %ax,%ax
  100b3f:	90                   	nop

00100b40 <memset>:
#include "types.h"
#include "x86.h"

void*
memset(void *dst, int c, uint n)
{
  100b40:	55                   	push   %ebp
  100b41:	89 e5                	mov    %esp,%ebp
  100b43:	57                   	push   %edi
  100b44:	8b 55 08             	mov    0x8(%ebp),%edx
  100b47:	8b 4d 10             	mov    0x10(%ebp),%ecx
  if ((int)dst%4 == 0 && n%4 == 0){
  100b4a:	89 d0                	mov    %edx,%eax
  100b4c:	09 c8                	or     %ecx,%eax
  100b4e:	a8 03                	test   $0x3,%al
  100b50:	75 1e                	jne    100b70 <memset+0x30>
    c &= 0xFF;
  100b52:	0f b6 45 0c          	movzbl 0xc(%ebp),%eax
    stosl(dst, (c<<24)|(c<<16)|(c<<8)|c, n/4);
  100b56:	c1 e9 02             	shr    $0x2,%ecx
  asm volatile("cld; rep stosl" :
  100b59:	89 d7                	mov    %edx,%edi
  100b5b:	69 c0 01 01 01 01    	imul   $0x1010101,%eax,%eax
  100b61:	fc                   	cld
  100b62:	f3 ab                	rep stos %eax,%es:(%edi)
  } else
    stosb(dst, c, n);
  return dst;
}
  100b64:	8b 7d fc             	mov    -0x4(%ebp),%edi
  100b67:	89 d0                	mov    %edx,%eax
  100b69:	c9                   	leave
  100b6a:	c3                   	ret
  100b6b:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  100b6f:	90                   	nop
  asm volatile("cld; rep stosb" :
  100b70:	8b 45 0c             	mov    0xc(%ebp),%eax
  100b73:	89 d7                	mov    %edx,%edi
  100b75:	fc                   	cld
  100b76:	f3 aa                	rep stos %al,%es:(%edi)
  100b78:	8b 7d fc             	mov    -0x4(%ebp),%edi
  100b7b:	89 d0                	mov    %edx,%eax
  100b7d:	c9                   	leave
  100b7e:	c3                   	ret
  100b7f:	90                   	nop

00100b80 <memcmp>:

int
memcmp(const void *v1, const void *v2, uint n)
{
  100b80:	55                   	push   %ebp
  100b81:	89 e5                	mov    %esp,%ebp
  100b83:	56                   	push   %esi
  100b84:	8b 75 10             	mov    0x10(%ebp),%esi
  100b87:	8b 45 08             	mov    0x8(%ebp),%eax
  100b8a:	53                   	push   %ebx
  100b8b:	8b 55 0c             	mov    0xc(%ebp),%edx
  const uchar *s1, *s2;

  s1 = v1;
  s2 = v2;
  while(n-- > 0){
  100b8e:	85 f6                	test   %esi,%esi
  100b90:	74 2e                	je     100bc0 <memcmp+0x40>
  100b92:	01 c6                	add    %eax,%esi
  100b94:	eb 14                	jmp    100baa <memcmp+0x2a>
  100b96:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  100b9d:	8d 76 00             	lea    0x0(%esi),%esi
    if(*s1 != *s2)
      return *s1 - *s2;
    s1++, s2++;
  100ba0:	83 c0 01             	add    $0x1,%eax
  100ba3:	83 c2 01             	add    $0x1,%edx
  while(n-- > 0){
  100ba6:	39 f0                	cmp    %esi,%eax
  100ba8:	74 16                	je     100bc0 <memcmp+0x40>
    if(*s1 != *s2)
  100baa:	0f b6 08             	movzbl (%eax),%ecx
  100bad:	0f b6 1a             	movzbl (%edx),%ebx
  100bb0:	38 d9                	cmp    %bl,%cl
  100bb2:	74 ec                	je     100ba0 <memcmp+0x20>
      return *s1 - *s2;
  100bb4:	0f b6 c1             	movzbl %cl,%eax
  100bb7:	29 d8                	sub    %ebx,%eax
  }

  return 0;
}
  100bb9:	5b                   	pop    %ebx
  100bba:	5e                   	pop    %esi
  100bbb:	5d                   	pop    %ebp
  100bbc:	c3                   	ret
  100bbd:	8d 76 00             	lea    0x0(%esi),%esi
  100bc0:	5b                   	pop    %ebx
  return 0;
  100bc1:	31 c0                	xor    %eax,%eax
}
  100bc3:	5e                   	pop    %esi
  100bc4:	5d                   	pop    %ebp
  100bc5:	c3                   	ret
  100bc6:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  100bcd:	8d 76 00             	lea    0x0(%esi),%esi

00100bd0 <memmove>:

void*
memmove(void *dst, const void *src, uint n)
{
  100bd0:	55                   	push   %ebp
  100bd1:	89 e5                	mov    %esp,%ebp
  100bd3:	57                   	push   %edi
  100bd4:	8b 55 08             	mov    0x8(%ebp),%edx
  100bd7:	8b 45 10             	mov    0x10(%ebp),%eax
  100bda:	56                   	push   %esi
  100bdb:	8b 75 0c             	mov    0xc(%ebp),%esi
  const char *s;
  char *d;

  s = src;
  d = dst;
  if(s < d && s + n > d){
  100bde:	39 d6                	cmp    %edx,%esi
  100be0:	73 26                	jae    100c08 <memmove+0x38>
  100be2:	8d 0c 06             	lea    (%esi,%eax,1),%ecx
  100be5:	39 ca                	cmp    %ecx,%edx
  100be7:	73 1f                	jae    100c08 <memmove+0x38>
    s += n;
    d += n;
    while(n-- > 0)
  100be9:	85 c0                	test   %eax,%eax
  100beb:	74 0f                	je     100bfc <memmove+0x2c>
  100bed:	83 e8 01             	sub    $0x1,%eax
      *--d = *--s;
  100bf0:	0f b6 0c 06          	movzbl (%esi,%eax,1),%ecx
  100bf4:	88 0c 02             	mov    %cl,(%edx,%eax,1)
    while(n-- > 0)
  100bf7:	83 e8 01             	sub    $0x1,%eax
  100bfa:	73 f4                	jae    100bf0 <memmove+0x20>
  } else
    while(n-- > 0)
      *d++ = *s++;

  return dst;
}
  100bfc:	5e                   	pop    %esi
  100bfd:	89 d0                	mov    %edx,%eax
  100bff:	5f                   	pop    %edi
  100c00:	5d                   	pop    %ebp
  100c01:	c3                   	ret
  100c02:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
    while(n-- > 0)
  100c08:	8d 0c 06             	lea    (%esi,%eax,1),%ecx
  100c0b:	89 d7                	mov    %edx,%edi
  100c0d:	85 c0                	test   %eax,%eax
  100c0f:	74 eb                	je     100bfc <memmove+0x2c>
  100c11:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
      *d++ = *s++;
  100c18:	a4                   	movsb  %ds:(%esi),%es:(%edi)
    while(n-- > 0)
  100c19:	39 ce                	cmp    %ecx,%esi
  100c1b:	75 fb                	jne    100c18 <memmove+0x48>
}
  100c1d:	5e                   	pop    %esi
  100c1e:	89 d0                	mov    %edx,%eax
  100c20:	5f                   	pop    %edi
  100c21:	5d                   	pop    %ebp
  100c22:	c3                   	ret
  100c23:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  100c2a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi

00100c30 <memcpy>:

// memcpy exists to placate GCC.  Use memmove.
void*
memcpy(void *dst, const void *src, uint n)
{
  return memmove(dst, src, n);
  100c30:	eb 9e                	jmp    100bd0 <memmove>
  100c32:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  100c39:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi

00100c40 <strncmp>:
}

int
strncmp(const char *p, const char *q, uint n)
{
  100c40:	55                   	push   %ebp
  100c41:	89 e5                	mov    %esp,%ebp
  100c43:	53                   	push   %ebx
  100c44:	8b 55 10             	mov    0x10(%ebp),%edx
  100c47:	8b 45 08             	mov    0x8(%ebp),%eax
  100c4a:	8b 4d 0c             	mov    0xc(%ebp),%ecx
  while(n > 0 && *p && *p == *q)
  100c4d:	85 d2                	test   %edx,%edx
  100c4f:	75 16                	jne    100c67 <strncmp+0x27>
  100c51:	eb 2d                	jmp    100c80 <strncmp+0x40>
  100c53:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  100c57:	90                   	nop
  100c58:	3a 19                	cmp    (%ecx),%bl
  100c5a:	75 12                	jne    100c6e <strncmp+0x2e>
    n--, p++, q++;
  100c5c:	83 c0 01             	add    $0x1,%eax
  100c5f:	83 c1 01             	add    $0x1,%ecx
  while(n > 0 && *p && *p == *q)
  100c62:	83 ea 01             	sub    $0x1,%edx
  100c65:	74 19                	je     100c80 <strncmp+0x40>
  100c67:	0f b6 18             	movzbl (%eax),%ebx
  100c6a:	84 db                	test   %bl,%bl
  100c6c:	75 ea                	jne    100c58 <strncmp+0x18>
  if(n == 0)
    return 0;
  return (uchar)*p - (uchar)*q;
  100c6e:	0f b6 00             	movzbl (%eax),%eax
  100c71:	0f b6 11             	movzbl (%ecx),%edx
}
  100c74:	8b 5d fc             	mov    -0x4(%ebp),%ebx
  100c77:	c9                   	leave
  return (uchar)*p - (uchar)*q;
  100c78:	29 d0                	sub    %edx,%eax
}
  100c7a:	c3                   	ret
  100c7b:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  100c7f:	90                   	nop
  100c80:	8b 5d fc             	mov    -0x4(%ebp),%ebx
    return 0;
  100c83:	31 c0                	xor    %eax,%eax
}
  100c85:	c9                   	leave
  100c86:	c3                   	ret
  100c87:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  100c8e:	66 90                	xchg   %ax,%ax

00100c90 <strncpy>:

char*
strncpy(char *s, const char *t, int n)
{
  100c90:	55                   	push   %ebp
  100c91:	89 e5                	mov    %esp,%ebp
  100c93:	57                   	push   %edi
  100c94:	56                   	push   %esi
  100c95:	8b 75 08             	mov    0x8(%ebp),%esi
  100c98:	53                   	push   %ebx
  100c99:	8b 55 10             	mov    0x10(%ebp),%edx
  char *os;

  os = s;
  while(n-- > 0 && (*s++ = *t++) != 0)
  100c9c:	89 f0                	mov    %esi,%eax
  100c9e:	eb 15                	jmp    100cb5 <strncpy+0x25>
  100ca0:	83 45 0c 01          	addl   $0x1,0xc(%ebp)
  100ca4:	8b 7d 0c             	mov    0xc(%ebp),%edi
  100ca7:	83 c0 01             	add    $0x1,%eax
  100caa:	0f b6 4f ff          	movzbl -0x1(%edi),%ecx
  100cae:	88 48 ff             	mov    %cl,-0x1(%eax)
  100cb1:	84 c9                	test   %cl,%cl
  100cb3:	74 13                	je     100cc8 <strncpy+0x38>
  100cb5:	89 d3                	mov    %edx,%ebx
  100cb7:	83 ea 01             	sub    $0x1,%edx
  100cba:	85 db                	test   %ebx,%ebx
  100cbc:	7f e2                	jg     100ca0 <strncpy+0x10>
    ;
  while(n-- > 0)
    *s++ = 0;
  return os;
}
  100cbe:	5b                   	pop    %ebx
  100cbf:	89 f0                	mov    %esi,%eax
  100cc1:	5e                   	pop    %esi
  100cc2:	5f                   	pop    %edi
  100cc3:	5d                   	pop    %ebp
  100cc4:	c3                   	ret
  100cc5:	8d 76 00             	lea    0x0(%esi),%esi
  while(n-- > 0)
  100cc8:	8d 0c 18             	lea    (%eax,%ebx,1),%ecx
  100ccb:	83 e9 01             	sub    $0x1,%ecx
  100cce:	85 d2                	test   %edx,%edx
  100cd0:	74 ec                	je     100cbe <strncpy+0x2e>
  100cd2:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
    *s++ = 0;
  100cd8:	83 c0 01             	add    $0x1,%eax
  100cdb:	89 ca                	mov    %ecx,%edx
  100cdd:	c6 40 ff 00          	movb   $0x0,-0x1(%eax)
  while(n-- > 0)
  100ce1:	29 c2                	sub    %eax,%edx
  100ce3:	85 d2                	test   %edx,%edx
  100ce5:	7f f1                	jg     100cd8 <strncpy+0x48>
}
  100ce7:	5b                   	pop    %ebx
  100ce8:	89 f0                	mov    %esi,%eax
  100cea:	5e                   	pop    %esi
  100ceb:	5f                   	pop    %edi
  100cec:	5d                   	pop    %ebp
  100ced:	c3                   	ret
  100cee:	66 90                	xchg   %ax,%ax

00100cf0 <safestrcpy>:

// Like strncpy but guaranteed to NUL-terminate.
char*
safestrcpy(char *s, const char *t, int n)
{
  100cf0:	55                   	push   %ebp
  100cf1:	89 e5                	mov    %esp,%ebp
  100cf3:	56                   	push   %esi
  100cf4:	8b 55 10             	mov    0x10(%ebp),%edx
  100cf7:	8b 75 08             	mov    0x8(%ebp),%esi
  100cfa:	53                   	push   %ebx
  100cfb:	8b 45 0c             	mov    0xc(%ebp),%eax
  char *os;

  os = s;
  if(n <= 0)
  100cfe:	85 d2                	test   %edx,%edx
  100d00:	7e 25                	jle    100d27 <safestrcpy+0x37>
  100d02:	8d 5c 10 ff          	lea    -0x1(%eax,%edx,1),%ebx
  100d06:	89 f2                	mov    %esi,%edx
  100d08:	eb 16                	jmp    100d20 <safestrcpy+0x30>
  100d0a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
    return os;
  while(--n > 0 && (*s++ = *t++) != 0)
  100d10:	0f b6 08             	movzbl (%eax),%ecx
  100d13:	83 c0 01             	add    $0x1,%eax
  100d16:	83 c2 01             	add    $0x1,%edx
  100d19:	88 4a ff             	mov    %cl,-0x1(%edx)
  100d1c:	84 c9                	test   %cl,%cl
  100d1e:	74 04                	je     100d24 <safestrcpy+0x34>
  100d20:	39 d8                	cmp    %ebx,%eax
  100d22:	75 ec                	jne    100d10 <safestrcpy+0x20>
    ;
  *s = 0;
  100d24:	c6 02 00             	movb   $0x0,(%edx)
  return os;
}
  100d27:	89 f0                	mov    %esi,%eax
  100d29:	5b                   	pop    %ebx
  100d2a:	5e                   	pop    %esi
  100d2b:	5d                   	pop    %ebp
  100d2c:	c3                   	ret
  100d2d:	8d 76 00             	lea    0x0(%esi),%esi

00100d30 <strlen>:

int
strlen(const char *s)
{
  100d30:	55                   	push   %ebp
  int n;

  for(n = 0; s[n]; n++)
  100d31:	31 c0                	xor    %eax,%eax
{
  100d33:	89 e5                	mov    %esp,%ebp
  100d35:	8b 55 08             	mov    0x8(%ebp),%edx
  for(n = 0; s[n]; n++)
  100d38:	80 3a 00             	cmpb   $0x0,(%edx)
  100d3b:	74 0c                	je     100d49 <strlen+0x19>
  100d3d:	8d 76 00             	lea    0x0(%esi),%esi
  100d40:	83 c0 01             	add    $0x1,%eax
  100d43:	80 3c 02 00          	cmpb   $0x0,(%edx,%eax,1)
  100d47:	75 f7                	jne    100d40 <strlen+0x10>
    ;
  return n;
}
  100d49:	5d                   	pop    %ebp
  100d4a:	c3                   	ret
  100d4b:	66 90                	xchg   %ax,%ax
  100d4d:	66 90                	xchg   %ax,%ax
  100d4f:	90                   	nop

00100d50 <mycpu>:

// Must be called with interrupts disabled to avoid the caller being
// rescheduled between reading lapicid and running through the loop.
struct cpu*
mycpu(void)
{
  100d50:	55                   	push   %ebp
  100d51:	89 e5                	mov    %esp,%ebp
  100d53:	53                   	push   %ebx
  100d54:	83 ec 04             	sub    $0x4,%esp
  asm volatile("pushfl; popl %0" : "=r" (eflags));
  100d57:	9c                   	pushf
  100d58:	58                   	pop    %eax
  int apicid, i;
  
  if(readeflags()&FL_IF)
  100d59:	f6 c4 02             	test   $0x2,%ah
  100d5c:	75 44                	jne    100da2 <mycpu+0x52>
    panic("mycpu called with interrupts enabled\n");
  
  apicid = lapicid();
  100d5e:	e8 cd f8 ff ff       	call   100630 <lapicid>
  // APIC IDs are not guaranteed to be contiguous. Maybe we should have
  // a reverse map, or reserve a register to store &cpus[i].
  for (i = 0; i < ncpu; ++i) {
  100d63:	8b 1d 98 24 10 00    	mov    0x102498,%ebx
  100d69:	85 db                	test   %ebx,%ebx
  100d6b:	7e 28                	jle    100d95 <mycpu+0x45>
  100d6d:	31 d2                	xor    %edx,%edx
  100d6f:	eb 0e                	jmp    100d7f <mycpu+0x2f>
  100d71:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  100d78:	83 c2 01             	add    $0x1,%edx
  100d7b:	39 da                	cmp    %ebx,%edx
  100d7d:	74 16                	je     100d95 <mycpu+0x45>
    if (cpus[i].apicid == apicid)
  100d7f:	0f b6 8a 9c 24 10 00 	movzbl 0x10249c(%edx),%ecx
  100d86:	39 c1                	cmp    %eax,%ecx
  100d88:	75 ee                	jne    100d78 <mycpu+0x28>
      return &cpus[i];
  }
  panic("unknown apicid\n");
  100d8a:	8b 5d fc             	mov    -0x4(%ebp),%ebx
      return &cpus[i];
  100d8d:	8d 82 9c 24 10 00    	lea    0x10249c(%edx),%eax
  100d93:	c9                   	leave
  100d94:	c3                   	ret
  panic("unknown apicid\n");
  100d95:	83 ec 0c             	sub    $0xc,%esp
  100d98:	68 b9 1d 10 00       	push   $0x101db9
  100d9d:	e8 8e f4 ff ff       	call   100230 <panic>
    panic("mycpu called with interrupts enabled\n");
  100da2:	83 ec 0c             	sub    $0xc,%esp
  100da5:	68 74 1e 10 00       	push   $0x101e74
  100daa:	e8 81 f4 ff ff       	call   100230 <panic>
  100daf:	90                   	nop

00100db0 <cpuid>:
cpuid() {
  100db0:	55                   	push   %ebp
  100db1:	89 e5                	mov    %esp,%ebp
  100db3:	83 ec 08             	sub    $0x8,%esp
  return mycpu()-cpus;
  100db6:	e8 95 ff ff ff       	call   100d50 <mycpu>
}
  100dbb:	c9                   	leave
  return mycpu()-cpus;
  100dbc:	2d 9c 24 10 00       	sub    $0x10249c,%eax
}
  100dc1:	c3                   	ret
  100dc2:	66 90                	xchg   %ax,%ax
  100dc4:	66 90                	xchg   %ax,%ax
  100dc6:	66 90                	xchg   %ax,%ax
  100dc8:	66 90                	xchg   %ax,%ax
  100dca:	66 90                	xchg   %ax,%ax
  100dcc:	66 90                	xchg   %ax,%ax
  100dce:	66 90                	xchg   %ax,%ax

00100dd0 <getcallerpcs>:
// #include "memlayout.h"

// Record the current call stack in pcs[] by following the %ebp chain.
void
getcallerpcs(void *v, uint pcs[])
{
  100dd0:	55                   	push   %ebp
  100dd1:	89 e5                	mov    %esp,%ebp
  100dd3:	53                   	push   %ebx
  100dd4:	8b 45 08             	mov    0x8(%ebp),%eax
  100dd7:	8b 4d 0c             	mov    0xc(%ebp),%ecx
  int i;

  ebp = (uint*)v - 2;
  for(i = 0; i < 10; i++){
    // if(ebp == 0 || ebp < (uint*)KERNBASE || ebp == (uint*)0xffffffff)
    if(ebp == 0 || ebp == (uint*)0xffffffff)
  100dda:	83 f8 07             	cmp    $0x7,%eax
  100ddd:	74 2e                	je     100e0d <getcallerpcs+0x3d>
  100ddf:	8d 50 f8             	lea    -0x8(%eax),%edx
  for(i = 0; i < 10; i++){
  100de2:	31 c0                	xor    %eax,%eax
  100de4:	eb 12                	jmp    100df8 <getcallerpcs+0x28>
  100de6:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  100ded:	8d 76 00             	lea    0x0(%esi),%esi
    if(ebp == 0 || ebp == (uint*)0xffffffff)
  100df0:	8d 5a ff             	lea    -0x1(%edx),%ebx
  100df3:	83 fb fd             	cmp    $0xfffffffd,%ebx
  100df6:	77 18                	ja     100e10 <getcallerpcs+0x40>
      break;
    pcs[i] = ebp[1];     // saved %eip
  100df8:	8b 5a 04             	mov    0x4(%edx),%ebx
  100dfb:	89 1c 81             	mov    %ebx,(%ecx,%eax,4)
  for(i = 0; i < 10; i++){
  100dfe:	83 c0 01             	add    $0x1,%eax
    ebp = (uint*)ebp[0]; // saved %ebp
  100e01:	8b 12                	mov    (%edx),%edx
  for(i = 0; i < 10; i++){
  100e03:	83 f8 0a             	cmp    $0xa,%eax
  100e06:	75 e8                	jne    100df0 <getcallerpcs+0x20>
  }
  for(; i < 10; i++)
    pcs[i] = 0;
  100e08:	8b 5d fc             	mov    -0x4(%ebp),%ebx
  100e0b:	c9                   	leave
  100e0c:	c3                   	ret
  for(i = 0; i < 10; i++){
  100e0d:	31 c0                	xor    %eax,%eax
  100e0f:	90                   	nop
  100e10:	8d 04 81             	lea    (%ecx,%eax,4),%eax
  100e13:	83 c1 28             	add    $0x28,%ecx
  100e16:	89 ca                	mov    %ecx,%edx
  100e18:	29 c2                	sub    %eax,%edx
  100e1a:	83 e2 04             	and    $0x4,%edx
  100e1d:	74 11                	je     100e30 <getcallerpcs+0x60>
    pcs[i] = 0;
  100e1f:	c7 00 00 00 00 00    	movl   $0x0,(%eax)
  for(; i < 10; i++)
  100e25:	83 c0 04             	add    $0x4,%eax
  100e28:	39 c1                	cmp    %eax,%ecx
  100e2a:	74 dc                	je     100e08 <getcallerpcs+0x38>
  100e2c:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
    pcs[i] = 0;
  100e30:	c7 00 00 00 00 00    	movl   $0x0,(%eax)
  for(; i < 10; i++)
  100e36:	83 c0 08             	add    $0x8,%eax
    pcs[i] = 0;
  100e39:	c7 40 fc 00 00 00 00 	movl   $0x0,-0x4(%eax)
  for(; i < 10; i++)
  100e40:	39 c1                	cmp    %eax,%ecx
  100e42:	75 ec                	jne    100e30 <getcallerpcs+0x60>
  100e44:	eb c2                	jmp    100e08 <getcallerpcs+0x38>

00100e46 <alltraps>:

  # vectors.S sends all traps here.
.globl alltraps
alltraps:
  # Build trap frame.
  pushal
  100e46:	60                   	pusha
  
  # Call trap(tf), where tf=%esp
  pushl %esp
  100e47:	54                   	push   %esp
  call trap
  100e48:	e8 c3 01 00 00       	call   101010 <trap>
  addl $4, %esp
  100e4d:	83 c4 04             	add    $0x4,%esp

00100e50 <trapret>:

  # Return falls through to trapret...
.globl trapret
trapret:
  popal
  100e50:	61                   	popa
  addl $0x8, %esp  # trapno and errcode
  100e51:	83 c4 08             	add    $0x8,%esp
  iret
  100e54:	cf                   	iret
  100e55:	66 90                	xchg   %ax,%ax
  100e57:	66 90                	xchg   %ax,%ax
  100e59:	66 90                	xchg   %ax,%ax
  100e5b:	66 90                	xchg   %ax,%ax
  100e5d:	66 90                	xchg   %ax,%ax
  100e5f:	90                   	nop

00100e60 <tvinit>:
void
tvinit(void)
{
  int i;

  for(i = 0; i < 256; i++)
  100e60:	ba 00 20 10 00       	mov    $0x102000,%edx
  100e65:	b8 e0 24 10 00       	mov    $0x1024e0,%eax
  100e6a:	66 0f ef ff          	pxor   %xmm7,%xmm7
  100e6e:	66 90                	xchg   %ax,%ax
    SETGATE(idt[i], 0, SEG_KCODE<<3, vectors[i], 0);
  100e70:	f3 0f 6f 5a 10       	movdqu 0x10(%edx),%xmm3
  100e75:	f3 0f 6f 0a          	movdqu (%edx),%xmm1
  100e79:	66 0f ef f6          	pxor   %xmm6,%xmm6
  100e7d:	83 c2 20             	add    $0x20,%edx
  100e80:	f3 0f 6f 42 e0       	movdqu -0x20(%edx),%xmm0
  100e85:	83 c0 40             	add    $0x40,%eax
  100e88:	66 0f 6f 2d a0 1f 10 	movdqa 0x101fa0,%xmm5
  100e8f:	00 
  100e90:	66 0f 61 cb          	punpcklwd %xmm3,%xmm1
  100e94:	66 0f db 68 d0       	pand   -0x30(%eax),%xmm5
  100e99:	66 0f 69 c3          	punpckhwd %xmm3,%xmm0
  100e9d:	66 0f 6f d1          	movdqa %xmm1,%xmm2
  100ea1:	66 0f 69 d0          	punpckhwd %xmm0,%xmm2
  100ea5:	66 0f 61 c8          	punpcklwd %xmm0,%xmm1
  100ea9:	f3 0f 6f 42 e0       	movdqu -0x20(%edx),%xmm0
  100eae:	66 0f 61 ca          	punpcklwd %xmm2,%xmm1
  100eb2:	f3 0f 6f 52 f0       	movdqu -0x10(%edx),%xmm2
  100eb7:	66 0f 72 d0 10       	psrld  $0x10,%xmm0
  100ebc:	66 0f 6f d9          	movdqa %xmm1,%xmm3
  100ec0:	66 0f 69 ce          	punpckhwd %xmm6,%xmm1
  100ec4:	66 0f 72 d2 10       	psrld  $0x10,%xmm2
  100ec9:	66 0f 6f e0          	movdqa %xmm0,%xmm4
  100ecd:	66 0f 61 de          	punpcklwd %xmm6,%xmm3
  100ed1:	66 0f 61 c2          	punpcklwd %xmm2,%xmm0
  100ed5:	66 0f 69 e2          	punpckhwd %xmm2,%xmm4
  100ed9:	66 0f 6f d0          	movdqa %xmm0,%xmm2
  100edd:	66 0f 61 c4          	punpcklwd %xmm4,%xmm0
  100ee1:	66 0f 69 d4          	punpckhwd %xmm4,%xmm2
  100ee5:	66 0f 61 c2          	punpcklwd %xmm2,%xmm0
  100ee9:	66 0f 6f d3          	movdqa %xmm3,%xmm2
  100eed:	66 0f 62 df          	punpckldq %xmm7,%xmm3
  100ef1:	66 0f 6f e0          	movdqa %xmm0,%xmm4
  100ef5:	66 0f 6a d7          	punpckhdq %xmm7,%xmm2
  100ef9:	66 0f 69 c6          	punpckhwd %xmm6,%xmm0
  100efd:	66 0f 61 e6          	punpcklwd %xmm6,%xmm4
  100f01:	66 0f eb ea          	por    %xmm2,%xmm5
  100f05:	66 0f 6f f1          	movdqa %xmm1,%xmm6
  100f09:	66 0f 6f d4          	movdqa %xmm4,%xmm2
  100f0d:	66 0f 62 f7          	punpckldq %xmm7,%xmm6
  100f11:	66 0f 6a cf          	punpckhdq %xmm7,%xmm1
  100f15:	66 0f db 2d b0 1f 10 	pand   0x101fb0,%xmm5
  100f1c:	00 
  100f1d:	66 0f 6a d7          	punpckhdq %xmm7,%xmm2
  100f21:	66 0f 62 e7          	punpckldq %xmm7,%xmm4
  100f25:	66 0f 73 f2 30       	psllq  $0x30,%xmm2
  100f2a:	66 0f 73 f4 30       	psllq  $0x30,%xmm4
  100f2f:	66 0f eb ea          	por    %xmm2,%xmm5
  100f33:	66 0f 6f 15 a0 1f 10 	movdqa 0x101fa0,%xmm2
  100f3a:	00 
  100f3b:	66 0f db 50 e0       	pand   -0x20(%eax),%xmm2
  100f40:	66 0f eb 2d c0 1f 10 	por    0x101fc0,%xmm5
  100f47:	00 
  100f48:	66 0f eb d6          	por    %xmm6,%xmm2
  100f4c:	66 0f 6f f0          	movdqa %xmm0,%xmm6
  100f50:	66 0f 6a c7          	punpckhdq %xmm7,%xmm0
  100f54:	66 0f 62 f7          	punpckldq %xmm7,%xmm6
  100f58:	66 0f 73 f0 30       	psllq  $0x30,%xmm0
  100f5d:	0f 29 68 d0          	movaps %xmm5,-0x30(%eax)
  100f61:	66 0f db 15 b0 1f 10 	pand   0x101fb0,%xmm2
  100f68:	00 
  100f69:	66 0f 73 f6 30       	psllq  $0x30,%xmm6
  100f6e:	66 0f eb d6          	por    %xmm6,%xmm2
  100f72:	66 0f 6f 35 a0 1f 10 	movdqa 0x101fa0,%xmm6
  100f79:	00 
  100f7a:	66 0f db 70 f0       	pand   -0x10(%eax),%xmm6
  100f7f:	66 0f eb 15 c0 1f 10 	por    0x101fc0,%xmm2
  100f86:	00 
  100f87:	66 0f eb ce          	por    %xmm6,%xmm1
  100f8b:	66 0f db 0d b0 1f 10 	pand   0x101fb0,%xmm1
  100f92:	00 
  100f93:	0f 29 50 e0          	movaps %xmm2,-0x20(%eax)
  100f97:	66 0f eb c1          	por    %xmm1,%xmm0
  100f9b:	66 0f 6f 0d a0 1f 10 	movdqa 0x101fa0,%xmm1
  100fa2:	00 
  100fa3:	66 0f db 48 c0       	pand   -0x40(%eax),%xmm1
  100fa8:	66 0f eb 05 c0 1f 10 	por    0x101fc0,%xmm0
  100faf:	00 
  100fb0:	66 0f eb d9          	por    %xmm1,%xmm3
  100fb4:	66 0f db 1d b0 1f 10 	pand   0x101fb0,%xmm3
  100fbb:	00 
  100fbc:	0f 29 40 f0          	movaps %xmm0,-0x10(%eax)
  for(i = 0; i < 256; i++)
  100fc0:	66 0f eb e3          	por    %xmm3,%xmm4
  100fc4:	66 0f eb 25 c0 1f 10 	por    0x101fc0,%xmm4
  100fcb:	00 
  100fcc:	0f 29 60 c0          	movaps %xmm4,-0x40(%eax)
  100fd0:	81 fa 00 24 10 00    	cmp    $0x102400,%edx
  100fd6:	0f 85 94 fe ff ff    	jne    100e70 <tvinit+0x10>
}
  100fdc:	c3                   	ret
  100fdd:	8d 76 00             	lea    0x0(%esi),%esi

00100fe0 <idtinit>:

void
idtinit(void)
{
  100fe0:	55                   	push   %ebp
  pd[0] = size-1;
  100fe1:	b8 ff 07 00 00       	mov    $0x7ff,%eax
  100fe6:	89 e5                	mov    %esp,%ebp
  100fe8:	83 ec 10             	sub    $0x10,%esp
  100feb:	66 89 45 fa          	mov    %ax,-0x6(%ebp)
  pd[1] = (uint)p;
  100fef:	b8 e0 24 10 00       	mov    $0x1024e0,%eax
  100ff4:	66 89 45 fc          	mov    %ax,-0x4(%ebp)
  pd[2] = (uint)p >> 16;
  100ff8:	c1 e8 10             	shr    $0x10,%eax
  100ffb:	66 89 45 fe          	mov    %ax,-0x2(%ebp)
  asm volatile("lidt (%0)" : : "r" (pd));
  100fff:	8d 45 fa             	lea    -0x6(%ebp),%eax
  101002:	0f 01 18             	lidtl  (%eax)
  lidt(idt, sizeof(idt));
}
  101005:	c9                   	leave
  101006:	c3                   	ret
  101007:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  10100e:	66 90                	xchg   %ax,%ax

00101010 <trap>:

//PAGEBREAK: 41
void
trap(struct trapframe *tf)
{
  101010:	55                   	push   %ebp
  101011:	89 e5                	mov    %esp,%ebp
  101013:	57                   	push   %edi
  101014:	56                   	push   %esi
  101015:	53                   	push   %ebx
  101016:	83 ec 0c             	sub    $0xc,%esp
  101019:	8b 5d 08             	mov    0x8(%ebp),%ebx
  switch(tf->trapno){
  10101c:	8b 43 20             	mov    0x20(%ebx),%eax
  10101f:	83 e8 20             	sub    $0x20,%eax
  101022:	83 f8 1f             	cmp    $0x1f,%eax
  101025:	77 7c                	ja     1010a3 <trap+0x93>
  101027:	ff 24 85 1c 1f 10 00 	jmp    *0x101f1c(,%eax,4)
  10102e:	66 90                	xchg   %ax,%ax
    mouseintr();
    lapiceoi();
    break;
  case T_IRQ0 + 7:
  case T_IRQ0 + IRQ_SPURIOUS:
    cprintf("cpu%d: spurious interrupt at %x:%x\n",
  101030:	8b 73 28             	mov    0x28(%ebx),%esi
  101033:	0f b7 5b 2c          	movzwl 0x2c(%ebx),%ebx
  101037:	e8 74 fd ff ff       	call   100db0 <cpuid>
  10103c:	56                   	push   %esi
  10103d:	53                   	push   %ebx
  10103e:	50                   	push   %eax
  10103f:	68 9c 1e 10 00       	push   $0x101e9c
  101044:	e8 67 f0 ff ff       	call   1000b0 <cprintf>
            cpuid(), tf->cs, tf->eip);
    lapiceoi();
  101049:	83 c4 10             	add    $0x10,%esp
  default:
    cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
            tf->trapno, cpuid(), tf->eip, rcr2());
    panic("trap");
  }
}
  10104c:	8d 65 f4             	lea    -0xc(%ebp),%esp
  10104f:	5b                   	pop    %ebx
  101050:	5e                   	pop    %esi
  101051:	5f                   	pop    %edi
  101052:	5d                   	pop    %ebp
    lapiceoi();
  101053:	e9 f8 f5 ff ff       	jmp    100650 <lapiceoi>
  101058:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  10105f:	90                   	nop
    mouseintr();
  101060:	e8 4b 0c 00 00       	call   101cb0 <mouseintr>
}
  101065:	8d 65 f4             	lea    -0xc(%ebp),%esp
  101068:	5b                   	pop    %ebx
  101069:	5e                   	pop    %esi
  10106a:	5f                   	pop    %edi
  10106b:	5d                   	pop    %ebp
    lapiceoi();
  10106c:	e9 df f5 ff ff       	jmp    100650 <lapiceoi>
  101071:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
    uartintr();
  101078:	e8 a3 fa ff ff       	call   100b20 <uartintr>
}
  10107d:	8d 65 f4             	lea    -0xc(%ebp),%esp
  101080:	5b                   	pop    %ebx
  101081:	5e                   	pop    %esi
  101082:	5f                   	pop    %edi
  101083:	5d                   	pop    %ebp
    lapiceoi();
  101084:	e9 c7 f5 ff ff       	jmp    100650 <lapiceoi>
  101089:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
    ticks++;
  101090:	83 05 c0 24 10 00 01 	addl   $0x1,0x1024c0
}
  101097:	8d 65 f4             	lea    -0xc(%ebp),%esp
  10109a:	5b                   	pop    %ebx
  10109b:	5e                   	pop    %esi
  10109c:	5f                   	pop    %edi
  10109d:	5d                   	pop    %ebp
    lapiceoi();
  10109e:	e9 ad f5 ff ff       	jmp    100650 <lapiceoi>

static inline uint
rcr2(void)
{
  uint val;
  asm volatile("movl %%cr2,%0" : "=r" (val));
  1010a3:	0f 20 d7             	mov    %cr2,%edi
    cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
  1010a6:	8b 73 28             	mov    0x28(%ebx),%esi
  1010a9:	e8 02 fd ff ff       	call   100db0 <cpuid>
  1010ae:	83 ec 0c             	sub    $0xc,%esp
  1010b1:	57                   	push   %edi
  1010b2:	56                   	push   %esi
  1010b3:	50                   	push   %eax
  1010b4:	ff 73 20             	push   0x20(%ebx)
  1010b7:	68 c0 1e 10 00       	push   $0x101ec0
  1010bc:	e8 ef ef ff ff       	call   1000b0 <cprintf>
    panic("trap");
  1010c1:	83 c4 14             	add    $0x14,%esp
  1010c4:	68 c9 1d 10 00       	push   $0x101dc9
  1010c9:	e8 62 f1 ff ff       	call   100230 <panic>

001010ce <vector0>:
# generated by vectors.pl - do not edit
# handlers
.globl alltraps
.globl vector0
vector0:
  pushl $0
  1010ce:	6a 00                	push   $0x0
  pushl $0
  1010d0:	6a 00                	push   $0x0
  jmp alltraps
  1010d2:	e9 6f fd ff ff       	jmp    100e46 <alltraps>

001010d7 <vector1>:
.globl vector1
vector1:
  pushl $0
  1010d7:	6a 00                	push   $0x0
  pushl $1
  1010d9:	6a 01                	push   $0x1
  jmp alltraps
  1010db:	e9 66 fd ff ff       	jmp    100e46 <alltraps>

001010e0 <vector2>:
.globl vector2
vector2:
  pushl $0
  1010e0:	6a 00                	push   $0x0
  pushl $2
  1010e2:	6a 02                	push   $0x2
  jmp alltraps
  1010e4:	e9 5d fd ff ff       	jmp    100e46 <alltraps>

001010e9 <vector3>:
.globl vector3
vector3:
  pushl $0
  1010e9:	6a 00                	push   $0x0
  pushl $3
  1010eb:	6a 03                	push   $0x3
  jmp alltraps
  1010ed:	e9 54 fd ff ff       	jmp    100e46 <alltraps>

001010f2 <vector4>:
.globl vector4
vector4:
  pushl $0
  1010f2:	6a 00                	push   $0x0
  pushl $4
  1010f4:	6a 04                	push   $0x4
  jmp alltraps
  1010f6:	e9 4b fd ff ff       	jmp    100e46 <alltraps>

001010fb <vector5>:
.globl vector5
vector5:
  pushl $0
  1010fb:	6a 00                	push   $0x0
  pushl $5
  1010fd:	6a 05                	push   $0x5
  jmp alltraps
  1010ff:	e9 42 fd ff ff       	jmp    100e46 <alltraps>

00101104 <vector6>:
.globl vector6
vector6:
  pushl $0
  101104:	6a 00                	push   $0x0
  pushl $6
  101106:	6a 06                	push   $0x6
  jmp alltraps
  101108:	e9 39 fd ff ff       	jmp    100e46 <alltraps>

0010110d <vector7>:
.globl vector7
vector7:
  pushl $0
  10110d:	6a 00                	push   $0x0
  pushl $7
  10110f:	6a 07                	push   $0x7
  jmp alltraps
  101111:	e9 30 fd ff ff       	jmp    100e46 <alltraps>

00101116 <vector8>:
.globl vector8
vector8:
  pushl $8
  101116:	6a 08                	push   $0x8
  jmp alltraps
  101118:	e9 29 fd ff ff       	jmp    100e46 <alltraps>

0010111d <vector9>:
.globl vector9
vector9:
  pushl $0
  10111d:	6a 00                	push   $0x0
  pushl $9
  10111f:	6a 09                	push   $0x9
  jmp alltraps
  101121:	e9 20 fd ff ff       	jmp    100e46 <alltraps>

00101126 <vector10>:
.globl vector10
vector10:
  pushl $10
  101126:	6a 0a                	push   $0xa
  jmp alltraps
  101128:	e9 19 fd ff ff       	jmp    100e46 <alltraps>

0010112d <vector11>:
.globl vector11
vector11:
  pushl $11
  10112d:	6a 0b                	push   $0xb
  jmp alltraps
  10112f:	e9 12 fd ff ff       	jmp    100e46 <alltraps>

00101134 <vector12>:
.globl vector12
vector12:
  pushl $12
  101134:	6a 0c                	push   $0xc
  jmp alltraps
  101136:	e9 0b fd ff ff       	jmp    100e46 <alltraps>

0010113b <vector13>:
.globl vector13
vector13:
  pushl $13
  10113b:	6a 0d                	push   $0xd
  jmp alltraps
  10113d:	e9 04 fd ff ff       	jmp    100e46 <alltraps>

00101142 <vector14>:
.globl vector14
vector14:
  pushl $14
  101142:	6a 0e                	push   $0xe
  jmp alltraps
  101144:	e9 fd fc ff ff       	jmp    100e46 <alltraps>

00101149 <vector15>:
.globl vector15
vector15:
  pushl $0
  101149:	6a 00                	push   $0x0
  pushl $15
  10114b:	6a 0f                	push   $0xf
  jmp alltraps
  10114d:	e9 f4 fc ff ff       	jmp    100e46 <alltraps>

00101152 <vector16>:
.globl vector16
vector16:
  pushl $0
  101152:	6a 00                	push   $0x0
  pushl $16
  101154:	6a 10                	push   $0x10
  jmp alltraps
  101156:	e9 eb fc ff ff       	jmp    100e46 <alltraps>

0010115b <vector17>:
.globl vector17
vector17:
  pushl $17
  10115b:	6a 11                	push   $0x11
  jmp alltraps
  10115d:	e9 e4 fc ff ff       	jmp    100e46 <alltraps>

00101162 <vector18>:
.globl vector18
vector18:
  pushl $0
  101162:	6a 00                	push   $0x0
  pushl $18
  101164:	6a 12                	push   $0x12
  jmp alltraps
  101166:	e9 db fc ff ff       	jmp    100e46 <alltraps>

0010116b <vector19>:
.globl vector19
vector19:
  pushl $0
  10116b:	6a 00                	push   $0x0
  pushl $19
  10116d:	6a 13                	push   $0x13
  jmp alltraps
  10116f:	e9 d2 fc ff ff       	jmp    100e46 <alltraps>

00101174 <vector20>:
.globl vector20
vector20:
  pushl $0
  101174:	6a 00                	push   $0x0
  pushl $20
  101176:	6a 14                	push   $0x14
  jmp alltraps
  101178:	e9 c9 fc ff ff       	jmp    100e46 <alltraps>

0010117d <vector21>:
.globl vector21
vector21:
  pushl $0
  10117d:	6a 00                	push   $0x0
  pushl $21
  10117f:	6a 15                	push   $0x15
  jmp alltraps
  101181:	e9 c0 fc ff ff       	jmp    100e46 <alltraps>

00101186 <vector22>:
.globl vector22
vector22:
  pushl $0
  101186:	6a 00                	push   $0x0
  pushl $22
  101188:	6a 16                	push   $0x16
  jmp alltraps
  10118a:	e9 b7 fc ff ff       	jmp    100e46 <alltraps>

0010118f <vector23>:
.globl vector23
vector23:
  pushl $0
  10118f:	6a 00                	push   $0x0
  pushl $23
  101191:	6a 17                	push   $0x17
  jmp alltraps
  101193:	e9 ae fc ff ff       	jmp    100e46 <alltraps>

00101198 <vector24>:
.globl vector24
vector24:
  pushl $0
  101198:	6a 00                	push   $0x0
  pushl $24
  10119a:	6a 18                	push   $0x18
  jmp alltraps
  10119c:	e9 a5 fc ff ff       	jmp    100e46 <alltraps>

001011a1 <vector25>:
.globl vector25
vector25:
  pushl $0
  1011a1:	6a 00                	push   $0x0
  pushl $25
  1011a3:	6a 19                	push   $0x19
  jmp alltraps
  1011a5:	e9 9c fc ff ff       	jmp    100e46 <alltraps>

001011aa <vector26>:
.globl vector26
vector26:
  pushl $0
  1011aa:	6a 00                	push   $0x0
  pushl $26
  1011ac:	6a 1a                	push   $0x1a
  jmp alltraps
  1011ae:	e9 93 fc ff ff       	jmp    100e46 <alltraps>

001011b3 <vector27>:
.globl vector27
vector27:
  pushl $0
  1011b3:	6a 00                	push   $0x0
  pushl $27
  1011b5:	6a 1b                	push   $0x1b
  jmp alltraps
  1011b7:	e9 8a fc ff ff       	jmp    100e46 <alltraps>

001011bc <vector28>:
.globl vector28
vector28:
  pushl $0
  1011bc:	6a 00                	push   $0x0
  pushl $28
  1011be:	6a 1c                	push   $0x1c
  jmp alltraps
  1011c0:	e9 81 fc ff ff       	jmp    100e46 <alltraps>

001011c5 <vector29>:
.globl vector29
vector29:
  pushl $0
  1011c5:	6a 00                	push   $0x0
  pushl $29
  1011c7:	6a 1d                	push   $0x1d
  jmp alltraps
  1011c9:	e9 78 fc ff ff       	jmp    100e46 <alltraps>

001011ce <vector30>:
.globl vector30
vector30:
  pushl $0
  1011ce:	6a 00                	push   $0x0
  pushl $30
  1011d0:	6a 1e                	push   $0x1e
  jmp alltraps
  1011d2:	e9 6f fc ff ff       	jmp    100e46 <alltraps>

001011d7 <vector31>:
.globl vector31
vector31:
  pushl $0
  1011d7:	6a 00                	push   $0x0
  pushl $31
  1011d9:	6a 1f                	push   $0x1f
  jmp alltraps
  1011db:	e9 66 fc ff ff       	jmp    100e46 <alltraps>

001011e0 <vector32>:
.globl vector32
vector32:
  pushl $0
  1011e0:	6a 00                	push   $0x0
  pushl $32
  1011e2:	6a 20                	push   $0x20
  jmp alltraps
  1011e4:	e9 5d fc ff ff       	jmp    100e46 <alltraps>

001011e9 <vector33>:
.globl vector33
vector33:
  pushl $0
  1011e9:	6a 00                	push   $0x0
  pushl $33
  1011eb:	6a 21                	push   $0x21
  jmp alltraps
  1011ed:	e9 54 fc ff ff       	jmp    100e46 <alltraps>

001011f2 <vector34>:
.globl vector34
vector34:
  pushl $0
  1011f2:	6a 00                	push   $0x0
  pushl $34
  1011f4:	6a 22                	push   $0x22
  jmp alltraps
  1011f6:	e9 4b fc ff ff       	jmp    100e46 <alltraps>

001011fb <vector35>:
.globl vector35
vector35:
  pushl $0
  1011fb:	6a 00                	push   $0x0
  pushl $35
  1011fd:	6a 23                	push   $0x23
  jmp alltraps
  1011ff:	e9 42 fc ff ff       	jmp    100e46 <alltraps>

00101204 <vector36>:
.globl vector36
vector36:
  pushl $0
  101204:	6a 00                	push   $0x0
  pushl $36
  101206:	6a 24                	push   $0x24
  jmp alltraps
  101208:	e9 39 fc ff ff       	jmp    100e46 <alltraps>

0010120d <vector37>:
.globl vector37
vector37:
  pushl $0
  10120d:	6a 00                	push   $0x0
  pushl $37
  10120f:	6a 25                	push   $0x25
  jmp alltraps
  101211:	e9 30 fc ff ff       	jmp    100e46 <alltraps>

00101216 <vector38>:
.globl vector38
vector38:
  pushl $0
  101216:	6a 00                	push   $0x0
  pushl $38
  101218:	6a 26                	push   $0x26
  jmp alltraps
  10121a:	e9 27 fc ff ff       	jmp    100e46 <alltraps>

0010121f <vector39>:
.globl vector39
vector39:
  pushl $0
  10121f:	6a 00                	push   $0x0
  pushl $39
  101221:	6a 27                	push   $0x27
  jmp alltraps
  101223:	e9 1e fc ff ff       	jmp    100e46 <alltraps>

00101228 <vector40>:
.globl vector40
vector40:
  pushl $0
  101228:	6a 00                	push   $0x0
  pushl $40
  10122a:	6a 28                	push   $0x28
  jmp alltraps
  10122c:	e9 15 fc ff ff       	jmp    100e46 <alltraps>

00101231 <vector41>:
.globl vector41
vector41:
  pushl $0
  101231:	6a 00                	push   $0x0
  pushl $41
  101233:	6a 29                	push   $0x29
  jmp alltraps
  101235:	e9 0c fc ff ff       	jmp    100e46 <alltraps>

0010123a <vector42>:
.globl vector42
vector42:
  pushl $0
  10123a:	6a 00                	push   $0x0
  pushl $42
  10123c:	6a 2a                	push   $0x2a
  jmp alltraps
  10123e:	e9 03 fc ff ff       	jmp    100e46 <alltraps>

00101243 <vector43>:
.globl vector43
vector43:
  pushl $0
  101243:	6a 00                	push   $0x0
  pushl $43
  101245:	6a 2b                	push   $0x2b
  jmp alltraps
  101247:	e9 fa fb ff ff       	jmp    100e46 <alltraps>

0010124c <vector44>:
.globl vector44
vector44:
  pushl $0
  10124c:	6a 00                	push   $0x0
  pushl $44
  10124e:	6a 2c                	push   $0x2c
  jmp alltraps
  101250:	e9 f1 fb ff ff       	jmp    100e46 <alltraps>

00101255 <vector45>:
.globl vector45
vector45:
  pushl $0
  101255:	6a 00                	push   $0x0
  pushl $45
  101257:	6a 2d                	push   $0x2d
  jmp alltraps
  101259:	e9 e8 fb ff ff       	jmp    100e46 <alltraps>

0010125e <vector46>:
.globl vector46
vector46:
  pushl $0
  10125e:	6a 00                	push   $0x0
  pushl $46
  101260:	6a 2e                	push   $0x2e
  jmp alltraps
  101262:	e9 df fb ff ff       	jmp    100e46 <alltraps>

00101267 <vector47>:
.globl vector47
vector47:
  pushl $0
  101267:	6a 00                	push   $0x0
  pushl $47
  101269:	6a 2f                	push   $0x2f
  jmp alltraps
  10126b:	e9 d6 fb ff ff       	jmp    100e46 <alltraps>

00101270 <vector48>:
.globl vector48
vector48:
  pushl $0
  101270:	6a 00                	push   $0x0
  pushl $48
  101272:	6a 30                	push   $0x30
  jmp alltraps
  101274:	e9 cd fb ff ff       	jmp    100e46 <alltraps>

00101279 <vector49>:
.globl vector49
vector49:
  pushl $0
  101279:	6a 00                	push   $0x0
  pushl $49
  10127b:	6a 31                	push   $0x31
  jmp alltraps
  10127d:	e9 c4 fb ff ff       	jmp    100e46 <alltraps>

00101282 <vector50>:
.globl vector50
vector50:
  pushl $0
  101282:	6a 00                	push   $0x0
  pushl $50
  101284:	6a 32                	push   $0x32
  jmp alltraps
  101286:	e9 bb fb ff ff       	jmp    100e46 <alltraps>

0010128b <vector51>:
.globl vector51
vector51:
  pushl $0
  10128b:	6a 00                	push   $0x0
  pushl $51
  10128d:	6a 33                	push   $0x33
  jmp alltraps
  10128f:	e9 b2 fb ff ff       	jmp    100e46 <alltraps>

00101294 <vector52>:
.globl vector52
vector52:
  pushl $0
  101294:	6a 00                	push   $0x0
  pushl $52
  101296:	6a 34                	push   $0x34
  jmp alltraps
  101298:	e9 a9 fb ff ff       	jmp    100e46 <alltraps>

0010129d <vector53>:
.globl vector53
vector53:
  pushl $0
  10129d:	6a 00                	push   $0x0
  pushl $53
  10129f:	6a 35                	push   $0x35
  jmp alltraps
  1012a1:	e9 a0 fb ff ff       	jmp    100e46 <alltraps>

001012a6 <vector54>:
.globl vector54
vector54:
  pushl $0
  1012a6:	6a 00                	push   $0x0
  pushl $54
  1012a8:	6a 36                	push   $0x36
  jmp alltraps
  1012aa:	e9 97 fb ff ff       	jmp    100e46 <alltraps>

001012af <vector55>:
.globl vector55
vector55:
  pushl $0
  1012af:	6a 00                	push   $0x0
  pushl $55
  1012b1:	6a 37                	push   $0x37
  jmp alltraps
  1012b3:	e9 8e fb ff ff       	jmp    100e46 <alltraps>

001012b8 <vector56>:
.globl vector56
vector56:
  pushl $0
  1012b8:	6a 00                	push   $0x0
  pushl $56
  1012ba:	6a 38                	push   $0x38
  jmp alltraps
  1012bc:	e9 85 fb ff ff       	jmp    100e46 <alltraps>

001012c1 <vector57>:
.globl vector57
vector57:
  pushl $0
  1012c1:	6a 00                	push   $0x0
  pushl $57
  1012c3:	6a 39                	push   $0x39
  jmp alltraps
  1012c5:	e9 7c fb ff ff       	jmp    100e46 <alltraps>

001012ca <vector58>:
.globl vector58
vector58:
  pushl $0
  1012ca:	6a 00                	push   $0x0
  pushl $58
  1012cc:	6a 3a                	push   $0x3a
  jmp alltraps
  1012ce:	e9 73 fb ff ff       	jmp    100e46 <alltraps>

001012d3 <vector59>:
.globl vector59
vector59:
  pushl $0
  1012d3:	6a 00                	push   $0x0
  pushl $59
  1012d5:	6a 3b                	push   $0x3b
  jmp alltraps
  1012d7:	e9 6a fb ff ff       	jmp    100e46 <alltraps>

001012dc <vector60>:
.globl vector60
vector60:
  pushl $0
  1012dc:	6a 00                	push   $0x0
  pushl $60
  1012de:	6a 3c                	push   $0x3c
  jmp alltraps
  1012e0:	e9 61 fb ff ff       	jmp    100e46 <alltraps>

001012e5 <vector61>:
.globl vector61
vector61:
  pushl $0
  1012e5:	6a 00                	push   $0x0
  pushl $61
  1012e7:	6a 3d                	push   $0x3d
  jmp alltraps
  1012e9:	e9 58 fb ff ff       	jmp    100e46 <alltraps>

001012ee <vector62>:
.globl vector62
vector62:
  pushl $0
  1012ee:	6a 00                	push   $0x0
  pushl $62
  1012f0:	6a 3e                	push   $0x3e
  jmp alltraps
  1012f2:	e9 4f fb ff ff       	jmp    100e46 <alltraps>

001012f7 <vector63>:
.globl vector63
vector63:
  pushl $0
  1012f7:	6a 00                	push   $0x0
  pushl $63
  1012f9:	6a 3f                	push   $0x3f
  jmp alltraps
  1012fb:	e9 46 fb ff ff       	jmp    100e46 <alltraps>

00101300 <vector64>:
.globl vector64
vector64:
  pushl $0
  101300:	6a 00                	push   $0x0
  pushl $64
  101302:	6a 40                	push   $0x40
  jmp alltraps
  101304:	e9 3d fb ff ff       	jmp    100e46 <alltraps>

00101309 <vector65>:
.globl vector65
vector65:
  pushl $0
  101309:	6a 00                	push   $0x0
  pushl $65
  10130b:	6a 41                	push   $0x41
  jmp alltraps
  10130d:	e9 34 fb ff ff       	jmp    100e46 <alltraps>

00101312 <vector66>:
.globl vector66
vector66:
  pushl $0
  101312:	6a 00                	push   $0x0
  pushl $66
  101314:	6a 42                	push   $0x42
  jmp alltraps
  101316:	e9 2b fb ff ff       	jmp    100e46 <alltraps>

0010131b <vector67>:
.globl vector67
vector67:
  pushl $0
  10131b:	6a 00                	push   $0x0
  pushl $67
  10131d:	6a 43                	push   $0x43
  jmp alltraps
  10131f:	e9 22 fb ff ff       	jmp    100e46 <alltraps>

00101324 <vector68>:
.globl vector68
vector68:
  pushl $0
  101324:	6a 00                	push   $0x0
  pushl $68
  101326:	6a 44                	push   $0x44
  jmp alltraps
  101328:	e9 19 fb ff ff       	jmp    100e46 <alltraps>

0010132d <vector69>:
.globl vector69
vector69:
  pushl $0
  10132d:	6a 00                	push   $0x0
  pushl $69
  10132f:	6a 45                	push   $0x45
  jmp alltraps
  101331:	e9 10 fb ff ff       	jmp    100e46 <alltraps>

00101336 <vector70>:
.globl vector70
vector70:
  pushl $0
  101336:	6a 00                	push   $0x0
  pushl $70
  101338:	6a 46                	push   $0x46
  jmp alltraps
  10133a:	e9 07 fb ff ff       	jmp    100e46 <alltraps>

0010133f <vector71>:
.globl vector71
vector71:
  pushl $0
  10133f:	6a 00                	push   $0x0
  pushl $71
  101341:	6a 47                	push   $0x47
  jmp alltraps
  101343:	e9 fe fa ff ff       	jmp    100e46 <alltraps>

00101348 <vector72>:
.globl vector72
vector72:
  pushl $0
  101348:	6a 00                	push   $0x0
  pushl $72
  10134a:	6a 48                	push   $0x48
  jmp alltraps
  10134c:	e9 f5 fa ff ff       	jmp    100e46 <alltraps>

00101351 <vector73>:
.globl vector73
vector73:
  pushl $0
  101351:	6a 00                	push   $0x0
  pushl $73
  101353:	6a 49                	push   $0x49
  jmp alltraps
  101355:	e9 ec fa ff ff       	jmp    100e46 <alltraps>

0010135a <vector74>:
.globl vector74
vector74:
  pushl $0
  10135a:	6a 00                	push   $0x0
  pushl $74
  10135c:	6a 4a                	push   $0x4a
  jmp alltraps
  10135e:	e9 e3 fa ff ff       	jmp    100e46 <alltraps>

00101363 <vector75>:
.globl vector75
vector75:
  pushl $0
  101363:	6a 00                	push   $0x0
  pushl $75
  101365:	6a 4b                	push   $0x4b
  jmp alltraps
  101367:	e9 da fa ff ff       	jmp    100e46 <alltraps>

0010136c <vector76>:
.globl vector76
vector76:
  pushl $0
  10136c:	6a 00                	push   $0x0
  pushl $76
  10136e:	6a 4c                	push   $0x4c
  jmp alltraps
  101370:	e9 d1 fa ff ff       	jmp    100e46 <alltraps>

00101375 <vector77>:
.globl vector77
vector77:
  pushl $0
  101375:	6a 00                	push   $0x0
  pushl $77
  101377:	6a 4d                	push   $0x4d
  jmp alltraps
  101379:	e9 c8 fa ff ff       	jmp    100e46 <alltraps>

0010137e <vector78>:
.globl vector78
vector78:
  pushl $0
  10137e:	6a 00                	push   $0x0
  pushl $78
  101380:	6a 4e                	push   $0x4e
  jmp alltraps
  101382:	e9 bf fa ff ff       	jmp    100e46 <alltraps>

00101387 <vector79>:
.globl vector79
vector79:
  pushl $0
  101387:	6a 00                	push   $0x0
  pushl $79
  101389:	6a 4f                	push   $0x4f
  jmp alltraps
  10138b:	e9 b6 fa ff ff       	jmp    100e46 <alltraps>

00101390 <vector80>:
.globl vector80
vector80:
  pushl $0
  101390:	6a 00                	push   $0x0
  pushl $80
  101392:	6a 50                	push   $0x50
  jmp alltraps
  101394:	e9 ad fa ff ff       	jmp    100e46 <alltraps>

00101399 <vector81>:
.globl vector81
vector81:
  pushl $0
  101399:	6a 00                	push   $0x0
  pushl $81
  10139b:	6a 51                	push   $0x51
  jmp alltraps
  10139d:	e9 a4 fa ff ff       	jmp    100e46 <alltraps>

001013a2 <vector82>:
.globl vector82
vector82:
  pushl $0
  1013a2:	6a 00                	push   $0x0
  pushl $82
  1013a4:	6a 52                	push   $0x52
  jmp alltraps
  1013a6:	e9 9b fa ff ff       	jmp    100e46 <alltraps>

001013ab <vector83>:
.globl vector83
vector83:
  pushl $0
  1013ab:	6a 00                	push   $0x0
  pushl $83
  1013ad:	6a 53                	push   $0x53
  jmp alltraps
  1013af:	e9 92 fa ff ff       	jmp    100e46 <alltraps>

001013b4 <vector84>:
.globl vector84
vector84:
  pushl $0
  1013b4:	6a 00                	push   $0x0
  pushl $84
  1013b6:	6a 54                	push   $0x54
  jmp alltraps
  1013b8:	e9 89 fa ff ff       	jmp    100e46 <alltraps>

001013bd <vector85>:
.globl vector85
vector85:
  pushl $0
  1013bd:	6a 00                	push   $0x0
  pushl $85
  1013bf:	6a 55                	push   $0x55
  jmp alltraps
  1013c1:	e9 80 fa ff ff       	jmp    100e46 <alltraps>

001013c6 <vector86>:
.globl vector86
vector86:
  pushl $0
  1013c6:	6a 00                	push   $0x0
  pushl $86
  1013c8:	6a 56                	push   $0x56
  jmp alltraps
  1013ca:	e9 77 fa ff ff       	jmp    100e46 <alltraps>

001013cf <vector87>:
.globl vector87
vector87:
  pushl $0
  1013cf:	6a 00                	push   $0x0
  pushl $87
  1013d1:	6a 57                	push   $0x57
  jmp alltraps
  1013d3:	e9 6e fa ff ff       	jmp    100e46 <alltraps>

001013d8 <vector88>:
.globl vector88
vector88:
  pushl $0
  1013d8:	6a 00                	push   $0x0
  pushl $88
  1013da:	6a 58                	push   $0x58
  jmp alltraps
  1013dc:	e9 65 fa ff ff       	jmp    100e46 <alltraps>

001013e1 <vector89>:
.globl vector89
vector89:
  pushl $0
  1013e1:	6a 00                	push   $0x0
  pushl $89
  1013e3:	6a 59                	push   $0x59
  jmp alltraps
  1013e5:	e9 5c fa ff ff       	jmp    100e46 <alltraps>

001013ea <vector90>:
.globl vector90
vector90:
  pushl $0
  1013ea:	6a 00                	push   $0x0
  pushl $90
  1013ec:	6a 5a                	push   $0x5a
  jmp alltraps
  1013ee:	e9 53 fa ff ff       	jmp    100e46 <alltraps>

001013f3 <vector91>:
.globl vector91
vector91:
  pushl $0
  1013f3:	6a 00                	push   $0x0
  pushl $91
  1013f5:	6a 5b                	push   $0x5b
  jmp alltraps
  1013f7:	e9 4a fa ff ff       	jmp    100e46 <alltraps>

001013fc <vector92>:
.globl vector92
vector92:
  pushl $0
  1013fc:	6a 00                	push   $0x0
  pushl $92
  1013fe:	6a 5c                	push   $0x5c
  jmp alltraps
  101400:	e9 41 fa ff ff       	jmp    100e46 <alltraps>

00101405 <vector93>:
.globl vector93
vector93:
  pushl $0
  101405:	6a 00                	push   $0x0
  pushl $93
  101407:	6a 5d                	push   $0x5d
  jmp alltraps
  101409:	e9 38 fa ff ff       	jmp    100e46 <alltraps>

0010140e <vector94>:
.globl vector94
vector94:
  pushl $0
  10140e:	6a 00                	push   $0x0
  pushl $94
  101410:	6a 5e                	push   $0x5e
  jmp alltraps
  101412:	e9 2f fa ff ff       	jmp    100e46 <alltraps>

00101417 <vector95>:
.globl vector95
vector95:
  pushl $0
  101417:	6a 00                	push   $0x0
  pushl $95
  101419:	6a 5f                	push   $0x5f
  jmp alltraps
  10141b:	e9 26 fa ff ff       	jmp    100e46 <alltraps>

00101420 <vector96>:
.globl vector96
vector96:
  pushl $0
  101420:	6a 00                	push   $0x0
  pushl $96
  101422:	6a 60                	push   $0x60
  jmp alltraps
  101424:	e9 1d fa ff ff       	jmp    100e46 <alltraps>

00101429 <vector97>:
.globl vector97
vector97:
  pushl $0
  101429:	6a 00                	push   $0x0
  pushl $97
  10142b:	6a 61                	push   $0x61
  jmp alltraps
  10142d:	e9 14 fa ff ff       	jmp    100e46 <alltraps>

00101432 <vector98>:
.globl vector98
vector98:
  pushl $0
  101432:	6a 00                	push   $0x0
  pushl $98
  101434:	6a 62                	push   $0x62
  jmp alltraps
  101436:	e9 0b fa ff ff       	jmp    100e46 <alltraps>

0010143b <vector99>:
.globl vector99
vector99:
  pushl $0
  10143b:	6a 00                	push   $0x0
  pushl $99
  10143d:	6a 63                	push   $0x63
  jmp alltraps
  10143f:	e9 02 fa ff ff       	jmp    100e46 <alltraps>

00101444 <vector100>:
.globl vector100
vector100:
  pushl $0
  101444:	6a 00                	push   $0x0
  pushl $100
  101446:	6a 64                	push   $0x64
  jmp alltraps
  101448:	e9 f9 f9 ff ff       	jmp    100e46 <alltraps>

0010144d <vector101>:
.globl vector101
vector101:
  pushl $0
  10144d:	6a 00                	push   $0x0
  pushl $101
  10144f:	6a 65                	push   $0x65
  jmp alltraps
  101451:	e9 f0 f9 ff ff       	jmp    100e46 <alltraps>

00101456 <vector102>:
.globl vector102
vector102:
  pushl $0
  101456:	6a 00                	push   $0x0
  pushl $102
  101458:	6a 66                	push   $0x66
  jmp alltraps
  10145a:	e9 e7 f9 ff ff       	jmp    100e46 <alltraps>

0010145f <vector103>:
.globl vector103
vector103:
  pushl $0
  10145f:	6a 00                	push   $0x0
  pushl $103
  101461:	6a 67                	push   $0x67
  jmp alltraps
  101463:	e9 de f9 ff ff       	jmp    100e46 <alltraps>

00101468 <vector104>:
.globl vector104
vector104:
  pushl $0
  101468:	6a 00                	push   $0x0
  pushl $104
  10146a:	6a 68                	push   $0x68
  jmp alltraps
  10146c:	e9 d5 f9 ff ff       	jmp    100e46 <alltraps>

00101471 <vector105>:
.globl vector105
vector105:
  pushl $0
  101471:	6a 00                	push   $0x0
  pushl $105
  101473:	6a 69                	push   $0x69
  jmp alltraps
  101475:	e9 cc f9 ff ff       	jmp    100e46 <alltraps>

0010147a <vector106>:
.globl vector106
vector106:
  pushl $0
  10147a:	6a 00                	push   $0x0
  pushl $106
  10147c:	6a 6a                	push   $0x6a
  jmp alltraps
  10147e:	e9 c3 f9 ff ff       	jmp    100e46 <alltraps>

00101483 <vector107>:
.globl vector107
vector107:
  pushl $0
  101483:	6a 00                	push   $0x0
  pushl $107
  101485:	6a 6b                	push   $0x6b
  jmp alltraps
  101487:	e9 ba f9 ff ff       	jmp    100e46 <alltraps>

0010148c <vector108>:
.globl vector108
vector108:
  pushl $0
  10148c:	6a 00                	push   $0x0
  pushl $108
  10148e:	6a 6c                	push   $0x6c
  jmp alltraps
  101490:	e9 b1 f9 ff ff       	jmp    100e46 <alltraps>

00101495 <vector109>:
.globl vector109
vector109:
  pushl $0
  101495:	6a 00                	push   $0x0
  pushl $109
  101497:	6a 6d                	push   $0x6d
  jmp alltraps
  101499:	e9 a8 f9 ff ff       	jmp    100e46 <alltraps>

0010149e <vector110>:
.globl vector110
vector110:
  pushl $0
  10149e:	6a 00                	push   $0x0
  pushl $110
  1014a0:	6a 6e                	push   $0x6e
  jmp alltraps
  1014a2:	e9 9f f9 ff ff       	jmp    100e46 <alltraps>

001014a7 <vector111>:
.globl vector111
vector111:
  pushl $0
  1014a7:	6a 00                	push   $0x0
  pushl $111
  1014a9:	6a 6f                	push   $0x6f
  jmp alltraps
  1014ab:	e9 96 f9 ff ff       	jmp    100e46 <alltraps>

001014b0 <vector112>:
.globl vector112
vector112:
  pushl $0
  1014b0:	6a 00                	push   $0x0
  pushl $112
  1014b2:	6a 70                	push   $0x70
  jmp alltraps
  1014b4:	e9 8d f9 ff ff       	jmp    100e46 <alltraps>

001014b9 <vector113>:
.globl vector113
vector113:
  pushl $0
  1014b9:	6a 00                	push   $0x0
  pushl $113
  1014bb:	6a 71                	push   $0x71
  jmp alltraps
  1014bd:	e9 84 f9 ff ff       	jmp    100e46 <alltraps>

001014c2 <vector114>:
.globl vector114
vector114:
  pushl $0
  1014c2:	6a 00                	push   $0x0
  pushl $114
  1014c4:	6a 72                	push   $0x72
  jmp alltraps
  1014c6:	e9 7b f9 ff ff       	jmp    100e46 <alltraps>

001014cb <vector115>:
.globl vector115
vector115:
  pushl $0
  1014cb:	6a 00                	push   $0x0
  pushl $115
  1014cd:	6a 73                	push   $0x73
  jmp alltraps
  1014cf:	e9 72 f9 ff ff       	jmp    100e46 <alltraps>

001014d4 <vector116>:
.globl vector116
vector116:
  pushl $0
  1014d4:	6a 00                	push   $0x0
  pushl $116
  1014d6:	6a 74                	push   $0x74
  jmp alltraps
  1014d8:	e9 69 f9 ff ff       	jmp    100e46 <alltraps>

001014dd <vector117>:
.globl vector117
vector117:
  pushl $0
  1014dd:	6a 00                	push   $0x0
  pushl $117
  1014df:	6a 75                	push   $0x75
  jmp alltraps
  1014e1:	e9 60 f9 ff ff       	jmp    100e46 <alltraps>

001014e6 <vector118>:
.globl vector118
vector118:
  pushl $0
  1014e6:	6a 00                	push   $0x0
  pushl $118
  1014e8:	6a 76                	push   $0x76
  jmp alltraps
  1014ea:	e9 57 f9 ff ff       	jmp    100e46 <alltraps>

001014ef <vector119>:
.globl vector119
vector119:
  pushl $0
  1014ef:	6a 00                	push   $0x0
  pushl $119
  1014f1:	6a 77                	push   $0x77
  jmp alltraps
  1014f3:	e9 4e f9 ff ff       	jmp    100e46 <alltraps>

001014f8 <vector120>:
.globl vector120
vector120:
  pushl $0
  1014f8:	6a 00                	push   $0x0
  pushl $120
  1014fa:	6a 78                	push   $0x78
  jmp alltraps
  1014fc:	e9 45 f9 ff ff       	jmp    100e46 <alltraps>

00101501 <vector121>:
.globl vector121
vector121:
  pushl $0
  101501:	6a 00                	push   $0x0
  pushl $121
  101503:	6a 79                	push   $0x79
  jmp alltraps
  101505:	e9 3c f9 ff ff       	jmp    100e46 <alltraps>

0010150a <vector122>:
.globl vector122
vector122:
  pushl $0
  10150a:	6a 00                	push   $0x0
  pushl $122
  10150c:	6a 7a                	push   $0x7a
  jmp alltraps
  10150e:	e9 33 f9 ff ff       	jmp    100e46 <alltraps>

00101513 <vector123>:
.globl vector123
vector123:
  pushl $0
  101513:	6a 00                	push   $0x0
  pushl $123
  101515:	6a 7b                	push   $0x7b
  jmp alltraps
  101517:	e9 2a f9 ff ff       	jmp    100e46 <alltraps>

0010151c <vector124>:
.globl vector124
vector124:
  pushl $0
  10151c:	6a 00                	push   $0x0
  pushl $124
  10151e:	6a 7c                	push   $0x7c
  jmp alltraps
  101520:	e9 21 f9 ff ff       	jmp    100e46 <alltraps>

00101525 <vector125>:
.globl vector125
vector125:
  pushl $0
  101525:	6a 00                	push   $0x0
  pushl $125
  101527:	6a 7d                	push   $0x7d
  jmp alltraps
  101529:	e9 18 f9 ff ff       	jmp    100e46 <alltraps>

0010152e <vector126>:
.globl vector126
vector126:
  pushl $0
  10152e:	6a 00                	push   $0x0
  pushl $126
  101530:	6a 7e                	push   $0x7e
  jmp alltraps
  101532:	e9 0f f9 ff ff       	jmp    100e46 <alltraps>

00101537 <vector127>:
.globl vector127
vector127:
  pushl $0
  101537:	6a 00                	push   $0x0
  pushl $127
  101539:	6a 7f                	push   $0x7f
  jmp alltraps
  10153b:	e9 06 f9 ff ff       	jmp    100e46 <alltraps>

00101540 <vector128>:
.globl vector128
vector128:
  pushl $0
  101540:	6a 00                	push   $0x0
  pushl $128
  101542:	68 80 00 00 00       	push   $0x80
  jmp alltraps
  101547:	e9 fa f8 ff ff       	jmp    100e46 <alltraps>

0010154c <vector129>:
.globl vector129
vector129:
  pushl $0
  10154c:	6a 00                	push   $0x0
  pushl $129
  10154e:	68 81 00 00 00       	push   $0x81
  jmp alltraps
  101553:	e9 ee f8 ff ff       	jmp    100e46 <alltraps>

00101558 <vector130>:
.globl vector130
vector130:
  pushl $0
  101558:	6a 00                	push   $0x0
  pushl $130
  10155a:	68 82 00 00 00       	push   $0x82
  jmp alltraps
  10155f:	e9 e2 f8 ff ff       	jmp    100e46 <alltraps>

00101564 <vector131>:
.globl vector131
vector131:
  pushl $0
  101564:	6a 00                	push   $0x0
  pushl $131
  101566:	68 83 00 00 00       	push   $0x83
  jmp alltraps
  10156b:	e9 d6 f8 ff ff       	jmp    100e46 <alltraps>

00101570 <vector132>:
.globl vector132
vector132:
  pushl $0
  101570:	6a 00                	push   $0x0
  pushl $132
  101572:	68 84 00 00 00       	push   $0x84
  jmp alltraps
  101577:	e9 ca f8 ff ff       	jmp    100e46 <alltraps>

0010157c <vector133>:
.globl vector133
vector133:
  pushl $0
  10157c:	6a 00                	push   $0x0
  pushl $133
  10157e:	68 85 00 00 00       	push   $0x85
  jmp alltraps
  101583:	e9 be f8 ff ff       	jmp    100e46 <alltraps>

00101588 <vector134>:
.globl vector134
vector134:
  pushl $0
  101588:	6a 00                	push   $0x0
  pushl $134
  10158a:	68 86 00 00 00       	push   $0x86
  jmp alltraps
  10158f:	e9 b2 f8 ff ff       	jmp    100e46 <alltraps>

00101594 <vector135>:
.globl vector135
vector135:
  pushl $0
  101594:	6a 00                	push   $0x0
  pushl $135
  101596:	68 87 00 00 00       	push   $0x87
  jmp alltraps
  10159b:	e9 a6 f8 ff ff       	jmp    100e46 <alltraps>

001015a0 <vector136>:
.globl vector136
vector136:
  pushl $0
  1015a0:	6a 00                	push   $0x0
  pushl $136
  1015a2:	68 88 00 00 00       	push   $0x88
  jmp alltraps
  1015a7:	e9 9a f8 ff ff       	jmp    100e46 <alltraps>

001015ac <vector137>:
.globl vector137
vector137:
  pushl $0
  1015ac:	6a 00                	push   $0x0
  pushl $137
  1015ae:	68 89 00 00 00       	push   $0x89
  jmp alltraps
  1015b3:	e9 8e f8 ff ff       	jmp    100e46 <alltraps>

001015b8 <vector138>:
.globl vector138
vector138:
  pushl $0
  1015b8:	6a 00                	push   $0x0
  pushl $138
  1015ba:	68 8a 00 00 00       	push   $0x8a
  jmp alltraps
  1015bf:	e9 82 f8 ff ff       	jmp    100e46 <alltraps>

001015c4 <vector139>:
.globl vector139
vector139:
  pushl $0
  1015c4:	6a 00                	push   $0x0
  pushl $139
  1015c6:	68 8b 00 00 00       	push   $0x8b
  jmp alltraps
  1015cb:	e9 76 f8 ff ff       	jmp    100e46 <alltraps>

001015d0 <vector140>:
.globl vector140
vector140:
  pushl $0
  1015d0:	6a 00                	push   $0x0
  pushl $140
  1015d2:	68 8c 00 00 00       	push   $0x8c
  jmp alltraps
  1015d7:	e9 6a f8 ff ff       	jmp    100e46 <alltraps>

001015dc <vector141>:
.globl vector141
vector141:
  pushl $0
  1015dc:	6a 00                	push   $0x0
  pushl $141
  1015de:	68 8d 00 00 00       	push   $0x8d
  jmp alltraps
  1015e3:	e9 5e f8 ff ff       	jmp    100e46 <alltraps>

001015e8 <vector142>:
.globl vector142
vector142:
  pushl $0
  1015e8:	6a 00                	push   $0x0
  pushl $142
  1015ea:	68 8e 00 00 00       	push   $0x8e
  jmp alltraps
  1015ef:	e9 52 f8 ff ff       	jmp    100e46 <alltraps>

001015f4 <vector143>:
.globl vector143
vector143:
  pushl $0
  1015f4:	6a 00                	push   $0x0
  pushl $143
  1015f6:	68 8f 00 00 00       	push   $0x8f
  jmp alltraps
  1015fb:	e9 46 f8 ff ff       	jmp    100e46 <alltraps>

00101600 <vector144>:
.globl vector144
vector144:
  pushl $0
  101600:	6a 00                	push   $0x0
  pushl $144
  101602:	68 90 00 00 00       	push   $0x90
  jmp alltraps
  101607:	e9 3a f8 ff ff       	jmp    100e46 <alltraps>

0010160c <vector145>:
.globl vector145
vector145:
  pushl $0
  10160c:	6a 00                	push   $0x0
  pushl $145
  10160e:	68 91 00 00 00       	push   $0x91
  jmp alltraps
  101613:	e9 2e f8 ff ff       	jmp    100e46 <alltraps>

00101618 <vector146>:
.globl vector146
vector146:
  pushl $0
  101618:	6a 00                	push   $0x0
  pushl $146
  10161a:	68 92 00 00 00       	push   $0x92
  jmp alltraps
  10161f:	e9 22 f8 ff ff       	jmp    100e46 <alltraps>

00101624 <vector147>:
.globl vector147
vector147:
  pushl $0
  101624:	6a 00                	push   $0x0
  pushl $147
  101626:	68 93 00 00 00       	push   $0x93
  jmp alltraps
  10162b:	e9 16 f8 ff ff       	jmp    100e46 <alltraps>

00101630 <vector148>:
.globl vector148
vector148:
  pushl $0
  101630:	6a 00                	push   $0x0
  pushl $148
  101632:	68 94 00 00 00       	push   $0x94
  jmp alltraps
  101637:	e9 0a f8 ff ff       	jmp    100e46 <alltraps>

0010163c <vector149>:
.globl vector149
vector149:
  pushl $0
  10163c:	6a 00                	push   $0x0
  pushl $149
  10163e:	68 95 00 00 00       	push   $0x95
  jmp alltraps
  101643:	e9 fe f7 ff ff       	jmp    100e46 <alltraps>

00101648 <vector150>:
.globl vector150
vector150:
  pushl $0
  101648:	6a 00                	push   $0x0
  pushl $150
  10164a:	68 96 00 00 00       	push   $0x96
  jmp alltraps
  10164f:	e9 f2 f7 ff ff       	jmp    100e46 <alltraps>

00101654 <vector151>:
.globl vector151
vector151:
  pushl $0
  101654:	6a 00                	push   $0x0
  pushl $151
  101656:	68 97 00 00 00       	push   $0x97
  jmp alltraps
  10165b:	e9 e6 f7 ff ff       	jmp    100e46 <alltraps>

00101660 <vector152>:
.globl vector152
vector152:
  pushl $0
  101660:	6a 00                	push   $0x0
  pushl $152
  101662:	68 98 00 00 00       	push   $0x98
  jmp alltraps
  101667:	e9 da f7 ff ff       	jmp    100e46 <alltraps>

0010166c <vector153>:
.globl vector153
vector153:
  pushl $0
  10166c:	6a 00                	push   $0x0
  pushl $153
  10166e:	68 99 00 00 00       	push   $0x99
  jmp alltraps
  101673:	e9 ce f7 ff ff       	jmp    100e46 <alltraps>

00101678 <vector154>:
.globl vector154
vector154:
  pushl $0
  101678:	6a 00                	push   $0x0
  pushl $154
  10167a:	68 9a 00 00 00       	push   $0x9a
  jmp alltraps
  10167f:	e9 c2 f7 ff ff       	jmp    100e46 <alltraps>

00101684 <vector155>:
.globl vector155
vector155:
  pushl $0
  101684:	6a 00                	push   $0x0
  pushl $155
  101686:	68 9b 00 00 00       	push   $0x9b
  jmp alltraps
  10168b:	e9 b6 f7 ff ff       	jmp    100e46 <alltraps>

00101690 <vector156>:
.globl vector156
vector156:
  pushl $0
  101690:	6a 00                	push   $0x0
  pushl $156
  101692:	68 9c 00 00 00       	push   $0x9c
  jmp alltraps
  101697:	e9 aa f7 ff ff       	jmp    100e46 <alltraps>

0010169c <vector157>:
.globl vector157
vector157:
  pushl $0
  10169c:	6a 00                	push   $0x0
  pushl $157
  10169e:	68 9d 00 00 00       	push   $0x9d
  jmp alltraps
  1016a3:	e9 9e f7 ff ff       	jmp    100e46 <alltraps>

001016a8 <vector158>:
.globl vector158
vector158:
  pushl $0
  1016a8:	6a 00                	push   $0x0
  pushl $158
  1016aa:	68 9e 00 00 00       	push   $0x9e
  jmp alltraps
  1016af:	e9 92 f7 ff ff       	jmp    100e46 <alltraps>

001016b4 <vector159>:
.globl vector159
vector159:
  pushl $0
  1016b4:	6a 00                	push   $0x0
  pushl $159
  1016b6:	68 9f 00 00 00       	push   $0x9f
  jmp alltraps
  1016bb:	e9 86 f7 ff ff       	jmp    100e46 <alltraps>

001016c0 <vector160>:
.globl vector160
vector160:
  pushl $0
  1016c0:	6a 00                	push   $0x0
  pushl $160
  1016c2:	68 a0 00 00 00       	push   $0xa0
  jmp alltraps
  1016c7:	e9 7a f7 ff ff       	jmp    100e46 <alltraps>

001016cc <vector161>:
.globl vector161
vector161:
  pushl $0
  1016cc:	6a 00                	push   $0x0
  pushl $161
  1016ce:	68 a1 00 00 00       	push   $0xa1
  jmp alltraps
  1016d3:	e9 6e f7 ff ff       	jmp    100e46 <alltraps>

001016d8 <vector162>:
.globl vector162
vector162:
  pushl $0
  1016d8:	6a 00                	push   $0x0
  pushl $162
  1016da:	68 a2 00 00 00       	push   $0xa2
  jmp alltraps
  1016df:	e9 62 f7 ff ff       	jmp    100e46 <alltraps>

001016e4 <vector163>:
.globl vector163
vector163:
  pushl $0
  1016e4:	6a 00                	push   $0x0
  pushl $163
  1016e6:	68 a3 00 00 00       	push   $0xa3
  jmp alltraps
  1016eb:	e9 56 f7 ff ff       	jmp    100e46 <alltraps>

001016f0 <vector164>:
.globl vector164
vector164:
  pushl $0
  1016f0:	6a 00                	push   $0x0
  pushl $164
  1016f2:	68 a4 00 00 00       	push   $0xa4
  jmp alltraps
  1016f7:	e9 4a f7 ff ff       	jmp    100e46 <alltraps>

001016fc <vector165>:
.globl vector165
vector165:
  pushl $0
  1016fc:	6a 00                	push   $0x0
  pushl $165
  1016fe:	68 a5 00 00 00       	push   $0xa5
  jmp alltraps
  101703:	e9 3e f7 ff ff       	jmp    100e46 <alltraps>

00101708 <vector166>:
.globl vector166
vector166:
  pushl $0
  101708:	6a 00                	push   $0x0
  pushl $166
  10170a:	68 a6 00 00 00       	push   $0xa6
  jmp alltraps
  10170f:	e9 32 f7 ff ff       	jmp    100e46 <alltraps>

00101714 <vector167>:
.globl vector167
vector167:
  pushl $0
  101714:	6a 00                	push   $0x0
  pushl $167
  101716:	68 a7 00 00 00       	push   $0xa7
  jmp alltraps
  10171b:	e9 26 f7 ff ff       	jmp    100e46 <alltraps>

00101720 <vector168>:
.globl vector168
vector168:
  pushl $0
  101720:	6a 00                	push   $0x0
  pushl $168
  101722:	68 a8 00 00 00       	push   $0xa8
  jmp alltraps
  101727:	e9 1a f7 ff ff       	jmp    100e46 <alltraps>

0010172c <vector169>:
.globl vector169
vector169:
  pushl $0
  10172c:	6a 00                	push   $0x0
  pushl $169
  10172e:	68 a9 00 00 00       	push   $0xa9
  jmp alltraps
  101733:	e9 0e f7 ff ff       	jmp    100e46 <alltraps>

00101738 <vector170>:
.globl vector170
vector170:
  pushl $0
  101738:	6a 00                	push   $0x0
  pushl $170
  10173a:	68 aa 00 00 00       	push   $0xaa
  jmp alltraps
  10173f:	e9 02 f7 ff ff       	jmp    100e46 <alltraps>

00101744 <vector171>:
.globl vector171
vector171:
  pushl $0
  101744:	6a 00                	push   $0x0
  pushl $171
  101746:	68 ab 00 00 00       	push   $0xab
  jmp alltraps
  10174b:	e9 f6 f6 ff ff       	jmp    100e46 <alltraps>

00101750 <vector172>:
.globl vector172
vector172:
  pushl $0
  101750:	6a 00                	push   $0x0
  pushl $172
  101752:	68 ac 00 00 00       	push   $0xac
  jmp alltraps
  101757:	e9 ea f6 ff ff       	jmp    100e46 <alltraps>

0010175c <vector173>:
.globl vector173
vector173:
  pushl $0
  10175c:	6a 00                	push   $0x0
  pushl $173
  10175e:	68 ad 00 00 00       	push   $0xad
  jmp alltraps
  101763:	e9 de f6 ff ff       	jmp    100e46 <alltraps>

00101768 <vector174>:
.globl vector174
vector174:
  pushl $0
  101768:	6a 00                	push   $0x0
  pushl $174
  10176a:	68 ae 00 00 00       	push   $0xae
  jmp alltraps
  10176f:	e9 d2 f6 ff ff       	jmp    100e46 <alltraps>

00101774 <vector175>:
.globl vector175
vector175:
  pushl $0
  101774:	6a 00                	push   $0x0
  pushl $175
  101776:	68 af 00 00 00       	push   $0xaf
  jmp alltraps
  10177b:	e9 c6 f6 ff ff       	jmp    100e46 <alltraps>

00101780 <vector176>:
.globl vector176
vector176:
  pushl $0
  101780:	6a 00                	push   $0x0
  pushl $176
  101782:	68 b0 00 00 00       	push   $0xb0
  jmp alltraps
  101787:	e9 ba f6 ff ff       	jmp    100e46 <alltraps>

0010178c <vector177>:
.globl vector177
vector177:
  pushl $0
  10178c:	6a 00                	push   $0x0
  pushl $177
  10178e:	68 b1 00 00 00       	push   $0xb1
  jmp alltraps
  101793:	e9 ae f6 ff ff       	jmp    100e46 <alltraps>

00101798 <vector178>:
.globl vector178
vector178:
  pushl $0
  101798:	6a 00                	push   $0x0
  pushl $178
  10179a:	68 b2 00 00 00       	push   $0xb2
  jmp alltraps
  10179f:	e9 a2 f6 ff ff       	jmp    100e46 <alltraps>

001017a4 <vector179>:
.globl vector179
vector179:
  pushl $0
  1017a4:	6a 00                	push   $0x0
  pushl $179
  1017a6:	68 b3 00 00 00       	push   $0xb3
  jmp alltraps
  1017ab:	e9 96 f6 ff ff       	jmp    100e46 <alltraps>

001017b0 <vector180>:
.globl vector180
vector180:
  pushl $0
  1017b0:	6a 00                	push   $0x0
  pushl $180
  1017b2:	68 b4 00 00 00       	push   $0xb4
  jmp alltraps
  1017b7:	e9 8a f6 ff ff       	jmp    100e46 <alltraps>

001017bc <vector181>:
.globl vector181
vector181:
  pushl $0
  1017bc:	6a 00                	push   $0x0
  pushl $181
  1017be:	68 b5 00 00 00       	push   $0xb5
  jmp alltraps
  1017c3:	e9 7e f6 ff ff       	jmp    100e46 <alltraps>

001017c8 <vector182>:
.globl vector182
vector182:
  pushl $0
  1017c8:	6a 00                	push   $0x0
  pushl $182
  1017ca:	68 b6 00 00 00       	push   $0xb6
  jmp alltraps
  1017cf:	e9 72 f6 ff ff       	jmp    100e46 <alltraps>

001017d4 <vector183>:
.globl vector183
vector183:
  pushl $0
  1017d4:	6a 00                	push   $0x0
  pushl $183
  1017d6:	68 b7 00 00 00       	push   $0xb7
  jmp alltraps
  1017db:	e9 66 f6 ff ff       	jmp    100e46 <alltraps>

001017e0 <vector184>:
.globl vector184
vector184:
  pushl $0
  1017e0:	6a 00                	push   $0x0
  pushl $184
  1017e2:	68 b8 00 00 00       	push   $0xb8
  jmp alltraps
  1017e7:	e9 5a f6 ff ff       	jmp    100e46 <alltraps>

001017ec <vector185>:
.globl vector185
vector185:
  pushl $0
  1017ec:	6a 00                	push   $0x0
  pushl $185
  1017ee:	68 b9 00 00 00       	push   $0xb9
  jmp alltraps
  1017f3:	e9 4e f6 ff ff       	jmp    100e46 <alltraps>

001017f8 <vector186>:
.globl vector186
vector186:
  pushl $0
  1017f8:	6a 00                	push   $0x0
  pushl $186
  1017fa:	68 ba 00 00 00       	push   $0xba
  jmp alltraps
  1017ff:	e9 42 f6 ff ff       	jmp    100e46 <alltraps>

00101804 <vector187>:
.globl vector187
vector187:
  pushl $0
  101804:	6a 00                	push   $0x0
  pushl $187
  101806:	68 bb 00 00 00       	push   $0xbb
  jmp alltraps
  10180b:	e9 36 f6 ff ff       	jmp    100e46 <alltraps>

00101810 <vector188>:
.globl vector188
vector188:
  pushl $0
  101810:	6a 00                	push   $0x0
  pushl $188
  101812:	68 bc 00 00 00       	push   $0xbc
  jmp alltraps
  101817:	e9 2a f6 ff ff       	jmp    100e46 <alltraps>

0010181c <vector189>:
.globl vector189
vector189:
  pushl $0
  10181c:	6a 00                	push   $0x0
  pushl $189
  10181e:	68 bd 00 00 00       	push   $0xbd
  jmp alltraps
  101823:	e9 1e f6 ff ff       	jmp    100e46 <alltraps>

00101828 <vector190>:
.globl vector190
vector190:
  pushl $0
  101828:	6a 00                	push   $0x0
  pushl $190
  10182a:	68 be 00 00 00       	push   $0xbe
  jmp alltraps
  10182f:	e9 12 f6 ff ff       	jmp    100e46 <alltraps>

00101834 <vector191>:
.globl vector191
vector191:
  pushl $0
  101834:	6a 00                	push   $0x0
  pushl $191
  101836:	68 bf 00 00 00       	push   $0xbf
  jmp alltraps
  10183b:	e9 06 f6 ff ff       	jmp    100e46 <alltraps>

00101840 <vector192>:
.globl vector192
vector192:
  pushl $0
  101840:	6a 00                	push   $0x0
  pushl $192
  101842:	68 c0 00 00 00       	push   $0xc0
  jmp alltraps
  101847:	e9 fa f5 ff ff       	jmp    100e46 <alltraps>

0010184c <vector193>:
.globl vector193
vector193:
  pushl $0
  10184c:	6a 00                	push   $0x0
  pushl $193
  10184e:	68 c1 00 00 00       	push   $0xc1
  jmp alltraps
  101853:	e9 ee f5 ff ff       	jmp    100e46 <alltraps>

00101858 <vector194>:
.globl vector194
vector194:
  pushl $0
  101858:	6a 00                	push   $0x0
  pushl $194
  10185a:	68 c2 00 00 00       	push   $0xc2
  jmp alltraps
  10185f:	e9 e2 f5 ff ff       	jmp    100e46 <alltraps>

00101864 <vector195>:
.globl vector195
vector195:
  pushl $0
  101864:	6a 00                	push   $0x0
  pushl $195
  101866:	68 c3 00 00 00       	push   $0xc3
  jmp alltraps
  10186b:	e9 d6 f5 ff ff       	jmp    100e46 <alltraps>

00101870 <vector196>:
.globl vector196
vector196:
  pushl $0
  101870:	6a 00                	push   $0x0
  pushl $196
  101872:	68 c4 00 00 00       	push   $0xc4
  jmp alltraps
  101877:	e9 ca f5 ff ff       	jmp    100e46 <alltraps>

0010187c <vector197>:
.globl vector197
vector197:
  pushl $0
  10187c:	6a 00                	push   $0x0
  pushl $197
  10187e:	68 c5 00 00 00       	push   $0xc5
  jmp alltraps
  101883:	e9 be f5 ff ff       	jmp    100e46 <alltraps>

00101888 <vector198>:
.globl vector198
vector198:
  pushl $0
  101888:	6a 00                	push   $0x0
  pushl $198
  10188a:	68 c6 00 00 00       	push   $0xc6
  jmp alltraps
  10188f:	e9 b2 f5 ff ff       	jmp    100e46 <alltraps>

00101894 <vector199>:
.globl vector199
vector199:
  pushl $0
  101894:	6a 00                	push   $0x0
  pushl $199
  101896:	68 c7 00 00 00       	push   $0xc7
  jmp alltraps
  10189b:	e9 a6 f5 ff ff       	jmp    100e46 <alltraps>

001018a0 <vector200>:
.globl vector200
vector200:
  pushl $0
  1018a0:	6a 00                	push   $0x0
  pushl $200
  1018a2:	68 c8 00 00 00       	push   $0xc8
  jmp alltraps
  1018a7:	e9 9a f5 ff ff       	jmp    100e46 <alltraps>

001018ac <vector201>:
.globl vector201
vector201:
  pushl $0
  1018ac:	6a 00                	push   $0x0
  pushl $201
  1018ae:	68 c9 00 00 00       	push   $0xc9
  jmp alltraps
  1018b3:	e9 8e f5 ff ff       	jmp    100e46 <alltraps>

001018b8 <vector202>:
.globl vector202
vector202:
  pushl $0
  1018b8:	6a 00                	push   $0x0
  pushl $202
  1018ba:	68 ca 00 00 00       	push   $0xca
  jmp alltraps
  1018bf:	e9 82 f5 ff ff       	jmp    100e46 <alltraps>

001018c4 <vector203>:
.globl vector203
vector203:
  pushl $0
  1018c4:	6a 00                	push   $0x0
  pushl $203
  1018c6:	68 cb 00 00 00       	push   $0xcb
  jmp alltraps
  1018cb:	e9 76 f5 ff ff       	jmp    100e46 <alltraps>

001018d0 <vector204>:
.globl vector204
vector204:
  pushl $0
  1018d0:	6a 00                	push   $0x0
  pushl $204
  1018d2:	68 cc 00 00 00       	push   $0xcc
  jmp alltraps
  1018d7:	e9 6a f5 ff ff       	jmp    100e46 <alltraps>

001018dc <vector205>:
.globl vector205
vector205:
  pushl $0
  1018dc:	6a 00                	push   $0x0
  pushl $205
  1018de:	68 cd 00 00 00       	push   $0xcd
  jmp alltraps
  1018e3:	e9 5e f5 ff ff       	jmp    100e46 <alltraps>

001018e8 <vector206>:
.globl vector206
vector206:
  pushl $0
  1018e8:	6a 00                	push   $0x0
  pushl $206
  1018ea:	68 ce 00 00 00       	push   $0xce
  jmp alltraps
  1018ef:	e9 52 f5 ff ff       	jmp    100e46 <alltraps>

001018f4 <vector207>:
.globl vector207
vector207:
  pushl $0
  1018f4:	6a 00                	push   $0x0
  pushl $207
  1018f6:	68 cf 00 00 00       	push   $0xcf
  jmp alltraps
  1018fb:	e9 46 f5 ff ff       	jmp    100e46 <alltraps>

00101900 <vector208>:
.globl vector208
vector208:
  pushl $0
  101900:	6a 00                	push   $0x0
  pushl $208
  101902:	68 d0 00 00 00       	push   $0xd0
  jmp alltraps
  101907:	e9 3a f5 ff ff       	jmp    100e46 <alltraps>

0010190c <vector209>:
.globl vector209
vector209:
  pushl $0
  10190c:	6a 00                	push   $0x0
  pushl $209
  10190e:	68 d1 00 00 00       	push   $0xd1
  jmp alltraps
  101913:	e9 2e f5 ff ff       	jmp    100e46 <alltraps>

00101918 <vector210>:
.globl vector210
vector210:
  pushl $0
  101918:	6a 00                	push   $0x0
  pushl $210
  10191a:	68 d2 00 00 00       	push   $0xd2
  jmp alltraps
  10191f:	e9 22 f5 ff ff       	jmp    100e46 <alltraps>

00101924 <vector211>:
.globl vector211
vector211:
  pushl $0
  101924:	6a 00                	push   $0x0
  pushl $211
  101926:	68 d3 00 00 00       	push   $0xd3
  jmp alltraps
  10192b:	e9 16 f5 ff ff       	jmp    100e46 <alltraps>

00101930 <vector212>:
.globl vector212
vector212:
  pushl $0
  101930:	6a 00                	push   $0x0
  pushl $212
  101932:	68 d4 00 00 00       	push   $0xd4
  jmp alltraps
  101937:	e9 0a f5 ff ff       	jmp    100e46 <alltraps>

0010193c <vector213>:
.globl vector213
vector213:
  pushl $0
  10193c:	6a 00                	push   $0x0
  pushl $213
  10193e:	68 d5 00 00 00       	push   $0xd5
  jmp alltraps
  101943:	e9 fe f4 ff ff       	jmp    100e46 <alltraps>

00101948 <vector214>:
.globl vector214
vector214:
  pushl $0
  101948:	6a 00                	push   $0x0
  pushl $214
  10194a:	68 d6 00 00 00       	push   $0xd6
  jmp alltraps
  10194f:	e9 f2 f4 ff ff       	jmp    100e46 <alltraps>

00101954 <vector215>:
.globl vector215
vector215:
  pushl $0
  101954:	6a 00                	push   $0x0
  pushl $215
  101956:	68 d7 00 00 00       	push   $0xd7
  jmp alltraps
  10195b:	e9 e6 f4 ff ff       	jmp    100e46 <alltraps>

00101960 <vector216>:
.globl vector216
vector216:
  pushl $0
  101960:	6a 00                	push   $0x0
  pushl $216
  101962:	68 d8 00 00 00       	push   $0xd8
  jmp alltraps
  101967:	e9 da f4 ff ff       	jmp    100e46 <alltraps>

0010196c <vector217>:
.globl vector217
vector217:
  pushl $0
  10196c:	6a 00                	push   $0x0
  pushl $217
  10196e:	68 d9 00 00 00       	push   $0xd9
  jmp alltraps
  101973:	e9 ce f4 ff ff       	jmp    100e46 <alltraps>

00101978 <vector218>:
.globl vector218
vector218:
  pushl $0
  101978:	6a 00                	push   $0x0
  pushl $218
  10197a:	68 da 00 00 00       	push   $0xda
  jmp alltraps
  10197f:	e9 c2 f4 ff ff       	jmp    100e46 <alltraps>

00101984 <vector219>:
.globl vector219
vector219:
  pushl $0
  101984:	6a 00                	push   $0x0
  pushl $219
  101986:	68 db 00 00 00       	push   $0xdb
  jmp alltraps
  10198b:	e9 b6 f4 ff ff       	jmp    100e46 <alltraps>

00101990 <vector220>:
.globl vector220
vector220:
  pushl $0
  101990:	6a 00                	push   $0x0
  pushl $220
  101992:	68 dc 00 00 00       	push   $0xdc
  jmp alltraps
  101997:	e9 aa f4 ff ff       	jmp    100e46 <alltraps>

0010199c <vector221>:
.globl vector221
vector221:
  pushl $0
  10199c:	6a 00                	push   $0x0
  pushl $221
  10199e:	68 dd 00 00 00       	push   $0xdd
  jmp alltraps
  1019a3:	e9 9e f4 ff ff       	jmp    100e46 <alltraps>

001019a8 <vector222>:
.globl vector222
vector222:
  pushl $0
  1019a8:	6a 00                	push   $0x0
  pushl $222
  1019aa:	68 de 00 00 00       	push   $0xde
  jmp alltraps
  1019af:	e9 92 f4 ff ff       	jmp    100e46 <alltraps>

001019b4 <vector223>:
.globl vector223
vector223:
  pushl $0
  1019b4:	6a 00                	push   $0x0
  pushl $223
  1019b6:	68 df 00 00 00       	push   $0xdf
  jmp alltraps
  1019bb:	e9 86 f4 ff ff       	jmp    100e46 <alltraps>

001019c0 <vector224>:
.globl vector224
vector224:
  pushl $0
  1019c0:	6a 00                	push   $0x0
  pushl $224
  1019c2:	68 e0 00 00 00       	push   $0xe0
  jmp alltraps
  1019c7:	e9 7a f4 ff ff       	jmp    100e46 <alltraps>

001019cc <vector225>:
.globl vector225
vector225:
  pushl $0
  1019cc:	6a 00                	push   $0x0
  pushl $225
  1019ce:	68 e1 00 00 00       	push   $0xe1
  jmp alltraps
  1019d3:	e9 6e f4 ff ff       	jmp    100e46 <alltraps>

001019d8 <vector226>:
.globl vector226
vector226:
  pushl $0
  1019d8:	6a 00                	push   $0x0
  pushl $226
  1019da:	68 e2 00 00 00       	push   $0xe2
  jmp alltraps
  1019df:	e9 62 f4 ff ff       	jmp    100e46 <alltraps>

001019e4 <vector227>:
.globl vector227
vector227:
  pushl $0
  1019e4:	6a 00                	push   $0x0
  pushl $227
  1019e6:	68 e3 00 00 00       	push   $0xe3
  jmp alltraps
  1019eb:	e9 56 f4 ff ff       	jmp    100e46 <alltraps>

001019f0 <vector228>:
.globl vector228
vector228:
  pushl $0
  1019f0:	6a 00                	push   $0x0
  pushl $228
  1019f2:	68 e4 00 00 00       	push   $0xe4
  jmp alltraps
  1019f7:	e9 4a f4 ff ff       	jmp    100e46 <alltraps>

001019fc <vector229>:
.globl vector229
vector229:
  pushl $0
  1019fc:	6a 00                	push   $0x0
  pushl $229
  1019fe:	68 e5 00 00 00       	push   $0xe5
  jmp alltraps
  101a03:	e9 3e f4 ff ff       	jmp    100e46 <alltraps>

00101a08 <vector230>:
.globl vector230
vector230:
  pushl $0
  101a08:	6a 00                	push   $0x0
  pushl $230
  101a0a:	68 e6 00 00 00       	push   $0xe6
  jmp alltraps
  101a0f:	e9 32 f4 ff ff       	jmp    100e46 <alltraps>

00101a14 <vector231>:
.globl vector231
vector231:
  pushl $0
  101a14:	6a 00                	push   $0x0
  pushl $231
  101a16:	68 e7 00 00 00       	push   $0xe7
  jmp alltraps
  101a1b:	e9 26 f4 ff ff       	jmp    100e46 <alltraps>

00101a20 <vector232>:
.globl vector232
vector232:
  pushl $0
  101a20:	6a 00                	push   $0x0
  pushl $232
  101a22:	68 e8 00 00 00       	push   $0xe8
  jmp alltraps
  101a27:	e9 1a f4 ff ff       	jmp    100e46 <alltraps>

00101a2c <vector233>:
.globl vector233
vector233:
  pushl $0
  101a2c:	6a 00                	push   $0x0
  pushl $233
  101a2e:	68 e9 00 00 00       	push   $0xe9
  jmp alltraps
  101a33:	e9 0e f4 ff ff       	jmp    100e46 <alltraps>

00101a38 <vector234>:
.globl vector234
vector234:
  pushl $0
  101a38:	6a 00                	push   $0x0
  pushl $234
  101a3a:	68 ea 00 00 00       	push   $0xea
  jmp alltraps
  101a3f:	e9 02 f4 ff ff       	jmp    100e46 <alltraps>

00101a44 <vector235>:
.globl vector235
vector235:
  pushl $0
  101a44:	6a 00                	push   $0x0
  pushl $235
  101a46:	68 eb 00 00 00       	push   $0xeb
  jmp alltraps
  101a4b:	e9 f6 f3 ff ff       	jmp    100e46 <alltraps>

00101a50 <vector236>:
.globl vector236
vector236:
  pushl $0
  101a50:	6a 00                	push   $0x0
  pushl $236
  101a52:	68 ec 00 00 00       	push   $0xec
  jmp alltraps
  101a57:	e9 ea f3 ff ff       	jmp    100e46 <alltraps>

00101a5c <vector237>:
.globl vector237
vector237:
  pushl $0
  101a5c:	6a 00                	push   $0x0
  pushl $237
  101a5e:	68 ed 00 00 00       	push   $0xed
  jmp alltraps
  101a63:	e9 de f3 ff ff       	jmp    100e46 <alltraps>

00101a68 <vector238>:
.globl vector238
vector238:
  pushl $0
  101a68:	6a 00                	push   $0x0
  pushl $238
  101a6a:	68 ee 00 00 00       	push   $0xee
  jmp alltraps
  101a6f:	e9 d2 f3 ff ff       	jmp    100e46 <alltraps>

00101a74 <vector239>:
.globl vector239
vector239:
  pushl $0
  101a74:	6a 00                	push   $0x0
  pushl $239
  101a76:	68 ef 00 00 00       	push   $0xef
  jmp alltraps
  101a7b:	e9 c6 f3 ff ff       	jmp    100e46 <alltraps>

00101a80 <vector240>:
.globl vector240
vector240:
  pushl $0
  101a80:	6a 00                	push   $0x0
  pushl $240
  101a82:	68 f0 00 00 00       	push   $0xf0
  jmp alltraps
  101a87:	e9 ba f3 ff ff       	jmp    100e46 <alltraps>

00101a8c <vector241>:
.globl vector241
vector241:
  pushl $0
  101a8c:	6a 00                	push   $0x0
  pushl $241
  101a8e:	68 f1 00 00 00       	push   $0xf1
  jmp alltraps
  101a93:	e9 ae f3 ff ff       	jmp    100e46 <alltraps>

00101a98 <vector242>:
.globl vector242
vector242:
  pushl $0
  101a98:	6a 00                	push   $0x0
  pushl $242
  101a9a:	68 f2 00 00 00       	push   $0xf2
  jmp alltraps
  101a9f:	e9 a2 f3 ff ff       	jmp    100e46 <alltraps>

00101aa4 <vector243>:
.globl vector243
vector243:
  pushl $0
  101aa4:	6a 00                	push   $0x0
  pushl $243
  101aa6:	68 f3 00 00 00       	push   $0xf3
  jmp alltraps
  101aab:	e9 96 f3 ff ff       	jmp    100e46 <alltraps>

00101ab0 <vector244>:
.globl vector244
vector244:
  pushl $0
  101ab0:	6a 00                	push   $0x0
  pushl $244
  101ab2:	68 f4 00 00 00       	push   $0xf4
  jmp alltraps
  101ab7:	e9 8a f3 ff ff       	jmp    100e46 <alltraps>

00101abc <vector245>:
.globl vector245
vector245:
  pushl $0
  101abc:	6a 00                	push   $0x0
  pushl $245
  101abe:	68 f5 00 00 00       	push   $0xf5
  jmp alltraps
  101ac3:	e9 7e f3 ff ff       	jmp    100e46 <alltraps>

00101ac8 <vector246>:
.globl vector246
vector246:
  pushl $0
  101ac8:	6a 00                	push   $0x0
  pushl $246
  101aca:	68 f6 00 00 00       	push   $0xf6
  jmp alltraps
  101acf:	e9 72 f3 ff ff       	jmp    100e46 <alltraps>

00101ad4 <vector247>:
.globl vector247
vector247:
  pushl $0
  101ad4:	6a 00                	push   $0x0
  pushl $247
  101ad6:	68 f7 00 00 00       	push   $0xf7
  jmp alltraps
  101adb:	e9 66 f3 ff ff       	jmp    100e46 <alltraps>

00101ae0 <vector248>:
.globl vector248
vector248:
  pushl $0
  101ae0:	6a 00                	push   $0x0
  pushl $248
  101ae2:	68 f8 00 00 00       	push   $0xf8
  jmp alltraps
  101ae7:	e9 5a f3 ff ff       	jmp    100e46 <alltraps>

00101aec <vector249>:
.globl vector249
vector249:
  pushl $0
  101aec:	6a 00                	push   $0x0
  pushl $249
  101aee:	68 f9 00 00 00       	push   $0xf9
  jmp alltraps
  101af3:	e9 4e f3 ff ff       	jmp    100e46 <alltraps>

00101af8 <vector250>:
.globl vector250
vector250:
  pushl $0
  101af8:	6a 00                	push   $0x0
  pushl $250
  101afa:	68 fa 00 00 00       	push   $0xfa
  jmp alltraps
  101aff:	e9 42 f3 ff ff       	jmp    100e46 <alltraps>

00101b04 <vector251>:
.globl vector251
vector251:
  pushl $0
  101b04:	6a 00                	push   $0x0
  pushl $251
  101b06:	68 fb 00 00 00       	push   $0xfb
  jmp alltraps
  101b0b:	e9 36 f3 ff ff       	jmp    100e46 <alltraps>

00101b10 <vector252>:
.globl vector252
vector252:
  pushl $0
  101b10:	6a 00                	push   $0x0
  pushl $252
  101b12:	68 fc 00 00 00       	push   $0xfc
  jmp alltraps
  101b17:	e9 2a f3 ff ff       	jmp    100e46 <alltraps>

00101b1c <vector253>:
.globl vector253
vector253:
  pushl $0
  101b1c:	6a 00                	push   $0x0
  pushl $253
  101b1e:	68 fd 00 00 00       	push   $0xfd
  jmp alltraps
  101b23:	e9 1e f3 ff ff       	jmp    100e46 <alltraps>

00101b28 <vector254>:
.globl vector254
vector254:
  pushl $0
  101b28:	6a 00                	push   $0x0
  pushl $254
  101b2a:	68 fe 00 00 00       	push   $0xfe
  jmp alltraps
  101b2f:	e9 12 f3 ff ff       	jmp    100e46 <alltraps>

00101b34 <vector255>:
.globl vector255
vector255:
  pushl $0
  101b34:	6a 00                	push   $0x0
  pushl $255
  101b36:	68 ff 00 00 00       	push   $0xff
  jmp alltraps
  101b3b:	e9 06 f3 ff ff       	jmp    100e46 <alltraps>

00101b40 <mousewait_send>:
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  101b40:	ba 64 00 00 00       	mov    $0x64,%edx
  101b45:	ec                   	in     (%dx),%al
mousewait_send(void) 
{
    // Implement your code here
    while(inb(CMD_PORT) & PORT_BSY)
    return;
}
  101b46:	c3                   	ret
  101b47:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  101b4e:	66 90                	xchg   %ax,%ax

00101b50 <mousewait_recv>:
  101b50:	ba 64 00 00 00       	mov    $0x64,%edx
  101b55:	8d 76 00             	lea    0x0(%esi),%esi
  101b58:	ec                   	in     (%dx),%al
void 
mousewait_recv(void) 
{
    // Implement your code here
    uchar r;
    while(((r=inb(CMD_PORT)) & PORT_READ) != PORT_READ);
  101b59:	a8 01                	test   $0x1,%al
  101b5b:	74 fb                	je     101b58 <mousewait_recv+0x8>
    return;
}
  101b5d:	c3                   	ret
  101b5e:	66 90                	xchg   %ax,%ax

00101b60 <mousecmd>:

// Send a one-byte command to the mouse controller, and wait for it
// to be properly acknowledged
void 
mousecmd(uchar cmd) 
{
  101b60:	55                   	push   %ebp
  101b61:	ba 64 00 00 00       	mov    $0x64,%edx
  101b66:	89 e5                	mov    %esp,%ebp
  101b68:	83 ec 08             	sub    $0x8,%esp
  101b6b:	ec                   	in     (%dx),%al
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  101b6c:	b8 d4 ff ff ff       	mov    $0xffffffd4,%eax
  101b71:	ee                   	out    %al,(%dx)
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  101b72:	ec                   	in     (%dx),%al
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  101b73:	ba 60 00 00 00       	mov    $0x60,%edx
  101b78:	0f b6 45 08          	movzbl 0x8(%ebp),%eax
  101b7c:	ee                   	out    %al,(%dx)
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  101b7d:	ba 64 00 00 00       	mov    $0x64,%edx
  101b82:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
  101b88:	ec                   	in     (%dx),%al
    while(((r=inb(CMD_PORT)) & PORT_READ) != PORT_READ);
  101b89:	a8 01                	test   $0x1,%al
  101b8b:	74 fb                	je     101b88 <mousecmd+0x28>
  101b8d:	ba 60 00 00 00       	mov    $0x60,%edx
  101b92:	ec                   	in     (%dx),%al
    mousewait_send();
    outb(DATA_PORT,cmd);
    mousewait_recv(); 

    uchar ack = inb(DATA_PORT); 
    if (ack != ACK) {
  101b93:	3c fa                	cmp    $0xfa,%al
  101b95:	75 09                	jne    101ba0 <mousecmd+0x40>
        cprintf("Mouse command acknowledgment error: %x\n", ack);
    }

    return;
}
  101b97:	c9                   	leave
  101b98:	c3                   	ret
  101b99:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
        cprintf("Mouse command acknowledgment error: %x\n", ack);
  101ba0:	83 ec 08             	sub    $0x8,%esp
  101ba3:	0f b6 c0             	movzbl %al,%eax
  101ba6:	50                   	push   %eax
  101ba7:	68 f4 1e 10 00       	push   $0x101ef4
  101bac:	e8 ff e4 ff ff       	call   1000b0 <cprintf>
  101bb1:	83 c4 10             	add    $0x10,%esp
}
  101bb4:	c9                   	leave
  101bb5:	c3                   	ret
  101bb6:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  101bbd:	8d 76 00             	lea    0x0(%esi),%esi

00101bc0 <mouseinit>:

void
mouseinit(void)
{
  101bc0:	55                   	push   %ebp
  101bc1:	ba 64 00 00 00       	mov    $0x64,%edx
  101bc6:	89 e5                	mov    %esp,%ebp
  101bc8:	56                   	push   %esi
  101bc9:	53                   	push   %ebx
  101bca:	ec                   	in     (%dx),%al
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  101bcb:	b8 a8 ff ff ff       	mov    $0xffffffa8,%eax
  101bd0:	ee                   	out    %al,(%dx)
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  101bd1:	ec                   	in     (%dx),%al
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  101bd2:	b8 20 00 00 00       	mov    $0x20,%eax
  101bd7:	ee                   	out    %al,(%dx)
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  101bd8:	b9 64 00 00 00       	mov    $0x64,%ecx
  101bdd:	8d 76 00             	lea    0x0(%esi),%esi
  101be0:	89 ca                	mov    %ecx,%edx
  101be2:	ec                   	in     (%dx),%al
    while(((r=inb(CMD_PORT)) & PORT_READ) != PORT_READ);
  101be3:	a8 01                	test   $0x1,%al
  101be5:	74 f9                	je     101be0 <mouseinit+0x20>
  101be7:	bb 60 00 00 00       	mov    $0x60,%ebx
  101bec:	89 da                	mov    %ebx,%edx
  101bee:	ec                   	in     (%dx),%al
  101bef:	89 c6                	mov    %eax,%esi
  101bf1:	89 ca                	mov    %ecx,%edx
  101bf3:	ec                   	in     (%dx),%al
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  101bf4:	b8 60 00 00 00       	mov    $0x60,%eax
  101bf9:	ee                   	out    %al,(%dx)
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  101bfa:	ec                   	in     (%dx),%al

    mousewait_send();
    outb(CMD_PORT,COMPAQ_CMD_BYTE);
    mousewait_recv();

    int Compaq_status_byte = inb(DATA_PORT) | 0x02;
  101bfb:	89 f0                	mov    %esi,%eax
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  101bfd:	89 da                	mov    %ebx,%edx
  101bff:	83 c8 02             	or     $0x2,%eax
  101c02:	ee                   	out    %al,(%dx)
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  101c03:	89 ca                	mov    %ecx,%edx
  101c05:	ec                   	in     (%dx),%al
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  101c06:	b8 d4 ff ff ff       	mov    $0xffffffd4,%eax
  101c0b:	ee                   	out    %al,(%dx)
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  101c0c:	ec                   	in     (%dx),%al
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  101c0d:	b8 f6 ff ff ff       	mov    $0xfffffff6,%eax
  101c12:	89 da                	mov    %ebx,%edx
  101c14:	ee                   	out    %al,(%dx)
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  101c15:	ba 64 00 00 00       	mov    $0x64,%edx
  101c1a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
  101c20:	ec                   	in     (%dx),%al
    while(((r=inb(CMD_PORT)) & PORT_READ) != PORT_READ);
  101c21:	a8 01                	test   $0x1,%al
  101c23:	74 fb                	je     101c20 <mouseinit+0x60>
  101c25:	ba 60 00 00 00       	mov    $0x60,%edx
  101c2a:	ec                   	in     (%dx),%al
    if (ack != ACK) {
  101c2b:	3c fa                	cmp    $0xfa,%al
  101c2d:	74 14                	je     101c43 <mouseinit+0x83>
        cprintf("Mouse command acknowledgment error: %x\n", ack);
  101c2f:	83 ec 08             	sub    $0x8,%esp
  101c32:	0f b6 c0             	movzbl %al,%eax
  101c35:	50                   	push   %eax
  101c36:	68 f4 1e 10 00       	push   $0x101ef4
  101c3b:	e8 70 e4 ff ff       	call   1000b0 <cprintf>
  101c40:	83 c4 10             	add    $0x10,%esp
  101c43:	ba 64 00 00 00       	mov    $0x64,%edx
  101c48:	ec                   	in     (%dx),%al
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  101c49:	b8 d4 ff ff ff       	mov    $0xffffffd4,%eax
  101c4e:	ee                   	out    %al,(%dx)
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  101c4f:	ec                   	in     (%dx),%al
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
  101c50:	b8 f4 ff ff ff       	mov    $0xfffffff4,%eax
  101c55:	ba 60 00 00 00       	mov    $0x60,%edx
  101c5a:	ee                   	out    %al,(%dx)
  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  101c5b:	ba 64 00 00 00       	mov    $0x64,%edx
  101c60:	ec                   	in     (%dx),%al
    while(((r=inb(CMD_PORT)) & PORT_READ) != PORT_READ);
  101c61:	a8 01                	test   $0x1,%al
  101c63:	74 fb                	je     101c60 <mouseinit+0xa0>
  101c65:	ba 60 00 00 00       	mov    $0x60,%edx
  101c6a:	ec                   	in     (%dx),%al
    if (ack != ACK) {
  101c6b:	3c fa                	cmp    $0xfa,%al
  101c6d:	74 14                	je     101c83 <mouseinit+0xc3>
        cprintf("Mouse command acknowledgment error: %x\n", ack);
  101c6f:	83 ec 08             	sub    $0x8,%esp
  101c72:	0f b6 c0             	movzbl %al,%eax
  101c75:	50                   	push   %eax
  101c76:	68 f4 1e 10 00       	push   $0x101ef4
  101c7b:	e8 30 e4 ff ff       	call   1000b0 <cprintf>
  101c80:	83 c4 10             	add    $0x10,%esp

    mousecmd(DEF_SETTING);

    mousecmd(ENB_PKT_STR);

    ioapicenable(IRQ_MOUSE,CPU_0);  // enable interrupt at IRQ_12
  101c83:	83 ec 08             	sub    $0x8,%esp
  101c86:	6a 00                	push   $0x0
  101c88:	6a 0c                	push   $0xc
  101c8a:	e8 71 e8 ff ff       	call   100500 <ioapicenable>
    cprintf("Mouse has been initialized\n");
  101c8f:	c7 04 24 ce 1d 10 00 	movl   $0x101dce,(%esp)
  101c96:	e8 15 e4 ff ff       	call   1000b0 <cprintf>
    return;
  101c9b:	83 c4 10             	add    $0x10,%esp
}
  101c9e:	8d 65 f8             	lea    -0x8(%ebp),%esp
  101ca1:	5b                   	pop    %ebx
  101ca2:	5e                   	pop    %esi
  101ca3:	5d                   	pop    %ebp
  101ca4:	c3                   	ret
  101ca5:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  101cac:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi

00101cb0 <mouseintr>:
  101cb0:	ba 64 00 00 00       	mov    $0x64,%edx
  101cb5:	ec                   	in     (%dx),%al
void
mouseintr(void)
{
    uchar Click=0;

    while(inb(CMD_PORT) & PORT_READ){
  101cb6:	a8 01                	test   $0x1,%al
  101cb8:	0f 84 af 00 00 00    	je     101d6d <mouseintr+0xbd>
{
  101cbe:	55                   	push   %ebp
  101cbf:	89 e5                	mov    %esp,%ebp
  101cc1:	57                   	push   %edi
  101cc2:	56                   	push   %esi
  101cc3:	be 60 00 00 00       	mov    $0x60,%esi
  101cc8:	53                   	push   %ebx
  101cc9:	bb 64 00 00 00       	mov    $0x64,%ebx
  101cce:	83 ec 0c             	sub    $0xc,%esp
  101cd1:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  101cd8:	89 da                	mov    %ebx,%edx
  101cda:	ec                   	in     (%dx),%al
    while(((r=inb(CMD_PORT)) & PORT_READ) != PORT_READ);
  101cdb:	a8 01                	test   $0x1,%al
  101cdd:	74 f9                	je     101cd8 <mouseintr+0x28>
  101cdf:	89 f2                	mov    %esi,%edx
  101ce1:	ec                   	in     (%dx),%al
  101ce2:	89 c7                	mov    %eax,%edi
        mousewait_recv();
        Click = inb(DATA_PORT);
        if(Click & LEFT  ) cprintf("LEFT\n");
  101ce4:	a8 01                	test   $0x1,%al
  101ce6:	75 70                	jne    101d58 <mouseintr+0xa8>
        if(Click & RIGHT ) cprintf("RIGHT\n");
  101ce8:	f7 c7 02 00 00 00    	test   $0x2,%edi
  101cee:	75 50                	jne    101d40 <mouseintr+0x90>
        if(Click & MIDDLE) cprintf("MID\n");
  101cf0:	83 e7 04             	and    $0x4,%edi
  101cf3:	75 33                	jne    101d28 <mouseintr+0x78>
  101cf5:	8d 76 00             	lea    0x0(%esi),%esi
  101cf8:	89 da                	mov    %ebx,%edx
  101cfa:	ec                   	in     (%dx),%al
    while(((r=inb(CMD_PORT)) & PORT_READ) != PORT_READ);
  101cfb:	a8 01                	test   $0x1,%al
  101cfd:	74 f9                	je     101cf8 <mouseintr+0x48>
  101cff:	89 f2                	mov    %esi,%edx
  101d01:	ec                   	in     (%dx),%al
  101d02:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
  101d08:	89 da                	mov    %ebx,%edx
  101d0a:	ec                   	in     (%dx),%al
  101d0b:	a8 01                	test   $0x1,%al
  101d0d:	74 f9                	je     101d08 <mouseintr+0x58>
  101d0f:	89 f2                	mov    %esi,%edx
  101d11:	ec                   	in     (%dx),%al
  101d12:	89 da                	mov    %ebx,%edx
  101d14:	ec                   	in     (%dx),%al
    while(inb(CMD_PORT) & PORT_READ){
  101d15:	a8 01                	test   $0x1,%al
  101d17:	75 bf                	jne    101cd8 <mouseintr+0x28>

    // Implement your code here
    
    return;

  101d19:	8d 65 f4             	lea    -0xc(%ebp),%esp
  101d1c:	5b                   	pop    %ebx
  101d1d:	5e                   	pop    %esi
  101d1e:	5f                   	pop    %edi
  101d1f:	5d                   	pop    %ebp
  101d20:	c3                   	ret
  101d21:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
        if(Click & MIDDLE) cprintf("MID\n");
  101d28:	83 ec 0c             	sub    $0xc,%esp
  101d2b:	68 f7 1d 10 00       	push   $0x101df7
  101d30:	e8 7b e3 ff ff       	call   1000b0 <cprintf>
  101d35:	83 c4 10             	add    $0x10,%esp
    while(((r=inb(CMD_PORT)) & PORT_READ) != PORT_READ);
  101d38:	eb be                	jmp    101cf8 <mouseintr+0x48>
  101d3a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
        if(Click & RIGHT ) cprintf("RIGHT\n");
  101d40:	83 ec 0c             	sub    $0xc,%esp
  101d43:	68 f0 1d 10 00       	push   $0x101df0
  101d48:	e8 63 e3 ff ff       	call   1000b0 <cprintf>
  101d4d:	83 c4 10             	add    $0x10,%esp
  101d50:	eb 9e                	jmp    101cf0 <mouseintr+0x40>
  101d52:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
        if(Click & LEFT  ) cprintf("LEFT\n");
  101d58:	83 ec 0c             	sub    $0xc,%esp
  101d5b:	68 ea 1d 10 00       	push   $0x101dea
  101d60:	e8 4b e3 ff ff       	call   1000b0 <cprintf>
  101d65:	83 c4 10             	add    $0x10,%esp
  101d68:	e9 7b ff ff ff       	jmp    101ce8 <mouseintr+0x38>
  101d6d:	c3                   	ret
