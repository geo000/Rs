// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// RsDoc.h : CRsDoc ��Ľӿ�
//


#pragma once


struct RectFExt
{
	RectFExt(){left = 0; right = 0; top = 0; bottom = 0;}
	RectFExt(double l, double r, double t, double b)
	{
		left = l;
		right = r;
		top = t;
		bottom = b;
	}
	double Width(){return (right-left);}
	double Height(){return (top-bottom);}
	bool IsEmpty()
	{
		if (right-left == 0 || top-bottom == 0)
		{
			return true;
		}
		return false;
	}
	bool Intersects(RectFExt& rect)
	{
		if ((left-rect.right)*(right-rect.left) < 0 && (top-rect.bottom)*(bottom-rect.top) < 0)
		{
			return true;
		}
		return false;
	}
	RectFExt Intersected(RectFExt& rect)
	{
		if (this->Intersects(rect))
		{
			return RectFExt(left > rect.left ? left : rect.left,
				right < rect.right ? right : rect.right,
				top < rect.top ? top : rect.top,
				bottom > rect.bottom ? bottom : rect.bottom);
		}
		return RectFExt();
	}
	void Free(){left = 0; right = 0; top = 0; bottom = 0;}

	double left;
	double right;
	double top;			//the big one
	double bottom;
};


class CRsDoc : public CDocument
{
protected: // �������л�����
	CRsDoc();
	DECLARE_DYNCREATE(CRsDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CRsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


private:
	
	IImageX* m_pImage;
	std::vector<CString> m_vecImagePath;
	double m_lfMinx;
	double m_lfMaxx;
	double m_lfMiny;
	double m_lfMaxy;
	double m_lfResolution;
	double m_lfScale;				//��ʾ�ֱ���
	int m_nBandNum;
	BOOL m_bIsGrey;

	int m_nBufWidth;
	int m_nBufHeight;
	int m_nCols;
	int m_nRows;
	int m_nWndWidth;
	int m_nWndHeight;
	int m_nScrollSizex;
	int m_nScrollSizey;

	double m_lfRealOriginx;
	double m_lfRealOriginy;

	unsigned char* m_pCurBuf;
	unsigned char* m_pBacBuf;
	RectFExt m_recBac;
	RectFExt m_recCur;

	BOOL m_bIsReady;

	void  FillData(RectFExt rect);
	void Geo2Buf(RectFExt rect, double lfxpos, double lfypos, long &nxpos, long &nypos);

public:
	std::vector<RectFExt> m_vecImageRect;

public:
	void GetBufSize(int &nBufWidth, int &nBufHeight);
	unsigned char* GetData();
	void Screen2Geo(long nxpos, long nypos, double &lfxpos, double &lfypos);
	void Geo2Screen(double lfxpos, double lfypos, double &lfxpos2, double &lfypos2);
	void ZoomOut(CPoint &pt);
	void ZoomIn(CPoint &pt);
	BOOL IsGrey();
	BOOL IsReady(); 
	void SetReady(BOOL bIsReady);
	void GetRealOrigin(double& lfRealOriginx, double& lfRealOriginy);
	void GetViewScale(double& lfScale);
	

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileOpen();
};
