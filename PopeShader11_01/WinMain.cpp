#pragma once
#include "WfWindow.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	WfWindow wf;

	if( FAILED( wf.InitWindow( hInstance, nCmdShow ) ) )
		return -1;

	if( !wf.Init() )
		return -1;

	int ret = wf.Run();

	wf.CleanUp();

	return ret;
}

