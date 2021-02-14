#pragma once

#include <DirectXMath.h>
using namespace DirectX;

struct ColorConstantBuffer
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

struct CBNeverChanges
{
	XMMATRIX view;
};
struct CBChangeOnResize
{
	XMMATRIX projection;
};
struct CBChangeEveryFrame
{
	XMMATRIX world;
};