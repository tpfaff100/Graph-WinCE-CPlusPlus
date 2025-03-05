<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: Graph - Win32 (WCE x86em) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"X86EMDbg/Graph.res" /d UNDER_CE=300 /d _WIN32_WCE=300 /d "UNICODE" /d "_UNICODE" /d "DEBUG" /d "WIN32_PLATFORM_PSPC" /d "_X86_" /d "x86" /d "i486" /r "C:\Documents and Settings\Administrator\Desktop\Graph\Graph.rc"" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP71F.tmp" with contents
[
/nologo /W3 /Zi /Od /D "DEBUG" /D "i486" /D UNDER_CE=300 /D _WIN32_WCE=300 /D "WIN32" /D "STRICT" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "WIN32_PLATFORM_PSPC" /D "UNICODE" /D "_UNICODE" /D "_X86_" /D "x86" /Fp"X86EMDbg/Graph.pch" /YX /Fo"X86EMDbg/" /Fd"X86EMDbg/" /Gz /c 
"C:\Documents and Settings\Administrator\Desktop\Graph\Graph.cpp"
"C:\Documents and Settings\Administrator\Desktop\Graph\StdAfx.cpp"
]
Creating command line "cl.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP71F.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP720.tmp" with contents
[
commctrl.lib coredll.lib corelibc.lib aygshell.lib /nologo /stack:0x10000,0x1000 /subsystem:windows /incremental:yes /pdb:"X86EMDbg/Graph.pdb" /debug /nodefaultlib:"OLDNAMES.lib" /nodefaultlib:libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib /nodefaultlib:oldnames.lib /out:"X86EMDbg/Graph.exe" /windowsce:emulation /MACHINE:IX86 
".\X86EMDbg\Graph.obj"
".\X86EMDbg\StdAfx.obj"
".\X86EMDbg\Graph.res"
]
Creating command line "link.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP720.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
Graph.cpp
C:\Documents and Settings\Administrator\Desktop\Graph\Graph.cpp(221) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
C:\Documents and Settings\Administrator\Desktop\Graph\Graph.cpp(221) : warning C4244: 'argument' : conversion from 'double' to 'int', possible loss of data
C:\Documents and Settings\Administrator\Desktop\Graph\Graph.cpp(202) : warning C4101: 'ptTrig' : unreferenced local variable
C:\Documents and Settings\Administrator\Desktop\Graph\Graph.cpp(204) : warning C4101: 'y' : unreferenced local variable
C:\Documents and Settings\Administrator\Desktop\Graph\Graph.cpp(203) : warning C4101: 'i' : unreferenced local variable
StdAfx.cpp
Linking...



<h3>Results</h3>
Graph.exe - 0 error(s), 5 warning(s)
</pre>
</body>
</html>
