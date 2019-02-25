#include "StdAfx.h"
#include "HGTools.h"
#include <WinBase.h>

//����ָ��Ŀ¼
void JT2_Tools::createDir(CString path)
{
	if (!JT2_Tools::isDir(path))
	{
		path = "\""+ path +"\"";
		char temp[512] = {0};
		temp[0] = 'm';
		temp[1] = 'd';
		temp[2] = ' ';
		JT2_Tools::Convert2FillChar(path,temp+3);
		system(temp);
	}
}

//ɾ�����Ŀ¼�µ��ļ�
void JT2_Tools::deleteFile(CString path)
{
	DeleteFile(path);
}

//ɾ�����Ŀ¼�Լ����������ļ�
bool JT2_Tools::deletePath(CString path)
{
	char DirName[MAX_PATH];
	Convert2FillChar(path,DirName);

	char szCurPath[MAX_PATH];       //���ڶ���������ʽ  
    _snprintf(szCurPath, MAX_PATH, "%s//*.*", DirName); //ƥ���ʽΪ*.*,����Ŀ¼�µ������ļ�  
    WIN32_FIND_DATAA FindFileData;        
    ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));  
    HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData);  
    BOOL IsFinded = TRUE;  
    while(IsFinded)  
    {  
        IsFinded = FindNextFileA(hFile, &FindFileData); //�ݹ������������ļ�  
        if( strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..") ) //�������"." ".."Ŀ¼  
        {  
            std::string strFileName = "";  
            strFileName = strFileName + DirName + "//" + FindFileData.cFileName;  
            std::string strTemp;  
            strTemp = strFileName;  
            if( isDir(strFileName.c_str()) ) //�����Ŀ¼����ݹ�ص���  
            {     
                printf("Ŀ¼Ϊ:%s/n", strFileName.c_str());  
                deletePath(strTemp.c_str());  
            }  
            else  
            {  
                DeleteFileA(strTemp.c_str());  
            }  
        }  
    }  
    FindClose(hFile);   
	return TRUE;  
}

//ɾ�����Ŀ¼���������ļ�(��������Ŀ¼)
void JT2_Tools::deletePathFiles(CString path)
{
	if (JT2_Tools::isDir(path))
	{
		path = "\""+ path +"\\*.*\" \/Q";
		char temp[512] = {0};
		temp[0] = 'd';
		temp[1] = 'e';
		temp[2] = 'l';
		temp[3] = ' ';
		JT2_Tools::Convert2FillChar(path,temp+4);
		system(temp);
		
	}
}

//�ж����·���Ƿ���һ���ļ�
bool JT2_Tools::isFileExists(CString path)
{
	return PathFileExists(path);
}
//�ж����·���Ƿ���һ��Ŀ¼
bool JT2_Tools::isDir(CString path)
{
	return PathIsDirectory(path);
}

//��ȡ��ǰ����EXE����·���������·��
CString JT2_Tools::getRelativePath(CString path)
{
	return getCurExeContainPath() + "\\" + path ;
}
//��ȡ��ǰ����EXE����·��
CString JT2_Tools::getCurExeFullPath()
{
	char ExeFile[2000];
	GetModuleFileName(NULL,ExeFile,500);
	CString file_path = ExeFile;

	return file_path;
}
//��ȡ��ǰ����EXE����·����Ŀ¼
CString JT2_Tools::getCurExeContainPath()
{
	CString file_path = getCurExeFullPath();
	file_path = getContainPath(file_path);

	return file_path;
}

//��ȡ�ļ�����·����Ŀ¼
CString JT2_Tools::getContainPath(CString path)
{
	CString file_path = path;
	file_path = file_path.Left(file_path.ReverseFind('\\'));
	return file_path;
}


//��ȡ�ļ����ƴ���׺��
CString JT2_Tools::getFileNameWithSuffix(CString path)
{
	CString file_path = path;
	file_path = file_path.Mid(file_path.ReverseFind('\\')+1);
	return file_path;
}
//��ȡ�ļ����Ʋ�����׺��
CString JT2_Tools::getFileName(CString path)
{
	CString file_path = getFileNameWithSuffix(path);
	file_path = file_path.Left(file_path.ReverseFind('.'));
	return file_path;
}
//��ó���ǰ�汾��
CString JT2_Tools::getVersion()
{
	return " v2.0";
}

//��CString ת���� char[]����
char* JT2_Tools::Convert2FillChar(CString src,char* dst)
{
	std::string src_string = src.GetBuffer();
	int i = 0;
	for(;i < src_string.size();i++)
	{
		dst[i] = src_string.at(i);
	}
	dst[i] = '\0';
	return dst;
}

//ת��Ϊstd::string
std::string JT2_Tools::Convert2StdString(int src)
{
	CString temp;
	temp.Format("%d",src);
	return temp.GetBuffer();
}

//ת��Ϊstd::string
std::string JT2_Tools::Convert2StdString(float src)
{
	CString temp;
	temp.Format("%f",src);
	return temp.GetBuffer();
}

//ת��ΪChar*
const char* JT2_Tools::Convert2Char(CString src)
{
	std::string temp = src.GetBuffer();
	return temp.c_str();
}
//ת��Ϊstd::string
std::string JT2_Tools::Convert2StdString(CString src)
{
	return src.GetBuffer();
}

//ת��Ϊstd::string
std::string JT2_Tools::Convert2StdString(char* src)
{
	return std::string(src);
}

//ת��ΪCString
CString JT2_Tools::Convert2CString(char* src)
{
	CString temp;
	temp.Format("%s",src);
	return temp;
}
//ת��ΪCString
CString JT2_Tools::Convert2CString(std::string src)
{
	CString temp;
	temp.Format("%s",src.c_str());
	return temp;
}

//��hgx�ļ�Ŀ¼����xml�ļ�Ŀ¼
CString JT2_Tools::getXmlPathFromHgxPath(CString pathName)
{
#ifndef YiWei
	if (pathName.Right(4).Left(3) = ".hg")//ƥ�� ".hg*"
#else
	if (pathName.Right(4).Left(3) = ".yw")
#endif
	
	{
		pathName = pathName.Left(pathName.GetLength()-4) + ".xml";
		return pathName;
	}
	return pathName;
}
//�������·����".",".."��·��ת�������ֱ���Ǵ��ַ��
CString JT2_Tools::getRealPathFromReleativePath(CString path)
{
	char tempPath[512] = {0};
	return PathCombine(tempPath,path,NULL);
}
////�������·����".",".."��·��ת�������ֱ���Ǵ��ַ��
//CString HGTools::getReleativePathFromRealPath(CString path)
//{
//	return "";
//}



//�����С�������ű���
float JT2_Tools::getMinScale(CRect& putterRect, CRect& containerRect)
{
	float scale_w = putterRect.Width()/((float)(containerRect.Width()));
	float scale_h = putterRect.Height()/((float)(containerRect.Height()));

	float scale = scale_w > scale_h ? scale_w : scale_h;

	return scale;
}

//�����С�������ű���
float JT2_Tools::getMinScale(float putterRect_w,float putterRect_h, float containerRect_w, float containerRect_h)
{
	float scale_w = putterRect_w/containerRect_w;
	float scale_h = putterRect_h/containerRect_h;

	float scale = scale_w > scale_h ? scale_h : scale_w;

	return scale;
}

//�������������ű���
float JT2_Tools::getMaxScale(float putterRect_w,float putterRect_h, float containerRect_w, float containerRect_h)
{
	float scale_w = containerRect_w/putterRect_w;
	float scale_h = containerRect_h/putterRect_h;

	float scale = scale_w < scale_h ? scale_w : scale_h;

	return scale;
}


//���ַ����������ķ��Ž��в��
int JT2_Tools::SplitCString(CString strIn, CStringArray& strAryRe, char division)
{
	strAryRe.RemoveAll();
	if (!strIn.IsEmpty())
	{
		int nCount = 0;
		int nPos = -1;
		nPos = strIn.Find(division);
		CString strTemp = strIn;
		while (nPos != -1)
		{
			CString strSubString = strTemp.Left(nPos);   
			strTemp = strTemp.Right(strTemp.GetLength() - nPos-1);   
			nPos = strTemp.Find(division); 
			nCount++;
			strAryRe.Add(strSubString);
		}

		if (nCount == strAryRe.GetSize())
		{
			CString str;
			int nSize = strIn.ReverseFind(division);
			str = strIn.Right(strIn.GetLength()-nSize-1);
			strAryRe.Add(str);
		}	
	}

	return strAryRe.GetSize();
}

//���ַ����������ķ��Ž��в��
int JT2_Tools::SplitCString(CString &strIn, CStringArray& strAryRe, LPCTSTR str)
{
	if (str == _T(""))
	{
		strAryRe.RemoveAll();
		return 0;
	}

	int nStart = 0;
	int nEnd = 0;
	CString strTmp;
	while ( (nEnd = strIn.Find(str, nStart)) != -1 )
	{
		strTmp = strIn.Mid(nStart, nEnd-nStart);
		strAryRe.Add(strTmp);
		nStart = nEnd+2;
	}
	strTmp = strIn.Mid(nStart, strIn.GetLength());
	strAryRe.Add(strTmp);

	return strAryRe.GetSize();

}

//����෴��
std::string JT2_Tools::GetNagativeValue( std::string value )
{
	float val = atof(value.c_str());
	val = val*-1;
	CString output;
	output.Format("%f",val);
	return output.GetBuffer();
}

//����ļ�����󱣴�ʱ��
long long JT2_Tools::GetFileLastTime(const CString filePath)
{
	WIN32_FIND_DATAA FindFileData;
	::memset(&FindFileData,0,sizeof(FindFileData));
	::FindClose(::FindFirstFileA(filePath,&FindFileData));
	FILETIME& lastWriteTime = FindFileData.ftLastWriteTime;
	return (((long long)lastWriteTime.dwHighDateTime)<<32)+lastWriteTime.dwLowDateTime;
}