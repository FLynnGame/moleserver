#ifndef _C_ROBOT_LOGIC_FRAME_H_INCLUDE_
#define _C_ROBOT_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "CmdHelper.h"

#define TIMER_ROBOT_OUT				1
#define TIMER_ROBOT_PENG			2
#define TIMER_ROBOT_GANG			3
#define TIMER_ROBOT_CHI				4
#define TIMER_ROBOT_TING			5
#define TIMER_ROBOT_HU				6
#define TIMER_ROBOT_GIVEUP			7
#define TIMER_ROBOT_READY			8
#define TIMER_ROBOT_START			9

class CRobotLogicFrame : public RobotLogicFrame
{
public:
	/// ���캯��
	CRobotLogicFrame();
	/// ��������
	~CRobotLogicFrame();

	/// ���õ�ǰ���
	virtual void SetGamePlayer(Player* pPlayer,Room* pRoom) { m_g_myself = pPlayer; m_g_myselfRoom = pRoom;}

	/// ���ڴ����û�׼�������Ϣ
	virtual void OnProcessPlayerReadyMes(int playerId);
	/// ���ڴ����û���ʼ��Ϸ��ʼ��Ϣ
	virtual void OnProcessPlayerGameStartMes();
	/// ���ڴ����û�������Ϸ��Ϣ
	virtual void OnProcessPlayerGameOverMes(void);
	/// ���ڴ����û�������Ϸ��������Ϣ
	virtual void OnProcessPlayerRoomMes(Json::Value &mes);
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

public:
	///������Ϣ
	void OnDealMJ(CMD_S_DealMJ cmdsDealMJ);

	///������Ϣ
	void OnOutMJ(CMD_S_OutMJ cmdsOutMJ);

	///������Ϣ
	void OnTouchMJ(CMD_S_TouchMJ cmdsTouchMJ);

	///������Ϣ
	void OnPengMJ(CMD_S_PengMJ cmdsPengMJ);

	///������Ϣ
	void OnGangMJ(CMD_S_GangMJ cmdsGangMJ);

	///������Ϣ
	void OnGiveUp(CMD_S_GiveUp cmdsGiveUp);

	///������Ϣ
	void OnHu(CMD_S_HU cmdsHu);

	///�������߼����ҳ�һ�������ƴ��ȥ
	MJBYTE GetUselessMj();

	///����
	void ReSet();

	int GetRestMjCount(MJBYTE bMj);

//�Զ����Ա����  �齫Ȩ�أ��ճɽṹ�ĸ���
private:
	typedef struct tagMjWeight
	{
		MJBYTE		bLessMj;						//��
		int			bMinCount;						//�ճɽṹ��Ҫ��������С�����������ѳ��ֵģ��ճɿ��Ӻ�˳��֮�ͣ����ֵԽ��˵��Խ���ܴճɺ��ƽṹ
	}MjWeight, *PMjWeight;

private:
	static const MJBYTE				bIsHuMJ[MAX_ORG_MJ/GAME_PLAYER];					/*!< ԭʼ���齫���� */
	MJBlock							m_bHUMJ[GAME_PLAYER][MAX_BLOCK_COUNT];				/*!< �������ƿ�(�����ܵ���) */
	int								m_iHUMJCount[GAME_PLAYER];							/*!< �����ƿ������ */
	MJBYTE							m_bHandMJ[GAME_PLAYER][MAX_HAND_MJ];				/*!< ���е��� */
	int								m_iHandMJCount[GAME_PLAYER];						/*!< ���е��Ƶ����� */
	MJBYTE							m_bOutMJ[GAME_PLAYER][MAX_OUT_MJ];					/*!< �Ѿ������� */
	int								m_iOutMJCount[GAME_PLAYER];

	MJBYTE							m_bExistMjCount[MAX_ARRAY_MJ];						/*!< �ѳ����齫�ĸ��� */
	enOperationGangType				enGangType;											/*!< �������� */

	MJBYTE							iOutMJ;												/*!< ������� */

	int								m_iOldOperUser;										/*!< �ϴε�ǰ��� */
	int								m_iOperUser;										/*!< ��ǰ������� */

	int								m_iSeOut;											/*!< �����һɫ */
	MJFLAG							m_BankerOper;										/*!< ������Ҳ��� */
	MJBYTE							m_GangMJ;
	enum time_range
	{
		MIN_TIME = 2,
		MAX_TIME = 5,
	};

	CMahJongLogic				   *m_pGameLogic;

private:
	Player *m_g_myself;
    Room *m_g_myselfRoom;
};

#endif
