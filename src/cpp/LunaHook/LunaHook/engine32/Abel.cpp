#include "Abel.h"

/********************************************************************************************
AbelSoftware hook:
  The game folder usually is made up many no extended name files(file name doesn't have '.').
  And these files have common prefix which is the game name, and 2 digit in order.


********************************************************************************************/
/** 7/31/2015
 *  Sample game オタカ� *  Hooked address: 0x4413b0
 *
 *  GDI functions are cached: TextOutA and GetTextExtentPoint32A
 *
 *  004413AB   90               NOP
 *  004413AC   90               NOP
 *  004413AD   90               NOP
 *  004413AE   90               NOP
 *  004413AF   90               NOP
 *  004413B0   6A FF            PUSH -0x1   ; jichi: text in arg1, but text painted character by character
 *  004413B2   68 D0714900      PUSH .004971D0
 *  004413B7   64:A1 00000000   MOV EAX,DWORD PTR FS:[0]
 *  004413BD   50               PUSH EAX
 *  004413BE   64:8925 00000000 MOV DWORD PTR FS:[0],ESP
 *  004413C5   83EC 4C          SUB ESP,0x4C
 *  004413C8   A1 C00B4B00      MOV EAX,DWORD PTR DS:[0x4B0BC0]
 *  004413CD   53               PUSH EBX
 *  004413CE   55               PUSH EBP
 *  004413CF   56               PUSH ESI
 *  004413D0   57               PUSH EDI
 *  004413D1   8BF1             MOV ESI,ECX
 *  004413D3   894424 48        MOV DWORD PTR SS:[ESP+0x48],EAX
 *  004413D7   894424 4C        MOV DWORD PTR SS:[ESP+0x4C],EAX
 *  004413DB   894424 58        MOV DWORD PTR SS:[ESP+0x58],EAX
 *  004413DF   8B4424 6C        MOV EAX,DWORD PTR SS:[ESP+0x6C]
 *  004413E3   33DB             XOR EBX,EBX
 *  004413E5   50               PUSH EAX
 *  004413E6   8D4C24 4C        LEA ECX,DWORD PTR SS:[ESP+0x4C]
 *  004413EA   895C24 68        MOV DWORD PTR SS:[ESP+0x68],EBX
 *  004413EE   E8 74520400      CALL .00486667
 *  004413F3   8B4C24 78        MOV ECX,DWORD PTR SS:[ESP+0x78]
 *  004413F7   51               PUSH ECX
 *  004413F8   8D4C24 50        LEA ECX,DWORD PTR SS:[ESP+0x50]
 *  004413FC   E8 66520400      CALL .00486667
 *  00441401   8B5424 7C        MOV EDX,DWORD PTR SS:[ESP+0x7C]
 *  00441405   8D4C24 58        LEA ECX,DWORD PTR SS:[ESP+0x58]
 *  00441409   52               PUSH EDX
 *  0044140A   E8 58520400      CALL .00486667
 *  0044140F   8B4424 70        MOV EAX,DWORD PTR SS:[ESP+0x70]
 *  00441413   894424 50        MOV DWORD PTR SS:[ESP+0x50],EAX
 *  00441417   8B4424 74        MOV EAX,DWORD PTR SS:[ESP+0x74]
 *  0044141B   83F8 FF          CMP EAX,-0x1
 *  0044141E   75 06            JNZ SHORT .00441426
 *  00441420   895C24 54        MOV DWORD PTR SS:[ESP+0x54],EBX
 *  00441424   EB 2E            JMP SHORT .00441454
 *  00441426   8BC8             MOV ECX,EAX
 *  00441428   33D2             XOR EDX,EDX
 *  0044142A   81E1 FF000000    AND ECX,0xFF
 *  00441430   8AD4             MOV DL,AH
 *  00441432   81C9 00FFFFFF    OR ECX,0xFFFFFF00
 *  00441438   81E2 FF000000    AND EDX,0xFF
 *  0044143E   C1E1 08          SHL ECX,0x8
 *  00441441   0BCA             OR ECX,EDX
 *  00441443   C1E8 10          SHR EAX,0x10
 *  00441446   C1E1 08          SHL ECX,0x8
 *  00441449   25 FF000000      AND EAX,0xFF
 *  0044144E   0BC8             OR ECX,EAX
 *  00441450   894C24 54        MOV DWORD PTR SS:[ESP+0x54],ECX
 *  00441454   8B4424 48        MOV EAX,DWORD PTR SS:[ESP+0x48]
 *  00441458   3958 F8          CMP DWORD PTR DS:[EAX-0x8],EBX
 *  0044145B   0F84 7A030000    JE .004417DB
 *  00441461   8B8E 08020000    MOV ECX,DWORD PTR DS:[ESI+0x208]
 *  00441467   83F9 20          CMP ECX,0x20
 *  0044146A   0F8D 35030000    JGE .004417A5
 *  00441470   0FBE00           MOVSX EAX,BYTE PTR DS:[EAX]
 *  00441473   83E8 09          SUB EAX,0x9
 *  00441476   0F84 29030000    JE .004417A5
 *  0044147C   48               DEC EAX
 *  0044147D   0F84 0A030000    JE .0044178D
 *  00441483   83E8 03          SUB EAX,0x3
 *  00441486   0F84 19030000    JE .004417A5
 *  0044148C   8BBE 38010000    MOV EDI,DWORD PTR DS:[ESI+0x138]
 *  00441492   68 80C84A00      PUSH .004AC880
 *  00441497   8BCF             MOV ECX,EDI
 *  00441499   E8 E2E9FDFF      CALL .0041FE80
 *  0044149E   3BC3             CMP EAX,EBX
 *  004414A0   7D 0F            JGE SHORT .004414B1
 *  004414A2   53               PUSH EBX
 *  004414A3   53               PUSH EBX
 *  004414A4   53               PUSH EBX
 *  004414A5   53               PUSH EBX
 *  004414A6   8D4C24 48        LEA ECX,DWORD PTR SS:[ESP+0x48]
 *  004414AA   E8 916DFDFF      CALL .00418240
 *  004414AF   EB 06            JMP SHORT .004414B7
 *  004414B1   8B4F 24          MOV ECX,DWORD PTR DS:[EDI+0x24]
 *  004414B4   8B0481           MOV EAX,DWORD PTR DS:[ECX+EAX*4]
 *  004414B7   8B48 04          MOV ECX,DWORD PTR DS:[EAX+0x4]
 *  004414BA   8B10             MOV EDX,DWORD PTR DS:[EAX]
 *  004414BC   894C24 24        MOV DWORD PTR SS:[ESP+0x24],ECX
 *  004414C0   895424 20        MOV DWORD PTR SS:[ESP+0x20],EDX
 *  004414C4   8B50 08          MOV EDX,DWORD PTR DS:[EAX+0x8]
 *  004414C7   8B40 0C          MOV EAX,DWORD PTR DS:[EAX+0xC]
 *  004414CA   8D4C24 10        LEA ECX,DWORD PTR SS:[ESP+0x10]
 *  004414CE   895424 28        MOV DWORD PTR SS:[ESP+0x28],EDX
 *  004414D2   51               PUSH ECX
 *  004414D3   8BCE             MOV ECX,ESI
 *  004414D5   894424 30        MOV DWORD PTR SS:[ESP+0x30],EAX
 *  004414D9   E8 52F3FFFF      CALL .00440830
 *  004414DE   8B5424 50        MOV EDX,DWORD PTR SS:[ESP+0x50]
 *  004414E2   33C9             XOR ECX,ECX
 *  004414E4   894C24 78        MOV DWORD PTR SS:[ESP+0x78],ECX
 *  004414E8   B8 B0B64900      MOV EAX,.0049B6B0
 *  004414ED   3B10             CMP EDX,DWORD PTR DS:[EAX]
 *  004414EF   7E 0B            JLE SHORT .004414FC
 *  004414F1   83C0 04          ADD EAX,0x4
 *  004414F4   41               INC ECX
 *  004414F5   3D C0B64900      CMP EAX,.0049B6C0
 *  004414FA  ^72 F1            JB SHORT .004414ED
 *  004414FC   8B5424 48        MOV EDX,DWORD PTR SS:[ESP+0x48]
 *  00441500   8D4424 18        LEA EAX,DWORD PTR SS:[ESP+0x18]
 *  00441504   894C24 78        MOV DWORD PTR SS:[ESP+0x78],ECX
 *  00441508   8B4C8E 3C        MOV ECX,DWORD PTR DS:[ESI+ECX*4+0x3C]
 *  0044150C   52               PUSH EDX
 *  0044150D   50               PUSH EAX
 *  0044150E   E8 3D34FCFF      CALL .00404950
 *  00441513   8B46 38          MOV EAX,DWORD PTR DS:[ESI+0x38]
 *  00441516   895C24 70        MOV DWORD PTR SS:[ESP+0x70],EBX
 *  0044151A   3BC3             CMP EAX,EBX
 *  0044151C   0F84 F9000000    JE .0044161B
 *  00441522   8B50 08          MOV EDX,DWORD PTR DS:[EAX+0x8]
 *  00441525   8B4E 78          MOV ECX,DWORD PTR DS:[ESI+0x78]
 *  00441528   3BCA             CMP ECX,EDX
 *  0044152A   0F8D EB000000    JGE .0044161B
 *  00441530   8B50 04          MOV EDX,DWORD PTR DS:[EAX+0x4]
 *  00441533   8B4424 10        MOV EAX,DWORD PTR SS:[ESP+0x10]
 *  00441537   8B7E 74          MOV EDI,DWORD PTR DS:[ESI+0x74]
 *  0044153A   8B2C8A           MOV EBP,DWORD PTR DS:[EDX+ECX*4]
 *  0044153D   8B4C24 18        MOV ECX,DWORD PTR SS:[ESP+0x18]
 *  00441541   897C24 7C        MOV DWORD PTR SS:[ESP+0x7C],EDI
 *  00441545   8B55 00          MOV EDX,DWORD PTR SS:[EBP]
 *  00441548   8D1C01           LEA EBX,DWORD PTR DS:[ECX+EAX]
 *  0044154B   8BCD             MOV ECX,EBP
 *  0044154D   FF52 08          CALL DWORD PTR DS:[EDX+0x8]
 *  00441550   3BF8             CMP EDI,EAX
 *  00441552   0F8D C3000000    JGE .0044161B
 *  00441558   EB 04            JMP SHORT .0044155E
 *  0044155A   8B7C24 7C        MOV EDI,DWORD PTR SS:[ESP+0x7C]
 *  0044155E   8B45 00          MOV EAX,DWORD PTR SS:[EBP]
 *  00441561   57               PUSH EDI
 *  00441562   8BCD             MOV ECX,EBP
 *  00441564   FF50 04          CALL DWORD PTR DS:[EAX+0x4]
 *  00441567   8BF8             MOV EDI,EAX
 *  00441569   8BCF             MOV ECX,EDI
 *  0044156B   8B17             MOV EDX,DWORD PTR DS:[EDI]
 *  0044156D   FF52 0C          CALL DWORD PTR DS:[EDX+0xC]
 *  00441570   85C0             TEST EAX,EAX
 *  00441572   0F84 A3000000    JE .0044161B
 *  00441578   8B07             MOV EAX,DWORD PTR DS:[EDI]
 *  0044157A   8D4C24 6C        LEA ECX,DWORD PTR SS:[ESP+0x6C]
 *  0044157E   51               PUSH ECX
 *  0044157F   8BCF             MOV ECX,EDI
 *  00441581   FF50 10          CALL DWORD PTR DS:[EAX+0x10]
 *  00441584   8B5424 6C        MOV EDX,DWORD PTR SS:[ESP+0x6C]
 *  00441588   8B4C24 78        MOV ECX,DWORD PTR SS:[ESP+0x78]
 *  0044158C   8D4424 30        LEA EAX,DWORD PTR SS:[ESP+0x30]
 *  00441590   52               PUSH EDX
 *  00441591   8B4C8E 3C        MOV ECX,DWORD PTR DS:[ESI+ECX*4+0x3C]
 *  00441595   50               PUSH EAX
 *  00441596   C64424 6C 01     MOV BYTE PTR SS:[ESP+0x6C],0x1
 *  0044159B   E8 B033FCFF      CALL .00404950
 *  004415A0   8B10             MOV EDX,DWORD PTR DS:[EAX]
 *  004415A2   8B86 E4030000    MOV EAX,DWORD PTR DS:[ESI+0x3E4]
 *  004415A8   03DA             ADD EBX,EDX
 *  004415AA   8B5424 6C        MOV EDX,DWORD PTR SS:[ESP+0x6C]
 *  004415AE   52               PUSH EDX
 *  004415AF   50               PUSH EAX
 *  004415B0   E8 BB020000      CALL .00441870
 *  004415B5   83C4 08          ADD ESP,0x8
 *  004415B8   85C0             TEST EAX,EAX
 *  004415BA   74 08            JE SHORT .004415C4
 *  004415BC   3B5C24 28        CMP EBX,DWORD PTR SS:[ESP+0x28]
 *  004415C0   7F 43            JG SHORT .00441605
 *  004415C2   EB 18            JMP SHORT .004415DC
 *  004415C4   8B4C24 6C        MOV ECX,DWORD PTR SS:[ESP+0x6C]
 *  004415C8   8B86 E0030000    MOV EAX,DWORD PTR DS:[ESI+0x3E0]
 *  004415CE   51               PUSH ECX
 *  004415CF   50               PUSH EAX
 *  004415D0   E8 9B020000      CALL .00441870
 *  004415D5   83C4 08          ADD ESP,0x8
 *  004415D8   85C0             TEST EAX,EAX
 *  004415DA   74 31            JE SHORT .0044160D
 *  004415DC   8D4C24 6C        LEA ECX,DWORD PTR SS:[ESP+0x6C]
 *  004415E0   C64424 64 00     MOV BYTE PTR SS:[ESP+0x64],0x0
 *  004415E5   E8 404F0400      CALL .0048652A
 *  004415EA   8B7C24 7C        MOV EDI,DWORD PTR SS:[ESP+0x7C]
 *  004415EE   8B55 00          MOV EDX,DWORD PTR SS:[EBP]
 *  004415F1   47               INC EDI
 *  004415F2   8BCD             MOV ECX,EBP
 *  004415F4   897C24 7C        MOV DWORD PTR SS:[ESP+0x7C],EDI
 *  004415F8   FF52 08          CALL DWORD PTR DS:[EDX+0x8]
 *  004415FB   3BF8             CMP EDI,EAX
 *  004415FD  ^0F8C 57FFFFFF    JL .0044155A
 *  00441603   EB 16            JMP SHORT .0044161B
 *  00441605   C74424 70 010000>MOV DWORD PTR SS:[ESP+0x70],0x1
 *  0044160D   8D4C24 6C        LEA ECX,DWORD PTR SS:[ESP+0x6C]
 *  00441611   C64424 64 00     MOV BYTE PTR SS:[ESP+0x64],0x0
 *  00441616   E8 0F4F0400      CALL .0048652A
 *  0044161B   8B4424 10        MOV EAX,DWORD PTR SS:[ESP+0x10]
 *  0044161F   8B4C24 18        MOV ECX,DWORD PTR SS:[ESP+0x18]
 *  00441623   03C8             ADD ECX,EAX
 *  00441625   8B4424 28        MOV EAX,DWORD PTR SS:[ESP+0x28]
 *  00441629   3BC8             CMP ECX,EAX
 *  0044162B   7E 18            JLE SHORT .00441645
 *  0044162D   8B5424 48        MOV EDX,DWORD PTR SS:[ESP+0x48]
 *  00441631   8B86 E0030000    MOV EAX,DWORD PTR DS:[ESI+0x3E0]
 *  00441637   52               PUSH EDX
 *  00441638   50               PUSH EAX
 *  00441639   E8 32020000      CALL .00441870
 *  0044163E   83C4 08          ADD ESP,0x8
 *  00441641   85C0             TEST EAX,EAX
 *  00441643   74 08            JE SHORT .0044164D
 *  00441645   8B4424 70        MOV EAX,DWORD PTR SS:[ESP+0x70]
 *  00441649   85C0             TEST EAX,EAX
 *  0044164B   74 3F            JE SHORT .0044168C
 *  0044164D   8B8E 08020000    MOV ECX,DWORD PTR DS:[ESI+0x208]
 *  00441653   41               INC ECX
 *  00441654   8BC1             MOV EAX,ECX
 *  00441656   898E 08020000    MOV DWORD PTR DS:[ESI+0x208],ECX
 *  0044165C   83F8 20          CMP EAX,0x20
 *  0044165F   0F8D 40010000    JGE .004417A5
 *  00441665   83EC 10          SUB ESP,0x10
 *  00441668   8B15 D0B04A00    MOV EDX,DWORD PTR DS:[0x4AB0D0]
 *  0044166E   8BDC             MOV EBX,ESP
 *  00441670   33C0             XOR EAX,EAX
 *  00441672   8B3D D4B04A00    MOV EDI,DWORD PTR DS:[0x4AB0D4]
 *  00441678   33C9             XOR ECX,ECX
 *  0044167A   8903             MOV DWORD PTR DS:[EBX],EAX
 *  0044167C   894B 04          MOV DWORD PTR DS:[EBX+0x4],ECX
 *  0044167F   8BCE             MOV ECX,ESI
 *  00441681   8953 08          MOV DWORD PTR DS:[EBX+0x8],EDX
 *  00441684   897B 0C          MOV DWORD PTR DS:[EBX+0xC],EDI
 *  00441687   E8 7418FCFF      CALL .00402F00
 *  0044168C   8B86 08020000    MOV EAX,DWORD PTR DS:[ESI+0x208]
 *  00441692   6A 00            PUSH 0x0
 *  00441694   8D0CC5 00000000  LEA ECX,DWORD PTR DS:[EAX*8]
 *  0044169B   2BC8             SUB ECX,EAX
 *  0044169D   8B948E 78040000  MOV EDX,DWORD PTR DS:[ESI+ECX*4+0x478]
 *  004416A4   8DAC8E 70040000  LEA EBP,DWORD PTR DS:[ESI+ECX*4+0x470]
 *  004416AB   52               PUSH EDX
 *  004416AC   8BCD             MOV ECX,EBP
 *  004416AE   E8 7D8A0000      CALL .0044A130
 *  004416B3   8BD8             MOV EBX,EAX
 *  004416B5   8D4424 48        LEA EAX,DWORD PTR SS:[ESP+0x48]
 *  004416B9   50               PUSH EAX
 *  004416BA   8D7B 08          LEA EDI,DWORD PTR DS:[EBX+0x8]
 *  004416BD   8BCF             MOV ECX,EDI
 *  004416BF   E8 534F0400      CALL .00486617
 *  004416C4   8D4C24 4C        LEA ECX,DWORD PTR SS:[ESP+0x4C]
 *  004416C8   51               PUSH ECX
 *  004416C9   8D4F 04          LEA ECX,DWORD PTR DS:[EDI+0x4]
 *  004416CC   E8 464F0400      CALL .00486617
 *  004416D1   8B5424 50        MOV EDX,DWORD PTR SS:[ESP+0x50]
 *  004416D5   8D4C24 58        LEA ECX,DWORD PTR SS:[ESP+0x58]
 *  004416D9   8957 08          MOV DWORD PTR DS:[EDI+0x8],EDX
 *  004416DC   8B4424 54        MOV EAX,DWORD PTR SS:[ESP+0x54]
 *  004416E0   51               PUSH ECX
 *  004416E1   8D4F 10          LEA ECX,DWORD PTR DS:[EDI+0x10]
 *  004416E4   8947 0C          MOV DWORD PTR DS:[EDI+0xC],EAX
 *  004416E7   E8 2B4F0400      CALL .00486617
 *  004416EC   8B45 08          MOV EAX,DWORD PTR SS:[EBP+0x8]
 *  004416EF   85C0             TEST EAX,EAX
 *  004416F1   74 04            JE SHORT .004416F7
 *  004416F3   8918             MOV DWORD PTR DS:[EAX],EBX
 *  004416F5   EB 03            JMP SHORT .004416FA
 *  004416F7   895D 04          MOV DWORD PTR SS:[EBP+0x4],EBX
 *  004416FA   83EC 10          SUB ESP,0x10
 *  004416FD   895D 08          MOV DWORD PTR SS:[EBP+0x8],EBX
 *  00441700   8B4424 20        MOV EAX,DWORD PTR SS:[ESP+0x20]
 *  00441704   8B5424 28        MOV EDX,DWORD PTR SS:[ESP+0x28]
 *  00441708   8B7C24 2C        MOV EDI,DWORD PTR SS:[ESP+0x2C]
 *  0044170C   8BDC             MOV EBX,ESP
 *  0044170E   8D4C02 02        LEA ECX,DWORD PTR DS:[EDX+EAX+0x2]
 *  00441712   8B5424 24        MOV EDX,DWORD PTR SS:[ESP+0x24]
 *  00441716   8903             MOV DWORD PTR DS:[EBX],EAX
 *  00441718   8D7C3A 02        LEA EDI,DWORD PTR DS:[EDX+EDI+0x2]
 *  0044171C   8953 04          MOV DWORD PTR DS:[EBX+0x4],EDX
 *  0044171F   894B 08          MOV DWORD PTR DS:[EBX+0x8],ECX
 *  00441722   8BCE             MOV ECX,ESI
 *  00441724   897B 0C          MOV DWORD PTR DS:[EBX+0xC],EDI
 *  00441727   E8 D417FCFF      CALL .00402F00
 *  0044172C   8B4424 4C        MOV EAX,DWORD PTR SS:[ESP+0x4C]
 *  00441730   8B48 F8          MOV ECX,DWORD PTR DS:[EAX-0x8]
 *  00441733   85C9             TEST ECX,ECX
 *  00441735   74 6E            JE SHORT .004417A5
 *  00441737   8B4E 3C          MOV ECX,DWORD PTR DS:[ESI+0x3C]
 *  0044173A   50               PUSH EAX
 *  0044173B   8D4424 24        LEA EAX,DWORD PTR SS:[ESP+0x24]
 *  0044173F   50               PUSH EAX
 *  00441740   E8 0B32FCFF      CALL .00404950
 *  00441745   8B5C24 20        MOV EBX,DWORD PTR SS:[ESP+0x20]
 *  00441749   8B4C24 18        MOV ECX,DWORD PTR SS:[ESP+0x18]
 *  0044174D   8B7C24 24        MOV EDI,DWORD PTR SS:[ESP+0x24]
 *  00441751   8BC3             MOV EAX,EBX
 *  00441753   2BC1             SUB EAX,ECX
 *  00441755   8BCF             MOV ECX,EDI
 *  00441757   99               CDQ
 *  00441758   2BC2             SUB EAX,EDX
 *  0044175A   8B5424 14        MOV EDX,DWORD PTR SS:[ESP+0x14]
 *  0044175E   F7D9             NEG ECX
 *  00441760   D1F8             SAR EAX,1
 *  00441762   03CA             ADD ECX,EDX
 *  00441764   8B5424 10        MOV EDX,DWORD PTR SS:[ESP+0x10]
 *  00441768   F7D8             NEG EAX
 *  0044176A   03C2             ADD EAX,EDX
 *  0044176C   83EC 10          SUB ESP,0x10
 *  0044176F   8D7C39 02        LEA EDI,DWORD PTR DS:[ECX+EDI+0x2]
 *  00441773   8D5418 02        LEA EDX,DWORD PTR DS:[EAX+EBX+0x2]
 *  00441777   8BDC             MOV EBX,ESP
 *  00441779   8903             MOV DWORD PTR DS:[EBX],EAX
 *  0044177B   894B 04          MOV DWORD PTR DS:[EBX+0x4],ECX
 *  0044177E   8BCE             MOV ECX,ESI
 *  00441780   8953 08          MOV DWORD PTR DS:[EBX+0x8],EDX
 *  00441783   897B 0C          MOV DWORD PTR DS:[EBX+0xC],EDI
 *  00441786   E8 7517FCFF      CALL .00402F00
 *  0044178B   EB 18            JMP SHORT .004417A5
 *  0044178D   8D41 29          LEA EAX,DWORD PTR DS:[ECX+0x29]
 *  00441790   8D14C5 00000000  LEA EDX,DWORD PTR DS:[EAX*8]
 *  00441797   2BD0             SUB EDX,EAX
 *  00441799   391C96           CMP DWORD PTR DS:[ESI+EDX*4],EBX
 *  0044179C   74 07            JE SHORT .004417A5
 *  0044179E   41               INC ECX
 *  0044179F   898E 08020000    MOV DWORD PTR DS:[ESI+0x208],ECX
 *  004417A5   8B86 E8020000    MOV EAX,DWORD PTR DS:[ESI+0x2E8]
 *  004417AB   33DB             XOR EBX,EBX
 *  004417AD   3BC3             CMP EAX,EBX
 *  004417AF   74 2A            JE SHORT .004417DB
 *  004417B1   399E C8030000    CMP DWORD PTR DS:[ESI+0x3C8],EBX
 *  004417B7   75 22            JNZ SHORT .004417DB
 *  004417B9   8B86 C4030000    MOV EAX,DWORD PTR DS:[ESI+0x3C4]
 *  004417BF   8BCE             MOV ECX,ESI
 *  004417C1   50               PUSH EAX
 *  004417C2   E8 89040000      CALL .00441C50
 *  004417C7   3B86 3C020000    CMP EAX,DWORD PTR DS:[ESI+0x23C]
 *  004417CD   74 06            JE SHORT .004417D5
 *  004417CF   8986 38020000    MOV DWORD PTR DS:[ESI+0x238],EAX
 *  004417D5   8986 3C020000    MOV DWORD PTR DS:[ESI+0x23C],EAX
 *  004417DB   399E 30020000    CMP DWORD PTR DS:[ESI+0x230],EBX
 *  004417E1   75 3C            JNZ SHORT .0044181F
 *  004417E3   8BCE             MOV ECX,ESI
 *  004417E5   E8 C6040000      CALL .00441CB0
 *  004417EA   85C0             TEST EAX,EAX
 *  004417EC   75 31            JNZ SHORT .0044181F
 *  004417EE   399E 18020000    CMP DWORD PTR DS:[ESI+0x218],EBX
 *  004417F4   74 29            JE SHORT .0044181F
 *  004417F6   83BE C4020000 64 CMP DWORD PTR DS:[ESI+0x2C4],0x64
 *  004417FD   74 20            JE SHORT .0044181F
 *  004417FF   8B86 08020000    MOV EAX,DWORD PTR DS:[ESI+0x208]
 *  00441805   83F8 20          CMP EAX,0x20
 *  00441808   7D 1D            JGE SHORT .00441827
 *  0044180A   83C0 29          ADD EAX,0x29
 *  0044180D   8D0CC5 00000000  LEA ECX,DWORD PTR DS:[EAX*8]
 *  00441814   2BC8             SUB ECX,EAX
 *  00441816   391C8E           CMP DWORD PTR DS:[ESI+ECX*4],EBX
 *  00441819   74 0C            JE SHORT .00441827
 *  0044181B   6A 01            PUSH 0x1
 *  0044181D   EB 01            JMP SHORT .00441820
 *  0044181F   53               PUSH EBX
 *  00441820   8BCE             MOV ECX,ESI
 *  00441822   E8 49C5FEFF      CALL .0042DD70
 *  00441827   8D4C24 58        LEA ECX,DWORD PTR SS:[ESP+0x58]
 *  0044182B   C74424 64 030000>MOV DWORD PTR SS:[ESP+0x64],0x3
 *  00441833   E8 F24C0400      CALL .0048652A
 *  00441838   8D4C24 4C        LEA ECX,DWORD PTR SS:[ESP+0x4C]
 *  0044183C   C64424 64 02     MOV BYTE PTR SS:[ESP+0x64],0x2
 *  00441841   E8 E44C0400      CALL .0048652A
 *  00441846   8D4C24 48        LEA ECX,DWORD PTR SS:[ESP+0x48]
 *  0044184A   C74424 64 FFFFFF>MOV DWORD PTR SS:[ESP+0x64],-0x1
 *  00441852   E8 D34C0400      CALL .0048652A
 *  00441857   8B4C24 5C        MOV ECX,DWORD PTR SS:[ESP+0x5C]
 *  0044185B   5F               POP EDI
 *  0044185C   5E               POP ESI
 *  0044185D   5D               POP EBP
 *  0044185E   64:890D 00000000 MOV DWORD PTR FS:[0],ECX
 *  00441865   5B               POP EBX
 *  00441866   83C4 58          ADD ESP,0x58
 *  00441869   C2 1400          RETN 0x14
 *  0044186C   90               NOP
 *  0044186D   90               NOP
 *  0044186E   90               NOP
 *  0044186F   90               NOP
 *
 *  Another sample game: 不条琸�界の探偵令嬢
 */
bool InsertAbelHook()
{
  // jichi: If this pattern failed again, try the following pattern instead:
  // 004413D3   894424 48        MOV DWORD PTR SS:[ESP+0x48],EAX
  // 004413D7   894424 4C        MOV DWORD PTR SS:[ESP+0x4C],EAX
  // 004413DB   894424 58        MOV DWORD PTR SS:[ESP+0x58],EAX

  const DWORD character[] = {0xc981d48a, 0xffffff00};
  if (DWORD j = SearchPattern(processStartAddress, processStopAddress - processStartAddress, character, sizeof(character)))
  {
    j += processStartAddress;
    for (DWORD i = j - 0x100; j > i; j--)
      if (*(WORD *)j == 0xff6a)
      {
        HookParam hp;
        hp.address = j;
        hp.offset = stackoffset(1);
        hp.type = USING_STRING | NO_CONTEXT;
        return NewHook(hp, "Abel");
      }
  }
  ConsoleOutput("Abel: failed");
  return false;
}

bool Abel::attach_function()
{

  return InsertAbelHook();
}