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

#include "pymt4_client_command.h"

namespace PyMT4
{
	
	LastErrorMap _lastErrorMap;
	boost::mutex _lastErrorMapLock;
	boost::mutex _dispatchLock;
	namespace Command
	{
		double AccountBalance()
		{return DispatchFunction<double>()(PyMT4::CmdId_AccountBalance);}

		double AccountCredit()
		{return DispatchFunction<double>()(PyMT4::CmdId_AccountCredit);}

		std::string AccountCompany()
		{return DispatchFunction<std::string>()(PyMT4::CmdId_AccountCompany);}

		std::string AccountCurrency()
		{return DispatchFunction<std::string>()(PyMT4::CmdId_AccountCurrency);}

		double AccountEquity()
		{return DispatchFunction<double>()(PyMT4::CmdId_AccountEquity);}

		double AccountFreeMargin()
		{return DispatchFunction<double>()(PyMT4::CmdId_AccountFreeMargin);}

		double AccountFreeMarginCheck(const std::string& symbol,const int32_t& cmd,const double& volume)
		{return DispatchFunction<double,const std::string& ,const int32_t& ,const double&>()(PyMT4::CmdId_AccountFreeMarginCheck,symbol,cmd,volume); }

		double AccountFreeMarginMode()
		{return DispatchFunction<double>()(PyMT4::CmdId_AccountFreeMarginMode);}

		int32_t AccountLeverage()
		{return DispatchFunction<int32_t>()(PyMT4::CmdId_AccountLeverage);}

		double AccountMargin()
		{return DispatchFunction<double>()(PyMT4::CmdId_AccountMargin);}

		std::string AccountName()
		{return DispatchFunction<std::string>()(PyMT4::CmdId_AccountName);}

		int32_t AccountNumber()
		{return DispatchFunction<int32_t>()(PyMT4::CmdId_AccountNumber);}

		double AccountProfit()
		{return DispatchFunction<double>()(PyMT4::CmdId_AccountProfit);}

		std::string AccountServer()
		{return DispatchFunction<std::string>()(PyMT4::CmdId_AccountServer);}

		int32_t AccountStopoutLevel()
		{return DispatchFunction<int32_t>()(PyMT4::CmdId_AccountStopoutLevel);}

		int32_t AccountStopoutMode()
		{return DispatchFunction<int32_t>()(PyMT4::CmdId_AccountStopoutMode);}


		/*Timeseries http://docs.mql4.com/series */


		int32_t iBars(const std::string& symbol,const int32_t& timeframe)
		{return DispatchFunction<int32_t,const std::string&,const int32_t&>()(PyMT4::CmdId_iBars,symbol,timeframe);}

		int32_t		iBarShift(const std::string& symbol,const Datetime& datetime,const bool& exact)
		{return DispatchFunction<int32_t,const std::string,const Datetime,const bool>()(CmdId_iBarShift,symbol,datetime,(BOOL)exact);}

		double		iClose(const std::string& symbol,const int32_t& timeframe, const int32_t& shift)
		{return DispatchFunction<double,const std::string,const int32_t,const int32_t>()(CmdId_iClose,symbol,timeframe,shift);}

		double		iHigh(const std::string& symbol, const int32_t& timeframe, const int32_t& shift)
		{return DispatchFunction<double,const std::string,const int32_t,const int32_t>()(CmdId_iHigh,symbol,timeframe,shift);}

		double		iHighest(const std::string& symbol, const int32_t& timeframe,const int32_t& type,const int32_t& count,const int32_t& start)
		{return DispatchFunction<double,const std::string,const int32_t,const int32_t,const int32_t,const int32_t>()(CmdId_iHighest,symbol,timeframe,type,count,start);}

		double		iLow(const std::string& symbol, const int32_t& timeframe,const int32_t& shift)
		{return DispatchFunction<double,const std::string,const int32_t,const int32_t,const int32_t,const int32_t>()(CmdId_iLow,symbol,timeframe,shift);}

		double		iLowest(const std::string& symbol, const int32_t& timeframe,const int32_t& type,const int32_t& count,const int32_t& start)
		{return DispatchFunction<double,const std::string,const int32_t,const int32_t,const int32_t,const int32_t>()(CmdId_iLowest,symbol,timeframe,type,count,start);}

		double		iOpen(const std::string& symbol, const int32_t& timeframe,const int32_t& shift)
		{return DispatchFunction<double,const std::string,const int32_t,const int32_t,const int32_t,const int32_t>()(CmdId_iOpen,symbol,timeframe,shift);}

		Datetime    iTime(const std::string& symbol, const int32_t& timeframe, const int32_t& shift)
		{return DispatchFunction<Datetime,const std::string,const int32_t,const int32_t,const int32_t,const int32_t>()(CmdId_iTime,symbol,timeframe,shift);}

		double		iVolume(const std::string& symbol, const int32_t& timeframe, const int32_t& shift)
		{return DispatchFunction<double,const std::string,const int32_t,const int32_t,const int32_t,const int32_t>()(CmdId_iVolume,symbol,timeframe,shift);}


		///* Trading Functions http://docs.mql4.com/trading */

		bool		OrderClose(const int32_t& ticket, const double& lots,const double& price, const int32_t& slippage, const Color& color)
		{return DispatchFunction<bool,const int32_t,const double,const double,const int32_t,const int32_t,const Color>()(CmdId_OrderClose,ticket,lots,price,slippage,color);}


		bool		OrderCloseBy(const int32_t& ticket, const double& opposite,const Color& color)
		{return DispatchFunction<bool,const int32_t,const double,const Color>()(CmdId_OrderCloseBy,ticket,opposite,color);}

		double		OrderClosePrice(const int32_t& ticket)		/* OrderSelect() */
		{return DispatchFunction<double,const int32_t>()(CmdId_OrderClosePrice,ticket);}

		Datetime	OrderCloseTime(const int32_t& ticket)		/* OrderSelect() */
		{return DispatchFunction<Datetime,const int32_t>()(CmdId_OrderCloseTime,ticket);}

		std::string OrderComment(const int32_t& ticket) 		/* OrderSelect() */
		{return DispatchFunction<std::string,const int32_t>()(CmdId_OrderComment,ticket);}

		double		OrderCommission(const int32_t& ticket)		/* OrderSelect() */
		{return DispatchFunction<double,const int32_t>()(CmdId_OrderCommission,ticket);}

		bool		OrderDelete(const int32_t& ticket,const Color& color) 
		{return DispatchFunction<bool,const int32_t,Color>()(CmdId_OrderDelete,ticket,color);}

		Datetime	OrderExpiration(const int32_t& ticket)		/* OrderSelect() */
		{return DispatchFunction<Datetime,const int32_t>()(CmdId_OrderExpiration,ticket);}

		double		OrderLots(const int32_t& ticket)			/* OrderSelect() */
		{return DispatchFunction<double,const int32_t>()(CmdId_OrderLots,ticket);}

		int32_t		OrderMagicNumber(const int32_t& ticket) 	/* OrderSelect() */
		{return DispatchFunction<int32_t,const int32_t>()(CmdId_OrderMagicNumber,ticket);}

		bool		OrderModify(const int32_t& ticket,const double& price, const double& stoploss, const double& takeprofit, const Datetime& expiration, const Color& arrow_color)
		{return DispatchFunction<bool,const int32_t,const double,const double, const double, const Datetime,const Color>()(CmdId_OrderModify,ticket,price,stoploss,takeprofit,expiration,arrow_color);}

		double		OrderOpenPrice(const int32_t& ticket)		/* OrderSelect() */
		{return DispatchFunction<double,const int32_t>()(CmdId_OrderOpenPrice,ticket);}

		Datetime	OrderOpenTime(const int32_t& ticket)		/* OrderSelect() */
		{return DispatchFunction<Datetime,const int32_t>()(CmdId_OrderOpenTime,ticket);}

		bool		OrderPrint(const int32_t& ticket)			/* OrderSelect() */
		{return DispatchFunction<bool,const int32_t>()(CmdId_OrderPrint,ticket);}

		double		OrderProfit(const int32_t& ticket)			/* OrderSelect() */
		{return DispatchFunction<double,const int32_t>()(CmdId_OrderProfit,ticket);}

		int32_t		OrderSelect(const int32_t& index, const int32_t& select, const int32_t& pool)
		{return DispatchFunction<int32_t,const int32_t,const int32_t,const int32_t>()(CmdId_OrderSelect,index,select,pool);}

		int32_t		OrderSend(
								const std::string& symbol, 
								const int32_t& cmd, 
								const double& volume, 
								const double& price, 
								const int32_t& slippage, 
								const double& stoploss, 
								const double& takeprofit, 
								const std::string& comment,
								const int32_t& magic,
								const Datetime& expiration,
								const Color& arrow_color
							)
		{return DispatchFunction<int32_t,const std::string, const int32_t,const double,const double,const int32_t,const double,const double,const std::string,const int32_t,const Datetime, const Color>()
			(CmdId_OrderSend,
			symbol,
			cmd,
			volume,
			price,
			slippage,
			stoploss,
			takeprofit,
			comment,
			magic,
			expiration,
			arrow_color);}


		int32_t		OrdersHistoryTotal()
		{return DispatchFunction<int32_t>()(CmdId_OrdersHistoryTotal);}

		double		OrderStopLoss(const int32_t& ticket)	 /* OrderSelect() */
		{return DispatchFunction<double,const int32_t>()(CmdId_OrderStopLoss,ticket);}

		int32_t		OrdersTotal()
		{return DispatchFunction<int32_t>()(CmdId_OrdersTotal);}

		double		OrderSwap(const int32_t& ticket)		 /* OrderSelect() */
		{return DispatchFunction<int32_t,const int32_t>()(CmdId_OrderSwap,ticket);}

		std::string OrderSymbol(const int32_t& ticket)		 /* OrderSelect() */
		{return DispatchFunction<std::string,const int32_t>()(CmdId_OrderSymbol,ticket);}

		double		OrderTakeProfit(const int32_t& ticket)  /* OrderSelect() */
		{return DispatchFunction<double,const int32_t>()(CmdId_OrderTakeProfit,ticket);}

		int32_t		OrderTicket(const int32_t& ticket)		 /* OrderSelect() */
		{return DispatchFunction<int32_t,const int32_t>()(CmdId_OrderTicket,ticket);}

		int32_t		OrderType(const int32_t& ticket)		 /* OrderSelect() */
		{return DispatchFunction<int32_t,const int32_t>()(CmdId_OrderType,ticket);}


		/* Checkup - http://docs.mql4.com/check/ */

		bool IsConnected()
		{return DispatchFunction<bool>()(CmdId_IsConnected);}

		bool IsDemo()
		{return DispatchFunction<bool>()(CmdId_IsDemo);}

		bool IsDllsAllowed()
		{return DispatchFunction<bool>()(CmdId_IsDllsAllowed);}

		bool IsExpertEnabled()
		{return DispatchFunction<bool>()(CmdId_IsExpertEnabled);}

		bool IsLibrariesAllowed()
		{return DispatchFunction<bool>()(CmdId_IsLibrariesAllowed);}

		bool IsOptimization()
		{return DispatchFunction<bool>()(CmdId_IsOptimization);}

		bool IsStopped()
		{return DispatchFunction<bool>()(CmdId_IsStopped);}

		bool IsTesting()
		{return DispatchFunction<bool>()(CmdId_IsTesting);}

		bool IsTradeAllowed()
		{return DispatchFunction<bool>()(CmdId_IsTradeAllowed);}

		bool IsTradeContextBusy()
		{return DispatchFunction<bool>()(CmdId_IsTradeContextBusy);}

		bool IsVisualMode()
		{return DispatchFunction<bool>()(CmdId_IsVisualMode);}

		int32_t  UninitializeReason()
		{return DispatchFunction<int32_t>()(CmdId_UninitializeReason);}



		/* Client Terminal http://docs.mql4.com/terminal */

		std::string TerminalCompany()
		{return DispatchFunction<std::string>()(CmdId_TerminalCompany);};

		std::string TerminalName()
		{return DispatchFunction<std::string>()(CmdId_TerminalName);};

		std::string TerminalPath()
		{return DispatchFunction<std::string>()(CmdId_TerminalPath);};

		/* Common Functions */

		bool Alert(const std::string& alert)
		{ return DispatchFunction<bool,const std::string>()(CmdId_Alert,alert);}

		bool Print(const std::string& print)
		{ return DispatchFunction<bool,const std::string>()(CmdId_Print,print);}

		bool Comment(const std::string& comment)
		{ return DispatchFunction<bool,const std::string>()(CmdId_Comment,comment);}

		int32_t GetTickCount()
		{return DispatchFunction<int32_t>()(CmdId_GetTickCount);}

		double MarketInfo(const std::string& symbol,const int& type)
		{return DispatchFunction<double,const std::string,const int>()(CmdId_MarketInfo,symbol,type);}

		bool PlaySound(const std::string& filename)
		{return DispatchFunction<bool,const std::string>()(CmdId_PlaySound,filename);}

		bool SendFTP(const std::string& filename,const std::string& ftp_path)
		{return DispatchFunction<bool,const std::string,const std::string>()(CmdId_SendFTP,filename,ftp_path);}

		bool SendMail(const std::string& subject,const std::string& text)
		{return DispatchFunction<bool,const std::string,const std::string>()(CmdId_SendMail,subject,text);}

		bool SendNotification(const std::string& message)
		{return DispatchFunction<bool,const std::string>()(CmdId_SendNotification,message);}

		PyMT4::Error GetLastError()
		{
			boost::mutex::scoped_lock errorMapLock(_lastErrorMapLock);
			LastErrorMap::const_iterator errorMapItem = _lastErrorMap.find(boost::this_thread::get_id());
			
			if (errorMapItem == _lastErrorMap.end())
				return ERR_NO_ERROR;
			
			return (PyMT4::Error)errorMapItem->second;
		}

	}
}