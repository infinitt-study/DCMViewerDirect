
// DCMViewerView.cpp: CDCMViewerView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "DCMViewer.h"
#endif

#include "DCMViewerDoc.h"
#include "DCMViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDCMViewerView

IMPLEMENT_DYNCREATE(CDCMViewerView, CView)

BEGIN_MESSAGE_MAP(CDCMViewerView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDCMViewerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CDCMViewerView 생성/소멸

CDCMViewerView::CDCMViewerView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CDCMViewerView::~CDCMViewerView()
{
}

BOOL CDCMViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CDCMViewerView 그리기

void CDCMViewerView::OnDraw(CDC* pDC)
{
	CDCMViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->m_pDataElement == nullptr) return;

	void* lpvBits = pDoc->m_pDataElement->getImageData();
	{
		std::shared_ptr<BITMAPINFO> pBitmapInfo((BITMAPINFO*)malloc(sizeof(BITMAPINFO)));
		if (pBitmapInfo) {
			pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			pBitmapInfo->bmiHeader.biWidth = 1024;
			pBitmapInfo->bmiHeader.biHeight = -1024;
			pBitmapInfo->bmiHeader.biPlanes = 1;
			pBitmapInfo->bmiHeader.biBitCount = 24;
			pBitmapInfo->bmiHeader.biCompression = BI_RGB;
			pBitmapInfo->bmiHeader.biSizeImage = 0;
			pBitmapInfo->bmiHeader.biClrImportant = 0;
			pBitmapInfo->bmiHeader.biClrUsed = 0;
			pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
			pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;

			SetDIBitsToDevice(pDC->m_hDC,
				0, 30, 1024, 1024,
				0, 0, 0, 1024,
				lpvBits, pBitmapInfo.get(), DIB_RGB_COLORS);  // SetDIBitsToDevice 를 이용해 윈도우에 영상 전 시
		}
	}

}


// CDCMViewerView 인쇄


void CDCMViewerView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDCMViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CDCMViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CDCMViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CDCMViewerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDCMViewerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDCMViewerView 진단

#ifdef _DEBUG
void CDCMViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CDCMViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDCMViewerDoc* CDCMViewerView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDCMViewerDoc)));
	return (CDCMViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CDCMViewerView 메시지 처리기


void CDCMViewerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
}
