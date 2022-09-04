readpe - Read Portable Executable
========================

Extract and show information from PE32/PE32+ format executables.

Features
========

* Output format is JSON;
* Extract info from DOS, NT, Sections, Import, Export, TLS, etc headers;
* Detect hook function by binding import;
* Show offsets, timestamps, names, etc userful information;
* Show TLS callback offsets;
* Calculate and show entropy of sections;
* etc. See to ```readpe --help```


Exit codes
==========


```0 - Success
1 - The platform type is not supported
2 - This file is not PE32/PE32+ 
3 - The file is currupted
4 - Usage errors 
5 - Errors occurs during in runtime
```

Examples
========

DOS Header
----------

```$ readpe --dos virus.exe_```

```
"Dos-Header" : {
        "e_magic" : "0x5A4D",
        "e_cblp" : "0x80",
        "e_cp" : "0x1",
        "e_crlc" : "0x0",
        "e_cparhdr" : "0x4",
        "e_minalloc" : "0x10",
        "e_maxalloc" : "0xFFFF",
        "e_ss" : "0x0",
        "e_sp" : "0x140",
        "e_csum" : "0x0",
        "e_ip" : "0x0",
        "e_cs" : "0x0",
        "e_lfarlc" : "0x40",
        "e_ovno" : "0x0",
        "e_res" : [
            "0x0",
            "0x0",
            "0x0",
            "0x0"
        ],
        "e_oemid" : "0x0",
        "e_oeminfo" : "0x0",
        "e_res2" : [
            "0x0",
            "0x0",
            "0x0",
            "0x0",
            "0x0",
            "0x0",
            "0x0",
            "0x0",
            "0x0",
            "0x0"
        ],
        "e_lfanew" : "0x80"
    }
```

TLS callback functons
---------------------


```$ readpe --tls-detail virus2.exe_```
```
"tls-directory-details" : {
        "callbacks" : [
                {
                        "rva" : "0x1009",
                        "offset" : "0x209"
                }
        ]
}
```

Export
------

```$ readpe --export-detail virus3.exe_```
```
"export-directory-details" : [
        {
                "name" : "CopyLZFile",
                "ordinal" : "0x1",
                "forward-to" : "kernel32.CopyLZFile"
        },
        {
                "name" : "GetExpandedNameA",
                "ordinal" : "0x2",
                "forward-to" : "kernel32.GetExpandedNameA"
        },
        {
                "name" : "GetExpandedNameW",
                "ordinal" : "0x3",
                "forward-to" : "kernel32.GetExpandedNameW"
        },
        {
                "name" : "LZClose",
                "ordinal" : "0x4",
                "forward-to" : "kernel32.LZClose"
        },
        {
                "name" : "LZCloseFile",
                "ordinal" : "0x5",
                "forward-to" : "kernel32.LZCloseFile"
        },
        {
                "name" : "LZCopy",
                "ordinal" : "0x6",
                "forward-to" : "kernel32.LZCopy"
        },
        {
                "name" : "LZCreateFileW",
                "ordinal" : "0x7",
                "forward-to" : "kernel32.LZCreateFileW"
        },
        {
                "name" : "LZDone",
                "ordinal" : "0x8",
                "forward-to" : "kernel32.LZDone"
        },
        {
                "name" : "LZInit",
                "ordinal" : "0x9",
                "forward-to" : "kernel32.LZInit"
        },
        {
                "name" : "LZOpenFileA",
                "ordinal" : "0xA",
                "forward-to" : "kernel32.LZOpenFileA"
        },
        {
                "name" : "LZOpenFileW",
                "ordinal" : "0xB",
                "forward-to" : "kernel32.LZOpenFileW"
        },
        {
                "name" : "LZRead",
                "ordinal" : "0xC",
                "forward-to" : "kernel32.LZRead"
        },
        {
                "name" : "LZSeek",
                "ordinal" : "0xD",
                "forward-to" : "kernel32.LZSeek"
        },
        {
                "name" : "LZStart",
                "ordinal" : "0xE",
                "forward-to" : "kernel32.LZStart"
        }
]
```

Contact
=======

Send your advices me to base64_decode('bnR2aXNpZ290aEBnbWFpbC5jb20=')
