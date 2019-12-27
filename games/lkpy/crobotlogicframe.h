#ifndef _C_ROBOT_LOGIC_FRAME_H_INCLUDE_
#define _C_ROBOT_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"

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

	// ��Ϣ����
protected:
  bool OnSubGameStatus(void* data, WORD data_size);
  bool OnSubGameConfig(void* data, WORD data_size);
  bool OnSubExchangeFishScore(void* data, WORD data_size);
  bool OnSubUserFire(void* data, WORD data_size);
  bool OnSubCatchFish(void* data, WORD data_size);
  bool OnSubBulletIonTimeout(void* data, WORD data_size);
  bool OnSubCatSweepFishResult(void* data, WORD data_size);
  bool OnSubSwitchScene(void* data, WORD data_size);

private:
  void ExchangeFishScore();
  void Fire();
  float GetAngle(WORD chair_id);

private:
	Player *m_g_myself;
    Room *m_g_myselfRoom;

private:
  int exchange_ratio_userscore_;
  int exchange_ratio_fishscore_;
  int exchange_count_;

  int min_bullet_multiple_;
  int max_bullet_multiple_;

  BulletKind current_bullet_kind_;
  int current_bullet_mulriple_;
  bool allow_fire_;

  SCORE exchange_fish_score_;
  SCORE fish_score_;

  float last_fire_angle_;
  int exchange_times_;
};

#endif
