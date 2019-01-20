/*--------------------------------------------------------------------------------------------------------------------*/
//	PltReadWrite.cpp       -- PltReadWrite�ļ���д��ʵ���ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2017.7.21
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#include "stdafx.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <iosfwd>
#include "PltReadWrite.h"


// �ⲿDataManager.dll��ͷ�ļ�
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleton.h"
#include "../../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../../include/DataManager/Algorithm/Algorithm.h"



// AbnormalShapeManager
#include "../../../include/AbnormalShapeManager/DataManager/CommonData.h"
#include "../../../include/AbnormalShapeManager/DataManager/CSingleton.h"
#include "../../../include/AbnormalShapeManager/DataManager/Layer/Layer.h"
#include "../../../include/AbnormalShapeManager/DataManager/Figure/Complex.h"
#include "../../../include/AbnormalShapeManager/DataManager/Figure/Arc.h"
#include "../../../include/AbnormalShapeManager/DataManager/Point/ShapePoint.h"
#include "../../../include/AbnormalShapeManager/GeneralInterface/GeneralInterface.h"

bool Is2LineEqual(Line& l1, Line& l2)
{
	if (l1.s_x == l2.s_x &&
		l1.s_y == l2.s_y &&
		l1.e_x == l2.e_x &&
		l1.e_y == l2.e_y )
	{
		return true;
	}
	else if (l1.s_x == l2.e_x &&
		l1.s_y == l2.e_y &&
		l1.e_x == l2.s_x &&
		l1.e_y == l2.s_y )
	{
		return true;
	}


	return false;
}



bool IsLineExist(Line& ln, vector<Line>& LineList)
{
	for (int i = 0; i < LineList.size(); i++)
	{
		Line& exist_ln = LineList.at(i);

		if (Is2LineEqual(exist_ln, ln))
		{
			return true;
		}
	}

	return false;
}

void DrawACircle(float s_x, float s_y, float r, std::ofstream& plt_file)
{
	string tmp_s;
	stringstream ss;

	ss << s_x << "," << s_y << ";" ;

	tmp_s = ss.str();



	// ȥ�����
	plt_file << "PU"<<  tmp_s << endl;

	ss.clear();
	ss.str("");

	ss << r;
	tmp_s = ss.str();


	plt_file << "PD;"<< endl;
	plt_file << "CI"<<  tmp_s << "," << "1" << endl;

	plt_file << "PU;"<< endl;
}



void DrawALine(float s_x, float s_y, float e_x, float e_y, std::ofstream& plt_file)
{

	string tmp_s;
	stringstream ss;

	ss << s_x << "," << s_y << ";" ;

	tmp_s = ss.str();

	// ȥ�����,��ȥ�����ߣ�����
	plt_file << "PU"<< endl;
	plt_file << "PA"<<  tmp_s << endl;
	plt_file << "PD;"<< endl;



	ss.clear();
	ss.str("");

	ss <<  e_x << "," << e_y << ";" ;
	tmp_s = ss.str();

	plt_file << "PA"<<  tmp_s << endl;


	plt_file << "PU"<<   endl;



}








/*-------------------------------------------------------*/
//	����˵����
//		���������DXF�ļ�
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*-------------------------------------------------------*/
bool PltReadWrite::OutputPlt(Panel* pPanel, string strPltFilePath) 
{


	//2017-7-20 yuanzb  �½��ļ���

	try
	{
		std::ofstream plt_file(strPltFilePath, ios::out);
		
		CSingleton* pSingleton = CSingleton::GetSingleton();
		BaseInfo& info = pSingleton->m_BaseInfo;


		if (plt_file)
		{



			float dist = info.m_PositionSignDist;
			int hori_num = pPanel->m_OrgLen/dist;
			int vert_num = pPanel->m_OrgWidth/dist;
			float radius = info.m_PositionSignSize/2;
			float offset = radius + info.m_PositionSignOffset;
			float pos_x = - offset;
			float pos_y = pPanel->m_OrgWidth + offset;


#if 1
			// ��һ����
			

			switch(info.m_PositionSignType)
			{
			case 0:

				// Բ
				DrawACircle(pos_x, pos_y, radius, plt_file);

				break;

			case 1:
				// ʮ��
				DrawALine(pos_x - radius, pos_y, pos_x + radius, pos_y, plt_file);
				DrawALine(pos_x, pos_y + radius, pos_x, pos_y - radius, plt_file);

				break;
			default:

				// ʮ��
				DrawALine(pos_x - radius, pos_y, pos_x + radius, pos_y, plt_file);
				DrawALine(pos_x, pos_y + radius, pos_x, pos_y - radius, plt_file);


				break;
			}








			for (int i = 1; i < hori_num; i++)
			{
				pos_x = i*dist;
				pos_y = pPanel->m_OrgWidth + offset;



				switch(info.m_PositionSignType)
				{
				case 0:

					// Բ
					DrawACircle(pos_x, pos_y, radius, plt_file);

					break;

				case 1:
					// ʮ��
					DrawALine(pos_x - radius, pos_y, pos_x + radius, pos_y, plt_file);
					DrawALine(pos_x, pos_y + radius, pos_x, pos_y - radius, plt_file);


					break;
				default:

					// ʮ��
					DrawALine(pos_x - radius, pos_y, pos_x + radius, pos_y, plt_file);
					DrawALine(pos_x, pos_y + radius, pos_x, pos_y - radius, plt_file);

					break;
				}


			}


			for (int i = 1; i < vert_num; i++)
			{
				pos_x = -offset;
				pos_y = i*dist;



				switch(info.m_PositionSignType)
				{
				case 0:
					// Բ
					DrawACircle(pos_x, pos_y, radius, plt_file);


					break;

				case 1:

					// ʮ��
					DrawALine(pos_x - radius, pos_y, pos_x + radius, pos_y, plt_file);
					DrawALine(pos_x, pos_y + radius, pos_x, pos_y - radius, plt_file);



					break;
				default:

					// ʮ��
					DrawALine(pos_x - radius, pos_y, pos_x + radius, pos_y, plt_file);
					DrawALine(pos_x, pos_y + radius, pos_x, pos_y - radius, plt_file);

					break;
				}


			}





#endif

















			// ̧��
			plt_file << "PU;" << endl;


			// С������

			// ��������㴦��ģ��



			vector<Line> CutLineList;


			vector<Component*> CpnList;
			pPanel->GetAllNeededComponent(CpnList);

			int nCpnCount = CpnList.size();
			for (int i_cpn = 0; i_cpn < nCpnCount; i_cpn++)
			{
				Component* pCpn = CpnList.at(i_cpn);


				// �����εĵ�ֶ�
				vector<PointInfo> points = pCpn->m_vOutlinePoint;
				vector<vector<PointInfo>> _pNewpoint;
				vector<PointInfo> _tmppoint;
				for (int n_p = 0;n_p < points.size();n_p++)
				{
					int _cut = points.at(n_p).cut;
					if (_cut == 1)
					{
						_tmppoint.push_back(points.at(n_p));
						_pNewpoint.push_back(_tmppoint);
						_tmppoint.clear();
					}
					else if (_cut == 2)
					{
						_tmppoint.clear();
						_tmppoint.push_back(points.at(n_p));
					}
					else
					{
						_tmppoint.push_back(points.at(n_p));
					}
				}

				// ����ô��ĵ�
				if (_tmppoint.size() > 0)
					_pNewpoint.push_back(_tmppoint);

				for (int _pn = 0;_pn < _pNewpoint.size();_pn++)
				{
					int nCount;
					vector<PointInfo> newpoint;
					vector<PointInfo> tmpnewpoint;

					// ��һ�ε�������
					tmpnewpoint = _pNewpoint.at(_pn);

					// �����غϵ�
					tmpnewpoint = ALGORITHM_API::FiltrateCoincidentPoints2(tmpnewpoint);

					// ���ݵ���֯������Ҫ�����ݽṹ
					Cad_Complex* pComplex = new Cad_Complex;

					// ���»�ȡ��״�����
					nCount = tmpnewpoint.size();
					for (int index = 0; index < nCount; index++)
					{
						PointInfo cur_point = tmpnewpoint[index];

						if (index == 0)
						{
							if (cur_point.sign == 0)	// ֱ��ֱ�����
							{
								ShapePoint* pPnt = new ShapePoint(cur_point.x, cur_point.y);

								pPnt->m_FigureType = FigureType_PolyLine;
								pComplex->AddShapePoint(pPnt);
							}
							else if (cur_point.sign == 1 || cur_point.sign == 3)	// Բ������Բ�����ӵ�(�������ݴ�����ʱ�ȼ���) ��Բ���յ�һ���㣬���һ��Բ��
							{
								float center_x, center_y, r,sign ;
								float mid_x, mid_y, vec_x, vec_y;
								PointInfo end_pnt = tmpnewpoint[index+1];

								// ��Բ��
								r = end_pnt.r;
								sign = end_pnt.dir;
								ALGORITHM_API::GetCircleCenter(cur_point.x, end_pnt.x,cur_point.y, end_pnt.y, r, sign, center_x, center_y);

								// �����е��������
								mid_x = (end_pnt.x - cur_point.x)/2.0 + cur_point.x ;
								mid_y = (end_pnt.y - cur_point.y)/2.0 + cur_point.y ;

								vec_x = mid_x - center_x;
								vec_y = mid_y - center_y;

								ALGORITHM_API::NormalVector(vec_x, vec_y, vec_x, vec_y);
								vec_x *= r;
								vec_y *= r;

								// ��Բ���е��������
								mid_x = center_x + vec_x;
								mid_y = center_y + vec_y;

								// ���Բ��������
								ShapePoint* pStart = new ShapePoint(cur_point.x, cur_point.y);
								ShapePoint* pMid = new ShapePoint(mid_x, mid_y);
								ShapePoint* pEnd = new ShapePoint(end_pnt.x, end_pnt.y);

								pStart->m_FigureType = pMid->m_FigureType = pEnd->m_FigureType = FigureType_Arc;


								pComplex->AddShapePoint(pStart);
								pComplex->AddShapePoint(pMid);
								pComplex->AddShapePoint(pEnd);

								//index++;
							}
						}
						else
						{
							PointInfo prev_point = tmpnewpoint[index-1];

							if (cur_point.sign == 0)	// ֱ��ֱ�����
							{
								ShapePoint* pPnt = new ShapePoint(cur_point.x, cur_point.y);

								pPnt->m_FigureType = FigureType_PolyLine;
								pComplex->AddShapePoint(pPnt);
							}
							else if (cur_point.sign == 1 || cur_point.sign == 3)	// Բ����� ��Բ���յ�һ���㣬���һ��Բ��
							{
								float center_x, center_y, r,sign ;
								float mid_x, mid_y, vec_x, vec_y;
								PointInfo end_pnt = tmpnewpoint[index+1];

								// ��Բ��
								r = end_pnt.r;
								sign = end_pnt.dir;	// Բ������
								ALGORITHM_API::GetCircleCenter(cur_point.x, end_pnt.x, cur_point.y, end_pnt.y, r, sign, center_x, center_y);

								// �����е��������
								mid_x = (end_pnt.x - cur_point.x)/2.0 + cur_point.x ;
								mid_y = (end_pnt.y - cur_point.y)/2.0 + cur_point.y ;

								vec_x = mid_x - center_x;
								vec_y = mid_y - center_y;

								ALGORITHM_API::NormalVector(vec_x, vec_y, vec_x, vec_y);
								vec_x *= r;
								vec_y *= r;

								// ��Բ���е��������
								mid_x = center_x + vec_x;
								mid_y = center_y + vec_y;

								// ���Բ��������
								ShapePoint* pStart = new ShapePoint(cur_point.x, cur_point.y);
								ShapePoint* pMid = new ShapePoint(mid_x, mid_y);
								ShapePoint* pEnd = new ShapePoint(end_pnt.x, end_pnt.y);

								if (prev_point.sign == 0 || prev_point.sign == 2)	// ǰһ��Ϊֱ�߻���Բ���յ㣬˵��ǰһ����ֱ�ߣ���ǰ���ͼ��ͼ����Ϊֱ��
								{
									pStart->m_FigureType = FigureType_PolyLine;
									pMid->m_FigureType = pEnd->m_FigureType = FigureType_Arc;
								}
								else
								{
									pStart->m_FigureType = pMid->m_FigureType = pEnd->m_FigureType = FigureType_Arc;
								}

								if (pStart->IsCloseTo(pComplex->GetTailShapePoint()) == FALSE)	// �������һ���غϣ�����ӵ�һ��
								{
									pComplex->AddShapePoint(pStart);
								}

								pComplex->AddShapePoint(pMid);
								pComplex->AddShapePoint(pEnd);
							}
						}
					}

					// ����ͼ���ѹ�����ϣ�д��dxf��
					pComplex->CalComplexFigureList();


					// ��ʼ��ͼ��



					int nFigCount = pComplex->m_ComplexFigureList.size();
					for (int i_fig = 0; i_fig < nFigCount; i_fig++)
					{
						Figure* pFig = pComplex->m_ComplexFigureList.at(i_fig);

						if (pFig->m_FigureType == FigureType_PolyLine)
						{
							// ֱ��
							// ��ʱʹ�û��߶εķ�ʽ���Ժ���û����ߵķ�ʽ

							int nPntCount = pFig->GetShapePointNum();
							for (int i_pnt = 0; i_pnt < nPntCount-1; i_pnt++)
							{
								ShapePoint* pFirst = pFig->GetShapePoint(i_pnt);
								ShapePoint* pNext = pFig->GetShapePoint(i_pnt+1);

								float real_start_x	= pCpn->m_x + pFirst->m_x;
								float real_start_y	= pCpn->m_y + pFirst->m_y;
								float real_end_x	= pCpn->m_x + pNext->m_x;
								float real_end_y	= pCpn->m_y + pNext->m_y;


#if 0

								string tmp_s;
								stringstream ss;

								ss << real_start_x << "," << real_start_y << ";" ;

								tmp_s = ss.str();


								// ��һ����Ҫ����
								if (i_fig == 0 && i_pnt == 0)
								{
									// ȥ�����
									plt_file << "PA" << tmp_s << endl;

									plt_file << "PD;"<< endl;
								}


								ss.clear();
								ss.str("");

								// ȥ���յ�
								ss << real_end_x << "," << real_end_y << ";" ;

								tmp_s = ss.str();

								plt_file << "PA" << tmp_s << endl;

								// ���һ����Ҫ̧��
								if (i_fig == nFigCount-1 && i_pnt == nPntCount-2)
								{

									plt_file << "PU;"<< endl;
								}



#else

								Line ln;

								ln.s_x = real_start_x;	
								ln.s_y = real_start_y;	
								ln.e_x = real_end_x;
								ln.e_y = real_end_y;

								if (IsLineExist(ln, CutLineList))
								{
									continue;
								}
								else
								{
									CutLineList.push_back(ln);

									DrawALine(ln.s_x,  ln.s_y,  ln.e_x, ln.e_y, plt_file);
								}			 
											 
											  





#endif

								

							}
						}
					}
				}
			}

			//  �ر��ļ�
			plt_file.close();

		}
	}
	catch(CException* e)
	{
		TCHAR   szError[1024];   
		e->GetErrorMessage(szError,1024);   //  e.GetErrorMessage(szError,1024); 
		::AfxMessageBox(szError); 
	}

	

	return true;
}

