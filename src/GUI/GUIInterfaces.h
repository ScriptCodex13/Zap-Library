#pragma once

#ifndef GUI_INTERFACES_H
#define GUI_INTERFACES_H

/*
There goes GUI contract for developers
//*/
#include<array>

namespace zap
{
	class IUIButtonContainer
	{
	public:
		virtual std::array<int, 2> GetClientSize() = 0;
		virtual std::array<int, 2> GetClientOriginalSize() = 0;
	};

	//for controls
	class IUIButtonEventListener
	{
	public:
		virtual void SetContainer(IUIButtonContainer* container) = 0;
		virtual bool HitTest(double x, double y) = 0;
		virtual bool OnMouseMove(double x, double y) = 0;
		virtual bool OnLMouseButtonDown(double x, double y) = 0;
		virtual bool OnLMouseButtonUp(double x, double y) = 0;
		virtual bool OnMouseEnter(double x, double y) = 0;
		virtual bool OnMouseLeave(double x, double y) = 0;
		virtual bool OnPress(double x, double y, int key) = 0;
		virtual bool OnRelease(double x, double y, int key) = 0;

		virtual bool OnLMouseClick(double x, double y) = 0;

	};

	//fancy type alias for IUIButtonEventListener
	using IUIButtonEventHandler = IUIButtonEventListener;
	class IUIComponent
	{
	public:
		virtual IUIButtonEventListener* GetUIListener() = 0;
	};


	//for containers
	class IUIButtonEventProvider
	{
	public:
		virtual void AddButtonEventHandler(IUIButtonEventListener* handler) = 0;
		//TODO: Is revoke really needed?
		//virtual void RevokeButtonEventHandler(IUIButtonEventListener* handler) = 0;
		virtual void InvokeDefaultHandlers() = 0;
		virtual void InvokeLMouseClickHandlers() = 0;

	};
}
#endif