/*--------------------------------------------------------------------------------------------------------------------*/
//	PltReadWrite.h      -- plt�ļ���д�������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2017.7.21
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_PltReadWrite_H_
#define		_PltReadWrite_H_


class Panel;

class AFX_EXT_CLASS PltReadWrite
{
public:
	static bool OutputPlt(Panel* pPanel, CString strPltFilePath);	

};



#endif