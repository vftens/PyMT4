"""
 (C) Copyright 2013 Rob Watson rmawatson [at] hotmail.com  and others.

 All rights reserved. This program and the accompanying materials
 are made available under the terms of the GNU Lesser General Public License
 (LGPL) version 2.1 which accompanies this distribution, and is available at
 http://www.gnu.org/licenses/lgpl-2.1.html

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.

 Contributors:
     Rob Watson ( rmawatson [at] hotmail )
"""

from PyMT4 import *

from threading import Thread
import time


class OnOrderManager(object):

    __tickets  = []
    __handlers = []
    
    __runthread = None
    __shutdown = False
    @classmethod
    def Initialize(cls):
    
        tickets = [OrderSelect(index,SELECT_BY_POS,MODE_TRADES) for index in range(OrdersTotal())]
        
        for ticket in tickets:
            if OrderType(ticket) in (OP_BUY ,OP_SELL):
                cls.__tickets.append(ticket)
                
        
        cls.__runthread = Thread(target=OnOrderManager.__run)
        cls.__runthread.start()
          
    @classmethod        
    def RegisterOnOrderHandler(cls,callback):
        cls.__handlers.append(callback)
        
    
    @classmethod
    def __run(cls):
        
        count = 0
        while not cls.__shutdown:
            if count == 10:
                count = 0
                tickets = [OrderSelect(index,SELECT_BY_POS,MODE_TRADES) for index in range(OrdersTotal())]
                for ticket in tickets:
                    if OrderType(ticket) in (OP_BUY ,OP_SELL) and ticket not in cls.__tickets:
                        for handler in cls.__handlers:
                            #try:
                            handler(ticket)
                            #except:pass
                                
                            
                        cls.__tickets.append(ticket)
            count +=1
            
            time.sleep(0.2)
            
            
        
        
        
    @classmethod
    def Shutdown(cls):
        if cls.__runthread:
            cls.__shutdown = True
            cls.__runthread.join()
            

RegisterOnOrderHandler = OnOrderManager.RegisterOnOrderHandler



def OnOrderHandler(ticket):

    orderSize   = OrderLots(ticket)
    orderStop   = OrderStopLoss(ticket)
    openPrice   = OrderOpenPrice(ticket)
    orderType   = OrderType(ticket)
    orderSymbol = OrderSymbol(ticket)
    
    lotStepValue = MarketInfo(orderSymbol,MODE_POINT)

    if not orderStop:
        newStop = openPrice + ((-50*lotStepValue*10) if orderType == OP_BUY else (50*lotStepValue*10))   
        OrderModify(ticket,0.0,newStop,0.0,0,0)
    
def OnTickHandler(symbol,bid,ask):
    print symbol,bid,ask

if __name__ == "__main__":
    
    print Connect()
    OnOrderManager.Initialize()
    RegisterOnOrderHandler(OnOrderHandler)
    RegisterOnTickHandler("*",OnTickHandler)
    
    try:
        while(True):
            time.sleep(0.5)
    except KeyboardInterrupt:
        pass
    
    
    OnOrderManager.Shutdown()
    Disconnect()

