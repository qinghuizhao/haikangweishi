#include "stdafx.h"
#include "Common.h"


CCommon::CCommon()
{
}


CCommon::~CCommon()
{
}
//��bAddSpec Ϊ�棬��ȷ��szDst·��ĩβ����"/", ��bAddSpecΪ�٣���ȥ��lpszPathĩβ��"/"��"/"
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
//Ƕ�״����ļ���
BOOL  CCommon::CreateDirectoryNested(LPCTSTR  lpszDir)
{
	if (::PathIsDirectory(lpszDir)) return TRUE;

	TCHAR   szPreDir[MAX_PATH];
	_tcscpy_s(szPreDir, lpszDir);
	//ȷ��·��ĩβû�з�б��
	ModifyPathSpec(szPreDir, FALSE);

	//��ȡ�ϼ�Ŀ¼
	BOOL  bGetPreDir = ::PathRemoveFileSpec(szPreDir);
	if (!bGetPreDir) return FALSE;

	//����ϼ�Ŀ¼������,��ݹ鴴���ϼ�Ŀ¼
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
	CString  m_szOrient = strPath.Mid(iPos + 1);//���ߵķ���
	return m_szOrient;
}
CString CCommon::GetAngle(CString anglePathTemp)
{
	
	int iPos = anglePathTemp.Find('_');
	anglePathTemp = anglePathTemp.Mid(iPos+1);
	iPos = anglePathTemp.Find('_');
	CString m_Angel = anglePathTemp.Mid(0,iPos);//���ߵĽǶ�	
	return m_Angel;
}
/*
void CCommon::getJustCurrentFile(string path, vector<string>& files)
{
	//�ļ���� 
	long  hFile = 0;
	//�ļ���Ϣ 
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

	// ��ʼ����
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

	// ���ҽ���
	FindClose(hFind);

	// ���Ƶ������
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
