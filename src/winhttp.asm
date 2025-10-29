.data
extern PA : qword
.code
ASMJmpToPA proc
jmp qword ptr [PA]
ASMJmpToPA endp
end