#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "GameLogic.h"
#include <map>
#include <vector>

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

private:
	Room *m_g_GameRoom;                                  /**< ��Ϸ���� */
	CGameLogic					m_GameLogic;										//��Ϸ�߼�


	std::map<uint32,std::string>	m_sUserName;


	int							m_nCurrentUser;				 						//��ǰ���
	int							m_nBankUser;										//ׯ��
	int							m_nSmallBlindUser;									//Сäע���
	int							m_nBigBlindUser;									//��äע���

	int							m_nAddCount;										//��ע����

	int64						m_nBasicScore;										//������ע
	int64						m_nTurnBasicGold;									//������ע������ע
	int64						m_nTurnBasicAdd;									//������ע���ټ�ע
	int64						m_nTurnAddGold[IDD_MAX_ROOM_USERCOUNT];				//���������ע
	int64						m_nAddGold[IDD_MAX_ROOM_USERCOUNT];					//��Ҽ�ע
	int64						m_nUserGold[IDD_MAX_ROOM_USERCOUNT];				//��ҳ���
	int64						m_nTurnStartGold[IDD_MAX_ROOM_USERCOUNT];


	bool						m_bPlayStates[IDD_MAX_ROOM_USERCOUNT];				//���״̬
	bool						m_bAddStates[IDD_MAX_ROOM_USERCOUNT];				//��ע״̬
	int							m_nAllPlayUserCount;								//������ע�����

	OperateType					m_UserOperate[IDD_MAX_ROOM_USERCOUNT];


	int64						m_nAddList[IDD_MAX_ROOM_USERCOUNT];					//��ע��Ŀ�б�
	int							m_nAddUserCount;									//��ע�����Ŀ
	int64						m_nPot[IDD_MAX_ROOM_USERCOUNT];						//�ʳس������
	int							m_nPotCount;										//�ʳ���Ŀ


	//��������Ϣ
	WORD						m_wCardData[IDD_MAX_CARD_COUNT];					//�ƶ�
	int							m_nSendCardPos;										//�����±�

	WORD						m_wHandCard[IDD_MAX_ROOM_USERCOUNT][IDD_HAND_COUNT];		//��������
	WORD						m_wBoardData[IDD_WEAVE_COUNT];						//����������
	int							m_nBoardCount;										//��������Ŀ

	//int64						m_nMaxTurnAdd;

	GameEndType					m_endType;



	//�û���ע
	void OnUserAddGold(int playerId, int64 nAddGold);
	//�û�����
	void OnUserGiveUp(int playerId);
	//��Ϸ����
	void OnGameOver(GameEndType endType);


	//��ע����
	bool isAddOver();
	//�ʳ�ͳ��
	void StatisticsPot();
	//��äע
	void Blind();
	//������
	void DealHandCard();
	//��������
	void DealBoardCard(int dealCardCount);
	//��һ�����
	int GetNextPlayer(int playerId);
	//�������
	void ClearData();
	//��ȡ���������ע
	int64 GetMaxTurnAddd();

#ifdef _DEBUG
	bool ForTest();
#endif
};

#endif
