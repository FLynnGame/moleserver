#include "../../include/MahJong/MahJongLogic.h"

void CMahJongLogic::Release()
{
	if (m_PMJAlgorithm)
	{
		delete m_PMJAlgorithm;
		m_PMJAlgorithm = 0;
	}
}
void CMahJongLogic::SetMahJongAlg(CMahJongAlg *PMJAlgorithm)
{
	if (m_PMJAlgorithm)
	{
		delete m_PMJAlgorithm;
		m_PMJAlgorithm = 0;
	}

	m_PMJAlgorithm = PMJAlgorithm;
}

CMahJongAlg * CMahJongLogic::GetMahJongAlg()
{
	return m_PMJAlgorithm;
}

//���Ʋ���
bool CMahJongLogic::OutMJ(MJBYTE bSelfHandMJ[], int &iSelfHandMJCount, MJBYTE bSelfOutMJ[], int &iSelfOutMJCount, MJBYTE bOutMJ)
{
	if (GetRequiredCheck())
	{
		if (iSelfHandMJCount <= 0)
		{
			return false;
		}
	}
	if (DeleteMJ(bSelfHandMJ, iSelfHandMJCount, bOutMJ))
	{
		bSelfOutMJ[iSelfOutMJCount++] = bOutMJ;
		//SortHandMJ(bSelfHandMJ, bSelfHandMJCount);
		return true;
	}
	return false;
}


//�������Ʋ���
bool CMahJongLogic::UnDoOutMJ(MJBYTE bSelfHandMJ[], int &iSelfHandMJCount, MJBYTE bSelfOutMJ[], int &iSelfOutMJCount, MJBYTE bOutMJ)
{
	//�ж���ǰ����Ӧ��3N+1���ҳ��������һ����Ϊָ����
	if(bSelfOutMJ[iSelfOutMJCount-1] == bOutMJ && iSelfHandMJCount %3 ==1)
	{
		iSelfOutMJCount--;
		bSelfHandMJ[iSelfHandMJCount++] = bOutMJ;
		return true;
	}
	else
	{
		return false;
	}
}

//�Բ���
bool CMahJongLogic::Chi(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bChi, enOperationChiType bChiType)
{
	//�ȼ������ǲ��ǺϷ�����
	MJBYTE bMjColor = m_PMJAlgorithm->GetMahJongColor();
	if (bMjColor & MahJongColor_Word)
	{
		if (MASK_MAHJONGKIND(bChi) >= MahJong_Word)
		{
			//��������ƻ���,�����
			return false;
		}
	}
	if (GetRequiredCheck())
	{
		if ((bChiType == Operation_Chi_Type_Left && MASK_MAHJONGNUM(bChi) > 7) || (bChiType == Operation_Chi_Type_Midd && MASK_MAHJONGNUM(bChi) > 8))
		{
			//*00�Է�,�Ե��Ǹ�����7����0*0�Է�,�Ե��Ǹ�����8����
			return false;
		}
		//����ǲ��ǶԵġ�����߳�7W����Ӧ�ô���89W
		switch(bChiType)
		{
			case Operation_Chi_Type_Left:
				{
					if (!CheckMJExist(bSelfHandMJ, iSelfHandCount, bChi+1) || !CheckMJExist(bSelfHandMJ, iSelfHandCount, bChi+2))
					{
						return false;
					}
				}
				break;
			case Operation_Chi_Type_Midd:
				{
					if (!CheckMJExist(bSelfHandMJ, iSelfHandCount, bChi-1) || !CheckMJExist(bSelfHandMJ, iSelfHandCount, bChi+1))
					{
						return false;
					}
				}
				break;
			case Operation_Chi_Type_Right:
				{
					if (!CheckMJExist(bSelfHandMJ, iSelfHandCount, bChi-2) || !CheckMJExist(bSelfHandMJ, iSelfHandCount, bChi-1))
					{
						return false;
					}
				}
				break;
		}
	}

	iLastOutMJCount--;
	switch(bChiType)
	{
	case Operation_Chi_Type_Left:
		{
			//��������ɾ�� bChi+1 bChi+2��һ����
			DeleteMJ(bSelfHandMJ, iSelfHandCount, bChi+1);
			DeleteMJ(bSelfHandMJ, iSelfHandCount, bChi+2);
			bSelfHuMJ[iSelfHuCount].first = bChi;
			bSelfHuMJ[iSelfHuCount].style = BlockStyle_Chi;
			iSelfHuCount++;
		}
		break;
	case Operation_Chi_Type_Midd:
		{
			//��������ɾ�� bChi-1 bChi+1��һ����
			DeleteMJ(bSelfHandMJ, iSelfHandCount, bChi-1);
			DeleteMJ(bSelfHandMJ, iSelfHandCount, bChi+1);
			bSelfHuMJ[iSelfHuCount].first = bChi-1;
			bSelfHuMJ[iSelfHuCount].style = BlockStyle_Chi;
			iSelfHuCount++;
		}
		break;
	case Operation_Chi_Type_Right:
		{
			//��������ɾ�� bChi-2 bChi-1��һ����
			DeleteMJ(bSelfHandMJ, iSelfHandCount, bChi-2);
			DeleteMJ(bSelfHandMJ, iSelfHandCount, bChi-1);
			bSelfHuMJ[iSelfHuCount].first = bChi-2;
			bSelfHuMJ[iSelfHuCount].style = BlockStyle_Chi;
			iSelfHuCount++;
		}
		break;
	}
	return true;
}

//�����Բ���
bool CMahJongLogic::UnDoChi(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bChi, enOperationChiType bChiType)
{
	//�����������һ��Ϊָ������
	if (bSelfHuMJ[iSelfHuCount-1].style == BlockStyle_Chi && iSelfHandCount % 3 == 2)
	{
		switch(bChiType)
		{
		case Operation_Chi_Type_Left:
			{
				//*00
				if (bSelfHuMJ[iSelfHuCount-1].first != bChi)
				{
					return false;
				}
			}
			break;
		case Operation_Chi_Type_Midd:
			{
				//0*0
				if (bSelfHuMJ[iSelfHuCount-1].first != bChi-1)
				{
					return false;
				}
			}
			break;
		case Operation_Chi_Type_Right:
			{
				//00*
				if (bSelfHuMJ[iSelfHuCount-1].first != bChi-2)
				{
					return false;
				}
			}
			break;
		}
	}
	else
	{
		return false;
	}

	bLastOutMJ[iLastOutMJCount++] = bChi;	//���سԵ��ĳ���
	iSelfHuCount--;	//ɾ���Եĺ��ƽṹ

	//���س��ƺ�ȥ�������Ʋ���
	switch(bChiType)
	{
	case Operation_Chi_Type_Left:
		{
			//�������в��� bChi+1 bChi+2��һ����
			bSelfHandMJ[iSelfHandCount++] = bChi+1;
			bSelfHandMJ[iSelfHandCount++] = bChi+2;
		}
		break;
	case Operation_Chi_Type_Midd:
		{
			//�������в��� bChi-1 bChi+1��һ����
			bSelfHandMJ[iSelfHandCount++] = bChi-1;
			bSelfHandMJ[iSelfHandCount++] = bChi+1;
		}
		break;
	case Operation_Chi_Type_Right:
		{
			//�������в��� bChi-2 bChi-1��һ����
			bSelfHandMJ[iSelfHandCount++] = bChi-2;
			bSelfHandMJ[iSelfHandCount++] = bChi-1;
		}
		break;
	}
	//����������
	m_PMJAlgorithm->SortHandMJ(bSelfHandMJ, iSelfHandCount);
	return true;
}

//������
bool CMahJongLogic::Peng(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bPeng)
{
	//�������ǲ��ǺϷ���
	if (GetRequiredCheck())
	{
		if (GetMJCount(bSelfHandMJ, iSelfHandCount, bPeng) < 2)
		{
			return false;
		}
	}

	iLastOutMJCount--;
	bSelfHuMJ[iSelfHuCount].first = bPeng;
	bSelfHuMJ[iSelfHuCount].style = BlockStyle_Peng;
	iSelfHuCount++;
	DeleteMJ(bSelfHandMJ, iSelfHandCount, bPeng);
	DeleteMJ(bSelfHandMJ, iSelfHandCount, bPeng);//ɾ2��

	return true;
}

//����������
bool CMahJongLogic::UnDoPeng(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bPeng)
{
	//���������һ��Ӧ����������
	if (bSelfHuMJ[iSelfHuCount-1].style == BlockStyle_Peng && bSelfHuMJ[iSelfHuCount-1].first == bPeng && iSelfHandCount % 3 == 2)
	{
		iSelfHuCount--;	//ɾ��������
		bSelfHandMJ[iSelfHandCount++] = bPeng;
		bSelfHandMJ[iSelfHandCount++] = bPeng;
		bLastOutMJ[iLastOutMJCount++] = bPeng;
		m_PMJAlgorithm->SortHandMJ(bSelfHandMJ, iSelfHandCount);
		return true;
	}
	else
	{
		return false;
	}
}

//�ܲ���
bool CMahJongLogic::Gang(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bGang, enOperationGangType bGangType)
{
	//�������ǲ��ǺϷ���
	if (GetRequiredCheck())
	{
		switch(bGangType)
		{
		case Operation_Gang_Type_Ming:
			{
				//�Լ�������3��һ����
				int iCount = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang);
				if (iCount < 3 && iLastOutMJCount > 0)
				{
					return false;
				}
			}
			break;
		case Operation_Gang_Type_An:
			{
				//�Լ�������4��һ����
				int iCount = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang);
				if (iCount < 4)
				{
					return false;
				}
			}
			break;
		case Operation_Gang_Type_ZiMoMing:
			{
				//��������һ����,�������һ����ָ����
				if (bSelfHandMJ[iSelfHandCount-1] != bGang)
				{
					return false;
				}
				bool bExistFlag = false;
				for (int i=0;i<iSelfHuCount;i++)
				{
					if (bSelfHuMJ[i].style == BlockStyle_Peng && bSelfHuMJ[i].first == bGang)
					{
						bExistFlag = true;
						break;
					}
				}
				if (!bExistFlag)
				{
					return false;
				}
			}
			break;
		case Operation_Gang_Type_XF:
			{
				if (bGang == 0x31)
				{
					int bGang01 = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang);
					int bGang02 = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang+0x01);
					int bGang03 = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang+0x02);
					int bGang04 = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang+0x03);
					if(!(bGang01 && bGang02 && bGang03 && bGang04))
					{
						return false;
					}
				}
				if (bGang == 0x35)
				{
					int bGang05 = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang);
					int bGang06 = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang+0x01);
					int bGang07 = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang+0x02);
					if(!(bGang05 && bGang06 &&bGang07))
					{
						return false;
					}
				}
			}
			break;
		default:
			{
				return false;
			}
		}
	}

	//��֤�ɹ�,����
	switch(bGangType)
	{
	case Operation_Gang_Type_Ming:
		{
			//ȥ��3��һ�������ƺͳ�����ҵ����һ�ų���,������ܵ���������
			m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);
			m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);
			m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);
			iLastOutMJCount--;

			bSelfHuMJ[iSelfHuCount].first = bGang;
			bSelfHuMJ[iSelfHuCount].style = BlockStyle_Gang_Ming;
			iSelfHuCount++;
		}
		break;
	case Operation_Gang_Type_An:
		{
			//ȥ��4��һ��������
			m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);
			m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);
			m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);
			m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);

			bSelfHuMJ[iSelfHuCount].first = bGang;
			bSelfHuMJ[iSelfHuCount].style = BlockStyle_Gang_An;
			iSelfHuCount++;
		}
		break;
	case Operation_Gang_Type_ZiMoMing:
		{
			//�����Ʊ����,ɾ��ָ������1��
			m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);

			for (int i=0;i<iSelfHuCount;i++)
			{
				if (bSelfHuMJ[i].style == BlockStyle_Peng && bSelfHuMJ[i].first == bGang)
				{
					bSelfHuMJ[i].style = BlockStyle_Gang_ZiMoMing;
					break;
				}
			}
		}
		break;
	case Operation_Gang_Type_XF:
		{
			//ȥ��4�Ŷ�������������
			if (bGang == 0x31)
			{
				m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);
				m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang+0x01);
				m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang+0x02);
				m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang+0x03);

				bSelfHuMJ[iSelfHuCount].first = bGang;
				bSelfHuMJ[iSelfHuCount].style = BlockStyle_Gang_XF;
				iSelfHuCount++;
			}
			else if (bGang == 0x35)
			{
				m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);
				m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang+0x01);
				m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang+0x02);

				bSelfHuMJ[iSelfHuCount].first = bGang;
				bSelfHuMJ[iSelfHuCount].style = BlockStyle_Gang_XF;
				iSelfHuCount++;
			}
		}
		break;
	}
	return true;
}


//�����ܲ���
bool CMahJongLogic::UnDoGang(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bGang, enOperationGangType bGangType)
{
	if (bSelfHuMJ[iSelfHuCount-1].first != bGang || iSelfHandCount % 3 != 1 || iSelfHuCount <= 0)
	{
		return false;
	}
	if (bGangType == Operation_Gang_Type_An && bSelfHuMJ[iSelfHuCount-1].style != BlockStyle_Gang_An)
	{
		return false;
	}
	else if (bGangType == Operation_Gang_Type_Ming && bSelfHuMJ[iSelfHuCount-1].style != BlockStyle_Gang_Ming)
	{
		return false;
	}
	else if (bGangType == Operation_Gang_Type_ZiMoMing && bSelfHuMJ[iSelfHuCount-1].style != BlockStyle_Gang_ZiMoMing)
	{
		return false;
	}

	switch(bGangType)
	{
	case Operation_Gang_Type_Ming:
		{
			//����3��һ�������ƺͳ�����ҵ����һ�ų���,ɾ������������
			bSelfHandMJ[iSelfHandCount++] = bGang;
			bSelfHandMJ[iSelfHandCount++] = bGang;
			bSelfHandMJ[iSelfHandCount++] = bGang;
			bLastOutMJ[iLastOutMJCount++] = bGang;
			iSelfHuCount--;
		}
		break;
	case Operation_Gang_Type_An:
		{
			//ȥ��4��һ��������
			bSelfHandMJ[iSelfHandCount++] = bGang;
			bSelfHandMJ[iSelfHandCount++] = bGang;
			bSelfHandMJ[iSelfHandCount++] = bGang;
			bSelfHandMJ[iSelfHandCount++] = bGang;
			iSelfHuCount--;
		}
		break;
	case Operation_Gang_Type_ZiMoMing:
		{
			//�����Ʊ����,ɾ��ָ������1��
			bSelfHandMJ[iSelfHandCount++] = bGang;
			for (int i=0;i<iSelfHuCount;i++)
			{
				if (bSelfHuMJ[i].style == BlockStyle_Gang_ZiMoMing && bSelfHuMJ[i].first == bGang)
				{
					bSelfHuMJ[i].style = BlockStyle_Peng;
					break;
				}
			}
		}
		break;
	}
	m_PMJAlgorithm->SortHandMJ(bSelfHandMJ, iSelfHandCount);
	return true;
}

//�ܲ���(�ܱ��˵���)
//bool CMahJongLogic::NormalGang(MJBlock bSelfHuMJ[], MJBYTE &bSelfHuCount, MJBYTE bSelfHandMJ[], MJBYTE &bSelfHandCount, MJBYTE bLastOutMJ[], MJBYTE &bLastOutMJCount, MJBYTE bGang)
//{
//	//���Ϸ���,Ҫ��3����ͬ���Ʋ��ܸܱ��˳�����
//	if (GetRequiredCheck())
//	{
//		if (GetMJCount(bSelfHandMJ, bSelfHandCount, bGang) < 3)
//		{
//			return false;
//		}
//	}
//
//	bLastOutMJCount--;
//
//	//ɾ������3�Σ���Ӻ�����
//	for (int i = 3;i > 0;i--)
//	{
//		DeleteMJ(bSelfHandMJ, bSelfHandCount, bGang);
//	}
//	bSelfHuMJ[bSelfHuCount].first = bGang;
//	bSelfHuMJ[bSelfHuCount].style = BlockStyle_Gang;
//	bSelfHuCount++;
//	return true;
//}

//�ܲ���(���Լ�������)
//bool CMahJongLogic::ZMGang(MJBlock bSelfHuMJ[], MJBYTE &bSelfHuCount, MJBYTE bSelfHandMJ[], MJBYTE &bSelfHandCount, MJBYTE bTouch)
//{
//	//�ж��Ƿ���������
//	bool isMing = false;
//	for (int i = 0;i<bSelfHuCount;i++)
//	{
//		if (bSelfHuMJ[i].style == BlockStyle_Peng && bSelfHuMJ[i].first == bTouch)
//		{
//			isMing = true;
//			break;
//		}
//	}
//
//	bool isAnGang = true;
//
//	if(GetRequiredCheck())
//	{
//		isAnGang = GetMJCount(bSelfHandMJ, bSelfHandCount, bTouch) == 4;
//	}
//
//	if (isMing)
//	{
//		//ɾ������1��
//		DeleteMJ(bSelfHandMJ, bSelfHandCount, bTouch);
//		for (int i = 0;i<bSelfHuCount;i++)
//		{
//			if (bSelfHuMJ[i].style == BlockStyle_Peng && bSelfHuMJ[i].first == bTouch)
//			{
//				bSelfHuMJ[i].style = BlockStyle_Gang;
//				break;
//			}
//		}
//	}
//	else if (isAnGang)
//	{
//		//ɾ������4�Σ���Ӱ��ܵ�������
//		DeleteMJ(bSelfHandMJ, bSelfHandCount, bTouch);
//		DeleteMJ(bSelfHandMJ, bSelfHandCount, bTouch);
//		DeleteMJ(bSelfHandMJ, bSelfHandCount, bTouch);
//		DeleteMJ(bSelfHandMJ, bSelfHandCount, bTouch);
//
//		bSelfHuMJ[bSelfHuCount].first = bTouch;
//		bSelfHuMJ[bSelfHuCount].style = BlockStyle_An_Gang;
//		bSelfHuCount++;
//	}
//	else
//	{
//		return false;
//	}
//
//	return true;
//}

int CMahJongLogic::GetRandomNum(int iMin, int iMax)
{
	return m_PMJAlgorithm->GetRandomNum(iMin, iMax);
}

//��������
void CMahJongLogic::RandomMJ(MJBYTE bAllMJ[], int iAllMJCount)
{
	m_PMJAlgorithm->RandomMJ(bAllMJ, iAllMJCount);
}

//���������С��������
void CMahJongLogic::SortHandMJ(MJBYTE bAllMJ[], int iAllMJCount)
{
	m_PMJAlgorithm->SortHandMJ(bAllMJ, iAllMJCount);
}

//У�����Ƿ����
bool CMahJongLogic::CheckMJExist(const MJBYTE bAllMJ[], int iAllMJCount, MJBYTE bMJ)
{
	return m_PMJAlgorithm->CheckMJExist(bAllMJ, iAllMJCount, bMJ);
}

//ɾ���齫
bool CMahJongLogic::DeleteMJ(MJBYTE bAllMJ[], int &iAllMJCount, MJBYTE bMJ)
{
	return m_PMJAlgorithm->DeleteMJ(bAllMJ, iAllMJCount, bMJ);
}
//���ָ���齫������
int CMahJongLogic::GetMJCount(MJBYTE bAllMJ[], int &iAllMJCount, MJBYTE bMJ)
{
	return m_PMJAlgorithm->GetMJCount(bAllMJ, iAllMJCount, bMJ);
}
//////////////////////////////////////////////////////////////////////////

//�ж��ܷ��:�ϼҴ���Ʋ��ܳ�
bool CMahJongLogic::IsChi(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJOut, MJBYTE &bChiType)
{
	MJFLAG bMjColor = m_PMJAlgorithm->GetMahJongColor();
	if (bMjColor & MahJongColor_Word)
	{
		if (MASK_MAHJONGKIND(bMJOut) >= MahJong_Word)
		{
			//��������ƻ���,�����
			return false;
		}
	}
	bChiType = 0;
	//bool bPosLeft = true, bPosMidd = true, bPosRight = true;
	//for (int i = 0 ;i < bSelfMJCount - 1; i++)
	//{
	//	//@**
	//	if( bSelfMJ[i] == bMJOut+1 && bPosLeft )
	//	{
	//		for( int j=i; j<bSelfMJCount; j++ )
	//		{
	//			if( bSelfMJ[j] == bMJOut+2 )
	//			{
	//				bChiType = bChiType | Operation_Chi_Type_Left;
	//				break;
	//			}
	//		}
	//		bPosLeft = false;
	//	}
	//	//*@*
	//	else if( bSelfMJ[i] == bMJOut-1 && bPosMidd )
	//	{
	//		for( int j=i; j<bSelfMJCount; j++ )
	//		{
	//			if( bSelfMJ[j] == bMJOut+1 )
	//			{
	//				bChiType = bChiType | Operation_Chi_Type_Midd;
	//				break;
	//			}
	//		}
	//		bPosMidd = false;
	//	}
	//	//**@
	//	else if( bSelfMJ[i] == bMJOut-2 && bPosRight )
	//	{
	//		for( int j=i; j<bSelfMJCount; j++ )
	//		{
	//			if( bSelfMJ[j] == bMJOut-1 )
	//			{
	//				bChiType = bChiType | Operation_Chi_Type_Right;
	//				break;
	//			}
	//		}
	//		bPosRight = false;
	//	}
	//}
	int iLLMJCount = MASK_MAHJONGNUM(bMJOut) > 2 ? GetMJCount(bSelfMJ, iSelfMJCount, bMJOut-2) : 0;
	int iLMJCount = MASK_MAHJONGNUM(bMJOut) > 1 ? GetMJCount(bSelfMJ, iSelfMJCount, bMJOut-1) : 0;
	int iRMJCount = MASK_MAHJONGNUM(bMJOut) < 9 ? GetMJCount(bSelfMJ, iSelfMJCount, bMJOut+1) : 0;
	int iRRMJCount = MASK_MAHJONGNUM(bMJOut) < 8 ? GetMJCount(bSelfMJ, iSelfMJCount, bMJOut+2) : 0;
	if (iLLMJCount > 0 && iLMJCount > 0)
	{
		bChiType = bChiType | Operation_Chi_Type_Right;
	}
	if (iLMJCount > 0 && iRMJCount > 0)
	{
		bChiType = bChiType | Operation_Chi_Type_Midd;
	}
	if (iRMJCount > 0 && iRRMJCount > 0)
	{
		bChiType = bChiType | Operation_Chi_Type_Left;
	}
	return bChiType > 0;
}

//�ж��ܷ���:�����������������
bool CMahJongLogic::IsPeng(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJOut)
{
	return m_PMJAlgorithm->GetMJCount(bSelfMJ, iSelfMJCount, bMJOut) >= 2;
}

//�ж��ܷ�����:���˳���
bool CMahJongLogic::IsOutMingGang(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJOut)
{
	return m_PMJAlgorithm->GetMJCount(bSelfMJ, iSelfMJCount, bMJOut) >= 3;
}

//�ж��Ƿ���������
bool CMahJongLogic::IsSelfMingGang(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bMJTouch)
{
	for(int i = 0; i < iSelfHUMJCount; i++)
	{
		if (bSelfHUMJ[i].style == BlockStyle_Peng && bSelfHUMJ[i].first == bMJTouch)
		{
			return true;
		}
	}
	return false;
}

//�ж��ܷ񰵸�:�Լ������Ʋ��ܰ���,���ɸܵ��Ʒ���bGang
bool CMahJongLogic::IsAnGang(MJBlock bSelfHUMJ[], int bSelfHUMJCount, MJBYTE bSelfMJ[], int bSelfMJCount, MJBYTE bGang[], int &bGangCount)
{
	bGangCount = 0;

	int iCount[MAX_ARRAY_MJ] = {0};//ÿһ���Ƶ�����
	for (int i=0; i < bSelfMJCount; i++)
	{
		iCount[MASK_MAHJONGKIND(bSelfMJ[i]) * 10 + MASK_MAHJONGNUM(bSelfMJ[i])]++;
	}
	//��4���ľ��ǰ���
	for (int i=0; i < MAX_ARRAY_MJ; i++)
	{
		if (iCount[i] >= 4)
		{
			bGang[bGangCount++] = MASK_MAHJONG(i/10, i%10);
		}
	}
	return bGangCount > 0 ? true:false;
}

//�ж��ܷ������:�Լ������Ʋ��������,���ɸܵ��Ʒ���bGang
bool CMahJongLogic::IsXFGang(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJTouch, MJBYTE bGang[], int &iGangCount)
{
	iGangCount = 0;

	int iCount[MAX_ARRAY_MJ] = {0};//ÿһ���Ƶ�����
	iCount[MASK_MAHJONGKIND(bMJTouch) * 10 + MASK_MAHJONGNUM(bMJTouch)]++;
	for (int i=0; i < iSelfMJCount; i++)
	{
		iCount[MASK_MAHJONGKIND(bSelfMJ[i]) * 10 + MASK_MAHJONGNUM(bSelfMJ[i])]++;
	}
	//������ĸ������
	for (int i=0; i < 4; i++)
	{
		//31,32,33,34����������  35,36,37�����з���
		if (iCount[31] >= 1 && iCount[32]>=1 && iCount[33]>=1 && iCount[34]>=1)
		{
			iCount[31]--;iCount[32]--;
			iCount[33]--;iCount[34]--;
			bGang[iGangCount++] = 0x31;
			continue;
		}
		else if (iCount[35]>=1 && iCount[36]>=1 && iCount[37]>=1)
		{
			iCount[35]--;iCount[36]--;iCount[37]--;
			bGang[iGangCount++] = 0x35;
			continue;
		}
		break;
	}
	return iGangCount > 0 ? true:false;

}
//�ж��ܷ񰵸�:�Լ������Ʋ��ܰ���,���ɸܵ��Ʒ���bGang
bool CMahJongLogic::IsAnGang(MJBlock bSelfHUMJ[], int bSelfHUMJCount, MJBYTE bSelfMJ[], int bSelfMJCount, MJBYTE bMJTouch, MJBYTE bGang[], int &bGangCount)
{
	bGangCount = 0;

	int iCount[MAX_ARRAY_MJ] = {0};//ÿһ���Ƶ�����
	iCount[MASK_MAHJONGKIND(bMJTouch) * 10 + MASK_MAHJONGNUM(bMJTouch)]++;
	for (int i=0; i < bSelfMJCount; i++)
	{
		iCount[MASK_MAHJONGKIND(bSelfMJ[i]) * 10 + MASK_MAHJONGNUM(bSelfMJ[i])]++;
	}
	//��4���ľ��ǰ���
	for (int i=0; i < MAX_ARRAY_MJ; i++)
	{
		if (iCount[i] >= 4)
		{
			bGang[bGangCount++] = MASK_MAHJONG(i/10, i%10);
		}
	}
	return bGangCount > 0 ? true:false;
}

//�ж��ܷ����
bool CMahJongLogic::IsHu(MJBlock bSelfHUMJ[], int bSelfHUMJCount, MJBYTE bSelfHandMJ[], int bSelfHandMJCount, MJBYTE bMJ)
{
	MJBYTE bHandMJ[MAX_HAND_MJ] = {0};
	memcpy(bHandMJ, bSelfHandMJ, sizeof(bHandMJ));
	MJBYTE bHandMJCount = bSelfHandMJCount;
	bHandMJ[bHandMJCount++] = bMJ;

	SortHandMJ(bHandMJ, bHandMJCount);

	//�ж��Ƿ����
	if (m_PMJAlgorithm->IsNormalHu(bSelfHUMJ, bSelfHUMJCount, bHandMJ, bHandMJCount))
	{
		return true;
	}

	return false;
}
