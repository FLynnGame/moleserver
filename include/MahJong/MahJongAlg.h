#ifndef MAHJONG_ALG_HEAD_FILE
#define MAHJONG_ALG_HEAD_FILE

#include "MahJongStruct.h"

//! �齫��ɫ
/*!
	�齫��ɫ,����ָ�����������еĻ�ɫ
*/
enum enMahJongColor
{
	MahJongColor_Invalid	= 0x00,											/*!< ��Чֵ */
	MahJongColor_Wan		= 0x01,											/*!< ���� */
	MahJongColor_Tiao		= 0x02,											/*!< ���� */
	MahJongColor_Bing		= 0x04,											/*!< Ͳ�� */
	MahJongColor_Word		= 0x08,											/*!< ���� */
	MahJongColor_Flower		= 0x10											/*!< ���� */
};


//! �齫ͨ���㷨
/*! �齫ͨ���㷨���ϣ�������齫�㷨ֻ��̳и��༴�ɡ�
	\n ��Ѫս�齫���㷨�ࣺCXZMahJongAlg
	\n class CXZMahJongAlg : public CMahJongAlg
	\n {
	\n 	//����ı���...
	\n
	\n 	//��չ�ķ���...
	\n }
	\n ��ʹ��ʱ������ʹ�û����ͨ�÷���������չ�ķ�����
	\n CXZMahJongAlg* xzAlg = new CXZMahJongAlg(MahJongColor_Wan|MahJongColor_Tiao|MahJongColor_Bing);
	\n xzAlg->����ķ���();
	\n xzAlg->��չ�ķ���();
	\note��ʼ��ʱ��Ҫ�����齫��ɫ
	\sa CMahJongLogic
*/
class CMahJongAlg
{
private:
	MJBYTE				m_bMahJongColor;									/*!< �����齫����������ɫ(�������ֻ���) */

public:
	//! ���캯��
	/*!
		�������������,����Ĭ�ϵ��齫��ɫ(�������ֻ�)
		\param bMahJongColor �齫��ɫ,ÿһλ����һ����ɫ,��enMahJongColor����.Ĭ����ɫ:�������ֻ�
	*/
	CMahJongAlg(MJBYTE bMahJongColor = (MahJongColor_Wan|MahJongColor_Tiao|MahJongColor_Bing|MahJongColor_Word|MahJongColor_Flower)){ srand((uint32)time(NULL)); m_bMahJongColor = bMahJongColor; }

	//! �����齫��ɫ
	/*!
		�����齫��ɫ,��MJBYTE��ʾ,ÿһλ����һ����ɫ
		\param bMahJongColor �齫��ɫ,ÿһλ����һ����ɫ,��enMahJongColor����
		\sa enMahJongColor, GetMahJongColor()
	*/
	inline void SetMahJongColor(MJBYTE bMahJongColor){ m_bMahJongColor = bMahJongColor; }

	//! ��ȡ�齫��ɫ
	/*!
		��ȡ�齫��ɫ,��MJBYTE��ʾ,ÿһλ����һ����ɫ
		\return �齫��ɫ,ÿһλ����һ����ɫ,��enMahJongColor����
		\sa enMahJongColor, SetMahJongColor()
	*/
	inline MJBYTE GetMahJongColor(){ return m_bMahJongColor; }

	//����
public:
	//! ��ȡ�����
	/*!
		��ȡָ����Χ�������
		\param iMin ����,�����Χ��߽�
		\param iMax ����,�����Χ�ұ߽�
		\return iMin��iMax֮��������
	*/
	virtual inline int GetRandomNum(int iMin, int iMax)
	{
		srand((uint32)time(NULL));
		return rand() % (iMax + 1 - iMin) + iMin;
	}

	//! ��������
	/*!
		��������������˳��,���ҵĴ�����RANDOM_TIMES����
		\param bAllMJ ������
		\param bAllMJCount �������С
		\sa RANDOM_TIMES
	*/
	virtual void RandomMJ(MJBYTE bAllMJ[], int bAllMJCount);

	//! ������������
	/*!
		���������С��������
		\param bAllMJ ������
		\param bAllMJCount �������С
	*/
	virtual void SortHandMJ(MJBYTE bAllMJ[], int bAllMJCount);

	//! У�����Ƿ����
	/*!
		У������ָ�����������Ƿ����
		\param bAllMJ ������
		\param bAllMJCount �������С
		\param bMJ ָ���齫��
		\return true-����;false-������
	*/
	virtual bool CheckMJExist(const MJBYTE bAllMJ[], int bAllMJCount, MJBYTE bMJ);

	//! ɾ��ָ���齫
	/*!
		����������ɾ��ָ���齫,�����ʱ���ص�(������Ϊ0),��������1
		\param bAllMJ ������
		\param bAllMJCount �������С
		\param bMJ ָ���齫��
		\return true-ɾ���ɹ�;false-ɾ��ʧ��
	*/
	virtual bool DeleteMJ(MJBYTE bAllMJ[], int &bAllMJCount, MJBYTE bMJ);

	//! ���ָ���齫������
	/*!
		\param bMJ ������
		\param bMJCount �������С
		\param bM ָ���齫��
		\return ָ���齫���齫�����е�����
	*/
	virtual int GetMJCount(MJBYTE bMJ[], int &bMJCount, MJBYTE bM);

	//////////////////////////////////////////////////////////////////////////

	//���������ж�
public:
	//! һ������ж�
	/*!
		�ж��Ƿ���һ����ƣ���3*N+2�����Ƿ��ܴճ� 1�Խ���N���齫������
		\param bSelfHUMJ ����������
		\param bSelfHUMJCount �����������С
		\param bSelfHandMJ ����������
		\param bSelfHandMJCount �����������С
		\return true-�ܺ�;FLASE-���ܺ�
		\note �ж�һ��ĺ���ʹ�������������
	*/
	virtual bool IsNormalHu(MJBlock bSelfHUMJ[], int bSelfHUMJCount, MJBYTE bSelfHandMJ[], int bSelfHandMJCount);

	//����
protected:
	//! һ������ж�
	/*!
		���ʣ��������Ƿ��ܺ����������ʾ
		\param bHandMJ ����������
		\param bHandMJCount �����������С
		\return true-�ܺ�;FLASE-���ܺ�
		\note �ж�һ��ĺ���ʹ�� IsNormalHu() ����
		\sa IsNormalHu(), CheckIsHu()
	*/
	virtual bool CheckRestHu(MJBYTE bHandMJ[], int bHandMJCount);

	//! һ������ж�
	/*!
		������Ƿ��ܺ��������Ƶĸ���������MAX_ARRAY_MJ��С��������
		\param bMJ �齫������
		\param bMaxCount �齫��������С
		\return true-�ܺ�;FLASE-���ܺ�
		\note �ж�һ��ĺ���ʹ�� IsNormalHu() ����
		\sa CheckRestHu(), MAX_ARRAY_MJ
	*/
	virtual bool CheckIsHu(MJBYTE bMJ[], int bMaxCount);

	//! ������Ϲ������Ƿ���ʣ����
	/*!
		������Ƿ�ʣ���ƣ������Ƶĸ���������MAX_ARRAY_MJ��С��������
		\param bMJ �齫������
		\param bMaxCount �齫��������С
		\return �����Ƶĸ���
		\note �ж�һ��ĺ���ʹ�� IsNormalHu() ����
		\sa MAX_ARRAY_MJ
	*/
	virtual int Remain(MJBYTE bMJ[], int bMaxCount);

	//! ��ȡ�������
	/*!
		����������п��ܵĺ������,�磺333 444 555 123 77Ҳ����Ϊ345 345 345 123 7738
		\param bMJ �齫������
		\param bMaxCount �齫��������С
		\param huTree �齫�����
		\return �Ƿ��ܴճ����
		\note ��ȡ�������ʹ�� GetHuCombination() ����
	*/
	virtual bool GetAllCombination(MJBYTE bMJ[], int bMaxCount, HuTree huTree);

	//! ��������ȡ�����ƽṹ
	/*!
		��������ȡ�����ƽṹ��������������(�������ƿ����飬��ʱ��(���ڵݹ��ݴ���һ�����)�������Ŀ������п����������������)
		\param huTree �齫�����
		\param bData �齫�������
		\param iCombinaCount �齫��������С
		\param bTmpData ��ʱ��,���ڵݹ��ݴ���һ�����
		\param iCombinaIndex ��ǰ�齫������齫�������,�����bMaxBlock�������һ������
		\param bMaxBlock �齫������齫������С
		\return �Ƿ��ܴճ�����1�����
		\note ��ȡ�������ʹ�� GetHuCombination() ����
	*/
	virtual void ExtractHuBlock(HuNode *huTree, MJBlock bData[][MAX_BLOCK_COUNT], int &iCombinaCount, MJBlock bTmpData[MAX_BLOCK_COUNT], int &iCombinaIndex, MJBYTE bMaxBlock);

	//! �ͷ������ڴ�
	/*!
		�ͷ������ڴ��ͷź��������ڴ�
		\param huTree �齫�������ָ��
	*/
	virtual void ReleaseHuTree(HuNode *huTree);

	//����
public:
	//! ��ÿ������
	/*!
		��ø���3���Ƶ�����
		\param bMJ1 ��һ��������
		\param bMJ2 �ڶ���������
		\param bMJ3 ������������
		\return ����������enBlockStyle
		\sa enBlockStyle
	*/
	virtual enBlockStyle GetBlockStyle(MJBYTE bMJ1, MJBYTE bMJ2, MJBYTE bMJ3);

	//! ���Ƽӵ�������
	/*!
		���õ�(�������˴��)�������������ƺ���һ��Ȼ������,���浽bCombineMJ��
		\param bHandMJ ��������
		\param iHandMJCount ���������С
		\param bMJ ������������
		\param bCombineMJ ���������
		\param iCombineMJCount ����������С
	*/
	virtual void CombineHandMJ(MJBYTE bHandMJ[], int iHandMJCount, MJBYTE bMJ, MJBYTE bCombineMJ[], int &iCombineMJCount);

	//! ����������п��ܵĺ������
	/*!
		����������п��ܵĺ������,(����,���Ƹ�������Ͽ�(����ǿ������)����ϸ���)
		\param bHandMJ ��������
		\param iHandMJCount ���������С
		\param bData ��Ͻṹ����
		\param iCombinaCount ��Ͻṹ�����С
	*/
	virtual void GetHuCombination(MJBYTE bHandMJ[], int iHandMJCount, MJBlock bData[][MAX_BLOCK_COUNT], int &iCombinaCount);
};

#endif
