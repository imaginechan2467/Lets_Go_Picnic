
#include "BlockMaker.h"
#include "texture.h"

BlockClass::BlockType BlockMakerClass::currentBlockType;
int BlockMakerClass::blockMakerTexture;

BlockMakerClass::BlockMakerClass()
{

}

BlockMakerClass::~BlockMakerClass()
{

}

void BlockMakerClass::Init(void)
{
	blockMakerTexture = SetTexture(BLOCK_TEXTURE_FILENAME, {1,1});
	TextureLoad("ブロックメーカー");
}

void BlockMakerClass::Uninit(void)
{
	TextureRelease(&blockMakerTexture, 1);
	blockMakerTexture = NULL;
}

void BlockMakerClass::UpdateAll(void)
{
}

void BlockMakerClass::DrawAll(void)
{
}

void BlockMakerClass::Update(void)
{

}

void BlockMakerClass::Draw(void)
{

}

void BlockMakerClass::SaveAll(void)
{
}

void BlockMakerClass::LoadAll(void)
{
}

void BlockMakerClass::ResetAll(bool isDeleteAll)
{
}

void BlockMakerClass::Delete(int index)
{
}

void BlockMakerClass::Create(void)
{
}

void BlockMakerClass::SetType(int type)
{
}

int BlockMakerClass::GetDetailType(void)
{
	return 0;
}
