/* (C) Copyright 2013 Rob Watson rmawatson [at] hotmail.com  and others.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * Contributors:
 *     Rob Watson ( rmawatson [at] hotmail )
 *
 */

 
#include "pymt4_server_plugin.h"
#include "pymt4_server_ioserver.h"
#include <windows.h>

/* ///////////////////////////// *
 * 
 * Entry Points
 *
 */

#ifndef _DEBUG


std::string lastOwner = "";

struct DebugConsole {
	DebugConsole() {
		AllocConsole();
		*stdout = *_fdopen(_open_osfhandle((intptr_t) GetStdHandle(STD_OUTPUT_HANDLE), _O_WRONLY), "a");
		*stderr = *_fdopen(_open_osfhandle((intptr_t) GetStdHandle(STD_ERROR_HANDLE), _O_WRONLY), "a");
		*stdin = *_fdopen(_open_osfhandle((intptr_t) GetStdHandle(STD_INPUT_HANDLE), _O_WRONLY), "r"); }
};

DebugConsole _console;
#endif


namespace PyMT4 {

BOOL isInitialized = FALSE;

boost::mutex pymt4_mutex;

EXPORT(BOOL) pymt4_initialize(const char* windowName,int32_t updateWindowHandle)
{
	boost::mutex::scoped_lock pymt4_lock(pymt4_mutex);

	if (isInitialized)
		return FALSE;

	IOServerPtr ioserver = IOServer::Instance();

	ioserver->registerChartWindow(windowName,(HWND)updateWindowHandle);

	isInitialized = TRUE;

	return TRUE;
}

EXPORT(BOOL) pymt4_isinitialized()
{	
	boost::mutex::scoped_lock pymt4_lock(pymt4_mutex);
	return isInitialized;
}

EXPORT(bool) pymt4_uninitialize(const char* windowName,int32_t updateWindowHandle)
{
	boost::mutex::scoped_lock pymt4_lock(pymt4_mutex);
	IOServerPtr ioserver = IOServer::Instance();
	ioserver->shutdown();
	
	isInitialized = FALSE;

	return TRUE;
}


EXPORT(BOOL) pymt4_notify(const char* windowName,int32_t updateWindowHandle)
{
	boost::mutex::scoped_lock pymt4_lock(pymt4_mutex);
	if (isInitialized)
	{
		IOServerPtr ioserver = IOServer::Instance();
		ioserver->chartWindowNotify(windowName,(HWND)updateWindowHandle);
	}
	return TRUE;
}

EXPORT(double) pymt4_getDoubleArgument()
{

	IOServerPtr ioserver = IOServer::Instance();
	return ioserver->getPodArgument<double>();
}

EXPORT(BOOL) pymt4_getBoolArgument()
{
	IOServerPtr ioserver = IOServer::Instance();
	return (BOOL)ioserver->getPodArgument<bool>();
}

EXPORT(const char*) pymt4_getStringArgument(char* string)
{

	IOServerPtr ioserver = IOServer::Instance();
	return ioserver->getStringArgument(string);
}

EXPORT(int) pymt4_getIntArgument()
{

	IOServerPtr ioserver = IOServer::Instance();
	return ioserver->getPodArgument<int>();
}


EXPORT(int) pymt4_requestPendingCommand()
{

	IOServerPtr ioserver = IOServer::Instance();

	return ioserver->pendingCommand();
}

EXPORT(BOOL) pymt4_setDoubleResult(double result,int32_t error)
{
	IOServerPtr ioserver = IOServer::Instance();
	ioserver->completeCommand<double>(result,error);
	return TRUE;
}

EXPORT(BOOL) pymt4_setStringResult(const char* result,int32_t error)
{
	std::string resultString(result);
	IOServerPtr ioserver = IOServer::Instance();
	ioserver->completeCommand<std::string>(resultString,error);
	return TRUE;
}

EXPORT(BOOL) pymt4_setIntResult(int32_t result,int32_t error)
{
	IOServerPtr ioserver = IOServer::Instance();
	ioserver->completeCommand<int32_t>(result,error);
	return TRUE;
}

EXPORT(BOOL) pymt4_setBoolResult(BOOL result,int32_t error)
{
	IOServerPtr ioserver = IOServer::Instance();
	ioserver->completeCommand<bool>((bool)result,error);
	return TRUE;
}

EXPORT(BOOL) pymt4_notifyOnTick(const char* symbol,double bid,double ask)
{
	if (isInitialized)
	{

		boost::mutex::scoped_lock pymt4_lock(pymt4_mutex);

		IOServerPtr ioserver = IOServer::Instance();


		ioserver->dispatchOnTick(symbol,bid,ask);
		return TRUE;
	}

	return FALSE;

}	


}

