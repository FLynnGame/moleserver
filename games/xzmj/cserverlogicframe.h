#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "CmdHelper.h"
#include "GameLogic.h"
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>

class CServerLogicFrame : public ServerLogicFrame
{
public:
	/// ���캯��
	CServerLogicFrame();
	/// ��������
	~CServerLogicFrame();

	/// ���õ�ǰӦ�÷���
	virtual void SetGameRoom(Room* pRoom) { m_g_GameRoom = pRoom; }
	/// ���ڴ����û���ʼ��Ϸ��ʼ��Ϣ
	virtual void OnProcessPlayerGameStartMes();
	/// ���ڴ����û�������Ϸ��������Ϣ
	virtual void OnProcessPlayerRoomMes(int playerId,Json::Value &mes);
	/// �����û����뷿����Ϣ
	virtual void OnProcessEnterRoomMsg(int playerId);
	/// �����û��뿪������Ϣ
	virtual void OnProcessLeaveRoomMsg(int playerId);
	/// �����û�����������Ϣ
	virtual void OnProcessReEnterRoomMes(int playerId);
	/// �����û�������Ϣ
	virtual void OnProcessOfflineRoomMes(int playerId);
	/// �����û���ʱ����Ϣ
	virtual void OnProcessTimerMsg(int timerId,int curTimer);

	//////////////////////////////////////////////////////////////////////////
	void Reset();
	//////////////////////////////////////////////////////////////////////////

	void OperOut(int iChairID, CMD_C_OutMJ &cmdOut, bool isTimeOut = false);

	void OperAUTO(int iChairID, bool isTimeOut);

	void OperTouch(int iChairID, int iTouchTpye = 0);

	void OperPeng(int playerId);

	void OperGang(int iChairID, CMD_C_GangMJ &cmdGang);

	void FanCount(int &iChairID, MJBYTE bMJ, int FanStyle[], enHuType enhuType, int64 &iHuPaiFan, int64 &iExtraFan);

	void LookHuaZhuDaJiao(CMD_S_GameOver &cmdsGameOver);

	void OperHu(int playerId, int iDianPaoPlayer, enHuType enhuType);

	void OperGameOver(enEndGameType endGameType);

	void OperGiveUp(int playId);

	void OperTrustee(int iChairID, bool isTimeOut = false);

	//iWalkType�������� 0��ʾ���� 1��ʾ����
	bool isHu(int &iChairID, MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfHandMJ[], int iSelfHandMJCount, MJBYTE bMJ, bool iWalkType = false);

	void UpdateOperation(int &iChairID, MJBYTE &iOutMJ, MJBlock bHuMJ[], MJBYTE iHuMJCount, MJBYTE bHandMJ[], MJBYTE iHandMJCount, MJBYTE bOutMJ[], MJBYTE iOutMJCount, int &OperationType, bool iWalkType = false);
	int  UpdatePriority(int &iChairID, int &iOutPlayer);

	//ȡ����ҽ���ʱ��״̬
	void getUserEndType(int iHuUser, enHuType enhuType, enUserEndType enUSEendType[]);

	void GoodMJToRobot(MJBYTE bAllMJ[], int iAllMJCount, int iChairID);

#ifdef ENABLE_DEBUG_TOOL
	BOOL ForTest();
#endif

//ö�ٶ��� ��ö���滻���еĺ�
private:

	enum Timer_Msg
	{
		//��������ʱ����ʹ��
		TIMER_Nomal	= 1,				//�Զ�����
		TIMER_BUFFER,				//����йܻ������ƺ��ӳ�2����ơ��ȴ��ͻ��˵Ķ������
		TIMER_CHANGETABLE,			//����
		TIMER_USER_OFFLINE,			//��ҵ���

		TIMER_QUIT = 9,
		TIMER_STRATDROP,			//��ҿ��ֵ���
		TIMER_START,
		TIMER_DELAY_READY,
		TIMER_DHU,
		TIMER_DHUNomal,
		TIMER_NewNomal,				//�����Զ���������
	};

	//��������غ궨��
	enum Other_HongDefine
	{
		GuaFengMaxCount = 16,
	};

	//�η�������غ궨��
	enum GuaFengRatio
	{
		GuaFeng_Null = 0,
		GuaFeng_Ming,
		GuaFeng_An_ZiMoMing,
	};
//�ṹ��Ķ���
private:
	/*!< ��ҹη����������Ϣ */
	typedef struct tagGuaFengInfo
	{
		enOperationGangType			eGangType;										/*!< �ܵ����� */
		//MJBYTE						eMJ;
		int							eGangPlayer;									/*!< �ܵ���� */
		int							eYinGanger[GAME_PLAYER];						/*!< ���ܵ����,-1Ϊû�������ܣ�0123��ʾ�������ID */
		int64							eGangSorce[GAME_PLAYER];										/*!< �ܵķ�����Ϊoperhu�ṩ���� */
	}GuaFengInfo;

	/*!< ��ҿ��ò�����Ϣ */
	typedef struct tagOperationInfo
	{
		MJFLAG						bOperation;										/*!< ��ҿ��ò��� */
		enOperationGangType			eGangType;										/*!< ����иܣ��ܵ����͡�ֻ��һ�� */
		int32						iGangCount;										/*!< ����Ŀ */
		MJBYTE						bMJGang[MAX_SELECT_GANG];						/*!< �ɸܵ��� */
	}OperationInfo;

	/*!< ��ҹ�ˮ������Ϣ */
	typedef struct tagGuoShuiInfo
	{
		MJBYTE						bGuoShuiMJ;
		int							bGSStartUser;
		int							bWhoHuUser;
		int							bGangPao;
	}GuoShuiInfo;

	/*!< ��Һ�����Ϣ */
	typedef struct tagEnHuInfo
	{
		enHuType					iHuInfoType;
		int							iHuUser;
		int							iDianPaoUser;
		MJBYTE						bHuMJ;
		int64							iHuFan;
		int64							iExtraFan;
		int							iFanStyle[MAX_FAN_STYLE];
	}EnHuInfo;

private:
	Room *m_g_GameRoom;                                  /**< ��Ϸ���� */

	CMahJongLogic				*m_pGameLogic;

	GuaFengInfo					m_GuaFengInfo[GuaFengMaxCount];						/*!< �洢�η����������Ϣ�����ڷ�����ҹη�����÷� */
	OperationInfo				m_OperationInfo[GAME_PLAYER];						/*!< ��ҵ������Ϣ */
	GuoShuiInfo					m_GuoShuiInfo[GAME_PLAYER];							/*!< ��ˮ������־ */
	EnHuInfo					m_HuInfo[GAME_PLAYER];								/*!< ��Һ�����Ϣ */
	int64							m_AllGangSorce[GAME_PLAYER];						/*!< ��Ҹܵ��ܷ֣������ܽ���Ͷ���ʱʹ�� */
	int64							m_AllHuSorce[GAME_PLAYER];							/*!< ��Һ����ܷ֣������ܽ���ʹ�� */
	int64							m_AllTotal[GAME_PLAYER];							/*!< ��������ܷ֣������ܽ���ʹ�� */

	static const MJBYTE			bOriginalMJ[MAX_ORG_MJ];							/*!< ԭʼ���齫���� */
	int							m_iBisicCore;
	MJBYTE						m_bAllMJ[MAX_ORG_MJ];								/*!< �齫������ */
	int							m_iTouchPos;										/*!< ����λ�� */
	int							m_iDiceNum01;										/*!< ������1 */
	int							m_iDiceNum02;										/*!< ������2 */
	int							m_iDiceNum11;										/*!< ������2 */
	int							m_iDiceNum12;										/*!< ������2 */
	int							m_iIsReRandomBanker;								/*!< 4�����һ�����Ƶľ��� */

	int							m_OperPriority[GAME_PLAYER];						/*!< ����ҿ��ò������ȼ� */
	int							m_TruBuffer[GAME_PLAYER];
	int							m_iBankerUser;										/*!< ׯ */

	//////////////////////////////////////////////////////////////////////////
	//������� m_iOldOperateUer�� m_iOldCurrentUer���ܲ����
	int							m_iOldOperateUer;									/*!<  �ϴβ������*/
	int							m_iOperateUer;										/*!<  ��ǰ�������*/
	int							m_iOldCurrentUer;									/*!<  ��һ�γ��ƵĲ������*/
	bool						m_StartDrop;

	MJBYTE						m_bTouchMJ;											/*!< ���������� */
	MJBYTE						m_bGangKaiMJ[GAME_PLAYER];							/*!< �ܿ����� */
	MJBYTE						m_bGangTouchMJ;										/*!< ��������� */
	bool						m_isLiuLie;											/*!< �Ƿ����� */
	int							m_isLiuLieUser[GAME_PLAYER];
	//bool						m_LastOutMJLose;									/*!< ���һ�γ����齫�Ƿ���ʧ */
	MJBlock						m_bHUMJ[GAME_PLAYER][MAX_BLOCK_COUNT];				/*!< �������ƿ�(�����ܵ���) */
	int							m_iHUMJCount[GAME_PLAYER];							/*!< �����ƿ������ */
	MJBYTE						m_bHandMJ[GAME_PLAYER][MAX_HAND_MJ];				/*!< ���е��� */
	int							m_iHandMJCount[GAME_PLAYER];						/*!< ���е��Ƶ����� */
	MJBYTE						m_bOutMJ[GAME_PLAYER][MAX_OUT_MJ];					/*!< �Ѿ������� */
	int							m_iOutMJCount[GAME_PLAYER];							/*!< �Ѿ������Ƶ����� */
	int							m_iAUTO_OutCount[GAME_PLAYER];						/*!< ����˴���������������򳬹����ξ��Զ��й� */

	int							m_TouchPlayer;										//��ǰ�������

	enPlayerStatu				enUserStatu[GAME_PLAYER];							/*!< ���״̬ */
	enPlayerStatu				enUserStatuOld[GAME_PLAYER];						/*!< ��ҵ���ǰ��״̬ */

	int							m_operationGaveUp[GAME_PLAYER];						/*!< ����������,��ʼ��Ϊoperation_hu���ܺ�����������Ϊ0�����ø������������²���ʱ�����ڼ������� */
	int							m_bQiangGangMJ[GAME_PLAYER];						/*!< ���ܺ��������齫��ֻҪ�ܺ�����ң�ÿ���˶���һ�� */
	int							m_isAlreadyHu;										/*!< �Ƿ��Ѿ����������ƣ��´γ���ʱ��� */

	//int							m_isTianDiHu;										/*!< 0ʲô��û�У�1��� 2�غ� */
	int							m_UserTouchFirst[GAME_PLAYER];						/*!< ��ҵ�һ�����ƣ�û����Ϊ0��������Ϊ1��Ϊ�غ�׼�� */
	int							m_isForceQuit[GAME_PLAYER];							/*!< �Ƿ���ǿ���˳����*/

	bool						m_StartQuit;										/*!< ���ֵ�ʱ���Ƿ������ǿ��*/
	int64							m_QuitSorce[GAME_PLAYER];							/*!< ���ǿ�˿۵ķ�*/
	MJBYTE						m_huMJ;												/*!< ���������齫*/

	int							m_zuozhuang;										/*!< ��ׯ����*/
	int							m_twice;
	int							m_DoubleHuPlayer;									/*!< һ�ڶ���ʱ��Ҫ�������й�*/
	int							m_iOldHandCount[GAME_PLAYER];
	int							m_HasHuTouch;
	int							m_QiangHuCount;										/*!<���ܺ���������-1û�и�ֵ*/
	int							m_GiveQiangHuCount;									/*!<���ܺ���������-1û�и�ֵ*/
	int							m_BeiQiangUser;
	int							m_ReadyCount;
	int64						m_UserMoney[GAME_PLAYER];
	CMD_S_GameOver				m_CmdsGameOver;

	int							m_StartUserID[GAME_PLAYER];
	int							m_ForceUserID[GAME_PLAYER];
	int							m_huRanderUser[GAME_PLAYER];
	int							m_HuPos;
#ifdef ENABLE_DEBUG_TOOL
	BOOL						m_IsDebug;											//������
#endif
};

#endif
