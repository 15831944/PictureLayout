set date=%date:~0,4%%date:~5,2%%date:~8,2%
if %time:~0,2% leq 9 (set hour=0%time:~1,1%) else (set hour=%time:~0,2%)
set min=%time:~3,2%%time:~6,2%
set DST=%date%%hour%%min%

set Release=�Ű����ҵ��(%DST%)

mkdir %Release%  

::ע����дҪ������dll�ļ�
copy  "bin\AbnormalShapeManager.dll" 	"%Release%"
copy  "bin\CppTest.exe" 				"%Release%"
copy  "bin\CSharpTest.exe" 				"%Release%"
copy  "bin\DataManager.dll" 			"%Release%"
copy  "bin\FileReadWrite.dll" 			"%Release%"
copy  "bin\muParser.dll" 				"%Release%"
copy  "bin\NewUI.dll" 					"%Release%"
copy  "bin\tinyxml.dll" 				"%Release%"
copy  "bin\pdflib.dll" 					"%Release%"
copy  "bin\VBTest.exe" 					"%Release%"




