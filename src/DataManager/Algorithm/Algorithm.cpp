#include "stdafx.h"
#include "Algorithm.h"
#include "../BaseDataType/CommonData/CommonData.h"
#include "../BaseDataType/Outline/Outline.h"
#include <algorithm>



// ������򣬳������ȣ������֮
int ALGORITHM_API::SortComponentList_Random(vector<Component*>& ComponentList)
{
	// �������;
	vector<Component*> new_list;

	srand((unsigned)time(0)); 

	// ��ԭʼ��������ѡȡС�壬���뵽�������У�
	while(ComponentList.size() > 0)
	{
		int nCount = ComponentList.size();
		int sel_id = rand() % nCount;

		Component* pCpn = ComponentList.at(sel_id);
		new_list.push_back(pCpn);
		ComponentList.erase(ComponentList.begin() + sel_id);
	}

	// ���¸�ֵ
	ComponentList = new_list;
	return 0;
}



bool ALGORITHM_API::New_MatchSuitableComponentNOutline(vector<Component*>& ComponentList,vector<Outline>& outline_list,Component*& pPerfectMatchComponent, bool &rotate_flag,int  nCpnID, int &nOutlineID, BaseInfo& base_info)
{

	bool bMatchFlag = false;
	Outline best_line;
	float prev_height = 0;
	bool bRotateFlag = false;

match_loop:


	// ѡ������������
	if (New_FindBestOutLine(outline_list, best_line, base_info.m_LayoutOrg) == true)
	{
		nOutlineID = best_line.m_index;
		int i_cpn , nLeftCpnSize = ComponentList.size();
		Component* pSuitableCpn = NULL;

		for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
		{
			Component* pCpn = ComponentList.at(i_cpn);

			
			// ����ת�Ƚ����Σ�ȡ�߶Ƚϵ͵�һ��

			if (best_line.Containable(pCpn) == true)
			{
				if(bMatchFlag == false)
				{
					//��һ��ѡ��
					prev_height = pCpn->m_RealWidth;

					pPerfectMatchComponent = pCpn;
					bRotateFlag = false;
					rotate_flag = bRotateFlag;
					bMatchFlag = true;
				}
				else
				{
					float new_height = pCpn->m_RealWidth;

					// ������ƥ���
					if (new_height < prev_height )
					{
						prev_height = pCpn->m_RealWidth;

						pPerfectMatchComponent = pCpn;
						bRotateFlag = false;
						rotate_flag = bRotateFlag;
						bMatchFlag = true;

					}
				}
			}

			if (pCpn->IsRotatable() == true)	// ƥ��ʱ��Ϊ���ټ�����������ʧ�棬��������ת��ֻ�Ƚ���ֵ������ʱ��������ת
			{
				Component tmp_cpn;
				tmp_cpn.m_RealLength = pCpn->m_RealWidth;
				tmp_cpn.m_RealWidth =  pCpn->m_RealLength;

				if (best_line.Containable(&tmp_cpn) == true)
				{
					if(bMatchFlag == false)
					{
						//��һ��ѡ��
						prev_height = pCpn->m_RealWidth;

						pPerfectMatchComponent = pCpn;
						bRotateFlag = true;
						rotate_flag = bRotateFlag;
						bMatchFlag = true;


					}
					else
					{
						
						float new_height = tmp_cpn.m_RealWidth;

						// ������ƥ���
 						if (new_height < prev_height )
 						{
							int r = 1/*rand() % 2*/;

							if (r)
							{
								prev_height = tmp_cpn.m_RealWidth;

								pPerfectMatchComponent = pCpn;
								bRotateFlag = true;
								rotate_flag = bRotateFlag;
								bMatchFlag = true;
							}


						}
					}
				}
			}


			// ���ҵ�
			if (bMatchFlag == true)
			{
// 				int r = rand() % 2;
// 
// 				if (r)
// 				{
					break;

//				}
			}

		}


		// ��ʱ��δ�ҵ�ƥ�����������ߣ�˵������������Ѿ��޷�ʹ�ã�����������������������һ�������ߺϲ�
		if (bMatchFlag == false)
		{
			vector<Outline> RealOutlineList;

			New_GetRealOutlineList(outline_list, RealOutlineList);

			vector<Outline>::iterator it =  std::find(RealOutlineList.begin(), RealOutlineList.end(), best_line) ;

			if (it == RealOutlineList.begin())
			{
				// ͷ ����һ���Ƚ�
				int cur_id = best_line.m_index;
				int next_id = best_line.m_index + 2;
				

				Outline& cur_real_line = outline_list.at(cur_id);
				Outline& next_real_line = outline_list.at(next_id);

				// �ϲ�
				cur_real_line.m_start_y = next_real_line.m_start_y;
				cur_real_line.m_end_x = next_real_line.m_end_x;
				cur_real_line.m_end_y = next_real_line.m_end_y;

				// ɾ��
				vector<Outline>::iterator start_it	= outline_list.begin() + cur_real_line.m_index;
				vector<Outline>::iterator end_it	= outline_list.begin() + cur_real_line.m_index + 2;
				outline_list.erase(start_it+1, end_it+1);

				ALGORITHM_API::New_UpdateOutlineListIndex(outline_list);



			}
			else if (it == RealOutlineList.begin() + RealOutlineList.size()-1)
			{
				// β ��ǰһ���Ƚ�
				int cur_id = best_line.m_index;
				int prev_id = best_line.m_index - 2;


				Outline& cur_real_line = outline_list.at(cur_id);
				Outline& prev_real_line = outline_list.at(prev_id);

				// �ϲ�
				cur_real_line.m_end_y = prev_real_line.m_start_y;
				cur_real_line.m_start_x = prev_real_line.m_start_x;
				cur_real_line.m_start_y = prev_real_line.m_start_y;

				// ɾ��
				vector<Outline>::iterator start_it	= outline_list.begin() + cur_real_line.m_index - 2;
				vector<Outline>::iterator end_it	= outline_list.begin() + cur_real_line.m_index ;
				outline_list.erase(start_it, end_it);
				ALGORITHM_API::New_UpdateOutlineListIndex(outline_list);



			}
			else if (it == RealOutlineList.end())
			{
				// ��������
				return false;
			}
			else
			{
				// �м䣬ǰ��Ƚ�

				int cur_id = best_line.m_index;
				int prev_id = best_line.m_index - 2;
				int next_id = best_line.m_index + 2;

				Outline& cur_real_line = outline_list.at(cur_id);
				Outline& prev_real_line = outline_list.at(prev_id);
				Outline& next_real_line = outline_list.at(next_id);

				if (prev_real_line.m_start_y < next_real_line.m_start_y)
				{
					// �ϲ�
					cur_real_line.m_end_y = prev_real_line.m_start_y;
					cur_real_line.m_start_x = prev_real_line.m_start_x;
					cur_real_line.m_start_y = prev_real_line.m_start_y;

					// ɾ��
					vector<Outline>::iterator start_it	= outline_list.begin() + cur_real_line.m_index - 2;
					vector<Outline>::iterator end_it	= outline_list.begin() + cur_real_line.m_index ;
					outline_list.erase(start_it, end_it);

					ALGORITHM_API::New_UpdateOutlineListIndex(outline_list);

				}
				else
				{

					// �ϲ�
					cur_real_line.m_start_y = next_real_line.m_start_y;
					cur_real_line.m_end_x = next_real_line.m_end_x;
					cur_real_line.m_end_y = next_real_line.m_end_y;

					// ɾ��
					vector<Outline>::iterator start_it	= outline_list.begin() + cur_real_line.m_index;
					vector<Outline>::iterator end_it	= outline_list.begin() + cur_real_line.m_index + 2;
					outline_list.erase(start_it+1, end_it+1);

					ALGORITHM_API::New_UpdateOutlineListIndex(outline_list);

				}


			}
		

			// ���²���
			goto match_loop;
		}

	}
	else
	{
		return false;
	}

	return bMatchFlag;
}



/*--------------------------------------------------------------------------------------*/
//	purpose:
//		ƥ����ʵ�С���������2����1����һ��
//	
//		��һ��ƥ�䣬�鿴�ܷ���������ܰ�������ת�ٿ��Ƿ��ܰ���
//		�ڶ���ƥ�䣺������������Ѿ���Ч����պ�����������������
//		
//	
//	param:
//		int &nCpnID			--	С��ID
//		int &nOutlineID		--	������ID
//
//	return:
//		true -- ��ƥ�䵽
//		false-- �޷�ƥ��
/*--------------------------------------------------------------------------------------*/
bool ALGORITHM_API::MatchSuitableComponentNOutline2(vector<Component*>& ComponentList,vector<Outline>& OutlineList, vector<Outline>& LowestOutlineList, int &nCpnID, int &nOutlineID)
{
match_loop:

	bool bMatchFlag = false;
	int i_cpn , nLeftCpnSize = ComponentList.size();
	int i_outline, nOutlineSize = LowestOutlineList.size();
	Component* pSuitableCpn = NULL;

	for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
	{
		Component* pCpn = ComponentList.at(i_cpn);

		for (i_outline = 0; i_outline < nOutlineSize; i_outline++)
		{
			Outline& line = LowestOutlineList.at(i_outline);

			if (line.Containable(pCpn) == true)
			{
				bMatchFlag = true;
				nCpnID = i_cpn;
				nOutlineID = i_outline;
				break;
			}
			else
			{
				if (pCpn->IsRotatable() == true)
				{
					pCpn->ClockwiseRotate90();
					if (line.Containable(pCpn) == true)
					{
						bMatchFlag = true;
						nCpnID = i_cpn;
						nOutlineID = i_outline;
						break;
					}
					else
					{
						pCpn->CouterClockwiseRotate90();
					}
				}
			}
		}

		// ���ҵ�
		if (bMatchFlag == true)
		{
			break;
		}

	}


	// ��ʱ��δ�ҵ�ƥ�����������ߣ�˵������������Ѿ��޷�ʹ�ã������������������գ�����һ��������
	if (bMatchFlag == false)
	{
		// ����������������
		LowestOutlineList.clear();

		// ���滹�������ߣ��������ң�û����ֱ�ӷ���false
		if (OutlineList.size() > 0)
		{
			// ѡ�����������
			int i, erase_id = 0;
			Outline first_line = OutlineList.at(0);
			int nCount = OutlineList.size();

			for (i = 1; i < nCount; i++)
			{
				Outline ln = OutlineList.at(i);

				if (ln.m_start_y == first_line.m_start_y)
				{
					erase_id = i;
				}
				else
				{
					break;
				}
			}

			// �����������Ԫ�ز���������������������������������Ƴ�
			for (i = 0; i <= erase_id; i++)
			{
				Outline ln = OutlineList.at(0);
				LowestOutlineList.push_back(ln);
				OutlineList.erase(OutlineList.begin());
			}

			// ���²���
			goto match_loop;
		}
	}



	return bMatchFlag;
}

// ��С��������
bool ALGORITHM_API::CompareOutlineGreater(const Outline& first, const Outline& second) 
{              
	return first < second;	
}


void ALGORITHM_API::New_UpdateOutlineListIndex(vector<Outline>& OutlineList)
{
	for (int i = 0 ; i < OutlineList.size(); i++)
	{
		Outline& ln = OutlineList.at(i);
		ln.m_index = i;
	}
}

/*--------------------------------------------------------------------------------------*/
//	purpose:
//		��ʼ������������
//	
//	param:
//		
//
//	return:
//		0 -- �ɹ�
/*--------------------------------------------------------------------------------------*/
int ALGORITHM_API::New_BuildOutlineList(Panel* pParent, vector<Outline>& OutlineList, int Org)
{

	// �������������
	OutlineList.clear();

	float start_x, start_y, end_x, end_y;
	Outline l1, l2, l3;

	switch(Org)
	{
	case LayoutOrg_LeftBottom:
	case LayoutOrg_RightBottom:

		// ��ߴ�ֱ��

		start_x = pParent->m_x;
		start_y = pParent->m_y + pParent->m_RealWidth;
		end_x	= pParent->m_x;
		end_y	= pParent->m_y;


		l1.SetOutline(start_x, end_x , start_y, end_y, pParent);
		l1.m_type = Outline::OutlineType_Vertical;

		// �м�ˮƽ��

		start_x = pParent->m_x;
		start_y = pParent->m_y ;
		end_x	= pParent->m_x + pParent->m_RealLength;
		end_y	= pParent->m_y;


		l2.SetOutline(start_x, end_x , start_y, end_y, pParent);
		l2.m_type = Outline::OutlineType_Horizon;

		// �ұߴ�ֱ��


		start_x = pParent->m_x + pParent->m_RealLength;
		start_y = pParent->m_y ;
		end_x	= pParent->m_x + pParent->m_RealLength;
		end_y	= pParent->m_y + pParent->m_RealWidth;


		l3.SetOutline(start_x, end_x , start_y, end_y, pParent);
		l3.m_type = Outline::OutlineType_Vertical;

		break;
	case LayoutOrg_LeftTop:
	case LayoutOrg_RightTop:

		start_x = pParent->m_x;
		start_y = pParent->m_y ;
		end_x	= pParent->m_x;
		end_y	= pParent->m_y + pParent->m_RealWidth;


		l1.SetOutline(start_x, end_x , start_y, end_y, pParent);
		l1.m_type = Outline::OutlineType_Vertical;

		// �м�ˮƽ��

		start_x = pParent->m_x;
		start_y = pParent->m_y + pParent->m_RealWidth;
		end_x	= pParent->m_x + pParent->m_RealLength;
		end_y	= pParent->m_y + pParent->m_RealWidth;


		l2.SetOutline(start_x, end_x , start_y, end_y, pParent);
		l2.m_type = Outline::OutlineType_Horizon;

		// �ұߴ�ֱ��


		start_x = pParent->m_x + pParent->m_RealLength;
		start_y = pParent->m_y + pParent->m_RealWidth;
		end_x	= pParent->m_x + pParent->m_RealLength;
		end_y	= pParent->m_y ;


		l3.SetOutline(start_x, end_x , start_y, end_y, pParent);
		l3.m_type = Outline::OutlineType_Vertical;


		break;


	}

	



	OutlineList.push_back(l1);
	OutlineList.push_back(l2);
	OutlineList.push_back(l3);
	New_UpdateOutlineListIndex(OutlineList);

	return true;
}


/*--------------------------------------------------------------------------------------*/
//	purpose:
//		��ʼ������������
//	
//	param:
//		
//
//	return:
//		0 -- �ɹ�
/*--------------------------------------------------------------------------------------*/
int ALGORITHM_API::BuildOutlineList(Panel* pParent, vector<Outline>& OutlineList, vector<Outline>& LowestOutlineList)
{
	// �������������
	OutlineList.clear();
	LowestOutlineList.clear();

	// ��ȡ���ϣ�����������
	vector<Node*> list;
	pParent->GetAllLeafNodes(list, NodeType_Remainder);	

	// ����������
	int i, nCount;
	nCount = list.size();

	// ������
	if(nCount == 0)
		return 0;

	for(int i = 0; i < nCount; i++)
	{
		Node* pNode = list.at(i);
		Component* pCpn = dynamic_cast<Component*>(pNode);

		Outline line;
		float start_x = pCpn->m_x;
		float end_x = pCpn->m_x + pCpn->m_RealLength;
		float start_y = pCpn->m_y;
		float end_y = pCpn->m_y + pCpn->m_RealWidth;

		// ����������
		line.SetOutline(start_x, end_x, start_y, end_y, pCpn);
		OutlineList.push_back(line);
	}

	// ��С��������
	sort(OutlineList.begin(), OutlineList.end(), CompareOutlineGreater); 

	// ѡ�����������
	int erase_id = 0;
	Outline first_line = OutlineList.at(0);
	nCount = OutlineList.size();

	for (i = 1; i < nCount; i++)
	{
		Outline ln = OutlineList.at(i);

		if (ln.m_start_y == first_line.m_start_y)
		{
			erase_id = i;
		}
		else
		{
			break;
		}
	}

	// �����������Ԫ�ز���������������������������������Ƴ�
	for (i = 0; i <= erase_id; i++)
	{
		Outline ln = OutlineList.at(0);
		LowestOutlineList.push_back(ln);
		OutlineList.erase(OutlineList.begin());
	}


	return 0;
}


void ALGORITHM_API::New_GetRealOutlineList(vector<Outline>& OutlineList, vector<Outline>& RealOutlineList)
{
	RealOutlineList.clear();

	for	(int i = 0; i < OutlineList.size(); i++)
	{
		Outline& tmp_ln = OutlineList.at(i);

		if (tmp_ln.m_type == Outline::OutlineType_Horizon)
		{
			RealOutlineList.push_back(tmp_ln);
		}

	}
}

// �ҵ����ʺϵ�������
bool ALGORITHM_API::New_FindBestOutLine(vector<Outline>& outline_list, Outline& ln, int Org)
{
	vector<Outline> RealOutlineList;

	New_GetRealOutlineList(outline_list, RealOutlineList);


	// ��С��������
	sort(RealOutlineList.begin(), RealOutlineList.end(), CompareOutlineGreater);

	if (RealOutlineList.size() > 0)
	{

		switch(Org)
		{
		case LayoutOrg_LeftBottom:		// ���½�

			// ����С��

			ln = RealOutlineList.at(0);

			break;
		case LayoutOrg_LeftTop:			// ���Ͻ�
			// ����С��

			ln = RealOutlineList.at(RealOutlineList.size()-1);

			break;						
		case LayoutOrg_RightBottom:		// ���½�


			ln = RealOutlineList.at(0);

			break;
		case LayoutOrg_RightTop:		// ���Ͻ�
			ln = RealOutlineList.at(RealOutlineList.size()-1);

			break;
		default:						// Ĭ�����½�


			ln = RealOutlineList.at(0);

			break;

		}

		return true;
	}


	return false;
}

/*---------------------------------------*/
//	����˵����
//
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
/*---------------------------------------*/
int ALGORITHM_API::New_LayoutOnePanel_LowestOutline(Panel* pSrcPanel, BaseInfo& Info, vector<Component*>& SrcComponentList, int CutStyle, int Org)
{
	int nCpnID = 0;
	int nOutlineID = 0;
	vector<Outline> outline_list;

	// ָ���ж�
	if (pSrcPanel == NULL)
	{
		return -1;
	}


	// ��������������
	New_BuildOutlineList(pSrcPanel, outline_list, Org);

	// ��������С��


	// ����С����Ҫ����
	while (SrcComponentList.size() > 0)
	{
		bool bMatchable = false;					// ƥ��ɹ���־
		bool bRotateFlag = false;					// ��ת��־
		int nRecommendCutDir = CutDir_Horizon;		// �����иʽ
		Component* pPerfectMatchComponent = NULL;	// ���ƥ���С��

		


		// ����С��

		// ��������������


		bMatchable = New_MatchSuitableComponentNOutline( SrcComponentList,  outline_list,  pPerfectMatchComponent,  bRotateFlag,  nCpnID,  nOutlineID,  Info);

		if (bMatchable == true)
		{
			Component* pPlaceCpn = pPerfectMatchComponent;		
			Outline& SelectLine = outline_list.at(nOutlineID);
			
			
			// �Ƿ���ת
			if (bRotateFlag == true)
			{
				pPlaceCpn->ClockwiseRotate90();
			}


			// ����
			ALGORITHM_API::New_KnifeOneOuline(outline_list, nOutlineID ,pSrcPanel,  pPerfectMatchComponent, Org, Info);

			// ɾ���������İ��������
			vector<Component*>::iterator it_cpn = find(SrcComponentList.begin(), SrcComponentList.end(), pPerfectMatchComponent);
			SrcComponentList.erase(it_cpn);

		}
		else
		{
			if (SrcComponentList.size() > 0)
				return 1;
			else
				return 0;	// ������
		}
	}

	if (SrcComponentList.size() > 0)
		return 1;
	else
		return 0;	// ������





}



/*---------------------------------------*/
//	����˵����
//
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
/*---------------------------------------*/
int ALGORITHM_API::LayoutOnePanel_LowestOutline(Panel* pSrcPanel, BaseInfo& Info, vector<Component*>& SrcComponentList, int CutStyle, int Org)
{
	int nCpnID = 0;
	vector<Outline> OutlineList, LowestOutlineList;
	
	// ָ���ж�
	if (pSrcPanel == NULL)
	{
		return -1;
	}

	// ���� �������� �����֮
	SortComponentList_Random(SrcComponentList);

	BuildOutlineList(pSrcPanel, OutlineList, LowestOutlineList);



	while (SrcComponentList.size() > 0)
	{
		bool bMatchable = false;
		int nCpnID, nOutlineID;

		bMatchable = MatchSuitableComponentNOutline2(SrcComponentList, OutlineList, LowestOutlineList, nCpnID, nOutlineID);
		if (bMatchable == true)
		{
			Component* pPlaceCpn = SrcComponentList.at(nCpnID);				
			Outline lowest_line = LowestOutlineList.at(nOutlineID);
			Component* pParentNode = lowest_line.m_pParent;

			// ���ID
// 			pPlaceCpn->m_CpnID = nCpnID;
// 			nCpnID++;

			int cut_dir;

			// ѡ�������иʽ
			switch(CutStyle)
			{
			case CutDir_Horizon:
				cut_dir = CutDir_Horizon;
				break;
			case CutDir_Vertical:
				cut_dir = CutDir_Vertical;
				break;
			case CutDir_Random:
				cut_dir = rand()%2;
				break;
			default:
				break;
			}

			// ����
			ALGORITHM_API::KnifeOneRemainder(pParentNode, pPlaceCpn, cut_dir, Info.m_SawKerfWidth, Org);

			// �ؽ����������
			BuildOutlineList(pSrcPanel, OutlineList, LowestOutlineList);

			// ɾ���������İ�������������
			vector<Component*>::iterator it_cpn;
			it_cpn = find(SrcComponentList.begin(), SrcComponentList.end(), pPlaceCpn);
			SrcComponentList.erase(it_cpn);
		}
		else
		{
			if (SrcComponentList.size() > 0)
				return 1;
			else
				return 0;	// ������
		}
	}

	if (SrcComponentList.size() > 0)
		return 1;
	else
		return 0;	// ������

	return 0;
}



/**  С�尴�տͻ�������
	@param[in]		
	@param[out]		
	@return			void
	@warning		
	@note			
	@see            
	*/
void ALGORITHM_API::SplitComponentListByCustomer(vector<Component*>& SrcComponentList, vector<vector<Component*>>& SplitComponentGroup)
{
	int nSrcCompSize = SrcComponentList.size();

	// ����
	for(int i = 0; i < nSrcCompSize; i++)
	{
		int nListToInsertItemID = -1;
		int nSplitCompGroupSize = SplitComponentGroup.size();
		Component* pSrcComponent = SrcComponentList[i];

		// �鿴����������Ƿ�����ͬ���ʡ���ȵķ���
		for(int j = 0; j < nSplitCompGroupSize; j++)
		{
			if(SplitComponentGroup[j].size() > 0 )		// ���ڷ���
			{
				Component* pSplitComponent = SplitComponentGroup[j][0];

				// �жϲ��ʡ�����Ƿ���ͬ
				if (pSplitComponent->m_strCustomerInfo.Compare(pSrcComponent->m_strCustomerInfo) == 0)
				{
					nListToInsertItemID = j;
					break;
				}
			}
		}

		if(nListToInsertItemID != -1)	// �ҵ����з���
		{
			SplitComponentGroup[nListToInsertItemID].push_back(pSrcComponent);
		}
		else							// δ�ҵ����飬�½����鲢����
		{
			vector<Component*> componentList;
			componentList.push_back(pSrcComponent);

			SplitComponentGroup.push_back(componentList);
		}
	}


}


/**  �����Ż����
	@param[in]		
	@param[out]		
	@return			void
	@warning		
	@note			
	@see            
	*/
int ALGORITHM_API::New_LayoutOnePanel_Greedy(Panel* pSrcPanel, BaseInfo& Info, vector<Component*>& SrcComponentList, int CutStyle, int Org)
{
	int nCpnID = 0;
	vector<Component*> RemainderList;

	// ָ���ж�
	if (pSrcPanel == NULL)
	{
		return -1;
	}

	// ������������
	BuildRemainderList(pSrcPanel, RemainderList);



	// ������÷�����Ϣ��������
// 	if (Info.m_bDownerFaceFirst == true)
// 	{
// 		vector<Component*> DownerInfoCpnList;
// 		vector<Component*>::iterator it;
// 		for (it = SrcComponentList.begin(); it != SrcComponentList.end();)
// 		{
// 			Component* pCpn = *it;
// 
// 			if (pCpn->HaveDownerFaceHole() == true || pCpn->HaveDownerFaceSlot() == true)
// 			{
// 				// ��ԭ����ɾ�������뷴����Ϣ����
// 				it = SrcComponentList.erase(it);
// 				DownerInfoCpnList.push_back(pCpn);
// 			}
// 			else
// 			{
// 				it++;
// 			}
// 		}
// 
// 		// ����ͷ��
// 		SrcComponentList.insert(SrcComponentList.begin(), DownerInfoCpnList.begin(), DownerInfoCpnList.end());
// 	}


	// �ͻ���Ϣ����
	if (Info.m_bCustomerFirst == true)
	{
		vector<vector<Component*>> SplitComponentGroup;

		SplitComponentListByCustomer(SrcComponentList, SplitComponentGroup);


		// ���������������
		SrcComponentList.clear();
		for(unsigned int i_group = 0; i_group < SplitComponentGroup.size(); i_group++)
		{
			vector<Component*>& cur_group = SplitComponentGroup.at(i_group);

			SrcComponentList.insert(SrcComponentList.begin(), cur_group.begin(), cur_group.end());
		}
	}




	// ����С����Ҫ����
	while (SrcComponentList.size() > 0)
	{
		bool bMatchable = false;					// ƥ��ɹ���־
		bool bRotateFlag = false;					// ��ת��־
		int nRecommendCutDir = CutDir_Horizon;		// �����иʽ
		Component* pPerfectMatchComponent = NULL, *pPerfectMatchRemainder = NULL;	// ���ƥ������Ϻ�С��

		bMatchable = MatchSuitableComponentNRemaider(RemainderList, SrcComponentList, pPerfectMatchComponent, bRotateFlag, nRecommendCutDir, pPerfectMatchRemainder, Info);
		if (bMatchable == true)
		{
			Component* pPlaceCpn = pPerfectMatchComponent;		
			Component* pParentNode = pPerfectMatchRemainder;

			// �Ƿ���ת
			if (bRotateFlag == true)
			{
				pPlaceCpn->ClockwiseRotate90();
			}

			// ѡ�������иʽ
			switch(CutStyle)
			{
			case CutDir_Horizon:
				nRecommendCutDir = CutDir_Horizon;
				break;
			case CutDir_Vertical:
				nRecommendCutDir = CutDir_Vertical;
				break;
			case CutDir_Random:
				nRecommendCutDir = rand()%2;
				break;
			default:
				break;
			}

			// ����
			ALGORITHM_API::New_KnifeOneRemainder(pPerfectMatchRemainder, pPerfectMatchComponent, nRecommendCutDir, Org, Info);

			// ɾ���������İ��������
			vector<Component*>::iterator it_cpn = find(SrcComponentList.begin(), SrcComponentList.end(), pPerfectMatchComponent);
			SrcComponentList.erase(it_cpn);

			vector<Component*>::iterator it_rmd = find(RemainderList.begin(), RemainderList.end(), pPerfectMatchRemainder);
			RemainderList.erase(it_rmd);

			// ���½�����������
			BuildRemainderList(pSrcPanel, RemainderList);

			// ��������
			//SortRemainderList_LengthFirst(RemainderList);

			switch(Info.m_LayoutOrg)
			{
			case LayoutOrg_LeftBottom:
			case LayoutOrg_RightBottom:
				SortRemainderList_Bottom2Top(RemainderList);
				break;
			case LayoutOrg_LeftTop:
			case LayoutOrg_RightTop:

				SortRemainderList_Top2Bottom(RemainderList);

				break;
			default:

				SortRemainderList_Bottom2Top(RemainderList);
				break;

			}


		}
		else
		{
			if (SrcComponentList.size() > 0)
				return 1;
			else
				return 0;	// ������
		}
	}

	if (SrcComponentList.size() > 0)
		return 1;
	else
		return 0;	// ������
}

/*---------------------------------------------------------*/
//	����˵����
//		�ڴ�����Ż�����
//
//	������
//		Panel * pSrcPanel					--	Ҫ�����Ĵ��
//		BaseInfo & base_info				--	��������Ļ�����Ϣ
//		vector<Component*> & SrcCpnList		--	С���б�
//		int cut_style						--	�иʽ
//
//	����ֵ:
//		0		--	���������,û��С��
//		1		--	���������,����С��
//		����	--	����
/*---------------------------------------------------------*/
int ALGORITHM_API::LayoutOnePanel_Greedy(Panel* pSrcPanel, BaseInfo& Info, vector<Component*>& SrcComponentList, int CutStyle, int Org)
{
	int nCpnID = 0;
	vector<Component*> RemainderList;
	
	// ָ���ж�
	if (pSrcPanel == NULL)
	{
		return -1;
	}

	// ������������
	BuildRemainderList(pSrcPanel, RemainderList);

	// ������÷�����Ϣ��������
	if (Info.m_bDownerFaceFirst == true)
	{
		vector<Component*> DownerInfoCpnList;
		vector<Component*>::iterator it;
		for (it = SrcComponentList.begin(); it != SrcComponentList.end();)
		{
			Component* pCpn = *it;

			if (pCpn->HaveDownerFaceHole() == true || pCpn->HaveDownerFaceSlot() == true)
			{
				// ��ԭ����ɾ�������뷴����Ϣ����
				it = SrcComponentList.erase(it);
				DownerInfoCpnList.push_back(pCpn);
			}
			else
			{
				it++;
			}
		}

		// ����ͷ��
		SrcComponentList.insert(SrcComponentList.begin(), DownerInfoCpnList.begin(), DownerInfoCpnList.end());
	}


	// ����С����Ҫ����
	while (SrcComponentList.size() > 0)
	{
		bool bMatchable = false;					// ƥ��ɹ���־
		bool bRotateFlag = false;					// ��ת��־
		int nRecommendCutDir = CutDir_Horizon;		// �����иʽ
		Component* pPerfectMatchComponent = NULL, *pPerfectMatchRemainder = NULL;	// ���ƥ������Ϻ�С��

		bMatchable = MatchSuitableComponentNRemaider(RemainderList, SrcComponentList, pPerfectMatchComponent, bRotateFlag, nRecommendCutDir, pPerfectMatchRemainder, Info);
		if (bMatchable == true)
		{
			Component* pPlaceCpn = pPerfectMatchComponent;		
			Component* pParentNode = pPerfectMatchRemainder;

			// �Ƿ���ת
			if (bRotateFlag == true)
			{
				pPlaceCpn->ClockwiseRotate90();
			}

			// ѡ�������иʽ
			switch(CutStyle)
			{
			case CutDir_Horizon:
				nRecommendCutDir = CutDir_Horizon;
				break;
			case CutDir_Vertical:
				nRecommendCutDir = CutDir_Vertical;
				break;
			case CutDir_Random:
				nRecommendCutDir = rand()%2;
				break;
			default:
				break;
			}

			// ����
			ALGORITHM_API::KnifeOneRemainder(pPerfectMatchRemainder, pPerfectMatchComponent, nRecommendCutDir, Info.m_SawKerfWidth, Org);

			// ɾ���������İ��������
			vector<Component*>::iterator it_cpn = find(SrcComponentList.begin(), SrcComponentList.end(), pPerfectMatchComponent);
			SrcComponentList.erase(it_cpn);

			vector<Component*>::iterator it_rmd = find(RemainderList.begin(), RemainderList.end(), pPerfectMatchRemainder);
			RemainderList.erase(it_rmd);

			// ���½�����������
			BuildRemainderList(pSrcPanel, RemainderList);

			// ��������
			SortRemainderList_LengthFirst(RemainderList);

		}
		else
		{
			if (SrcComponentList.size() > 0)
				return 1;
			else
				return 0;	// ������
		}
	}

	if (SrcComponentList.size() > 0)
		return 1;
	else
		return 0;	// ������
}



bool  ALGORITHM_API::New_KnifeOneOuline(vector<Outline>& outline_list, int nOutlineID, Component* pParentNode, Component* pPlaceCpn,  int Org, BaseInfo& b_info)
{
	// �ж�Ҫ�и��С���Ƿ�Ϸ�
	if (pPlaceCpn->IsLegal() == false)
	{
		return false;
	}


	float start_x, start_y, end_x, end_y;
	Outline& cur_line = outline_list.at(nOutlineID);
	Outline new_ver_line, new_hor_line;
	OutlineList::iterator insert_it;
	OutlineList tmp_list;


	switch(Org)
	{
	case LayoutOrg_LeftBottom:		// ���½�

		// ����С��
		pPlaceCpn->m_x = cur_line.m_start_x;
		pPlaceCpn->m_y = cur_line.m_start_y;

		// ����һ����ֱ�ߺ�һ���µ�

		start_x = cur_line.m_start_x + pPlaceCpn->m_RealLength + b_info.m_x_space;
		start_y = cur_line.m_start_y + pPlaceCpn->m_RealWidth + b_info.m_y_space;
		end_x	= start_x;
		end_y	= cur_line.m_start_y;

		new_ver_line.SetOutline(start_x, end_x, start_y, end_y, NULL);
		new_ver_line.m_type = Outline::OutlineType_Vertical;

		start_x = cur_line.m_start_x + pPlaceCpn->m_RealLength + b_info.m_x_space;
		start_y = cur_line.m_start_y;
		end_x	= cur_line.m_end_x;
		end_y	= cur_line.m_end_y;

		new_hor_line.SetOutline(start_x, end_x, start_y, end_y, NULL);
		new_hor_line.m_type = Outline::OutlineType_Horizon;
		
		tmp_list.push_back(new_ver_line);
		tmp_list.push_back(new_hor_line);

		// �޸ĵ�ǰ����������

		start_x = cur_line.m_start_x ;
		start_y = cur_line.m_start_y + pPlaceCpn->m_RealWidth + b_info.m_y_space;
		end_x	= new_ver_line.m_start_x;
		end_y	= new_ver_line.m_start_y;
		cur_line.SetOutline(start_x, end_x, start_y, end_y, NULL);

		insert_it = outline_list.begin()+nOutlineID+1;

		outline_list.insert(insert_it, tmp_list.begin(), tmp_list.end());
		ALGORITHM_API::New_UpdateOutlineListIndex(outline_list);

		pParentNode->m_type = NodeType_CombinePanel;
		pParentNode->AddChild(pPlaceCpn);


		break;
	case LayoutOrg_LeftTop:			// ���Ͻ�
		// ����С��
		pPlaceCpn->m_x = cur_line.m_start_x;
		pPlaceCpn->m_y = cur_line.m_start_y - pPlaceCpn->m_RealWidth;

		// ����һ����ֱ�ߺ�һ���µ�

		start_x = cur_line.m_start_x + pPlaceCpn->m_RealLength + b_info.m_x_space;
		start_y = cur_line.m_start_y - pPlaceCpn->m_RealWidth - b_info.m_y_space;
		end_x	= start_x;
		end_y	= cur_line.m_start_y;

		new_ver_line.SetOutline(start_x, end_x, start_y, end_y, NULL);
		new_ver_line.m_type = Outline::OutlineType_Vertical;

		start_x = cur_line.m_start_x + pPlaceCpn->m_RealLength + b_info.m_x_space;
		start_y = cur_line.m_start_y;
		end_x	= cur_line.m_end_x;
		end_y	= cur_line.m_end_y;

		new_hor_line.SetOutline(start_x, end_x, start_y, end_y, NULL);
		new_hor_line.m_type = Outline::OutlineType_Horizon;

		tmp_list.push_back(new_ver_line);
		tmp_list.push_back(new_hor_line);

		// �޸ĵ�ǰ����������

		start_x = cur_line.m_start_x ;
		start_y = cur_line.m_start_y - pPlaceCpn->m_RealWidth - b_info.m_y_space;
		end_x	= new_ver_line.m_start_x;
		end_y	= new_ver_line.m_start_y;
		cur_line.SetOutline(start_x, end_x, start_y, end_y, NULL);

		insert_it = outline_list.begin()+nOutlineID+1;

		outline_list.insert(insert_it, tmp_list.begin(), tmp_list.end());
		ALGORITHM_API::New_UpdateOutlineListIndex(outline_list);

		pParentNode->m_type = NodeType_CombinePanel;
		pParentNode->AddChild(pPlaceCpn);
		


		break;						
	case LayoutOrg_RightBottom:		// ���½�
		// ����С��
		pPlaceCpn->m_x = cur_line.m_end_x - pPlaceCpn->m_RealLength;
		pPlaceCpn->m_y = cur_line.m_end_y;

		// ����һ����ֱ�ߺ�һ���µ�

		start_x = cur_line.m_end_x - pPlaceCpn->m_RealLength - b_info.m_x_space;
		start_y = cur_line.m_end_y ;
		end_x	= start_x;
		end_y	= cur_line.m_end_y + pPlaceCpn->m_RealWidth + b_info.m_y_space;

		new_ver_line.SetOutline(start_x, end_x, start_y, end_y, NULL);
		new_ver_line.m_type = Outline::OutlineType_Vertical;

		start_x = cur_line.m_end_x - pPlaceCpn->m_RealLength - b_info.m_x_space;
		start_y = cur_line.m_end_y + pPlaceCpn->m_RealWidth + b_info.m_y_space;
		end_x	= cur_line.m_end_x;
		end_y	= cur_line.m_end_y + pPlaceCpn->m_RealWidth + b_info.m_y_space;

		new_hor_line.SetOutline(start_x, end_x, start_y, end_y, NULL);
		new_hor_line.m_type = Outline::OutlineType_Horizon;

		tmp_list.push_back(new_ver_line);
		tmp_list.push_back(new_hor_line);

		// �޸ĵ�ǰ����������

		start_x = cur_line.m_start_x ;
		start_y = cur_line.m_start_y ;
		end_x	= new_ver_line.m_start_x;
		end_y	= new_ver_line.m_start_y;
		cur_line.SetOutline(start_x, end_x, start_y, end_y, NULL);

		insert_it = outline_list.begin()+nOutlineID+1;

		outline_list.insert(insert_it, tmp_list.begin(), tmp_list.end());
		ALGORITHM_API::New_UpdateOutlineListIndex(outline_list);

		pParentNode->m_type = NodeType_CombinePanel;
		pParentNode->AddChild(pPlaceCpn);
	

		break;
	case LayoutOrg_RightTop:		// ���Ͻ�
		// ����С��
		pPlaceCpn->m_x = cur_line.m_end_x - pPlaceCpn->m_RealLength;
		pPlaceCpn->m_y = cur_line.m_end_y - pPlaceCpn->m_RealWidth;

		// ����һ����ֱ�ߺ�һ���µ�

		start_x = cur_line.m_end_x - pPlaceCpn->m_RealLength - b_info.m_x_space;
		start_y = cur_line.m_end_y;
		end_x	= start_x;
		end_y	= cur_line.m_start_y - pPlaceCpn->m_RealWidth - b_info.m_y_space;

		new_ver_line.SetOutline(start_x, end_x, start_y, end_y, NULL);
		new_ver_line.m_type = Outline::OutlineType_Vertical;

		start_x = cur_line.m_end_x - pPlaceCpn->m_RealLength - b_info.m_x_space;
		start_y = cur_line.m_end_y - pPlaceCpn->m_RealWidth - b_info.m_y_space;
		end_x	= cur_line.m_end_x;
		end_y	= cur_line.m_end_y - pPlaceCpn->m_RealWidth - b_info.m_y_space;

		new_hor_line.SetOutline(start_x, end_x, start_y, end_y, NULL);
		new_hor_line.m_type = Outline::OutlineType_Horizon;

		tmp_list.push_back(new_ver_line);
		tmp_list.push_back(new_hor_line);

		// �޸ĵ�ǰ����������

		start_x = cur_line.m_start_x ;
		start_y = cur_line.m_start_y;
		end_x	= new_ver_line.m_start_x;
		end_y	= new_ver_line.m_start_y;
		cur_line.SetOutline(start_x, end_x, start_y, end_y, NULL);

		insert_it = outline_list.begin()+nOutlineID+1;

		outline_list.insert(insert_it, tmp_list.begin(), tmp_list.end());
		ALGORITHM_API::New_UpdateOutlineListIndex(outline_list);

		pParentNode->m_type = NodeType_CombinePanel;
		pParentNode->AddChild(pPlaceCpn);

		

		break;
	default:						// Ĭ�����½�


		// ����С��
		pPlaceCpn->m_x = cur_line.m_start_x;
		pPlaceCpn->m_y = cur_line.m_start_y;

		// ����һ����ֱ�ߺ�һ���µ�

		start_x = cur_line.m_start_x + pPlaceCpn->m_RealLength + b_info.m_x_space;
		start_y = cur_line.m_start_y + pPlaceCpn->m_RealWidth + b_info.m_y_space;
		end_x	= start_x;
		end_y	= cur_line.m_start_y;

		new_ver_line.SetOutline(start_x, end_x, start_y, end_y, NULL);
		new_ver_line.m_type = Outline::OutlineType_Vertical;

		start_x = cur_line.m_start_x + pPlaceCpn->m_RealLength + b_info.m_x_space;
		start_y = cur_line.m_start_y;
		end_x	= cur_line.m_end_x;
		end_y	= cur_line.m_end_y;

		new_hor_line.SetOutline(start_x, end_x, start_y, end_y, NULL);
		new_hor_line.m_type = Outline::OutlineType_Horizon;

		tmp_list.push_back(new_ver_line);
		tmp_list.push_back(new_hor_line);

		// �޸ĵ�ǰ����������

		start_x = cur_line.m_start_x ;
		start_y = cur_line.m_start_y + pPlaceCpn->m_RealWidth + b_info.m_y_space;
		end_x	= new_ver_line.m_start_x;
		end_y	= new_ver_line.m_start_y;
		cur_line.SetOutline(start_x, end_x, start_y, end_y, NULL);

		insert_it = outline_list.begin()+nOutlineID+1;

		outline_list.insert(insert_it, tmp_list.begin(), tmp_list.end());
		ALGORITHM_API::New_UpdateOutlineListIndex(outline_list);

		pParentNode->m_type = NodeType_CombinePanel;
		pParentNode->AddChild(pPlaceCpn);


		break;

	}

	return true;
}


bool ALGORITHM_API::New_KnifeOneRemainder(Component* pParentNode, Component* pPlaceCpn, int CutDir, int Org, BaseInfo& b_info)
{
	// �ж�Ҫ�и��С���Ƿ�Ϸ�
	if (pPlaceCpn->IsLegal() == false)
	{
		return false;
	}


	if (CutDir == CutDir_Vertical)
	{

		// ��һ��������һ�������ڵ�һ��Ϊ��
		Component* pLeft = new Component;
		Component* pRight = new Component;

		switch(Org)
		{
		case LayoutOrg_LeftBottom:		// ���½�

			// ��ڵ�
			pLeft->m_x				= pParentNode->m_x;				// ���ڵ��x
			pLeft->m_y				= pParentNode->m_y;				// ���ڵ��y
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;		// С��ĳ���
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;		// ���ڵ���
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x + pPlaceCpn->m_RealLength + /*SawKerfWidth*/b_info.m_x_space;				// ���ڵ����½� + С�峤�� + ���
			pRight->m_y				= pParentNode->m_y;														// ���ڵ��y
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - /*SawKerfWidth*/b_info.m_x_space;		// ���ڵ㳤�� - С�峤�� - ���
			pRight->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;
		case LayoutOrg_LeftTop:			// ���Ͻ�

			// ��ڵ�
			pLeft->m_x				= pParentNode->m_x;				// ���ڵ��x
			pLeft->m_y				= pParentNode->m_y;				// ���ڵ��y
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;		// С��ĳ���
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;		// ���ڵ���
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x + pPlaceCpn->m_RealLength + /*SawKerfWidth*/b_info.m_x_space;				// ���ڵ����½� + С�峤�� + ���
			pRight->m_y				= pParentNode->m_y;														// ���ڵ��y
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - /*SawKerfWidth*/b_info.m_x_space;		// ���ڵ㳤�� - С�峤�� - ���
			pRight->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;


			break;						
		case LayoutOrg_RightBottom:		// ���½�

			// ��ڵ�
			pLeft->m_x				= pParentNode->m_x + pParentNode->m_RealLength - pPlaceCpn->m_RealLength;// ���ڵ��x + ���ڵ�ĳ��� - С�峤��
			pLeft->m_y				= pParentNode->m_y;														// ���ڵ��y
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;												// С��ĳ���
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x;			// ���ڵ��x
			pRight->m_y				= pParentNode->m_y;														// ���ڵ��y
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - /*SawKerfWidth*/b_info.m_x_space;	// ���ڵ㳤�� - С�峤�� - ���
			pRight->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;
		case LayoutOrg_RightTop:		// ���Ͻ�

			// ��ڵ�
			pLeft->m_x				= pParentNode->m_x + pParentNode->m_RealLength - pPlaceCpn->m_RealLength;// ���ڵ��x + ���ڵ�ĳ��� - С�峤��
			pLeft->m_y				= pParentNode->m_y;														// ���ڵ��y
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;												// С��ĳ���
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x;			// ���ڵ��x
			pRight->m_y				= pParentNode->m_y;														// ���ڵ��y
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - /*SawKerfWidth*/b_info.m_x_space;		// ���ڵ㳤�� - С�峤�� - ���
			pRight->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;
		default:						// Ĭ�����½�

			// ��ڵ�
			pLeft->m_x				= pParentNode->m_x;
			pLeft->m_y				= pParentNode->m_y;
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x + pPlaceCpn->m_RealLength + /*SawKerfWidth*/b_info.m_x_space;				// ���ڵ����½� + С�峤�� + ���
			pRight->m_y				= pParentNode->m_y;
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - /*SawKerfWidth*/b_info.m_x_space;		// ���ڵ㳤�� - С�峤�� - ���
			pRight->m_RealWidth		= pParentNode->m_RealWidth;
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;

		}




		// ���ڵ�һ��Ϊ��, ���ڵ����ʹ����ϱ�Ϊ��ϰ�
		pParentNode->m_type = NodeType_CombinePanel;

		if (pLeft->IsLegal() == true)
		{
			pParentNode->AddChild(pLeft);
		}
		else
		{
			delete pLeft;
			pLeft = NULL;
		}

		if (pRight->IsLegal() == true)
		{
			pParentNode->AddChild(pRight);
		}
		else
		{
			delete pRight;
			pRight = NULL;
		}



		// �ڶ��� ����һ������ڵ���һ��Ϊ������Ϊ��Ҫ��С�������
		Component* pSecondRight = new Component;
		switch(Org)
		{
		case LayoutOrg_LeftBottom:		// ���½�

			// ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;
			pPlaceCpn->m_y = pLeft->m_y;
			pPlaceCpn->m_type = NodeType_NeededComponent;


			// ��һ������
			pSecondRight->m_x			= pLeft->m_x;													// ���ڵ��x
			pSecondRight->m_y			= pLeft->m_y + pPlaceCpn->m_RealWidth + /*SawKerfWidth*/b_info.m_y_space;				// ���ڵ��y + С���� + ���
			pSecondRight->m_RealLength	= pLeft->m_RealLength;											// ���ڵ�ĳ�
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - /*SawKerfWidth*/b_info.m_y_space;	// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;

			break;
		case LayoutOrg_LeftTop:			// ���Ͻ�

			// ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;												// ���ڵ��x
			pPlaceCpn->m_y = pLeft->m_y + pLeft->m_RealWidth - pPlaceCpn->m_RealWidth;	// ���ڵ��y + ���ڵ��� - С��Ŀ��
			pPlaceCpn->m_type = NodeType_NeededComponent;


			// ��һ������
			pSecondRight->m_x			= pLeft->m_x;													// ���ڵ��x
			pSecondRight->m_y			= pLeft->m_y;													// ���ڵ��y
			pSecondRight->m_RealLength	= pLeft->m_RealLength;											// ���ڵ�ĳ�
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - /*SawKerfWidth*/b_info.m_y_space;	// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;

			break;						
		case LayoutOrg_RightBottom:		// ���½�

			// ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;	// ���ڵ��x
			pPlaceCpn->m_y = pLeft->m_y;	// ���ڵ��y
			pPlaceCpn->m_type = NodeType_NeededComponent;


			// ��һ������
			pSecondRight->m_x			= pLeft->m_x;													// ���ڵ��x
			pSecondRight->m_y			= pLeft->m_y  + pPlaceCpn->m_RealWidth + /*SawKerfWidth*/b_info.m_y_space;			// ���ڵ��y + С��Ŀ�� + ���
			pSecondRight->m_RealLength	= pLeft->m_RealLength;											// ���ڵ�ĳ�
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - /*SawKerfWidth*/b_info.m_y_space;	// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;




			break;
		case LayoutOrg_RightTop:		// ���Ͻ�

			// ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;												// ���ڵ��x
			pPlaceCpn->m_y = pLeft->m_y + pLeft->m_RealWidth - pPlaceCpn->m_RealWidth;	// ���ڵ��y + ���ڵ��� - С��Ŀ��
			pPlaceCpn->m_type = NodeType_NeededComponent;


			// ��һ������
			pSecondRight->m_x			= pLeft->m_x;													// ���ڵ��x
			pSecondRight->m_y			= pLeft->m_y;													// ���ڵ��y
			pSecondRight->m_RealLength	= pLeft->m_RealLength;											// ���ڵ�ĳ�
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - /*SawKerfWidth*/b_info.m_y_space;	// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;


			break;
		default:						// Ĭ�����½�

			// ����С��,��С��������ɾ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;
			pPlaceCpn->m_y = pLeft->m_y;
			pPlaceCpn->m_type = NodeType_NeededComponent;


			// ��һ������
			pSecondRight->m_x			= pLeft->m_x;			
			pSecondRight->m_y			= pLeft->m_y + pPlaceCpn->m_RealWidth + /*SawKerfWidth*/b_info.m_y_space;		// ���ڵ����½� + С���� + ���
			pSecondRight->m_RealLength	= pLeft->m_RealLength;	
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - /*SawKerfWidth*/b_info.m_y_space;	// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;

			break;
		}


		// ��ڵ�����ӽڵ㣬���ͱ�Ϊ��ϰ�
		pLeft->m_type = NodeType_CombinePanel;

		pLeft->AddChild(pPlaceCpn);

		if (pSecondRight->IsLegal() == true)
		{
			pLeft->AddChild(pSecondRight);
		}
		else
		{
			delete pSecondRight;
			pSecondRight = NULL;
		}



	}
	else
	{
		// ��һ��������һ�������ڵ�һ��Ϊ��

		Component* pLeft = new Component;	
		Component* pRight = new Component;

		switch(Org)
		{
		case LayoutOrg_LeftBottom:		// ���½�

			// ��ڵ�													
			pLeft->m_x				= pParentNode->m_x;				// ���ڵ����½� 
			pLeft->m_y				= pParentNode->m_y;				// ���ڵ����½� 
			pLeft->m_RealLength		= pParentNode->m_RealLength;	// ���ڵ㳤�� 
			pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// С����
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x;																		// ���ڵ����½� 
			pRight->m_y				= pParentNode->m_y + pPlaceCpn->m_RealWidth + /*SawKerfWidth*/b_info.m_y_space;			// ���ڵ����½� + С����
			pRight->m_RealLength	= pParentNode->m_RealLength;															// ���ڵ㳤��
			pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - /*SawKerfWidth*/b_info.m_y_space;						// ���ڵ��� - С���� - ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;

		case LayoutOrg_LeftTop:			// ���Ͻ�

			// ��ڵ�																
			pLeft->m_x				= pParentNode->m_x;				// ���ڵ�x  
			pLeft->m_y				= pParentNode->m_y + pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth;				// ���ڵ�y + ���ڵ��� - С����
			pLeft->m_RealLength		= pParentNode->m_RealLength;	// ���ڵ㳤�� 
			pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// С����
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x;																			// ���ڵ�x
			pRight->m_y				= pParentNode->m_y ;																		// ���ڵ�y
			pRight->m_RealLength	= pParentNode->m_RealLength;																// ���ڵ㳤��
			pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - /*SawKerfWidth*/b_info.m_y_space;		// ���ڵ��� - С���� - ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;


			break;

		case LayoutOrg_RightBottom:		// ���½�

			// ��ڵ�															
			pLeft->m_x				= pParentNode->m_x;				// ���ڵ����½� 
			pLeft->m_y				= pParentNode->m_y;				// ���ڵ����½� 
			pLeft->m_RealLength		= pParentNode->m_RealLength;	// ���ڵ㳤�� 
			pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// С����
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x;																			// ���ڵ����½� 
			pRight->m_y				= pParentNode->m_y + pPlaceCpn->m_RealWidth + /*SawKerfWidth*/b_info.m_y_space;				// ���ڵ����½� + С����
			pRight->m_RealLength	= pParentNode->m_RealLength;																// ���ڵ㳤��
			pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - /*SawKerfWidth*/b_info.m_y_space;		// ���ڵ��� - С���� - ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;


			break;

		case LayoutOrg_RightTop:		// ���Ͻ�
			// ��ڵ�															
			pLeft->m_x				= pParentNode->m_x;														// ���ڵ�x  
			pLeft->m_y				= pParentNode->m_y + pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth;	// ���ڵ�y + ���ڵ��� - С����
			pLeft->m_RealLength		= pParentNode->m_RealLength;											// ���ڵ㳤�� 
			pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;												// С����
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x;														// ���ڵ�x
			pRight->m_y				= pParentNode->m_y ;													// ���ڵ�y
			pRight->m_RealLength	= pParentNode->m_RealLength;											// ���ڵ㳤��
			pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - /*SawKerfWidth*/b_info.m_y_space;		// ���ڵ��� - С���� - ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;

		default:						// Ĭ�����½�

			// ��ڵ�															
			pLeft->m_x				= pParentNode->m_x;				// ���ڵ����½� 
			pLeft->m_y				= pParentNode->m_y;				// ���ڵ����½� 
			pLeft->m_RealLength		= pParentNode->m_RealLength;	// ���ڵ㳤�� 
			pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// С����
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x;																// ���ڵ����½� 
			pRight->m_y				= pParentNode->m_y + pPlaceCpn->m_RealWidth + /*SawKerfWidth*/b_info.m_y_space;				// ���ڵ����½� + С����
			pRight->m_RealLength	= pParentNode->m_RealLength;													// ���ڵ㳤��
			pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - /*SawKerfWidth*/b_info.m_y_space;		// ���ڵ��� - С���� - ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;


			break;
		}


		// ���ڵ�һ��Ϊ��, ���ڵ����ʹ����ϱ�Ϊ��ϰ�
		pParentNode->m_type = NodeType_CombinePanel;

		if (pLeft->IsLegal() == true)
		{
			pParentNode->AddChild(pLeft);
		}
		else
		{
			delete pLeft;
			pLeft = NULL;
		}

		if (pRight->IsLegal() == true)
		{
			pParentNode->AddChild(pRight);
		}
		else
		{
			delete pRight;
			pRight = NULL;
		}
		// ����һ�����ҽڵ���һ��Ϊ������Ϊ��Ҫ��С�������

		Component* pSecondRight = new Component;
		switch(Org)
		{
		case LayoutOrg_LeftBottom:		// ���½�

			// ����С��,��С��������ɾ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;		// ���ڵ��x
			pPlaceCpn->m_y = pLeft->m_y;		// ���ڵ��y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// ��һ������
			pSecondRight->m_x			= pLeft->m_x + pPlaceCpn->m_RealLength + /*SawKerfWidth*/b_info.m_x_space;			// ���ڵ�x + С���� + ���
			pSecondRight->m_y			= pLeft->m_y;																		// ���ڵ�y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - /*SawKerfWidth*/b_info.m_x_space;	// ���ڵ㳤�� - С�峤�� - ���
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;																// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;

			break;
		case LayoutOrg_LeftTop:			// ���Ͻ�

			// ����С��,��С��������ɾ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;		// ���ڵ�x
			pPlaceCpn->m_y = pLeft->m_y;		// ���ڵ�y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// ��һ������
			pSecondRight->m_x			= pLeft->m_x + pPlaceCpn->m_RealLength + /*SawKerfWidth*/b_info.m_x_space;			// ���ڵ�x + С���� + ���
			pSecondRight->m_y			= pLeft->m_y;																		// ���ڵ�y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - /*SawKerfWidth*/b_info.m_x_space;	// ���ڵ㳤�� - С�峤�� - ���
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;																// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;


			break;						
		case LayoutOrg_RightBottom:		// ���½�

			// ����С��,��С��������ɾ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x + pLeft->m_RealLength - pPlaceCpn->m_RealLength;	// ���ڵ�x + ���ڵ㳤�� - С�峤��
			pPlaceCpn->m_y = pLeft->m_y;													// ���ڵ�y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// ��һ������
			pSecondRight->m_x			= pLeft->m_x;																		// ���ڵ�x 
			pSecondRight->m_y			= pLeft->m_y;																		// ���ڵ�y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - /*SawKerfWidth*/b_info.m_x_space;	// ���ڵ㳤�� - С�峤�� - ���
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;																// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;

			break;
		case LayoutOrg_RightTop:		// ���Ͻ�

			// ����С��,��С��������ɾ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x + pLeft->m_RealLength - pPlaceCpn->m_RealLength;		// ���ڵ�x + ���ڵ㳤�� - С�峤��
			pPlaceCpn->m_y = pLeft->m_y;		// ���ڵ�y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// ��һ������
			pSecondRight->m_x			= pLeft->m_x;																		// ���ڵ�x 
			pSecondRight->m_y			= pLeft->m_y;																		// ���ڵ�y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - /*SawKerfWidth*/b_info.m_x_space;	// ���ڵ㳤�� - С�峤�� - ���
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;																// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;


			break;
		default:						// Ĭ�����½�

			// ����С��,��С��������ɾ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;		// ���ڵ��x
			pPlaceCpn->m_y = pLeft->m_y;		// ���ڵ��y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// ��һ������
			pSecondRight->m_x			= pLeft->m_x + pPlaceCpn->m_RealLength + /*SawKerfWidth*/b_info.m_x_space;			// ���ڵ�x + С���� + ���
			pSecondRight->m_y			= pLeft->m_y;																		// ���ڵ�y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - /*SawKerfWidth*/b_info.m_x_space;	// ���ڵ㳤�� - С�峤�� - ���
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;																// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;


			break;
		}




		// ��ڵ�����ӽڵ㣬���ͱ�Ϊ��ϰ�
		pLeft->m_type = NodeType_CombinePanel;
		pLeft->AddChild(pPlaceCpn);

		if (pSecondRight->IsLegal() == true)
		{
			pLeft->AddChild(pSecondRight);
		}
		else
		{
			delete pSecondRight;
			pSecondRight = NULL;
		}
	}

	return true;
}

/*---------------------------------------*/
//	����˵����
//		��һ�������и��һ����Ҫ��С�壬�����ʣ�࣬
//		��������0~2������
//		
//		ע����ڵ�Ϊ����С����������С������Ͻڵ㣬
//			�ҽڵ�Ϊ���Ͻڵ㣬�����귽���޹�
//				
//
//	������
//		Component* pParentNode	--	���и�İ��ָ�� 
//		Component* pPlaceCpn	--	��Ҫ��С��ָ��
//		int CutDir				--	�и��	0:����	1:����
//		float SawKerfWidth		--	��Ƭ���	
//		
//	����ֵ:
//
//
/*---------------------------------------*/
bool ALGORITHM_API::KnifeOneRemainder(Component* pParentNode, Component* pPlaceCpn, int CutDir, float SawKerfWidth, int Org)
{
	// �ж�Ҫ�и��С���Ƿ�Ϸ�
	if (pPlaceCpn->IsLegal() == false)
	{
		return false;
	}


	if (CutDir == CutDir_Vertical)
	{

		// ��һ��������һ�������ڵ�һ��Ϊ��
		Component* pLeft = new Component;
		Component* pRight = new Component;

		switch(Org)
		{
		case LayoutOrg_LeftBottom:		// ���½�

			// ��ڵ�
			pLeft->m_x				= pParentNode->m_x;				// ���ڵ��x
			pLeft->m_y				= pParentNode->m_y;				// ���ڵ��y
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;		// С��ĳ���
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;		// ���ڵ���
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// ���ڵ����½� + С�峤�� + ���
			pRight->m_y				= pParentNode->m_y;														// ���ڵ��y
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pRight->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;
		case LayoutOrg_LeftTop:			// ���Ͻ�

			// ��ڵ�
			pLeft->m_x				= pParentNode->m_x;				// ���ڵ��x
			pLeft->m_y				= pParentNode->m_y;				// ���ڵ��y
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;		// С��ĳ���
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;		// ���ڵ���
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// ���ڵ����½� + С�峤�� + ���
			pRight->m_y				= pParentNode->m_y;														// ���ڵ��y
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pRight->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;


			break;						
		case LayoutOrg_RightBottom:		// ���½�

			// ��ڵ�
			pLeft->m_x				= pParentNode->m_x + pParentNode->m_RealLength - pPlaceCpn->m_RealLength;// ���ڵ��x + ���ڵ�ĳ��� - С�峤��
			pLeft->m_y				= pParentNode->m_y;														// ���ڵ��y
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;												// С��ĳ���
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x;			// ���ڵ��x
			pRight->m_y				= pParentNode->m_y;														// ���ڵ��y
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pRight->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;
		case LayoutOrg_RightTop:		// ���Ͻ�

			// ��ڵ�
			pLeft->m_x				= pParentNode->m_x + pParentNode->m_RealLength - pPlaceCpn->m_RealLength;// ���ڵ��x + ���ڵ�ĳ��� - С�峤��
			pLeft->m_y				= pParentNode->m_y;														// ���ڵ��y
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;												// С��ĳ���
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x;			// ���ڵ��x
			pRight->m_y				= pParentNode->m_y;														// ���ڵ��y
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pRight->m_RealWidth		= pParentNode->m_RealWidth;												// ���ڵ���
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;
		default:						// Ĭ�����½�

			// ��ڵ�
			pLeft->m_x				= pParentNode->m_x;
			pLeft->m_y				= pParentNode->m_y;
			pLeft->m_RealLength		= pPlaceCpn->m_RealLength;
			pLeft->m_RealWidth		= pParentNode->m_RealWidth;
			pLeft->m_Thickness		= pParentNode->m_Thickness;
			pLeft->m_Texture		= pParentNode->m_Texture;
			pLeft->m_type			= NodeType_Remainder;
			pLeft->m_Material		= pParentNode->m_Material;

			// �ҽڵ�
			pRight->m_x				= pParentNode->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// ���ڵ����½� + С�峤�� + ���
			pRight->m_y				= pParentNode->m_y;
			pRight->m_RealLength	= pParentNode->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pRight->m_RealWidth		= pParentNode->m_RealWidth;
			pRight->m_Thickness		= pParentNode->m_Thickness;
			pRight->m_Texture		= pParentNode->m_Texture;
			pRight->m_type			= NodeType_Remainder;
			pRight->m_Material		= pParentNode->m_Material;

			break;
				
		}
		



		// ���ڵ�һ��Ϊ��, ���ڵ����ʹ����ϱ�Ϊ��ϰ�
		pParentNode->m_type = NodeType_CombinePanel;

		if (pLeft->IsLegal() == true)
		{
			pParentNode->AddChild(pLeft);
		}
		else
		{
			delete pLeft;
			pLeft = NULL;
		}

		if (pRight->IsLegal() == true)
		{
			pParentNode->AddChild(pRight);
		}
		else
		{
			delete pRight;
			pRight = NULL;
		}



		// �ڶ��� ����һ������ڵ���һ��Ϊ������Ϊ��Ҫ��С�������
		Component* pSecondRight = new Component;
		switch(Org)
		{
			case LayoutOrg_LeftBottom:		// ���½�

				// ����С��
				pPlaceCpn->m_pParent = pLeft;
				pPlaceCpn->m_x = pLeft->m_x;
				pPlaceCpn->m_y = pLeft->m_y;
				pPlaceCpn->m_type = NodeType_NeededComponent;


				// ��һ������
				pSecondRight->m_x			= pLeft->m_x;													// ���ڵ��x
				pSecondRight->m_y			= pLeft->m_y + pPlaceCpn->m_RealWidth + SawKerfWidth;			// ���ڵ��y + С���� + ���
				pSecondRight->m_RealLength	= pLeft->m_RealLength;											// ���ڵ�ĳ�
				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;	// ���ڵ��� - С���� - ���
				pSecondRight->m_Thickness	= pLeft->m_Thickness;
				pSecondRight->m_Texture		= pLeft->m_Texture;
				pSecondRight->m_type		= NodeType_Remainder;
				pSecondRight->m_Material	= pLeft->m_Material;

				break;
			case LayoutOrg_LeftTop:			// ���Ͻ�

				// ����С��
				pPlaceCpn->m_pParent = pLeft;
				pPlaceCpn->m_x = pLeft->m_x;												// ���ڵ��x
				pPlaceCpn->m_y = pLeft->m_y + pLeft->m_RealWidth - pPlaceCpn->m_RealWidth;	// ���ڵ��y + ���ڵ��� - С��Ŀ��
				pPlaceCpn->m_type = NodeType_NeededComponent;


				// ��һ������
				pSecondRight->m_x			= pLeft->m_x;													// ���ڵ��x
				pSecondRight->m_y			= pLeft->m_y;													// ���ڵ��y
				pSecondRight->m_RealLength	= pLeft->m_RealLength;											// ���ڵ�ĳ�
				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;	// ���ڵ��� - С���� - ���
				pSecondRight->m_Thickness	= pLeft->m_Thickness;
				pSecondRight->m_Texture		= pLeft->m_Texture;
				pSecondRight->m_type		= NodeType_Remainder;
				pSecondRight->m_Material	= pLeft->m_Material;

				break;						
			case LayoutOrg_RightBottom:		// ���½�

				// ����С��
				pPlaceCpn->m_pParent = pLeft;
				pPlaceCpn->m_x = pLeft->m_x;	// ���ڵ��x
				pPlaceCpn->m_y = pLeft->m_y;	// ���ڵ��y
				pPlaceCpn->m_type = NodeType_NeededComponent;


				// ��һ������
				pSecondRight->m_x			= pLeft->m_x;													// ���ڵ��x
				pSecondRight->m_y			= pLeft->m_y  + pPlaceCpn->m_RealWidth + SawKerfWidth;			// ���ڵ��y + С��Ŀ�� + ���
				pSecondRight->m_RealLength	= pLeft->m_RealLength;											// ���ڵ�ĳ�
				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;	// ���ڵ��� - С���� - ���
				pSecondRight->m_Thickness	= pLeft->m_Thickness;
				pSecondRight->m_Texture		= pLeft->m_Texture;
				pSecondRight->m_type		= NodeType_Remainder;
				pSecondRight->m_Material	= pLeft->m_Material;




				break;
			case LayoutOrg_RightTop:		// ���Ͻ�

				// ����С��
				pPlaceCpn->m_pParent = pLeft;
				pPlaceCpn->m_x = pLeft->m_x;												// ���ڵ��x
				pPlaceCpn->m_y = pLeft->m_y + pLeft->m_RealWidth - pPlaceCpn->m_RealWidth;	// ���ڵ��y + ���ڵ��� - С��Ŀ��
				pPlaceCpn->m_type = NodeType_NeededComponent;


				// ��һ������
				pSecondRight->m_x			= pLeft->m_x;													// ���ڵ��x
				pSecondRight->m_y			= pLeft->m_y;													// ���ڵ��y
				pSecondRight->m_RealLength	= pLeft->m_RealLength;											// ���ڵ�ĳ�
				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;	// ���ڵ��� - С���� - ���
				pSecondRight->m_Thickness	= pLeft->m_Thickness;
				pSecondRight->m_Texture		= pLeft->m_Texture;
				pSecondRight->m_type		= NodeType_Remainder;
				pSecondRight->m_Material	= pLeft->m_Material;


				break;
			default:						// Ĭ�����½�

				// ����С��,��С��������ɾ����С��
				pPlaceCpn->m_pParent = pLeft;
				pPlaceCpn->m_x = pLeft->m_x;
				pPlaceCpn->m_y = pLeft->m_y;
				pPlaceCpn->m_type = NodeType_NeededComponent;


				// ��һ������
				pSecondRight->m_x			= pLeft->m_x;			
				pSecondRight->m_y			= pLeft->m_y + pPlaceCpn->m_RealWidth + SawKerfWidth;			// ���ڵ����½� + С���� + ���
				pSecondRight->m_RealLength	= pLeft->m_RealLength;	
				pSecondRight->m_RealWidth	= pLeft->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;	// ���ڵ��� - С���� - ���
				pSecondRight->m_Thickness	= pLeft->m_Thickness;
				pSecondRight->m_Texture		= pLeft->m_Texture;
				pSecondRight->m_type		= NodeType_Remainder;
				pSecondRight->m_Material	= pLeft->m_Material;

				break;
		}


		// ��ڵ�����ӽڵ㣬���ͱ�Ϊ��ϰ�
		pLeft->m_type = NodeType_CombinePanel;

		pLeft->AddChild(pPlaceCpn);

		if (pSecondRight->IsLegal() == true)
		{
			pLeft->AddChild(pSecondRight);
		}
		else
		{
			delete pSecondRight;
			pSecondRight = NULL;
		}



	}
	else
	{
		// ��һ��������һ�������ڵ�һ��Ϊ��

		Component* pLeft = new Component;	
		Component* pRight = new Component;

		switch(Org)
		{
			case LayoutOrg_LeftBottom:		// ���½�

				// ��ڵ�													
				pLeft->m_x				= pParentNode->m_x;				// ���ڵ����½� 
				pLeft->m_y				= pParentNode->m_y;				// ���ڵ����½� 
				pLeft->m_RealLength		= pParentNode->m_RealLength;	// ���ڵ㳤�� 
				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// С����
				pLeft->m_Thickness		= pParentNode->m_Thickness;
				pLeft->m_Texture		= pParentNode->m_Texture;
				pLeft->m_type			= NodeType_Remainder;
				pLeft->m_Material		= pParentNode->m_Material;

				// �ҽڵ�
				pRight->m_x				= pParentNode->m_x;																// ���ڵ����½� 
				pRight->m_y				= pParentNode->m_y + pPlaceCpn->m_RealWidth + SawKerfWidth;				// ���ڵ����½� + С����
				pRight->m_RealLength	= pParentNode->m_RealLength;													// ���ڵ㳤��
				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;		// ���ڵ��� - С���� - ���
				pRight->m_Thickness		= pParentNode->m_Thickness;
				pRight->m_Texture		= pParentNode->m_Texture;
				pRight->m_type			= NodeType_Remainder;
				pRight->m_Material		= pParentNode->m_Material;

				break;

			case LayoutOrg_LeftTop:			// ���Ͻ�

				// ��ڵ�																
				pLeft->m_x				= pParentNode->m_x;				// ���ڵ�x  
				pLeft->m_y				= pParentNode->m_y + pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth;				// ���ڵ�y + ���ڵ��� - С����
				pLeft->m_RealLength		= pParentNode->m_RealLength;	// ���ڵ㳤�� 
				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// С����
				pLeft->m_Thickness		= pParentNode->m_Thickness;
				pLeft->m_Texture		= pParentNode->m_Texture;
				pLeft->m_type			= NodeType_Remainder;
				pLeft->m_Material		= pParentNode->m_Material;

				// �ҽڵ�
				pRight->m_x				= pParentNode->m_x;														// ���ڵ�x
				pRight->m_y				= pParentNode->m_y ;													// ���ڵ�y
				pRight->m_RealLength	= pParentNode->m_RealLength;											// ���ڵ㳤��
				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;		// ���ڵ��� - С���� - ���
				pRight->m_Thickness		= pParentNode->m_Thickness;
				pRight->m_Texture		= pParentNode->m_Texture;
				pRight->m_type			= NodeType_Remainder;
				pRight->m_Material		= pParentNode->m_Material;


				break;

			case LayoutOrg_RightBottom:		// ���½�

				// ��ڵ�															
				pLeft->m_x				= pParentNode->m_x;				// ���ڵ����½� 
				pLeft->m_y				= pParentNode->m_y;				// ���ڵ����½� 
				pLeft->m_RealLength		= pParentNode->m_RealLength;	// ���ڵ㳤�� 
				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// С����
				pLeft->m_Thickness		= pParentNode->m_Thickness;
				pLeft->m_Texture		= pParentNode->m_Texture;
				pLeft->m_type			= NodeType_Remainder;
				pLeft->m_Material		= pParentNode->m_Material;

				// �ҽڵ�
				pRight->m_x				= pParentNode->m_x;														// ���ڵ����½� 
				pRight->m_y				= pParentNode->m_y + pPlaceCpn->m_RealWidth + SawKerfWidth;				// ���ڵ����½� + С����
				pRight->m_RealLength	= pParentNode->m_RealLength;											// ���ڵ㳤��
				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;		// ���ڵ��� - С���� - ���
				pRight->m_Thickness		= pParentNode->m_Thickness;
				pRight->m_Texture		= pParentNode->m_Texture;
				pRight->m_type			= NodeType_Remainder;
				pRight->m_Material		= pParentNode->m_Material;


				break;

			case LayoutOrg_RightTop:		// ���Ͻ�
				// ��ڵ�															
				pLeft->m_x				= pParentNode->m_x;														// ���ڵ�x  
				pLeft->m_y				= pParentNode->m_y + pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth;	// ���ڵ�y + ���ڵ��� - С����
				pLeft->m_RealLength		= pParentNode->m_RealLength;											// ���ڵ㳤�� 
				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;												// С����
				pLeft->m_Thickness		= pParentNode->m_Thickness;
				pLeft->m_Texture		= pParentNode->m_Texture;
				pLeft->m_type			= NodeType_Remainder;
				pLeft->m_Material		= pParentNode->m_Material;

				// �ҽڵ�
				pRight->m_x				= pParentNode->m_x;														// ���ڵ�x
				pRight->m_y				= pParentNode->m_y ;													// ���ڵ�y
				pRight->m_RealLength	= pParentNode->m_RealLength;											// ���ڵ㳤��
				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;		// ���ڵ��� - С���� - ���
				pRight->m_Thickness		= pParentNode->m_Thickness;
				pRight->m_Texture		= pParentNode->m_Texture;
				pRight->m_type			= NodeType_Remainder;
				pRight->m_Material		= pParentNode->m_Material;

				break;

			default:						// Ĭ�����½�

				// ��ڵ�															
				pLeft->m_x				= pParentNode->m_x;				// ���ڵ����½� 
				pLeft->m_y				= pParentNode->m_y;				// ���ڵ����½� 
				pLeft->m_RealLength		= pParentNode->m_RealLength;	// ���ڵ㳤�� 
				pLeft->m_RealWidth		= pPlaceCpn->m_RealWidth;		// С����
				pLeft->m_Thickness		= pParentNode->m_Thickness;
				pLeft->m_Texture		= pParentNode->m_Texture;
				pLeft->m_type			= NodeType_Remainder;
				pLeft->m_Material		= pParentNode->m_Material;

				// �ҽڵ�
				pRight->m_x				= pParentNode->m_x;																// ���ڵ����½� 
				pRight->m_y				= pParentNode->m_y + pPlaceCpn->m_RealWidth + SawKerfWidth;				// ���ڵ����½� + С����
				pRight->m_RealLength	= pParentNode->m_RealLength;													// ���ڵ㳤��
				pRight->m_RealWidth		= pParentNode->m_RealWidth - pPlaceCpn->m_RealWidth - SawKerfWidth;		// ���ڵ��� - С���� - ���
				pRight->m_Thickness		= pParentNode->m_Thickness;
				pRight->m_Texture		= pParentNode->m_Texture;
				pRight->m_type			= NodeType_Remainder;
				pRight->m_Material		= pParentNode->m_Material;


				break;
		}


		// ���ڵ�һ��Ϊ��, ���ڵ����ʹ����ϱ�Ϊ��ϰ�
		pParentNode->m_type = NodeType_CombinePanel;

		if (pLeft->IsLegal() == true)
		{
			pParentNode->AddChild(pLeft);
		}
		else
		{
			delete pLeft;
			pLeft = NULL;
		}

		if (pRight->IsLegal() == true)
		{
			pParentNode->AddChild(pRight);
		}
		else
		{
			delete pRight;
			pRight = NULL;
		}
		// ����һ�����ҽڵ���һ��Ϊ������Ϊ��Ҫ��С�������

		Component* pSecondRight = new Component;
		switch(Org)
		{
		case LayoutOrg_LeftBottom:		// ���½�

			// ����С��,��С��������ɾ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;		// ���ڵ��x
			pPlaceCpn->m_y = pLeft->m_y;		// ���ڵ��y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// ��һ������
			pSecondRight->m_x			= pLeft->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// ���ڵ�x + С���� + ���
			pSecondRight->m_y			= pLeft->m_y;													// ���ڵ�y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;											// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;
			
			break;
		case LayoutOrg_LeftTop:			// ���Ͻ�

			// ����С��,��С��������ɾ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;		// ���ڵ�x
			pPlaceCpn->m_y = pLeft->m_y;		// ���ڵ�y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// ��һ������
			pSecondRight->m_x			= pLeft->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// ���ڵ�x + С���� + ���
			pSecondRight->m_y			= pLeft->m_y;													// ���ڵ�y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;											// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;


			break;						
		case LayoutOrg_RightBottom:		// ���½�

			// ����С��,��С��������ɾ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x + pLeft->m_RealLength - pPlaceCpn->m_RealLength;	// ���ڵ�x + ���ڵ㳤�� - С�峤��
			pPlaceCpn->m_y = pLeft->m_y;													// ���ڵ�y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// ��һ������
			pSecondRight->m_x			= pLeft->m_x;													// ���ڵ�x 
			pSecondRight->m_y			= pLeft->m_y;													// ���ڵ�y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;											// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;

			break;
		case LayoutOrg_RightTop:		// ���Ͻ�

			// ����С��,��С��������ɾ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x + pLeft->m_RealLength - pPlaceCpn->m_RealLength;		// ���ڵ�x + ���ڵ㳤�� - С�峤��
			pPlaceCpn->m_y = pLeft->m_y;		// ���ڵ�y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// ��һ������
			pSecondRight->m_x			= pLeft->m_x;													// ���ڵ�x 
			pSecondRight->m_y			= pLeft->m_y;													// ���ڵ�y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;											// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;
			

			break;
		default:						// Ĭ�����½�

			// ����С��,��С��������ɾ����С��
			pPlaceCpn->m_pParent = pLeft;
			pPlaceCpn->m_x = pLeft->m_x;		// ���ڵ��x
			pPlaceCpn->m_y = pLeft->m_y;		// ���ڵ��y
			pPlaceCpn->m_type = NodeType_NeededComponent;

			// ��һ������
			pSecondRight->m_x			= pLeft->m_x + pPlaceCpn->m_RealLength + SawKerfWidth;			// ���ڵ�x + С���� + ���
			pSecondRight->m_y			= pLeft->m_y;													// ���ڵ�y 
			pSecondRight->m_RealLength	= pLeft->m_RealLength - pPlaceCpn->m_RealLength - SawKerfWidth;	// ���ڵ㳤�� - С�峤�� - ���
			pSecondRight->m_RealWidth	= pLeft->m_RealWidth ;											// ���ڵ��� - С���� - ���
			pSecondRight->m_Thickness	= pLeft->m_Thickness;
			pSecondRight->m_Texture		= pLeft->m_Texture;
			pSecondRight->m_type		= NodeType_Remainder;
			pSecondRight->m_Material	= pLeft->m_Material;
			

			break;
		}


		

		// ��ڵ�����ӽڵ㣬���ͱ�Ϊ��ϰ�
		pLeft->m_type = NodeType_CombinePanel;
		pLeft->AddChild(pPlaceCpn);

		if (pSecondRight->IsLegal() == true)
		{
			pLeft->AddChild(pSecondRight);
		}
		else
		{
			delete pSecondRight;
			pSecondRight = NULL;
		}
	}

	return true;
}



/*--------------------------------------------------------------------------------------*/
//	purpose:
//		��ʼ������������
//
//	param:
//		
//
//	return:
//		0 -- �ɹ�
/*--------------------------------------------------------------------------------------*/
int ALGORITHM_API::BuildRemainderList(Panel* pParent, vector<Component*>& RemainderList)
{
	// �������������
	RemainderList.clear();

	// ��ȡ���ϣ�����������
	vector<Node*> list;
	pParent->GetAllLeafNodes(list, NodeType_Remainder);	

	int i, nCount;
	nCount = list.size();

	// Node*תΪComponent*
	for(i = 0; i < nCount; i++)
	{
		Component* pNode = static_cast<Component*>(list[i]);
		RemainderList.push_back(pNode);
	}

	return 0;
}


/*--------------------------------------------------------------------------------------*/
//	purpose:
//		ƥ����ʵ�С��������ߡ�
//		̰���㷨��˼�룺ÿ�ε����ϳ��Ȼ��߿�ȱ������õ�����˷���С��ѡ��һ����ȫƥ���
//						���Ȼ��߿�Ⱦ��˳�������ƥ����س��ȷ����и���ƥ����ؿ�ȷ���
//						�и�
//		
//		
//	
//	param:
//		int &nCpnID			--	С��ID
//		int &nOutlineID		--	������ID
//
//	return:
//		true -- ��ƥ�䵽
//		false-- �޷�ƥ��
/*--------------------------------------------------------------------------------------*/

bool ALGORITHM_API::MatchSuitableComponentNRemaider(vector<Component*>& RemainderList, vector<Component*>& ComponentList, Component*& pPerfectMatchComponent, bool &rotate_flag, int &cut_dir, Component*& pPerfectMatchRemainder, BaseInfo& base_info)
{
	int i_remainder, nRemainderNum = RemainderList.size();
	int i_cpn, nLeftCpnSize = ComponentList.size();

	bool bMatchFlag = false;
	bool bRotateFlag = false;
	float min_remain_length, min_remain_width, min_dist;

	if (base_info.m_bCustomerFirst == true)
	{

		for(i_remainder = 0; i_remainder < nRemainderNum; i_remainder++)
		{
			Component* pRmd = RemainderList.at(i_remainder);

			for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
			{
				Component* pCpn = ComponentList.at(i_cpn);

				if (pRmd->Containable(pCpn) == true)
				{
					if(bMatchFlag == false)
					{
						//��һ��ѡ��
						min_remain_length = pRmd->m_RealLength - pCpn->m_RealLength;
						min_remain_width = pRmd->m_RealWidth - pCpn->m_RealWidth;

						min_dist = min(min_remain_length, min_remain_width);

						pPerfectMatchComponent = pCpn;
						pPerfectMatchRemainder = pRmd;
						bRotateFlag = false;
						rotate_flag = bRotateFlag;
						bMatchFlag = true;


						// ��ȫƥ�䣬����
						if (min_remain_length == 0.0)
						{
							cut_dir = CutDir_Horizon;

							return true;
						}
						else if (min_remain_width == 0.0)
						{
							cut_dir = CutDir_Vertical;

							return true;
						}
						else
						{
							if (min_remain_width < min_remain_length)
							{
								cut_dir = CutDir_Vertical;
							}
							else
							{
								cut_dir = CutDir_Horizon;
							}
						}

					}
					else
					{
						float remaim_length = pRmd->m_RealLength - pCpn->m_RealLength;
						float remain_width = pRmd->m_RealWidth - pCpn->m_RealWidth;

						// ������ƥ��� ���Ҳ�������з�����Ϣ�ҵ�ǰ����޷�����Ϣ
						if (	(remaim_length < min_dist || remain_width < min_dist) 
							&& (pPerfectMatchComponent->m_strCustomerInfo == pCpn->m_strCustomerInfo))
						{
							min_remain_length = remaim_length;
							min_remain_width = remain_width;

							min_dist = min(min_remain_length, min_remain_width);

							pPerfectMatchComponent = pCpn;
							pPerfectMatchRemainder = pRmd;
							bRotateFlag = false;
							rotate_flag = bRotateFlag;

							// ��ȫƥ�䣬����
							if (min_remain_length == 0.0)
							{
								cut_dir = CutDir_Horizon;

								return true;
							}
							else if (min_remain_width == 0.0)
							{
								cut_dir = CutDir_Vertical;

								return true;
							}
							else
							{
								if (min_remain_width < min_remain_length)
								{
									cut_dir = CutDir_Vertical;
								}
								else
								{
									cut_dir = CutDir_Horizon;
								}
							}
						}
					}
				}

				if (pCpn->IsRotatable() == true)	// ƥ��ʱ��Ϊ���ټ�����������ʧ�棬��������ת��ֻ�Ƚ���ֵ������ʱ��������ת
				{
					Component tmp_cpn;
					tmp_cpn.m_RealLength = pCpn->m_RealWidth;
					tmp_cpn.m_RealWidth =  pCpn->m_RealLength;

					if (pRmd->Containable(&tmp_cpn) == true)
					{
						if(bMatchFlag == false)
						{
							//��һ��ѡ��
							min_remain_length	= pRmd->m_RealLength - pCpn->m_RealWidth;
							min_remain_width	= pRmd->m_RealWidth - pCpn->m_RealLength;


							min_dist = min(min_remain_length, min_remain_width);

							pPerfectMatchComponent = pCpn;
							pPerfectMatchRemainder = pRmd;
							bRotateFlag = true;
							bMatchFlag = true;
							rotate_flag = bRotateFlag;


							// ��ȫƥ�䣬����
							if (min_remain_length == 0.0)
							{
								cut_dir = CutDir_Horizon;

								return true;
							}
							else if (min_remain_width == 0.0)
							{
								cut_dir = CutDir_Vertical;

								return true;
							}
							else
							{
								if (min_remain_width < min_remain_length)
								{
									cut_dir = CutDir_Vertical;
								}
								else
								{
									cut_dir = CutDir_Horizon;
								}
							}
						}
						else
						{
							float remaim_length = pRmd->m_RealLength - pCpn->m_RealWidth;
							float remain_width	= pRmd->m_RealWidth - pCpn->m_RealLength;

							// ������ƥ��� ���Ҳ�������з�����Ϣ�ҵ�ǰ����޷�����Ϣ
							if (	(remaim_length < min_dist || remain_width < min_dist) 
								&& !(pPerfectMatchComponent->HaveDownerFaceInfo() ==true && pCpn->HaveDownerFaceInfo() == false))
							{
								min_remain_length = remaim_length;
								min_remain_width = remain_width;

								min_dist = min(min_remain_length, min_remain_width);

								pPerfectMatchComponent = pCpn;
								pPerfectMatchRemainder = pRmd;
								bRotateFlag = true;
								rotate_flag = bRotateFlag;

								// ��ȫƥ�䣬����
								if (min_remain_length == 0.0)
								{
									cut_dir = CutDir_Horizon;

									return true;
								}
								else if (min_remain_width == 0.0)
								{
									cut_dir = CutDir_Vertical;

									return true;
								}
								else
								{
									if (min_remain_width < min_remain_length)
									{
										cut_dir = CutDir_Vertical;
									}
									else
									{
										cut_dir = CutDir_Horizon;
									}
								}
							}
						}
					}
				}
			}

			//2018-10-27 yuanzb  ƥ�䵽��ֱ���˳�����ΪͼƬ���������ڹ涨�ߴ�������޳���Ҫ���մ������»��ߴ��������ţ�����ռ��˷Ѻܴ�
			// ������ϴ������»��ߴ������ţ����Խ�������������ϻ������£�ֻ�����ϲ�����ʱ��ѡ����һ������
			if (bMatchFlag == true)
			{
				return true;
			}

		}
	}
	else
	{

		for(i_remainder = 0; i_remainder < nRemainderNum; i_remainder++)
		{
			Component* pRmd = RemainderList.at(i_remainder);

			for (i_cpn = 0; i_cpn < nLeftCpnSize; i_cpn++)
			{
				Component* pCpn = ComponentList.at(i_cpn);

				if (pRmd->Containable(pCpn) == true)
				{
					if(bMatchFlag == false)
					{
						//��һ��ѡ��
						min_remain_length = pRmd->m_RealLength - pCpn->m_RealLength;
						min_remain_width = pRmd->m_RealWidth - pCpn->m_RealWidth;

						min_dist = min(min_remain_length, min_remain_width);

						pPerfectMatchComponent = pCpn;
						pPerfectMatchRemainder = pRmd;
						bRotateFlag = false;
						rotate_flag = bRotateFlag;
						bMatchFlag = true;


						// ��ȫƥ�䣬����
						if (min_remain_length == 0.0)
						{
							cut_dir = CutDir_Horizon;

							return true;
						}
						else if (min_remain_width == 0.0)
						{
							cut_dir = CutDir_Vertical;

							return true;
						}
						else
						{
							if (min_remain_width < min_remain_length)
							{
								cut_dir = CutDir_Vertical;
							}
							else
							{
								cut_dir = CutDir_Horizon;
							}
						}

					}
					else
					{
						float remaim_length = pRmd->m_RealLength - pCpn->m_RealLength;
						float remain_width = pRmd->m_RealWidth - pCpn->m_RealWidth;

						// ������ƥ���
						if (remaim_length < min_dist || remain_width < min_dist)
						{
							min_remain_length = remaim_length;
							min_remain_width = remain_width;

							min_dist = min(min_remain_length, min_remain_width);

							pPerfectMatchComponent = pCpn;
							pPerfectMatchRemainder = pRmd;
							bRotateFlag = false;
							rotate_flag = bRotateFlag;

							// ��ȫƥ�䣬����
							if (min_remain_length == 0.0)
							{
								cut_dir = CutDir_Horizon;

								return true;
							}
							else if (min_remain_width == 0.0)
							{
								cut_dir = CutDir_Vertical;

								return true;
							}
							else
							{
								if (min_remain_width < min_remain_length)
								{
									cut_dir = CutDir_Vertical;
								}
								else
								{
									cut_dir = CutDir_Horizon;
								}
							}
						}
					}
				}

				if (pCpn->IsRotatable() == true)	// ƥ��ʱ��Ϊ���ټ�����������ʧ�棬��������ת��ֻ�Ƚ���ֵ������ʱ��������ת
				{
					Component tmp_cpn;
					tmp_cpn.m_RealLength = pCpn->m_RealWidth;
					tmp_cpn.m_RealWidth =  pCpn->m_RealLength;

					if (pRmd->Containable(&tmp_cpn) == true)
					{
						if(bMatchFlag == false)
						{
							//��һ��ѡ��
							min_remain_length	= pRmd->m_RealLength - pCpn->m_RealWidth;
							min_remain_width	= pRmd->m_RealWidth - pCpn->m_RealLength;


							min_dist = min(min_remain_length, min_remain_width);

							pPerfectMatchComponent = pCpn;
							pPerfectMatchRemainder = pRmd;
							bRotateFlag = true;
							bMatchFlag = true;
							rotate_flag = bRotateFlag;


							// ��ȫƥ�䣬����
							if (min_remain_length == 0.0)
							{
								cut_dir = CutDir_Horizon;

								return true;
							}
							else if (min_remain_width == 0.0)
							{
								cut_dir = CutDir_Vertical;

								return true;
							}
							else
							{
								if (min_remain_width < min_remain_length)
								{
									cut_dir = CutDir_Vertical;
								}
								else
								{
									cut_dir = CutDir_Horizon;
								}
							}
						}
						else
						{
							float remaim_length = pRmd->m_RealLength - pCpn->m_RealWidth;
							float remain_width	= pRmd->m_RealWidth - pCpn->m_RealLength;

							// ������ƥ���
							if (remaim_length < min_dist || remain_width < min_dist)
							{
								min_remain_length = remaim_length;
								min_remain_width = remain_width;

								min_dist = min(min_remain_length, min_remain_width);

								pPerfectMatchComponent = pCpn;
								pPerfectMatchRemainder = pRmd;
								bRotateFlag = true;
								rotate_flag = bRotateFlag;

								// ��ȫƥ�䣬����
								if (min_remain_length == 0.0)
								{
									cut_dir = CutDir_Horizon;

									return true;
								}
								else if (min_remain_width == 0.0)
								{
									cut_dir = CutDir_Vertical;

									return true;
								}
								else
								{
									if (min_remain_width < min_remain_length)
									{
										cut_dir = CutDir_Vertical;
									}
									else
									{
										cut_dir = CutDir_Horizon;
									}
								}
							}
						}
					}
				}
			}

			//2018-10-27 yuanzb  ƥ�䵽��ֱ���˳�����ΪͼƬ���������ڹ涨�ߴ�������޳���Ҫ���մ������»��ߴ��������ţ�����ռ��˷Ѻܴ�
			// ������ϴ������»��ߴ������ţ����Խ�������������ϻ������£�ֻ�����ϲ�����ʱ��ѡ����һ������
			if (bMatchFlag == true)
			{
				return true;
			}

		}
	}


	return bMatchFlag;

}



 bool  ALGORITHM_API::ComponentCompareHigher(const Component* pfirst, const Component* psecond) 
{
	Component* p1 = const_cast<Component*>(pfirst);
	Component* p2 = const_cast<Component*>(psecond);

	if ((p1->m_y + p1->m_RealWidth) > (p2->m_y +p2->m_RealWidth))
	{
		return true;
	}


	return false;	
}

 bool ALGORITHM_API::ComponentCompareLower(const Component* pfirst, const Component* psecond) 
{
	Component* p1 = const_cast<Component*>(pfirst);
	Component* p2 = const_cast<Component*>(psecond);

	if (p1->m_y < p2->m_y)
	{
		return true;
	}


	return false;	
}



// �ж�˭�Ƚ�С
bool ALGORITHM_API::ComponentCompareSmaller(const Component* pfirst, const Component* psecond) 
{                 
	Component* p1 = const_cast<Component*>(pfirst);
	Component* p2 = const_cast<Component*>(psecond);


	return (*p1) < (*p2);	
}




/*--------------------------------------------------------------------------------------*/
//	purpose:
//		�Ӵ�С������򣬳������ȣ������֮
//	
//	param:
//		void
//
//	return:
//		0 -- �ɹ�
/*--------------------------------------------------------------------------------------*/
int ALGORITHM_API::SortRemainderList_LengthFirst(vector<Component*>& RemainderList)
{
	//  ����Ӵ�С����
	sort(RemainderList.begin(), RemainderList.end(), ComponentCompareSmaller);

	return 0;
}





 int  ALGORITHM_API::SortRemainderList_Top2Bottom(vector<Component*>& RemainderList)
 {
//  ����Ӹߵ�������
	 sort(RemainderList.begin(), RemainderList.end(), ComponentCompareHigher);

	 return 0;
 }

// �����Ͻ��дӵ͵�������
int ALGORITHM_API::SortRemainderList_Bottom2Top(vector<Component*>& RemainderList)
{
	//  ����ӵ͵�������
	sort(RemainderList.begin(), RemainderList.end(), ComponentCompareLower);

	return 0;
}










/*******************************************************************************************/

// ����С������
void ALGORITHM_API::CopyComponentList(vector<Component*>& SrcList, vector<Component*>& DstList)
{
	int i_cpn, nComponentCount;

	DstList.clear();
	nComponentCount = SrcList.size();
	for(i_cpn = 0; i_cpn < nComponentCount; i_cpn++)
	{
		Component* pCpn = SrcList.at(i_cpn);
		Component* pCopy = static_cast<Component*>(pCpn->CopyNode());


		DstList.push_back(pCopy);
	}
}

// �ж�˭�Ƚ�С
bool ComponentCompareArea(const Component* pfirst, const Component* psecond) 
{                 
	Component* p1 = const_cast<Component*>(pfirst);
	Component* p2 = const_cast<Component*>(psecond);


	return p1->IsAreaLargeThan(*p2);	
}


/*---------------------------------------*/
//	����˵����
//		��������ȱ������νṹ
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
/*---------------------------------------*/
void ALGORITHM_API::TravelNeedComponent_AreaLargeFirst(Component* pCpn, vector<Component*>& cpn_list)
{
	int i, nCount = pCpn->m_ChildrenList.size();

	// �ȸ����� ��Ҷ�Ӽӵ㿪ʼ����
	if (pCpn->m_type == NodeType_NeededComponent)
	{
		cpn_list.push_back(pCpn);
	}

	// �ռ��ӽڵ�
	vector<Component*> tmp_list;

	for(i = 0; i < nCount; i++)
	{
		Component* pTmpCpn = static_cast<Component*>(pCpn->m_ChildrenList.at(i));
		tmp_list.push_back(pTmpCpn);
	}

	// �����������
	sort(tmp_list.begin(), tmp_list.end(), ComponentCompareArea);


	for(i = 0; i < nCount; i++)
	{
		Component* pNode = tmp_list.at(i);
		TravelNeedComponent_AreaLargeFirst(pNode, cpn_list);
	}

	

	return ;
}

/*---------------------------------------------------*/
/*				��ѧ��ؼ���ӿ�					 */
/*---------------------------------------------------*/

/*---------------------------------------*/
//	����˵����
//		��ȡ��PointInfo��֮�����
//
//
//	������
//
//
//
//
//	����ֵ:
//		����
//
/*---------------------------------------*/
float ALGORITHM_API::GetPointsDistance(PointInfo p1, PointInfo p2)
{
	float dx, dy, dis_point;
	dx = p1.x - p2.x;
	dy = p1.y - p2.y;

	// ���������
	dis_point = sqrt(dx*dx + dy*dy);

	return dis_point;
}


#define  COINCIDENT_DIST		(0.2)		// �غϾ����ж�

// �����غϵĵ�
vector<PointInfo> ALGORITHM_API::FiltrateCoincidentPoints(vector<PointInfo>& oldpoint)
{
	// �����غϵ�
	int old_point_size = oldpoint.size();
	vector<PointInfo> filtrationpoint;

	for (int i = 0; i < old_point_size; i++)
	{
		// ѡ��
		PointInfo cur_point, next_point;
		cur_point = oldpoint.at(i);
		next_point = oldpoint.at((i + 1) % old_point_size);

		// ȡֵ
		float  dx, dy;
		dx = cur_point.x - next_point.x;
		dy = cur_point.y - next_point.y;

		// ���������
		float dis_point = sqrt(dx*dx + dy*dy);

		// ��ǰ������һ�����غϵ�
		if (dis_point < COINCIDENT_DIST)
		{
			//2016 yuanzb  ���һ�����һ���غϣ������һ�㲻��Բ���յ㣬ɾ��
			if (i == old_point_size-1)
			{
				if (cur_point.sign == 2 || cur_point.sign == 3)	// Բ���յ㣬���
				{
					filtrationpoint.push_back(cur_point);
				}

				continue;
			}

			// ��Ҫ�ϲ���
			PointInfo new_one;
			new_one.x = cur_point.x;
			new_one.y = cur_point.y;
			if (cur_point.sign  == 2 && next_point.sign ==1)			// Բ��β����Բ����㣬��Ϊ�м��
			{
				new_one = cur_point;
				new_one.sign = 3;	// Բ�����ӵ�
			}
			else if (cur_point.sign == 1 && next_point.sign == 2)	// Բ����β���غϣ�ɾ�����㣬����ֱ�ߵ�
			{
				new_one = cur_point;
				new_one.sign = 0;
				new_one.r = 0;
			}
			else if (cur_point.sign == 2 && next_point.sign == 0)	// Բ��β���ֱ�ߣ�ɾ��ֱ�ߵ�
			{
				new_one = cur_point;
			}
			else if (cur_point.sign == 0 && next_point.sign == 1)	// ֱ�߽�Բ�����
			{
				new_one = next_point;
			}
			else													// ֱ�߽�ֱ�ߣ�ֱ��ɾ�˵ڶ�����
			{
				new_one = cur_point;
			}

			// �ж��Ƿ����������ͬ����ĵ�
			int nNewCount = filtrationpoint.size();
			if (nNewCount > 0)
			{
				float d;
				PointInfo prev_new_one;

				prev_new_one = filtrationpoint.at(nNewCount-1);
				d = ALGORITHM_API::GetPointsDistance(prev_new_one, new_one);

				// �غ�
				if (d < COINCIDENT_DIST)
				{
					if (prev_new_one.sign == 0 && new_one.sign == 1)	// ֱ�ߺ�Բ������غ�
					{
						// ɾ�����һ����
						filtrationpoint.pop_back();
						filtrationpoint.push_back(new_one);
					}
				}
				else
				{
					// ���غϣ����
					filtrationpoint.push_back(new_one);
					i++;
				}

			}
			else
			{
				// ���غϣ����
				filtrationpoint.push_back(new_one);
				i++;
			}
		}
		else
		{
			// �ж��Ƿ����������ͬ����ĵ�
			int nNewCount = filtrationpoint.size();
			if (nNewCount > 0)
			{
				float d;
				PointInfo prev_new_one;

				prev_new_one = filtrationpoint.at(nNewCount-1);
				d = ALGORITHM_API::GetPointsDistance(prev_new_one, cur_point);

				// �غ�
				if (d < COINCIDENT_DIST)
				{
					if (prev_new_one.sign == 0 && cur_point.sign == 1)	// ֱ�ߺ�Բ������غ�
					{
						// ɾ�����һ����
						filtrationpoint.pop_back();
						filtrationpoint.push_back(cur_point);
					}
				}
				else
				{
					// ���غϣ����
					filtrationpoint.push_back(cur_point);
				}
			}
			else
			{
				filtrationpoint.push_back(cur_point);
			}
		}
	}

	// �����һ�����һ�����Ƿ�Ϊֱ�������һ�����غ�,���ε�����һ�����ڵ���3������ض��������ݴ���
	int nCount;
	float dist;
	PointInfo first_pnt, last_pnt;

	nCount = filtrationpoint.size();
	first_pnt = filtrationpoint.at(0);
	last_pnt = filtrationpoint.at(nCount-1);

	dist = ALGORITHM_API::GetPointsDistance(first_pnt, last_pnt);
	if (dist < COINCIDENT_DIST)
	{
		if (last_pnt.sign == 0)
		{
			filtrationpoint.pop_back();
		}
	}

	return filtrationpoint;
}


// �����غϵĵ�,��֤ͼ�αպ����һ������ֱ��ҲҪ��ӣ�
vector<PointInfo> ALGORITHM_API::FiltrateCoincidentPoints2(vector<PointInfo>& oldpoint)
{
	// �����غϵ�
	int old_point_size = oldpoint.size();
	vector<PointInfo> filtrationpoint;

	for (int i = 0; i < old_point_size; i++)
	{
		// ѡ��
		PointInfo cur_point, next_point;
		cur_point = oldpoint.at(i);
		next_point = oldpoint.at((i + 1) % old_point_size);

		// ȡֵ
		float dx, dy;
		dx = cur_point.x - next_point.x;
		dy = cur_point.y - next_point.y;

		// ���������
		float dis_point = sqrt(dx*dx + dy*dy);

		// ��ǰ������һ�����غϵ�
		if (dis_point < COINCIDENT_DIST)
		{
			//2016 yuanzb  ���һ��ֱ�����
			if (i == old_point_size-1)
			{
				// �ж��Ƿ����������ͬ����ĵ�
				int nNewCount = filtrationpoint.size();
				if (nNewCount > 0)
				{
					float d;
					PointInfo prev_new_one;

					prev_new_one = filtrationpoint.at(nNewCount-1);
					d = ALGORITHM_API::GetPointsDistance(prev_new_one, cur_point);

					// �غ�
					if (d < COINCIDENT_DIST)
					{
						// �����, ����
						continue;
					}
					else
					{
						// ���غϣ����

						// ���һ����ΪԲ������ǣ�ֱ�Ӹ�Ϊֱ�ߵ�
						if (cur_point.sign == 1)
						{
							cur_point.sign = 0;
						}


						filtrationpoint.push_back(cur_point);
						continue;
					}
				}
				else
				{
					filtrationpoint.push_back(cur_point);
					continue;
				}
			}

			// ��Ҫ�ϲ���
			PointInfo new_one;
  			new_one.x = cur_point.x;
			new_one.y = cur_point.y;
			if (cur_point.sign  == 2 && next_point.sign ==1)			// Բ��β����Բ����㣬��Ϊ�м��
			{
				new_one = cur_point;
				new_one.sign = 3;	// Բ�����ӵ�
			}
			else if (cur_point.sign == 1 && next_point.sign == 2)	// Բ����β���غϣ�ɾ�����㣬����ֱ�ߵ�
			{
				new_one = cur_point;
				new_one.sign = 0; 
				new_one.r = 0;
			}
			else if (cur_point.sign == 3 && next_point.sign == 2)	// Բ�����ӵ����յ��غϣ�����β��
			{
				new_one = next_point;

				// ����β��뾶Ϊ0������������ӵ�İ뾶Ϊ׼
				if (cur_point.r > next_point.r)
				{
					new_one.r = cur_point.r;
				}

			}
			else if (cur_point.sign == 2 && next_point.sign == 0)	// Բ��β���ֱ�ߣ�ɾ��ֱ�ߵ�
			{
				new_one = cur_point;
			}
			else if (cur_point.sign == 0 && next_point.sign == 1)	// ֱ�߽�Բ����� 
			{
				new_one = next_point;
			}
			else if (cur_point.sign == 1 && next_point.sign == 3)	// Բ����������ӵ��غϣ�ɾ�����ӵ�
			{
				new_one = cur_point;
			}
			else													// ֱ�߽�ֱ�ߣ�ֱ��ɾ�˵ڶ�����
			{
				new_one = cur_point;
			}



			// �ж��Ƿ����������ͬ����ĵ�
			int nNewCount = filtrationpoint.size();
			if (nNewCount > 0)
			{
				float d;
				PointInfo prev_new_one;

				prev_new_one = filtrationpoint.at(nNewCount-1);
				d = ALGORITHM_API::GetPointsDistance(prev_new_one, new_one);

				// �غ�
				if (d < COINCIDENT_DIST)
				{
					if (prev_new_one.sign == 0 && new_one.sign == 1)	// ֱ�ߺ�Բ������غ�
					{
						// ɾ�����һ����
						filtrationpoint.pop_back();
						filtrationpoint.push_back(new_one);
					}
				}
				else
				{
					// ���غϣ����
					filtrationpoint.push_back(new_one);
					//i++;
				}

			}
			else
			{
				// ���غϣ����
				filtrationpoint.push_back(new_one);
				//i++;
			}
		}
		else
		{
			// �ж��Ƿ����������ͬ����ĵ�
			int nNewCount = filtrationpoint.size();
			if (nNewCount > 0)
			{
				float d;
				PointInfo prev_new_one;

				prev_new_one = filtrationpoint.at(nNewCount-1);
				d = ALGORITHM_API::GetPointsDistance(prev_new_one, cur_point);

				// �غ�
				if (d < COINCIDENT_DIST)
				{
					// �����
				}
				else
				{
					// ���غϣ����
					filtrationpoint.push_back(cur_point);
				}
			}
			else
			{
				filtrationpoint.push_back(cur_point);
			}
		}
	}

	return filtrationpoint;
}

/*---------------------------------------*/
//	����˵����
//			��ת��
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
/*---------------------------------------*/
void ALGORITHM_API::rotatepoint(float x0,float y0,float & x,float & y,float arc)
{
	float mid_x = x - x0;
	float mid_y = y - y0;

	float rotate_x = mid_x*cos(arc) - mid_y*sin(arc);
	float rotate_y = mid_x*sin(arc) + mid_y*cos(arc);

	x = rotate_x + x0;
	y = rotate_y + y0; 
}




/*---------------------------------------*/
//	����˵����
//		��ȡԲ��
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
/*---------------------------------------*/
void ALGORITHM_API::getCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y)
{
	float len_beginToend = sqrt((beginx - endx)*(beginx - endx) + (beginy - endy)*(beginy - endy));
	float dis_x = (beginx - endx)/len_beginToend*radius;
	float dis_y = (beginy - endy)/len_beginToend*radius;

	float mid_x = endx + dis_x;
	float mid_y = endy + dis_y;

	float mid_x_move = mid_x - endx;
	float mid_y_move = mid_y - endy;

	float arc = (3.1415926-2*asin((0.5*len_beginToend)/radius))/2;

	center_x = mid_x_move*cos(arc) - mid_y_move*sin(arc);
	center_y = mid_x_move*sin(arc) + mid_y_move*cos(arc);

	center_x = center_x + endx;
	center_y = center_y + endy;
}

/*---------------------------------------*/
//	����˵����
//		����ȡԲ��,�Ľ���getCircleCenter�Ĵ���
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
/*---------------------------------------*/
void ALGORITHM_API::GetCircleCenter(float beginx,float endx,float beginy,float endy,float radius,int sign,float & center_x,float & center_y)
{
	float dx = beginx - endx;
	float dy = beginy - endy;
	float chord_len = sqrt(dx*dx + dy*dy);
	float sita = acos(0.5*chord_len/radius);	// �Ƕ�
	float new_x, new_y;

	// ˳ʱ����ת��ָ��Բ�ĵ������� ������180��ʱ��Ҫ��ʱ����ת
	if (sign == 1)
		ClockwiseRotate(dx, dy, new_x, new_y, sita);
	else
		CouterClockwiseRotate(dx, dy, new_x, new_y, sita);

	// �����Ĺ淶��
	float normal_new_x, normal_new_y;
	NormalVector(new_x, new_y, normal_new_x, normal_new_y);

	// β��ָ��Բ�ĵ�������׼ȷֵ
	float radius_x, radius_y;
	radius_x = normal_new_x * radius;
	radius_y = normal_new_y * radius;

	// β��+ָ��Բ�������� ���Բ�����ꡣ
	center_x = endx + radius_x;
	center_y = endy + radius_y;

}

/*---------------------------------------*/
//	����˵����
//		��ʱ����ת
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
/*---------------------------------------*/
void ALGORITHM_API::CouterClockwiseRotate(float x0,float y0,float & x,float & y,float arc)
{
	float mid_x = x0;
	float mid_y = y0;

	float rotate_x = mid_x*cos(arc) - mid_y*sin(arc);
	float rotate_y = mid_x*sin(arc) + mid_y*cos(arc);

	x = rotate_x ;
	y = rotate_y ; 
}

/*---------------------------------------*/
//	����˵����
//		˳ʱ����ת
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
/*---------------------------------------*/
void ALGORITHM_API::ClockwiseRotate(float x0,float y0,float & x,float & y,float arc)
{
	float mid_x = x0;
	float mid_y = y0;

	float rotate_x = mid_x*cos(arc) + mid_y*sin(arc);
	float rotate_y = -mid_x*sin(arc) + mid_y*cos(arc);

	x = rotate_x ;
	y = rotate_y ; 
}

/*---------------------------------------*/
//	����˵����
//		�����淶��
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
/*---------------------------------------*/
void ALGORITHM_API::NormalVector(float x0,float y0,float & x,float & y)
{
	float len = sqrt(x0*x0 + y0*y0);

	x = x0/len;
	y = y0/len;

}
