// RasterPane.cpp : implementation file
//

#include "stdafx.h"
#include "Rs.h"
#include "RasterPane.h"


// CRasterPane

IMPLEMENT_DYNAMIC(CRasterPane, CDockablePane)

CRasterPane::CRasterPane()
{

}

CRasterPane::~CRasterPane()
{
}


BEGIN_MESSAGE_MAP(CRasterPane, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CRasterPane message handlers




void CRasterPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_ctrlImageList.GetSafeHwnd() != NULL)
	{
		CRect rectClient;
		GetClientRect(rectClient);
		m_ctrlImageList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	}
}


int CRasterPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if (!m_ctrlImageList.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT/*|LVS_NOCOLUMNHEADER*/, CRect(0, 0, 0, 0), this, 0))
	{
		TRACE0("����Ӱ���б�ʧ��\n");
		return -1;
	}
	m_ctrlImageList.InsertColumn(0, _T("Ӱ���б�"));
	m_ctrlImageList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_ctrlImageList.SetColumnWidth(0, 500);
	return 0;
}
