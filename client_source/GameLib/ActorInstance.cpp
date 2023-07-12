#include "StdAfx.h"
#include "ActorInstance.h"
#include "AreaTerrain.h"
#include "RaceData.h"
#include "../SpeedTreeLib/SpeedTreeForestDirectX8.h"
#include "../SpeedTreeLib/SpeedTreeWrapper.h"

enum
{
#ifdef ENABLE_WOLFMAN
	MAIN_RACE_MAX_NUM = 9,
#else
	MAIN_RACE_MAX_NUM = 8,
#endif
};

void CActorInstance::INSTANCEBASE_Deform()
{
	Deform();
	TraceProcess();
}

void CActorInstance::INSTANCEBASE_Transform()
{
	if (m_pkHorse)
	{
		m_pkHorse->INSTANCEBASE_Transform();

		m_x = m_pkHorse->NEW_GetCurPixelPositionRef().x;
		m_y = -m_pkHorse->NEW_GetCurPixelPositionRef().y;
		m_z = m_pkHorse->NEW_GetCurPixelPositionRef().z;
		m_bNeedUpdateCollision = TRUE;			
	}
	
	Update();
	TransformProcess();
	Transform();
	UpdatePointInstance();
	ShakeProcess();
	UpdateBoundingSphere();
	UpdateAttribute();
}

void CActorInstance::OnUpdate()
{
	if (!IsParalysis())
		CGraphicThingInstance::OnUpdate();

	UpdateAttachingInstances();
	__BlendAlpha_Update();
}

IBackground& CActorInstance::GetBackground()
{
	return IBackground::Instance();
}


void CActorInstance::SetMainInstance()
{
	m_isMain=true;
}

void CActorInstance::SetParalysis(bool isParalysis)
{
	m_isParalysis=isParalysis;
}

void CActorInstance::SetFaint(bool isFaint)
{
	m_isFaint=isFaint;
}

void CActorInstance::SetSleep(bool isSleep)
{
	m_isSleep=isSleep;

	Stop();
}

void CActorInstance::SetResistFallen(bool isResistFallen)
{
	m_isResistFallen=isResistFallen;
}

void CActorInstance::SetReachScale(float fScale)
{
	m_fReachScale=fScale;
}

float CActorInstance::__GetReachScale()
{
	return m_fReachScale;
}

float CActorInstance::__GetAttackSpeed()
{
	return m_fAtkSpd;
}

WORD CActorInstance::__GetCurrentComboType()
{
	if (IsBowMode())
		return 0;
	if (IsHandMode())
		return 0;
	if (__IsMountingHorse())
		return 0;

	return m_wcurComboType;
}

void CActorInstance::SetComboType(WORD wComboType)
{
	m_wcurComboType = wComboType;
}

void CActorInstance::SetAttackSpeed(float fAtkSpd)
{
	m_fAtkSpd=fAtkSpd;
}

void CActorInstance::SetMoveSpeed(float fMovSpd)
{
	if (m_fMovSpd==fMovSpd)
		return;

	m_fMovSpd=fMovSpd;

	if (__IsMoveMotion())
	{
		Stop();
		Move();
	}
}

void CActorInstance::SetFishingPosition(D3DXVECTOR3 & rv3Position)
{
	m_v3FishingPosition = rv3Position;
}

void  CActorInstance::Move()
{
	if (m_isWalking)
	{
		SetLoopMotion(CRaceMotionData::NAME_WALK, 0.15f, m_fMovSpd);
	}
	else
	{
		SetLoopMotion(CRaceMotionData::NAME_RUN, 0.15f, m_fMovSpd);
	}
}

void  CActorInstance::Stop(float fBlendingTime)
{
	__ClearMotion();
	SetLoopMotion(CRaceMotionData::NAME_WAIT, fBlendingTime);
}

void CActorInstance::SetOwner(DWORD dwOwnerVID)
{
	m_fOwnerBaseTime=GetLocalTime();
	m_dwOwnerVID=dwOwnerVID;
}

void CActorInstance::SetActorType(UINT eType)
{
	m_eActorType=eType;
}

UINT CActorInstance::GetActorType() const
{
	return m_eActorType;
}

bool CActorInstance::IsHandMode()
{
	if (CRaceMotionData::MODE_GENERAL==GetMotionMode())
		return true;

	if (CRaceMotionData::MODE_HORSE==GetMotionMode())
		return true;

	return false;
}

bool CActorInstance::IsTwoHandMode()
{
	if (CRaceMotionData::MODE_TWOHAND_SWORD==GetMotionMode())
		return true;

	return false;
}

bool CActorInstance::IsBowMode()
{
	if (CRaceMotionData::MODE_BOW==GetMotionMode())
		return true;

	if (CRaceMotionData::MODE_HORSE_BOW==GetMotionMode())
		return true;

	return false;
}

bool CActorInstance::IsPoly()
{
	if (TYPE_POLY==m_eActorType)
		return true;

	if (TYPE_PC==m_eActorType)
		if (m_eRace >= MAIN_RACE_MAX_NUM)
			return TRUE;

	return false;
}

bool CActorInstance::IsPC()
{
	if (TYPE_PC==m_eActorType)
		return true;

	return false;
}

bool CActorInstance::IsNPC()
{
	if (TYPE_NPC==m_eActorType)
		return true;

	return false;
}

bool CActorInstance::IsEnemy()
{
	if (TYPE_ENEMY==m_eActorType)
		return true;

	return false;
}

bool CActorInstance::IsStone()
{
	if (TYPE_STONE==m_eActorType)
		return true;

	return false;
}

bool CActorInstance::IsWarp()
{
	if (TYPE_WARP==m_eActorType)
		return true;

	return false;
}

bool CActorInstance::IsGoto()
{
	if (TYPE_GOTO==m_eActorType)
		return true;

	return false;
}

bool CActorInstance::IsBuilding()
{
	if (TYPE_BUILDING==m_eActorType)
		return true;

	return false;	
}

bool CActorInstance::IsDoor()
{
	if (TYPE_DOOR==m_eActorType)
		return true;

	return false;
}

bool CActorInstance::IsMount()
{
	if (TYPE_MOUNT == m_eActorType)
	{
		return true;
	}

	return false;
}

bool CActorInstance::IsPet()
{
	if (TYPE_PET == m_eActorType)
	{
		return true;
	}

	return false;
}

bool CActorInstance::IsObject()
{
	if (TYPE_OBJECT==m_eActorType)
		return true;

	return false;
}

void CActorInstance::DestroySystem()
{
}

void CActorInstance::DieEnd()
{
	Die();

	CGraphicThingInstance::SetMotionAtEnd();
}

void CActorInstance::Die()
{
	if (m_isRealDead)
		return;

	if (__IsMoveMotion())
		Stop();

	SetAdvancingRotation(GetRotation());

	if (IsStone())
	{
		InterceptOnceMotion(CRaceMotionData::NAME_DEAD);
	}
	else
	{
		InterceptOnceMotion(CRaceMotionData::NAME_DEAD);
	}

	m_isRealDead = TRUE;
}

BOOL CActorInstance::IsSleep()
{
	return m_isSleep;
}

BOOL CActorInstance::IsParalysis()
{
	return m_isParalysis;
}

BOOL CActorInstance::IsFaint()
{
	return m_isFaint;
}

BOOL CActorInstance::IsResistFallen()
{
	return m_isResistFallen;
}

BOOL CActorInstance::IsMoving()
{
	return __IsMoveMotion();
}

BOOL CActorInstance::IsWaiting()
{
	return __IsWaitMotion();
}

BOOL CActorInstance::IsDead()
{
	return m_isRealDead;
}

BOOL CActorInstance::IsKnockDown()
{
	return __IsKnockDownMotion();
}

BOOL CActorInstance::IsDamage()
{
	return __IsDamageMotion();
}

BOOL CActorInstance::IsAttacked()
{
	if (IsPushing())
		return TRUE;

	if (__IsDamageMotion())
		return TRUE;

	if (__IsKnockDownMotion())
		return TRUE;

	if (__IsDieMotion())
		return TRUE;

	return FALSE;
}

void CActorInstance::PhysicsProcess()
{
	m_PhysicsObject.Update(m_fSecondElapsed);
	AddMovement(m_PhysicsObject.GetXMovement(), m_PhysicsObject.GetYMovement(), 0.0f);
}

void CActorInstance::__AccumulationMovement(float fRot)
{
	if (CRaceMotionData::NAME_WAIT == __GetCurrentMotionIndex())
		return;

	D3DXMATRIX s_matRotationZ;
	D3DXMatrixRotationZ(&s_matRotationZ, D3DXToRadian(fRot));
	UpdateTransform(&s_matRotationZ, GetAverageSecondElapsed());

	AddMovement(s_matRotationZ._41, s_matRotationZ._42, s_matRotationZ._43);
}

void CActorInstance::AccumulationMovement()
{
	if (m_pkTree)
		return;

	if (m_pkHorse)
	{
		m_pkHorse->__AccumulationMovement(m_fcurRotation);
		return;
	}

	__AccumulationMovement(m_fAdvancingRotation);
}

void CActorInstance::TransformProcess()
{
	if (!IsParalysis())
	{
		m_x += m_v3Movement.x;
		m_y += m_v3Movement.y;
		m_z += m_v3Movement.z;
	}

	__InitializeMovement();

	SetPosition(m_x, m_y, m_z);
}

void CActorInstance::OnUpdateCollisionData(const CStaticCollisionDataVector * pscdVector)
{
	assert(pscdVector);
	CStaticCollisionDataVector::const_iterator it;
	for(it = pscdVector->begin();it!=pscdVector->end();++it)
	{
		const CStaticCollisionData & c_rColliData = *it;
		const D3DXMATRIX & c_rMatrix = GetTransform();
		AddCollision(&c_rColliData, &c_rMatrix);
	}
}

void CActorInstance::OnUpdateHeighInstance(CAttributeInstance * pAttributeInstance)
{
	assert(pAttributeInstance);
	SetHeightInstance(pAttributeInstance);
}

bool CActorInstance::OnGetObjectHeight(float fX, float fY, float * pfHeight)
{
	if (!m_pHeightAttributeInstance)
		return false;

	if (TYPE_BUILDING != GetType())
		return false;

	return m_pHeightAttributeInstance->GetHeight(fX, fY, pfHeight) == 1 ? true : false;
}

void CActorInstance::Revive()
{
	m_isSleep = FALSE;
	m_isParalysis = FALSE;
	m_isFaint = FALSE;
	m_isRealDead = FALSE;
	m_isStun = FALSE;
	m_isWalking = FALSE;
	m_isMain = FALSE;
	m_isResistFallen = FALSE;

	__InitializeCollisionData();
}

BOOL CActorInstance::IsStun()
{
	return m_isStun;
}

void CActorInstance::Stun()
{
	m_isStun = TRUE;
}

void CActorInstance::SetWalkMode()
{
	m_isWalking = TRUE;
	if (CRaceMotionData::NAME_RUN == GET_MOTION_INDEX(m_kCurMotNode.dwMotionKey))
		SetLoopMotion(CRaceMotionData::NAME_WALK, 0.15f, m_fMovSpd);
}

void CActorInstance::SetRunMode()
{
	m_isWalking = FALSE;
	if (CRaceMotionData::NAME_WALK == GET_MOTION_INDEX(m_kCurMotNode.dwMotionKey))
		SetLoopMotion(CRaceMotionData::NAME_RUN, 0.15f, m_fMovSpd);
}

MOTION_KEY CActorInstance::GetNormalAttackIndex()
{
	WORD wMotionIndex;
	m_pkCurRaceData->GetNormalAttackIndex(GetMotionMode(), &wMotionIndex);

	return MAKE_MOTION_KEY(GetMotionMode(), wMotionIndex);
}

void CActorInstance::__InitializeMovement()
{
	m_v3Movement.x = 0.0f;
	m_v3Movement.y = 0.0f;
	m_v3Movement.z = 0.0f;
}

void CActorInstance::AddMovement(float fx, float fy, float fz)
{
	m_v3Movement.x += fx;
	m_v3Movement.y += fy;
	m_v3Movement.z += fz;
}

const float gc_fActorSlideMoveSpeed = 5.0f;

void CActorInstance::AdjustDynamicCollisionMovement(const CActorInstance * c_pActorInstance)
{
	if (m_pkHorse)
	{
		m_pkHorse->AdjustDynamicCollisionMovement(c_pActorInstance);
		return;
	}

	if (isAttacking() )
		return;
	
	UINT uActorType = c_pActorInstance->GetActorType();
	if( (uActorType == TYPE_BUILDING) || (uActorType == TYPE_OBJECT) || (uActorType == TYPE_DOOR) || (uActorType == TYPE_STONE))
	{
		BlockMovement();
	}
	else
	{

		float move_length = D3DXVec3Length(&m_v3Movement);
		if (move_length>gc_fActorSlideMoveSpeed)
			m_v3Movement*=gc_fActorSlideMoveSpeed/move_length;

		TCollisionPointInstanceListIterator itMain = m_BodyPointInstanceList.begin();
		for (; itMain != m_BodyPointInstanceList.end(); ++itMain)
		{
			CDynamicSphereInstanceVector & c_rMainSphereVector = (*itMain).SphereInstanceVector;
			for (DWORD i = 0; i < c_rMainSphereVector.size(); ++i)
			{
				CDynamicSphereInstance & c_rMainSphere = c_rMainSphereVector[i];

				TCollisionPointInstanceList::const_iterator itOpp = c_pActorInstance->m_BodyPointInstanceList.begin();
				for(;itOpp != c_pActorInstance->m_BodyPointInstanceList.end();++itOpp)
				{
					CSphereCollisionInstance s;
					s.GetAttribute().fRadius=itOpp->SphereInstanceVector[0].fRadius;
					s.GetAttribute().v3Position=itOpp->SphereInstanceVector[0].v3Position;
					D3DXVECTOR3 v3Delta = s.GetCollisionMovementAdjust(c_rMainSphere);
					m_v3Movement+=v3Delta;
					c_rMainSphere.v3Position+=v3Delta;

					if (v3Delta.x !=0.0f || v3Delta.y !=0.0f || v3Delta.z !=0.0f )
					{
						move_length = D3DXVec3Length(&m_v3Movement);
						if (move_length>gc_fActorSlideMoveSpeed)
						{
							m_v3Movement*=gc_fActorSlideMoveSpeed/move_length;
							c_rMainSphere.v3Position = c_rMainSphere.v3LastPosition;
							c_rMainSphere.v3Position+=m_v3Movement;
						}
					}
				}
			}
		}
	}
}


void CActorInstance::__AdjustCollisionMovement(const CGraphicObjectInstance * c_pGraphicObjectInstance)
{
	if (m_pkHorse)
	{
		m_pkHorse->__AdjustCollisionMovement(c_pGraphicObjectInstance);
		return;
	}

	if (m_v3Movement.x == 0.0f && m_v3Movement.y == 0.0f && m_v3Movement.z == 0.0f) 
		return;

	float move_length = D3DXVec3Length(&m_v3Movement);
	if (move_length>gc_fActorSlideMoveSpeed)
		m_v3Movement*=gc_fActorSlideMoveSpeed/move_length;

	TCollisionPointInstanceListIterator itMain = m_BodyPointInstanceList.begin();
	for (; itMain != m_BodyPointInstanceList.end(); ++itMain)
	{
		CDynamicSphereInstanceVector & c_rMainSphereVector = (*itMain).SphereInstanceVector;
		for (DWORD i = 0; i < c_rMainSphereVector.size(); ++i)
		{
			CDynamicSphereInstance & c_rMainSphere = c_rMainSphereVector[i];

			D3DXVECTOR3 v3Delta = c_pGraphicObjectInstance->GetCollisionMovementAdjust(c_rMainSphere);
			m_v3Movement+=v3Delta;
			c_rMainSphere.v3Position+=v3Delta;

			if (v3Delta.x !=0.0f || v3Delta.y !=0.0f || v3Delta.z !=0.0f )
			{
				move_length = D3DXVec3Length(&m_v3Movement);
				if (move_length>gc_fActorSlideMoveSpeed)
				{
					m_v3Movement*=gc_fActorSlideMoveSpeed/move_length;
					c_rMainSphere.v3Position = c_rMainSphere.v3LastPosition;
					c_rMainSphere.v3Position+=m_v3Movement;
				}
			}
		}
	}
}

BOOL CActorInstance::IsMovement()
{
	if (m_pkHorse)
		if (m_pkHorse->IsMovement())
			return TRUE;
		
	if (0.0f != m_v3Movement.x)
		return TRUE;
	if (0.0f != m_v3Movement.y)
		return TRUE;
	if (0.0f != m_v3Movement.z)
		return TRUE;

	return FALSE;
}

bool CActorInstance::IsReversedAxis()
{
	switch (GetRace())
	{
	case 20029:
	case 20030:
	case 20101:
	case 20102:
	case 20103:
	case 20104:
	case 20105:
	case 20106:
	case 20107:
	case 20108:
	case 20109:
	case 20110:
	case 20111:
	case 20112:
	case 20113:
	case 20114:
	case 20115:
	case 20116:
	case 20117:
	case 20118:
	case 20119:
	case 20120:
	case 20121:
	case 20122:
	case 20123:
	case 20124:
	case 20125:
	case 20201:
	case 20202:
	case 20203:
	case 20204:
	case 20205:
	case 20206:
	case 20207:
	case 20208:
	case 20209:
	case 20210:
	case 20211:
	case 20212:
	case 20213:
	case 20214:
	case 20215:
	case 20216:
	case 20217:
	case 20218:
	case 20219:
	case 20220:
	case 20221:
	case 20222:
	case 20226:
	case 20227:
	case 20231:
	case 20232:
	case 20239:
	case 20240:
	case 20243:
	case 20244:
	case 20245:
	case 20250:
	case 20251:
	case 20252:
	case 20253:
	case 20254:
	case 20255:
	case 20256:
	case 20257:
	case 20258:
	case 20259:
	case 20260:
	case 20261:
	case 20262:
		return true;
		break;
	default:
		return false;
		break;
	}
}

float CActorInstance::GetHeight()
{
	return CGraphicThingInstance::GetHeight(IsReversedAxis());
}

bool CActorInstance::IntersectDefendingSphere()
{
	for (TCollisionPointInstanceList::iterator it = m_DefendingPointInstanceList.begin(); it != m_DefendingPointInstanceList.end(); ++it)
	{
		CDynamicSphereInstanceVector & rSphereInstanceVector = (*it).SphereInstanceVector;

		CDynamicSphereInstanceVector::iterator it2 = rSphereInstanceVector.begin();
		for (; it2 != rSphereInstanceVector.end(); ++it2)
		{
			CDynamicSphereInstance & rInstance = *it2;
			D3DXVECTOR3 v3SpherePosition = rInstance.v3Position;
			float fRadius = rInstance.fRadius;

			D3DXVECTOR3 v3Orig;
			D3DXVECTOR3 v3Dir;
			float fRange;
			ms_Ray.GetStartPoint(&v3Orig);
			ms_Ray.GetDirection(&v3Dir, &fRange);

			D3DXVECTOR3 v3Distance = v3Orig - v3SpherePosition;
			float b = D3DXVec3Dot(&v3Dir, &v3Distance);
			float c = D3DXVec3Dot(&v3Distance, &v3Distance) - fRadius*fRadius;

			if (b*b - c >= 0)
				return true;
		}
	}
	return false;
}

bool CActorInstance::__IsMountingHorse()
{
	return NULL != m_pkHorse;
}

void CActorInstance::MountHorse(CActorInstance * pkHorse)
{
	m_pkHorse = pkHorse;

	if (m_pkHorse)
	{
		m_pkHorse->SetCurPixelPosition(NEW_GetCurPixelPositionRef());
		m_pkHorse->SetRotation(GetRotation());
		m_pkHorse->SetAdvancingRotation(GetRotation());
	}
}

void CActorInstance::__CreateTree(const char * c_szFileName)
{
	__DestroyTree();

	CSpeedTreeForestDirectX8& rkForest=CSpeedTreeForestDirectX8::Instance();
	m_pkTree=rkForest.CreateInstance(m_x, m_y, m_z, GetCaseCRC32(c_szFileName, strlen(c_szFileName)), c_szFileName);
	m_pkTree->SetPosition(m_x, m_y, m_z);
	m_pkTree->UpdateBoundingSphere();
	m_pkTree->UpdateCollisionData();
}

void CActorInstance::__DestroyTree()
{
	if (!m_pkTree)
		return;

	CSpeedTreeForestDirectX8::Instance().DeleteInstance(m_pkTree);
}

void CActorInstance::__SetTreePosition(float fx, float fy, float fz)
{
	if (!m_pkTree)
		return;
	if (m_x == fx && m_y == fy && m_z == fz)
		return;

	m_pkTree->SetPosition(fx, fy, fz);
	m_pkTree->UpdateBoundingSphere();
	m_pkTree->UpdateCollisionData();
}

void CActorInstance::ClearAttachingEffect()
{
	__ClearAttachingEffect();
}

void CActorInstance::Destroy()
{
	ClearFlyTargeter();

	m_HitDataMap.clear();
	m_MotionDeque.clear();

	if (m_pAttributeInstance)
	{
		m_pAttributeInstance->Clear();
		CAttributeInstance::Delete(m_pAttributeInstance);
		m_pAttributeInstance = NULL;
	}

	__ClearAttachingEffect();

	CGraphicThingInstance::Clear();

	__DestroyWeaponTrace();
	__DestroyTree();
	__Initialize();
}

void CActorInstance::__InitializeRotationData()
{
	m_fAtkDirRot = 0.0f;
	m_fcurRotation = 0.0f;
	m_rotBegin = 0.0f;
	m_rotEnd = 0.0f;
	m_rotEndTime = 0.0f;
	m_rotBeginTime = 0.0f;
	m_rotBlendTime = 0.0f;
	m_fAdvancingRotation = 0.0f;
	m_rotX = 0.0f;
	m_rotY = 0.0f;
}


void CActorInstance::__InitializeStateData()
{
	m_bEffectInitialized = false;

	m_isPreInput = FALSE;
	m_isNextPreInput = FALSE;

	m_isSleep = FALSE;
	m_isParalysis = FALSE;
	m_isFaint = FALSE;
	m_isRealDead = FALSE;
	m_isWalking = FALSE;
	m_isMain = FALSE;
	m_isStun = FALSE;
	m_isHiding = FALSE;
	m_isResistFallen = FALSE;

	m_iRenderMode = RENDER_MODE_NORMAL;
	m_fAlphaValue = 0.0f;
	m_AddColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	
	m_dwMtrlColor=0xffffffff;
	m_dwMtrlAlpha=0xff000000;

	m_dwBattleHitEffectID = 0;
	m_dwBattleAttachEffectID = 0;
	m_bShowMonsterEffects = 0;
}

void CActorInstance::__InitializeMotionData()
{	
	m_wcurMotionMode = CRaceMotionData::MODE_GENERAL;
	m_wcurComboType = 0;

	m_fReachScale=1.0f;
	m_fMovSpd=1.0f;
	m_fAtkSpd=1.0f;

	m_fInvisibleTime = 0.0f;

	m_kSplashArea.isEnableHitProcess=TRUE;
	m_kSplashArea.uSkill=0;
	m_kSplashArea.MotionKey=0;
	m_kSplashArea.fDisappearingTime = 0.0f;
	m_kSplashArea.SphereInstanceVector.clear();
	m_kSplashArea.HittedInstanceMap.clear();

	memset(&m_kCurMotNode, 0, sizeof(m_kCurMotNode));

	__ClearCombo();
}

void CActorInstance::__Initialize()
{
	m_pkCurRaceMotionData=NULL;
	m_pkCurRaceData=NULL;
	m_pkHorse=NULL;
	m_pkTree=NULL;

	m_fOwnerBaseTime=0.0f;

	m_eActorType = TYPE_PC;
	m_eRace = 0;

	m_eShape = 0;
	m_eHair = 0;

	m_dwSelfVID = 0;
	m_dwOwnerVID = 0;

	m_pkEventHandler =  NULL;

	m_PhysicsObject.Initialize();

	m_pAttributeInstance = NULL;

	m_pFlyEventHandler = 0;

	m_v3FishingPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_iFishingEffectID = -1;

	m_pkHorse = NULL;

	__InitializePositionData();
	__InitializeRotationData();
	__InitializeMotionData();
	__InitializeStateData();
	__InitializeCollisionData();

	__BlendAlpha_Initialize();

	ClearFlyTargeter();	
	m_bIsQuiverEquipped = false;
	m_dwQuiverEffectID = 0;
}

CActorInstance::CActorInstance()
{
	__Initialize();
	m_PhysicsObject.SetActorInstance(this);
}

CActorInstance::~CActorInstance()
{
	Destroy();
}