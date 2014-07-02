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


#pragma once

#include "pymt4_client.h"
#include "pymt4_client_dispatcher.h"




#define DECLFUNCTION(NAME,RETURN, ... ) \
namespace PyMT4 { \
	RETURN NAME( __VA_ARGS__ ) { \
		\
	} \
}


namespace PyMT4
{


	namespace Command
	{

		/* Account information http://docs.mql4.com/account */

		double		AccountBalance();
		double		AccountCredit();
		std::string AccountCompany();
		std::string AccountCurrency();
		double		AccountEquity();
		double		AccountFreeMargin();
		double		AccountFreeMarginCheck(const std::string& symbol,const int32_t& cmd,const double& volume);
		double		AccountFreeMarginMode();
		int32_t		AccountLeverage();
		double		AccountMargin();
		std::string AccountName();
		int32_t		AccountNumber();
		double		AccountProfit();
		std::string AccountServer();
		int32_t		AccountStopoutLevel();
		int32_t		AccountStopoutMode();

		/* Timeseries Access http://docs.mql4.com/series */

		int32_t		iBars(const std::string& symbol,const int32_t& timeframe);
		int32_t		iBarShift(const std::string& symbol,const Datetime& datetime,const bool& exact=false);
		double		iClose(const std::string& symbol,const int32_t& timeframe, const int32_t& shift);
		double		iHigh(const std::string& symbol, const int32_t& timeframe, const int32_t& shift);
		double		iHighest(const std::string& symbol, const int32_t& timeframe,const int32_t& type,const int32_t& count=0,const int32_t& start=0);
		double		iLow(const std::string& symbol, const int32_t& timeframe,const int32_t& shift);
		double		iLowest(const std::string& symbol, const int32_t& timeframe,const int32_t& type,const int32_t& count=0,const int32_t& start=0);
		double		iOpen(const std::string& symbol, const int32_t& timeframe,const int32_t& shift);
		Datetime    iTime(const std::string& symbol, const int32_t& timeframe, const int32_t& shift);
		double		iVolume(const std::string& symbol, const int32_t& timeframe, const int32_t& shift);

		/*  Trading functions http://docs.mql4.com/trading */

		bool		OrderClose(const int32_t& ticket, const double& lots,const double& price, const int32_t& slippage, const Color& Color=CLR_NONE);
		bool		OrderCloseBy(const int32_t& ticket, const double& opposite,const Color& Color=CLR_NONE);
		double		OrderClosePrice(const int32_t& ticket);		/* OrderSelect() */
		Datetime	OrderCloseTime(const int32_t& ticket);		/* OrderSelect() */
		std::string OrderComment(const int32_t& ticket);		/* OrderSelect() */
		double		OrderCommission(const int32_t& ticket);		/* OrderSelect() */
		bool		OrderDelete(const int32_t& ticket,const Color& Color=CLR_NONE); 
		Datetime	OrderExpiration(const int32_t& ticket);		/* OrderSelect() */
		double		OrderLots(const int32_t& ticket);			/* OrderSelect() */
		int32_t		OrderMagicNumber(const int32_t& ticket);	/* OrderSelect() */
		bool		OrderModify(const int32_t& ticket,const double& price, const double& stoploss, const double& takeprofit, const Datetime& expiration, const Color& arrow_color=CLR_NONE);
		double		OrderOpenPrice(const int32_t& ticket);		/* OrderSelect() */
		Datetime	OrderOpenTime(const int32_t& ticket);		/* OrderSelect() */
		bool		OrderPrint(const int32_t& ticket);			/* OrderSelect() */
		double		OrderProfit(const int32_t& ticket);			/* OrderSelect() */
		int32_t		OrderSelect(const int32_t& index, const int32_t& select, const int32_t& pool=MODE_TRADES);
		int32_t		OrderSend(
								const std::string& symbol, 
								const int32_t& cmd, const double& volume, 
								const double& price, 
								const int32_t& slippage, 
								const double& stoploss, 
								const double& takeprofit, 
								const std::string& comment="",
								const int32_t& magic=0,
								const Datetime& expiration=DATETIME_DEFAULT,
								const Color& arrow_color=CLR_NONE
							);

		int32_t		OrdersHistoryTotal();
		double		OrderStopLoss(const int32_t& ticket);	 /* OrderSelect() */
		int32_t		OrdersTotal();
		double		OrderSwap(const int32_t& ticket);		 /* OrderSelect() */
		std::string OrderSymbol(const int32_t& ticket);		 /* OrderSelect() */
		double		OrderTakeProfit(const int32_t& ticket);  /* OrderSelect() */
		int32_t		OrderTicket(const int32_t& ticket);		 /* OrderSelect() */
		int32_t		OrderType(const int32_t& ticket);		 /* OrderSelect() */

		/* Checkup - http://docs.mql4.com/check/ */

		//int32_t GetLastError();
		bool IsConnected();
		bool IsDemo();
		bool IsDllsAllowed();
		bool IsExpertEnabled();
		bool IsLibrariesAllowed();
		bool IsOptimization();
		bool IsStopped();
		bool IsTesting();
		bool IsTradeAllowed();
		bool IsTradeContextBusy();
		bool IsVisualMode();
		int  UninitializeReason();


		/* Client Terminal http://docs.mql4.com/terminal */

		std::string TerminalCompany();
		std::string TerminalName();
		std::string TerminalPath();

		/* Common Functions  http://docs.mql4.com/common */

		bool Alert(const std::string&);
		bool Comment(const std::string&);
		bool Print(const std::string&);



		int32_t GetTickCount();
		double MarketInfo(const std::string& symbol,const int& type);
		bool PlaySound(const std::string& filename);
		bool SendFTP(const std::string& filename,const std::string& ftp_path);
		bool SendMail(const std::string& subject,const std::string& text);
		bool SendNotification(const std::string& message); 

		PyMT4::Error GetLastError();

	}
}