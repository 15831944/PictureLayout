/*--------------------------------------------------------------------------------------------------------------------*/
//	PdfReadWrite.h      -- pdf�ļ���д�������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2017.7.21
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_PdfReadWrite_H_
#define		_PdfReadWrite_H_


using namespace std ;
class Panel;

class AFX_EXT_CLASS PdfReadWrite
{
public:
	static bool OutputPdf(Panel* pPanel, string strPdfFilePath);	

};



#endif