/*--------------------------------------------------------------------------------------------------------------------*/
//	PltReadWrite.h      -- plt�ļ���д�������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2019.7.21
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_PltReadWrite_H_
#define		_PltReadWrite_H_


using namespace std ;
class Panel;

class AFX_EXT_CLASS PltReadWrite
{
public:
	static bool OutputPlt(Panel* pPanel, string strPltFilePath);	

};



#endif