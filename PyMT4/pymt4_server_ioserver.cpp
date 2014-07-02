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


#include "pymt4_server_ioserver.h"

using namespace boost;

namespace PyMT4
{

/* ///////////////////////////// *
 * 
 * Static Variable Definition 
 *
 */

IOServerPtr IOServer::m_instance;


/* ///////////////////////////// *
 * 
 * IOServer Class Definition
 *
 */

int32_t IOServer::WindowUpdateMsg = RegisterWindowMessage(L"MetaTrader4_Internal_Message");

IOServer::~IOServer()
{
	shutdown();
}

IOServer::IOServer() :
m_iosWork(m_ioService),
m_acceptor(m_ioService),
_shutdownRequest(false)
{	
	m_iosThread.reset(new thread( boost::bind(&io_service::run,&m_ioService)));

	m_acceptor.open(tcp::v4());
	m_acceptor.bind(tcp::endpoint(tcp::v4(),DEFAULT_PORT));
	m_acceptor.listen();

	acceptSession();
}


void IOServer::acceptSession()
{
	IOSessionPtr pendingSession(new IOSession(m_ioService));
	m_acceptor.async_accept(pendingSession->socket(),bind(&IOServer::acceptHandler,this,pendingSession,asio::placeholders::error));
}

void IOServer::acceptHandler(IOSessionPtr session,const system::error_code& error)
{
	if(!error)
	{
		boost::mutex::scoped_lock scopedlock(_servermutex);
		_sessionList.insert(make_pair(session->id(),session.get()));
		session->initialize();
		acceptSession();
	}
}


void IOServer::registerChartWindow(const char* chartName,HWND chartHandle )
{
	_registeredWindowList[chartName] = chartHandle;
}

void IOServer::chartWindowNotify(const char* chartName,HWND chartHandle)
{
	//BOOST_FOREACH(IOSessionList::value_type sessionData,_sessionList)
	//{
	//	//CommandIdentifier commandId = sessionData.second->pendingCommand();
	//	//if ( commandId == CommandUnknown)
	//	//	continue;

	//	//_pendingCommandList.push_back(make_pair(sessionData.first,commandId));
	//}
}

void IOServer::requestChartsUpdate()
{
	BOOST_FOREACH(ChartWindowList::value_type chartWindowData,_registeredWindowList)
	{
		PostMessageA(chartWindowData.second,IOServer::WindowUpdateMsg,2,1);
	}

}


void IOServer::queueCommand(PendingCommandPtr command)
{
	boost::mutex::scoped_lock scopedlock(_servermutex);

	_pendingCommandList.push_back(command);
	_processCondition.notify_all();
}


int32_t IOServer::pendingCommand()
{
	boost::mutex::scoped_lock scopedLock(_servermutex);

	if (!_pendingCommandList.size())
	{
		_processCondition.timed_wait(scopedLock,boost::posix_time::milliseconds(1000));
	}

	CommandIdentifier commandId = CheckShutdownCondition;

	if (_pendingCommandList.size())
	{
		_currentCommand = _pendingCommandList.front();
		_pendingCommandList.pop_front();
		commandId = _currentCommand->commandId;
	}
	return (int)commandId;
}



const char* IOServer::getStringArgument(char* stringbuffer)
{
	std::string result;
	Serializer<std::string>::deserializeItem(&result,&_currentCommand->dataPos);

	std::copy(result.begin(),result.end(),stringbuffer);
	stringbuffer[result.size()] = NULL;
	
	return stringbuffer;
}

void IOServer::shutdown()
{

	{
		boost::mutex::scoped_lock scopedlock(_servermutex);
		BOOST_FOREACH(IOSessionMap::value_type& sessionData,_sessionList)
		{
			IOSession* session = sessionData.second;
			session->disconnect();
		}

		m_ioService.stop();
	}


	m_iosThread->join();
	IOServer::m_instance.reset();
}

void IOServer::notifyResult(const MessageTypeIdentifier&,const MessageUID& replyToUid)
{
	boost::mutex::scoped_lock scopedlock(_servermutex);

	_onTickResults[replyToUid]->setResult(Buffer().begin(),Buffer().begin());
}

bool IOServer::dispatchOnTick(const std::string& symbol, const double& bid, const double& ask)
{
	{
		boost::mutex::scoped_lock scopedlock(_servermutex);

		_onTickResults.clear();
		BOOST_FOREACH(IOSessionMap::value_type& sessionData,_sessionList)
		{

			MessageEventPtr messageEvent = MessageEvent::Create(EvtId_OnTick);

			Buffer& buffer = messageEvent->messageBuffer();

			Serializer<std::string>::serializeItem(const_cast<std::string*>(&symbol),&buffer);
			Serializer<double>::serializeItem(const_cast<double*>(&bid),&buffer);
			Serializer<double>::serializeItem(const_cast<double*>(&ask),&buffer);


			IOSession* session = sessionData.second;
			session->writeMessage(messageEvent);

			_onTickResults[messageEvent->uid()] = PendingResult::Create();

		}

	}
	
	BOOST_FOREACH(OnTickResultList::value_type& resultItem, _onTickResults)
	{
		resultItem.second->waitForResult<bool>();
	}


	return true;
}

IOServerPtr IOServer::Instance()
{
	if (!m_instance)
	{

		m_instance.reset(new IOServer);

	}
	return m_instance;
}

}