#include "stdafx.h"
#include "Common.h"


CCommon::CCommon()
{
}


CCommon::~CCommon()
{
}
//若bAddSpec 为真，则确保szDst路径末尾带有"/", 若bAddSpec为假，则去除lpszPath末尾的"/"或"/"
template<size_t nSize>
void CCommon::ModifyPathSpec(TCHAR(&szDst)[nSize], BOOL  bAddSpec)
{
	int nLen = lstrlen(szDst);
	ASSERT(nLen > 0);
	TCHAR  ch = szDst[nLen - 1];
	if ((ch == _T('//')) || (ch == _T('/')))
	{
		if (!bAddSpec)
		{
			szDst[nLen - 1] = _T('/0');
		}
	}
	else
	{
		if (bAddSpec)
		{
			szDst[nLen] = _T('//');
			szDst[nLen + 1] = _T('/0');
		}
	}
}
//嵌套创建文件夹
BOOL  CCommon::CreateDirectoryNested(LPCTSTR  lpszDir)
{
	if (::PathIsDirectory(lpszDir)) return TRUE;

	TCHAR   szPreDir[MAX_PATH];
	_tcscpy_s(szPreDir, lpszDir);
	//确保路径末尾没有反斜杠
	ModifyPathSpec(szPreDir, FALSE);

	//获取上级目录
	BOOL  bGetPreDir = ::PathRemoveFileSpec(szPreDir);
	if (!bGetPreDir) return FALSE;

	//如果上级目录不存在,则递归创建上级目录
	if (!::PathIsDirectory(szPreDir))
	{
		CreateDirectoryNested(szPreDir);
	}

	return ::CreateDirectory(lpszDir, NULL);
}

BOOL CCommon::gr_create_folder(CString csPath)
{
	return CreateDirectoryNested(csPath);
}
void CCommon::SCanDiskFile(const CString& strPath, vector<CString>& vec, CString strType)
{
	CFileFind find;
	CString strTemp = strPath;
	CString strDirectory = strPath + _T("\\*.*");
	CString strFile;
	BOOL IsFind = find.FindFile(strDirectory);
	while (IsFind)
	{
		IsFind = find.FindNextFile();

		if (find.IsDots())
		{
			continue;
		}
		else if (find.IsDirectory())
		{
			strFile = find.GetFileName();
			strTemp = strPath;
			strTemp = strTemp + _T("\\") + strFile;
			SCanDiskFile(strTemp, vec, strType);

		}
		else
		{
			// 			if (!findElement(m_vAllDict, strTemp))
			// 			{
			// 				m_vAllDict.push_back(strTemp);
			// 			}

			strFile = find.GetFilePath();
			CString ExternName = GetExtName(strFile);
			if (ExternName == strType)
			{
				vec.push_back(strFile);
			}
		}

	}
	find.Close();
}

BOOL CCommon::findElement(vector<CString>& beg, CString elem)
{
	vector<CString>::iterator Sbegin = beg.begin();
	vector<CString>::iterator Send = beg.end();
	for (; Sbegin != Send; Sbegin++)
	{
		if (elem == *Sbegin)
		{
			return true;
		}
	}
	return FALSE;
}

CString CCommon::GetExtName(CString strPath)
{
	int pos = strPath.Find('.');
	if (pos == -1)
	{
		return strPath;
	}
	else
	{
		return GetExtName(strPath.Mid(pos + 1));
	}

}
int CCommon::gr_cstring_to_pchar(CString str, char* p, int len)
{
	size_t sz;
	wcstombs_s(&sz, p, len, str, _TRUNCATE);
	return sz;
}
CString CCommon::SCanFinalFile(const CString& strPath)
{
	CString strRemain;
	int iPos = strPath.ReverseFind('\\');
	CString  m_szOrient = strPath.Mid(iPos + 1);//行走的方向
	return m_szOrient;
}
CString CCommon::GetAngle(CString anglePathTemp)
{
	
	int iPos = anglePathTemp.Find('_');
	anglePathTemp = anglePathTemp.Mid(iPos+1);
	iPos = anglePathTemp.Find('_');
	CString m_Angel = anglePathTemp.Mid(0,iPos);//行走的角度	
	return m_Angel;
}
/*
void CCommon::getJustCurrentFile(string path, vector<string>& files)
{
	//文件句柄 
	long  hFile = 0;
	//文件信息 
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				;
			}
			else
			{
				files.push_back(fileinfo.name);
				//files.push_back(p.assign(path).append("\\").append(fileinfo.name) ); 
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}*/
/*
char** CCommon::EnumFiles(const char *directory, int *count)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	char result[MAX_PATH][MAX_PATH];
	char **returnresult;
	char pattern[MAX_PATH];
	int i = 0, j;

	// 开始查找
	strcpy(pattern, directory);
	strcat(pattern, "\\*.*");
	hFind = FindFirstFile(pattern, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		*count = 0;
		return NULL;
	}
	else
	{
		do
		{
			strcpy(result[i++], FindFileData.cFileName);
		} while (FindNextFile(hFind, &FindFileData) != 0);
	}

	// 查找结束
	FindClose(hFind);

	// 复制到结果中
	returnresult = (char **)calloc(i, sizeof(char *));

	for (j = 0; j < i; j++)
	{
		returnresult[j] = (char *)calloc(MAX_PATH, sizeof(char));
		strcpy(returnresult[j], result[j]);
	}
	*count = i;
	return returnresult;
}
*/
