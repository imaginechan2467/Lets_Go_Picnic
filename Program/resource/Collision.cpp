/*==============================================================================

   [Collision.cpp]
														 Author : takamizawa
--------------------------------------------------------------------------------
	memo:
==============================================================================*/

//*****************************************************************************
// INCLUDE
//*****************************************************************************
#include"Collision.h"
#include "Player.h"

//*****************************************************************************
// �ÓI�����o�ϐ���`
//*****************************************************************************

//*****************************************************************************
// �֐���`
//*****************************************************************************
bool Collision::CollisionCircleAndCircleHit(const CollisionCircle * pA, const CollisionCircle * pB)
{
	vec3f vec = pA->center - pB->center;
	return D3DXVec3LengthSq(&vec) < (pA->radius + pB->radius) * (pA->radius + pB->radius);
}

Collision::ColInfo_2 Collision::GetBoxVertexHit(const vec3f & Apos,
	const vec3f & Asize, const vec3f & Bpos, const vec3f & Bsize, DIRfor3D preDir)
{
	ColInfo_2 info = {};
	memset(&info, 0, sizeof(info));

	//��O�E��
	if ((Apos.x + Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x + Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&//X
		(Apos.z - Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z - Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f) &&
		(Apos.y - Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y - Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f))
	{
		info.hitvertex[cubedir_RUF] = true;
		if (preDir == dir3_right)
		{
			info.adjustpos.x = Bpos.x - Bsize.x * 0.5f - Asize.x * 0.5f;
		}
		else if (preDir == dir3_under)
		{
			info.adjustpos.y = Bpos.y + Bsize.y * 0.5f + Asize.y * 0.5f;
		}
		else if (preDir == dir3_back)
		{
			info.adjustpos.z = Bpos.z + Bsize.z * 0.5f + Asize.z * 0.5f;
		}
	}

	//���E��
	if ((Apos.x + Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x + Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&//X
		(Apos.z + Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z + Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f) &&
		(Apos.y - Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y - Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f))
	{
		info.hitvertex[cubedir_RUB] = true;
		if (preDir == dir3_right)
		{
			info.adjustpos.x = Bpos.x - Bsize.x * 0.5f - Asize.x * 0.5f;
		}
		else if (preDir == dir3_under)
		{
			info.adjustpos.y = Bpos.y + Bsize.y * 0.5f + Asize.y * 0.5f;
		}
		else if (preDir == dir3_forward)
		{
			info.adjustpos.z = Bpos.z - Bsize.z * 0.5f - Asize.z * 0.5f;
		}
	}

	//��O�E��
	if ((Apos.x + Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x + Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&//X
		(Apos.z - Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z - Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f) &&
		(Apos.y + Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y + Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f))
	{
		info.hitvertex[cubedir_RTF] = true;
		if (preDir == dir3_right)
		{
			info.adjustpos.x = Bpos.x - Bsize.x * 0.5f - Asize.x * 0.5f;
		}
		else if (preDir == dir3_top)
		{
			info.adjustpos.y = Bpos.y - Bsize.y * 0.5f - Asize.y * 0.5f;
		}
		else if (preDir == dir3_back)
		{
			info.adjustpos.z = Bpos.z + Bsize.z * 0.5f + Asize.z * 0.5f;
		}
	}

	//���E��
	if ((Apos.x + Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x + Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&//X
		(Apos.z + Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z + Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f) &&
		(Apos.y + Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y + Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f))
	{
		info.hitvertex[cubedir_RTB] = true;
		if (preDir == dir3_right)
		{
			info.adjustpos.x = Bpos.x - Bsize.x * 0.5f - Asize.x * 0.5f;
		}
		else if (preDir == dir3_top)
		{
			info.adjustpos.y = Bpos.y - Bsize.y * 0.5f - Asize.y * 0.5f;
		}
		else if (preDir == dir3_forward)
		{
			info.adjustpos.z = Bpos.z - Bsize.z * 0.5f - Asize.z * 0.5f;
		}
	}

	//=====================��

	//��O����
	if ((Apos.x - Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x - Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&//X
		(Apos.z - Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z - Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f) &&
		(Apos.y - Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y - Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f))
	{
		info.hitvertex[cubedir_LUF] = true;
		if (preDir == dir3_left)
		{
			info.adjustpos.x = Bpos.x + Bsize.x * 0.5f + Asize.x * 0.5f;
		}
		else if (preDir == dir3_under)
		{
			info.adjustpos.y = Bpos.y + Bsize.y * 0.5f + Asize.y * 0.5f;
		}
		else if (preDir == dir3_back)
		{
			info.adjustpos.z = Bpos.z + Bsize.z * 0.5f + Asize.z * 0.5f;
		}
	}

	//������
	if ((Apos.x - Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x - Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&//X
		(Apos.z + Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z + Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f) &&
		(Apos.y - Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y - Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f))
	{
		info.hitvertex[cubedir_LUB] = true;
		if (preDir == dir3_left)
		{
			info.adjustpos.x = Bpos.x + Bsize.x * 0.5f + Asize.x * 0.5f;
		}
		else if (preDir == dir3_under)
		{
			info.adjustpos.y = Bpos.y + Bsize.y * 0.5f + Asize.y * 0.5f;
		}
		else if (preDir == dir3_forward)
		{
			info.adjustpos.z = Bpos.z - Bsize.z * 0.5f - Asize.z * 0.5f;
		}
	}

	//��O����
	if ((Apos.x - Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x - Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&//X
		(Apos.z - Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z - Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f) &&
		(Apos.y + Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y + Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f))
	{
		info.hitvertex[cubedir_LTF] = true;
		if (preDir == dir3_left)
		{
			info.adjustpos.x = Bpos.x + Bsize.x * 0.5f + Asize.x * 0.5f;
		}
		else if (preDir == dir3_top)
		{
			info.adjustpos.y = Bpos.y - Bsize.y * 0.5f - Asize.y * 0.5f;
		}
		else if (preDir == dir3_back)
		{
			info.adjustpos.z = Bpos.z + Bsize.z * 0.5f + Asize.z * 0.5f;
		}
	}

	//������
	if ((Apos.x - Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x - Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&//X
		(Apos.z + Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z + Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f) &&
		(Apos.y + Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y + Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f))
	{
		info.hitvertex[cubedir_LTB] = true;
		if (preDir == dir3_left)
		{
			info.adjustpos.x = Bpos.x + Bsize.x * 0.5f + Asize.x * 0.5f;
		}
		else if (preDir == dir3_top)
		{
			info.adjustpos.y = Bpos.y - Bsize.y * 0.5f - Asize.y * 0.5f;
		}
		else if (preDir == dir3_forward)
		{
			info.adjustpos.z = Bpos.z - Bsize.z * 0.5f - Asize.z * 0.5f;
		}
	}

	return info;
}

bool Collision::GetBoxColSimple(const vec3f & Apos, const vec3f & Asize, 
	const vec3f & Bpos, const vec3f & Bsize)
{
	/*if ((Apos.x + Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x - Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&
		(Apos.y + Asize.y * 0.5f <= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y - Asize.y * 0.5f >= Bpos.y + Bsize.y * 0.5f) &&
		(Apos.z + Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z - Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f))
	{
		return true;
	}
	return false;*/

	//X�����Ԃ��Ă邩�ǂ���
	//if (!(((Apos.x - Asize.x * 0.5f <= Bpos.x - Bsize.x * 0.5f) &&
	//	(Apos.x + Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f)) ||
	//	((Apos.x - Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&
	//	(Apos.x + Asize.x * 0.5f >= Bpos.x + Bpos.x * 0.5f)) ||
	//	((Apos.x - Asize.x * 0.5f <= Bpos.x -Bsize.x * 0.5f) &&
	//	(Apos.x + Asize.x * 0.5f >= Bpos.x + Bsize.x * 0.5f))))
	//{
	//	return false;
	//}

	////Y
	//if (!(((Apos.y - Asize.y * 0.5f <= Bpos.y - Bsize.y * 0.5f) &&
	//	(Apos.y + Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f)) ||
	//	((Apos.y - Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f) &&
	//	(Apos.y + Asize.y * 0.5f >= Bpos.y + Bsize.y * 0.5f)) ||
	//	((Apos.y - Asize.y * 0.5f <= Bpos.y - Bsize.y * 0.5f) &&
	//	(Apos.y + Asize.y * 0.5f >= Bpos.y + Bsize.y * 0.5f))))
	//{
	//	return false;
	//}

	////Z
	//if (!(((Apos.z - Asize.z * 0.5f <= Bpos.z - Bsize.z * 0.5f) &&
	//	(Apos.z + Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f)) ||
	//	((Apos.z - Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
	//	(Apos.z + Asize.z * 0.5f >= Bpos.z + Bsize.z * 0.5f)) ||
	//	((Apos.z - Asize.z * 0.5f <- Bpos.z - Bsize.z * 0.5f) &&
	//	(Apos.z + Asize.z * 0.5f >= Bpos.z + Bsize.z * 0.5f))))
	//{
	//	return false;
	//}

	//return true;

	vec3f length = Bpos - Apos;
	float flength = D3DXVec3Length(&length);

	if (flength <= 1.0f + 1.0f)
	{
		return true;
	}
	return false;

}

bool Collision::GetBoxOnlyHit(const vec3f & Apos, const vec3f & Asize, 
	const vec3f & Bpos, const vec3f & Bsize)
{
	//��O�E��
	if ((Apos.x + Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x + Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&//X
		(Apos.z - Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z - Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f) &&
		(Apos.y - Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y - Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f))
	{
		return true;
	}

	//���E��
	if ((Apos.x + Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x + Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&//X
		(Apos.z + Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z + Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f) &&
		(Apos.y - Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y - Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f))
	{
		return true;
	}

	//��O�E��
	if ((Apos.x + Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x + Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&//X
		(Apos.z - Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z - Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f) &&
		(Apos.y + Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y + Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f))
	{
		return true;
	}

	//���E��
	if ((Apos.x + Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x + Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&//X
		(Apos.z + Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z + Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f) &&
		(Apos.y + Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y + Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f))
	{
		return true;
	}

	//=====================��

	//��O����
	if ((Apos.x - Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x - Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&//X
		(Apos.z - Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z - Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f) &&
		(Apos.y - Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y - Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f))
	{
		return true;
	}

	//������
	if ((Apos.x - Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x - Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&//X
		(Apos.z + Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z + Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f) &&
		(Apos.y - Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y - Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f))
	{
		return true;
	}

	//��O����
	if ((Apos.x - Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x - Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&//X
		(Apos.z - Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z - Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f) &&
		(Apos.y + Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y + Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f))
	{
		return true;
	}

	//������
	if ((Apos.x - Asize.x * 0.5f >= Bpos.x - Bsize.x * 0.5f) &&
		(Apos.x - Asize.x * 0.5f <= Bpos.x + Bsize.x * 0.5f) &&//X
		(Apos.z + Asize.z * 0.5f <= Bpos.z + Bsize.z * 0.5f) &&
		(Apos.z + Asize.z * 0.5f >= Bpos.z - Bsize.z * 0.5f) &&
		(Apos.y + Asize.y * 0.5f >= Bpos.y - Bsize.y * 0.5f) &&
		(Apos.y + Asize.y * 0.5f <= Bpos.y + Bsize.y * 0.5f))
	{
		return true;
	}

	return false;
}
