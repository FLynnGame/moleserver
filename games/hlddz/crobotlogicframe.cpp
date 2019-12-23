#include "crobotlogicframe.h"

//��Ϸʱ��
#ifdef _DEBUG
#define TIME_OUT_CARD				3									//����ʱ��
#else
#define TIME_OUT_CARD				3									//����ʱ��
#endif
#define TIME_START_GAME				5									//��ʼʱ��
#define TIME_CALL_SCORE				3									//�з�ʱ��

//����ʱ��
#define TIME_LESS					3									//����ʱ��
#define TIME_DISPATCH				5									//����ʱ��

//��Ϸʱ��
#define IDI_OUT_CARD				(IDD_TIMER_GAME_START+30)			//����ʱ��
#define IDI_START_GAME				(IDD_TIMER_GAME_START+31)			//��ʼʱ��
#define IDI_CALL_SCORE				(IDD_TIMER_GAME_START+32)			//�з�ʱ��

/// ���캯��
CRobotLogicFrame::CRobotLogicFrame() :m_g_myself(NULL), m_g_myselfRoom(NULL)
{
	//�˿˱���
	m_cbTurnCardCount=0;
	memset(m_cbTurnCardData,0,sizeof(m_cbTurnCardData));
	m_wOutCardUser = INVALID_CHAIR ;
	m_wBankerUser = INVALID_CHAIR ;

	//�����˿�
	m_cbHandCardCount=0;
	memset(m_cbHandCardData,0,sizeof(m_cbHandCardData));
	m_cbCurrentLandScore = 4 ;

	//�зֱ���
	m_cbCallScorePhase=CSD_NORMAL;

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
	//�˿˱���
	m_cbTurnCardCount=0;
	memset(m_cbTurnCardData,0,sizeof(m_cbTurnCardData));

	//�����˿�
	m_cbHandCardCount=0;
	memset(m_cbHandCardData,0,sizeof(m_cbHandCardData));
}

/// ���ڴ����û�������Ϸ��Ϣ
void CRobotLogicFrame::OnProcessPlayerGameOverMes(void)
{
	//��ʼ����
	uint32 nElapse=rand()%TIME_START_GAME+TIME_LESS;
	m_g_myself->StartTimer(IDI_START_GAME,nElapse);
}

/// ���ڴ����û�������Ϸ��������Ϣ
void CRobotLogicFrame::OnProcessPlayerRoomMes(Json::Value &mes)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

	int subMsgID = mes["MsgSubId"].asInt();

	switch(subMsgID)
	{
	case SUB_S_SEND_CARD://����
		{
			uint16 wCurrentUser = mes["CurrentUser"].asInt();
			//��������
			uint16 bBackCard[3];
			for (int i=0;i<3;i++)
			{
				bBackCard[i]=mes["bBackCard"][i].asInt();
				m_cbBackCard[i]=m_GameLogic.OnChangeCardDate(bBackCard[i]);
			}
			//�����˿�
			uint16 cbHandCardData[3][20];
			uint8 ChangeHandCardData[3][20];
			uint8 uindex = 0;
			for ( uint16 i = 0; i < GAME_PLAYER; i++ )
			{
				for ( uint16 j = 0; j < 17; j++ )
				{
					cbHandCardData[i][j]=mes["cbHandCardData"][uindex++].asInt();
					ChangeHandCardData[i][j]=m_GameLogic.OnChangeCardDate(cbHandCardData[i][j]);
				}
				m_GameLogic.SetUserCard(i, ChangeHandCardData[i], 17) ;
			}

			m_cbCallScorePhase=CSD_NORMAL;

			//�˿˱���
			m_cbTurnCardCount=0;
			memset(m_cbTurnCardData,0,sizeof(m_cbTurnCardData));
			m_cbCurrentLandScore = 4 ;

			//�����˿�
			uint8 cbLandScoreCardData[20] ;
			memcpy(cbLandScoreCardData, ChangeHandCardData[m_g_myself->GetChairIndex()], sizeof( uint8 )*17) ;
			memcpy(cbLandScoreCardData+17, m_cbBackCard, sizeof( uint8 )*BACK_COUNT) ;
			m_GameLogic.SetLandScoreCardData(cbLandScoreCardData, 20) ;

			//�����˿�
			m_cbHandCardCount=17;
			memcpy(m_cbHandCardData, ChangeHandCardData[m_g_myself->GetChairIndex()], sizeof( uint8 )*17) ;

			//�����˿�
			m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount,ST_ORDER);

			//��Ҵ���
			if (m_g_myself->GetChairIndex()==wCurrentUser)
			{
				uint32 nElapse=rand()%TIME_CALL_SCORE+TIME_LESS;
				m_g_myself->StartTimer(IDI_CALL_SCORE,nElapse+TIME_DISPATCH);
			}
		}
		return;
	case SUB_S_LAND_SCORE://�з�
		{
			uint16 pChairId = mes["ChairId"].asInt();
			m_cbCurrentLandScore = mes["CurrentLandScore"].asInt();
			uint16 wCurrentUser = mes["CurrentUser"].asInt();
			int wShowTimes = mes["ShowTimes"].asInt();
			m_cbCallScorePhase = mes["cbCallScorePhase"].asInt();
			int cbPreCallScorePhase = mes["PreCallScorePhase"].asInt();

			//����ׯ��
		/*	m_wBankerUser = pCallScore->bLandUser ;
			m_GameLogic.SetBanker(pCallScore->bLandUser) ;*/

			//�û�����
			if (m_g_myself->GetChairIndex()==wCurrentUser && m_cbCallScorePhase!=CSD_GAMESTART)
			{
				uint32 nElapse=rand()%TIME_CALL_SCORE+TIME_LESS;
				m_g_myself->StartTimer(IDI_CALL_SCORE,nElapse);
			}
		}
		return;
	case SUB_S_GAME_START://��ʼ
		{
			uint16 wCurrentUser = mes["CurrentUser"].asInt();
			m_wBankerUser =  mes["wBankerUser"].asInt();

			if (m_cbHandCardCount>17)
			{
				return ;
			}

			//����ׯ��
			m_GameLogic.SetBanker(m_wBankerUser) ;

			////���õ���
			m_GameLogic.SetBackCard(m_wBankerUser, m_cbBackCard, BACK_COUNT) ;

			//�����˿�
			if (m_wBankerUser==m_g_myself->GetChairIndex())
			{
				//�����˿�
				m_cbHandCardCount+=BACK_COUNT;
				memcpy(&m_cbHandCardData[NORMAL_COUNT],m_cbBackCard,sizeof( uint8 )*BACK_COUNT);

				//�����˿�
				m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount,ST_ORDER);

			}

			//�������
			if (wCurrentUser==m_g_myself->GetChairIndex())
			{
				uint32 nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
				m_g_myself->StartTimer(IDI_OUT_CARD,nElapse);
			}
		}
		return;
	case SUB_S_BRIGHT_CARD://����
		{
			uint16 pChairId = mes["ChairId"].asInt();
			int wShowTimes = mes["ShowTimes"].asInt();
		}
		return;
	case SUB_S_OUT_CARD://����
		{
			uint16 pChairId = mes["ChairId"].asInt();
			int wShowTimes=mes["ShowTimes"].asInt();
			uint16 wCurrentUser=mes["CurrentUser"].asInt();
			bool bYaPaiFlag;
			bYaPaiFlag=((mes["YaPaiFlag"].asInt()==1)?true:false);
			uint16 bOutCardCount=mes["OutCardCount"].asInt();
			uint16 bOutCardData[20];
			uint8 ChangeHandCardData[20];
			for (int i=0;i<bOutCardCount;i++)
			{
				bOutCardData[i]=mes["OutCardData"][i].asInt();
				ChangeHandCardData[i]=m_GameLogic.OnChangeCardDate(bOutCardData[i]);
			}

			//���Ʊ���
			if (wCurrentUser==pChairId)
			{
				m_cbTurnCardCount=0;
				memset(m_cbTurnCardData,0,sizeof(m_cbTurnCardData));
			}
			else
			{
				m_cbTurnCardCount=bOutCardCount;
				m_wOutCardUser = pChairId;
				memcpy(m_cbTurnCardData,ChangeHandCardData,bOutCardCount*sizeof(uint8));
			}

			//�����ж�
			//int bCardType=m_GameLogic.GetCardType(m_cbTurnCardData,bOutCardCount);

			//�������
			if (m_g_myself->GetChairIndex()==wCurrentUser)
			{
				/*if (bCardType==CT_MISSILE_CARD)
				{
					m_g_myself->StartTimer(IDI_OUT_CARD,3);
				}
				else
				{*/
					uint32 nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
					m_g_myself->StartTimer(IDI_OUT_CARD,nElapse);
				//}
			}

			//���ñ���
			m_GameLogic.RemoveUserCardData(pChairId, m_cbTurnCardData, m_cbTurnCardCount) ;
		}
		return;
	case SUB_S_PASS_CARD://����
		{
			uint16 pChairId = mes["ChairId"].asInt();
			uint16 wCurrentUser=mes["CurrentUser"].asInt();
			int bNewTurn=mes["bNewTurn"].asInt();

			//һ���ж�
			if (bNewTurn==1)
			{
				m_cbTurnCardCount=0;
				memset(m_cbTurnCardData,0,sizeof(m_cbTurnCardData));
			}
			//�������
			if (m_g_myself->GetChairIndex()==wCurrentUser)
			{
				uint32 nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
				m_g_myself->StartTimer(IDI_OUT_CARD,nElapse);
			}
		}
		return;
	case SUB_S_TRUSTEE://�й�
		{
			uint16 pChairId = mes["ChairId"].asInt();
			int isAutoPlay=mes["isAutoPlay"].asInt();
		}
		return;
	case SUB_S_GAME_END://����
		{
			//���ñ���
			m_cbTurnCardCount=0;
			m_cbHandCardCount=0;
			memset(m_cbTurnCardData,0,sizeof(m_cbTurnCardData));
			memset(m_cbHandCardData,0,sizeof(m_cbHandCardData));

			//ɾ��ʱ��
			m_g_myself->StopAllTimer();
		}
		return;
	case SUB_S_MATCH_END:
		{
			//���ñ���
			m_cbTurnCardCount=0;
			m_cbHandCardCount=0;
			memset(m_cbTurnCardData,0,sizeof(m_cbTurnCardData));
			memset(m_cbHandCardData,0,sizeof(m_cbHandCardData));

			//ɾ��ʱ��
			m_g_myself->StopAllTimer();

/*			m_g_myself->SetMatchCount(m_g_myself->GetMatchCount()-1);

			if(m_g_myself->GetMatchCount() > 0)
			{
				m_g_myself->SendReadyMsg();
			}		*/
		}
		return;
	case SUB_S_LAND_SCORE_STATION://�зֽ׶ζ�������
		{
			uint16 pChairId = mes["ChairId"].asInt();
			uint16 wCurrentUser = mes["CurrentUser"].asInt();
			int wShowTimes = mes["ShowTimes"].asInt();
			int bCallScorePhase = mes["CallScorePhase"].asInt();

			//��ʾ����ҷ���
			int bScore[3];
			for (int i=0;i<GAME_PLAYER;i++)
			{
				bScore[i]=mes["bScore"][i].asInt();
			}
			//����й�
			bool bUserTrustee[3];
			for (int i=0;i<GAME_PLAYER;i++)
			{
				bUserTrustee[i]=(mes["bUserTrustee"][i].asInt()==0)?false:true;
			}
			//����
			uint16 bBackCard[3];
			for (int i=0;i<3;i++)
			{
				bBackCard[i]=mes["bBackCard"][i].asInt();
			}

			//�����������
			uint16 bHandCardData[3][20];
			uint8 ChangeHandCardData[3][20];
			uint8 uindex = 0;
			for ( uint16 i = 0; i < GAME_PLAYER; i++ )
			{
				for ( uint16 j = 0; j < 17; j++ )
				{
					bHandCardData[i][j]=mes["bHandCardData"][uindex++].asInt();
					ChangeHandCardData[i][j]=m_GameLogic.OnChangeCardDate(bHandCardData[i][j]);
				}
			}

			//�˿�����
			m_cbHandCardCount=NORMAL_COUNT;
			memcpy(m_cbHandCardData,ChangeHandCardData[m_g_myself->GetChairIndex()],sizeof(uint8)*m_cbHandCardCount);

			//�з�����
			if (m_g_myself->GetChairIndex()==wCurrentUser)
			{
				uint32 nElapse=rand()%TIME_CALL_SCORE+TIME_LESS;
				m_g_myself->StartTimer(IDI_CALL_SCORE,nElapse+TIME_DISPATCH);
			}
		}
		return;
	case SUB_S_PLAYING_STATION://���ƽ׶ζ�������
		{
			uint16 pChairId = mes["ChairId"].asInt();
			uint16 wCurrentUser=mes["wCurrentUser"].asInt();
			int wShowTimes=mes["wShowTimes"].asInt();
			uint16 wLandUser=mes["wLandUser"].asInt();
			uint16 wTurnWiner=mes["wTurnWiner"].asInt();
			bool wMatchOver=(mes["wMatchOver"].asInt()==1)?true:false;
			//����
			int bBrightCard[GAME_PLAYER];
			for (int i=0;i<GAME_PLAYER;i++)
			{
				bBrightCard[i]=mes["bBrightCard"][i].asInt();
			}
			//�й�
			bool bUserTrustee[3];
			for (int i=0;i<GAME_PLAYER;i++)
			{
				bUserTrustee[i]=(mes["bUserTrustee"][i].asInt()==0)?false:true;
			}
			//����
			bool IsPass[3];
			for (int i=0;i<GAME_PLAYER;i++)
			{
				IsPass[i]=(mes["IsPass"][i].asInt()==0)?false:true;
			}

			//�����������
			m_cbTurnCardCount=mes["cbTurnCardCount"].asInt();
			for (int i=0;i<m_cbTurnCardCount;i++)
			{
				m_cbTurnCardData[i]=mes["cbTurnCardData"][i].asInt();
			}

			//����
			uint16 bBackCard[3];
			for (int i=0;i<3;i++)
			{
				bBackCard[i]=mes["bBackCard"][i].asInt();
			}

			//�����������
			int bHandCardCount[3];
			uint16 bHandCardData[3][20];
			uint8 ChangeHandCardData[3][20];

			for ( uint16 i = 0; i < GAME_PLAYER; i++ )
			{
				bHandCardCount[i]=mes["bHandCardCount"][i].asInt();
				for ( uint16 j = 0; j < bHandCardCount[i]; j++ )
				{
					bHandCardData[i][j]=mes["bHandCardData"][i][j].asInt();
					ChangeHandCardData[i][j]=m_GameLogic.OnChangeCardDate(bHandCardData[i][j]);
				}
			}

			//�˿�����
			m_cbHandCardCount=bHandCardCount[m_g_myself->GetChairIndex()];
			memcpy(m_cbHandCardData,ChangeHandCardData[m_g_myself->GetChairIndex()],sizeof(uint8)*m_cbHandCardCount);

			//�������
			if (wCurrentUser==m_g_myself->GetChairIndex())
			{
				uint32 nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
				m_g_myself->StartTimer(IDI_OUT_CARD,nElapse);
			}
		}
		return;
	}
}

/// �����û����뷿����Ϣ
void CRobotLogicFrame::OnProcessEnterRoomMsg(int playerId)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

	if(playerId == m_g_myself->GetChairIndex())
		m_g_myself->SendReadyMsg();
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

	switch (timerId)
	{
	case IDI_START_GAME:	//��ʼ��Ϸ
		{
			//��ʼ�ж�
			if (curTimer==0)
			{
				m_g_myself->SendReadyMsg();
				m_g_myself->StopTimer(IDI_START_GAME);
			}
		}
		return ;
	case IDI_CALL_SCORE:	//�û��з�
		{
			if(curTimer==0)
			{
				m_g_myself->StopTimer(IDI_CALL_SCORE);
				//���ñ���
				uint16 bLandScore;
				bLandScore=m_GameLogic.LandScore(m_g_myself->GetChairIndex(), m_cbCurrentLandScore, m_cbCallScorePhase);

				//��������
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = SUB_C_LAND_SCORE;
                out["LandScore"] = bLandScore;
				m_g_myself->SendGameMsg(out);
			}
		}
		return ;
	case IDI_OUT_CARD:		//�û�����
		{
			if(curTimer==0)
			{
				m_g_myself->StopTimer(IDI_OUT_CARD);
				//�˿˷���
				tagOutCardResult OutCardResult;
			/*try
				{
			#ifdef _DEBUG
					CString str0 = m_pIAndroidUserItem->GetMeUserItem()->GetUserData()->szAccounts;
					str0 += "\n\n" ;
					OutputDebugString(str0) ;

			#endif*/
					m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount,m_cbTurnCardData,m_cbTurnCardCount, m_wOutCardUser, m_g_myself->GetChairIndex(), OutCardResult);

			//#ifdef _DEBUG
			//		CString str1 = "finish search";
			//		str1 += "\n\n" ;
			//		OutputDebugString(str1) ;

			//#endif
			//	}
			//	catch(...)
			//	{
			//		//��������ã�ʹ�ý��������if����
			//		ZeroMemory(OutCardResult.cbResultCard, sizeof(OutCardResult.cbResultCard)) ;
			//		OutCardResult.cbCardCount = 10 ;
			//	}
				//���ͺϷ��ж�
				if(OutCardResult.cbCardCount>0 && CT_ERROR==m_GameLogic.GetCardType(OutCardResult.cbResultCard, OutCardResult.cbCardCount))
				{
			/*#ifdef _DEBUG
					CString str ;
					for(WORD i=0; i<OutCardResult.cbCardCount; ++i)
					{
						CString strTmp ;
						strTmp.Format("%d,", m_GameLogic.GetCardValue(OutCardResult.cbResultCard[i])) ;
						str += strTmp ;
					}
					str += "\n\n" ;
					OutputDebugString(str) ;

			#endif*/
					memset(&OutCardResult,0, sizeof(OutCardResult)) ;
				}

				//�ȳ��Ʋ���Ϊ��
				if(m_cbTurnCardCount==0)
				{
					assert(OutCardResult.cbCardCount>0) ;
					if(OutCardResult.cbCardCount==0)
					{
						//��Сһ��
						OutCardResult.cbCardCount = 1 ;
						OutCardResult.cbResultCard[0]=m_cbHandCardData[m_cbHandCardCount-1] ;
					}
				}

				//����ж�
				if (OutCardResult.cbCardCount>0)
				{
					//�Ϸ��ж�
					if(m_cbTurnCardCount>0 && !m_GameLogic.CompareCard(m_cbTurnCardData, OutCardResult.cbResultCard, m_cbTurnCardCount, OutCardResult.cbCardCount))
					{

						//��������
                        Json::Value out;
                        out["MsgId"] = IDD_MESSAGE_ROOM;
                        out["MsgSubId"] = SUB_C_PASS_CARD;
						m_g_myself->SendGameMsg(out);
						return ;
					}

					//ɾ���˿�
					m_cbHandCardCount-=OutCardResult.cbCardCount;
					m_GameLogic.RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,m_cbHandCardData,m_cbHandCardCount+OutCardResult.cbCardCount);

                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_ROOM;
                    out["MsgSubId"] = SUB_C_OUT_CARD;
                    Json::Value arrayvalue;
					for (int i=0;i<OutCardResult.cbCardCount;i++)
					{
						arrayvalue[i]=m_GameLogic.ChangeCardDate(OutCardResult.cbResultCard[i]);
					}
					out["cards"] = arrayvalue;
					m_g_myself->SendGameMsg(out);

			#ifdef _DEBUG
					CString str1 = TEXT("success search");
					str1 += "\n\n" ;
					OutputDebugString(str1) ;

			#endif
				}
				else
				{
					//��������
                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_ROOM;
                    out["MsgSubId"] = SUB_C_PASS_CARD;
					m_g_myself->SendGameMsg(out);
					return ;
				}

			#ifdef _DEBUG
				CString str = TEXT("finish all");
				str += "\n\n" ;
				OutputDebugString(str) ;

			#endif
				return ;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
