
// DCMViewerDoc.cpp: CDCMViewerDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "DCMViewer.h"
#endif

#include "DCMViewerDoc.h"
#include "DataElement.h"

#include <propkey.h>
#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDCMViewerDoc

IMPLEMENT_DYNCREATE(CDCMViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CDCMViewerDoc, CDocument)
END_MESSAGE_MAP()


// CDCMViewerDoc 생성/소멸

CDCMViewerDoc::CDCMViewerDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CDCMViewerDoc::~CDCMViewerDoc()
{
}



BOOL CDCMViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	std::vector<DataElementPtr> dataElements;
	CFile file;
	//LPCTSTR lpszPathName = _T("D:\\dicom-image-sample\\MRBRAIN.DCM");
	//LPCTSTR lpszPathName = _T("D:\\dicom-image-sample\\0015.DCM");
	LPCTSTR lpszPathName = _T("D:\\dicom-image-sample\\0004.DCM");
	if (file.Open(lpszPathName, CFile::modeRead | CFile::typeBinary)) {
		
		file.Seek(128, CFile::begin);
		int32_t dicmTag = 0;

		file.Read(&dicmTag, sizeof(dicmTag));
		if (dicmTag != 0x4d434944) {
			TRACE("NOT DICOM File Format Error\n");
			return FALSE;
		}
		TRACE("DICOM File Format Success\n");

		//for (int i = 0; i < 85; i++) { //MRBRAIN.DCM
		//for (int i = 0; i < 76; i++) { //0015.dcm
		for (int i = 0; i < 30; i++) { //0004.dcm
			UINT readCount = file.Read(&dicmTag, sizeof(dicmTag));
			if (sizeof(dicmTag) != readCount) {
				TRACE("file read error : tag read\n");
				return FALSE;
			}

			DataElementPtr pDataElement = make_shared<DataElement>(dicmTag);
			if (false == pDataElement->Load(file)) {
				return FALSE;
			}

			dataElements.push_back(pDataElement);
		}

		file.Close();
	}

	return TRUE;
}




// CDCMViewerDoc serialization

void CDCMViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CDCMViewerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CDCMViewerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CDCMViewerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDCMViewerDoc 진단

#ifdef _DEBUG
void CDCMViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDCMViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDCMViewerDoc 명령


BOOL CDCMViewerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;


	return TRUE;
}
