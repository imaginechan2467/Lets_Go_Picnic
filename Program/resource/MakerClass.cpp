
#include "MakerClass.h"
#include "texture.h"


MakerClass::MakerClass()
{

}

MakerClass::~MakerClass()
{

}

void MakerClass::Draw(void)
{
}

void MakerClass::Update(void)
{
}

void MakerClass::DrawAll(void)
{
}

void MakerClass::UpdateAll(void)
{
}

void MakerClass::SetType(int type)
{
}

void MakerClass::Refresh(void)
{
	pos = {};
	size = { 1.0f,1.0f,1.0f };
	isActive = true;
	objType = objtype_system;
	color = COLOR_WHITE_DEBUG;
	stageNumber = GetCurrentStage();
}
