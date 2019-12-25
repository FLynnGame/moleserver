#include "../../include/MahJong/MahJongAlg.h"

//���������С��������
void CMahJongAlg::SortHandMJ(MJBYTE bAllMJ[], int iAllMJCount)
{
	MJBYTE bTmp = -1;
	for (int i=0;i<iAllMJCount;i++)
	{
		for (int j=i+1;j<iAllMJCount;j++)
		{
			if (bAllMJ[i] > bAllMJ[j])
			{
				bTmp = bAllMJ[i];
				bAllMJ[i] = bAllMJ[j];
				bAllMJ[j] = bTmp;
			}
		}
	}
}

//��������
void CMahJongAlg::RandomMJ(MJBYTE bAllMJ[], int iAllMJCount)
{
	int index=0 ,iTmp=0;
	int iRandCount = rand() % 6 + 5;
	for (int i = 0;i < iRandCount; i++)
	{
		for( int j=0; j<iAllMJCount; j++ )
		{
			index = GetRandomNum(0, iAllMJCount-1);
			if( j!=index )
			{
				iTmp = bAllMJ[j];
				bAllMJ[j] = bAllMJ[index];
				bAllMJ[index] = iTmp;
			}
		}
	}
}

//У�����Ƿ����
bool CMahJongAlg::CheckMJExist(const MJBYTE bAllMJ[], int iAllMJCount, MJBYTE bMJ)
{
	for (int i=0;i<iAllMJCount;i++)
	{
		if (bAllMJ[i] == bMJ)
		{
			return true;
		}
	}
	return false;
}

//ɾ���齫
bool CMahJongAlg::DeleteMJ(MJBYTE bAllMJ[], int &iAllMJCount, MJBYTE bMJ)
{
	if (iAllMJCount <= 0)
	{
		return false;
	}
	if (bAllMJ[0] == CONST_MJ_HIDE)
	{
		for (int i=0;i<iAllMJCount;i++)
		{
			if (bAllMJ[i] == CONST_MJ_HIDE)
			{
				for(int j=i;j<iAllMJCount-1;j++)
				{
					bAllMJ[j] = bAllMJ[j+1];
				}
				iAllMJCount -= 1;
				return true;
			}
		}
		//iAllMJCount --;
		return true;
	}
	else
	{
		for (int i=0;i<iAllMJCount;i++)
		{
			if (bAllMJ[i] == bMJ)
			{
				for(int j=i;j<iAllMJCount-1;j++)
				{
					bAllMJ[j] = bAllMJ[j+1];
				}
				iAllMJCount -= 1;
				return true;
			}
		}
	}
	return false;
}

//���ָ���齫������
int CMahJongAlg::GetMJCount(MJBYTE bMJ[], int &iMJCount, MJBYTE bM)
{
	int iCount = 0;
	for (int i=0; i<iMJCount;i++)
	{
		if (bMJ[i] == bM)
		{
			iCount++;
		}
	}
	return iCount;
}

//////////////////////////////////////////////////////////////////////////
bool CMahJongAlg::IsNormalHu(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfHandMJ[], int iSelfHandMJCount)
{
	return CheckRestHu(bSelfHandMJ, iSelfHandMJCount);
}

//���ʣ������Ƿ��ܺ����������ʾ
bool CMahJongAlg::CheckRestHu(MJBYTE bHandMJ[], int iHandMJCount)
{
	MJBYTE bCheckMJ[MAX_ARRAY_MJ] = {0};
	for (int i=0; i<iHandMJCount; i++)
	{
		if (m_bMahJongColor & MahJong_Flower)
		{
			if (MASK_MAHJONGKIND(bHandMJ[i]) == MahJong_Flower)
			{
				return false;
			}
		}

		bCheckMJ[MASK_MAHJONGKIND(bHandMJ[i])*10 + MASK_MAHJONGNUM(bHandMJ[i])]++;
	}

	return CheckIsHu(bCheckMJ, MAX_ARRAY_MJ);
}

//������Ƿ��ܺ��������Ƶĸ���������38��С��������
bool CMahJongAlg::CheckIsHu(MJBYTE bMJ[], int iMaxCount)
{
	//static int JIANG = 0;						// ���Ʊ�־,�����͡��������������еġ����� ,�ŵ���0��λ��
	if( !Remain(bMJ, iMaxCount) ) return 1;     // �ݹ��˳����������û��ʣ��,����Ʒ��ء�
	int i=0;
	for(i=1; !bMJ[i] && i<iMaxCount; i++);  // �ҵ����Ƶĵط�,i���ǵ�ǰ��, PAI[i]�Ǹ���
	//printf("i = %d\n",i);						// ������Ϣ

	// 4�����(����) �����ﲻ���ڸ�
	//if ( bMJ[i] == 4 )							// �����ǰ��������4��
	//{
	//	bMJ[i] = 0;								// ����ȫ��4����
	//	if( CheckIsHu(bMJ, iMaxCount) ) return 1;      // ���ʣ�������ϳɹ�,����
	//	bMJ[i] = 4;								// ����,ȡ��4�����
	//}

	// 3�����(���)
	if ( bMJ[i] >= 3 )							// �����ǰ�Ʋ�����3��
	{
		bMJ[i] -= 3;							// ��ȥ3����
		if( CheckIsHu(bMJ, iMaxCount) ) return 1;      // ���ʣ�������ϳɹ�,����
		bMJ[i] += 3;							// ȡ��3�����
	}

	// 2�����(����)
	if ( !bMJ[0] && bMJ[i] >= 2 )				// ���֮ǰû�н���,�ҵ�ǰ�Ʋ�����2��
	{
		bMJ[0] = 1;								// ���ý��Ʊ�־
		bMJ[i] -= 2;							// ��ȥ2����
		if( CheckIsHu(bMJ, iMaxCount) ) return 1;      // ���ʣ�������ϳɹ�,����
		bMJ[i] += 2;							// ȡ��2�����
		bMJ[0] = 0;								// ������Ʊ�־
	}

	if ( i > 30 )    return 0;					// �����������з��ס�û��˳�����,����

	// ˳�����,ע���Ǵ�ǰ������ϣ�
	if( i%10 != 8 && i%10 != 9  &&				// �ų���ֵΪ8��9����
		bMJ[i+1] && bMJ[i+2] )					// �������������������
	{
		bMJ[i]--;
		bMJ[i+1]--;
		bMJ[i+2]--;								// ��������1
		if( CheckIsHu(bMJ, iMaxCount) ) return 1;      // ���ʣ�������ϳɹ�,����
		bMJ[i]++;
		bMJ[i+1]++;
		bMJ[i+2]++;								// �ָ�������
	}
	// �޷�ȫ�����,���ͣ�
	return 0;
}

//�ж��͸�������:������Ƿ�ʣ���ƣ������Ƶĸ���������38��С��������
int CMahJongAlg::Remain(MJBYTE bMJ[], int iMaxCount)
{
	int sum = 0;
	for(int i=1; i<iMaxCount; i++)
	{
		sum += bMJ[i];
	}
	return sum;
}

//����������п��ܵĺ������
bool CMahJongAlg::GetAllCombination(MJBYTE bMJ[], int iMaxCount, HuTree huTree)
{
	//static int JIANG = 0;						// ���Ʊ�־,�����͡��������������еġ����� ,�ŵ���0��λ��
	if( !Remain(bMJ, iMaxCount) ) return true;     // �ݹ��˳����������û��ʣ��,����Ʒ��ء�
	int i=0;
	for(i=1; !bMJ[i] && i<iMaxCount; i++);  // �ҵ����Ƶĵط�,i���ǵ�ǰ��, PAI[i]�Ǹ���

	// 3�����(����)
	if ( bMJ[i] >= 3 )							// �����ǰ�Ʋ�����3��
	{
		HuNode *huKe = new HuNode;
		huKe->mjBlock.first = MASK_MAHJONG(i/10, i%10);
		huKe->mjBlock.style = BlockStyle_Ke;
		huKe->tJiang = 0;
		huKe->tShun = 0;
		huKe->tKe = 0;
		huTree->tKe = huKe;

		bMJ[i] -= 3;							// ��ȥ3����
		GetAllCombination(bMJ, iMaxCount, huTree->tKe);
		bMJ[i] += 3;							// ȡ��3�����
	}

	// 2�����(����)
	if ( !bMJ[0] && bMJ[i] >= 2 )				// ���֮ǰû�н���,�ҵ�ǰ�Ʋ�����2��
	{
		HuNode *huJiang = new HuNode;
		huJiang->mjBlock.first = MASK_MAHJONG(i/10, i%10);
		huJiang->mjBlock.style = BlockStyle_Jiang;
		huJiang->tJiang = 0;
		huJiang->tShun = 0;
		huJiang->tKe = 0;
		huTree->tJiang = huJiang;

		bMJ[0] = 1;								// ���ý��Ʊ�־
		bMJ[i] -= 2;							// ��ȥ2����
		GetAllCombination(bMJ, iMaxCount, huTree->tJiang);
		bMJ[i] += 2;							// ȡ��2�����
		bMJ[0] = 0;								// ������Ʊ�־
	}

	// ˳�����,ע���Ǵ�ǰ������ϣ�
	if( i < 30 && i%10 != 8 && i%10 != 9  && bMJ[i+1] && bMJ[i+2] )		// �ų���ֵΪ8��9���� �������������������
	{
		HuNode *huShun = new HuNode;
		huShun->mjBlock.first = MASK_MAHJONG(i/10, i%10);
		huShun->mjBlock.style = BlockStyle_Shun;
		huShun->tJiang = 0;
		huShun->tShun = 0;
		huShun->tKe = 0;
		huTree->tShun = huShun;

		// ��������1
		bMJ[i]--;
		bMJ[i+1]--;
		bMJ[i+2]--;
		GetAllCombination(bMJ, iMaxCount, huTree->tShun);

		// �ָ�������
		bMJ[i]++;
		bMJ[i+1]++;
		bMJ[i+2]++;
	}
	//���ܺ��ƣ����Բ�������չ����
	return false;
}

//��������ȡ�����ƽṹ��������������
void CMahJongAlg::ExtractHuBlock(HuNode *huTree, MJBlock bData[][MAX_BLOCK_COUNT], int &iCombinaCount, MJBlock bTmpData[MAX_BLOCK_COUNT], int &iCombinaIndex, MJBYTE bMaxBlock)
{
	if (huTree->tJiang == 0 && huTree->tKe == 0 && huTree->tShun == 0)
	{
		//�������ˣ�����Ŀ�Բ���
		if (iCombinaIndex == bMaxBlock)
		{
			//�����ģ��ǺϷ�����,��ϼ�����+1
			memcpy(bData[iCombinaCount], bTmpData, sizeof(MJBlock) * (MAX_HAND_MJ / 3 +1));
			iCombinaCount++;
		}
		else
		{
			//���������ģ����ʧ��,���¼���
		}
		//������0
		iCombinaIndex = 0;
		return;
	}
	else
	{
		//ĳһ���������Գɹ�
		if (huTree->tKe != 0)
		{
			MJBYTE bBackIndex = iCombinaIndex;
			bTmpData[iCombinaIndex].first = huTree->tKe->mjBlock.first;
			bTmpData[iCombinaIndex].style = huTree->tKe->mjBlock.style;
			iCombinaIndex++;

			ExtractHuBlock(huTree->tKe, bData, iCombinaCount, bTmpData, iCombinaIndex, bMaxBlock);

			//��ԭ,�����ַ�ʽ���
			iCombinaIndex = bBackIndex;
		}
		if (huTree->tShun != 0)
		{
			MJBYTE bBackIndex = iCombinaIndex;
			bTmpData[iCombinaIndex].first = huTree->tShun->mjBlock.first;
			bTmpData[iCombinaIndex].style = huTree->tShun->mjBlock.style;
			iCombinaIndex++;

			ExtractHuBlock(huTree->tShun, bData, iCombinaCount, bTmpData, iCombinaIndex, bMaxBlock);

			//��ԭ,�����ַ�ʽ���
			iCombinaIndex = bBackIndex;
		}
		if (huTree->tJiang != 0)
		{
			MJBYTE bBackIndex = iCombinaIndex;
			bTmpData[iCombinaIndex].first = huTree->tJiang->mjBlock.first;
			bTmpData[iCombinaIndex].style = huTree->tJiang->mjBlock.style;
			iCombinaIndex++;

			ExtractHuBlock(huTree->tJiang, bData, iCombinaCount, bTmpData, iCombinaIndex, bMaxBlock);

			//��ԭ,�����ַ�ʽ���
			iCombinaIndex = bBackIndex;
		}
		return;
	}
}

void CMahJongAlg::ReleaseHuTree(HuNode *huTree)
{
	if (huTree == 0)
	{
		return;
	}
	if (huTree->tJiang != 0)
	{
		ReleaseHuTree(huTree->tJiang);
		huTree->tJiang = 0;
	}
	if (huTree->tKe != 0)
	{
		ReleaseHuTree(huTree->tKe);
		huTree->tKe = 0;
	}
	if (huTree->tShun != 0)
	{
		ReleaseHuTree(huTree->tShun);
		huTree->tShun = 0;
	}
	//ɾ��
	try
	{
		delete huTree;
	}
	catch (...)
	{
	}
	huTree=0;
}


//��ÿ������
enBlockStyle CMahJongAlg::GetBlockStyle(MJBYTE bMJ1, MJBYTE bMJ2, MJBYTE bMJ3)
{
	//������
	MJBYTE bTemp = 0;
	if (bMJ1 > bMJ2 && bMJ1 > bMJ3)
	{
		//bmj1���
		bTemp = bMJ1;
		bMJ1 = bMJ3;
		bMJ3 = bTemp;

		if (bMJ1 > bMJ2)
		{
			bTemp = bMJ1;
			bMJ1 = bMJ2;
			bMJ2 = bTemp;
		}
	}
	else if (bMJ1 > bMJ2 || bMJ1 > bMJ3)
	{
		//bmj1�м�
		bTemp = bMJ1;
		bMJ1 = bMJ2;
		bMJ2 = bTemp;

		if (bMJ1 > bMJ3)
		{
			bTemp = bMJ1;
			bMJ1 = bMJ3;
			bMJ3 = bTemp;
		}
	}
	else
	{
		//bmj1��С
		if (bMJ2 > bMJ3)
		{
			bTemp = bMJ2;
			bMJ3 = bMJ2;
			bMJ2 = bTemp;
		}
	}

	if (bMJ1 == bMJ2 && bMJ2 == bMJ3)
	{
		return BlockStyle_Ke;
	}
	else if (bMJ1 == bMJ2 -1 && bMJ2 == bMJ3 -1)
	{
		return BlockStyle_Shun;
	}
	else
	{
		return BlockStyle_Null;
	}
}

//���õ�(�������˴��)�������������ƺ���һ��Ȼ������
void CMahJongAlg::CombineHandMJ(MJBYTE bHandMJ[], int iHandMJCount, MJBYTE bMJ, MJBYTE bCombineMJ[], int &iCombineMJCount)
{
	iCombineMJCount = iHandMJCount;
	memcpy(bCombineMJ, bHandMJ, sizeof(MJBYTE) * iCombineMJCount);
	bCombineMJ[iCombineMJCount++] = bMJ;
	SortHandMJ(bCombineMJ, iCombineMJCount);
}

//����������п��ܵĺ������
void CMahJongAlg::GetHuCombination(MJBYTE bHandMJ[], int iHandMJCount, MJBlock bData[][MAX_BLOCK_COUNT], int &iCombinaCount)
{
	MJBYTE bMJ[MAX_ARRAY_MJ] = {0};
	for (int i=0;i<iHandMJCount;i++)
	{
		bMJ[MASK_MAHJONGKIND(bHandMJ[i])*10 + MASK_MAHJONGNUM(bHandMJ[i])]++;
	}

	HuNode* root = new HuNode;
	root->mjBlock.first = 0;
	root->mjBlock.style = BlockStyle_Null;
	root->tJiang = 0;
	root->tKe = 0;
	root->tShun = 0;
	GetAllCombination(bMJ, MAX_ARRAY_MJ, root);

	HuNode* pNode = root;
	int iIndex = 0;
	MJBlock bTmpData[MAX_BLOCK_COUNT];
	ExtractHuBlock(pNode, bData, iCombinaCount, bTmpData, iIndex, (iHandMJCount+2)/3);
	pNode = 0;
	ReleaseHuTree(root);
}
