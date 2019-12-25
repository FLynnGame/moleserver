#include "crobotlogicframe.h"
#include "GameLogic.h"

/// ���캯��
CRobotLogicFrame::CRobotLogicFrame() :m_g_myself(NULL), m_g_myselfRoom(NULL)
{
	//��ʼ���㷨��
	CMahJongAlg *alg = new CMahJongAlg(MahJongColor_Wan|MahJongColor_Tiao|MahJongColor_Bing|MahJongColor_Word);
	m_pGameLogic = new CMahJongLogic();
	m_pGameLogic->SetMahJongAlg(alg);
	m_pGameLogic->SetRequiredCheck(false);
}

/// ��������
CRobotLogicFrame::~CRobotLogicFrame()
{
	if (m_pGameLogic)
	{
		delete m_pGameLogic;
		m_pGameLogic = 0;
	}
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
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;
}

/// ���ڴ����û�������Ϸ��Ϣ
void CRobotLogicFrame::OnProcessPlayerGameOverMes(void)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

	//��ʼ����
	int nElapse = m_pGameLogic->GetRandomNum(MIN_TIME+2, MAX_TIME+1);
	m_g_myself->StartTimer(TIMER_ROBOT_READY,nElapse);
}

/// ���ڴ����û�������Ϸ��������Ϣ
void CRobotLogicFrame::OnProcessPlayerRoomMes(Json::Value &mes)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

	int iMsgID = mes["MsgSubId"].asInt();

	switch(iMsgID)
	{
	case ID_S_GAMEMSG_START:
		{
			int iChairID = m_g_myself->GetChairIndex();
			if (m_iOperUser == iChairID)
			{
				if (m_BankerOper & Operation_Hu)
				{
					m_BankerOper = 0;
					int iTimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
					m_g_myself->StartTimer(TIMER_ROBOT_HU, iTimeSpace);
				}
				else if(m_BankerOper & Operation_Gang)
				{
					m_BankerOper = 0;
					iOutMJ = m_GangMJ;
					enGangType = Operation_Gang_Type_An;
					int itimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
					m_g_myself->StartTimer(TIMER_ROBOT_GANG, itimeSpace);
				}
				else
				{
					int itimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, 5);
					m_g_myself->StartTimer(TIMER_ROBOT_OUT, itimeSpace);
				}
			}
		}
		break;
	case ID_S_GAMEMSG_DEALMJ:
		{
			ReSet();
			CMD_S_DealMJ cmdsDealMJ;
			memset(&cmdsDealMJ, 0, sizeof(CMD_S_DealMJ));
			CmdHelper::DeCodeMessage(cmdsDealMJ, mes);
			OnDealMJ(cmdsDealMJ);
		}
		break;
	case ID_S_GAMEMSG_OUT:
		{
			CMD_S_OutMJ cmdsOutMJ;
			memset(&cmdsOutMJ, 0, sizeof(CMD_S_OutMJ));
			CmdHelper::DeCodeMessage(cmdsOutMJ, mes);
			OnOutMJ(cmdsOutMJ);
		}
		break;
	case ID_S_GAMEMSG_TOUCH:
		{
			CMD_S_TouchMJ cmdsTouchMJ;
			memset(&cmdsTouchMJ, 0, sizeof(CMD_S_TouchMJ));
			CmdHelper::DeCodeMessage(cmdsTouchMJ, mes);
			OnTouchMJ(cmdsTouchMJ);
		}
		break;
	case ID_S_GAMEMSG_PENG:
		{
			CMD_S_PengMJ cmdsPeng;
			memset(&cmdsPeng, 0, sizeof(CMD_S_PengMJ));
			CmdHelper::DeCodeMessage(cmdsPeng, mes);
			OnPengMJ(cmdsPeng);
		}
		break;
	case ID_S_GAMEMSG_GANG:
		{
			CMD_S_GangMJ cmdsGang;
			memset(&cmdsGang, 0, sizeof(CMD_S_GangMJ));
			CmdHelper::DeCodeMessage(cmdsGang, mes);
			OnGangMJ(cmdsGang);
		}
		break;
	case ID_S_GAMEMSG_GIVEUP:
		{
			CMD_S_GiveUp cmdsGiveUp;
			memset(&cmdsGiveUp, 0, sizeof(CMD_S_GiveUp));
			CmdHelper::DeCodeMessage(cmdsGiveUp, mes);
			OnGiveUp(cmdsGiveUp);
		}
		break;
	case ID_S_GAMEMSG_HU:
		{
			CMD_S_HU cmdsHu;
			memset(&cmdsHu, 0, sizeof(CMD_S_HU));
			CmdHelper::DeCodeMessage(cmdsHu, mes);
			OnHu(cmdsHu);
		}
		break;
	case ID_S_GAMEMSG_GAMEOVER:
		{
			//��Ϸ�������ر����ж�ʱ��
			m_g_myself->StopAllTimer();
		}
	case ID_S_GAMEMSG_CHANGETABLE:
		{
		}
		break;
	}
}

/// �����û����뷿����Ϣ
void CRobotLogicFrame::OnProcessEnterRoomMsg(int playerId)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

	if(playerId == m_g_myself->GetChairIndex())
	{
		m_g_myself->SendReadyMsg();
	}
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

	if (curTimer)
	{
		return;
	}

	switch(timerId)
	{
	case TIMER_ROBOT_START:
		{
			m_g_myself->StopTimer(TIMER_ROBOT_START);
            Json::Value msgStart;
            msgStart["MsgId"] = IDD_MESSAGE_ROOM;
            msgStart["MsgSubId"] = ID_C_GAMEMSG_READY;
			m_g_myself->SendGameMsg(msgStart);
		}
		break;
	case TIMER_ROBOT_OUT:
		{
			m_g_myself->StopTimer(TIMER_ROBOT_OUT);
			CMD_C_OutMJ cmdcOutMJ;
			memset(&cmdcOutMJ, 0, sizeof(CMD_C_OutMJ));
			cmdcOutMJ.bOutMJ = GetUselessMj();

            Json::Value msgOut;
            msgOut["MsgId"] = IDD_MESSAGE_ROOM;
            msgOut["MsgSubId"] = ID_C_GAMEMSG_OUT;
			CmdHelper::CodeMessage(cmdcOutMJ, msgOut);
			m_g_myself->SendGameMsg(msgOut);
		}
		break;
	case TIMER_ROBOT_HU:
		{
			m_g_myself->StopTimer(TIMER_ROBOT_HU);

            Json::Value msgOut;
            msgOut["MsgId"] = IDD_MESSAGE_ROOM;
            msgOut["MsgSubId"] = ID_C_GAMEMSG_HU;
			m_g_myself->SendGameMsg(msgOut);
		}
		break;
	case TIMER_ROBOT_PENG:
		{
			m_g_myself->StopTimer(TIMER_ROBOT_PENG);

            Json::Value msgOut;
            msgOut["MsgId"] = IDD_MESSAGE_ROOM;
            msgOut["MsgSubId"] = ID_C_GAMEMSG_PENG;
			m_g_myself->SendGameMsg(msgOut);
		}
		break;
	case TIMER_ROBOT_GANG:
		{
			m_g_myself->StopTimer(TIMER_ROBOT_GANG);

			CMD_C_GangMJ cmdcGangMJ;
			memset(&cmdcGangMJ, 0, sizeof(CMD_C_GangMJ));
			cmdcGangMJ.bGangMJ = iOutMJ;
			cmdcGangMJ.bGangType = enGangType;

            Json::Value msgOut;
            msgOut["MsgId"] = IDD_MESSAGE_ROOM;
            msgOut["MsgSubId"] = ID_C_GAMEMSG_GANG;
			CmdHelper::CodeMessage(cmdcGangMJ, msgOut);
			m_g_myself->SendGameMsg(msgOut);
		}
		break;
	case TIMER_ROBOT_GIVEUP:
		{
			m_g_myself->StopTimer(TIMER_ROBOT_GIVEUP);

            Json::Value msgOut;
            msgOut["MsgId"] = IDD_MESSAGE_ROOM;
            msgOut["MsgSubId"] = ID_C_GAMEMSG_GIVEUP;
			m_g_myself->SendGameMsg(msgOut);
		}
		break;
	case TIMER_ROBOT_READY:
		{
			m_g_myself->StopTimer(TIMER_ROBOT_READY);
			m_g_myself->SendReadyMsg();
		}
		break;
	}
}

///������Ϣ
void CRobotLogicFrame::OnPengMJ(CMD_S_PengMJ cmdsPengMJ)
{
	assert(m_g_myself != NULL);
	if (m_g_myself == NULL)return;

	m_g_myself->StopAllTimer();

	m_iOperUser = cmdsPengMJ.wCurrentUser;
	int iOutUser = cmdsPengMJ.wLastOutUser;
	MJBYTE bPengMJ = cmdsPengMJ.bMJPeng;

	bool bSuccess = m_pGameLogic->Peng(m_bHUMJ[m_iOperUser], m_iHUMJCount[m_iOperUser], m_bHandMJ[m_iOperUser], m_iHandMJCount[m_iOperUser], m_bOutMJ[iOutUser], m_iOutMJCount[iOutUser], bPengMJ);
	if (bSuccess)
	{
		//����Գɹ�����ô׼�����һ����
		if (m_iOperUser == m_g_myself->GetChairIndex())
		{
			int iTimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
			m_g_myself->StartTimer(TIMER_ROBOT_OUT, iTimeSpace);
		}
	}
	return;
}

///������Ϣ
void CRobotLogicFrame::OnGangMJ(CMD_S_GangMJ cmdsGangMJ)
{
	assert(m_g_myself != NULL);
	if (m_g_myself == NULL)return;

	m_g_myself->StopAllTimer();

	m_iOperUser = cmdsGangMJ.wCurrentUser;
	MJBYTE bGangMJ = cmdsGangMJ.bMJGang;

	bool bSuccess = m_pGameLogic->Gang(m_bHUMJ[m_iOperUser], m_iHUMJCount[m_iOperUser], m_bHandMJ[m_iOperUser], m_iHandMJCount[m_iOperUser], m_bOutMJ[m_iOldOperUser], m_iOutMJCount[m_iOldOperUser], bGangMJ, (enOperationGangType)cmdsGangMJ.bGangType);
	if (bSuccess)
	{
		//����ܳɹ����Զ���һ���ƣ���ô׼�����һ����
		if (m_iOperUser == m_g_myself->GetChairIndex())
		{
			if (m_iHandMJCount[m_iOperUser] % 3 == 2)
			{
				int iTimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
				m_g_myself->StartTimer(TIMER_ROBOT_OUT, iTimeSpace);
			}
		}
	}
	return;
}

///������Ϣ
void CRobotLogicFrame::OnTouchMJ(CMD_S_TouchMJ cmdsTouchMJ)
{
	assert(m_g_myself != NULL);
	if (m_g_myself == NULL)return;

	m_g_myself->StopAllTimer();

	int iChairID = m_g_myself->GetChairIndex();
	m_iOperUser = cmdsTouchMJ.wCurrentUser;
	MJBYTE bTouchMJ = cmdsTouchMJ.bTouchMJ;
	m_iHandMJCount[m_iOperUser]++;

	if (m_iOperUser == iChairID && m_iHandMJCount[iChairID]%3==2)
	{
		m_bHandMJ[iChairID][m_iHandMJCount[iChairID]-1] = bTouchMJ;
		if (cmdsTouchMJ.bOperation & Operation_Hu)
		{
			int iTimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
			m_g_myself->StartTimer(TIMER_ROBOT_HU, iTimeSpace);
		}
		else if (cmdsTouchMJ.bOperation & Operation_Gang)
		{
			iOutMJ = cmdsTouchMJ.bMJGang[0];
			enGangType = (enOperationGangType)cmdsTouchMJ.bGangType;
			int iTimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
			m_g_myself->StartTimer(TIMER_ROBOT_GANG, iTimeSpace);
		}
		else
		{
			int iTimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
			m_g_myself->StartTimer(TIMER_ROBOT_OUT, iTimeSpace);
		}
	}
	return;
}

///������Ϣ
void CRobotLogicFrame::OnDealMJ(CMD_S_DealMJ cmdsDealMJ)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

	m_g_myself->StopAllTimer();

	int iChairID = m_g_myself->GetChairIndex();
	m_iOperUser = cmdsDealMJ.iBankerUser;

	//��������
	memcpy(m_bHandMJ[iChairID], cmdsDealMJ.bHandMJ, sizeof(MJBYTE)*MAX_HAND_MJ);

	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (i == m_iOperUser)
		{
			m_iHandMJCount[i] = MAX_HAND_MJ;
		}
		else
		{
			m_iHandMJCount[i] = MAX_HAND_MJ-1;
		}
	}
	int itimeSpace = (TIME_Dice*2+TIME_BuildWall+TIME_Deal+TIME_ServerDelay)/1000;
	m_g_myself->StartTimer(TIMER_ROBOT_START, itimeSpace);

	//����в�������ôִ�в���
	//MJFLAG tmpOperation = cmdsDealMJ.bOperation;
	m_BankerOper = cmdsDealMJ.bOperation;
	m_GangMJ = cmdsDealMJ.bMJGang[0];
	return;
}

///������Ϣ
void CRobotLogicFrame::OnOutMJ(CMD_S_OutMJ cmdsOutMJ)
{
	m_g_myself->StopAllTimer();

	int iOutMJUser = cmdsOutMJ.wOutMJUser;
	int myChairID = m_g_myself->GetChairIndex();

	m_iOldOperUser = m_iOperUser;
	m_iOperUser = (m_iOperUser+1)%GAME_PLAYER;

	MJBYTE iTmpOutMJ = cmdsOutMJ.bOutMJ;
	iOutMJ = cmdsOutMJ.bOutMJ;

	//��¼���ֵ���
	int iIndex = MASK_MAHJONGKIND(iTmpOutMJ)*10+MASK_MAHJONGNUM(iTmpOutMJ);
	assert(iIndex>0);
	m_bExistMjCount[iIndex]++;

	bool bSuccess = m_pGameLogic->OutMJ(m_bHandMJ[iOutMJUser], m_iHandMJCount[iOutMJUser], m_bOutMJ[iOutMJUser], m_iOutMJCount[iOutMJUser], iTmpOutMJ);
	if (bSuccess)
	{
		//����в�������ôֱ��ִ�в�����
		if (cmdsOutMJ.bOperation)
		{
			//����к�����ô��
			if (cmdsOutMJ.bOperation & Operation_Hu)
			{
				int iTimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
				m_g_myself->StartTimer(TIMER_ROBOT_HU, iTimeSpace);
			}
			//����иܣ���ô��
			else if (cmdsOutMJ.bOperation & Operation_Gang)
			{
				enGangType = Operation_Gang_Type_Ming;
				iOutMJ = iTmpOutMJ;
				int iTimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
				m_g_myself->StartTimer(TIMER_ROBOT_GANG, iTimeSpace);
			}
			else if (cmdsOutMJ.bOperation & Operation_Peng)
			{
				int iTimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
				m_g_myself->StartTimer(TIMER_ROBOT_PENG, iTimeSpace);
			}
			else
			{
				int itimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
				m_g_myself->StartTimer(TIMER_ROBOT_OUT, itimeSpace);
			}
		}
	}
	return;
}


///������Ϣ
void CRobotLogicFrame::OnGiveUp(CMD_S_GiveUp cmdsGiveUp)
{
	m_g_myself->StopAllTimer();

	//����в�������ôֱ��ִ�в�����
	if (cmdsGiveUp.bOperation)
	{
		//����к�����ô��
		if (cmdsGiveUp.bOperation & Operation_Hu)
		{
			int iTimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
			m_g_myself->StartTimer(TIMER_ROBOT_HU, iTimeSpace);
		}
		//����иܣ���ô��
		else if (cmdsGiveUp.bOperation & Operation_Gang)
		{
			enGangType = Operation_Gang_Type_Ming;
			iOutMJ = cmdsGiveUp.bOutMJ;
			int iTimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
			m_g_myself->StartTimer(TIMER_ROBOT_GANG, iTimeSpace);
		}
		//�����������ô��
		else if (cmdsGiveUp.bOperation & Operation_Peng)
		{
			int iTimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
			m_g_myself->StartTimer(TIMER_ROBOT_PENG, iTimeSpace);
		}
		else
		{
			if (m_iOperUser == m_g_myself->GetChairIndex())
			{
				int itimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
				m_g_myself->StartTimer(TIMER_ROBOT_OUT, itimeSpace);
			}
		}
	}
	return;
}

void CRobotLogicFrame::OnHu(CMD_S_HU cmdsHu)
{
	m_g_myself->StopAllTimer();
	//����в�������ôֱ��ִ�в�����
	if (cmdsHu.bOperation)
	{
		//����к�����ô��
		if (cmdsHu.bOperation & Operation_Hu)
		{
			int iTimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
			m_g_myself->StartTimer(TIMER_ROBOT_HU, iTimeSpace);
		}
		//����иܣ���ô��
		else if (cmdsHu.bOperation & Operation_Gang)
		{
			enGangType = Operation_Gang_Type_Ming;
			iOutMJ = cmdsHu.bUserHuMJ;
			int iTimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
			m_g_myself->StartTimer(TIMER_ROBOT_GANG, iTimeSpace);
		}
		//�����������ô��
		else if (cmdsHu.bOperation & Operation_Peng)
		{
			int iTimeSpace = m_pGameLogic->GetRandomNum(MIN_TIME, MAX_TIME);
			m_g_myself->StartTimer(TIMER_ROBOT_PENG, iTimeSpace);
		}
	}
	return;
}

void CRobotLogicFrame::ReSet()
{
	memset(m_bHUMJ, 0, sizeof(MJBlock)*GAME_PLAYER*MAX_BLOCK_COUNT);
	memset(m_iHUMJCount, 0, sizeof(int)*GAME_PLAYER);
	memset(m_bHandMJ, 0, sizeof(MJBYTE)*GAME_PLAYER*MAX_HAND_MJ);
	memset(m_iHandMJCount,0, sizeof(int)*GAME_PLAYER);
	memset(m_bOutMJ, 0, sizeof(MJBYTE)*GAME_PLAYER*MAX_OUT_MJ);
	memset(m_iOutMJCount, 0, sizeof(int)*GAME_PLAYER);

	memset(m_bExistMjCount, 0 ,sizeof(MJBYTE)*MAX_ARRAY_MJ);

	enGangType = Operation_Gang_Type_Null;

	iOutMJ = 0;

	m_iOldOperUser = ChairLoc_Invalid;
	m_iOperUser = ChairLoc_Invalid;

	m_iSeOut = -1;
	m_BankerOper = 0;
	m_GangMJ = 0;
}

//�������߼����ҳ�һ�������ƴ��ȥ
MJBYTE CRobotLogicFrame::GetUselessMj()
{
	WORD wMeChairID = m_g_myself->GetChairIndex();

	MJBYTE bOutMj = INVALID_BYTE;
	MjWeight mjWeight[MAX_HAND_MJ];
	int iMjWeightCount = 0;
	memset(mjWeight, 0, sizeof(mjWeight));

	if (m_iSeOut == -1)
	{
		int iTiaoSeCount = 0; int iWanSeCount = 0; int iTongSeCount = 0;
		for (int i = 0; i < m_iHUMJCount[wMeChairID]; ++i)
		{
			if (m_bHUMJ[wMeChairID][i].style == BlockStyle_Peng)
			{
				if (MASK_MAHJONGKIND(m_bHUMJ[wMeChairID][i].first) == 0)
				{
					iWanSeCount += 3;
				}
				else if (MASK_MAHJONGKIND(m_bHUMJ[wMeChairID][i].first) == 0x01)
				{
					iTiaoSeCount += 3;
				}
				else if (MASK_MAHJONGKIND(m_bHUMJ[wMeChairID][i].first) == 0x02)
				{
					iTongSeCount += 3;
				}
			}
			else if (m_bHUMJ[wMeChairID][i].style >= BlockStyle_Gang_Ming)
			{
				if (MASK_MAHJONGKIND(m_bHUMJ[wMeChairID][i].first) == 0)
				{
					iWanSeCount += 4;
				}
				else if (MASK_MAHJONGKIND(m_bHUMJ[wMeChairID][i].first) == 0x01)
				{
					iTiaoSeCount += 4;
				}
				else if (MASK_MAHJONGKIND(m_bHUMJ[wMeChairID][i].first) == 0x02)
				{
					iTongSeCount += 4;
				}
			}
		}
		for (int i = 0; i < m_iHandMJCount[wMeChairID]; ++i)
		{
			if (MASK_MAHJONGKIND(m_bHandMJ[wMeChairID][i]) == 0)
			{
				iWanSeCount += 1;
			}
			else if (MASK_MAHJONGKIND(m_bHandMJ[wMeChairID][i]) == 0x01)
			{
				iTiaoSeCount += 1;
			}
			else if (MASK_MAHJONGKIND(m_bHandMJ[wMeChairID][i]) == 0x02)
			{
				iTongSeCount += 1;
			}
		}
		for (int i = 20; i >= 0; --i)
		{
			iWanSeCount--;
			iTiaoSeCount--;
			iTongSeCount--;
			if (iWanSeCount == 0)
			{
				m_iSeOut = 0;
				break;
			}
			else if (iTiaoSeCount == 0)
			{
				m_iSeOut = 0x01;
				break;
			}
			else if (iTongSeCount == 0)
			{
				m_iSeOut = 0x02;
				break;
			}
		}
	}

	for (int i = 0; i < m_iHandMJCount[wMeChairID]; ++i)
	{
		if (MASK_MAHJONGKIND(m_bHandMJ[wMeChairID][i]) == m_iSeOut)
		{
			bOutMj = m_bHandMJ[wMeChairID][i];
			return bOutMj;
		}
	}

	//����������ɫ֮�ڣ�ѡһ�������õ���(û�гɶԻ�û������)���
	for (int i=0; i < m_iHandMJCount[wMeChairID]; i++)
	{
		MJBYTE bMj = m_bHandMJ[wMeChairID][i];
		if ((i>0 && bMj == m_bHandMJ[wMeChairID][i-1]) || (iMjWeightCount>0 && bMj==mjWeight[iMjWeightCount-1].bLessMj))
		{
			continue;
		}
		int iMjCount = m_pGameLogic->GetMJCount(m_bHandMJ[wMeChairID], m_iHandMJCount[wMeChairID], bMj);
		if (iMjCount >= 3)
		{
			//��һ������
			continue;
		}

		int iMjLeftCount = 0, iMjLeft2Count = 0;
		int iMjRightCount = 0, iMjRight2Count = 0;
		if (MASK_MAHJONGNUM(bMj) > 1)
		{
			iMjLeftCount = m_pGameLogic->GetMJCount(m_bHandMJ[wMeChairID], m_iHandMJCount[wMeChairID], bMj-1);
			if (MASK_MAHJONGNUM(bMj) > 2)
			{
				iMjLeft2Count = m_pGameLogic->GetMJCount(m_bHandMJ[wMeChairID], m_iHandMJCount[wMeChairID], bMj-2);
			}
		}
		if (MASK_MAHJONGNUM(bMj) < 9)
		{
			iMjRightCount = m_pGameLogic->GetMJCount(m_bHandMJ[wMeChairID], m_iHandMJCount[wMeChairID], bMj+1);
			if (MASK_MAHJONGNUM(bMj) < 8)
			{
				iMjRight2Count = m_pGameLogic->GetMJCount(m_bHandMJ[wMeChairID], m_iHandMJCount[wMeChairID], bMj+2);
			}
		}
		if (iMjCount == 1 && iMjLeftCount == 0 && iMjRightCount == 0 && iMjLeft2Count == 0 && iMjRight2Count == 0)
		{
			//�ǵ��ƣ��ͳ�����
			bOutMj = bMj;
			break;
		}
		if ((iMjLeftCount > 0 && iMjRightCount > 0) || (iMjLeft2Count > 0 && iMjLeftCount > 0) || (iMjRightCount > 0 && iMjRight2Count > 0))
		{
			//��һ��˳��
			continue;
		}

		mjWeight[iMjWeightCount].bLessMj = bMj;

		//�ճɶ��ӵĸ���
		mjWeight[iMjWeightCount].bMinCount += GetRestMjCount(bMj);

		//�ճ�˳�ӵĸ��ʣ�3��˳����С����֮��
		int iMinCount = 0;
		//*00
		if (MASK_MAHJONGNUM(bMj) < 8)
		{
			int iLeftCount = GetRestMjCount(bMj+1);
			int iLeft2Count = GetRestMjCount(bMj+2);
			iMinCount += (iLeftCount < iLeft2Count ? iLeftCount : iLeft2Count);
		}
		//0*0
		if (MASK_MAHJONGNUM(bMj) < 9 && MASK_MAHJONGNUM(bMj) > 1)
		{
			int iLeftCount = GetRestMjCount(bMj-1);
			int iRightCount = GetRestMjCount(bMj+1);
			iMinCount += (iLeftCount < iRightCount ? iLeftCount : iRightCount);
		}
		//00*
		if (MASK_MAHJONGNUM(bMj) > 2)
		{
			int iLeftCount = GetRestMjCount(bMj-1);
			int iLeft2Count = GetRestMjCount(bMj-2);
			iMinCount += (iLeftCount < iLeft2Count ? iLeftCount : iLeft2Count);
		}

		mjWeight[iMjWeightCount].bMinCount += iMinCount;
		iMjWeightCount++;
	}
	if (bOutMj == INVALID_BYTE)
	{
		if (iMjWeightCount > 0)
		{
			//ȡȨ����С��
			int iMinIndex = 0;
			for (int k = 1; k < iMjWeightCount; k++)
			{
				if (mjWeight[k].bMinCount < mjWeight[iMinIndex].bMinCount)
				{
					iMinIndex = k;
				}
			}
			bOutMj = mjWeight[iMinIndex].bLessMj;
		}
		else
		{
			//�����һ�Ű�
			bOutMj = m_bHandMJ[wMeChairID][m_iHandMJCount[wMeChairID]-1];
		}
	}
	return bOutMj;
}

int CRobotLogicFrame::GetRestMjCount(MJBYTE bMj)
{
	WORD wMeChairID = m_g_myself->GetChairIndex();
	int iIndex = MASK_MAHJONGKIND(bMj) * 10 + MASK_MAHJONGNUM(bMj);
	assert(iIndex>0);
	return (4 - m_bExistMjCount[iIndex] - m_pGameLogic->GetMJCount(m_bHandMJ[wMeChairID], m_iHandMJCount[wMeChairID], bMj));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
