#include "pch.h"
#include "CAnimation2D.h"

#include "CAnimator2D.h"
#include "CTimeMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "Ptr.h"


CAnimation2D::CAnimation2D()
	: m_pOwner(nullptr)
	, m_iCurFrmIdx(0)
	, m_fAccTime(false)
	, m_bFinish(false)
{
}

CAnimation2D::~CAnimation2D()
{
}

void CAnimation2D::finalupdate()
{

	if (m_bFinish)
		return;

	m_fAccTime += DT;

	if (m_vecFrm[m_iCurFrmIdx].fDuration < m_fAccTime)
	{
		m_fAccTime -= m_vecFrm[m_iCurFrmIdx].fDuration;
		
		if (m_vecFrm.size() - 1 <= m_iCurFrmIdx)
		{
			m_bFinish = true;
		}
		else
		{
			++m_iCurFrmIdx;
		}		
	}
}

void CAnimation2D::UpdateData()
{
	static CConstBuffer* pBuffer = CDevice::GetInst()->GetCB(CB_TYPE::ANIM2D);

	tAnim2D info = {};
	info.useAnim2D = 1;
	info.Atlas_Width = m_pAtlasTex->Width();
	info.Atlas_Height = m_pAtlasTex->Height();

	info.vBackgroundSize = m_vBackgroundSize;
	info.vLT = m_vecFrm[m_iCurFrmIdx].vLT;	
	info.vSlice = m_vecFrm[m_iCurFrmIdx].vSlice;
	info.vOffset = m_vecFrm[m_iCurFrmIdx].vOffset;

	pBuffer->SetData(&info, sizeof(tAnim2D));
	pBuffer->UpdateData();


	m_pAtlasTex->UpdateData((int)PIPELINE_STAGE::PS, 10);

}

void CAnimation2D::Create(Ptr<CTexture> _Atlas, Vec2 _vBackgroundSizePixel, Vec2 _vLT, Vec2 _vSlice, Vec2 _vStep
	, float _fDuration, int _iFrameCount)
{
	assert(_Atlas.Get());

	m_pAtlasTex = _Atlas;
	
	float fWidth = m_pAtlasTex->Width();
	float fHeight = m_pAtlasTex->Height();

	// 픽셀 좌표를 0~1 UV 로 전환
	Vec2 vLT = _vLT / Vec2(fWidth, fHeight);
	Vec2 vSlice = _vSlice / Vec2(fWidth, fHeight);
	Vec2 vStep = _vStep / Vec2(fWidth, fHeight);

	m_vBackgroundSize = _vBackgroundSizePixel / Vec2(fWidth, fHeight);

	// 프레임 정보 생성
	for (int i = 0; i < _iFrameCount; ++i)
	{
		tAnim2DFrame frm = {};
		
		frm.vLT = vLT + (vStep * (float)i);
		frm.vSlice = vSlice;
		frm.fDuration = _fDuration;

		m_vecFrm.push_back(frm);
	}
}

void CAnimation2D::Create(Ptr<CTexture> _Atlas, vector<AnimToolData> _vec)
{
	assert(_Atlas.Get());

	m_pAtlasTex = _Atlas;

	float fWidth = m_pAtlasTex->Width();
	float fHeight = m_pAtlasTex->Height();

	m_vBackgroundSize = _vec[0].BackGroundSize;

	// 프레임 정보 생성
	for (int i = 0; i < _vec.size(); ++i)
	{
		tAnim2DFrame frm = {};

		frm.vLT = _vec[i].LTPos;
		frm.vSlice = _vec[i].ImgScale;
		frm.fDuration = _vec[i].duration;
		frm.vOffset = _vec[i].Offset;
		m_vecFrm.push_back(frm);
	}
}

void CAnimation2D::Create(Ptr<CTexture> _Atlas, CAnimation2D* _anim)
{
	assert(_Atlas.Get());

	m_pAtlasTex = _Atlas;
	CAnimation2D* pAnim = _anim;
	pAnim->GetBackGroundSize();
	vector<tAnim2DFrame> pVecFrm = pAnim->GetvecFrm();
	int FrameCount = pAnim->GetvecFrm().size();
	float fWidth = m_pAtlasTex->Width();
	float fHeight = m_pAtlasTex->Height();

	m_vBackgroundSize = pAnim->GetBackGroundSize() / Vec2(fWidth, fHeight);

	// 프레임 정보 생성
	for (int i = 0; i < FrameCount; ++i)
	{
		// 픽셀 좌표를 0~1 UV 로 전환
		Vec2 vLT = (pVecFrm[i].vLT) / Vec2(fWidth, fHeight);
		Vec2 vSlice = pVecFrm[i].vSlice / Vec2(fWidth, fHeight);
		Vec2 vOffset = pVecFrm[i].vOffset / Vec2(fWidth, fHeight);
		tAnim2DFrame frm = {};

		frm.vLT = vLT;
		frm.vSlice = vSlice;
		frm.fDuration = pVecFrm[i].fDuration;;
		frm.vOffset = vOffset;
		m_vecFrm.push_back(frm);
	}
}

void CAnimation2D::SaveToScene(FILE* _pFile)
{
	CEntity::SaveToScene(_pFile);
	
	size_t i = m_vecFrm.size();
	fwrite(&i, sizeof(size_t), 1, _pFile);
	fwrite(m_vecFrm.data(), sizeof(tAnim2DFrame), i, _pFile);
	fwrite(&m_vBackgroundSize, sizeof(Vec2), 1, _pFile);
	
	SaveResPtr(m_pAtlasTex, _pFile);
}

void CAnimation2D::LoadFromScene(FILE* _pFile)
{
	CEntity::LoadFromScene(_pFile);

	size_t i = 0;
	fread(&i, sizeof(size_t), 1, _pFile);
	m_vecFrm.resize(i);
	fread(m_vecFrm.data(), sizeof(tAnim2DFrame), i, _pFile);
	fread(&m_vBackgroundSize, sizeof(Vec2), 1, _pFile);

	LoadResPtr(m_pAtlasTex, _pFile);
}

void CAnimation2D::SaveAnimFile(FILE* _pFile)
{
	size_t i = m_vecFrm.size();
	fwrite(&i, sizeof(size_t), 1, _pFile);
	fwrite(m_vecFrm.data(), sizeof(tAnim2DFrame), i, _pFile);
	fwrite(&m_vBackgroundSize, sizeof(Vec2), 1, _pFile);

	SaveResPtr(m_pAtlasTex, _pFile);
}

void CAnimation2D::LoadAnimFile(FILE* _pFile)
{
	size_t i = 0;
	fread(&i, sizeof(size_t), 1, _pFile);
	m_vecFrm.resize(i);
	fread(m_vecFrm.data(), sizeof(tAnim2DFrame), i, _pFile);
	fread(&m_vBackgroundSize, sizeof(Vec2), 1, _pFile);

	LoadResPtr(m_pAtlasTex, _pFile);
	int n = 0;
}
