format      PE GUI
include     'include\win32a.inc'
entry       $
            invoke ExitProcess,0
            ret
proc        callback,handle,reason,reserved
            cmp     [reason],DLL_PROCESS_ATTACH
            jnz     @f
            invoke  MessageBox,0,0,0,0
@@:         ret
endp
data        9
            dd a ; StartAddressOfRawData;
            dd a ; EndAddressOfRawData
            dd a ; AddressOfIndex
            dd c ; AddressOfCallBacks
a           dd 0 ;
c           dd callback ; Array Of Callbacks
            dd 0        ; NULL - end of Array Of Callbacks
end data
section '.idata' import data readable

  library kernel,'KERNEL32.DLL',\
          user,'USER32.DLL'

  import kernel,\
         ExitProcess,'ExitProcess'
  import user,\
         MessageBox,'MessageBoxA'
