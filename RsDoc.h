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
#include <algorithm>
#include <fstream>


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

struct PolygonExt
{
	PolygonExt(int point_count, double* px, double* py)
		: point_count_(point_count)
		, px_(px)
		, py_(py)
		, index_name_("")
	{

	}

	PolygonExt(int point_count, double* px, double* py, CString index_name)
		: point_count_(point_count)
		, px_(px)
		, py_(py)
		, index_name_(index_name)
	{

	}

	void Free()
	{
		point_count_ = 0;
		delete []px_;
		px_ = NULL;
		py_ = NULL;
	}

	int point_count_;
	double* px_;
	double* py_;
	CString index_name_;
};

struct NodeProperty
{
	NodeProperty(int shared_by)
		: shared_by_(shared_by)
		, available_(true)
	{

	}
	int shared_by_;
	CString index_name_n_[3];
	bool available_;
};

static bool NotImportant(NodeProperty& center, NodeProperty& left, NodeProperty& right)
{
	CString leftstr = center.index_name_n_[0];

	if (leftstr != left.index_name_n_[0] && leftstr != left.index_name_n_[1]
	&& leftstr != left.index_name_n_[2])
	{
		return false;
	}

	if (leftstr != right.index_name_n_[0] && leftstr != right.index_name_n_[1]
	&& leftstr != right.index_name_n_[2])
	{
		return false;
	}

	return true;
}

struct PolygonExt2
{
	PolygonExt2(int point_count, double* px, double* py, CString index_name)
		: point_count_(point_count)
		, px_(px)
		, py_(py)
		, index_name_(index_name)
	{
		np_.resize(point_count_, 1);
	}

	void DeletePoint()
	{
		int temp_point_count = 0;
		std::for_each(np_.begin(), np_.end(),
			[&temp_point_count](NodeProperty np)
		{
			if (np.available_)
			{
				++temp_point_count;
			}
		});

		double* temp_px = new double[temp_point_count];
		double* temp_py = new double[temp_point_count];
		memset(temp_px, 0, sizeof(double)*temp_point_count);
		memset(temp_py, 0, sizeof(double)*temp_point_count);

		for (int i = 0, offset = 0; i < temp_point_count; ++i)
		{
			while(!np_[i+offset].available_)
			{
				++offset;
			}
			temp_px[i] = px_[i+offset];
			temp_py[i] = py_[i+offset];
		}

		point_count_ = temp_point_count;
		delete []px_;
		delete []py_;
		px_ = temp_px;
		py_ = temp_py;

		auto ite = np_.begin();
		while (ite != np_.end())
		{
			if (ite->available_ == false)
			{
				ite = np_.erase(ite);
				continue;
			}
			++ite;
		}
	}
	void Output(CString path)
	{
		path += index_name_;
		path += _T(".rrlx");
		std::fstream outfile;
		outfile.open(path.LockBuffer(), std::ios::out);
		if (outfile != NULL)
		{
			outfile<<std::fixed;
			outfile<<point_count_<<std::endl;
			for (int i = 0; i < point_count_; ++i)
			{
				outfile<<px_[i]<<"   "<<py_[i]<<"   "<<0<<std::endl;
			}
		}
		outfile.close();
	}
	void ResetPoint(CString index, double px, double py, double newx, double newy)
	{
		if (index == index_name_ || index == _T(""))
		{
			for (int i = 0; i < point_count_; ++i)
			{
				if (px_[i] == px && py_[i] == py)
				{
					px_[i] = newx;
					py_[i] = newy;
				}
			}
		}
	}
	void Free()
	{
		delete []px_;
		delete []py_;
		px_ = NULL;
		py_ = NULL;
	}

	bool operator==(const PolygonExt2& poly)const
	{
		return index_name_ == poly.index_name_;
	}

	int point_count_;
	double* px_;
	double* py_;
	CString index_name_;
	std::vector<NodeProperty> np_;
};

static bool Compare(double ax, double ay, double bx, double by)
{
	return (ay < by) || (fabs(ay-by) < 0.000001 && ax < bx);
}

static double cross(double ox, double oy, double ax, double ay, double bx, double by)
{
	return (ax-ox)*(by-oy)-(ay-oy)*(bx-ox);
}

static double length2(double ax, double ay, double bx, double by)
{
	return (ax-bx)*(ax-bx)+(ay-by)*(ay-by);
}

static bool farer(double ox, double oy, double ax, double ay, double bx, double by)
{
	return length2(ox, oy, ax, ay) > length2(ox, oy, bx, by);
}

static void ConvexHull(PolygonExt& poly)
{
	double* px = poly.px_;
	double* py = poly.py_;
	int point_count = poly.point_count_;

	int start = 0;
	for (int i = 1; i < point_count; ++i)
	{
		if (Compare(px[i], py[i], px[start], py[start]))
		{
			start = i;
		}
	}

	std::vector<int> index;
	index.push_back(start);

	int m = 1;
	while (true)
	{
		int next = start;
		for (int i = 0; i < point_count;++i)
		{
			double c = cross(px[index[m-1]], py[index[m-1]], px[i], py[i], px[next], py[next]);
			if (c > 0 ||
				c == 0 &&
				farer(px[index[m-1]], py[index[m-1]], px[i], py[i], px[next], py[next]))
			{
				next = i;
			}
		}
		if (next == start)
		{
			break;
		}
		index.push_back(next);
		++m;
	}

	poly.point_count_ = index.size();
	double* tempx = new double[index.size()];
	double* tempy = new double[index.size()];
	for (unsigned int i = 0; i < index.size(); ++i)
	{
		tempx[i] = px[index[i]];
		tempy[i] = py[index[i]];
	}
	delete []poly.px_;
	delete []poly.py_;
	poly.px_ = tempx;
	poly.py_ = tempy;
}

static void ConvexHull(PolygonExt2& poly)
{
	double* px = poly.px_;
	double* py = poly.py_;
	int point_count = poly.point_count_;

	int start = 0;
	for (int i = 1; i < point_count; ++i)
	{
		if (Compare(px[i], py[i], px[start], py[start]))
		{
			start = i;
		}
	}

	std::vector<int> index;
	index.push_back(start);

	int m = 1;
	while (true)
	{
		int next = start;
		for (int i = 0; i < point_count;++i)
		{
			double c = cross(px[index[m-1]], py[index[m-1]], px[i], py[i], px[next], py[next]);
			if (c > 0 ||
				c == 0 &&
				farer(px[index[m-1]], py[index[m-1]], px[i], py[i], px[next], py[next]))
			{
				next = i;
			}
		}
		if (next == start)
		{
			break;
		}
		index.push_back(next);
		++m;
	}

	poly.point_count_ = index.size();
	double* tempx = new double[index.size()];
	double* tempy = new double[index.size()];
	for (unsigned int i = 0; i < index.size(); ++i)
	{
		tempx[i] = px[index[i]];
		tempy[i] = py[index[i]];
	}
	delete []poly.px_;
	delete []poly.py_;
	poly.px_ = tempx;
	poly.py_ = tempy;
}

static PolygonExt2 Intersects(PolygonExt2& poly1, PolygonExt2& poly2)
{

}

struct PointEx{
	PointEx(){x = 0; y = 0;}
	PointEx(int xpos, int ypos):x(xpos), y(ypos){}
	int x;
	int y;

	bool operator==(const PointEx& pt)const {return (x == pt.x && y == pt.y);}
	void operator=(const PointEx& pt){x = pt.x; y = pt.y;}
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
	CDrawing m_dxffile;
	std::vector<CString> m_vecImagePath;
	std::vector<CString> m_vecShapePath;
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

	long m_nRealOriginx;
	long m_nRealOriginy;

	unsigned char* m_pCurBuf;
	unsigned char* m_pBacBuf;
	RectFExt m_recBac;
	RectFExt m_recCur;
	int m_nRed;
	int m_nGreen;
	int m_nBlue;

	int m_nRealBandNum;

	BOOL m_bIsReady;

	int* m_pRasterState;
	int* m_pVectorState;

	std::vector<double*> m_vecX;
	std::vector<double*> m_vecY;
	std::vector<int> m_vecPointNum;
	std::vector<int> m_vecPolygonNum;

	void  FillData(RectFExt rect);
	void Geo2Buf(RectFExt rect, double lfxpos, double lfypos, long &nxpos, long &nypos);

public:
	std::vector<RectFExt> m_vecImageRect;
	std::vector<PolygonExt> m_vecMosaicLine;
	std::vector<PolygonExt> m_vecEffectivePoly;

public:
	void GetBufSize(int &nBufWidth, int &nBufHeight);
	unsigned char* GetData();
	void Screen2Geo(long nxpos, long nypos, double &lfxpos, double &lfypos);
	void Geo2Screen(double lfxpos, double lfypos, long &nxpos, long &nypos);
	void ZoomOut(CPoint &pt);
	void ZoomIn(CPoint &pt);
	BOOL IsGrey();
	BOOL IsReady(); 
	void SetReady(BOOL bIsReady);
	void GetRealOrigin(long& nRealOriginx, long& nRealOriginy);
	void SetRealOrigin(long nRealOriginx, long nRealOriginy);
	void GetViewScale(double& lfScale);
	void UpdateData();
	void UpdateRasterList();
	void UpdateVectorList();
	void SetRasterState(int nIndex, BOOL bState);
	void SetVectorState(int nIndex, BOOL bState);
	void UpdateRasterState();
	void UpdateVectorState();
	int* GetRasterState();
	int* GetVectorState();
	void GetShapeIterator(std::vector<double*>::iterator& iteX, std::vector<double*>::iterator& iteY, std::vector<int>::iterator& iteNum, std::vector<int>::iterator& itePolyNum);
	void GetShapeIterEnd(std::vector<double*>::iterator& iteX, std::vector<double*>::iterator& iteY, std::vector<int>::iterator& iteNum, std::vector<int>::iterator& itePolyNum);
	std::vector<PolygonExt>& GetPolygonVec();
	std::vector<PolygonExt>& GetEffectivepoly();
	void ParsePolygon();
	void ParseEffective();
	void OutputResultImg(std::vector<PolygonExt2>& polygons);


// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileOpen();
	afx_msg void OnBandcomb();
	afx_msg void OnAddraster();
	afx_msg void OnAddvector();
	afx_msg void OnGenerateline();
	afx_msg void OnDxf2dsm();
	afx_msg void OnOptimize();
	afx_msg void OnEffectpoly();
	afx_msg void OnOptimize2();
	afx_msg void OnLoadmosaic();
};
