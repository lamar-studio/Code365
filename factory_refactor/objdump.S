
./test.out:     file format elf64-x86-64


Disassembly of section .init:

0000000000401010 <_init>:
  401010:	48 83 ec 08          	sub    $0x8,%rsp
  401014:	48 8b 05 dd 3f 20 00 	mov    0x203fdd(%rip),%rax        # 604ff8 <_DYNAMIC+0x1f0>
  40101b:	48 85 c0             	test   %rax,%rax
  40101e:	74 05                	je     401025 <_init+0x15>
  401020:	e8 5b 02 00 00       	callq  401280 <_Unwind_Resume@plt+0x10>
  401025:	48 83 c4 08          	add    $0x8,%rsp
  401029:	c3                   	retq   

Disassembly of section .plt:

0000000000401030 <_ZSt20__throw_length_errorPKc@plt-0x10>:
  401030:	ff 35 d2 3f 20 00    	pushq  0x203fd2(%rip)        # 605008 <_GLOBAL_OFFSET_TABLE_+0x8>
  401036:	ff 25 d4 3f 20 00    	jmpq   *0x203fd4(%rip)        # 605010 <_GLOBAL_OFFSET_TABLE_+0x10>
  40103c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000401040 <_ZSt20__throw_length_errorPKc@plt>:
  401040:	ff 25 d2 3f 20 00    	jmpq   *0x203fd2(%rip)        # 605018 <_GLOBAL_OFFSET_TABLE_+0x18>
  401046:	68 00 00 00 00       	pushq  $0x0
  40104b:	e9 e0 ff ff ff       	jmpq   401030 <_init+0x20>

0000000000401050 <printf@plt>:
  401050:	ff 25 ca 3f 20 00    	jmpq   *0x203fca(%rip)        # 605020 <_GLOBAL_OFFSET_TABLE_+0x20>
  401056:	68 01 00 00 00       	pushq  $0x1
  40105b:	e9 d0 ff ff ff       	jmpq   401030 <_init+0x20>

0000000000401060 <_ZNSolsEi@plt>:
  401060:	ff 25 c2 3f 20 00    	jmpq   *0x203fc2(%rip)        # 605028 <_GLOBAL_OFFSET_TABLE_+0x28>
  401066:	68 02 00 00 00       	pushq  $0x2
  40106b:	e9 c0 ff ff ff       	jmpq   401030 <_init+0x20>

0000000000401070 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6substrEmm@plt>:
  401070:	ff 25 ba 3f 20 00    	jmpq   *0x203fba(%rip)        # 605030 <_GLOBAL_OFFSET_TABLE_+0x30>
  401076:	68 03 00 00 00       	pushq  $0x3
  40107b:	e9 b0 ff ff ff       	jmpq   401030 <_init+0x20>

0000000000401080 <puts@plt>:
  401080:	ff 25 b2 3f 20 00    	jmpq   *0x203fb2(%rip)        # 605038 <_GLOBAL_OFFSET_TABLE_+0x38>
  401086:	68 04 00 00 00       	pushq  $0x4
  40108b:	e9 a0 ff ff ff       	jmpq   401030 <_init+0x20>

0000000000401090 <_ZdlPv@plt>:
  401090:	ff 25 aa 3f 20 00    	jmpq   *0x203faa(%rip)        # 605040 <_GLOBAL_OFFSET_TABLE_+0x40>
  401096:	68 05 00 00 00       	pushq  $0x5
  40109b:	e9 90 ff ff ff       	jmpq   401030 <_init+0x20>

00000000004010a0 <__cxa_rethrow@plt>:
  4010a0:	ff 25 a2 3f 20 00    	jmpq   *0x203fa2(%rip)        # 605048 <_GLOBAL_OFFSET_TABLE_+0x48>
  4010a6:	68 06 00 00 00       	pushq  $0x6
  4010ab:	e9 80 ff ff ff       	jmpq   401030 <_init+0x20>

00000000004010b0 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6lengthEv@plt>:
  4010b0:	ff 25 9a 3f 20 00    	jmpq   *0x203f9a(%rip)        # 605050 <_GLOBAL_OFFSET_TABLE_+0x50>
  4010b6:	68 07 00 00 00       	pushq  $0x7
  4010bb:	e9 70 ff ff ff       	jmpq   401030 <_init+0x20>

00000000004010c0 <_ZNSt8ios_base4InitC1Ev@plt>:
  4010c0:	ff 25 92 3f 20 00    	jmpq   *0x203f92(%rip)        # 605058 <_GLOBAL_OFFSET_TABLE_+0x58>
  4010c6:	68 08 00 00 00       	pushq  $0x8
  4010cb:	e9 60 ff ff ff       	jmpq   401030 <_init+0x20>

00000000004010d0 <__libc_start_main@plt>:
  4010d0:	ff 25 8a 3f 20 00    	jmpq   *0x203f8a(%rip)        # 605060 <_GLOBAL_OFFSET_TABLE_+0x60>
  4010d6:	68 09 00 00 00       	pushq  $0x9
  4010db:	e9 50 ff ff ff       	jmpq   401030 <_init+0x20>

00000000004010e0 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE13find_first_ofEcm@plt>:
  4010e0:	ff 25 82 3f 20 00    	jmpq   *0x203f82(%rip)        # 605068 <_GLOBAL_OFFSET_TABLE_+0x68>
  4010e6:	68 0a 00 00 00       	pushq  $0xa
  4010eb:	e9 40 ff ff ff       	jmpq   401030 <_init+0x20>

00000000004010f0 <__cxa_atexit@plt>:
  4010f0:	ff 25 7a 3f 20 00    	jmpq   *0x203f7a(%rip)        # 605070 <_GLOBAL_OFFSET_TABLE_+0x70>
  4010f6:	68 0b 00 00 00       	pushq  $0xb
  4010fb:	e9 30 ff ff ff       	jmpq   401030 <_init+0x20>

0000000000401100 <_ZNSt8ios_base4InitD1Ev@plt>:
  401100:	ff 25 72 3f 20 00    	jmpq   *0x203f72(%rip)        # 605078 <_GLOBAL_OFFSET_TABLE_+0x78>
  401106:	68 0c 00 00 00       	pushq  $0xc
  40110b:	e9 20 ff ff ff       	jmpq   401030 <_init+0x20>

0000000000401110 <atof@plt>:
  401110:	ff 25 6a 3f 20 00    	jmpq   *0x203f6a(%rip)        # 605080 <_GLOBAL_OFFSET_TABLE_+0x80>
  401116:	68 0d 00 00 00       	pushq  $0xd
  40111b:	e9 10 ff ff ff       	jmpq   401030 <_init+0x20>

0000000000401120 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>:
  401120:	ff 25 62 3f 20 00    	jmpq   *0x203f62(%rip)        # 605088 <_GLOBAL_OFFSET_TABLE_+0x88>
  401126:	68 0e 00 00 00       	pushq  $0xe
  40112b:	e9 00 ff ff ff       	jmpq   401030 <_init+0x20>

0000000000401130 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1Ev@plt>:
  401130:	ff 25 5a 3f 20 00    	jmpq   *0x203f5a(%rip)        # 605090 <_GLOBAL_OFFSET_TABLE_+0x90>
  401136:	68 0f 00 00 00       	pushq  $0xf
  40113b:	e9 f0 fe ff ff       	jmpq   401030 <_init+0x20>

0000000000401140 <strlen@plt>:
  401140:	ff 25 52 3f 20 00    	jmpq   *0x203f52(%rip)        # 605098 <_GLOBAL_OFFSET_TABLE_+0x98>
  401146:	68 10 00 00 00       	pushq  $0x10
  40114b:	e9 e0 fe ff ff       	jmpq   401030 <_init+0x20>

0000000000401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>:
  401150:	ff 25 4a 3f 20 00    	jmpq   *0x203f4a(%rip)        # 6050a0 <_GLOBAL_OFFSET_TABLE_+0xa0>
  401156:	68 11 00 00 00       	pushq  $0x11
  40115b:	e9 d0 fe ff ff       	jmpq   401030 <_init+0x20>

0000000000401160 <_ZNSolsEb@plt>:
  401160:	ff 25 42 3f 20 00    	jmpq   *0x203f42(%rip)        # 6050a8 <_GLOBAL_OFFSET_TABLE_+0xa8>
  401166:	68 12 00 00 00       	pushq  $0x12
  40116b:	e9 c0 fe ff ff       	jmpq   401030 <_init+0x20>

0000000000401170 <atoi@plt>:
  401170:	ff 25 3a 3f 20 00    	jmpq   *0x203f3a(%rip)        # 6050b0 <_GLOBAL_OFFSET_TABLE_+0xb0>
  401176:	68 13 00 00 00       	pushq  $0x13
  40117b:	e9 b0 fe ff ff       	jmpq   401030 <_init+0x20>

0000000000401180 <__stack_chk_fail@plt>:
  401180:	ff 25 32 3f 20 00    	jmpq   *0x203f32(%rip)        # 6050b8 <_GLOBAL_OFFSET_TABLE_+0xb8>
  401186:	68 14 00 00 00       	pushq  $0x14
  40118b:	e9 a0 fe ff ff       	jmpq   401030 <_init+0x20>

0000000000401190 <strcmp@plt>:
  401190:	ff 25 2a 3f 20 00    	jmpq   *0x203f2a(%rip)        # 6050c0 <_GLOBAL_OFFSET_TABLE_+0xc0>
  401196:	68 15 00 00 00       	pushq  $0x15
  40119b:	e9 90 fe ff ff       	jmpq   401030 <_init+0x20>

00000000004011a0 <_ZNSaIcED1Ev@plt>:
  4011a0:	ff 25 22 3f 20 00    	jmpq   *0x203f22(%rip)        # 6050c8 <_GLOBAL_OFFSET_TABLE_+0xc8>
  4011a6:	68 16 00 00 00       	pushq  $0x16
  4011ab:	e9 80 fe ff ff       	jmpq   401030 <_init+0x20>

00000000004011b0 <_ZNSolsEPFRSoS_E@plt>:
  4011b0:	ff 25 1a 3f 20 00    	jmpq   *0x203f1a(%rip)        # 6050d0 <_GLOBAL_OFFSET_TABLE_+0xd0>
  4011b6:	68 17 00 00 00       	pushq  $0x17
  4011bb:	e9 70 fe ff ff       	jmpq   401030 <_init+0x20>

00000000004011c0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEaSERKS4_@plt>:
  4011c0:	ff 25 12 3f 20 00    	jmpq   *0x203f12(%rip)        # 6050d8 <_GLOBAL_OFFSET_TABLE_+0xd8>
  4011c6:	68 18 00 00 00       	pushq  $0x18
  4011cb:	e9 60 fe ff ff       	jmpq   401030 <_init+0x20>

00000000004011d0 <__cxa_end_catch@plt>:
  4011d0:	ff 25 0a 3f 20 00    	jmpq   *0x203f0a(%rip)        # 6050e0 <_GLOBAL_OFFSET_TABLE_+0xe0>
  4011d6:	68 19 00 00 00       	pushq  $0x19
  4011db:	e9 50 fe ff ff       	jmpq   401030 <_init+0x20>

00000000004011e0 <_ZSt17__throw_bad_allocv@plt>:
  4011e0:	ff 25 02 3f 20 00    	jmpq   *0x203f02(%rip)        # 6050e8 <_GLOBAL_OFFSET_TABLE_+0xe8>
  4011e6:	68 1a 00 00 00       	pushq  $0x1a
  4011eb:	e9 40 fe ff ff       	jmpq   401030 <_init+0x20>

00000000004011f0 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE5c_strEv@plt>:
  4011f0:	ff 25 fa 3e 20 00    	jmpq   *0x203efa(%rip)        # 6050f0 <_GLOBAL_OFFSET_TABLE_+0xf0>
  4011f6:	68 1b 00 00 00       	pushq  $0x1b
  4011fb:	e9 30 fe ff ff       	jmpq   401030 <_init+0x20>

0000000000401200 <_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@plt>:
  401200:	ff 25 f2 3e 20 00    	jmpq   *0x203ef2(%rip)        # 6050f8 <_GLOBAL_OFFSET_TABLE_+0xf8>
  401206:	68 1c 00 00 00       	pushq  $0x1c
  40120b:	e9 20 fe ff ff       	jmpq   401030 <_init+0x20>

0000000000401210 <__cxa_begin_catch@plt>:
  401210:	ff 25 ea 3e 20 00    	jmpq   *0x203eea(%rip)        # 605100 <_GLOBAL_OFFSET_TABLE_+0x100>
  401216:	68 1d 00 00 00       	pushq  $0x1d
  40121b:	e9 10 fe ff ff       	jmpq   401030 <_init+0x20>

0000000000401220 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1ERKS4_@plt>:
  401220:	ff 25 e2 3e 20 00    	jmpq   *0x203ee2(%rip)        # 605108 <_GLOBAL_OFFSET_TABLE_+0x108>
  401226:	68 1e 00 00 00       	pushq  $0x1e
  40122b:	e9 00 fe ff ff       	jmpq   401030 <_init+0x20>

0000000000401230 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1EPKcRKS3_@plt>:
  401230:	ff 25 da 3e 20 00    	jmpq   *0x203eda(%rip)        # 605110 <_GLOBAL_OFFSET_TABLE_+0x110>
  401236:	68 1f 00 00 00       	pushq  $0x1f
  40123b:	e9 f0 fd ff ff       	jmpq   401030 <_init+0x20>

0000000000401240 <_ZNSaIcEC1Ev@plt>:
  401240:	ff 25 d2 3e 20 00    	jmpq   *0x203ed2(%rip)        # 605118 <_GLOBAL_OFFSET_TABLE_+0x118>
  401246:	68 20 00 00 00       	pushq  $0x20
  40124b:	e9 e0 fd ff ff       	jmpq   401030 <_init+0x20>

0000000000401250 <__gxx_personality_v0@plt>:
  401250:	ff 25 ca 3e 20 00    	jmpq   *0x203eca(%rip)        # 605120 <_GLOBAL_OFFSET_TABLE_+0x120>
  401256:	68 21 00 00 00       	pushq  $0x21
  40125b:	e9 d0 fd ff ff       	jmpq   401030 <_init+0x20>

0000000000401260 <_Znwm@plt>:
  401260:	ff 25 c2 3e 20 00    	jmpq   *0x203ec2(%rip)        # 605128 <_GLOBAL_OFFSET_TABLE_+0x128>
  401266:	68 22 00 00 00       	pushq  $0x22
  40126b:	e9 c0 fd ff ff       	jmpq   401030 <_init+0x20>

0000000000401270 <_Unwind_Resume@plt>:
  401270:	ff 25 ba 3e 20 00    	jmpq   *0x203eba(%rip)        # 605130 <_GLOBAL_OFFSET_TABLE_+0x130>
  401276:	68 23 00 00 00       	pushq  $0x23
  40127b:	e9 b0 fd ff ff       	jmpq   401030 <_init+0x20>

Disassembly of section .plt.got:

0000000000401280 <.plt.got>:
  401280:	ff 25 72 3d 20 00    	jmpq   *0x203d72(%rip)        # 604ff8 <_DYNAMIC+0x1f0>
  401286:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

0000000000401290 <_start>:
  401290:	31 ed                	xor    %ebp,%ebp
  401292:	49 89 d1             	mov    %rdx,%r9
  401295:	5e                   	pop    %rsi
  401296:	48 89 e2             	mov    %rsp,%rdx
  401299:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  40129d:	50                   	push   %rax
  40129e:	54                   	push   %rsp
  40129f:	49 c7 c0 70 2f 40 00 	mov    $0x402f70,%r8
  4012a6:	48 c7 c1 00 2f 40 00 	mov    $0x402f00,%rcx
  4012ad:	48 c7 c7 7b 2d 40 00 	mov    $0x402d7b,%rdi
  4012b4:	e8 17 fe ff ff       	callq  4010d0 <__libc_start_main@plt>
  4012b9:	f4                   	hlt    
  4012ba:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000004012c0 <deregister_tm_clones>:
  4012c0:	b8 4f 51 60 00       	mov    $0x60514f,%eax
  4012c5:	55                   	push   %rbp
  4012c6:	48 2d 48 51 60 00    	sub    $0x605148,%rax
  4012cc:	48 83 f8 0e          	cmp    $0xe,%rax
  4012d0:	48 89 e5             	mov    %rsp,%rbp
  4012d3:	76 1b                	jbe    4012f0 <deregister_tm_clones+0x30>
  4012d5:	b8 00 00 00 00       	mov    $0x0,%eax
  4012da:	48 85 c0             	test   %rax,%rax
  4012dd:	74 11                	je     4012f0 <deregister_tm_clones+0x30>
  4012df:	5d                   	pop    %rbp
  4012e0:	bf 48 51 60 00       	mov    $0x605148,%edi
  4012e5:	ff e0                	jmpq   *%rax
  4012e7:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
  4012ee:	00 00 
  4012f0:	5d                   	pop    %rbp
  4012f1:	c3                   	retq   
  4012f2:	0f 1f 40 00          	nopl   0x0(%rax)
  4012f6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  4012fd:	00 00 00 

0000000000401300 <register_tm_clones>:
  401300:	be 48 51 60 00       	mov    $0x605148,%esi
  401305:	55                   	push   %rbp
  401306:	48 81 ee 48 51 60 00 	sub    $0x605148,%rsi
  40130d:	48 c1 fe 03          	sar    $0x3,%rsi
  401311:	48 89 e5             	mov    %rsp,%rbp
  401314:	48 89 f0             	mov    %rsi,%rax
  401317:	48 c1 e8 3f          	shr    $0x3f,%rax
  40131b:	48 01 c6             	add    %rax,%rsi
  40131e:	48 d1 fe             	sar    %rsi
  401321:	74 15                	je     401338 <register_tm_clones+0x38>
  401323:	b8 00 00 00 00       	mov    $0x0,%eax
  401328:	48 85 c0             	test   %rax,%rax
  40132b:	74 0b                	je     401338 <register_tm_clones+0x38>
  40132d:	5d                   	pop    %rbp
  40132e:	bf 48 51 60 00       	mov    $0x605148,%edi
  401333:	ff e0                	jmpq   *%rax
  401335:	0f 1f 00             	nopl   (%rax)
  401338:	5d                   	pop    %rbp
  401339:	c3                   	retq   
  40133a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000401340 <__do_global_dtors_aux>:
  401340:	80 3d 29 3f 20 00 00 	cmpb   $0x0,0x203f29(%rip)        # 605270 <completed.7594>
  401347:	75 11                	jne    40135a <__do_global_dtors_aux+0x1a>
  401349:	55                   	push   %rbp
  40134a:	48 89 e5             	mov    %rsp,%rbp
  40134d:	e8 6e ff ff ff       	callq  4012c0 <deregister_tm_clones>
  401352:	5d                   	pop    %rbp
  401353:	c6 05 16 3f 20 00 01 	movb   $0x1,0x203f16(%rip)        # 605270 <completed.7594>
  40135a:	f3 c3                	repz retq 
  40135c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000401360 <frame_dummy>:
  401360:	bf 00 4e 60 00       	mov    $0x604e00,%edi
  401365:	48 83 3f 00          	cmpq   $0x0,(%rdi)
  401369:	75 05                	jne    401370 <frame_dummy+0x10>
  40136b:	eb 93                	jmp    401300 <register_tm_clones>
  40136d:	0f 1f 00             	nopl   (%rax)
  401370:	b8 00 00 00 00       	mov    $0x0,%eax
  401375:	48 85 c0             	test   %rax,%rax
  401378:	74 f1                	je     40136b <frame_dummy+0xb>
  40137a:	55                   	push   %rbp
  40137b:	48 89 e5             	mov    %rsp,%rbp
  40137e:	ff d0                	callq  *%rax
  401380:	5d                   	pop    %rbp
  401381:	e9 7a ff ff ff       	jmpq   401300 <register_tm_clones>

0000000000401386 <_ZN8HWConfigC1Ev>:
#include <string>
#include <map>

#include "HWConfig.h"

HWConfig::HWConfig()
  401386:	55                   	push   %rbp
  401387:	48 89 e5             	mov    %rsp,%rbp
  40138a:	53                   	push   %rbx
  40138b:	48 83 ec 18          	sub    $0x18,%rsp
  40138f:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  401393:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401397:	48 89 c7             	mov    %rax,%rdi
  40139a:	e8 6f 0a 00 00       	callq  401e0e <_ZNSt6vectorI10tagCfgInfoSaIS0_EEC1Ev>
{
    loadHwConfigInfo();
  40139f:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4013a3:	48 89 c7             	mov    %rax,%rdi
  4013a6:	e8 23 00 00 00       	callq  4013ce <_ZN8HWConfig16loadHwConfigInfoEv>
}
  4013ab:	eb 1a                	jmp    4013c7 <_ZN8HWConfigC1Ev+0x41>
  4013ad:	48 89 c3             	mov    %rax,%rbx
#include <string>
#include <map>

#include "HWConfig.h"

HWConfig::HWConfig()
  4013b0:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4013b4:	48 89 c7             	mov    %rax,%rdi
  4013b7:	e8 6e 0a 00 00       	callq  401e2a <_ZNSt6vectorI10tagCfgInfoSaIS0_EED1Ev>
  4013bc:	48 89 d8             	mov    %rbx,%rax
  4013bf:	48 89 c7             	mov    %rax,%rdi
  4013c2:	e8 a9 fe ff ff       	callq  401270 <_Unwind_Resume@plt>
{
    loadHwConfigInfo();
}
  4013c7:	48 83 c4 18          	add    $0x18,%rsp
  4013cb:	5b                   	pop    %rbx
  4013cc:	5d                   	pop    %rbp
  4013cd:	c3                   	retq   

00000000004013ce <_ZN8HWConfig16loadHwConfigInfoEv>:

void HWConfig::loadHwConfigInfo()
{
  4013ce:	55                   	push   %rbp
  4013cf:	48 89 e5             	mov    %rsp,%rbp
  4013d2:	53                   	push   %rbx
  4013d3:	48 81 ec e8 01 00 00 	sub    $0x1e8,%rsp
  4013da:	48 89 bd 18 fe ff ff 	mov    %rdi,-0x1e8(%rbp)
  4013e1:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  4013e8:	00 00 
  4013ea:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
  4013ee:	31 c0                	xor    %eax,%eax
    const char *info = "CPU:i5-6200U;MEM:8;USB:2/6;SSD:128+256;MEMC:32;HDD:500;WIFI:true;FAN:1;VGA:1;LCD:1920X1080";
  4013f0:	48 c7 85 38 fe ff ff 	movq   $0x402f88,-0x1c8(%rbp)
  4013f7:	88 2f 40 00 
    string str;
  4013fb:	48 8d 85 40 fe ff ff 	lea    -0x1c0(%rbp),%rax
  401402:	48 89 c7             	mov    %rax,%rdi
  401405:	e8 26 fd ff ff       	callq  401130 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1Ev@plt>
    char buf[128] = {0};
  40140a:	48 8d 95 60 ff ff ff 	lea    -0xa0(%rbp),%rdx
  401411:	b8 00 00 00 00       	mov    $0x0,%eax
  401416:	b9 10 00 00 00       	mov    $0x10,%ecx
  40141b:	48 89 d7             	mov    %rdx,%rdi
  40141e:	f3 48 ab             	rep stos %rax,%es:(%rdi)
    int cnt = 0;
  401421:	c7 85 28 fe ff ff 00 	movl   $0x0,-0x1d8(%rbp)
  401428:	00 00 00 

    for (unsigned int i = 0; i < strlen(info); i++) {
  40142b:	c7 85 2c fe ff ff 00 	movl   $0x0,-0x1d4(%rbp)
  401432:	00 00 00 
  401435:	8b 9d 2c fe ff ff    	mov    -0x1d4(%rbp),%ebx
  40143b:	48 8b 85 38 fe ff ff 	mov    -0x1c8(%rbp),%rax
  401442:	48 89 c7             	mov    %rax,%rdi
  401445:	e8 f6 fc ff ff       	callq  401140 <strlen@plt>
  40144a:	48 39 c3             	cmp    %rax,%rbx
  40144d:	0f 83 ea 01 00 00    	jae    40163d <_ZN8HWConfig16loadHwConfigInfoEv+0x26f>
        CFG_INFO_T entry;
  401453:	48 8d 85 20 ff ff ff 	lea    -0xe0(%rbp),%rax
  40145a:	48 89 c7             	mov    %rax,%rdi
  40145d:	e8 b2 08 00 00       	callq  401d14 <_ZN10tagCfgInfoC1Ev>

        if (info[i] != ';') {
  401462:	8b 95 2c fe ff ff    	mov    -0x1d4(%rbp),%edx
  401468:	48 8b 85 38 fe ff ff 	mov    -0x1c8(%rbp),%rax
  40146f:	48 01 d0             	add    %rdx,%rax
  401472:	0f b6 00             	movzbl (%rax),%eax
  401475:	3c 3b                	cmp    $0x3b,%al
  401477:	74 30                	je     4014a9 <_ZN8HWConfig16loadHwConfigInfoEv+0xdb>
            buf[cnt++]  = info[i];
  401479:	8b 85 28 fe ff ff    	mov    -0x1d8(%rbp),%eax
  40147f:	8d 50 01             	lea    0x1(%rax),%edx
  401482:	89 95 28 fe ff ff    	mov    %edx,-0x1d8(%rbp)
  401488:	8b 8d 2c fe ff ff    	mov    -0x1d4(%rbp),%ecx
  40148e:	48 8b 95 38 fe ff ff 	mov    -0x1c8(%rbp),%rdx
  401495:	48 01 ca             	add    %rcx,%rdx
  401498:	0f b6 12             	movzbl (%rdx),%edx
  40149b:	48 98                	cltq   
  40149d:	88 94 05 60 ff ff ff 	mov    %dl,-0xa0(%rbp,%rax,1)
  4014a4:	e9 79 01 00 00       	jmpq   401622 <_ZN8HWConfig16loadHwConfigInfoEv+0x254>
        } else {
            buf[cnt] = '\0';
  4014a9:	8b 85 28 fe ff ff    	mov    -0x1d8(%rbp),%eax
  4014af:	48 98                	cltq   
  4014b1:	c6 84 05 60 ff ff ff 	movb   $0x0,-0xa0(%rbp,%rax,1)
  4014b8:	00 
            str = string(buf);
  4014b9:	48 8d 85 27 fe ff ff 	lea    -0x1d9(%rbp),%rax
  4014c0:	48 89 c7             	mov    %rax,%rdi
  4014c3:	e8 78 fd ff ff       	callq  401240 <_ZNSaIcEC1Ev@plt>
  4014c8:	48 8d 95 27 fe ff ff 	lea    -0x1d9(%rbp),%rdx
  4014cf:	48 8d 8d 60 ff ff ff 	lea    -0xa0(%rbp),%rcx
  4014d6:	48 8d 85 60 fe ff ff 	lea    -0x1a0(%rbp),%rax
  4014dd:	48 89 ce             	mov    %rcx,%rsi
  4014e0:	48 89 c7             	mov    %rax,%rdi
  4014e3:	e8 48 fd ff ff       	callq  401230 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1EPKcRKS3_@plt>
  4014e8:	48 8d 95 60 fe ff ff 	lea    -0x1a0(%rbp),%rdx
  4014ef:	48 8d 85 40 fe ff ff 	lea    -0x1c0(%rbp),%rax
  4014f6:	48 89 d6             	mov    %rdx,%rsi
  4014f9:	48 89 c7             	mov    %rax,%rdi
  4014fc:	e8 bf fc ff ff       	callq  4011c0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEaSERKS4_@plt>
  401501:	48 8d 85 60 fe ff ff 	lea    -0x1a0(%rbp),%rax
  401508:	48 89 c7             	mov    %rax,%rdi
  40150b:	e8 40 fc ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
  401510:	48 8d 85 27 fe ff ff 	lea    -0x1d9(%rbp),%rax
  401517:	48 89 c7             	mov    %rax,%rdi
  40151a:	e8 81 fc ff ff       	callq  4011a0 <_ZNSaIcED1Ev@plt>

            int idx = str.find_first_of(':');
  40151f:	48 8d 85 40 fe ff ff 	lea    -0x1c0(%rbp),%rax
  401526:	ba 00 00 00 00       	mov    $0x0,%edx
  40152b:	be 3a 00 00 00       	mov    $0x3a,%esi
  401530:	48 89 c7             	mov    %rax,%rdi
  401533:	e8 a8 fb ff ff       	callq  4010e0 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE13find_first_ofEcm@plt>
  401538:	89 85 30 fe ff ff    	mov    %eax,-0x1d0(%rbp)
            entry.key = str.substr(0, idx);
  40153e:	8b 85 30 fe ff ff    	mov    -0x1d0(%rbp),%eax
  401544:	48 63 d0             	movslq %eax,%rdx
  401547:	48 8d 85 80 fe ff ff 	lea    -0x180(%rbp),%rax
  40154e:	48 8d b5 40 fe ff ff 	lea    -0x1c0(%rbp),%rsi
  401555:	48 89 d1             	mov    %rdx,%rcx
  401558:	ba 00 00 00 00       	mov    $0x0,%edx
  40155d:	48 89 c7             	mov    %rax,%rdi
  401560:	e8 0b fb ff ff       	callq  401070 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6substrEmm@plt>
  401565:	48 8d 95 80 fe ff ff 	lea    -0x180(%rbp),%rdx
  40156c:	48 8d 85 20 ff ff ff 	lea    -0xe0(%rbp),%rax
  401573:	48 89 d6             	mov    %rdx,%rsi
  401576:	48 89 c7             	mov    %rax,%rdi
  401579:	e8 42 fc ff ff       	callq  4011c0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEaSERKS4_@plt>
  40157e:	48 8d 85 80 fe ff ff 	lea    -0x180(%rbp),%rax
  401585:	48 89 c7             	mov    %rax,%rdi
  401588:	e8 c3 fb ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
            entry.val = str.substr(idx+1, str.length()-idx-1);
  40158d:	48 8d 85 40 fe ff ff 	lea    -0x1c0(%rbp),%rax
  401594:	48 89 c7             	mov    %rax,%rdi
  401597:	e8 14 fb ff ff       	callq  4010b0 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6lengthEv@plt>
  40159c:	48 89 c2             	mov    %rax,%rdx
  40159f:	8b 85 30 fe ff ff    	mov    -0x1d0(%rbp),%eax
  4015a5:	48 98                	cltq   
  4015a7:	48 29 c2             	sub    %rax,%rdx
  4015aa:	48 89 d0             	mov    %rdx,%rax
  4015ad:	48 8d 48 ff          	lea    -0x1(%rax),%rcx
  4015b1:	8b 85 30 fe ff ff    	mov    -0x1d0(%rbp),%eax
  4015b7:	83 c0 01             	add    $0x1,%eax
  4015ba:	48 63 d0             	movslq %eax,%rdx
  4015bd:	48 8d 85 a0 fe ff ff 	lea    -0x160(%rbp),%rax
  4015c4:	48 8d b5 40 fe ff ff 	lea    -0x1c0(%rbp),%rsi
  4015cb:	48 89 c7             	mov    %rax,%rdi
  4015ce:	e8 9d fa ff ff       	callq  401070 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6substrEmm@plt>
  4015d3:	48 8d 85 a0 fe ff ff 	lea    -0x160(%rbp),%rax
  4015da:	48 8d 95 20 ff ff ff 	lea    -0xe0(%rbp),%rdx
  4015e1:	48 83 c2 20          	add    $0x20,%rdx
  4015e5:	48 89 c6             	mov    %rax,%rsi
  4015e8:	48 89 d7             	mov    %rdx,%rdi
  4015eb:	e8 d0 fb ff ff       	callq  4011c0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEaSERKS4_@plt>
  4015f0:	48 8d 85 a0 fe ff ff 	lea    -0x160(%rbp),%rax
  4015f7:	48 89 c7             	mov    %rax,%rdi
  4015fa:	e8 51 fb ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
            cfgInfos.push_back(entry);
  4015ff:	48 8b 85 18 fe ff ff 	mov    -0x1e8(%rbp),%rax
  401606:	48 8d 95 20 ff ff ff 	lea    -0xe0(%rbp),%rdx
  40160d:	48 89 d6             	mov    %rdx,%rsi
  401610:	48 89 c7             	mov    %rax,%rdi
  401613:	e8 78 08 00 00       	callq  401e90 <_ZNSt6vectorI10tagCfgInfoSaIS0_EE9push_backERKS0_>

            cnt = 0;
  401618:	c7 85 28 fe ff ff 00 	movl   $0x0,-0x1d8(%rbp)
  40161f:	00 00 00 
    string str;
    char buf[128] = {0};
    int cnt = 0;

    for (unsigned int i = 0; i < strlen(info); i++) {
        CFG_INFO_T entry;
  401622:	48 8d 85 20 ff ff ff 	lea    -0xe0(%rbp),%rax
  401629:	48 89 c7             	mov    %rax,%rdi
  40162c:	e8 2f 07 00 00       	callq  401d60 <_ZN10tagCfgInfoD1Ev>
    const char *info = "CPU:i5-6200U;MEM:8;USB:2/6;SSD:128+256;MEMC:32;HDD:500;WIFI:true;FAN:1;VGA:1;LCD:1920X1080";
    string str;
    char buf[128] = {0};
    int cnt = 0;

    for (unsigned int i = 0; i < strlen(info); i++) {
  401631:	83 85 2c fe ff ff 01 	addl   $0x1,-0x1d4(%rbp)
  401638:	e9 f8 fd ff ff       	jmpq   401435 <_ZN8HWConfig16loadHwConfigInfoEv+0x67>

            cnt = 0;
        }
    }

    if (cnt != 0) {
  40163d:	83 bd 28 fe ff ff 00 	cmpl   $0x0,-0x1d8(%rbp)
  401644:	0f 84 c7 01 00 00    	je     401811 <_ZN8HWConfig16loadHwConfigInfoEv+0x443>
        CFG_INFO_T entry;
  40164a:	48 8d 85 20 ff ff ff 	lea    -0xe0(%rbp),%rax
  401651:	48 89 c7             	mov    %rax,%rdi
  401654:	e8 bb 06 00 00       	callq  401d14 <_ZN10tagCfgInfoC1Ev>

        buf[cnt] = '\0';
  401659:	8b 85 28 fe ff ff    	mov    -0x1d8(%rbp),%eax
  40165f:	48 98                	cltq   
  401661:	c6 84 05 60 ff ff ff 	movb   $0x0,-0xa0(%rbp,%rax,1)
  401668:	00 
        str = string(buf);
  401669:	48 8d 85 27 fe ff ff 	lea    -0x1d9(%rbp),%rax
  401670:	48 89 c7             	mov    %rax,%rdi
  401673:	e8 c8 fb ff ff       	callq  401240 <_ZNSaIcEC1Ev@plt>
  401678:	48 8d 95 27 fe ff ff 	lea    -0x1d9(%rbp),%rdx
  40167f:	48 8d 8d 60 ff ff ff 	lea    -0xa0(%rbp),%rcx
  401686:	48 8d 85 c0 fe ff ff 	lea    -0x140(%rbp),%rax
  40168d:	48 89 ce             	mov    %rcx,%rsi
  401690:	48 89 c7             	mov    %rax,%rdi
  401693:	e8 98 fb ff ff       	callq  401230 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1EPKcRKS3_@plt>
  401698:	48 8d 95 c0 fe ff ff 	lea    -0x140(%rbp),%rdx
  40169f:	48 8d 85 40 fe ff ff 	lea    -0x1c0(%rbp),%rax
  4016a6:	48 89 d6             	mov    %rdx,%rsi
  4016a9:	48 89 c7             	mov    %rax,%rdi
  4016ac:	e8 0f fb ff ff       	callq  4011c0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEaSERKS4_@plt>
  4016b1:	48 8d 85 c0 fe ff ff 	lea    -0x140(%rbp),%rax
  4016b8:	48 89 c7             	mov    %rax,%rdi
  4016bb:	e8 90 fa ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
  4016c0:	48 8d 85 27 fe ff ff 	lea    -0x1d9(%rbp),%rax
  4016c7:	48 89 c7             	mov    %rax,%rdi
  4016ca:	e8 d1 fa ff ff       	callq  4011a0 <_ZNSaIcED1Ev@plt>

        int idx = str.find_first_of(':');
  4016cf:	48 8d 85 40 fe ff ff 	lea    -0x1c0(%rbp),%rax
  4016d6:	ba 00 00 00 00       	mov    $0x0,%edx
  4016db:	be 3a 00 00 00       	mov    $0x3a,%esi
  4016e0:	48 89 c7             	mov    %rax,%rdi
  4016e3:	e8 f8 f9 ff ff       	callq  4010e0 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE13find_first_ofEcm@plt>
  4016e8:	89 85 34 fe ff ff    	mov    %eax,-0x1cc(%rbp)
        entry.key = str.substr(0, idx);
  4016ee:	8b 85 34 fe ff ff    	mov    -0x1cc(%rbp),%eax
  4016f4:	48 63 d0             	movslq %eax,%rdx
  4016f7:	48 8d 85 e0 fe ff ff 	lea    -0x120(%rbp),%rax
  4016fe:	48 8d b5 40 fe ff ff 	lea    -0x1c0(%rbp),%rsi
  401705:	48 89 d1             	mov    %rdx,%rcx
  401708:	ba 00 00 00 00       	mov    $0x0,%edx
  40170d:	48 89 c7             	mov    %rax,%rdi
  401710:	e8 5b f9 ff ff       	callq  401070 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6substrEmm@plt>
  401715:	48 8d 95 e0 fe ff ff 	lea    -0x120(%rbp),%rdx
  40171c:	48 8d 85 20 ff ff ff 	lea    -0xe0(%rbp),%rax
  401723:	48 89 d6             	mov    %rdx,%rsi
  401726:	48 89 c7             	mov    %rax,%rdi
  401729:	e8 92 fa ff ff       	callq  4011c0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEaSERKS4_@plt>
  40172e:	48 8d 85 e0 fe ff ff 	lea    -0x120(%rbp),%rax
  401735:	48 89 c7             	mov    %rax,%rdi
  401738:	e8 13 fa ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
        entry.val = str.substr(idx+1, str.length()-idx-1);
  40173d:	48 8d 85 40 fe ff ff 	lea    -0x1c0(%rbp),%rax
  401744:	48 89 c7             	mov    %rax,%rdi
  401747:	e8 64 f9 ff ff       	callq  4010b0 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6lengthEv@plt>
  40174c:	48 89 c2             	mov    %rax,%rdx
  40174f:	8b 85 34 fe ff ff    	mov    -0x1cc(%rbp),%eax
  401755:	48 98                	cltq   
  401757:	48 29 c2             	sub    %rax,%rdx
  40175a:	48 89 d0             	mov    %rdx,%rax
  40175d:	48 8d 48 ff          	lea    -0x1(%rax),%rcx
  401761:	8b 85 34 fe ff ff    	mov    -0x1cc(%rbp),%eax
  401767:	83 c0 01             	add    $0x1,%eax
  40176a:	48 63 d0             	movslq %eax,%rdx
  40176d:	48 8d 85 00 ff ff ff 	lea    -0x100(%rbp),%rax
  401774:	48 8d b5 40 fe ff ff 	lea    -0x1c0(%rbp),%rsi
  40177b:	48 89 c7             	mov    %rax,%rdi
  40177e:	e8 ed f8 ff ff       	callq  401070 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6substrEmm@plt>
  401783:	48 8d 85 00 ff ff ff 	lea    -0x100(%rbp),%rax
  40178a:	48 8d 95 20 ff ff ff 	lea    -0xe0(%rbp),%rdx
  401791:	48 83 c2 20          	add    $0x20,%rdx
  401795:	48 89 c6             	mov    %rax,%rsi
  401798:	48 89 d7             	mov    %rdx,%rdi
  40179b:	e8 20 fa ff ff       	callq  4011c0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEaSERKS4_@plt>
  4017a0:	48 8d 85 00 ff ff ff 	lea    -0x100(%rbp),%rax
  4017a7:	48 89 c7             	mov    %rax,%rdi
  4017aa:	e8 a1 f9 ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
        cfgInfos.push_back(entry);
  4017af:	48 8b 85 18 fe ff ff 	mov    -0x1e8(%rbp),%rax
  4017b6:	48 8d 95 20 ff ff ff 	lea    -0xe0(%rbp),%rdx
  4017bd:	48 89 d6             	mov    %rdx,%rsi
  4017c0:	48 89 c7             	mov    %rax,%rdi
  4017c3:	e8 c8 06 00 00       	callq  401e90 <_ZNSt6vectorI10tagCfgInfoSaIS0_EE9push_backERKS0_>
        mlog("last add entry: key@[%s]\t val@[%s]\t buf@[%s]", entry.key.c_str(), entry.val.c_str());
  4017c8:	48 8d 85 20 ff ff ff 	lea    -0xe0(%rbp),%rax
  4017cf:	48 83 c0 20          	add    $0x20,%rax
  4017d3:	48 89 c7             	mov    %rax,%rdi
  4017d6:	e8 15 fa ff ff       	callq  4011f0 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE5c_strEv@plt>
  4017db:	48 89 c3             	mov    %rax,%rbx
  4017de:	48 8d 85 20 ff ff ff 	lea    -0xe0(%rbp),%rax
  4017e5:	48 89 c7             	mov    %rax,%rdi
  4017e8:	e8 03 fa ff ff       	callq  4011f0 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE5c_strEv@plt>
  4017ed:	48 89 da             	mov    %rbx,%rdx
  4017f0:	48 89 c6             	mov    %rax,%rsi
  4017f3:	bf e8 2f 40 00       	mov    $0x402fe8,%edi
  4017f8:	b8 00 00 00 00       	mov    $0x0,%eax
  4017fd:	e8 4e f8 ff ff       	callq  401050 <printf@plt>
            cnt = 0;
        }
    }

    if (cnt != 0) {
        CFG_INFO_T entry;
  401802:	48 8d 85 20 ff ff ff 	lea    -0xe0(%rbp),%rax
  401809:	48 89 c7             	mov    %rax,%rdi
  40180c:	e8 4f 05 00 00       	callq  401d60 <_ZN10tagCfgInfoD1Ev>
}

void HWConfig::loadHwConfigInfo()
{
    const char *info = "CPU:i5-6200U;MEM:8;USB:2/6;SSD:128+256;MEMC:32;HDD:500;WIFI:true;FAN:1;VGA:1;LCD:1920X1080";
    string str;
  401811:	48 8d 85 40 fe ff ff 	lea    -0x1c0(%rbp),%rax
  401818:	48 89 c7             	mov    %rax,%rdi
  40181b:	e8 30 f9 ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
        entry.val = str.substr(idx+1, str.length()-idx-1);
        cfgInfos.push_back(entry);
        mlog("last add entry: key@[%s]\t val@[%s]\t buf@[%s]", entry.key.c_str(), entry.val.c_str());
    }

}
  401820:	90                   	nop
  401821:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401825:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  40182c:	00 00 
  40182e:	0f 84 ef 00 00 00    	je     401923 <_ZN8HWConfig16loadHwConfigInfoEv+0x555>
  401834:	e9 e5 00 00 00       	jmpq   40191e <_ZN8HWConfig16loadHwConfigInfoEv+0x550>
  401839:	48 89 c3             	mov    %rax,%rbx

        if (info[i] != ';') {
            buf[cnt++]  = info[i];
        } else {
            buf[cnt] = '\0';
            str = string(buf);
  40183c:	48 8d 85 60 fe ff ff 	lea    -0x1a0(%rbp),%rax
  401843:	48 89 c7             	mov    %rax,%rdi
  401846:	e8 05 f9 ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
  40184b:	eb 03                	jmp    401850 <_ZN8HWConfig16loadHwConfigInfoEv+0x482>
  40184d:	48 89 c3             	mov    %rax,%rbx
  401850:	48 8d 85 27 fe ff ff 	lea    -0x1d9(%rbp),%rax
  401857:	48 89 c7             	mov    %rax,%rdi
  40185a:	e8 41 f9 ff ff       	callq  4011a0 <_ZNSaIcED1Ev@plt>
  40185f:	eb 2b                	jmp    40188c <_ZN8HWConfig16loadHwConfigInfoEv+0x4be>
  401861:	48 89 c3             	mov    %rax,%rbx

            int idx = str.find_first_of(':');
            entry.key = str.substr(0, idx);
  401864:	48 8d 85 80 fe ff ff 	lea    -0x180(%rbp),%rax
  40186b:	48 89 c7             	mov    %rax,%rdi
  40186e:	e8 dd f8 ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
  401873:	eb 17                	jmp    40188c <_ZN8HWConfig16loadHwConfigInfoEv+0x4be>
  401875:	48 89 c3             	mov    %rax,%rbx
            entry.val = str.substr(idx+1, str.length()-idx-1);
  401878:	48 8d 85 a0 fe ff ff 	lea    -0x160(%rbp),%rax
  40187f:	48 89 c7             	mov    %rax,%rdi
  401882:	e8 c9 f8 ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
  401887:	eb 03                	jmp    40188c <_ZN8HWConfig16loadHwConfigInfoEv+0x4be>
  401889:	48 89 c3             	mov    %rax,%rbx
    string str;
    char buf[128] = {0};
    int cnt = 0;

    for (unsigned int i = 0; i < strlen(info); i++) {
        CFG_INFO_T entry;
  40188c:	48 8d 85 20 ff ff ff 	lea    -0xe0(%rbp),%rax
  401893:	48 89 c7             	mov    %rax,%rdi
  401896:	e8 c5 04 00 00       	callq  401d60 <_ZN10tagCfgInfoD1Ev>
  40189b:	eb 67                	jmp    401904 <_ZN8HWConfig16loadHwConfigInfoEv+0x536>
  40189d:	48 89 c3             	mov    %rax,%rbx

    if (cnt != 0) {
        CFG_INFO_T entry;

        buf[cnt] = '\0';
        str = string(buf);
  4018a0:	48 8d 85 c0 fe ff ff 	lea    -0x140(%rbp),%rax
  4018a7:	48 89 c7             	mov    %rax,%rdi
  4018aa:	e8 a1 f8 ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
  4018af:	eb 03                	jmp    4018b4 <_ZN8HWConfig16loadHwConfigInfoEv+0x4e6>
  4018b1:	48 89 c3             	mov    %rax,%rbx
  4018b4:	48 8d 85 27 fe ff ff 	lea    -0x1d9(%rbp),%rax
  4018bb:	48 89 c7             	mov    %rax,%rdi
  4018be:	e8 dd f8 ff ff       	callq  4011a0 <_ZNSaIcED1Ev@plt>
  4018c3:	eb 2b                	jmp    4018f0 <_ZN8HWConfig16loadHwConfigInfoEv+0x522>
  4018c5:	48 89 c3             	mov    %rax,%rbx

        int idx = str.find_first_of(':');
        entry.key = str.substr(0, idx);
  4018c8:	48 8d 85 e0 fe ff ff 	lea    -0x120(%rbp),%rax
  4018cf:	48 89 c7             	mov    %rax,%rdi
  4018d2:	e8 79 f8 ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
  4018d7:	eb 17                	jmp    4018f0 <_ZN8HWConfig16loadHwConfigInfoEv+0x522>
  4018d9:	48 89 c3             	mov    %rax,%rbx
        entry.val = str.substr(idx+1, str.length()-idx-1);
  4018dc:	48 8d 85 00 ff ff ff 	lea    -0x100(%rbp),%rax
  4018e3:	48 89 c7             	mov    %rax,%rdi
  4018e6:	e8 65 f8 ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
  4018eb:	eb 03                	jmp    4018f0 <_ZN8HWConfig16loadHwConfigInfoEv+0x522>
  4018ed:	48 89 c3             	mov    %rax,%rbx
            cnt = 0;
        }
    }

    if (cnt != 0) {
        CFG_INFO_T entry;
  4018f0:	48 8d 85 20 ff ff ff 	lea    -0xe0(%rbp),%rax
  4018f7:	48 89 c7             	mov    %rax,%rdi
  4018fa:	e8 61 04 00 00       	callq  401d60 <_ZN10tagCfgInfoD1Ev>
  4018ff:	eb 03                	jmp    401904 <_ZN8HWConfig16loadHwConfigInfoEv+0x536>
  401901:	48 89 c3             	mov    %rax,%rbx
}

void HWConfig::loadHwConfigInfo()
{
    const char *info = "CPU:i5-6200U;MEM:8;USB:2/6;SSD:128+256;MEMC:32;HDD:500;WIFI:true;FAN:1;VGA:1;LCD:1920X1080";
    string str;
  401904:	48 8d 85 40 fe ff ff 	lea    -0x1c0(%rbp),%rax
  40190b:	48 89 c7             	mov    %rax,%rdi
  40190e:	e8 3d f8 ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
  401913:	48 89 d8             	mov    %rbx,%rax
  401916:	48 89 c7             	mov    %rax,%rdi
  401919:	e8 52 f9 ff ff       	callq  401270 <_Unwind_Resume@plt>
        entry.val = str.substr(idx+1, str.length()-idx-1);
        cfgInfos.push_back(entry);
        mlog("last add entry: key@[%s]\t val@[%s]\t buf@[%s]", entry.key.c_str(), entry.val.c_str());
    }

}
  40191e:	e8 5d f8 ff ff       	callq  401180 <__stack_chk_fail@plt>
  401923:	48 81 c4 e8 01 00 00 	add    $0x1e8,%rsp
  40192a:	5b                   	pop    %rbx
  40192b:	5d                   	pop    %rbp
  40192c:	c3                   	retq   
  40192d:	90                   	nop

000000000040192e <_ZN8HWConfigD1Ev>:

HWConfig::~HWConfig()
  40192e:	55                   	push   %rbp
  40192f:	48 89 e5             	mov    %rsp,%rbp
  401932:	48 83 ec 10          	sub    $0x10,%rsp
  401936:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40193a:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40193e:	48 89 c7             	mov    %rax,%rdi
  401941:	e8 e4 04 00 00       	callq  401e2a <_ZNSt6vectorI10tagCfgInfoSaIS0_EED1Ev>
{

}
  401946:	90                   	nop
  401947:	c9                   	leaveq 
  401948:	c3                   	retq   
  401949:	90                   	nop

000000000040194a <_ZN8HWConfig12getBoolValueEPKc>:

/***********getter*************/
bool HWConfig::getBoolValue(const char *key)
{
  40194a:	55                   	push   %rbp
  40194b:	48 89 e5             	mov    %rsp,%rbp
  40194e:	48 83 ec 20          	sub    $0x20,%rsp
  401952:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  401956:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
    const char *str = getStringValue(key);
  40195a:	48 8b 55 e0          	mov    -0x20(%rbp),%rdx
  40195e:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401962:	48 89 d6             	mov    %rdx,%rsi
  401965:	48 89 c7             	mov    %rax,%rdi
  401968:	e8 f5 00 00 00       	callq  401a62 <_ZN8HWConfig14getStringValueEPKc>
  40196d:	48 89 45 f8          	mov    %rax,-0x8(%rbp)

    if (str == NULL) {
  401971:	48 83 7d f8 00       	cmpq   $0x0,-0x8(%rbp)
  401976:	75 1d                	jne    401995 <_ZN8HWConfig12getBoolValueEPKc+0x4b>
        log("notfound for [%s]-[%s]", key);
  401978:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
  40197c:	48 89 c6             	mov    %rax,%rsi
  40197f:	bf 16 30 40 00       	mov    $0x403016,%edi
  401984:	b8 00 00 00 00       	mov    $0x0,%eax
  401989:	e8 c2 f6 ff ff       	callq  401050 <printf@plt>
        return false;
  40198e:	b8 00 00 00 00       	mov    $0x0,%eax
  401993:	eb 21                	jmp    4019b6 <_ZN8HWConfig12getBoolValueEPKc+0x6c>
    }

    if (strcmp(str,"true") == 0) {
  401995:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401999:	be 2e 30 40 00       	mov    $0x40302e,%esi
  40199e:	48 89 c7             	mov    %rax,%rdi
  4019a1:	e8 ea f7 ff ff       	callq  401190 <strcmp@plt>
  4019a6:	85 c0                	test   %eax,%eax
  4019a8:	75 07                	jne    4019b1 <_ZN8HWConfig12getBoolValueEPKc+0x67>
        return true;
  4019aa:	b8 01 00 00 00       	mov    $0x1,%eax
  4019af:	eb 05                	jmp    4019b6 <_ZN8HWConfig12getBoolValueEPKc+0x6c>
    } else {
        return false;
  4019b1:	b8 00 00 00 00       	mov    $0x0,%eax
    }
}
  4019b6:	c9                   	leaveq 
  4019b7:	c3                   	retq   

00000000004019b8 <_ZN8HWConfig11getIntValueEPKc>:

int HWConfig::getIntValue(const char *key)
{
  4019b8:	55                   	push   %rbp
  4019b9:	48 89 e5             	mov    %rsp,%rbp
  4019bc:	48 83 ec 20          	sub    $0x20,%rsp
  4019c0:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  4019c4:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
    const char *str = getStringValue(key);
  4019c8:	48 8b 55 e0          	mov    -0x20(%rbp),%rdx
  4019cc:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4019d0:	48 89 d6             	mov    %rdx,%rsi
  4019d3:	48 89 c7             	mov    %rax,%rdi
  4019d6:	e8 87 00 00 00       	callq  401a62 <_ZN8HWConfig14getStringValueEPKc>
  4019db:	48 89 45 f8          	mov    %rax,-0x8(%rbp)

    if (!str) {
  4019df:	48 83 7d f8 00       	cmpq   $0x0,-0x8(%rbp)
  4019e4:	75 07                	jne    4019ed <_ZN8HWConfig11getIntValueEPKc+0x35>
        return -1;
  4019e6:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  4019eb:	eb 0c                	jmp    4019f9 <_ZN8HWConfig11getIntValueEPKc+0x41>
    } else {
        return atoi(str);
  4019ed:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4019f1:	48 89 c7             	mov    %rax,%rdi
  4019f4:	e8 77 f7 ff ff       	callq  401170 <atoi@plt>
    }
}
  4019f9:	c9                   	leaveq 
  4019fa:	c3                   	retq   
  4019fb:	90                   	nop

00000000004019fc <_ZN8HWConfig13getFloatValueEPKc>:

float HWConfig::getFloatValue(const char* key)
{
  4019fc:	55                   	push   %rbp
  4019fd:	48 89 e5             	mov    %rsp,%rbp
  401a00:	48 83 ec 20          	sub    $0x20,%rsp
  401a04:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  401a08:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
    const char *str = getStringValue(key);
  401a0c:	48 8b 55 e0          	mov    -0x20(%rbp),%rdx
  401a10:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401a14:	48 89 d6             	mov    %rdx,%rsi
  401a17:	48 89 c7             	mov    %rax,%rdi
  401a1a:	e8 43 00 00 00       	callq  401a62 <_ZN8HWConfig14getStringValueEPKc>
  401a1f:	48 89 45 f8          	mov    %rax,-0x8(%rbp)

    if(str == NULL) {
  401a23:	48 83 7d f8 00       	cmpq   $0x0,-0x8(%rbp)
  401a28:	75 26                	jne    401a50 <_ZN8HWConfig13getFloatValueEPKc+0x54>
        cout<<"notfound"<<endl;
  401a2a:	be 33 30 40 00       	mov    $0x403033,%esi
  401a2f:	bf 60 51 60 00       	mov    $0x605160,%edi
  401a34:	e8 e7 f6 ff ff       	callq  401120 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  401a39:	be 00 12 40 00       	mov    $0x401200,%esi
  401a3e:	48 89 c7             	mov    %rax,%rdi
  401a41:	e8 6a f7 ff ff       	callq  4011b0 <_ZNSolsEPFRSoS_E@plt>
        return -1.0;
  401a46:	f3 0f 10 05 7e 16 00 	movss  0x167e(%rip),%xmm0        # 4030cc <_IO_stdin_used+0x14c>
  401a4d:	00 
  401a4e:	eb 10                	jmp    401a60 <_ZN8HWConfig13getFloatValueEPKc+0x64>
    }

    return atof(str);
  401a50:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401a54:	48 89 c7             	mov    %rax,%rdi
  401a57:	e8 b4 f6 ff ff       	callq  401110 <atof@plt>
  401a5c:	f2 0f 5a c0          	cvtsd2ss %xmm0,%xmm0
}
  401a60:	c9                   	leaveq 
  401a61:	c3                   	retq   

0000000000401a62 <_ZN8HWConfig14getStringValueEPKc>:

const char* HWConfig::getStringValue(const char *key)
{
  401a62:	55                   	push   %rbp
  401a63:	48 89 e5             	mov    %rsp,%rbp
  401a66:	53                   	push   %rbx
  401a67:	48 83 ec 28          	sub    $0x28,%rsp
  401a6b:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
  401a6f:	48 89 75 d0          	mov    %rsi,-0x30(%rbp)
    mlog("finding key[%s]", key);
  401a73:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
  401a77:	48 89 c6             	mov    %rax,%rsi
  401a7a:	bf 3c 30 40 00       	mov    $0x40303c,%edi
  401a7f:	b8 00 00 00 00       	mov    $0x0,%eax
  401a84:	e8 c7 f5 ff ff       	callq  401050 <printf@plt>
    for(unsigned int i = 0; i < cfgInfos.size(); i++) {
  401a89:	c7 45 ec 00 00 00 00 	movl   $0x0,-0x14(%rbp)
  401a90:	8b 5d ec             	mov    -0x14(%rbp),%ebx
  401a93:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
  401a97:	48 89 c7             	mov    %rax,%rdi
  401a9a:	e8 6d 04 00 00       	callq  401f0c <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE4sizeEv>
  401a9f:	48 39 c3             	cmp    %rax,%rbx
  401aa2:	0f 92 c0             	setb   %al
  401aa5:	84 c0                	test   %al,%al
  401aa7:	0f 84 8a 00 00 00    	je     401b37 <_ZN8HWConfig14getStringValueEPKc+0xd5>
        if(strcmp(cfgInfos[i].key.c_str(), key) == 0) {
  401aad:	8b 55 ec             	mov    -0x14(%rbp),%edx
  401ab0:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
  401ab4:	48 89 d6             	mov    %rdx,%rsi
  401ab7:	48 89 c7             	mov    %rax,%rdi
  401aba:	e8 73 04 00 00       	callq  401f32 <_ZNSt6vectorI10tagCfgInfoSaIS0_EEixEm>
  401abf:	48 89 c7             	mov    %rax,%rdi
  401ac2:	e8 29 f7 ff ff       	callq  4011f0 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE5c_strEv@plt>
  401ac7:	48 89 c2             	mov    %rax,%rdx
  401aca:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
  401ace:	48 89 c6             	mov    %rax,%rsi
  401ad1:	48 89 d7             	mov    %rdx,%rdi
  401ad4:	e8 b7 f6 ff ff       	callq  401190 <strcmp@plt>
  401ad9:	85 c0                	test   %eax,%eax
  401adb:	0f 94 c0             	sete   %al
  401ade:	84 c0                	test   %al,%al
  401ae0:	74 4c                	je     401b2e <_ZN8HWConfig14getStringValueEPKc+0xcc>
            mlog("finded key[%s]", cfgInfos[i].key.c_str());
  401ae2:	8b 55 ec             	mov    -0x14(%rbp),%edx
  401ae5:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
  401ae9:	48 89 d6             	mov    %rdx,%rsi
  401aec:	48 89 c7             	mov    %rax,%rdi
  401aef:	e8 3e 04 00 00       	callq  401f32 <_ZNSt6vectorI10tagCfgInfoSaIS0_EEixEm>
  401af4:	48 89 c7             	mov    %rax,%rdi
  401af7:	e8 f4 f6 ff ff       	callq  4011f0 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE5c_strEv@plt>
  401afc:	48 89 c6             	mov    %rax,%rsi
  401aff:	bf 4d 30 40 00       	mov    $0x40304d,%edi
  401b04:	b8 00 00 00 00       	mov    $0x0,%eax
  401b09:	e8 42 f5 ff ff       	callq  401050 <printf@plt>
            return cfgInfos[i].val.c_str();
  401b0e:	8b 55 ec             	mov    -0x14(%rbp),%edx
  401b11:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
  401b15:	48 89 d6             	mov    %rdx,%rsi
  401b18:	48 89 c7             	mov    %rax,%rdi
  401b1b:	e8 12 04 00 00       	callq  401f32 <_ZNSt6vectorI10tagCfgInfoSaIS0_EEixEm>
  401b20:	48 83 c0 20          	add    $0x20,%rax
  401b24:	48 89 c7             	mov    %rax,%rdi
  401b27:	e8 c4 f6 ff ff       	callq  4011f0 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE5c_strEv@plt>
  401b2c:	eb 49                	jmp    401b77 <_ZN8HWConfig14getStringValueEPKc+0x115>
}

const char* HWConfig::getStringValue(const char *key)
{
    mlog("finding key[%s]", key);
    for(unsigned int i = 0; i < cfgInfos.size(); i++) {
  401b2e:	83 45 ec 01          	addl   $0x1,-0x14(%rbp)
  401b32:	e9 59 ff ff ff       	jmpq   401a90 <_ZN8HWConfig14getStringValueEPKc+0x2e>
            mlog("finded key[%s]", cfgInfos[i].key.c_str());
            return cfgInfos[i].val.c_str();
        }
    }

    cout<<"DEBUG: ["<<key<<"] not found"<<endl;
  401b37:	be 5d 30 40 00       	mov    $0x40305d,%esi
  401b3c:	bf 60 51 60 00       	mov    $0x605160,%edi
  401b41:	e8 da f5 ff ff       	callq  401120 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  401b46:	48 89 c2             	mov    %rax,%rdx
  401b49:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
  401b4d:	48 89 c6             	mov    %rax,%rsi
  401b50:	48 89 d7             	mov    %rdx,%rdi
  401b53:	e8 c8 f5 ff ff       	callq  401120 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  401b58:	be 66 30 40 00       	mov    $0x403066,%esi
  401b5d:	48 89 c7             	mov    %rax,%rdi
  401b60:	e8 bb f5 ff ff       	callq  401120 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  401b65:	be 00 12 40 00       	mov    $0x401200,%esi
  401b6a:	48 89 c7             	mov    %rax,%rdi
  401b6d:	e8 3e f6 ff ff       	callq  4011b0 <_ZNSolsEPFRSoS_E@plt>
    return NULL;
  401b72:	b8 00 00 00 00       	mov    $0x0,%eax
}
  401b77:	48 83 c4 28          	add    $0x28,%rsp
  401b7b:	5b                   	pop    %rbx
  401b7c:	5d                   	pop    %rbp
  401b7d:	c3                   	retq   

0000000000401b7e <_ZN8HWConfig8printAllEv>:

/*------------------------------------ for DEBUG ---------------------------------------*/
void HWConfig::printAll()
{
  401b7e:	55                   	push   %rbp
  401b7f:	48 89 e5             	mov    %rsp,%rbp
  401b82:	53                   	push   %rbx
  401b83:	48 81 ec 88 00 00 00 	sub    $0x88,%rsp
  401b8a:	48 89 bd 78 ff ff ff 	mov    %rdi,-0x88(%rbp)
  401b91:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401b98:	00 00 
  401b9a:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
  401b9e:	31 c0                	xor    %eax,%eax
    log("--------print All loaded Entry------------");
  401ba0:	bf 78 30 40 00       	mov    $0x403078,%edi
  401ba5:	e8 d6 f4 ff ff       	callq  401080 <puts@plt>
    for (vector<CFG_INFO_T>::iterator it=cfgInfos.begin(); it!= cfgInfos.end(); it++) {
  401baa:	48 8b 85 78 ff ff ff 	mov    -0x88(%rbp),%rax
  401bb1:	48 89 c7             	mov    %rax,%rdi
  401bb4:	e8 99 03 00 00       	callq  401f52 <_ZNSt6vectorI10tagCfgInfoSaIS0_EE5beginEv>
  401bb9:	48 89 45 80          	mov    %rax,-0x80(%rbp)
  401bbd:	48 8b 85 78 ff ff ff 	mov    -0x88(%rbp),%rax
  401bc4:	48 89 c7             	mov    %rax,%rdi
  401bc7:	e8 ce 03 00 00       	callq  401f9a <_ZNSt6vectorI10tagCfgInfoSaIS0_EE3endEv>
  401bcc:	48 89 45 90          	mov    %rax,-0x70(%rbp)
  401bd0:	48 8d 55 90          	lea    -0x70(%rbp),%rdx
  401bd4:	48 8d 45 80          	lea    -0x80(%rbp),%rax
  401bd8:	48 89 d6             	mov    %rdx,%rsi
  401bdb:	48 89 c7             	mov    %rax,%rdi
  401bde:	e8 03 04 00 00       	callq  401fe6 <_ZN9__gnu_cxxneIP10tagCfgInfoSt6vectorIS1_SaIS1_EEEEbRKNS_17__normal_iteratorIT_T0_EESB_>
  401be3:	84 c0                	test   %al,%al
  401be5:	74 74                	je     401c5b <_ZN8HWConfig8printAllEv+0xdd>
        CFG_INFO_T entry = *it;
  401be7:	48 8d 45 80          	lea    -0x80(%rbp),%rax
  401beb:	48 89 c7             	mov    %rax,%rdi
  401bee:	e8 91 04 00 00       	callq  402084 <_ZNK9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEEdeEv>
  401bf3:	48 89 c2             	mov    %rax,%rdx
  401bf6:	48 8d 45 a0          	lea    -0x60(%rbp),%rax
  401bfa:	48 89 d6             	mov    %rdx,%rsi
  401bfd:	48 89 c7             	mov    %rax,%rdi
  401c00:	e8 a7 01 00 00       	callq  401dac <_ZN10tagCfgInfoC1ERKS_>
        log("  key:%s\t val:%s", entry.key.c_str(), entry.val.c_str());
  401c05:	48 8d 45 a0          	lea    -0x60(%rbp),%rax
  401c09:	48 83 c0 20          	add    $0x20,%rax
  401c0d:	48 89 c7             	mov    %rax,%rdi
  401c10:	e8 db f5 ff ff       	callq  4011f0 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE5c_strEv@plt>
  401c15:	48 89 c3             	mov    %rax,%rbx
  401c18:	48 8d 45 a0          	lea    -0x60(%rbp),%rax
  401c1c:	48 89 c7             	mov    %rax,%rdi
  401c1f:	e8 cc f5 ff ff       	callq  4011f0 <_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE5c_strEv@plt>
  401c24:	48 89 da             	mov    %rbx,%rdx
  401c27:	48 89 c6             	mov    %rax,%rsi
  401c2a:	bf a3 30 40 00       	mov    $0x4030a3,%edi
  401c2f:	b8 00 00 00 00       	mov    $0x0,%eax
  401c34:	e8 17 f4 ff ff       	callq  401050 <printf@plt>
/*------------------------------------ for DEBUG ---------------------------------------*/
void HWConfig::printAll()
{
    log("--------print All loaded Entry------------");
    for (vector<CFG_INFO_T>::iterator it=cfgInfos.begin(); it!= cfgInfos.end(); it++) {
        CFG_INFO_T entry = *it;
  401c39:	48 8d 45 a0          	lea    -0x60(%rbp),%rax
  401c3d:	48 89 c7             	mov    %rax,%rdi
  401c40:	e8 1b 01 00 00       	callq  401d60 <_ZN10tagCfgInfoD1Ev>

/*------------------------------------ for DEBUG ---------------------------------------*/
void HWConfig::printAll()
{
    log("--------print All loaded Entry------------");
    for (vector<CFG_INFO_T>::iterator it=cfgInfos.begin(); it!= cfgInfos.end(); it++) {
  401c45:	48 8d 45 80          	lea    -0x80(%rbp),%rax
  401c49:	be 00 00 00 00       	mov    $0x0,%esi
  401c4e:	48 89 c7             	mov    %rax,%rdi
  401c51:	e8 cc 03 00 00       	callq  402022 <_ZN9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEEppEi>
  401c56:	e9 62 ff ff ff       	jmpq   401bbd <_ZN8HWConfig8printAllEv+0x3f>
        CFG_INFO_T entry = *it;
        log("  key:%s\t val:%s", entry.key.c_str(), entry.val.c_str());
    }
    log("--------print All loaded Entry------------");
  401c5b:	bf 78 30 40 00       	mov    $0x403078,%edi
  401c60:	e8 1b f4 ff ff       	callq  401080 <puts@plt>
}
  401c65:	90                   	nop
  401c66:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401c6a:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  401c71:	00 00 
  401c73:	74 21                	je     401c96 <_ZN8HWConfig8printAllEv+0x118>
  401c75:	eb 1a                	jmp    401c91 <_ZN8HWConfig8printAllEv+0x113>
  401c77:	48 89 c3             	mov    %rax,%rbx
/*------------------------------------ for DEBUG ---------------------------------------*/
void HWConfig::printAll()
{
    log("--------print All loaded Entry------------");
    for (vector<CFG_INFO_T>::iterator it=cfgInfos.begin(); it!= cfgInfos.end(); it++) {
        CFG_INFO_T entry = *it;
  401c7a:	48 8d 45 a0          	lea    -0x60(%rbp),%rax
  401c7e:	48 89 c7             	mov    %rax,%rdi
  401c81:	e8 da 00 00 00       	callq  401d60 <_ZN10tagCfgInfoD1Ev>
  401c86:	48 89 d8             	mov    %rbx,%rax
  401c89:	48 89 c7             	mov    %rax,%rdi
  401c8c:	e8 df f5 ff ff       	callq  401270 <_Unwind_Resume@plt>
        log("  key:%s\t val:%s", entry.key.c_str(), entry.val.c_str());
    }
    log("--------print All loaded Entry------------");
}
  401c91:	e8 ea f4 ff ff       	callq  401180 <__stack_chk_fail@plt>
  401c96:	48 81 c4 88 00 00 00 	add    $0x88,%rsp
  401c9d:	5b                   	pop    %rbx
  401c9e:	5d                   	pop    %rbp
  401c9f:	c3                   	retq   

0000000000401ca0 <_Z41__static_initialization_and_destruction_0ii>:
  401ca0:	55                   	push   %rbp
  401ca1:	48 89 e5             	mov    %rsp,%rbp
  401ca4:	48 83 ec 10          	sub    $0x10,%rsp
  401ca8:	89 7d fc             	mov    %edi,-0x4(%rbp)
  401cab:	89 75 f8             	mov    %esi,-0x8(%rbp)
  401cae:	83 7d fc 01          	cmpl   $0x1,-0x4(%rbp)
  401cb2:	75 27                	jne    401cdb <_Z41__static_initialization_and_destruction_0ii+0x3b>
  401cb4:	81 7d f8 ff ff 00 00 	cmpl   $0xffff,-0x8(%rbp)
  401cbb:	75 1e                	jne    401cdb <_Z41__static_initialization_and_destruction_0ii+0x3b>
  extern wostream wclog;	/// Linked to standard error (buffered)
#endif
  //@}

  // For construction of filebuffers for cout, cin, cerr, clog et. al.
  static ios_base::Init __ioinit;
  401cbd:	bf 71 52 60 00       	mov    $0x605271,%edi
  401cc2:	e8 f9 f3 ff ff       	callq  4010c0 <_ZNSt8ios_base4InitC1Ev@plt>
  401cc7:	ba 40 51 60 00       	mov    $0x605140,%edx
  401ccc:	be 71 52 60 00       	mov    $0x605271,%esi
  401cd1:	bf 00 11 40 00       	mov    $0x401100,%edi
  401cd6:	e8 15 f4 ff ff       	callq  4010f0 <__cxa_atexit@plt>
  401cdb:	90                   	nop
  401cdc:	c9                   	leaveq 
  401cdd:	c3                   	retq   

0000000000401cde <_GLOBAL__sub_I__ZN8HWConfigC2Ev>:
  401cde:	55                   	push   %rbp
  401cdf:	48 89 e5             	mov    %rsp,%rbp
  401ce2:	be ff ff 00 00       	mov    $0xffff,%esi
  401ce7:	bf 01 00 00 00       	mov    $0x1,%edi
  401cec:	e8 af ff ff ff       	callq  401ca0 <_Z41__static_initialization_and_destruction_0ii>
  401cf1:	5d                   	pop    %rbp
  401cf2:	c3                   	retq   

0000000000401cf3 <_ZnwmPv>:
void operator delete[](void*, const std::nothrow_t&) _GLIBCXX_USE_NOEXCEPT
  __attribute__((__externally_visible__));

// Default placement versions of operator new.
inline void* operator new(std::size_t, void* __p) _GLIBCXX_USE_NOEXCEPT
{ return __p; }
  401cf3:	55                   	push   %rbp
  401cf4:	48 89 e5             	mov    %rsp,%rbp
  401cf7:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401cfb:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  401cff:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  401d03:	5d                   	pop    %rbp
  401d04:	c3                   	retq   

0000000000401d05 <_ZdlPvS_>:
inline void* operator new[](std::size_t, void* __p) _GLIBCXX_USE_NOEXCEPT
{ return __p; }

// Default placement versions of operator delete.
inline void operator delete  (void*, void*) _GLIBCXX_USE_NOEXCEPT { }
  401d05:	55                   	push   %rbp
  401d06:	48 89 e5             	mov    %rsp,%rbp
  401d09:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401d0d:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  401d11:	90                   	nop
  401d12:	5d                   	pop    %rbp
  401d13:	c3                   	retq   

0000000000401d14 <_ZN10tagCfgInfoC1Ev>:
            if(HWConfig_DEBUG) printf(format"\n", ##args);    \
        } while(0)

using namespace std;

typedef struct tagCfgInfo {
  401d14:	55                   	push   %rbp
  401d15:	48 89 e5             	mov    %rsp,%rbp
  401d18:	53                   	push   %rbx
  401d19:	48 83 ec 18          	sub    $0x18,%rsp
  401d1d:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  401d21:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401d25:	48 89 c7             	mov    %rax,%rdi
  401d28:	e8 03 f4 ff ff       	callq  401130 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1Ev@plt>
  401d2d:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401d31:	48 83 c0 20          	add    $0x20,%rax
  401d35:	48 89 c7             	mov    %rax,%rdi
  401d38:	e8 f3 f3 ff ff       	callq  401130 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1Ev@plt>
  401d3d:	eb 1a                	jmp    401d59 <_ZN10tagCfgInfoC1Ev+0x45>
  401d3f:	48 89 c3             	mov    %rax,%rbx
  401d42:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401d46:	48 89 c7             	mov    %rax,%rdi
  401d49:	e8 02 f4 ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
  401d4e:	48 89 d8             	mov    %rbx,%rax
  401d51:	48 89 c7             	mov    %rax,%rdi
  401d54:	e8 17 f5 ff ff       	callq  401270 <_Unwind_Resume@plt>
  401d59:	48 83 c4 18          	add    $0x18,%rsp
  401d5d:	5b                   	pop    %rbx
  401d5e:	5d                   	pop    %rbp
  401d5f:	c3                   	retq   

0000000000401d60 <_ZN10tagCfgInfoD1Ev>:
  401d60:	55                   	push   %rbp
  401d61:	48 89 e5             	mov    %rsp,%rbp
  401d64:	53                   	push   %rbx
  401d65:	48 83 ec 18          	sub    $0x18,%rsp
  401d69:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  401d6d:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401d71:	48 83 c0 20          	add    $0x20,%rax
  401d75:	48 89 c7             	mov    %rax,%rdi
  401d78:	e8 d3 f3 ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
  401d7d:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401d81:	48 89 c7             	mov    %rax,%rdi
  401d84:	e8 c7 f3 ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
  401d89:	eb 1a                	jmp    401da5 <_ZN10tagCfgInfoD1Ev+0x45>
  401d8b:	48 89 c3             	mov    %rax,%rbx
  401d8e:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401d92:	48 89 c7             	mov    %rax,%rdi
  401d95:	e8 b6 f3 ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
  401d9a:	48 89 d8             	mov    %rbx,%rax
  401d9d:	48 89 c7             	mov    %rax,%rdi
  401da0:	e8 cb f4 ff ff       	callq  401270 <_Unwind_Resume@plt>
  401da5:	48 83 c4 18          	add    $0x18,%rsp
  401da9:	5b                   	pop    %rbx
  401daa:	5d                   	pop    %rbp
  401dab:	c3                   	retq   

0000000000401dac <_ZN10tagCfgInfoC1ERKS_>:
  401dac:	55                   	push   %rbp
  401dad:	48 89 e5             	mov    %rsp,%rbp
  401db0:	53                   	push   %rbx
  401db1:	48 83 ec 18          	sub    $0x18,%rsp
  401db5:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  401db9:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
  401dbd:	48 8b 55 e0          	mov    -0x20(%rbp),%rdx
  401dc1:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401dc5:	48 89 d6             	mov    %rdx,%rsi
  401dc8:	48 89 c7             	mov    %rax,%rdi
  401dcb:	e8 50 f4 ff ff       	callq  401220 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1ERKS4_@plt>
  401dd0:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
  401dd4:	48 8d 50 20          	lea    0x20(%rax),%rdx
  401dd8:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401ddc:	48 83 c0 20          	add    $0x20,%rax
  401de0:	48 89 d6             	mov    %rdx,%rsi
  401de3:	48 89 c7             	mov    %rax,%rdi
  401de6:	e8 35 f4 ff ff       	callq  401220 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1ERKS4_@plt>
  401deb:	eb 1a                	jmp    401e07 <_ZN10tagCfgInfoC1ERKS_+0x5b>
  401ded:	48 89 c3             	mov    %rax,%rbx
  401df0:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401df4:	48 89 c7             	mov    %rax,%rdi
  401df7:	e8 54 f3 ff ff       	callq  401150 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev@plt>
  401dfc:	48 89 d8             	mov    %rbx,%rax
  401dff:	48 89 c7             	mov    %rax,%rdi
  401e02:	e8 69 f4 ff ff       	callq  401270 <_Unwind_Resume@plt>
  401e07:	48 83 c4 18          	add    $0x18,%rsp
  401e0b:	5b                   	pop    %rbx
  401e0c:	5d                   	pop    %rbp
  401e0d:	c3                   	retq   

0000000000401e0e <_ZNSt6vectorI10tagCfgInfoSaIS0_EEC1Ev>:
      // (assign() and get_allocator() are also listed in this section)

      /**
       *  @brief  Creates a %vector with no elements.
       */
      vector()
  401e0e:	55                   	push   %rbp
  401e0f:	48 89 e5             	mov    %rsp,%rbp
  401e12:	48 83 ec 10          	sub    $0x10,%rsp
  401e16:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
#if __cplusplus >= 201103L
      noexcept(is_nothrow_default_constructible<_Alloc>::value)
#endif
      : _Base() { }
  401e1a:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401e1e:	48 89 c7             	mov    %rax,%rdi
  401e21:	e8 8c 02 00 00       	callq  4020b2 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EEC1Ev>
  401e26:	90                   	nop
  401e27:	c9                   	leaveq 
  401e28:	c3                   	retq   
  401e29:	90                   	nop

0000000000401e2a <_ZNSt6vectorI10tagCfgInfoSaIS0_EED1Ev>:
       *  The dtor only erases the elements, and note that if the
       *  elements themselves are pointers, the pointed-to memory is
       *  not touched in any way.  Managing the pointer is the user's
       *  responsibility.
       */
      ~vector() _GLIBCXX_NOEXCEPT
  401e2a:	55                   	push   %rbp
  401e2b:	48 89 e5             	mov    %rsp,%rbp
  401e2e:	53                   	push   %rbx
  401e2f:	48 83 ec 18          	sub    $0x18,%rsp
  401e33:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
      { std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
		      _M_get_Tp_allocator()); }
  401e37:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401e3b:	48 89 c7             	mov    %rax,%rdi
  401e3e:	e8 fd 02 00 00       	callq  402140 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE19_M_get_Tp_allocatorEv>
  401e43:	48 89 c2             	mov    %rax,%rdx
       *  elements themselves are pointers, the pointed-to memory is
       *  not touched in any way.  Managing the pointer is the user's
       *  responsibility.
       */
      ~vector() _GLIBCXX_NOEXCEPT
      { std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
  401e46:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401e4a:	48 8b 48 08          	mov    0x8(%rax),%rcx
  401e4e:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401e52:	48 8b 00             	mov    (%rax),%rax
  401e55:	48 89 ce             	mov    %rcx,%rsi
  401e58:	48 89 c7             	mov    %rax,%rdi
  401e5b:	e8 ee 02 00 00       	callq  40214e <_ZSt8_DestroyIP10tagCfgInfoS0_EvT_S2_RSaIT0_E>
		      _M_get_Tp_allocator()); }
  401e60:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401e64:	48 89 c7             	mov    %rax,%rdi
  401e67:	e8 62 02 00 00       	callq  4020ce <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EED1Ev>
  401e6c:	eb 1a                	jmp    401e88 <_ZNSt6vectorI10tagCfgInfoSaIS0_EED1Ev+0x5e>
  401e6e:	48 89 c3             	mov    %rax,%rbx
  401e71:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401e75:	48 89 c7             	mov    %rax,%rdi
  401e78:	e8 51 02 00 00       	callq  4020ce <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EED1Ev>
  401e7d:	48 89 d8             	mov    %rbx,%rax
  401e80:	48 89 c7             	mov    %rax,%rdi
  401e83:	e8 e8 f3 ff ff       	callq  401270 <_Unwind_Resume@plt>
  401e88:	48 83 c4 18          	add    $0x18,%rsp
  401e8c:	5b                   	pop    %rbx
  401e8d:	5d                   	pop    %rbp
  401e8e:	c3                   	retq   
  401e8f:	90                   	nop

0000000000401e90 <_ZNSt6vectorI10tagCfgInfoSaIS0_EE9push_backERKS0_>:
       *  to it.  Due to the nature of a %vector this operation can be
       *  done in constant time if the %vector has preallocated space
       *  available.
       */
      void
      push_back(const value_type& __x)
  401e90:	55                   	push   %rbp
  401e91:	48 89 e5             	mov    %rsp,%rbp
  401e94:	48 83 ec 10          	sub    $0x10,%rsp
  401e98:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401e9c:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
      {
	if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
  401ea0:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401ea4:	48 8b 50 08          	mov    0x8(%rax),%rdx
  401ea8:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401eac:	48 8b 40 10          	mov    0x10(%rax),%rax
  401eb0:	48 39 c2             	cmp    %rax,%rdx
  401eb3:	74 31                	je     401ee6 <_ZNSt6vectorI10tagCfgInfoSaIS0_EE9push_backERKS0_+0x56>
	  {
	    _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
  401eb5:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401eb9:	48 8b 48 08          	mov    0x8(%rax),%rcx
  401ebd:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401ec1:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
  401ec5:	48 89 ce             	mov    %rcx,%rsi
  401ec8:	48 89 c7             	mov    %rax,%rdi
  401ecb:	e8 a8 02 00 00       	callq  402178 <_ZN9__gnu_cxx14__alloc_traitsISaI10tagCfgInfoEE9constructIS1_EEvRS2_PS1_RKT_>
	                             __x);
	    ++this->_M_impl._M_finish;
  401ed0:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401ed4:	48 8b 40 08          	mov    0x8(%rax),%rax
  401ed8:	48 8d 50 40          	lea    0x40(%rax),%rdx
  401edc:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401ee0:	48 89 50 08          	mov    %rdx,0x8(%rax)
#if __cplusplus >= 201103L
	  _M_emplace_back_aux(__x);
#else
	  _M_insert_aux(end(), __x);
#endif
      }
  401ee4:	eb 22                	jmp    401f08 <_ZNSt6vectorI10tagCfgInfoSaIS0_EE9push_backERKS0_+0x78>
	  }
	else
#if __cplusplus >= 201103L
	  _M_emplace_back_aux(__x);
#else
	  _M_insert_aux(end(), __x);
  401ee6:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401eea:	48 89 c7             	mov    %rax,%rdi
  401eed:	e8 a8 00 00 00       	callq  401f9a <_ZNSt6vectorI10tagCfgInfoSaIS0_EE3endEv>
  401ef2:	48 89 c1             	mov    %rax,%rcx
  401ef5:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
  401ef9:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401efd:	48 89 ce             	mov    %rcx,%rsi
  401f00:	48 89 c7             	mov    %rax,%rdi
  401f03:	e8 e2 02 00 00       	callq  4021ea <_ZNSt6vectorI10tagCfgInfoSaIS0_EE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPS0_S2_EERKS0_>
#endif
      }
  401f08:	90                   	nop
  401f09:	c9                   	leaveq 
  401f0a:	c3                   	retq   
  401f0b:	90                   	nop

0000000000401f0c <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE4sizeEv>:
#endif

      // [23.2.4.2] capacity
      /**  Returns the number of elements in the %vector.  */
      size_type
      size() const _GLIBCXX_NOEXCEPT
  401f0c:	55                   	push   %rbp
  401f0d:	48 89 e5             	mov    %rsp,%rbp
  401f10:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
      { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
  401f14:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401f18:	48 8b 40 08          	mov    0x8(%rax),%rax
  401f1c:	48 89 c2             	mov    %rax,%rdx
  401f1f:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401f23:	48 8b 00             	mov    (%rax),%rax
  401f26:	48 29 c2             	sub    %rax,%rdx
  401f29:	48 89 d0             	mov    %rdx,%rax
  401f2c:	48 c1 f8 06          	sar    $0x6,%rax
  401f30:	5d                   	pop    %rbp
  401f31:	c3                   	retq   

0000000000401f32 <_ZNSt6vectorI10tagCfgInfoSaIS0_EEixEm>:
       *  Note that data access with this operator is unchecked and
       *  out_of_range lookups are not defined. (For checked lookups
       *  see at().)
       */
      reference
      operator[](size_type __n) _GLIBCXX_NOEXCEPT
  401f32:	55                   	push   %rbp
  401f33:	48 89 e5             	mov    %rsp,%rbp
  401f36:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401f3a:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
      { return *(this->_M_impl._M_start + __n); }
  401f3e:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401f42:	48 8b 00             	mov    (%rax),%rax
  401f45:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
  401f49:	48 c1 e2 06          	shl    $0x6,%rdx
  401f4d:	48 01 d0             	add    %rdx,%rax
  401f50:	5d                   	pop    %rbp
  401f51:	c3                   	retq   

0000000000401f52 <_ZNSt6vectorI10tagCfgInfoSaIS0_EE5beginEv>:
       *  Returns a read/write iterator that points to the first
       *  element in the %vector.  Iteration is done in ordinary
       *  element order.
       */
      iterator
      begin() _GLIBCXX_NOEXCEPT
  401f52:	55                   	push   %rbp
  401f53:	48 89 e5             	mov    %rsp,%rbp
  401f56:	48 83 ec 20          	sub    $0x20,%rsp
  401f5a:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  401f5e:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401f65:	00 00 
  401f67:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
  401f6b:	31 c0                	xor    %eax,%eax
      { return iterator(this->_M_impl._M_start); }
  401f6d:	48 8b 55 e8          	mov    -0x18(%rbp),%rdx
  401f71:	48 8d 45 f0          	lea    -0x10(%rbp),%rax
  401f75:	48 89 d6             	mov    %rdx,%rsi
  401f78:	48 89 c7             	mov    %rax,%rdi
  401f7b:	e8 30 06 00 00       	callq  4025b0 <_ZN9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEEC1ERKS2_>
  401f80:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  401f84:	48 8b 4d f8          	mov    -0x8(%rbp),%rcx
  401f88:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
  401f8f:	00 00 
  401f91:	74 05                	je     401f98 <_ZNSt6vectorI10tagCfgInfoSaIS0_EE5beginEv+0x46>
  401f93:	e8 e8 f1 ff ff       	callq  401180 <__stack_chk_fail@plt>
  401f98:	c9                   	leaveq 
  401f99:	c3                   	retq   

0000000000401f9a <_ZNSt6vectorI10tagCfgInfoSaIS0_EE3endEv>:
       *  Returns a read/write iterator that points one past the last
       *  element in the %vector.  Iteration is done in ordinary
       *  element order.
       */
      iterator
      end() _GLIBCXX_NOEXCEPT
  401f9a:	55                   	push   %rbp
  401f9b:	48 89 e5             	mov    %rsp,%rbp
  401f9e:	48 83 ec 20          	sub    $0x20,%rsp
  401fa2:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  401fa6:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401fad:	00 00 
  401faf:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
  401fb3:	31 c0                	xor    %eax,%eax
      { return iterator(this->_M_impl._M_finish); }
  401fb5:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401fb9:	48 8d 50 08          	lea    0x8(%rax),%rdx
  401fbd:	48 8d 45 f0          	lea    -0x10(%rbp),%rax
  401fc1:	48 89 d6             	mov    %rdx,%rsi
  401fc4:	48 89 c7             	mov    %rax,%rdi
  401fc7:	e8 e4 05 00 00       	callq  4025b0 <_ZN9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEEC1ERKS2_>
  401fcc:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  401fd0:	48 8b 4d f8          	mov    -0x8(%rbp),%rcx
  401fd4:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
  401fdb:	00 00 
  401fdd:	74 05                	je     401fe4 <_ZNSt6vectorI10tagCfgInfoSaIS0_EE3endEv+0x4a>
  401fdf:	e8 9c f1 ff ff       	callq  401180 <__stack_chk_fail@plt>
  401fe4:	c9                   	leaveq 
  401fe5:	c3                   	retq   

0000000000401fe6 <_ZN9__gnu_cxxneIP10tagCfgInfoSt6vectorIS1_SaIS1_EEEEbRKNS_17__normal_iteratorIT_T0_EESB_>:
    _GLIBCXX_NOEXCEPT
    { return __lhs.base() != __rhs.base(); }

  template<typename _Iterator, typename _Container>
    inline bool
    operator!=(const __normal_iterator<_Iterator, _Container>& __lhs,
  401fe6:	55                   	push   %rbp
  401fe7:	48 89 e5             	mov    %rsp,%rbp
  401fea:	53                   	push   %rbx
  401feb:	48 83 ec 18          	sub    $0x18,%rsp
  401fef:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  401ff3:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
	       const __normal_iterator<_Iterator, _Container>& __rhs)
    _GLIBCXX_NOEXCEPT
    { return __lhs.base() != __rhs.base(); }
  401ff7:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  401ffb:	48 89 c7             	mov    %rax,%rdi
  401ffe:	e8 cb 05 00 00       	callq  4025ce <_ZNK9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEE4baseEv>
  402003:	48 8b 18             	mov    (%rax),%rbx
  402006:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
  40200a:	48 89 c7             	mov    %rax,%rdi
  40200d:	e8 bc 05 00 00       	callq  4025ce <_ZNK9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEE4baseEv>
  402012:	48 8b 00             	mov    (%rax),%rax
  402015:	48 39 c3             	cmp    %rax,%rbx
  402018:	0f 95 c0             	setne  %al
  40201b:	48 83 c4 18          	add    $0x18,%rsp
  40201f:	5b                   	pop    %rbx
  402020:	5d                   	pop    %rbp
  402021:	c3                   	retq   

0000000000402022 <_ZN9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEEppEi>:
	++_M_current;
	return *this;
      }

      __normal_iterator
      operator++(int) _GLIBCXX_NOEXCEPT
  402022:	55                   	push   %rbp
  402023:	48 89 e5             	mov    %rsp,%rbp
  402026:	48 83 ec 30          	sub    $0x30,%rsp
  40202a:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
  40202e:	89 75 d4             	mov    %esi,-0x2c(%rbp)
  402031:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  402038:	00 00 
  40203a:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
  40203e:	31 c0                	xor    %eax,%eax
      { return __normal_iterator(_M_current++); }
  402040:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
  402044:	48 8b 00             	mov    (%rax),%rax
  402047:	48 8d 48 40          	lea    0x40(%rax),%rcx
  40204b:	48 8b 55 d8          	mov    -0x28(%rbp),%rdx
  40204f:	48 89 0a             	mov    %rcx,(%rdx)
  402052:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
  402056:	48 8d 55 f0          	lea    -0x10(%rbp),%rdx
  40205a:	48 8d 45 e0          	lea    -0x20(%rbp),%rax
  40205e:	48 89 d6             	mov    %rdx,%rsi
  402061:	48 89 c7             	mov    %rax,%rdi
  402064:	e8 47 05 00 00       	callq  4025b0 <_ZN9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEEC1ERKS2_>
  402069:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
  40206d:	48 8b 4d f8          	mov    -0x8(%rbp),%rcx
  402071:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
  402078:	00 00 
  40207a:	74 05                	je     402081 <_ZN9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEEppEi+0x5f>
  40207c:	e8 ff f0 ff ff       	callq  401180 <__stack_chk_fail@plt>
  402081:	c9                   	leaveq 
  402082:	c3                   	retq   
  402083:	90                   	nop

0000000000402084 <_ZNK9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEEdeEv>:
		      _Container>::__type>& __i) _GLIBCXX_NOEXCEPT
        : _M_current(__i.base()) { }

      // Forward iterator requirements
      reference
      operator*() const _GLIBCXX_NOEXCEPT
  402084:	55                   	push   %rbp
  402085:	48 89 e5             	mov    %rsp,%rbp
  402088:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
      { return *_M_current; }
  40208c:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402090:	48 8b 00             	mov    (%rax),%rax
  402093:	5d                   	pop    %rbp
  402094:	c3                   	retq   
  402095:	90                   	nop

0000000000402096 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE12_Vector_implD1Ev>:
      typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template
        rebind<_Tp>::other _Tp_alloc_type;
      typedef typename __gnu_cxx::__alloc_traits<_Tp_alloc_type>::pointer
       	pointer;

      struct _Vector_impl 
  402096:	55                   	push   %rbp
  402097:	48 89 e5             	mov    %rsp,%rbp
  40209a:	48 83 ec 10          	sub    $0x10,%rsp
  40209e:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4020a2:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4020a6:	48 89 c7             	mov    %rax,%rdi
  4020a9:	e8 6c 05 00 00       	callq  40261a <_ZNSaI10tagCfgInfoED1Ev>
  4020ae:	90                   	nop
  4020af:	c9                   	leaveq 
  4020b0:	c3                   	retq   
  4020b1:	90                   	nop

00000000004020b2 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EEC1Ev>:

      allocator_type
      get_allocator() const _GLIBCXX_NOEXCEPT
      { return allocator_type(_M_get_Tp_allocator()); }

      _Vector_base()
  4020b2:	55                   	push   %rbp
  4020b3:	48 89 e5             	mov    %rsp,%rbp
  4020b6:	48 83 ec 10          	sub    $0x10,%rsp
  4020ba:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
      : _M_impl() { }
  4020be:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4020c2:	48 89 c7             	mov    %rax,%rdi
  4020c5:	e8 12 05 00 00       	callq  4025dc <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE12_Vector_implC1Ev>
  4020ca:	90                   	nop
  4020cb:	c9                   	leaveq 
  4020cc:	c3                   	retq   
  4020cd:	90                   	nop

00000000004020ce <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EED1Ev>:
	    _M_create_storage(__n);
	  }
      }
#endif

      ~_Vector_base() _GLIBCXX_NOEXCEPT
  4020ce:	55                   	push   %rbp
  4020cf:	48 89 e5             	mov    %rsp,%rbp
  4020d2:	53                   	push   %rbx
  4020d3:	48 83 ec 18          	sub    $0x18,%rsp
  4020d7:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
      { _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage
		      - this->_M_impl._M_start); }
  4020db:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4020df:	48 8b 40 10          	mov    0x10(%rax),%rax
	  }
      }
#endif

      ~_Vector_base() _GLIBCXX_NOEXCEPT
      { _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage
  4020e3:	48 89 c2             	mov    %rax,%rdx
		      - this->_M_impl._M_start); }
  4020e6:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4020ea:	48 8b 00             	mov    (%rax),%rax
	  }
      }
#endif

      ~_Vector_base() _GLIBCXX_NOEXCEPT
      { _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage
  4020ed:	48 29 c2             	sub    %rax,%rdx
  4020f0:	48 89 d0             	mov    %rdx,%rax
  4020f3:	48 c1 f8 06          	sar    $0x6,%rax
  4020f7:	48 89 c2             	mov    %rax,%rdx
  4020fa:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4020fe:	48 8b 08             	mov    (%rax),%rcx
  402101:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  402105:	48 89 ce             	mov    %rcx,%rsi
  402108:	48 89 c7             	mov    %rax,%rdi
  40210b:	e8 26 05 00 00       	callq  402636 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE13_M_deallocateEPS0_m>
		      - this->_M_impl._M_start); }
  402110:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  402114:	48 89 c7             	mov    %rax,%rdi
  402117:	e8 7a ff ff ff       	callq  402096 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE12_Vector_implD1Ev>
  40211c:	eb 1a                	jmp    402138 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EED1Ev+0x6a>
  40211e:	48 89 c3             	mov    %rax,%rbx
  402121:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  402125:	48 89 c7             	mov    %rax,%rdi
  402128:	e8 69 ff ff ff       	callq  402096 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE12_Vector_implD1Ev>
  40212d:	48 89 d8             	mov    %rbx,%rax
  402130:	48 89 c7             	mov    %rax,%rdi
  402133:	e8 38 f1 ff ff       	callq  401270 <_Unwind_Resume@plt>
  402138:	48 83 c4 18          	add    $0x18,%rsp
  40213c:	5b                   	pop    %rbx
  40213d:	5d                   	pop    %rbp
  40213e:	c3                   	retq   
  40213f:	90                   	nop

0000000000402140 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE19_M_get_Tp_allocatorEv>:
      
    public:
      typedef _Alloc allocator_type;

      _Tp_alloc_type&
      _M_get_Tp_allocator() _GLIBCXX_NOEXCEPT
  402140:	55                   	push   %rbp
  402141:	48 89 e5             	mov    %rsp,%rbp
  402144:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
      { return *static_cast<_Tp_alloc_type*>(&this->_M_impl); }
  402148:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40214c:	5d                   	pop    %rbp
  40214d:	c3                   	retq   

000000000040214e <_ZSt8_DestroyIP10tagCfgInfoS0_EvT_S2_RSaIT0_E>:
	__traits::destroy(__alloc, std::__addressof(*__first));
    }

  template<typename _ForwardIterator, typename _Tp>
    inline void
    _Destroy(_ForwardIterator __first, _ForwardIterator __last,
  40214e:	55                   	push   %rbp
  40214f:	48 89 e5             	mov    %rsp,%rbp
  402152:	48 83 ec 20          	sub    $0x20,%rsp
  402156:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40215a:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  40215e:	48 89 55 e8          	mov    %rdx,-0x18(%rbp)
	     allocator<_Tp>&)
    {
      _Destroy(__first, __last);
  402162:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
  402166:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40216a:	48 89 d6             	mov    %rdx,%rsi
  40216d:	48 89 c7             	mov    %rax,%rdi
  402170:	e8 f6 04 00 00       	callq  40266b <_ZSt8_DestroyIP10tagCfgInfoEvT_S2_>
    }
  402175:	90                   	nop
  402176:	c9                   	leaveq 
  402177:	c3                   	retq   

0000000000402178 <_ZN9__gnu_cxx14__alloc_traitsISaI10tagCfgInfoEE9constructIS1_EEvRS2_PS1_RKT_>:

    static void deallocate(_Alloc& __a, pointer __p, size_type __n)
    { __a.deallocate(__p, __n); }

    template<typename _Tp>
      static void construct(_Alloc& __a, pointer __p, const _Tp& __arg)
  402178:	55                   	push   %rbp
  402179:	48 89 e5             	mov    %rsp,%rbp
  40217c:	48 83 ec 20          	sub    $0x20,%rsp
  402180:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  402184:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  402188:	48 89 55 e8          	mov    %rdx,-0x18(%rbp)
      { __a.construct(__p, __arg); }
  40218c:	48 8b 55 e8          	mov    -0x18(%rbp),%rdx
  402190:	48 8b 4d f0          	mov    -0x10(%rbp),%rcx
  402194:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402198:	48 89 ce             	mov    %rcx,%rsi
  40219b:	48 89 c7             	mov    %rax,%rdi
  40219e:	e8 ef 04 00 00       	callq  402692 <_ZN9__gnu_cxx13new_allocatorI10tagCfgInfoE9constructEPS1_RKS1_>
  4021a3:	90                   	nop
  4021a4:	c9                   	leaveq 
  4021a5:	c3                   	retq   

00000000004021a6 <_ZN10tagCfgInfoaSERKS_>:
  4021a6:	55                   	push   %rbp
  4021a7:	48 89 e5             	mov    %rsp,%rbp
  4021aa:	48 83 ec 10          	sub    $0x10,%rsp
  4021ae:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4021b2:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  4021b6:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
  4021ba:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4021be:	48 89 d6             	mov    %rdx,%rsi
  4021c1:	48 89 c7             	mov    %rax,%rdi
  4021c4:	e8 f7 ef ff ff       	callq  4011c0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEaSERKS4_@plt>
  4021c9:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  4021cd:	48 8d 50 20          	lea    0x20(%rax),%rdx
  4021d1:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4021d5:	48 83 c0 20          	add    $0x20,%rax
  4021d9:	48 89 d6             	mov    %rdx,%rsi
  4021dc:	48 89 c7             	mov    %rax,%rdi
  4021df:	e8 dc ef ff ff       	callq  4011c0 <_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEaSERKS4_@plt>
  4021e4:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4021e8:	c9                   	leaveq 
  4021e9:	c3                   	retq   

00000000004021ea <_ZNSt6vectorI10tagCfgInfoSaIS0_EE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPS0_S2_EERKS0_>:
      vector<_Tp, _Alloc>::
      _M_insert_aux(iterator __position, _Args&&... __args)
#else
  template<typename _Tp, typename _Alloc>
    void
    vector<_Tp, _Alloc>::
  4021ea:	55                   	push   %rbp
  4021eb:	48 89 e5             	mov    %rsp,%rbp
  4021ee:	41 54                	push   %r12
  4021f0:	53                   	push   %rbx
  4021f1:	48 81 ec a0 00 00 00 	sub    $0xa0,%rsp
  4021f8:	48 89 bd 68 ff ff ff 	mov    %rdi,-0x98(%rbp)
  4021ff:	48 89 b5 60 ff ff ff 	mov    %rsi,-0xa0(%rbp)
  402206:	48 89 95 58 ff ff ff 	mov    %rdx,-0xa8(%rbp)
  40220d:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  402214:	00 00 
  402216:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
  40221a:	31 c0                	xor    %eax,%eax
    _M_insert_aux(iterator __position, const _Tp& __x)
#endif
    {
      if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
  40221c:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  402223:	48 8b 50 08          	mov    0x8(%rax),%rdx
  402227:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  40222e:	48 8b 40 10          	mov    0x10(%rax),%rax
  402232:	48 39 c2             	cmp    %rax,%rdx
  402235:	0f 84 cc 00 00 00    	je     402307 <_ZNSt6vectorI10tagCfgInfoSaIS0_EE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPS0_S2_EERKS0_+0x11d>
	{
	  _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
			           _GLIBCXX_MOVE(*(this->_M_impl._M_finish
  40223b:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  402242:	48 8b 40 08          	mov    0x8(%rax),%rax
    _M_insert_aux(iterator __position, const _Tp& __x)
#endif
    {
      if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
	{
	  _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
  402246:	48 8d 50 c0          	lea    -0x40(%rax),%rdx
  40224a:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  402251:	48 8b 48 08          	mov    0x8(%rax),%rcx
  402255:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  40225c:	48 89 ce             	mov    %rcx,%rsi
  40225f:	48 89 c7             	mov    %rax,%rdi
  402262:	e8 11 ff ff ff       	callq  402178 <_ZN9__gnu_cxx14__alloc_traitsISaI10tagCfgInfoEE9constructIS1_EEvRS2_PS1_RKT_>
			           _GLIBCXX_MOVE(*(this->_M_impl._M_finish
				                   - 1)));
	  ++this->_M_impl._M_finish;
  402267:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  40226e:	48 8b 40 08          	mov    0x8(%rax),%rax
  402272:	48 8d 50 40          	lea    0x40(%rax),%rdx
  402276:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  40227d:	48 89 50 08          	mov    %rdx,0x8(%rax)
#if __cplusplus < 201103L
	  _Tp __x_copy = __x;
  402281:	48 8b 95 58 ff ff ff 	mov    -0xa8(%rbp),%rdx
  402288:	48 8d 45 a0          	lea    -0x60(%rbp),%rax
  40228c:	48 89 d6             	mov    %rdx,%rsi
  40228f:	48 89 c7             	mov    %rax,%rdi
  402292:	e8 15 fb ff ff       	callq  401dac <_ZN10tagCfgInfoC1ERKS_>
#endif
	  _GLIBCXX_MOVE_BACKWARD3(__position.base(),
  402297:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  40229e:	48 8b 40 08          	mov    0x8(%rax),%rax
  4022a2:	4c 8d 60 c0          	lea    -0x40(%rax),%r12
  4022a6:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  4022ad:	48 8b 40 08          	mov    0x8(%rax),%rax
  4022b1:	48 8d 58 80          	lea    -0x80(%rax),%rbx
  4022b5:	48 8d 85 60 ff ff ff 	lea    -0xa0(%rbp),%rax
  4022bc:	48 89 c7             	mov    %rax,%rdi
  4022bf:	e8 0a 03 00 00       	callq  4025ce <_ZNK9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEE4baseEv>
  4022c4:	48 8b 00             	mov    (%rax),%rax
  4022c7:	4c 89 e2             	mov    %r12,%rdx
  4022ca:	48 89 de             	mov    %rbx,%rsi
  4022cd:	48 89 c7             	mov    %rax,%rdi
  4022d0:	e8 25 04 00 00       	callq  4026fa <_ZSt13copy_backwardIP10tagCfgInfoS1_ET0_T_S3_S2_>
				  this->_M_impl._M_finish - 2,
				  this->_M_impl._M_finish - 1);
#if __cplusplus < 201103L
	  *__position = __x_copy;
  4022d5:	48 8d 85 60 ff ff ff 	lea    -0xa0(%rbp),%rax
  4022dc:	48 89 c7             	mov    %rax,%rdi
  4022df:	e8 a0 fd ff ff       	callq  402084 <_ZNK9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEEdeEv>
  4022e4:	48 89 c2             	mov    %rax,%rdx
  4022e7:	48 8d 45 a0          	lea    -0x60(%rbp),%rax
  4022eb:	48 89 c6             	mov    %rax,%rsi
  4022ee:	48 89 d7             	mov    %rdx,%rdi
  4022f1:	e8 b0 fe ff ff       	callq  4021a6 <_ZN10tagCfgInfoaSERKS_>
	  _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
			           _GLIBCXX_MOVE(*(this->_M_impl._M_finish
				                   - 1)));
	  ++this->_M_impl._M_finish;
#if __cplusplus < 201103L
	  _Tp __x_copy = __x;
  4022f6:	48 8d 45 a0          	lea    -0x60(%rbp),%rax
  4022fa:	48 89 c7             	mov    %rax,%rdi
  4022fd:	e8 5e fa ff ff       	callq  401d60 <_ZN10tagCfgInfoD1Ev>
			- this->_M_impl._M_start);
	  this->_M_impl._M_start = __new_start;
	  this->_M_impl._M_finish = __new_finish;
	  this->_M_impl._M_end_of_storage = __new_start + __len;
	}
    }
  402302:	e9 89 02 00 00       	jmpq   402590 <_ZNSt6vectorI10tagCfgInfoSaIS0_EE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPS0_S2_EERKS0_+0x3a6>
#endif
	}
      else
	{
	  const size_type __len =
	    _M_check_len(size_type(1), "vector::_M_insert_aux");
  402307:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  40230e:	ba b5 30 40 00       	mov    $0x4030b5,%edx
  402313:	be 01 00 00 00       	mov    $0x1,%esi
  402318:	48 89 c7             	mov    %rax,%rdi
  40231b:	e8 26 04 00 00       	callq  402746 <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE12_M_check_lenEmPKc>
  402320:	48 89 45 88          	mov    %rax,-0x78(%rbp)
	  const size_type __elems_before = __position - begin();
  402324:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  40232b:	48 89 c7             	mov    %rax,%rdi
  40232e:	e8 1f fc ff ff       	callq  401f52 <_ZNSt6vectorI10tagCfgInfoSaIS0_EE5beginEv>
  402333:	48 89 85 70 ff ff ff 	mov    %rax,-0x90(%rbp)
  40233a:	48 8d 95 70 ff ff ff 	lea    -0x90(%rbp),%rdx
  402341:	48 8d 85 60 ff ff ff 	lea    -0xa0(%rbp),%rax
  402348:	48 89 d6             	mov    %rdx,%rsi
  40234b:	48 89 c7             	mov    %rax,%rdi
  40234e:	e8 df 04 00 00       	callq  402832 <_ZN9__gnu_cxxmiIP10tagCfgInfoSt6vectorIS1_SaIS1_EEEENS_17__normal_iteratorIT_T0_E15difference_typeERKS9_SC_>
  402353:	48 89 45 90          	mov    %rax,-0x70(%rbp)
	  pointer __new_start(this->_M_allocate(__len));
  402357:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  40235e:	48 8b 55 88          	mov    -0x78(%rbp),%rdx
  402362:	48 89 d6             	mov    %rdx,%rsi
  402365:	48 89 c7             	mov    %rax,%rdi
  402368:	e8 09 05 00 00       	callq  402876 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE11_M_allocateEm>
  40236d:	48 89 45 98          	mov    %rax,-0x68(%rbp)
	  pointer __new_finish(__new_start);
  402371:	48 8b 45 98          	mov    -0x68(%rbp),%rax
  402375:	48 89 45 80          	mov    %rax,-0x80(%rbp)
	    {
	      // The order of the three operations is dictated by the C++0x
	      // case, where the moves could alter a new element belonging
	      // to the existing vector.  This is an issue only for callers
	      // taking the element by const lvalue ref (see 23.1/13).
	      _Alloc_traits::construct(this->_M_impl,
  402379:	48 8b 45 90          	mov    -0x70(%rbp),%rax
  40237d:	48 c1 e0 06          	shl    $0x6,%rax
  402381:	48 89 c2             	mov    %rax,%rdx
  402384:	48 8b 45 98          	mov    -0x68(%rbp),%rax
  402388:	48 8d 0c 02          	lea    (%rdx,%rax,1),%rcx
  40238c:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  402393:	48 8b 95 58 ff ff ff 	mov    -0xa8(%rbp),%rdx
  40239a:	48 89 ce             	mov    %rcx,%rsi
  40239d:	48 89 c7             	mov    %rax,%rdi
  4023a0:	e8 d3 fd ff ff       	callq  402178 <_ZN9__gnu_cxx14__alloc_traitsISaI10tagCfgInfoEE9constructIS1_EEvRS2_PS1_RKT_>
#if __cplusplus >= 201103L
				       std::forward<_Args>(__args)...);
#else
	                               __x);
#endif
	      __new_finish = pointer();
  4023a5:	48 c7 45 80 00 00 00 	movq   $0x0,-0x80(%rbp)
  4023ac:	00 

	      __new_finish
		= std::__uninitialized_move_if_noexcept_a
		(this->_M_impl._M_start, __position.base(),
		 __new_start, _M_get_Tp_allocator());
  4023ad:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  4023b4:	48 89 c7             	mov    %rax,%rdi
  4023b7:	e8 84 fd ff ff       	callq  402140 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE19_M_get_Tp_allocatorEv>
  4023bc:	48 89 c3             	mov    %rax,%rbx
#endif
	      __new_finish = pointer();

	      __new_finish
		= std::__uninitialized_move_if_noexcept_a
		(this->_M_impl._M_start, __position.base(),
  4023bf:	48 8d 85 60 ff ff ff 	lea    -0xa0(%rbp),%rax
  4023c6:	48 89 c7             	mov    %rax,%rdi
  4023c9:	e8 00 02 00 00       	callq  4025ce <_ZNK9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEE4baseEv>
  4023ce:	48 8b 30             	mov    (%rax),%rsi
  4023d1:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  4023d8:	48 8b 00             	mov    (%rax),%rax
#else
	                               __x);
#endif
	      __new_finish = pointer();

	      __new_finish
  4023db:	48 8b 55 98          	mov    -0x68(%rbp),%rdx
  4023df:	48 89 d9             	mov    %rbx,%rcx
  4023e2:	48 89 c7             	mov    %rax,%rdi
  4023e5:	e8 bf 04 00 00       	callq  4028a9 <_ZSt34__uninitialized_move_if_noexcept_aIP10tagCfgInfoS1_SaIS0_EET0_T_S4_S3_RT1_>
  4023ea:	48 89 45 80          	mov    %rax,-0x80(%rbp)
		= std::__uninitialized_move_if_noexcept_a
		(this->_M_impl._M_start, __position.base(),
		 __new_start, _M_get_Tp_allocator());

	      ++__new_finish;
  4023ee:	48 83 45 80 40       	addq   $0x40,-0x80(%rbp)

	      __new_finish
		= std::__uninitialized_move_if_noexcept_a
		(__position.base(), this->_M_impl._M_finish,
		 __new_finish, _M_get_Tp_allocator());
  4023f3:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  4023fa:	48 89 c7             	mov    %rax,%rdi
  4023fd:	e8 3e fd ff ff       	callq  402140 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE19_M_get_Tp_allocatorEv>
  402402:	49 89 c4             	mov    %rax,%r12

	      ++__new_finish;

	      __new_finish
		= std::__uninitialized_move_if_noexcept_a
		(__position.base(), this->_M_impl._M_finish,
  402405:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  40240c:	48 8b 58 08          	mov    0x8(%rax),%rbx
  402410:	48 8d 85 60 ff ff ff 	lea    -0xa0(%rbp),%rax
  402417:	48 89 c7             	mov    %rax,%rdi
  40241a:	e8 af 01 00 00       	callq  4025ce <_ZNK9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEE4baseEv>
  40241f:	48 8b 00             	mov    (%rax),%rax
		(this->_M_impl._M_start, __position.base(),
		 __new_start, _M_get_Tp_allocator());

	      ++__new_finish;

	      __new_finish
  402422:	48 8b 55 80          	mov    -0x80(%rbp),%rdx
  402426:	4c 89 e1             	mov    %r12,%rcx
  402429:	48 89 de             	mov    %rbx,%rsi
  40242c:	48 89 c7             	mov    %rax,%rdi
  40242f:	e8 75 04 00 00       	callq  4028a9 <_ZSt34__uninitialized_move_if_noexcept_aIP10tagCfgInfoS1_SaIS0_EET0_T_S4_S3_RT1_>
  402434:	48 89 45 80          	mov    %rax,-0x80(%rbp)
		std::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
	      _M_deallocate(__new_start, __len);
	      __throw_exception_again;
	    }
	  std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
			_M_get_Tp_allocator());
  402438:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  40243f:	48 89 c7             	mov    %rax,%rdi
  402442:	e8 f9 fc ff ff       	callq  402140 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE19_M_get_Tp_allocatorEv>
  402447:	48 89 c2             	mov    %rax,%rdx
	      else
		std::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
	      _M_deallocate(__new_start, __len);
	      __throw_exception_again;
	    }
	  std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
  40244a:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  402451:	48 8b 48 08          	mov    0x8(%rax),%rcx
  402455:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  40245c:	48 8b 00             	mov    (%rax),%rax
  40245f:	48 89 ce             	mov    %rcx,%rsi
  402462:	48 89 c7             	mov    %rax,%rdi
  402465:	e8 e4 fc ff ff       	callq  40214e <_ZSt8_DestroyIP10tagCfgInfoS0_EvT_S2_RSaIT0_E>
			_M_get_Tp_allocator());
	  _M_deallocate(this->_M_impl._M_start,
			this->_M_impl._M_end_of_storage
			- this->_M_impl._M_start);
  40246a:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  402471:	48 8b 40 10          	mov    0x10(%rax),%rax
	      _M_deallocate(__new_start, __len);
	      __throw_exception_again;
	    }
	  std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
			_M_get_Tp_allocator());
	  _M_deallocate(this->_M_impl._M_start,
  402475:	48 89 c2             	mov    %rax,%rdx
			this->_M_impl._M_end_of_storage
			- this->_M_impl._M_start);
  402478:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  40247f:	48 8b 00             	mov    (%rax),%rax
	      _M_deallocate(__new_start, __len);
	      __throw_exception_again;
	    }
	  std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
			_M_get_Tp_allocator());
	  _M_deallocate(this->_M_impl._M_start,
  402482:	48 29 c2             	sub    %rax,%rdx
  402485:	48 89 d0             	mov    %rdx,%rax
  402488:	48 c1 f8 06          	sar    $0x6,%rax
  40248c:	48 89 c2             	mov    %rax,%rdx
  40248f:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  402496:	48 8b 08             	mov    (%rax),%rcx
  402499:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  4024a0:	48 89 ce             	mov    %rcx,%rsi
  4024a3:	48 89 c7             	mov    %rax,%rdi
  4024a6:	e8 8b 01 00 00       	callq  402636 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE13_M_deallocateEPS0_m>
			this->_M_impl._M_end_of_storage
			- this->_M_impl._M_start);
	  this->_M_impl._M_start = __new_start;
  4024ab:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  4024b2:	48 8b 55 98          	mov    -0x68(%rbp),%rdx
  4024b6:	48 89 10             	mov    %rdx,(%rax)
	  this->_M_impl._M_finish = __new_finish;
  4024b9:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  4024c0:	48 8b 55 80          	mov    -0x80(%rbp),%rdx
  4024c4:	48 89 50 08          	mov    %rdx,0x8(%rax)
	  this->_M_impl._M_end_of_storage = __new_start + __len;
  4024c8:	48 8b 45 88          	mov    -0x78(%rbp),%rax
  4024cc:	48 c1 e0 06          	shl    $0x6,%rax
  4024d0:	48 89 c2             	mov    %rax,%rdx
  4024d3:	48 8b 45 98          	mov    -0x68(%rbp),%rax
  4024d7:	48 01 c2             	add    %rax,%rdx
  4024da:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  4024e1:	48 89 50 10          	mov    %rdx,0x10(%rax)
	}
    }
  4024e5:	e9 a6 00 00 00       	jmpq   402590 <_ZNSt6vectorI10tagCfgInfoSaIS0_EE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPS0_S2_EERKS0_+0x3a6>
  4024ea:	48 89 c3             	mov    %rax,%rbx
	  _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
			           _GLIBCXX_MOVE(*(this->_M_impl._M_finish
				                   - 1)));
	  ++this->_M_impl._M_finish;
#if __cplusplus < 201103L
	  _Tp __x_copy = __x;
  4024ed:	48 8d 45 a0          	lea    -0x60(%rbp),%rax
  4024f1:	48 89 c7             	mov    %rax,%rdi
  4024f4:	e8 67 f8 ff ff       	callq  401d60 <_ZN10tagCfgInfoD1Ev>
  4024f9:	48 89 d8             	mov    %rbx,%rax
  4024fc:	48 89 c7             	mov    %rax,%rdi
  4024ff:	e8 6c ed ff ff       	callq  401270 <_Unwind_Resume@plt>
	      __new_finish
		= std::__uninitialized_move_if_noexcept_a
		(__position.base(), this->_M_impl._M_finish,
		 __new_finish, _M_get_Tp_allocator());
	    }
          __catch(...)
  402504:	48 89 c7             	mov    %rax,%rdi
  402507:	e8 04 ed ff ff       	callq  401210 <__cxa_begin_catch@plt>
	    {
	      if (!__new_finish)
  40250c:	48 83 7d 80 00       	cmpq   $0x0,-0x80(%rbp)
  402511:	75 26                	jne    402539 <_ZNSt6vectorI10tagCfgInfoSaIS0_EE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPS0_S2_EERKS0_+0x34f>
		_Alloc_traits::destroy(this->_M_impl,
  402513:	48 8b 45 90          	mov    -0x70(%rbp),%rax
  402517:	48 c1 e0 06          	shl    $0x6,%rax
  40251b:	48 89 c2             	mov    %rax,%rdx
  40251e:	48 8b 45 98          	mov    -0x68(%rbp),%rax
  402522:	48 01 c2             	add    %rax,%rdx
  402525:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  40252c:	48 89 d6             	mov    %rdx,%rsi
  40252f:	48 89 c7             	mov    %rax,%rdi
  402532:	e8 a4 03 00 00       	callq  4028db <_ZN9__gnu_cxx14__alloc_traitsISaI10tagCfgInfoEE7destroyERS2_PS1_>
  402537:	eb 25                	jmp    40255e <_ZNSt6vectorI10tagCfgInfoSaIS0_EE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPS0_S2_EERKS0_+0x374>
		                       __new_start + __elems_before);
	      else
		std::_Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
  402539:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  402540:	48 89 c7             	mov    %rax,%rdi
  402543:	e8 f8 fb ff ff       	callq  402140 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE19_M_get_Tp_allocatorEv>
  402548:	48 89 c2             	mov    %rax,%rdx
  40254b:	48 8b 4d 80          	mov    -0x80(%rbp),%rcx
  40254f:	48 8b 45 98          	mov    -0x68(%rbp),%rax
  402553:	48 89 ce             	mov    %rcx,%rsi
  402556:	48 89 c7             	mov    %rax,%rdi
  402559:	e8 f0 fb ff ff       	callq  40214e <_ZSt8_DestroyIP10tagCfgInfoS0_EvT_S2_RSaIT0_E>
	      _M_deallocate(__new_start, __len);
  40255e:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
  402565:	48 8b 55 88          	mov    -0x78(%rbp),%rdx
  402569:	48 8b 4d 98          	mov    -0x68(%rbp),%rcx
  40256d:	48 89 ce             	mov    %rcx,%rsi
  402570:	48 89 c7             	mov    %rax,%rdi
  402573:	e8 be 00 00 00       	callq  402636 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE13_M_deallocateEPS0_m>
	      __throw_exception_again;
  402578:	e8 23 eb ff ff       	callq  4010a0 <__cxa_rethrow@plt>
  40257d:	48 89 c3             	mov    %rax,%rbx
	      __new_finish
		= std::__uninitialized_move_if_noexcept_a
		(__position.base(), this->_M_impl._M_finish,
		 __new_finish, _M_get_Tp_allocator());
	    }
          __catch(...)
  402580:	e8 4b ec ff ff       	callq  4011d0 <__cxa_end_catch@plt>
  402585:	48 89 d8             	mov    %rbx,%rax
  402588:	48 89 c7             	mov    %rax,%rdi
  40258b:	e8 e0 ec ff ff       	callq  401270 <_Unwind_Resume@plt>
			- this->_M_impl._M_start);
	  this->_M_impl._M_start = __new_start;
	  this->_M_impl._M_finish = __new_finish;
	  this->_M_impl._M_end_of_storage = __new_start + __len;
	}
    }
  402590:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  402594:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  40259b:	00 00 
  40259d:	74 05                	je     4025a4 <_ZNSt6vectorI10tagCfgInfoSaIS0_EE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPS0_S2_EERKS0_+0x3ba>
  40259f:	e8 dc eb ff ff       	callq  401180 <__stack_chk_fail@plt>
  4025a4:	48 81 c4 a0 00 00 00 	add    $0xa0,%rsp
  4025ab:	5b                   	pop    %rbx
  4025ac:	41 5c                	pop    %r12
  4025ae:	5d                   	pop    %rbp
  4025af:	c3                   	retq   

00000000004025b0 <_ZN9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEEC1ERKS2_>:

      _GLIBCXX_CONSTEXPR __normal_iterator() _GLIBCXX_NOEXCEPT
      : _M_current(_Iterator()) { }

      explicit
      __normal_iterator(const _Iterator& __i) _GLIBCXX_NOEXCEPT
  4025b0:	55                   	push   %rbp
  4025b1:	48 89 e5             	mov    %rsp,%rbp
  4025b4:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4025b8:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
      : _M_current(__i) { }
  4025bc:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  4025c0:	48 8b 10             	mov    (%rax),%rdx
  4025c3:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4025c7:	48 89 10             	mov    %rdx,(%rax)
  4025ca:	90                   	nop
  4025cb:	5d                   	pop    %rbp
  4025cc:	c3                   	retq   
  4025cd:	90                   	nop

00000000004025ce <_ZNK9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEE4baseEv>:
      __normal_iterator
      operator-(difference_type __n) const _GLIBCXX_NOEXCEPT
      { return __normal_iterator(_M_current - __n); }

      const _Iterator&
      base() const _GLIBCXX_NOEXCEPT
  4025ce:	55                   	push   %rbp
  4025cf:	48 89 e5             	mov    %rsp,%rbp
  4025d2:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
      { return _M_current; }
  4025d6:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4025da:	5d                   	pop    %rbp
  4025db:	c3                   	retq   

00000000004025dc <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE12_Vector_implC1Ev>:
      {
	pointer _M_start;
	pointer _M_finish;
	pointer _M_end_of_storage;

	_Vector_impl()
  4025dc:	55                   	push   %rbp
  4025dd:	48 89 e5             	mov    %rsp,%rbp
  4025e0:	48 83 ec 10          	sub    $0x10,%rsp
  4025e4:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
	: _Tp_alloc_type(), _M_start(), _M_finish(), _M_end_of_storage()
  4025e8:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4025ec:	48 89 c7             	mov    %rax,%rdi
  4025ef:	e8 0e 03 00 00       	callq  402902 <_ZNSaI10tagCfgInfoEC1Ev>
  4025f4:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4025f8:	48 c7 00 00 00 00 00 	movq   $0x0,(%rax)
  4025ff:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402603:	48 c7 40 08 00 00 00 	movq   $0x0,0x8(%rax)
  40260a:	00 
  40260b:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40260f:	48 c7 40 10 00 00 00 	movq   $0x0,0x10(%rax)
  402616:	00 
	{ }
  402617:	90                   	nop
  402618:	c9                   	leaveq 
  402619:	c3                   	retq   

000000000040261a <_ZNSaI10tagCfgInfoED1Ev>:
      : __allocator_base<_Tp>(__a) { }

      template<typename _Tp1>
        allocator(const allocator<_Tp1>&) throw() { }

      ~allocator() throw() { }
  40261a:	55                   	push   %rbp
  40261b:	48 89 e5             	mov    %rsp,%rbp
  40261e:	48 83 ec 10          	sub    $0x10,%rsp
  402622:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  402626:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40262a:	48 89 c7             	mov    %rax,%rdi
  40262d:	e8 ec 02 00 00       	callq  40291e <_ZN9__gnu_cxx13new_allocatorI10tagCfgInfoED1Ev>
  402632:	90                   	nop
  402633:	c9                   	leaveq 
  402634:	c3                   	retq   
  402635:	90                   	nop

0000000000402636 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE13_M_deallocateEPS0_m>:
	typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Tr;
	return __n != 0 ? _Tr::allocate(_M_impl, __n) : pointer();
      }

      void
      _M_deallocate(pointer __p, size_t __n)
  402636:	55                   	push   %rbp
  402637:	48 89 e5             	mov    %rsp,%rbp
  40263a:	48 83 ec 20          	sub    $0x20,%rsp
  40263e:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  402642:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  402646:	48 89 55 e8          	mov    %rdx,-0x18(%rbp)
      {
	typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Tr;
	if (__p)
  40264a:	48 83 7d f0 00       	cmpq   $0x0,-0x10(%rbp)
  40264f:	74 17                	je     402668 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE13_M_deallocateEPS0_m+0x32>
	  _Tr::deallocate(_M_impl, __p, __n);
  402651:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402655:	48 8b 55 e8          	mov    -0x18(%rbp),%rdx
  402659:	48 8b 4d f0          	mov    -0x10(%rbp),%rcx
  40265d:	48 89 ce             	mov    %rcx,%rsi
  402660:	48 89 c7             	mov    %rax,%rdi
  402663:	e8 c1 02 00 00       	callq  402929 <_ZN9__gnu_cxx14__alloc_traitsISaI10tagCfgInfoEE10deallocateERS2_PS1_m>
      }
  402668:	90                   	nop
  402669:	c9                   	leaveq 
  40266a:	c3                   	retq   

000000000040266b <_ZSt8_DestroyIP10tagCfgInfoEvT_S2_>:
   * a trivial destructor, the compiler should optimize all of this
   * away, otherwise the objects' destructors must be invoked.
   */
  template<typename _ForwardIterator>
    inline void
    _Destroy(_ForwardIterator __first, _ForwardIterator __last)
  40266b:	55                   	push   %rbp
  40266c:	48 89 e5             	mov    %rsp,%rbp
  40266f:	48 83 ec 10          	sub    $0x10,%rsp
  402673:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  402677:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
                       _Value_type;
      std::_Destroy_aux<__has_trivial_destructor(_Value_type)>::
  40267b:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
  40267f:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402683:	48 89 d6             	mov    %rdx,%rsi
  402686:	48 89 c7             	mov    %rax,%rdi
  402689:	e8 c9 02 00 00       	callq  402957 <_ZNSt12_Destroy_auxILb0EE9__destroyIP10tagCfgInfoEEvT_S4_>
	__destroy(__first, __last);
    }
  40268e:	90                   	nop
  40268f:	c9                   	leaveq 
  402690:	c3                   	retq   
  402691:	90                   	nop

0000000000402692 <_ZN9__gnu_cxx13new_allocatorI10tagCfgInfoE9constructEPS1_RKS1_>:
        destroy(_Up* __p) { __p->~_Up(); }
#else
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 402. wrong new expression in [some_] allocator::construct
      void 
      construct(pointer __p, const _Tp& __val) 
  402692:	55                   	push   %rbp
  402693:	48 89 e5             	mov    %rsp,%rbp
  402696:	41 55                	push   %r13
  402698:	41 54                	push   %r12
  40269a:	53                   	push   %rbx
  40269b:	48 83 ec 28          	sub    $0x28,%rsp
  40269f:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
  4026a3:	48 89 75 d0          	mov    %rsi,-0x30(%rbp)
  4026a7:	48 89 55 c8          	mov    %rdx,-0x38(%rbp)
      { ::new((void *)__p) _Tp(__val); }
  4026ab:	4c 8b 65 d0          	mov    -0x30(%rbp),%r12
  4026af:	4c 89 e6             	mov    %r12,%rsi
  4026b2:	bf 40 00 00 00       	mov    $0x40,%edi
  4026b7:	e8 37 f6 ff ff       	callq  401cf3 <_ZnwmPv>
  4026bc:	48 89 c3             	mov    %rax,%rbx
  4026bf:	48 85 db             	test   %rbx,%rbx
  4026c2:	74 2a                	je     4026ee <_ZN9__gnu_cxx13new_allocatorI10tagCfgInfoE9constructEPS1_RKS1_+0x5c>
  4026c4:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
  4026c8:	48 89 c6             	mov    %rax,%rsi
  4026cb:	48 89 df             	mov    %rbx,%rdi
  4026ce:	e8 d9 f6 ff ff       	callq  401dac <_ZN10tagCfgInfoC1ERKS_>
  4026d3:	eb 19                	jmp    4026ee <_ZN9__gnu_cxx13new_allocatorI10tagCfgInfoE9constructEPS1_RKS1_+0x5c>
  4026d5:	49 89 c5             	mov    %rax,%r13
  4026d8:	4c 89 e6             	mov    %r12,%rsi
  4026db:	48 89 df             	mov    %rbx,%rdi
  4026de:	e8 22 f6 ff ff       	callq  401d05 <_ZdlPvS_>
  4026e3:	4c 89 e8             	mov    %r13,%rax
  4026e6:	48 89 c7             	mov    %rax,%rdi
  4026e9:	e8 82 eb ff ff       	callq  401270 <_Unwind_Resume@plt>
  4026ee:	90                   	nop
  4026ef:	48 83 c4 28          	add    $0x28,%rsp
  4026f3:	5b                   	pop    %rbx
  4026f4:	41 5c                	pop    %r12
  4026f6:	41 5d                	pop    %r13
  4026f8:	5d                   	pop    %rbp
  4026f9:	c3                   	retq   

00000000004026fa <_ZSt13copy_backwardIP10tagCfgInfoS1_ET0_T_S3_S2_>:
   *  Result may not be in the range (first,last].  Use copy instead.  Note
   *  that the start of the output range may overlap [first,last).
  */
  template<typename _BI1, typename _BI2>
    inline _BI2
    copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
  4026fa:	55                   	push   %rbp
  4026fb:	48 89 e5             	mov    %rsp,%rbp
  4026fe:	53                   	push   %rbx
  4026ff:	48 83 ec 28          	sub    $0x28,%rsp
  402703:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  402707:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
  40270b:	48 89 55 d8          	mov    %rdx,-0x28(%rbp)
	    typename iterator_traits<_BI1>::value_type,
	    typename iterator_traits<_BI2>::value_type>)
      __glibcxx_requires_valid_range(__first, __last);

      return (std::__copy_move_backward_a2<__is_move_iterator<_BI1>::__value>
	      (std::__miter_base(__first), std::__miter_base(__last),
  40270f:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
  402713:	48 89 c7             	mov    %rax,%rdi
  402716:	e8 74 02 00 00       	callq  40298f <_ZSt12__miter_baseIP10tagCfgInfoENSt11_Miter_baseIT_E13iterator_typeES3_>
  40271b:	48 89 c3             	mov    %rax,%rbx
  40271e:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  402722:	48 89 c7             	mov    %rax,%rdi
  402725:	e8 65 02 00 00       	callq  40298f <_ZSt12__miter_baseIP10tagCfgInfoENSt11_Miter_baseIT_E13iterator_typeES3_>
  40272a:	48 89 c1             	mov    %rax,%rcx
	       __result));
  40272d:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
  402731:	48 89 c2             	mov    %rax,%rdx
  402734:	48 89 de             	mov    %rbx,%rsi
  402737:	48 89 cf             	mov    %rcx,%rdi
  40273a:	e8 6a 02 00 00       	callq  4029a9 <_ZSt23__copy_move_backward_a2ILb0EP10tagCfgInfoS1_ET1_T0_S3_S2_>
    }
  40273f:	48 83 c4 28          	add    $0x28,%rsp
  402743:	5b                   	pop    %rbx
  402744:	5d                   	pop    %rbp
  402745:	c3                   	retq   

0000000000402746 <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE12_M_check_lenEmPKc>:
        _M_emplace_back_aux(_Args&&... __args);
#endif

      // Called by the latter.
      size_type
      _M_check_len(size_type __n, const char* __s) const
  402746:	55                   	push   %rbp
  402747:	48 89 e5             	mov    %rsp,%rbp
  40274a:	53                   	push   %rbx
  40274b:	48 83 ec 48          	sub    $0x48,%rsp
  40274f:	48 89 7d c8          	mov    %rdi,-0x38(%rbp)
  402753:	48 89 75 c0          	mov    %rsi,-0x40(%rbp)
  402757:	48 89 55 b8          	mov    %rdx,-0x48(%rbp)
  40275b:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  402762:	00 00 
  402764:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
  402768:	31 c0                	xor    %eax,%eax
      {
	if (max_size() - size() < __n)
  40276a:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
  40276e:	48 89 c7             	mov    %rax,%rdi
  402771:	e8 8c 02 00 00       	callq  402a02 <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE8max_sizeEv>
  402776:	48 89 c3             	mov    %rax,%rbx
  402779:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
  40277d:	48 89 c7             	mov    %rax,%rdi
  402780:	e8 87 f7 ff ff       	callq  401f0c <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE4sizeEv>
  402785:	48 29 c3             	sub    %rax,%rbx
  402788:	48 89 da             	mov    %rbx,%rdx
  40278b:	48 8b 45 c0          	mov    -0x40(%rbp),%rax
  40278f:	48 39 c2             	cmp    %rax,%rdx
  402792:	0f 92 c0             	setb   %al
  402795:	84 c0                	test   %al,%al
  402797:	74 0c                	je     4027a5 <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE12_M_check_lenEmPKc+0x5f>
	  __throw_length_error(__N(__s));
  402799:	48 8b 45 b8          	mov    -0x48(%rbp),%rax
  40279d:	48 89 c7             	mov    %rax,%rdi
  4027a0:	e8 9b e8 ff ff       	callq  401040 <_ZSt20__throw_length_errorPKc@plt>

	const size_type __len = size() + std::max(size(), __n);
  4027a5:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
  4027a9:	48 89 c7             	mov    %rax,%rdi
  4027ac:	e8 5b f7 ff ff       	callq  401f0c <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE4sizeEv>
  4027b1:	48 89 c3             	mov    %rax,%rbx
  4027b4:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
  4027b8:	48 89 c7             	mov    %rax,%rdi
  4027bb:	e8 4c f7 ff ff       	callq  401f0c <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE4sizeEv>
  4027c0:	48 89 45 d8          	mov    %rax,-0x28(%rbp)
  4027c4:	48 8d 55 c0          	lea    -0x40(%rbp),%rdx
  4027c8:	48 8d 45 d8          	lea    -0x28(%rbp),%rax
  4027cc:	48 89 d6             	mov    %rdx,%rsi
  4027cf:	48 89 c7             	mov    %rax,%rdi
  4027d2:	e8 4d 02 00 00       	callq  402a24 <_ZSt3maxImERKT_S2_S2_>
  4027d7:	48 8b 00             	mov    (%rax),%rax
  4027da:	48 01 d8             	add    %rbx,%rax
  4027dd:	48 89 45 e0          	mov    %rax,-0x20(%rbp)
	return (__len < size() || __len > max_size()) ? max_size() : __len;
  4027e1:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
  4027e5:	48 89 c7             	mov    %rax,%rdi
  4027e8:	e8 1f f7 ff ff       	callq  401f0c <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE4sizeEv>
  4027ed:	48 3b 45 e0          	cmp    -0x20(%rbp),%rax
  4027f1:	77 12                	ja     402805 <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE12_M_check_lenEmPKc+0xbf>
  4027f3:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
  4027f7:	48 89 c7             	mov    %rax,%rdi
  4027fa:	e8 03 02 00 00       	callq  402a02 <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE8max_sizeEv>
  4027ff:	48 3b 45 e0          	cmp    -0x20(%rbp),%rax
  402803:	73 0e                	jae    402813 <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE12_M_check_lenEmPKc+0xcd>
  402805:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
  402809:	48 89 c7             	mov    %rax,%rdi
  40280c:	e8 f1 01 00 00       	callq  402a02 <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE8max_sizeEv>
  402811:	eb 04                	jmp    402817 <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE12_M_check_lenEmPKc+0xd1>
  402813:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
      }
  402817:	48 8b 4d e8          	mov    -0x18(%rbp),%rcx
  40281b:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
  402822:	00 00 
  402824:	74 05                	je     40282b <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE12_M_check_lenEmPKc+0xe5>
  402826:	e8 55 e9 ff ff       	callq  401180 <__stack_chk_fail@plt>
  40282b:	48 83 c4 48          	add    $0x48,%rsp
  40282f:	5b                   	pop    %rbx
  402830:	5d                   	pop    %rbp
  402831:	c3                   	retq   

0000000000402832 <_ZN9__gnu_cxxmiIP10tagCfgInfoSt6vectorIS1_SaIS1_EEEENS_17__normal_iteratorIT_T0_E15difference_typeERKS9_SC_>:
#endif
    { return __lhs.base() - __rhs.base(); }

  template<typename _Iterator, typename _Container>
    inline typename __normal_iterator<_Iterator, _Container>::difference_type
    operator-(const __normal_iterator<_Iterator, _Container>& __lhs,
  402832:	55                   	push   %rbp
  402833:	48 89 e5             	mov    %rsp,%rbp
  402836:	53                   	push   %rbx
  402837:	48 83 ec 18          	sub    $0x18,%rsp
  40283b:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  40283f:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
	      const __normal_iterator<_Iterator, _Container>& __rhs)
    _GLIBCXX_NOEXCEPT
    { return __lhs.base() - __rhs.base(); }
  402843:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  402847:	48 89 c7             	mov    %rax,%rdi
  40284a:	e8 7f fd ff ff       	callq  4025ce <_ZNK9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEE4baseEv>
  40284f:	48 8b 00             	mov    (%rax),%rax
  402852:	48 89 c3             	mov    %rax,%rbx
  402855:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
  402859:	48 89 c7             	mov    %rax,%rdi
  40285c:	e8 6d fd ff ff       	callq  4025ce <_ZNK9__gnu_cxx17__normal_iteratorIP10tagCfgInfoSt6vectorIS1_SaIS1_EEE4baseEv>
  402861:	48 8b 00             	mov    (%rax),%rax
  402864:	48 29 c3             	sub    %rax,%rbx
  402867:	48 89 d8             	mov    %rbx,%rax
  40286a:	48 c1 f8 06          	sar    $0x6,%rax
  40286e:	48 83 c4 18          	add    $0x18,%rsp
  402872:	5b                   	pop    %rbx
  402873:	5d                   	pop    %rbp
  402874:	c3                   	retq   
  402875:	90                   	nop

0000000000402876 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE11_M_allocateEm>:

    public:
      _Vector_impl _M_impl;

      pointer
      _M_allocate(size_t __n)
  402876:	55                   	push   %rbp
  402877:	48 89 e5             	mov    %rsp,%rbp
  40287a:	48 83 ec 10          	sub    $0x10,%rsp
  40287e:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  402882:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
      {
	typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Tr;
	return __n != 0 ? _Tr::allocate(_M_impl, __n) : pointer();
  402886:	48 83 7d f0 00       	cmpq   $0x0,-0x10(%rbp)
  40288b:	74 15                	je     4028a2 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE11_M_allocateEm+0x2c>
  40288d:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402891:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
  402895:	48 89 d6             	mov    %rdx,%rsi
  402898:	48 89 c7             	mov    %rax,%rdi
  40289b:	e8 af 01 00 00       	callq  402a4f <_ZN9__gnu_cxx14__alloc_traitsISaI10tagCfgInfoEE8allocateERS2_m>
  4028a0:	eb 05                	jmp    4028a7 <_ZNSt12_Vector_baseI10tagCfgInfoSaIS0_EE11_M_allocateEm+0x31>
  4028a2:	b8 00 00 00 00       	mov    $0x0,%eax
      }
  4028a7:	c9                   	leaveq 
  4028a8:	c3                   	retq   

00000000004028a9 <_ZSt34__uninitialized_move_if_noexcept_aIP10tagCfgInfoS1_SaIS0_EET0_T_S4_S3_RT1_>:
    }

  template<typename _InputIterator, typename _ForwardIterator,
	   typename _Allocator>
    inline _ForwardIterator
    __uninitialized_move_if_noexcept_a(_InputIterator __first,
  4028a9:	55                   	push   %rbp
  4028aa:	48 89 e5             	mov    %rsp,%rbp
  4028ad:	48 83 ec 20          	sub    $0x20,%rsp
  4028b1:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4028b5:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  4028b9:	48 89 55 e8          	mov    %rdx,-0x18(%rbp)
  4028bd:	48 89 4d e0          	mov    %rcx,-0x20(%rbp)
				       _ForwardIterator __result,
				       _Allocator& __alloc)
    {
      return std::__uninitialized_copy_a
	(_GLIBCXX_MAKE_MOVE_IF_NOEXCEPT_ITERATOR(__first),
	 _GLIBCXX_MAKE_MOVE_IF_NOEXCEPT_ITERATOR(__last), __result, __alloc);
  4028c1:	48 8b 4d e0          	mov    -0x20(%rbp),%rcx
  4028c5:	48 8b 55 e8          	mov    -0x18(%rbp),%rdx
  4028c9:	48 8b 75 f0          	mov    -0x10(%rbp),%rsi
  4028cd:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4028d1:	48 89 c7             	mov    %rax,%rdi
  4028d4:	e8 a0 01 00 00       	callq  402a79 <_ZSt22__uninitialized_copy_aIP10tagCfgInfoS1_S0_ET0_T_S3_S2_RSaIT1_E>
    }
  4028d9:	c9                   	leaveq 
  4028da:	c3                   	retq   

00000000004028db <_ZN9__gnu_cxx14__alloc_traitsISaI10tagCfgInfoEE7destroyERS2_PS1_>:

    static void destroy(_Alloc& __a, pointer __p)
  4028db:	55                   	push   %rbp
  4028dc:	48 89 e5             	mov    %rsp,%rbp
  4028df:	48 83 ec 10          	sub    $0x10,%rsp
  4028e3:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4028e7:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
    { __a.destroy(__p); }
  4028eb:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
  4028ef:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4028f3:	48 89 d6             	mov    %rdx,%rsi
  4028f6:	48 89 c7             	mov    %rax,%rdi
  4028f9:	e8 ac 01 00 00       	callq  402aaa <_ZN9__gnu_cxx13new_allocatorI10tagCfgInfoE7destroyEPS1_>
  4028fe:	90                   	nop
  4028ff:	c9                   	leaveq 
  402900:	c3                   	retq   
  402901:	90                   	nop

0000000000402902 <_ZNSaI10tagCfgInfoEC1Ev>:
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 2103. std::allocator propagate_on_container_move_assignment
      typedef true_type propagate_on_container_move_assignment;
#endif

      allocator() throw() { }
  402902:	55                   	push   %rbp
  402903:	48 89 e5             	mov    %rsp,%rbp
  402906:	48 83 ec 10          	sub    $0x10,%rsp
  40290a:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40290e:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402912:	48 89 c7             	mov    %rax,%rdi
  402915:	e8 b0 01 00 00       	callq  402aca <_ZN9__gnu_cxx13new_allocatorI10tagCfgInfoEC1Ev>
  40291a:	90                   	nop
  40291b:	c9                   	leaveq 
  40291c:	c3                   	retq   
  40291d:	90                   	nop

000000000040291e <_ZN9__gnu_cxx13new_allocatorI10tagCfgInfoED1Ev>:
      new_allocator(const new_allocator&) _GLIBCXX_USE_NOEXCEPT { }

      template<typename _Tp1>
        new_allocator(const new_allocator<_Tp1>&) _GLIBCXX_USE_NOEXCEPT { }

      ~new_allocator() _GLIBCXX_USE_NOEXCEPT { }
  40291e:	55                   	push   %rbp
  40291f:	48 89 e5             	mov    %rsp,%rbp
  402922:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  402926:	90                   	nop
  402927:	5d                   	pop    %rbp
  402928:	c3                   	retq   

0000000000402929 <_ZN9__gnu_cxx14__alloc_traitsISaI10tagCfgInfoEE10deallocateERS2_PS1_m>:

    static pointer
    allocate(_Alloc& __a, size_type __n)
    { return __a.allocate(__n); }

    static void deallocate(_Alloc& __a, pointer __p, size_type __n)
  402929:	55                   	push   %rbp
  40292a:	48 89 e5             	mov    %rsp,%rbp
  40292d:	48 83 ec 20          	sub    $0x20,%rsp
  402931:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  402935:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  402939:	48 89 55 e8          	mov    %rdx,-0x18(%rbp)
    { __a.deallocate(__p, __n); }
  40293d:	48 8b 55 e8          	mov    -0x18(%rbp),%rdx
  402941:	48 8b 4d f0          	mov    -0x10(%rbp),%rcx
  402945:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402949:	48 89 ce             	mov    %rcx,%rsi
  40294c:	48 89 c7             	mov    %rax,%rdi
  40294f:	e8 82 01 00 00       	callq  402ad6 <_ZN9__gnu_cxx13new_allocatorI10tagCfgInfoE10deallocateEPS1_m>
  402954:	90                   	nop
  402955:	c9                   	leaveq 
  402956:	c3                   	retq   

0000000000402957 <_ZNSt12_Destroy_auxILb0EE9__destroyIP10tagCfgInfoEEvT_S4_>:
  template<bool>
    struct _Destroy_aux
    {
      template<typename _ForwardIterator>
        static void
        __destroy(_ForwardIterator __first, _ForwardIterator __last)
  402957:	55                   	push   %rbp
  402958:	48 89 e5             	mov    %rsp,%rbp
  40295b:	48 83 ec 10          	sub    $0x10,%rsp
  40295f:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  402963:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
	{
	  for (; __first != __last; ++__first)
  402967:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40296b:	48 3b 45 f0          	cmp    -0x10(%rbp),%rax
  40296f:	74 1b                	je     40298c <_ZNSt12_Destroy_auxILb0EE9__destroyIP10tagCfgInfoEEvT_S4_+0x35>
	    std::_Destroy(std::__addressof(*__first));
  402971:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402975:	48 89 c7             	mov    %rax,%rdi
  402978:	e8 7c 01 00 00       	callq  402af9 <_ZSt11__addressofI10tagCfgInfoEPT_RS1_>
  40297d:	48 89 c7             	mov    %rax,%rdi
  402980:	e8 82 01 00 00       	callq  402b07 <_ZSt8_DestroyI10tagCfgInfoEvPT_>
    {
      template<typename _ForwardIterator>
        static void
        __destroy(_ForwardIterator __first, _ForwardIterator __last)
	{
	  for (; __first != __last; ++__first)
  402985:	48 83 45 f8 40       	addq   $0x40,-0x8(%rbp)
  40298a:	eb db                	jmp    402967 <_ZNSt12_Destroy_auxILb0EE9__destroyIP10tagCfgInfoEEvT_S4_+0x10>
	    std::_Destroy(std::__addressof(*__first));
	}
  40298c:	90                   	nop
  40298d:	c9                   	leaveq 
  40298e:	c3                   	retq   

000000000040298f <_ZSt12__miter_baseIP10tagCfgInfoENSt11_Miter_baseIT_E13iterator_typeES3_>:
    : _Iter_base<_Iterator, __is_move_iterator<_Iterator>::__value>
    { };

  template<typename _Iterator>
    inline typename _Miter_base<_Iterator>::iterator_type
    __miter_base(_Iterator __it)
  40298f:	55                   	push   %rbp
  402990:	48 89 e5             	mov    %rsp,%rbp
  402993:	48 83 ec 10          	sub    $0x10,%rsp
  402997:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    { return std::_Miter_base<_Iterator>::_S_base(__it); }
  40299b:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40299f:	48 89 c7             	mov    %rax,%rdi
  4029a2:	e8 7b 01 00 00       	callq  402b22 <_ZNSt10_Iter_baseIP10tagCfgInfoLb0EE7_S_baseES1_>
  4029a7:	c9                   	leaveq 
  4029a8:	c3                   	retq   

00000000004029a9 <_ZSt23__copy_move_backward_a2ILb0EP10tagCfgInfoS1_ET1_T0_S3_S2_>:
								 __result);
    }

  template<bool _IsMove, typename _BI1, typename _BI2>
    inline _BI2
    __copy_move_backward_a2(_BI1 __first, _BI1 __last, _BI2 __result)
  4029a9:	55                   	push   %rbp
  4029aa:	48 89 e5             	mov    %rsp,%rbp
  4029ad:	41 54                	push   %r12
  4029af:	53                   	push   %rbx
  4029b0:	48 83 ec 20          	sub    $0x20,%rsp
  4029b4:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  4029b8:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
  4029bc:	48 89 55 d8          	mov    %rdx,-0x28(%rbp)
    {
      return _BI2(std::__copy_move_backward_a<_IsMove>
		  (std::__niter_base(__first), std::__niter_base(__last),
  4029c0:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
  4029c4:	48 89 c7             	mov    %rax,%rdi
  4029c7:	e8 64 01 00 00       	callq  402b30 <_ZSt12__niter_baseIP10tagCfgInfoENSt11_Niter_baseIT_E13iterator_typeES3_>
  4029cc:	49 89 c4             	mov    %rax,%r12
  4029cf:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
  4029d3:	48 89 c7             	mov    %rax,%rdi
  4029d6:	e8 55 01 00 00       	callq  402b30 <_ZSt12__niter_baseIP10tagCfgInfoENSt11_Niter_baseIT_E13iterator_typeES3_>
  4029db:	48 89 c3             	mov    %rax,%rbx
  4029de:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4029e2:	48 89 c7             	mov    %rax,%rdi
  4029e5:	e8 46 01 00 00       	callq  402b30 <_ZSt12__niter_baseIP10tagCfgInfoENSt11_Niter_baseIT_E13iterator_typeES3_>
		   std::__niter_base(__result)));
  4029ea:	4c 89 e2             	mov    %r12,%rdx
  4029ed:	48 89 de             	mov    %rbx,%rsi
  4029f0:	48 89 c7             	mov    %rax,%rdi
  4029f3:	e8 52 01 00 00       	callq  402b4a <_ZSt22__copy_move_backward_aILb0EP10tagCfgInfoS1_ET1_T0_S3_S2_>
    }
  4029f8:	48 83 c4 20          	add    $0x20,%rsp
  4029fc:	5b                   	pop    %rbx
  4029fd:	41 5c                	pop    %r12
  4029ff:	5d                   	pop    %rbp
  402a00:	c3                   	retq   
  402a01:	90                   	nop

0000000000402a02 <_ZNKSt6vectorI10tagCfgInfoSaIS0_EE8max_sizeEv>:
      size() const _GLIBCXX_NOEXCEPT
      { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }

      /**  Returns the size() of the largest possible %vector.  */
      size_type
      max_size() const _GLIBCXX_NOEXCEPT
  402a02:	55                   	push   %rbp
  402a03:	48 89 e5             	mov    %rsp,%rbp
  402a06:	48 83 ec 10          	sub    $0x10,%rsp
  402a0a:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
      { return _Alloc_traits::max_size(_M_get_Tp_allocator()); }
  402a0e:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402a12:	48 89 c7             	mov    %rax,%rdi
  402a15:	e8 7c 01 00 00       	callq  402b96 <_ZNKSt12_Vector_baseI10tagCfgInfoSaIS0_EE19_M_get_Tp_allocatorEv>
  402a1a:	48 89 c7             	mov    %rax,%rdi
  402a1d:	e8 59 01 00 00       	callq  402b7b <_ZN9__gnu_cxx14__alloc_traitsISaI10tagCfgInfoEE8max_sizeERKS2_>
  402a22:	c9                   	leaveq 
  402a23:	c3                   	retq   

0000000000402a24 <_ZSt3maxImERKT_S2_S2_>:
   *  preprocessor macro.
  */
  template<typename _Tp>
    _GLIBCXX14_CONSTEXPR
    inline const _Tp&
    max(const _Tp& __a, const _Tp& __b)
  402a24:	55                   	push   %rbp
  402a25:	48 89 e5             	mov    %rsp,%rbp
  402a28:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  402a2c:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
    {
      // concept requirements
      __glibcxx_function_requires(_LessThanComparableConcept<_Tp>)
      //return  __a < __b ? __b : __a;
      if (__a < __b)
  402a30:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402a34:	48 8b 10             	mov    (%rax),%rdx
  402a37:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  402a3b:	48 8b 00             	mov    (%rax),%rax
  402a3e:	48 39 c2             	cmp    %rax,%rdx
  402a41:	73 06                	jae    402a49 <_ZSt3maxImERKT_S2_S2_+0x25>
	return __b;
  402a43:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  402a47:	eb 04                	jmp    402a4d <_ZSt3maxImERKT_S2_S2_+0x29>
      return __a;
  402a49:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    }
  402a4d:	5d                   	pop    %rbp
  402a4e:	c3                   	retq   

0000000000402a4f <_ZN9__gnu_cxx14__alloc_traitsISaI10tagCfgInfoEE8allocateERS2_m>:
    typedef typename _Alloc::const_reference        const_reference;
    typedef typename _Alloc::size_type              size_type;
    typedef typename _Alloc::difference_type        difference_type;

    static pointer
    allocate(_Alloc& __a, size_type __n)
  402a4f:	55                   	push   %rbp
  402a50:	48 89 e5             	mov    %rsp,%rbp
  402a53:	48 83 ec 10          	sub    $0x10,%rsp
  402a57:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  402a5b:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
    { return __a.allocate(__n); }
  402a5f:	48 8b 4d f0          	mov    -0x10(%rbp),%rcx
  402a63:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402a67:	ba 00 00 00 00       	mov    $0x0,%edx
  402a6c:	48 89 ce             	mov    %rcx,%rsi
  402a6f:	48 89 c7             	mov    %rax,%rdi
  402a72:	e8 2d 01 00 00       	callq  402ba4 <_ZN9__gnu_cxx13new_allocatorI10tagCfgInfoE8allocateEmPKv>
  402a77:	c9                   	leaveq 
  402a78:	c3                   	retq   

0000000000402a79 <_ZSt22__uninitialized_copy_aIP10tagCfgInfoS1_S0_ET0_T_S3_S2_RSaIT1_E>:
	}
    }

  template<typename _InputIterator, typename _ForwardIterator, typename _Tp>
    inline _ForwardIterator
    __uninitialized_copy_a(_InputIterator __first, _InputIterator __last,
  402a79:	55                   	push   %rbp
  402a7a:	48 89 e5             	mov    %rsp,%rbp
  402a7d:	48 83 ec 20          	sub    $0x20,%rsp
  402a81:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  402a85:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  402a89:	48 89 55 e8          	mov    %rdx,-0x18(%rbp)
  402a8d:	48 89 4d e0          	mov    %rcx,-0x20(%rbp)
			   _ForwardIterator __result, allocator<_Tp>&)
    { return std::uninitialized_copy(__first, __last, __result); }
  402a91:	48 8b 55 e8          	mov    -0x18(%rbp),%rdx
  402a95:	48 8b 4d f0          	mov    -0x10(%rbp),%rcx
  402a99:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402a9d:	48 89 ce             	mov    %rcx,%rsi
  402aa0:	48 89 c7             	mov    %rax,%rdi
  402aa3:	e8 3e 01 00 00       	callq  402be6 <_ZSt18uninitialized_copyIP10tagCfgInfoS1_ET0_T_S3_S2_>
  402aa8:	c9                   	leaveq 
  402aa9:	c3                   	retq   

0000000000402aaa <_ZN9__gnu_cxx13new_allocatorI10tagCfgInfoE7destroyEPS1_>:
      void 
      construct(pointer __p, const _Tp& __val) 
      { ::new((void *)__p) _Tp(__val); }

      void 
      destroy(pointer __p) { __p->~_Tp(); }
  402aaa:	55                   	push   %rbp
  402aab:	48 89 e5             	mov    %rsp,%rbp
  402aae:	48 83 ec 10          	sub    $0x10,%rsp
  402ab2:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  402ab6:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  402aba:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  402abe:	48 89 c7             	mov    %rax,%rdi
  402ac1:	e8 9a f2 ff ff       	callq  401d60 <_ZN10tagCfgInfoD1Ev>
  402ac6:	90                   	nop
  402ac7:	c9                   	leaveq 
  402ac8:	c3                   	retq   
  402ac9:	90                   	nop

0000000000402aca <_ZN9__gnu_cxx13new_allocatorI10tagCfgInfoEC1Ev>:
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 2103. propagate_on_container_move_assignment
      typedef std::true_type propagate_on_container_move_assignment;
#endif

      new_allocator() _GLIBCXX_USE_NOEXCEPT { }
  402aca:	55                   	push   %rbp
  402acb:	48 89 e5             	mov    %rsp,%rbp
  402ace:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  402ad2:	90                   	nop
  402ad3:	5d                   	pop    %rbp
  402ad4:	c3                   	retq   
  402ad5:	90                   	nop

0000000000402ad6 <_ZN9__gnu_cxx13new_allocatorI10tagCfgInfoE10deallocateEPS1_m>:
	return static_cast<_Tp*>(::operator new(__n * sizeof(_Tp)));
      }

      // __p is not permitted to be a null pointer.
      void
      deallocate(pointer __p, size_type)
  402ad6:	55                   	push   %rbp
  402ad7:	48 89 e5             	mov    %rsp,%rbp
  402ada:	48 83 ec 20          	sub    $0x20,%rsp
  402ade:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  402ae2:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  402ae6:	48 89 55 e8          	mov    %rdx,-0x18(%rbp)
      { ::operator delete(__p); }
  402aea:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  402aee:	48 89 c7             	mov    %rax,%rdi
  402af1:	e8 9a e5 ff ff       	callq  401090 <_ZdlPv@plt>
  402af6:	90                   	nop
  402af7:	c9                   	leaveq 
  402af8:	c3                   	retq   

0000000000402af9 <_ZSt11__addressofI10tagCfgInfoEPT_RS1_>:
   *  @brief Same as C++11 std::addressof
   *  @ingroup utilities
   */
  template<typename _Tp>
    inline _Tp*
    __addressof(_Tp& __r) _GLIBCXX_NOEXCEPT
  402af9:	55                   	push   %rbp
  402afa:	48 89 e5             	mov    %rsp,%rbp
  402afd:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    {
      return reinterpret_cast<_Tp*>
	(&const_cast<char&>(reinterpret_cast<const volatile char&>(__r)));
  402b01:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    }
  402b05:	5d                   	pop    %rbp
  402b06:	c3                   	retq   

0000000000402b07 <_ZSt8_DestroyI10tagCfgInfoEvPT_>:
  /**
   * Destroy the object pointed to by a pointer type.
   */
  template<typename _Tp>
    inline void
    _Destroy(_Tp* __pointer)
  402b07:	55                   	push   %rbp
  402b08:	48 89 e5             	mov    %rsp,%rbp
  402b0b:	48 83 ec 10          	sub    $0x10,%rsp
  402b0f:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    { __pointer->~_Tp(); }
  402b13:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402b17:	48 89 c7             	mov    %rax,%rdi
  402b1a:	e8 41 f2 ff ff       	callq  401d60 <_ZN10tagCfgInfoD1Ev>
  402b1f:	90                   	nop
  402b20:	c9                   	leaveq 
  402b21:	c3                   	retq   

0000000000402b22 <_ZNSt10_Iter_baseIP10tagCfgInfoLb0EE7_S_baseES1_>:
  // untouched
  template<typename _Iterator, bool _HasBase>
    struct _Iter_base
    {
      typedef _Iterator iterator_type;
      static iterator_type _S_base(_Iterator __it)
  402b22:	55                   	push   %rbp
  402b23:	48 89 e5             	mov    %rsp,%rbp
  402b26:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
      { return __it; }
  402b2a:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402b2e:	5d                   	pop    %rbp
  402b2f:	c3                   	retq   

0000000000402b30 <_ZSt12__niter_baseIP10tagCfgInfoENSt11_Niter_baseIT_E13iterator_typeES3_>:
    : _Iter_base<_Iterator, __is_normal_iterator<_Iterator>::__value>
    { };

  template<typename _Iterator>
    inline typename _Niter_base<_Iterator>::iterator_type
    __niter_base(_Iterator __it)
  402b30:	55                   	push   %rbp
  402b31:	48 89 e5             	mov    %rsp,%rbp
  402b34:	48 83 ec 10          	sub    $0x10,%rsp
  402b38:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    { return std::_Niter_base<_Iterator>::_S_base(__it); }
  402b3c:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402b40:	48 89 c7             	mov    %rax,%rdi
  402b43:	e8 da ff ff ff       	callq  402b22 <_ZNSt10_Iter_baseIP10tagCfgInfoLb0EE7_S_baseES1_>
  402b48:	c9                   	leaveq 
  402b49:	c3                   	retq   

0000000000402b4a <_ZSt22__copy_move_backward_aILb0EP10tagCfgInfoS1_ET1_T0_S3_S2_>:
	}
    };

  template<bool _IsMove, typename _BI1, typename _BI2>
    inline _BI2
    __copy_move_backward_a(_BI1 __first, _BI1 __last, _BI2 __result)
  402b4a:	55                   	push   %rbp
  402b4b:	48 89 e5             	mov    %rsp,%rbp
  402b4e:	48 83 ec 30          	sub    $0x30,%rsp
  402b52:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  402b56:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
  402b5a:	48 89 55 d8          	mov    %rdx,-0x28(%rbp)
      typedef typename iterator_traits<_BI2>::value_type _ValueType2;
      typedef typename iterator_traits<_BI1>::iterator_category _Category;
      const bool __simple = (__is_trivial(_ValueType1)
	                     && __is_pointer<_BI1>::__value
	                     && __is_pointer<_BI2>::__value
			     && __are_same<_ValueType1, _ValueType2>::__value);
  402b5e:	c6 45 ff 00          	movb   $0x0,-0x1(%rbp)

      return std::__copy_move_backward<_IsMove, __simple,
	                               _Category>::__copy_move_b(__first,
								 __last,
								 __result);
  402b62:	48 8b 55 d8          	mov    -0x28(%rbp),%rdx
  402b66:	48 8b 4d e0          	mov    -0x20(%rbp),%rcx
  402b6a:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  402b6e:	48 89 ce             	mov    %rcx,%rsi
  402b71:	48 89 c7             	mov    %rax,%rdi
  402b74:	e8 9e 00 00 00       	callq  402c17 <_ZNSt20__copy_move_backwardILb0ELb0ESt26random_access_iterator_tagE13__copy_move_bIP10tagCfgInfoS4_EET0_T_S6_S5_>
    }
  402b79:	c9                   	leaveq 
  402b7a:	c3                   	retq   

0000000000402b7b <_ZN9__gnu_cxx14__alloc_traitsISaI10tagCfgInfoEE8max_sizeERKS2_>:
      { __a.construct(__p, __arg); }

    static void destroy(_Alloc& __a, pointer __p)
    { __a.destroy(__p); }

    static size_type max_size(const _Alloc& __a)
  402b7b:	55                   	push   %rbp
  402b7c:	48 89 e5             	mov    %rsp,%rbp
  402b7f:	48 83 ec 10          	sub    $0x10,%rsp
  402b83:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    { return __a.max_size(); }
  402b87:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402b8b:	48 89 c7             	mov    %rax,%rdi
  402b8e:	e8 df 00 00 00       	callq  402c72 <_ZNK9__gnu_cxx13new_allocatorI10tagCfgInfoE8max_sizeEv>
  402b93:	c9                   	leaveq 
  402b94:	c3                   	retq   
  402b95:	90                   	nop

0000000000402b96 <_ZNKSt12_Vector_baseI10tagCfgInfoSaIS0_EE19_M_get_Tp_allocatorEv>:
      _Tp_alloc_type&
      _M_get_Tp_allocator() _GLIBCXX_NOEXCEPT
      { return *static_cast<_Tp_alloc_type*>(&this->_M_impl); }

      const _Tp_alloc_type&
      _M_get_Tp_allocator() const _GLIBCXX_NOEXCEPT
  402b96:	55                   	push   %rbp
  402b97:	48 89 e5             	mov    %rsp,%rbp
  402b9a:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
      { return *static_cast<const _Tp_alloc_type*>(&this->_M_impl); }
  402b9e:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402ba2:	5d                   	pop    %rbp
  402ba3:	c3                   	retq   

0000000000402ba4 <_ZN9__gnu_cxx13new_allocatorI10tagCfgInfoE8allocateEmPKv>:
      { return std::__addressof(__x); }

      // NB: __n is permitted to be 0.  The C++ standard says nothing
      // about what the return value is when __n == 0.
      pointer
      allocate(size_type __n, const void* = 0)
  402ba4:	55                   	push   %rbp
  402ba5:	48 89 e5             	mov    %rsp,%rbp
  402ba8:	48 83 ec 20          	sub    $0x20,%rsp
  402bac:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  402bb0:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  402bb4:	48 89 55 e8          	mov    %rdx,-0x18(%rbp)
      { 
	if (__n > this->max_size())
  402bb8:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  402bbc:	48 89 c7             	mov    %rax,%rdi
  402bbf:	e8 ae 00 00 00       	callq  402c72 <_ZNK9__gnu_cxx13new_allocatorI10tagCfgInfoE8max_sizeEv>
  402bc4:	48 3b 45 f0          	cmp    -0x10(%rbp),%rax
  402bc8:	0f 92 c0             	setb   %al
  402bcb:	84 c0                	test   %al,%al
  402bcd:	74 05                	je     402bd4 <_ZN9__gnu_cxx13new_allocatorI10tagCfgInfoE8allocateEmPKv+0x30>
	  std::__throw_bad_alloc();
  402bcf:	e8 0c e6 ff ff       	callq  4011e0 <_ZSt17__throw_bad_allocv@plt>

	return static_cast<_Tp*>(::operator new(__n * sizeof(_Tp)));
  402bd4:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  402bd8:	48 c1 e0 06          	shl    $0x6,%rax
  402bdc:	48 89 c7             	mov    %rax,%rdi
  402bdf:	e8 7c e6 ff ff       	callq  401260 <_Znwm@plt>
      }
  402be4:	c9                   	leaveq 
  402be5:	c3                   	retq   

0000000000402be6 <_ZSt18uninitialized_copyIP10tagCfgInfoS1_ET0_T_S3_S2_>:
   *
   *  Like copy(), but does not require an initialized output range.
  */
  template<typename _InputIterator, typename _ForwardIterator>
    inline _ForwardIterator
    uninitialized_copy(_InputIterator __first, _InputIterator __last,
  402be6:	55                   	push   %rbp
  402be7:	48 89 e5             	mov    %rsp,%rbp
  402bea:	48 83 ec 30          	sub    $0x30,%rsp
  402bee:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  402bf2:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
  402bf6:	48 89 55 d8          	mov    %rdx,-0x28(%rbp)
      typedef typename iterator_traits<_InputIterator>::value_type
	_ValueType1;
      typedef typename iterator_traits<_ForwardIterator>::value_type
	_ValueType2;
#if __cplusplus < 201103L
      const bool __assignable = true;
  402bfa:	c6 45 ff 01          	movb   $0x1,-0x1(%rbp)
#endif

      return std::__uninitialized_copy<__is_trivial(_ValueType1)
				       && __is_trivial(_ValueType2)
				       && __assignable>::
	__uninit_copy(__first, __last, __result);
  402bfe:	48 8b 55 d8          	mov    -0x28(%rbp),%rdx
  402c02:	48 8b 4d e0          	mov    -0x20(%rbp),%rcx
  402c06:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  402c0a:	48 89 ce             	mov    %rcx,%rsi
  402c0d:	48 89 c7             	mov    %rax,%rdi
  402c10:	e8 71 00 00 00       	callq  402c86 <_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIP10tagCfgInfoS3_EET0_T_S5_S4_>
    }
  402c15:	c9                   	leaveq 
  402c16:	c3                   	retq   

0000000000402c17 <_ZNSt20__copy_move_backwardILb0ELb0ESt26random_access_iterator_tagE13__copy_move_bIP10tagCfgInfoS4_EET0_T_S6_S5_>:
  template<>
    struct __copy_move_backward<false, false, random_access_iterator_tag>
    {
      template<typename _BI1, typename _BI2>
        static _BI2
        __copy_move_b(_BI1 __first, _BI1 __last, _BI2 __result)
  402c17:	55                   	push   %rbp
  402c18:	48 89 e5             	mov    %rsp,%rbp
  402c1b:	48 83 ec 30          	sub    $0x30,%rsp
  402c1f:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  402c23:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
  402c27:	48 89 55 d8          	mov    %rdx,-0x28(%rbp)
        {
	  typename iterator_traits<_BI1>::difference_type __n;
	  for (__n = __last - __first; __n > 0; --__n)
  402c2b:	48 8b 55 e0          	mov    -0x20(%rbp),%rdx
  402c2f:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  402c33:	48 29 c2             	sub    %rax,%rdx
  402c36:	48 89 d0             	mov    %rdx,%rax
  402c39:	48 c1 f8 06          	sar    $0x6,%rax
  402c3d:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
  402c41:	48 83 7d f8 00       	cmpq   $0x0,-0x8(%rbp)
  402c46:	7e 24                	jle    402c6c <_ZNSt20__copy_move_backwardILb0ELb0ESt26random_access_iterator_tagE13__copy_move_bIP10tagCfgInfoS4_EET0_T_S6_S5_+0x55>
	    *--__result = *--__last;
  402c48:	48 83 6d e0 40       	subq   $0x40,-0x20(%rbp)
  402c4d:	48 83 6d d8 40       	subq   $0x40,-0x28(%rbp)
  402c52:	48 8b 55 e0          	mov    -0x20(%rbp),%rdx
  402c56:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
  402c5a:	48 89 d6             	mov    %rdx,%rsi
  402c5d:	48 89 c7             	mov    %rax,%rdi
  402c60:	e8 41 f5 ff ff       	callq  4021a6 <_ZN10tagCfgInfoaSERKS_>
      template<typename _BI1, typename _BI2>
        static _BI2
        __copy_move_b(_BI1 __first, _BI1 __last, _BI2 __result)
        {
	  typename iterator_traits<_BI1>::difference_type __n;
	  for (__n = __last - __first; __n > 0; --__n)
  402c65:	48 83 6d f8 01       	subq   $0x1,-0x8(%rbp)
  402c6a:	eb d5                	jmp    402c41 <_ZNSt20__copy_move_backwardILb0ELb0ESt26random_access_iterator_tagE13__copy_move_bIP10tagCfgInfoS4_EET0_T_S6_S5_+0x2a>
	    *--__result = *--__last;
	  return __result;
  402c6c:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
	}
  402c70:	c9                   	leaveq 
  402c71:	c3                   	retq   

0000000000402c72 <_ZNK9__gnu_cxx13new_allocatorI10tagCfgInfoE8max_sizeEv>:
      void
      deallocate(pointer __p, size_type)
      { ::operator delete(__p); }

      size_type
      max_size() const _GLIBCXX_USE_NOEXCEPT
  402c72:	55                   	push   %rbp
  402c73:	48 89 e5             	mov    %rsp,%rbp
  402c76:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
      { return size_t(-1) / sizeof(_Tp); }
  402c7a:	48 b8 ff ff ff ff ff 	movabs $0x3ffffffffffffff,%rax
  402c81:	ff ff 03 
  402c84:	5d                   	pop    %rbp
  402c85:	c3                   	retq   

0000000000402c86 <_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIP10tagCfgInfoS3_EET0_T_S5_S4_>:
  template<bool _TrivialValueTypes>
    struct __uninitialized_copy
    {
      template<typename _InputIterator, typename _ForwardIterator>
        static _ForwardIterator
        __uninit_copy(_InputIterator __first, _InputIterator __last,
  402c86:	55                   	push   %rbp
  402c87:	48 89 e5             	mov    %rsp,%rbp
  402c8a:	53                   	push   %rbx
  402c8b:	48 83 ec 38          	sub    $0x38,%rsp
  402c8f:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
  402c93:	48 89 75 d0          	mov    %rsi,-0x30(%rbp)
  402c97:	48 89 55 c8          	mov    %rdx,-0x38(%rbp)
		      _ForwardIterator __result)
        {
	  _ForwardIterator __cur = __result;
  402c9b:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
  402c9f:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
	  __try
	    {
	      for (; __first != __last; ++__first, ++__cur)
  402ca3:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
  402ca7:	48 3b 45 d0          	cmp    -0x30(%rbp),%rax
  402cab:	74 2a                	je     402cd7 <_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIP10tagCfgInfoS3_EET0_T_S5_S4_+0x51>
		std::_Construct(std::__addressof(*__cur), *__first);
  402cad:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  402cb1:	48 89 c7             	mov    %rax,%rdi
  402cb4:	e8 40 fe ff ff       	callq  402af9 <_ZSt11__addressofI10tagCfgInfoEPT_RS1_>
  402cb9:	48 89 c2             	mov    %rax,%rdx
  402cbc:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
  402cc0:	48 89 c6             	mov    %rax,%rsi
  402cc3:	48 89 d7             	mov    %rdx,%rdi
  402cc6:	e8 4c 00 00 00       	callq  402d17 <_ZSt10_ConstructI10tagCfgInfoS0_EvPT_RKT0_>
		      _ForwardIterator __result)
        {
	  _ForwardIterator __cur = __result;
	  __try
	    {
	      for (; __first != __last; ++__first, ++__cur)
  402ccb:	48 83 45 d8 40       	addq   $0x40,-0x28(%rbp)
  402cd0:	48 83 45 e8 40       	addq   $0x40,-0x18(%rbp)
  402cd5:	eb cc                	jmp    402ca3 <_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIP10tagCfgInfoS3_EET0_T_S5_S4_+0x1d>
		std::_Construct(std::__addressof(*__cur), *__first);
	      return __cur;
  402cd7:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  402cdb:	eb 33                	jmp    402d10 <_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIP10tagCfgInfoS3_EET0_T_S5_S4_+0x8a>
	    }
	  __catch(...)
  402cdd:	48 89 c7             	mov    %rax,%rdi
  402ce0:	e8 2b e5 ff ff       	callq  401210 <__cxa_begin_catch@plt>
	    {
	      std::_Destroy(__result, __cur);
  402ce5:	48 8b 55 e8          	mov    -0x18(%rbp),%rdx
  402ce9:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
  402ced:	48 89 d6             	mov    %rdx,%rsi
  402cf0:	48 89 c7             	mov    %rax,%rdi
  402cf3:	e8 73 f9 ff ff       	callq  40266b <_ZSt8_DestroyIP10tagCfgInfoEvT_S2_>
	      __throw_exception_again;
  402cf8:	e8 a3 e3 ff ff       	callq  4010a0 <__cxa_rethrow@plt>
  402cfd:	48 89 c3             	mov    %rax,%rbx
	    {
	      for (; __first != __last; ++__first, ++__cur)
		std::_Construct(std::__addressof(*__cur), *__first);
	      return __cur;
	    }
	  __catch(...)
  402d00:	e8 cb e4 ff ff       	callq  4011d0 <__cxa_end_catch@plt>
  402d05:	48 89 d8             	mov    %rbx,%rax
  402d08:	48 89 c7             	mov    %rax,%rdi
  402d0b:	e8 60 e5 ff ff       	callq  401270 <_Unwind_Resume@plt>
	    {
	      std::_Destroy(__result, __cur);
	      __throw_exception_again;
	    }
	}
  402d10:	48 83 c4 38          	add    $0x38,%rsp
  402d14:	5b                   	pop    %rbx
  402d15:	5d                   	pop    %rbp
  402d16:	c3                   	retq   

0000000000402d17 <_ZSt10_ConstructI10tagCfgInfoS0_EvPT_RKT0_>:
    _Construct(_T1* __p, _Args&&... __args)
    { ::new(static_cast<void*>(__p)) _T1(std::forward<_Args>(__args)...); }
#else
  template<typename _T1, typename _T2>
    inline void
    _Construct(_T1* __p, const _T2& __value)
  402d17:	55                   	push   %rbp
  402d18:	48 89 e5             	mov    %rsp,%rbp
  402d1b:	41 55                	push   %r13
  402d1d:	41 54                	push   %r12
  402d1f:	53                   	push   %rbx
  402d20:	48 83 ec 18          	sub    $0x18,%rsp
  402d24:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
  402d28:	48 89 75 d0          	mov    %rsi,-0x30(%rbp)
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 402. wrong new expression in [some_]allocator::construct
      ::new(static_cast<void*>(__p)) _T1(__value);
  402d2c:	4c 8b 65 d8          	mov    -0x28(%rbp),%r12
  402d30:	4c 89 e6             	mov    %r12,%rsi
  402d33:	bf 40 00 00 00       	mov    $0x40,%edi
  402d38:	e8 b6 ef ff ff       	callq  401cf3 <_ZnwmPv>
  402d3d:	48 89 c3             	mov    %rax,%rbx
  402d40:	48 85 db             	test   %rbx,%rbx
  402d43:	74 2a                	je     402d6f <_ZSt10_ConstructI10tagCfgInfoS0_EvPT_RKT0_+0x58>
  402d45:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
  402d49:	48 89 c6             	mov    %rax,%rsi
  402d4c:	48 89 df             	mov    %rbx,%rdi
  402d4f:	e8 58 f0 ff ff       	callq  401dac <_ZN10tagCfgInfoC1ERKS_>
    }
  402d54:	eb 19                	jmp    402d6f <_ZSt10_ConstructI10tagCfgInfoS0_EvPT_RKT0_+0x58>
  402d56:	49 89 c5             	mov    %rax,%r13
    inline void
    _Construct(_T1* __p, const _T2& __value)
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 402. wrong new expression in [some_]allocator::construct
      ::new(static_cast<void*>(__p)) _T1(__value);
  402d59:	4c 89 e6             	mov    %r12,%rsi
  402d5c:	48 89 df             	mov    %rbx,%rdi
  402d5f:	e8 a1 ef ff ff       	callq  401d05 <_ZdlPvS_>
  402d64:	4c 89 e8             	mov    %r13,%rax
  402d67:	48 89 c7             	mov    %rax,%rdi
  402d6a:	e8 01 e5 ff ff       	callq  401270 <_Unwind_Resume@plt>
    }
  402d6f:	90                   	nop
  402d70:	48 83 c4 18          	add    $0x18,%rsp
  402d74:	5b                   	pop    %rbx
  402d75:	41 5c                	pop    %r12
  402d77:	41 5d                	pop    %r13
  402d79:	5d                   	pop    %rbp
  402d7a:	c3                   	retq   

0000000000402d7b <main>:
#include <iostream>

#include "HWConfig.h"

int main()
{
  402d7b:	55                   	push   %rbp
  402d7c:	48 89 e5             	mov    %rsp,%rbp
  402d7f:	41 54                	push   %r12
  402d81:	53                   	push   %rbx
  402d82:	48 83 ec 20          	sub    $0x20,%rsp
    HWConfig *ini = new HWConfig();
  402d86:	bf 18 00 00 00       	mov    $0x18,%edi
  402d8b:	e8 d0 e4 ff ff       	callq  401260 <_Znwm@plt>
  402d90:	48 89 c3             	mov    %rax,%rbx
  402d93:	48 89 df             	mov    %rbx,%rdi
  402d96:	e8 eb e5 ff ff       	callq  401386 <_ZN8HWConfigC1Ev>
  402d9b:	48 89 5d e0          	mov    %rbx,-0x20(%rbp)

    ini->printAll();
  402d9f:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
  402da3:	48 89 c7             	mov    %rax,%rdi
  402da6:	e8 d3 ed ff ff       	callq  401b7e <_ZN8HWConfig8printAllEv>

    int mem = ini->getIntValue("MEM");
  402dab:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
  402daf:	be d0 30 40 00       	mov    $0x4030d0,%esi
  402db4:	48 89 c7             	mov    %rax,%rdi
  402db7:	e8 fc eb ff ff       	callq  4019b8 <_ZN8HWConfig11getIntValueEPKc>
  402dbc:	89 45 dc             	mov    %eax,-0x24(%rbp)
    bool bWifi = ini->getBoolValue("WIFI");
  402dbf:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
  402dc3:	be d4 30 40 00       	mov    $0x4030d4,%esi
  402dc8:	48 89 c7             	mov    %rax,%rdi
  402dcb:	e8 7a eb ff ff       	callq  40194a <_ZN8HWConfig12getBoolValueEPKc>
  402dd0:	88 45 db             	mov    %al,-0x25(%rbp)
    const char *str_Cpu = ini->getStringValue("CPU");
  402dd3:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
  402dd7:	be d9 30 40 00       	mov    $0x4030d9,%esi
  402ddc:	48 89 c7             	mov    %rax,%rdi
  402ddf:	e8 7e ec ff ff       	callq  401a62 <_ZN8HWConfig14getStringValueEPKc>
  402de4:	48 89 45 e8          	mov    %rax,-0x18(%rbp)

    cout<<"[MEM]-int:\t"<<mem<<endl;
  402de8:	be dd 30 40 00       	mov    $0x4030dd,%esi
  402ded:	bf 60 51 60 00       	mov    $0x605160,%edi
  402df2:	e8 29 e3 ff ff       	callq  401120 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  402df7:	48 89 c2             	mov    %rax,%rdx
  402dfa:	8b 45 dc             	mov    -0x24(%rbp),%eax
  402dfd:	89 c6                	mov    %eax,%esi
  402dff:	48 89 d7             	mov    %rdx,%rdi
  402e02:	e8 59 e2 ff ff       	callq  401060 <_ZNSolsEi@plt>
  402e07:	be 00 12 40 00       	mov    $0x401200,%esi
  402e0c:	48 89 c7             	mov    %rax,%rdi
  402e0f:	e8 9c e3 ff ff       	callq  4011b0 <_ZNSolsEPFRSoS_E@plt>
    cout<<"[WIFI]-bool:\t"<<bWifi<<endl;
  402e14:	0f b6 5d db          	movzbl -0x25(%rbp),%ebx
  402e18:	be e9 30 40 00       	mov    $0x4030e9,%esi
  402e1d:	bf 60 51 60 00       	mov    $0x605160,%edi
  402e22:	e8 f9 e2 ff ff       	callq  401120 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  402e27:	89 de                	mov    %ebx,%esi
  402e29:	48 89 c7             	mov    %rax,%rdi
  402e2c:	e8 2f e3 ff ff       	callq  401160 <_ZNSolsEb@plt>
  402e31:	be 00 12 40 00       	mov    $0x401200,%esi
  402e36:	48 89 c7             	mov    %rax,%rdi
  402e39:	e8 72 e3 ff ff       	callq  4011b0 <_ZNSolsEPFRSoS_E@plt>
    cout<<"[CPU]-string:\t"<<str_Cpu<<endl;
  402e3e:	be f7 30 40 00       	mov    $0x4030f7,%esi
  402e43:	bf 60 51 60 00       	mov    $0x605160,%edi
  402e48:	e8 d3 e2 ff ff       	callq  401120 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  402e4d:	48 89 c2             	mov    %rax,%rdx
  402e50:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  402e54:	48 89 c6             	mov    %rax,%rsi
  402e57:	48 89 d7             	mov    %rdx,%rdi
  402e5a:	e8 c1 e2 ff ff       	callq  401120 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  402e5f:	be 00 12 40 00       	mov    $0x401200,%esi
  402e64:	48 89 c7             	mov    %rax,%rdi
  402e67:	e8 44 e3 ff ff       	callq  4011b0 <_ZNSolsEPFRSoS_E@plt>

    delete ini;
  402e6c:	48 8b 5d e0          	mov    -0x20(%rbp),%rbx
  402e70:	48 85 db             	test   %rbx,%rbx
  402e73:	74 10                	je     402e85 <main+0x10a>
  402e75:	48 89 df             	mov    %rbx,%rdi
  402e78:	e8 b1 ea ff ff       	callq  40192e <_ZN8HWConfigD1Ev>
  402e7d:	48 89 df             	mov    %rbx,%rdi
  402e80:	e8 0b e2 ff ff       	callq  401090 <_ZdlPv@plt>

    return 0;
  402e85:	b8 00 00 00 00       	mov    $0x0,%eax
  402e8a:	eb 16                	jmp    402ea2 <main+0x127>
  402e8c:	49 89 c4             	mov    %rax,%r12

#include "HWConfig.h"

int main()
{
    HWConfig *ini = new HWConfig();
  402e8f:	48 89 df             	mov    %rbx,%rdi
  402e92:	e8 f9 e1 ff ff       	callq  401090 <_ZdlPv@plt>
  402e97:	4c 89 e0             	mov    %r12,%rax
  402e9a:	48 89 c7             	mov    %rax,%rdi
  402e9d:	e8 ce e3 ff ff       	callq  401270 <_Unwind_Resume@plt>
    cout<<"[CPU]-string:\t"<<str_Cpu<<endl;

    delete ini;

    return 0;
}
  402ea2:	48 83 c4 20          	add    $0x20,%rsp
  402ea6:	5b                   	pop    %rbx
  402ea7:	41 5c                	pop    %r12
  402ea9:	5d                   	pop    %rbp
  402eaa:	c3                   	retq   

0000000000402eab <_Z41__static_initialization_and_destruction_0ii>:
  402eab:	55                   	push   %rbp
  402eac:	48 89 e5             	mov    %rsp,%rbp
  402eaf:	48 83 ec 10          	sub    $0x10,%rsp
  402eb3:	89 7d fc             	mov    %edi,-0x4(%rbp)
  402eb6:	89 75 f8             	mov    %esi,-0x8(%rbp)
  402eb9:	83 7d fc 01          	cmpl   $0x1,-0x4(%rbp)
  402ebd:	75 27                	jne    402ee6 <_Z41__static_initialization_and_destruction_0ii+0x3b>
  402ebf:	81 7d f8 ff ff 00 00 	cmpl   $0xffff,-0x8(%rbp)
  402ec6:	75 1e                	jne    402ee6 <_Z41__static_initialization_and_destruction_0ii+0x3b>
  402ec8:	bf 72 52 60 00       	mov    $0x605272,%edi
  402ecd:	e8 ee e1 ff ff       	callq  4010c0 <_ZNSt8ios_base4InitC1Ev@plt>
  402ed2:	ba 40 51 60 00       	mov    $0x605140,%edx
  402ed7:	be 72 52 60 00       	mov    $0x605272,%esi
  402edc:	bf 00 11 40 00       	mov    $0x401100,%edi
  402ee1:	e8 0a e2 ff ff       	callq  4010f0 <__cxa_atexit@plt>
  402ee6:	90                   	nop
  402ee7:	c9                   	leaveq 
  402ee8:	c3                   	retq   

0000000000402ee9 <_GLOBAL__sub_I_main>:
  402ee9:	55                   	push   %rbp
  402eea:	48 89 e5             	mov    %rsp,%rbp
  402eed:	be ff ff 00 00       	mov    $0xffff,%esi
  402ef2:	bf 01 00 00 00       	mov    $0x1,%edi
  402ef7:	e8 af ff ff ff       	callq  402eab <_Z41__static_initialization_and_destruction_0ii>
  402efc:	5d                   	pop    %rbp
  402efd:	c3                   	retq   
  402efe:	66 90                	xchg   %ax,%ax

0000000000402f00 <__libc_csu_init>:
  402f00:	41 57                	push   %r15
  402f02:	41 56                	push   %r14
  402f04:	41 89 ff             	mov    %edi,%r15d
  402f07:	41 55                	push   %r13
  402f09:	41 54                	push   %r12
  402f0b:	4c 8d 25 ce 1e 20 00 	lea    0x201ece(%rip),%r12        # 604de0 <__frame_dummy_init_array_entry>
  402f12:	55                   	push   %rbp
  402f13:	48 8d 2d de 1e 20 00 	lea    0x201ede(%rip),%rbp        # 604df8 <__init_array_end>
  402f1a:	53                   	push   %rbx
  402f1b:	49 89 f6             	mov    %rsi,%r14
  402f1e:	49 89 d5             	mov    %rdx,%r13
  402f21:	4c 29 e5             	sub    %r12,%rbp
  402f24:	48 83 ec 08          	sub    $0x8,%rsp
  402f28:	48 c1 fd 03          	sar    $0x3,%rbp
  402f2c:	e8 df e0 ff ff       	callq  401010 <_init>
  402f31:	48 85 ed             	test   %rbp,%rbp
  402f34:	74 20                	je     402f56 <__libc_csu_init+0x56>
  402f36:	31 db                	xor    %ebx,%ebx
  402f38:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
  402f3f:	00 
  402f40:	4c 89 ea             	mov    %r13,%rdx
  402f43:	4c 89 f6             	mov    %r14,%rsi
  402f46:	44 89 ff             	mov    %r15d,%edi
  402f49:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  402f4d:	48 83 c3 01          	add    $0x1,%rbx
  402f51:	48 39 eb             	cmp    %rbp,%rbx
  402f54:	75 ea                	jne    402f40 <__libc_csu_init+0x40>
  402f56:	48 83 c4 08          	add    $0x8,%rsp
  402f5a:	5b                   	pop    %rbx
  402f5b:	5d                   	pop    %rbp
  402f5c:	41 5c                	pop    %r12
  402f5e:	41 5d                	pop    %r13
  402f60:	41 5e                	pop    %r14
  402f62:	41 5f                	pop    %r15
  402f64:	c3                   	retq   
  402f65:	90                   	nop
  402f66:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  402f6d:	00 00 00 

0000000000402f70 <__libc_csu_fini>:
  402f70:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000402f74 <_fini>:
  402f74:	48 83 ec 08          	sub    $0x8,%rsp
  402f78:	48 83 c4 08          	add    $0x8,%rsp
  402f7c:	c3                   	retq   
