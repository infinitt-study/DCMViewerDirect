
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

#define TR_OW	0x574f
#define TR_AS	0x5341
#define TR_PN	0x4e50
#define TR_AT	0x5441
#define TR_SH	0x4853
#define TR_CS	0x5343
#define TR_SL	0x4c53
#define TR_DS	0x5344
#define TR_SQ	0x5153
#define TR_DA	0x4144
#define TR_SS	0x5353
#define TR_DT	0x5444
#define TR_ST	0x5453
#define TR_FL	0x4c46
#define TR_TM	0x4d54
#define TR_FD	0x4446
#define TR_UI	0x4955
#define TR_IS	0x5349
#define TR_UL   0x4c55
#define TR_LO	0x4f4c
#define TR_UN	0x4e55
#define TR_LT	0x544c
#define TR_US	0x5355
#define TR_OB	0x424f

BOOL CDCMViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CFile file;
	LPCTSTR lpszPathName = _T("dcm-file\\MRBRAIN.DCM");
	if (file.Open(lpszPathName, CFile::modeRead | CFile::typeBinary)) {
		
		file.Seek(128, CFile::begin);
		int32_t dicmTag = 0;
		short tr;
		short size;
		long value;

		file.Read(&dicmTag, sizeof(dicmTag));
		if (dicmTag == 0x4d434944) {
			TRACE("DICOM File Format Success\n");
		}
		
		for (int i = 0; i < 2;i++) {
			file.Read(&dicmTag, sizeof(dicmTag));
			if (dicmTag == 0x00000002) {
				file.Read(&tr, sizeof(tr));

				switch (tr) {
				case TR_UL:
					file.Read(&size, sizeof(size));
					file.Read(&value, sizeof(value));
					TRACE("DICOM TR_UL = [%d]\n", value);
					break;
				}
			}
			else if (dicmTag == 0x00010002) {
				file.Read(&tr, sizeof(tr));

				switch (tr) {
				case TR_OB:
				{
					short vr_lo, vr_hi;
					file.Read(&vr_lo, sizeof(vr_lo));
					file.Read(&vr_hi, sizeof(vr_hi));
					if (vr_lo == 0) {
						//2byte skip 
						file.Seek(2, CFile::current);
					}
					else {
						file.Seek(vr_lo, CFile::current);
					}

					vector<char> array(vr_hi);

					file.Read(array.data(), vr_hi);
					TRACE("DICOM TR_OB = size %d\n", vr_hi);
					for (const auto& contents : array) {
						TRACE("DICOM TR_OB = [%x]\n", contents);
					}
				}
					break;
				}
			}
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
