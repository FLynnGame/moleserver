#ifndef MAHJONG_LOGIC_HEAD_FILE
#define MAHJONG_LOGIC_HEAD_FILE

#include "MahJongAlg.h"

//!  �齫ͨ���߼�
/*!
	�齫ͨ�õ��ж��߼��Ͳ����߼����ϣ����������Ƶ�����չ���ࡣ
	\n ��Ѫս�齫���㷨�ࣺCXZMahJongLogic
	\n class CXZMahJongLogic : public CMahJongLogic
	\n {
	\n	//����ı���...
	\n
	\n	//��չ�ķ���...
	\n }
	\n ��ʹ��ʱ������ʹ�û����ͨ�÷���������չ�ķ�����
	\n CXZMahJongLogic* xzLogic = new CXZMahJongLogic();
	\n xzLogic->SetMahJongAlg(new CXZMahJongAlg(MahJongColor_Wan|MahJongColor_Tiao|MahJongColor_Bing));//�����㷨
	\n xzLogic->����ķ���();
	\n xzLogic->��չ�ķ���();
	\note ʹ��ʱ��Ҫ���þ�����齫�㷨
*/
class CMahJongLogic
{
private:
	CMahJongAlg				*m_PMJAlgorithm;							/*!< �߼�������㷨��,�������������� */
	bool					m_bRequiredCheck;							/*!< ���ò����߼�ʱ�Ƿ���Ҫ����Ƿ�Ϸ�,���������Ʋ���ʱ��Ч */

public:
	//! ���캯��
	/*!
		��ʼ����Ա����,����Ĭ��ֵ
		\sa ~CMahJongLogic()
	*/
	CMahJongLogic() {m_PMJAlgorithm = 0; m_bRequiredCheck = false;}

	//! ��������
	/*!
		�ͷ���Դ
		\sa ~CMahJongLogic(), Release()
	*/
	~CMahJongLogic() { Release(); }

	//! �ͷ���Դ����
	/*!
		�ͷ���Դ
	*/
	void Release();

	//! ����ָ�����㷨
	/*!
		����ָ�����㷨,��������������
		\param PMJAlgorithm �齫�㷨ָ��
		\sa GetMahJongAlg()
	*/
	void SetMahJongAlg(CMahJongAlg *PMJAlgorithm);

	//! ��ȡ�齫�㷨
	/*!
		\return �����ϴ����õ��齫�㷨ָ��
		\sa SetMahJongAlg()
	*/
	CMahJongAlg *GetMahJongAlg();

	//! �����Ƿ���Ҫ�������
	/*!
		���ú��ڵ��ò�������ʱ������������кϷ��Լ��
		\note �������Ʋ�������ʧЧ
		\sa GetRequiredCheck()
	*/
	inline void SetRequiredCheck(bool bRequiredCheck = true){ m_bRequiredCheck = bRequiredCheck; }

	//! ��ȡ�Ƿ���Ҫ�������
	/*!
		\sa SetRequiredCheck()
	*/
	inline bool GetRequiredCheck(){ return m_bRequiredCheck; }

	//��������
public:
	//! ���Ʋ���
	/*!
		����:���ƴ�����������ɾ��,�ӵ�����������
		\param bSelfHandMJ ��������
		\param iSelfHandMJCount ���������С
		\param bSelfOutMJ ��������
		\param iSelfOutMJCount ���������С
		\param bOutMJ Ҫ������
		\return �����Ƿ�ɹ�
		\note �����������,��������ֱ�Ӽ�1,��Ҫ�����Ƽӵ�����������,��Ҫ�����Ƿ�ִ�м��
		\sa UnDoOutMJ(),SetRequiredCheck(),GetRequiredCheck()
	*/
	virtual bool OutMJ(MJBYTE bSelfHandMJ[], int &iSelfHandMJCount, MJBYTE bSelfOutMJ[], int &iSelfOutMJCount, MJBYTE bOutMJ);


	//! �������Ʋ���
	/*!
		���Ʋ����ж�ʧ��,�������Ʋ���:���ƴӳ���������ɾ��,�ӵ�������.����������Ӧ����3N*2��
		\param bSelfHandMJ ��������
		\param iSelfHandMJCount ���������С
		\param bSelfOutMJ ��������
		\param iSelfOutMJCount ���������С
		\param bOutMJ ������
		\return �����Ƿ�ɹ�
		\note �������������ڶԵ�ǰ�Լ�����,���ܶ������Ʋ���
		\sa OutMJ(),SetRequiredCheck(),GetRequiredCheck()
	*/
	virtual bool UnDoOutMJ(MJBYTE bSelfHandMJ[], int &iSelfHandMJCount, MJBYTE bSelfOutMJ[], int &iSelfOutMJCount, MJBYTE bOutMJ);


	//! ���Ʋ���
	/*!
		���ݳ��Ƶ�����,��ɾ���������,��ӳ��Ƶ�����,ɾ���������һ����
		\param bSelfHuMJ ��������
		\param iSelfHuCount ���������С
		\param bSelfHandMJ ��������
		\param iSelfHandCount ���������С
		\param bLastOutMJ ��������ҵĳ�������
		\param iLastOutMJCount ��������ҵĳ��������С
		\param bChi Ҫ�Ե���
		\param bChiType ���Ƶ�����
		\return �����Ƿ�ɹ�
		\note �����������,��������ֱ�Ӽ�2,���ṹ�ӵ�����������.��Ҫ�����Ƿ�ִ�м��
		\sa UnDoChi(), IsChi(), SetRequiredCheck(), GetRequiredCheck()
	*/
	virtual bool Chi(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bChi, enOperationChiType bChiType);


	//! �������Ʋ���
	/*!
		���Ʋ����ж�ʧ��,�������Ʋ���:���ݳ��Ƶ�����,����������,ɾ�������гԵ���,��ӳ������һ����
		\param bSelfHuMJ ��������
		\param iSelfHuCount ���������С
		\param bSelfHandMJ ��������
		\param iSelfHandCount ���������С
		\param bLastOutMJ ��������ҵĳ�������
		\param iLastOutMJCount ��������ҵĳ��������С
		\param bChi �Ե���
		\param bChiType ���Ƶ�����
		\return �����Ƿ�ɹ�
		\note �������������ڶԵ�ǰ�Լ�����,���ܶ������Ʋ���
		\sa Chi(), IsChi(), SetRequiredCheck(), GetRequiredCheck()
	*/
	virtual bool UnDoChi(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bChi, enOperationChiType bChiType);


	//! ���Ʋ���
	/*!
		����:���ϴγ�����ҳ������һ����ɾ��,��ӵ�������ҵĺ�����,ͬʱɾ�����������������
		\param bSelfHuMJ ��������
		\param iSelfHuCount ���������С
		\param bSelfHandMJ ��������
		\param iSelfHandCount ���������С
		\param bLastOutMJ ��������ҵĳ�������
		\param iLastOutMJCount ��������ҵĳ��������С
		\param bPeng Ҫ������
		\return �����Ƿ�ɹ�
		\note �����������,��������ֱ�Ӽ�2,���ṹ�ӵ�����������.��Ҫ�����Ƿ�ִ�м��
		\sa UnDoPeng(), IsPeng(), SetRequiredCheck(), GetRequiredCheck()
	*/
	virtual bool Peng(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bPeng);


	//! �������Ʋ���
	/*!
		���Ʋ����ж�ʧ��,�������Ʋ���::�����ϴγ�����ҳ������һ����,ɾ��������������,������������������
		\param bSelfHuMJ ��������
		\param iSelfHuCount ���������С
		\param bSelfHandMJ ��������
		\param iSelfHandCount ���������С
		\param bLastOutMJ ��������ҵĳ�������
		\param iLastOutMJCount ��������ҵĳ��������С
		\param bPeng Ҫ������
		\return �����Ƿ�ɹ�
		\note �������������ڶԵ�ǰ�Լ�����,���ܶ������Ʋ���
		\sa Peng(), IsPeng(), SetRequiredCheck(), GetRequiredCheck()
	*/
	virtual bool UnDoPeng(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bPeng);


	//! �ܲ���
	/*!
		����:��Ϊ���ܡ��������ܡ����ܡ�
		����:����������ҵ����һ���ƺ��Լ�3��ָ������ɾ��,�������
		��������:��1��ָ������ɾ��,�����Ƹ�Ϊ����
		����:ɾ��4��ָ������,��Ӱ���
		\param bSelfHuMJ ��������
		\param iSelfHuCount ���������С
		\param bSelfHandMJ ��������
		\param iSelfHandCount ���������С
		\param bLastOutMJ ��������ҵĳ�������
		\param iLastOutMJCount ��������ҵĳ��������С
		\param bGang Ҫ�ܵ���
		\param bGangType �ܵ�����
		\return �����Ƿ�ɹ�
		\note ��Ҫ�����Ƿ�ִ�м��
		\sa UnDoGang(), IsOutMingGang(), IsSelfMingGang(), IsAnGang(), SetRequiredCheck(), GetRequiredCheck()
	*/
	virtual bool Gang(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bGang, enOperationGangType bGangType);


	//! �����ܲ���
	/*!
		����:��Ϊ���ܡ��������ܡ����ܡ�
		����:���س�����ҵ����һ���ƺ��Լ�3��ָ������,ɾ������
		��������:����1��ָ������,�����ܸ�Ϊ����
		����:����4��ָ������,ɾ������
		\param bSelfHuMJ ��������
		\param iSelfHuCount ���������С
		\param bSelfHandMJ ��������
		\param iSelfHandCount ���������С
		\param bLastOutMJ ��������ҵĳ�������
		\param iLastOutMJCount ��������ҵĳ��������С
		\param bGang Ҫ�ܵ���
		\param bGangType �ܵ�����
		\return �����Ƿ�ɹ�
		\note ��Ҫ�����Ƿ�ִ�м��
		\sa Gang(), IsOutMingGang(), IsSelfMingGang(), IsAnGang(), SetRequiredCheck(), GetRequiredCheck()
	*/
	virtual bool UnDoGang(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bGang, enOperationGangType bGangType);


	// ��ͨ���ܲ���
	/*
		����:���˴������,��ͨ����,����������ҵ����һ����ɾ��
		�����������ɾ������ӵ�������Һ�����,����Ǻ�����ı����ͣ�������Ϊ�ܣ�
		\param bSelfHuMJ ��������
		\param bSelfHuCount ���������С
		\param bSelfHandMJ ��������
		\param bSelfHandCount ���������С
		\param bLastOutMJ ��������ҵĳ�������
		\param bLastOutMJCount ��������ҵĳ��������С
		\param bGang Ҫ�ܵ���
		\return �����Ƿ�ɹ�
		\note �����������,��������ֱ�Ӽ�2,���ṹ�ӵ�����������.��Ҫ�����Ƿ�ִ�м��
		\sa UnDoNormalGang(), IsOutMingGang(), IsSelfMingGang(), IsAnGang(), ZMGang(), SetRequiredCheck(), GetRequiredCheck()
	*/
	//virtual bool NormalGang(MJBlock bSelfHuMJ[], MJBYTE &bSelfHuCount, MJBYTE bSelfHandMJ[], MJBYTE &bSelfHandCount, MJBYTE bLastOutMJ[], MJBYTE &bLastOutMJCount, MJBYTE bGang);

	// �������ܺͰ��ܲ���
	/*
		����:�Լ�������,���������ܺͰ���,�������ƴ�������ɾ��
		��������:�ı�����������ͣ�������Ϊ�ܣ�
		����:ɾ������
		\param bSelfHuMJ ��������
		\param bSelfHuCount ���������С
		\param bSelfHandMJ ��������
		\param bSelfHandCount ���������С
		\param bTouch Ҫ�ܵ���,��������
		\return �����Ƿ�ɹ�
		\note �����������,��������ֱ�Ӽ�2,���ṹ�ӵ�����������.��Ҫ�����Ƿ�ִ�м��
		\sa IsOutMingGang(), IsSelfMingGang(), IsAnGang(), NormalGang(), SetRequiredCheck(), GetRequiredCheck()
	*/
	//virtual bool ZMGang(MJBlock bSelfHuMJ[], MJBYTE &bSelfHuCount, MJBYTE bSelfHandMJ[], MJBYTE &bSelfHandCount, MJBYTE bTouch);

	//��������
public:
	//! ��ȡ�����
	/*!
		��ȡָ����Χ�������
		\param iMin ����,�����Χ��߽�
		\param iMax ����,�����Χ�ұ߽�
		\return iMin��iMax֮��������
	*/
	virtual int GetRandomNum(int iMin, int iMax);

	//! ��������
	/*!
		��������������˳��,���ҵĴ�����RANDOM_TIMES����
		\param bAllMJ ������
		\param iAllMJCount �������С
		\sa RANDOM_TIMES
	*/
	virtual void RandomMJ(MJBYTE bAllMJ[], int iAllMJCount);

	//! ������������
	/*!
		���������С��������
		\param bAllMJ ������
		\param iAllMJCount �������С
	*/
	virtual void SortHandMJ(MJBYTE bAllMJ[], int iAllMJCount);

	//! У�����Ƿ����
	/*!
		У������ָ�����������Ƿ����
		\param bAllMJ ������
		\param iAllMJCount �������С
		\param bMJ ָ���齫��
		\return true-����;false-������
	*/
	virtual bool CheckMJExist(const MJBYTE bAllMJ[], int iAllMJCount, MJBYTE bMJ);

	//! ɾ��ָ���齫
	/*!
		����������ɾ��ָ���齫,�����ʱ���ص�(������Ϊ0),��������1
		\param bAllMJ ������
		\param iAllMJCount �������С
		\param bMJ ָ���齫��
		\return true-ɾ���ɹ�;false-ɾ��ʧ��
	*/
	virtual bool DeleteMJ(MJBYTE bAllMJ[], int &iAllMJCount, MJBYTE bMJ);

	//! ���ָ���齫������
	/*!
		\param bAllMJ ������
		\param iAllMJCount �������С
		\param bMJ ָ���齫��
		\return ָ���齫���齫�����е�����
	*/
	virtual int GetMJCount(MJBYTE bAllMJ[], int &iAllMJCount, MJBYTE bMJ);

	//////////////////////////////////////////////////////////////////////////

	//�ж�����
public:
	//! �����ж�
	/*!
		�ж��ܷ��,�������������ܷ����123������
		\param bSelfHUMJ ��������
		\param iSelfHUMJCount ���������С
		\param bSelfMJ ��������
		\param iSelfMJCount ���������С
		\param bMJOut ���˳�����
		\param bChiType ����ܳ�,�����ܳԵ�����
		\return �Ƿ��ܳ�
		\\note �ϼҴ���Ʋ��ܳ�
		\sa Chi()
	*/
	virtual bool IsChi(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJOut, MJFLAG &bChiType);

	//! �����ж�
	/*!
		�ж��ܷ���,�������������ܷ����111������.�����������������
		\param bSelfHUMJ ��������
		\param iSelfHUMJCount ���������С
		\param bSelfMJ ��������
		\param iSelfMJCount ���������С
		\param bMJOut ���˳�����
		\return �Ƿ�����
		\sa Peng()
	*/
	virtual bool IsPeng(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJOut);

	//! �ж��ܷ�����
	/*!
		�ж��ܷ�����:���˳����������Ƿ���3����ͬ��
		\param bSelfHUMJ ��������
		\param iSelfHUMJCount ���������С
		\param bSelfMJ ��������
		\param iSelfMJCount ���������С
		\param bMJOut ���˳�����
		\return �Ƿ�������
		\sa IsSelfMingGang(), IsAnGang(), NormalGang(), ZMGang()
	*/
	virtual bool IsOutMingGang(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJOut);

	//! �ж��Ƿ���������
	/*!
		�ж��ܷ���������:�Լ��������Ƿ�����������ͬ
		\param bSelfHUMJ ��������
		\param iSelfHUMJCount ���������С
		\param bMJTouch ������
		\return �Ƿ�����������
		\sa IsOutMingGang(), IsAnGang(), NormalGang(), ZMGang()
	*/
	virtual bool IsSelfMingGang(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bMJTouch);

	//! �ж��ܷ񰵸�
	/*!
		�ж��ܷ񰵸�:�Լ������Ʋ��ܰ���,����ͬʱ���ڶ������,���ɸܵ��Ʒ���bGang
		\param bSelfHUMJ ��������
		\param iSelfHUMJCount ���������С
		\param bSelfMJ ��������
		\param iSelfMJCount ���������С
		\param bGang ��������
		\param iGangCount ���������С
		\return �Ƿ��ܰ���
		\note �������ѷ�������
		\sa IsOutMingGang(), IsSelfMingGang(), NormalGang(), ZMGang()
	*/
	virtual bool IsAnGang(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bGang[], int &iGangCount);

		//! �ж��ܷ񰵸�
	/*!
		�ж��ܷ񰵸�:�Լ������Ʋ��ܰ���,����ͬʱ���ڶ������,���ɸܵ��Ʒ���bGang
		\param bSelfHUMJ ��������
		\param iSelfHUMJCount ���������С
		\param bSelfMJ ��������
		\param iSelfMJCount ���������С
		\param bMJTouch ���ĵ�14����
		\param bGang ��������
		\param iGangCount ���������С
		\return �Ƿ��ܰ���
		\note �����ƻ�δ��������
		\sa IsOutMingGang(), IsSelfMingGang(), NormalGang(), ZMGang()
	*/
	virtual bool IsAnGang(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJTouch, MJBYTE bGang[], int &iGangCount);


	//! �ж��ܷ������
	/*!
	�ж��ܷ������:�Լ������Ʋ��������,����ͬʱ���ڶ�������,���ɸܵ��Ʒ���bGang
	\param bSelfHUMJ ��������
	\param iSelfHUMJCount ���������С
	\param bSelfMJ ��������
	\param iSelfMJCount ���������С
	\param bMJTouch ���ĵ�14����
	\param bGang ��������
	\param iGangCount ���������С
	\return �Ƿ��������
	\note �����ƻ�δ��������
	\sa IsOutMingGang(), IsSelfMingGang(), NormalGang(), ZMGang()
	*/
	virtual bool IsXFGang(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJTouch, MJBYTE bGang[], int &iGangCount);


	//! �ж��ܷ����
	/*!
		�ж��ܷ����:ֻ�ж��Ƿ������111 111 111 111 11һ�����,���������Ҫ���ض�����������ʵ���߼�
		\param bSelfHUMJ ��������
		\param iSelfHUMJCount ���������С
		\param bSelfHandMJ ��������
		\param iSelfHandMJCount ���������С
		\param bMJ �ж����Ƶĵ�14����
		\return �Ƿ��ܺ���
		\sa IsOutMingGang(), IsSelfMingGang(), NormalGang(), ZMGang()
	*/
	virtual bool IsHu(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfHandMJ[], int iSelfHandMJCount, MJBYTE bMJ);
};

#endif
