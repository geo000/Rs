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

// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// Rs.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

GLuint g_tex = 0;

int PtInRegionEx(double x, double y, double* pX, double* pY, int nSum, double lfSnap)
{
	if (nSum <= 0 || NULL == pX || NULL == pY)
	{
		return FALSE;
	}
	int i0,i1,i2, ret=0;
	double xmin1, xmax1, ymin1, ymax1;
	for( int i=0; i<nSum; i++ )
	{
		i0 = i;
		i1 = (i0+1)%nSum;

		//		if( pX[i0] == x && pY[i0] == y )
		if( _SnapSame( pX[i0],x,lfSnap ) && _SnapSame( pY[i0],y,lfSnap ) )
			return 0;

		if( pX[i0]<pX[i1] ){ xmin1 = pX[i0]; xmax1 = pX[i1]; }
		else { xmin1 = pX[i1]; xmax1 = pX[i0]; }
		if( pY[i0]<pY[i1] ){ ymin1 = pY[i0]; ymax1 = pY[i1]; }
		else { ymin1 = pY[i1]; ymax1 = pY[i0]; }
		//		if( y<ymin1 || y>ymax1 || x>xmax1 )continue;
		if( _SnapLarge2(ymin1,y,lfSnap) || _SnapLarge2(y,ymax1,lfSnap) || _SnapLarge2(x,xmax1,lfSnap) )
			continue;




		if( pY[i1] == pY[i0] )
			//		if( _SnapSame(pY[i1],pY[i0],lfSnap) )
		{
			//			if( pY[i0]==y )
			if( _SnapSame(pY[i0],y,lfSnap) )
			{
				//				if( ( pX[i0]>x && pX[i1]<x ) ||
				//					( pX[i0]<x && pX[i1]>x ) )
				if( ( _SnapLarge(pX[i0],x,lfSnap) && _SnapLarge(x,pX[i1],lfSnap) ) ||
					( _SnapLarge(x,pX[i0],lfSnap) && _SnapLarge(pX[i1],x,lfSnap) ) )
					return 0;
			}
		}
		//		if( y==ymin1 || y==ymax1 )
		else if( _SnapSame(y,ymin1,lfSnap) || _SnapSame(y,ymax1,lfSnap) )
		{
			if(_SnapSame(y,ymin1,lfSnap) && _SnapSame(y,ymax1,lfSnap)&&
				(( _SnapLarge(pX[i0],x,lfSnap) && _SnapLarge(x,pX[i1],lfSnap) ) ||
				( _SnapLarge(x,pX[i0],lfSnap) && _SnapLarge(pX[i1],x,lfSnap) )))
			{
				return 0;
			}
			//			if( y==pY[i0] && x<pX[i0] )
			if( _SnapSame(y,pY[i0],lfSnap) && _SnapLarge(pX[i0],x,lfSnap) )
			{
				i2 = i0;
				while( true )
				{
					i2 = (i2-1+nSum)%nSum;
					if( pY[i2] == pY[i0] )
					{
						if( i2 == i1 || i2 == i0 )break;
						continue;
					}
					if( ( _SnapLarge(y,pY[i2],lfSnap) && _SnapLarge(pY[i1],y,lfSnap) ) ||
						( _SnapLarge(pY[i2],y,lfSnap) && _SnapLarge(y,pY[i1],lfSnap) ) )
					{
						ret ++;
					}
					break;
				}
			}
		}
		else if( _SnapSame(x,xmin1,lfSnap) || _SnapSame(x,xmax1,lfSnap))
		{
			if(_SnapSame(x,xmin1,lfSnap) && _SnapSame(x,xmax1,lfSnap)&&
				(( _SnapLarge(y,pY[i0],lfSnap) && _SnapLarge(pY[i1],y,lfSnap) ) ||
				( _SnapLarge(pY[i0],y,lfSnap) && _SnapLarge(y,pY[i1],lfSnap) )))
			{
				return 0;
			}
			//			if( x==xmin1 )
			if( _SnapSame(x,xmin1,lfSnap) )
			{
				ret ++;
			}
		}
		//		if( x<xmin1 )ret++;
		else if( _SnapLarge(xmin1,x,lfSnap) )
			ret ++;
		else
		{
			xmax1 = (y-pY[i0])*(pX[i1]-pX[i0])/(pY[i1]-pY[i0])+pX[i0];
			//			if( x==xmax1 )return 0;//������???����|?
			if( _SnapSame(x,xmax1,lfSnap) )return 0;
			//			if( x<xmax1 )ret++;
			if( _SnapLarge(xmax1,x,lfSnap) )ret ++;
		}
	}
	return 1==ret%2? 1:-1;
}
