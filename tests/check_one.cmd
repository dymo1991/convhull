Convex_Hull.exe < %1 > %1.test

if not exist %1.ok goto done
det %1.test %1.ok
if ERRORLEVEL 1 goto error

goto done
:error

echo Error on test %1
pause

:done



