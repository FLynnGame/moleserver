#include "crobotlogicframe.h"
#include "GameLogic.h"

// ��ʱ����ʶ
#define IDI_SWITCH_SCENE				  1									// �л��������ܴ���
#define IDI_EXCHANGE_FISHSCORE    2                 // �����ӵ�
#define IDI_FIRE                  3
#define IDI_STAND_UP              4

// ʱ���ʶ
#define TIME_SWITCH_SCENE				  7									// �л��������ܴ��� ���7���� �ͻ������޸� ���Ӧ��ҲҪ�޸�
#define TIME_EXCHANGE_FISHSCORE   2
#define TIME_FIRE                 2											// ���ﲻ�ܸĳ�1 �������ѭ��, �����Ҫ�ӿ��ӵ������ٶ�ֻ�ܸ�ƽ̨.

/// ���캯��
CRobotLogicFrame::CRobotLogicFrame() :m_g_myself(NULL), m_g_myselfRoom(NULL)
{
  exchange_ratio_userscore_ = 1;
  exchange_ratio_fishscore_ = 1;
  exchange_count_ = 1;
  exchange_times_ = 1;
  allow_fire_ = false;
  current_bullet_kind_ = BULLET_KIND_2_NORMAL;
  current_bullet_mulriple_ = 10;
  exchange_fish_score_ = 0;
  fish_score_ = 0;
  last_fire_angle_ = 0.f;
  min_bullet_multiple_ = 1;
  max_bullet_multiple_ = 9900;
}

/// ��������
CRobotLogicFrame::~CRobotLogicFrame()
{

}

/// ���ڴ����û�׼�������Ϣ
void CRobotLogicFrame::OnProcessPlayerReadyMes(int playerId)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;
}

/// ���ڴ����û���ʼ��Ϸ��ʼ��Ϣ
void CRobotLogicFrame::OnProcessPlayerGameStartMes()
{

}

/// ���ڴ����û�������Ϸ��Ϣ
void CRobotLogicFrame::OnProcessPlayerGameOverMes(void)
{

}

/// ���ڴ����û�������Ϸ��������Ϣ
void CRobotLogicFrame::OnProcessPlayerRoomMes(Json::Value &mes)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

}

/// �����û����뷿����Ϣ
void CRobotLogicFrame::OnProcessEnterRoomMsg(int playerId)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

}

/// �����û��뿪������Ϣ
void CRobotLogicFrame::OnProcessLeaveRoomMsg(int playerId)
{

}

/// �����û�������Ϣ
void CRobotLogicFrame::OnProcessOfflineRoomMes(int playerId)
{

}

/// �����û�����������Ϣ
void CRobotLogicFrame::OnProcessReEnterRoomMes(int playerId)
{

}

/// �����û���ʱ����Ϣ
void CRobotLogicFrame::OnProcessTimerMsg(int timerId,int curTimer)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
