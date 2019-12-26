#include "CBasicMatchingMode.h"

#include <algorithm>

#include "../GameFrameManager.h"
#include "../gameframe/PlayerManager.h"
#include "../gameframe/RoomManager.h"

initialiseSingleton(CTabelFrameManager);

//////////////////////////////////////////////////////////////////////////

/**
 * ���캯��
 */
CTabelFrameManager::CTabelFrameManager()
	: m_matchMode(MATCHMODE_NULL),m_MaxStartPlayerCount(0),m_MaxMatchingLoopCount(0),m_BasicMatchingMode(NULL)
{

}

/**
 * ��������
 */
CTabelFrameManager::~CTabelFrameManager()
{
	if(m_BasicMatchingMode) SafeDelete(m_BasicMatchingMode);
	m_BasicMatchingMode = NULL;
}

/// �����������
void CTabelFrameManager::Clear(void)
{
	//ASSERT(m_BasicMatchingMode != NULL);
	if(m_BasicMatchingMode == NULL) return;

	m_BasicMatchingMode->Clear();

	if(m_BasicMatchingMode) SafeDelete(m_BasicMatchingMode);
	m_BasicMatchingMode = NULL;
}

/// ���ָ��������Ƿ��ڱ�����
bool CTabelFrameManager::IsExist(CPlayer *pPlayer)
{
	assert(m_BasicMatchingMode != NULL);
	if(m_BasicMatchingMode == NULL) return false;

	return m_BasicMatchingMode->IsExist(pPlayer);
}

/// �ӱ�������ɾ�����
bool CTabelFrameManager::DeletePlayer(CPlayer *pPlayer,bool isMatching)
{
	assert(m_BasicMatchingMode != NULL);
	if(m_BasicMatchingMode == NULL) return false;

	return m_BasicMatchingMode->DeletePlayer(pPlayer,isMatching);
}

/// ��ӱ����õ�����
bool CTabelFrameManager::AddRoom(CRoom *pRoom)
{
	assert(m_BasicMatchingMode != NULL);
	if(m_BasicMatchingMode == NULL) return false;

	return m_BasicMatchingMode->AddRoom(pRoom);
}

/// �����ҵ���������
bool CTabelFrameManager::AddPlayer(CPlayer *pPlayer)
{
	assert(m_BasicMatchingMode != NULL);
	if(m_BasicMatchingMode == NULL) return false;

	return m_BasicMatchingMode->AddPlayer(pPlayer);
}

/// �õ�������������
int CTabelFrameManager::GetMatchPlayerCount(void)
{
	assert(m_BasicMatchingMode != NULL);
	if(m_BasicMatchingMode == NULL) return false;

	return m_BasicMatchingMode->GetMatchPlayerCount();
}

/// ��ȡָ����ұ���������
void CTabelFrameManager::GetPlayerRanking(CPlayer *pPlayer)
{
	assert(m_BasicMatchingMode != NULL);
	if(m_BasicMatchingMode == NULL) return;

	return m_BasicMatchingMode->GetPlayerRanking(pPlayer);
}

/// ����������
void CTabelFrameManager::Update(void)
{
	assert(m_BasicMatchingMode != NULL);
	if(m_BasicMatchingMode == NULL) return;

	return m_BasicMatchingMode->Update();
}

