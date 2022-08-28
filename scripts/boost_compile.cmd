
@rem b2.exe building...
call .\bootstrap.bat

@rem Win32 libraries building...
.\b2.exe --without-wave ^
    --without-python ^
    --without-graph_parallel ^
    --without-graph ^
    stage --stagedir=stage32 ^
    toolset=msvc ^
    variant=debug ^
    link=static ^
    threading=multi ^
    runtime-link=static

.\b2.exe --without-wave ^
    --without-python ^
    --without-graph_parallel ^
    --without-graph ^
    stage --stagedir=stage32 ^
    toolset=msvc ^
    variant=release ^
    link=static ^
    threading=multi ^
    runtime-link=static

@rem x64 libraries building...
.\b2.exe --without-wave ^
    --without-python ^
    --without-graph_parallel ^
    --without-graph ^
    stage --stagedir=stage64 ^
    toolset=msvc ^
    variant=debug ^
    link=static ^
    threading=multi ^
    runtime-link=static ^
    address-model=64

.\b2.exe --without-wave ^
    --without-python ^
    --without-graph_parallel ^
    --without-graph ^
    stage --stagedir=stage64 ^
    toolset=msvc ^
    variant=release ^
    link=static ^
    threading=multi ^
    runtime-link=static ^
    address-model=64


@rem MSVC 2008:
@rem Tools -> Options... -> Project and solutions -> VC++ Directories -> Library files

@rem MSVC 2010:
@rem 