#pragma once
#include <vector>
#include"opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
class CCommon
{
public:
	CCommon();
	~CCommon();
public:
	static BOOL gr_create_folder(CString csPath);
	static BOOL CreateDirectoryNested(LPCTSTR  lpszDir);
	template<size_t nSize>
	static void ModifyPathSpec(TCHAR(&szDst)[nSize], BOOL  bAddSpec);
	static void SCanDiskFile(const CString& strPath, vector<CString>&vec, CString strType);
	static BOOL findElement(vector<CString>& beg, CString elem);
	static CString GetExtName(CString strPath);
	static int gr_cstring_to_pchar(CString str, char* p, int len);
	static CString SCanFinalFile(const CString& strPath);
	static CString GetAngle(CString anglePathTemp);
	
	
};

