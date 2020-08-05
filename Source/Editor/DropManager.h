#pragma once
#include <oleidl.h>
#include "imgui.h"
#include "imgui_internal.h"
#include <vector>
#include <string>

// create a class inheriting from IDropTarget
class DropManager final : public IDropTarget
{
public:
	//--- implement the IUnknown parts
	// you could do this the proper way with InterlockedIncrement etc,
	// but I've left out stuff that's not exactly necessary for brevity
	ULONG AddRef() override { return 1; }
	ULONG Release() override { return 0; }

	// we handle drop targets, let others know
	HRESULT QueryInterface(REFIID riid, void** ppvObject) override;;


	//--- implement the IDropTarget parts

	// occurs when we drag files into our applications view
	HRESULT DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override;

	// occurs when we drag files out from our applications view
	HRESULT DragLeave() override;

	// occurs when we drag the mouse over our applications view whilst carrying files (post Enter, pre Leave)
	HRESULT DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override;

	// occurs when we release the mouse button to finish the drag-drop operation
	HRESULT Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override;

	static void Update();
	static bool GetDragDropFiles(std::vector<std::wstring>& aVec);
	static bool GetIsDragging();

private:
	static bool isDragging;
	static bool hasDropped;
	static std::vector<std::wstring> files;
};
