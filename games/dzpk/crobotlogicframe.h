#ifndef _C_ROBOT_LOGIC_FRAME_H_INCLUDE_
#define _C_ROBOT_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "GameLogic.h"

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

private:
	Player *m_g_myself;
    Room *m_g_myselfRoom;

	CGameLogic m_GameLogic;


	int64				m_nUserGold[IDD_MAX_ROOM_USERCOUNT];		//ӵ�г���
	int64				m_nBasicScore;								//������ע

	int64				m_nTurnAddGold[IDD_MAX_ROOM_USERCOUNT];		//��ע

	WORD				m_wHandCard[IDD_HAND_COUNT];				//����

	int64				m_nTurnBasicAdd;							//�������ټ�ע
	int64				m_nTurnBasicGold;							//�������ٸ�ע

	//int64				m_nMaxTurnAdd;								//���������ע

	int					m_nBankUser;								//ׯ��
	int					m_nSmallBlindUser;							//Сäע
	int					m_nBigBlindUser;							//��äע
	int					m_nCurrentUser;								//��ǰ���


	WORD				m_wBoartCard[IDD_WEAVE_COUNT];				//����������
	int					m_nBoartCount;								//��������Ŀ

	WORD				m_wBestType;								//�������
	WORD				m_wBestWeave[IDD_WEAVE_COUNT];				//������

	int					m_winType;									//������Ӯ��
	int					m_nAddCount;								//��ע����

	bool				m_bIsGameOver;

	int64				m_nWinGold[IDD_MAX_ROOM_USERCOUNT][IDD_MAX_ROOM_USERCOUNT];

public:
	//ai��ע
	void OnAiAddGold();
	//
	void ClearData();
};

#endif
