set date=%date:~0,4%%date:~5,2%%date:~8,2%
if %time:~0,2% leq 9 (set hour=0%time:~1,1%) else (set hour=%time:~0,2%)
set min=%time:~3,2%%time:~6,2%
set DST=%date%%hour%%min%

set Debug=�Ű����ҵ��Debug��(%DST%)

mkdir %Debug%  

::ע����дҪ������dll�ļ�
copy  "bin\AbnormalShapeManagerd.dll" 	"%Debug%"
copy  "bin\CppTestd.exe" 				"%Debug%"
copy  "bin\CSharpTest.exe" 				"%Debug%"
copy  "bin\DataManagerd.dll" 			"%Debug%"
copy  "bin\FileReadWrited.dll" 			"%Debug%"
copy  "bin\muParserd.dll" 				"%Debug%"
copy  "bin\NewUId.dll" 					"%Debug%"
copy  "bin\tinyxmld.dll" 				"%Debug%"
copy  "bin\pdflib.dll" 					"%Debug%"
copy  "bin\QRCoded.dll" 				"%Debug%"
copy  "bin\VBTest.exe" 					"%Debug%"




