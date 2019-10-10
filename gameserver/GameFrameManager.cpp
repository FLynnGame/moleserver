#include "GameFrameManager.h"
#include "../include/Common/defines.h"
#include "../../include/molnet/Network.h"
#include "gameframe/DBOperator.h"
#include "gameframe/RoomManager.h"
#include "gameframe/PlayerManager.h"

#include <map>

#define IDD_SEND_PLAYER_MAX_COUNT         30               // һ����෢�Ͷ��ٸ��˵�����


initialiseSingleton(GameFrameManager);

/**
 * ���캯��
 */
GameFrameManager::GameFrameManager()
{
}

/**
 * ��������
 */
GameFrameManager::~GameFrameManager()
{
}

/**
 * ���ڴ�����յ���������Ϣ
 *
 * @param connId Ҫ����Ŀͻ��˵�����ID
 * @param mes ���յ��Ŀͻ��˵���Ϣ
 */
void GameFrameManager::OnProcessNetMes(uint32 connId,CMolMessageIn *mes)
{
	if(mes == NULL) return;

    Json::Reader reader;
    Json::Value json_object;

    if (!reader.parse(mes->getData(), json_object))
    {
        LOG_ERROR("���ݽ���ʧ��:%s",mes->getData());
        return;
    }

	switch(json_object["MsgId"].asInt())
	{
	case IDD_MESSAGE_FRAME:                            // �����Ϣ
		{
			OnProcessFrameMes(connId,json_object);
		}
		break;
	case IDD_MESSAGE_GAME_LOGIN:                       // ��¼��Ϣ
		{
			OnProcessUserLoginMes(connId,json_object);
		}
		break;
	case IDD_MESSAGE_ROOM:                             // ������Ϣ
		{
			ServerRoomManager.OnProcessNetMes(connId,json_object);
		}
		break;
	default:
		break;
	}
}

/**
 * ���ڴ����������������Ϣ
 *
 * @param connId Ҫ����Ŀͻ��˵�����ID
 */
void GameFrameManager::OnProcessConnectedNetMes(uint32 connId)
{
    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_CONNECT;
    root["MsgSubId"] = IDD_MESSAGE_CONNECT_SUCESS;

    Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
}

/**
 * ���ڴ������ڶϿ�����������Ϣ
 *
 * @param connId Ҫ����Ŀͻ��˵�����ID
 */
void GameFrameManager::OnProcessDisconnectNetMes(uint32 connId)
{
	ServerRoomManager.OnProcessDisconnectNetMes(connId);
}

/// ���������Ϣ
void GameFrameManager::UpdatePlayerInfo(Player *pPlayer)
{
	if(pPlayer == NULL) return;

	Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_UPDATE_USER_DATA;
    root["ID"] = pPlayer->GetID();
    root["State"] = pPlayer->GetState();
    root["Type"] = pPlayer->GetType();
    root["RoomId"] = pPlayer->GetRoomId();
    root["ChairIndex"] = pPlayer->GetChairIndex();
    root["Name"] = pPlayer->GetName();
    root["Level"] = pPlayer->GetLevel();
    root["Money"] = (uint32)pPlayer->GetMoney();
    root["BankMoney"] = (uint32)pPlayer->GetBankMoney();
    root["Revenue"] = (uint32)pPlayer->GetRevenue();
    root["TotalResult"] = (uint32)pPlayer->GetTotalResult();
    root["Experience"] = pPlayer->GetExperience();
    root["TotalBureau"] = pPlayer->GetTotalBureau();
    root["SuccessBureau"] = pPlayer->GetSuccessBureau();
    root["FailBureau"] = pPlayer->GetFailBureau();
    root["RunawayBureau"] = pPlayer->GetRunawayBureau();
    root["SuccessRate"] = pPlayer->GetSuccessRate();
    root["Runawayrate"] = pPlayer->GetRunawayrate();

	ServerPlayerManager.SendMsgToEveryone(root);
}

/**
 * �����û���¼ϵͳ��Ϣ
 *
 * @param connId Ҫ����Ŀͻ���
 * @param mes Ҫ����Ŀͻ��˵���Ϣ
 */
void GameFrameManager::OnProcessUserLoginMes(uint32 connId,Json::Value &mes)
{
	std::string pUserName = mes["UserName"].asString();
	std::string pUserPW = mes["UserPW"].asString();
	int pDeviceType = mes["DeviceType"].asInt();

	uint32 pUserId = ServerDBOperator.IsExistUser(pUserName.c_str(),pUserPW.c_str());

	if(pUserId <= 0)
	{
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
        root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_FAIL;
        Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		return;
	}

	//��鵱ǰ�����Ƿ񳬹�ָ����������
	if(ServerPlayerManager.GetPlayerCount() >= m_ServerSet.TableCount * m_ServerSet.PlayerCount)
	{
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
        root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_FULL;
        Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		return;
	}

	UserDataStru pUserData;
	if(!ServerDBOperator.GetUserData(pUserId,pUserData))
	{
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
        root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_FAIL;
        Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		return;
	}

	// ���Ȳ��ҵ�ǰ����б����Ƿ��������û����������,�����û����ڵ���״̬�£��ʹ����û��ĵ�������
	CPlayer *pPlayer = ServerPlayerManager.GetLostPlayer(pUserId);
	if(pPlayer)
	{
		// �����豸����
		pPlayer->SetDeviceType((PlayerDeviceType)pDeviceType);

		if(pPlayer->GetState() == PLAYERSTATE_LOSTLINE)
		{
			pPlayer->SetConnectID(connId);

			// �ȴӵ����б������������
			ServerPlayerManager.DeleteLostPlayer(pPlayer->GetID());

			// Ȼ�����¼��뵽����б���
			ServerPlayerManager.Reset(pPlayer);

			// ����ҷ��ͳɹ���¼��������Ϣ
			SendPlayerLoginSuccess(pPlayer);
			return;
		}
	}

	pPlayer = ServerPlayerManager.GetPlayerById(pUserId);
	if(pPlayer)
	{
		// �����豸����
		pPlayer->SetDeviceType((PlayerDeviceType)pDeviceType);

		if(pPlayer->GetState() != PLAYERSTATE_LOSTLINE)
		{
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
            root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_EXIST;
            root["ServerPort"] = m_ServerSet.m_iServerPort;
            root["GameType"] = m_ServerSet.m_GameType;
            root["GameState"] = 1;
            Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			return;
		}
	}
	else
	{
		// ����û��Ƿ�����һ����Ϸ��������
		uint32 pserverid,pgametype;
		int32 proomid,pchairid;
		pserverid=proomid=pchairid=pgametype=0;
		if(ServerDBOperator.IsExistUserGaming(pUserId,&pserverid,&proomid,&pchairid,&pgametype) &&
			(pserverid > 0 || proomid > -1 || pchairid > -1 || pgametype > 0))
		{
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
            root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_EXIST;
            root["ServerPort"] = pserverid;
            root["GameType"] = pgametype;
            root["GameState"] = 2;
            Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			return;
		}

		CPlayer *pPlayer = new CPlayer(pUserId,connId);
		pPlayer->SetName(pUserName.c_str());
		pPlayer->SetMoney(pUserData.Money);
		pPlayer->SetBankMoney(pUserData.BankMoney);
		pPlayer->SetRevenue(pUserData.Revenue);
		pPlayer->SetTotalResult(pUserData.TotalResult);
		pPlayer->SetLevel(pUserData.Level);
		pPlayer->SetExperience(pUserData.Experience);
//		pPlayer->SetUserAvatar(pUserData.UserAvatar);
		pPlayer->SetTotalBureau(pUserData.TotalBureau);
		pPlayer->SetSuccessBureau(pUserData.SBureau);
		pPlayer->SetFailBureau(pUserData.FailBureau);
		pPlayer->SetRunawayBureau(pUserData.RunawayBureau);
		pPlayer->SetSuccessRate(pUserData.SuccessRate);
		pPlayer->SetRunawayrate(pUserData.RunawayRate);
		pPlayer->SetSex(pUserData.sex);
		pPlayer->SetSysType(pUserData.gType);
		pPlayer->SetRealName(pUserData.realName);
		pPlayer->SetLoginIP((uint32)inet_addr(pUserData.UserIP));
		pPlayer->SetDeviceType((PlayerDeviceType)pDeviceType);

		// �����¼�ɹ�������Ҽ��뵽��������
		if(ServerPlayerManager.AddPlayer(pPlayer))
		{
			SendPlayerLoginSuccess(pPlayer);
		}
		else
		{
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
            root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_FAIL;
            Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		}
	}
}

/// ����ָ����ҵ�½�ɹ�����Ϣ
void GameFrameManager::SendPlayerLoginSuccess(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return;

	pPlayer->setCurGameID(m_ServerSet.m_GameType);
	pPlayer->setCurServerId(m_ServerSet.m_iServerPort);
	ServerDBOperator.SetPlayerGameState(pPlayer);

	// ����ҷ��ͳɹ���¼��������Ϣ
    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
    root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_SUCESS;
    root["ID"] = pPlayer->GetID();
    root["State"] = pPlayer->GetState();
    root["Type"] = pPlayer->GetType();
    root["RoomId"] = pPlayer->GetRoomId();
    root["ChairIndex"] = pPlayer->GetChairIndex();
    root["Name"] = pPlayer->GetName();
    //root["UserAvatar"] = pPlayer->GetUserAvatar();
    root["Level"] = pPlayer->GetLevel();
    root["Money"] = (uint32)pPlayer->GetMoney();
    root["BankMoney"] = (uint32)pPlayer->GetBankMoney();
    root["Revenue"] = (uint32)pPlayer->GetRevenue();
    root["TotalResult"] = (uint32)pPlayer->GetTotalResult();
    root["Experience"] = pPlayer->GetExperience();
    root["TotalBureau"] = pPlayer->GetTotalBureau();
    root["SuccessBureau"] = pPlayer->GetSuccessBureau();
    root["FailBureau"] = pPlayer->GetFailBureau();
    root["SuccessBureau"] = pPlayer->GetSuccessBureau();
    root["SuccessRate"] = pPlayer->GetSuccessRate();
    root["Runawayrate"] = pPlayer->GetRunawayrate();
    root["Sex"] = pPlayer->GetSex();
    root["RealName"] = pPlayer->GetRealName();
    root["LoginIP"] = pPlayer->GetLoginIP();
    root["DeviceType"] = pPlayer->GetDeviceType();
    root["CurGameID"] = pPlayer->getCurGameID();
    root["CurServerId"] = pPlayer->getCurServerId();

	ServerPlayerManager.SendMsgToEveryone(root);
}

/**
 * �ȴ�����Ϸ�����Ϣ
 *
 * @param connId Ҫ����Ŀͻ���
 * @param mes Ҫ����Ŀͻ��˵���Ϣ
 */
void GameFrameManager::OnProcessFrameMes(uint32 connId,Json::Value &mes)
{
	switch(mes["MsgSubId"].asInt())
	{
	case IDD_MESSAGE_LOOKON_ENTER_ROOM:              // �Թ۽�����Ϸ����
		{
			CPlayer *pPlayer = ServerPlayerManager.GetNewPlayer(connId);
			if(pPlayer == NULL) return;

			// ����û��Ƿ�����һ����Ϸ��������
			uint32 pserverid,pgametype;
			int32 proomid,pchairid;
			pserverid=proomid=pchairid=pgametype=0;
			if(ServerDBOperator.IsExistUserGaming(pPlayer->GetID(),&pserverid,&proomid,&pchairid,&pgametype) &&
				(m_ServerSet.m_iServerPort != pserverid || m_ServerSet.m_GameType != pgametype))
			{
				Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_EXIST;
                root["serverid"] = pserverid;
                root["gametype"] = pgametype;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			int pRoomIndex = mes["RoomIndex"].asInt();
			int pChairIndex = mes["ChairIndex"].asInt();

			if(pRoomIndex >= 0 && pChairIndex >= 0)
			{
				if(ServerRoomManager.AddLookOnPlayer(pPlayer,pRoomIndex,pChairIndex))
				{
					// ��������������е��������ͨ����ҽ�����Ϸ�������Ϣ
                    Json::Value root;
                    root["MsgId"] = IDD_MESSAGE_FRAME;
                    root["MsgSubId"] = IDD_MESSAGE_LOOKON_ENTER_ROOM;
                    root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_SUCC;
                    root["ID"] = pPlayer->GetID();
                    root["RoomId"] = pPlayer->GetRoomId();
                    root["ChairIndex"] = pPlayer->GetChairIndex();

					ServerPlayerManager.SendMsgToEveryone(root);

					CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
					if(pRoom) pRoom->OnProcessEnterRoomMsg(pPlayer->GetChairIndex());

					return;
				}
			}

			// ��ǰ��ҷ��ͽ�����Ϸ����ʧ�ܵ���Ϣ
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_LOOKON_ENTER_ROOM;
            root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FAIL;
            root["ID"] = pPlayer->GetID();
            Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		}
		break;
	case IDD_MESSAGE_ENTER_ROOM:                     // ������Ϸ����
		{
			CPlayer *pPlayer = ServerPlayerManager.GetNewPlayer(connId);
			if(pPlayer == NULL)
				return;

			// �������Ƿ��Ѿ��ڷ������ˣ�����ڷ����У��ǲ��ܽ��뷿���
			CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
			if(pRoom != NULL)
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_EXIST;
                root["serverid"] = 0;
                root["GameType"] = m_ServerSet.m_GameType;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			int pRoomIndex = mes["RoomIndex"].asInt();
			int pChairIndex = mes["ChairIndex"].asInt();
			std::string pEnterPWd = mes["EnterPWd"].asString();
			int64 pEnterfirst = mes["Enterfirst"].asInt();
			int64 pEntersecond = mes["Entersecond"].asInt();

			pPlayer->SetEnterPassword(pEnterPWd);
			pPlayer->SetEnterMoneyRect(pEnterfirst,pEntersecond);

			if(pRoomIndex < 0 || pRoomIndex >= 65535) pRoomIndex = -1;
			if(pChairIndex < 0 || pChairIndex >= 65535) pChairIndex = -1;

			// ����û��Ƿ�����һ����Ϸ��������
			uint32 pserverid,pgametype;
			int32 proomid,pchairid;
			pserverid=proomid=pchairid=pgametype=0;
			if(ServerDBOperator.IsExistUserGaming(pPlayer->GetID(),&pserverid,&proomid,&pchairid,&pgametype) &&
				(m_ServerSet.m_iServerPort != pserverid || m_ServerSet.m_GameType != pgametype))
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_EXIST;
                root["serverid"] = pserverid;
                root["GameType"] = pgametype;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			// �����û����ϵ�Ǯ����������
			OnProcessUserInfo(pPlayer);

			// ����û��Ľ���Ƿ����㷿��Ҫ��
			if(pPlayer->GetMoney() < m_ServerSet.lastMoney ||
				pPlayer->GetMoney() < m_ServerSet.m_Pielement * m_g_ServerServiceManager->GetRoomLastDouble())
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_LASTMONEY;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			JoinPlayerToGameRoom(pPlayer,pRoomIndex,pChairIndex,m_ServerSet.m_QueueGaming);
		}
		break;
	case IDD_MESSAGE_CHANGER_ROOM:                  // ������Ϸ����
		{
			CPlayer *pPlayer = ServerPlayerManager.GetPlayer(connId);
			if(pPlayer == NULL) return;

			// ���Ƚű��������û��뿪��Ϸ����
			CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
			if(pRoom == NULL) return;

			// ����������Ϸ���ǲ��ܻ�����
			if(pPlayer->GetState() == PLAYERSTATE_GAMING)
				return;

			// ����û������Թ��û�����ô���Ƚ�����Ϸ
			if(!pPlayer->IsLookOn())
			{
				pRoom->OnProcessLeaveRoomMsg(pPlayer->GetChairIndex());
			}

			// ��ǰ�������������û���������뿪������Ϣ
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_LEAVE_ROOM;
            root["UserID"] = pPlayer->GetID();

			ServerPlayerManager.SendMsgToEveryone(root);

			// �ӷ������������û�
			pRoom->ClearPlayer(pPlayer);

			// ������ҵ�״̬Ϊ�뿪����
			pPlayer->SetRoomId(-1);
			pPlayer->SetChairIndex(-1);

			// ׼�����˵��û��Ƿ������ʼ��Ϸ
			pRoom->OnProcessContinueGaming();

			// ����û��Ƿ�����һ����Ϸ��������
			uint32 pserverid,pgametype;
			int32 proomid,pchairid;

			pserverid=proomid=pchairid=pgametype=0;
			if(ServerDBOperator.IsExistUserGaming(pPlayer->GetID(),&pserverid,&proomid,&pchairid,&pgametype) &&
				(m_ServerSet.m_iServerPort != pserverid || m_ServerSet.m_GameType != pgametype))
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_EXIST;
                root["serverid"] = pserverid;
                root["GameType"] = pgametype;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			// �����û����ϵ�Ǯ����������
			OnProcessUserInfo(pPlayer);

			// ����û��Ľ���Ƿ����㷿��Ҫ��
			if(pPlayer->GetMoney() < m_ServerSet.lastMoney ||
				pPlayer->GetMoney() < m_ServerSet.m_Pielement * m_g_ServerServiceManager->GetRoomLastDouble())
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_LASTMONEY;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			JoinPlayerToGameRoom(pPlayer,-1,-1,m_ServerSet.m_QueueGaming);
		}
		break;
	case IDD_MESSAGE_USER_CHAT:                     // �û�����
		{
			int senduserID = mes["senduserID"].asInt();
			int receiverID = mes["receiverID"].asInt();
			std::string ChatMsg = mes["ChatMsg"].asString();

            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_USER_CHAT;
            root["senduserID"] = senduserID;
            root["receiverID"] = receiverID;
            root["ChatMsg"] = ChatMsg;

			if(receiverID == -1)
				ServerPlayerManager.SendMsgToEveryone(root);
			else
			{
				ServerPlayerManager.SendMsgToPlayer(senduserID,root);
				ServerPlayerManager.SendMsgToPlayer(receiverID,root);
			}
		}
		break;
	case IDD_MESSAGE_UPDATE_USER_MONEY:             // �����û����ϵ�Ǯ
		{
		    uint32 pUserId = mes["UserId"].asInt();

			CPlayer *pPlayer = ServerPlayerManager.GetPlayerById(pUserId);
			if(pPlayer == NULL) return;

			// �����û�������
			OnProcessUserInfo(pPlayer);
		}
		break;
	case IDD_MESSAGE_LEAVE_ROOM:                    // �뿪��Ϸ����
		{
			// �����û��뿪����
			OnProcessGameLeaveRoomMes(connId,mes);
		}
		break;
	case IDD_MESSAGE_GET_ONLINE_PLAYERS:             // �õ��������
		{
			ServerPlayerManager.LockPlayerList();

			int pPlayerCount = 0;
			std::vector<std::vector<CPlayer*> > pPlayerList;
			std::vector<CPlayer*> pNewPlayers;

			std::map<uint32,CPlayer*>::iterator iter = ServerPlayerManager.GetPlayerList().begin();
			for(;iter != ServerPlayerManager.GetPlayerList().end();++iter)
			{
				if((*iter).second == NULL) continue;

				pPlayerCount+=1;
				pNewPlayers.push_back((*iter).second);

				if(pPlayerCount >= IDD_SEND_PLAYER_MAX_COUNT)
				{
					pPlayerList.push_back(pNewPlayers);
					pPlayerCount=0;
					pNewPlayers.clear();
				}
			}

			iter = ServerPlayerManager.GetRobotList().begin();
			for(;iter != ServerPlayerManager.GetRobotList().end();++iter)
			{
				if((*iter).second == NULL) continue;

				pPlayerCount+=1;
				pNewPlayers.push_back((*iter).second);

				if(pPlayerCount >= IDD_SEND_PLAYER_MAX_COUNT)
				{
					pPlayerList.push_back(pNewPlayers);
					pPlayerCount=0;
					pNewPlayers.clear();
				}
			}

			iter = ServerPlayerManager.GetLostPlayerList().begin();
			for(;iter != ServerPlayerManager.GetLostPlayerList().end();++iter)
			{
				if((*iter).second == NULL) continue;

				pPlayerCount+=1;
				pNewPlayers.push_back((*iter).second);

				if(pPlayerCount >= IDD_SEND_PLAYER_MAX_COUNT)
				{
					pPlayerList.push_back(pNewPlayers);
					pPlayerCount=0;
					pNewPlayers.clear();
				}
			}

			if(pPlayerCount > 0 && !pNewPlayers.empty())
				pPlayerList.push_back(pNewPlayers);

			for(int i=0;i<(int)pPlayerList.size();i++)
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_GET_ONLINE_PLAYERS;
                root["PlayerCount"] = (int)pPlayerList[i].size();

				for(int k=0;k<(int)pPlayerList[i].size();k++)
				{
					root[k]["ID"] = pPlayerList[i][k]->GetID();
					root[k]["State"] = pPlayerList[i][k]->GetState();
					root[k]["Type"] = pPlayerList[i][k]->GetType();
					root[k]["RoomId"] = pPlayerList[i][k]->GetRoomId();
					root[k]["ChairIndex"] = pPlayerList[i][k]->GetChairIndex();
					root[k]["Name"] = pPlayerList[i][k]->GetName();
					root[k]["Level"] = pPlayerList[i][k]->GetLevel();
					root[k]["Money"] = (uint32)pPlayerList[i][k]->GetMoney();
					root[k]["BankMoney"] = (uint32)pPlayerList[i][k]->GetBankMoney();
					root[k]["Revenue"] = (uint32)pPlayerList[i][k]->GetRevenue();
					root[k]["TotalResult"] = (uint32)pPlayerList[i][k]->GetTotalResult();
					root[k]["Experience"] = pPlayerList[i][k]->GetExperience();
					root[k]["TotalBureau"] = pPlayerList[i][k]->GetTotalBureau();
					root[k]["SuccessBureau"] = pPlayerList[i][k]->GetSuccessBureau();
					root[k]["FailBureau"] = pPlayerList[i][k]->GetFailBureau();
					root[k]["RunawayBureau"] = pPlayerList[i][k]->GetRunawayBureau();
					root[k]["SuccessRate"] = pPlayerList[i][k]->GetSuccessRate();
					root[k]["Runawayrate"] = pPlayerList[i][k]->GetRunawayrate();
					root[k]["Sex"] = pPlayerList[i][k]->GetSex();
					root[k]["RealName"] = pPlayerList[i][k]->GetRealName();
					root[k]["LoginIP"] = pPlayerList[i][k]->GetLoginIP();
					root[k]["DeviceType"] = pPlayerList[i][k]->GetDeviceType();
				}

				Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			}

            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_GET_ONLINE_PLAYERS_END;
            Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());

			ServerPlayerManager.UnlockPlayerList();
		}
		break;
	case IDD_MESSAGE_GET_ROOM_LIST:                  // �õ���ǰ�������з����б�
		{
			ServerRoomManager.LockRoomList();

			std::vector<CRoom*> pRoomList = ServerRoomManager.GetRoomList();
			if(pRoomList.empty())
			{
				ServerRoomManager.UnlockRoomList();
				return;
			}

            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_GET_ROOM_LIST;

			// ����д�������з��������
			root["RoomCount"] = (int)pRoomList.size();
			root["GameName"] = m_ServerSet.GameName;
			root["GameType"] = m_ServerSet.GameType;
			root["PlayerCount"] = m_ServerSet.PlayerCount;

			std::vector<CRoom*>::iterator iter = pRoomList.begin();
			for(int index=0;iter != pRoomList.end();iter++,index++)
			{
				if((*iter) == NULL) continue;

				int64 pfirst,psecond;
				pfirst=psecond = 0;
				(*iter)->GetEnterMoneyRect(&pfirst,&psecond);

				// ���ͷ���״̬
				root[index]["RoomState"] = (int)(*iter)->GetRoomState();
				root[index]["EnterPassword"] = (*iter)->getEnterPassword();
				root[index]["first"] = (uint32)pfirst;
				root[index]["second"] = (uint32)psecond;
				root[index]["PlayerCount"] = (*iter)->GetPlayerCount();

				// ��ʼ���ͷ����������Ϣ
				for(int i=0;i<(*iter)->GetMaxPlayer();i++)
				{
					Player *pPlayer = (*iter)->GetPlayer(i);
					if(pPlayer == NULL) continue;

					root[index][i]["ID"] = pPlayer->GetID();
					root[index][i]["ChairIndex"] = pPlayer->GetChairIndex();
				}
			}

			Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());

			ServerRoomManager.UnlockRoomList();
		}
		break;
	case IDD_MESSAGE_READY_START:                     // ���׼����ʼ��Ϸ
		{
			OnProcessGameReadyMes(connId,mes);
		}
		break;
	case IDD_MESSAGE_REENTER_ROOM:                    // ���»ص���Ϸ
		{
			CPlayer *pPlayer = ServerPlayerManager.GetPlayer(connId);
			if(pPlayer == NULL) return;

			CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
			if(pRoom == NULL || pRoom->GetRoomState() != ROOMSTATE_GAMING)
			{
				// ��ǰ��ҷ��ͽ�����Ϸ����ʧ�ܵ���Ϣ
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FAIL;
                root["ID"] = pPlayer->GetID();
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());

				return;
			}

            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_REENTER_ROOM;
            root["ID"] = pPlayer->GetID();
			ServerPlayerManager.SendMsgToEveryone(root);

			pPlayer->SetState(PLAYERSTATE_GAMING);

			pRoom->OnProcessReEnterRoomMes(pPlayer->GetChairIndex());
		}
		break;
	default:
		break;
	}
}

/// ���������Ϣ
void GameFrameManager::OnProcessUserInfo(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return;

	UserDataStru pUserData;
	if(ServerDBOperator.GetUserData(pPlayer->GetID(),pUserData))
	{
		bool isSendUpdateMsg = false;

		if(pPlayer->GetMoney() != pUserData.Money || pPlayer->GetBankMoney() != pUserData.BankMoney ||
			pPlayer->GetSex() != pUserData.sex || pPlayer->GetRealName() != pUserData.realName)
			isSendUpdateMsg = true;

		pPlayer->SetMoney(pUserData.Money);
		pPlayer->SetBankMoney(pUserData.BankMoney);
		pPlayer->SetSex(pUserData.sex);
		pPlayer->SetRealName(pUserData.realName);
		pPlayer->SetLoginIP((uint32)inet_addr(pUserData.UserIP));

		if(isSendUpdateMsg)
		{
			UpdatePlayerMoney(pPlayer);
		}
	}
}

/// ����������ϵ�Ǯ
void GameFrameManager::UpdatePlayerMoney(Player *pPlayer)
{
	if(pPlayer == NULL) return;

    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_UPDATE_USER_MONEY;
    root["ID"] = pPlayer->GetID();
    root["Money"] = (uint32)pPlayer->GetMoney();
    root["BankMoney"] = (uint32)pPlayer->GetBankMoney();
    root["Sex"] = pPlayer->GetSex();
    root["RealName"] = pPlayer->GetRealName();

	ServerPlayerManager.SendMsgToEveryone(root);
}

/// ���ڴ����û�׼����Ϣ
void GameFrameManager::OnProcessGameReadyMes(uint32 connId,Json::Value &mes)
{
	CPlayer *pPlayer = ServerPlayerManager.GetPlayer(connId);
	if(pPlayer == NULL) return;

	CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
	if(pRoom == NULL) return;

	// �������û�����������ֻ������ͨ�û�
	if(pPlayer->GetState() != PLAYERSTATE_NORAML)
		return;

	// ����������Ϸ��Ҳ������
	if(pRoom->GetRoomState() == ROOMSTATE_GAMING)
		return;

    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_READY_START;
    root["ID"] = pPlayer->GetID();
	ServerPlayerManager.SendMsgToEveryone(root);

	pPlayer->SetState(PLAYERSTATE_READY);
	pPlayer->SetReadyTime((DWORD)time(NULL));

	if(pRoom->GetMaster() < 0)
		pRoom->SetMaster(pPlayer->GetChairIndex());

	pRoom->OnProcessPlayerReadyMes(pPlayer->GetChairIndex());
}

/// ���ڴ����û��뿪��Ϣ
void GameFrameManager::OnProcessGameLeaveRoomMes(uint32 connId,Json::Value &mes)
{
	CPlayer *pPlayer = ServerPlayerManager.GetPlayer(connId);
	if(pPlayer == NULL) return;

	CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
	if(pRoom == NULL) return;

	if(!pPlayer->IsLookOn())
	{
		pRoom->OnProcessLeaveRoomMsg(pPlayer->GetChairIndex());

		//pPlayer->SetState(PLAYERSTATE_NORAML);
	}

    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_LEAVE_ROOM;
    root["ID"] = pPlayer->GetID();

	ServerPlayerManager.SendMsgToEveryone(root);

	pRoom->ClearPlayer(pPlayer);

	pPlayer->SetRoomId(-1);
	pPlayer->SetChairIndex(-1);
}

bool GameFrameManager::JoinPlayerToGameRoom(CPlayer *pPlayer,int pRoomIndex,int pChairIndex,bool isQueue)
{
	if(pPlayer == NULL)
		return false;

	if(AddPlayerInServer(pPlayer,pRoomIndex,pChairIndex,isQueue,true)) return true;

	if(!isQueue)
	{
		if(m_ServerSet.m_GameStartMode == enStartMode_FullReady ||
			m_ServerSet.m_GameStartMode == enStartMode_TimeControl)
		{
			if(!AddPlayerInServer(pPlayer,pRoomIndex,pChairIndex,isQueue,false))
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;

				if(!pPlayer->getEnterPassword().empty())
					root["MsgSubId2"] = IDD_MESSAGE_PERSONROOM_ENTER_ROOM_FAIL;
				else
					root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FAIL;

				root["ID"] = pPlayer->GetID();

				Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			}
		}
		else
		{
			CRoom *pRoom = ServerRoomManager.GetFreeRoom();
			if(pRoom != NULL)
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FAIL;
                root["ID"] = pPlayer->GetID();

				Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			}
			else
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FULL;
                root["ID"] = pPlayer->GetID();

				Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			}
		}
	}

	pPlayer->SetEnterMoneyRect(0,0);
	pPlayer->SetEnterPassword("");

	return false;
}

bool GameFrameManager::AddPlayerInServer(CPlayer *pPlayer,int pRoomIndex,int pChairIndex,bool isQueue,bool isGaming)
{
	if(ServerRoomManager.AddPlayer(pPlayer,pRoomIndex,pChairIndex,isQueue,isGaming))
	{
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_FRAME;
        root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
        root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_SUCC;
        root["ID"] = pPlayer->GetID();
        root["RoomId"] = pPlayer->GetRoomId();
        root["ChairIndex"] = pPlayer->GetChairIndex();

		ServerPlayerManager.SendMsgToEveryone(root);

		CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
		if(pRoom)
		{
			pRoom->OnProcessEnterRoomMsg(pPlayer->GetChairIndex());
		}

		pPlayer->SetEnterMoneyRect(0,0);
		pPlayer->SetEnterPassword("");

		return true;
	}

	return false;
}

/// ���·�����Ϣ
void GameFrameManager::UpdateGameRoomInfo(void)
{
    ServerDBOperator.UpdateGameRoomOnlinePlayerCount(m_ServerSet.RoomId,
                                                     ServerPlayerManager.GetPlayerCount(),
                                                     ServerPlayerManager.GetRobotCount());
}

