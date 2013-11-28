/**
 * Task for switching a Module
 *
 * Copyright 2013 Thinstuff Technologies GmbH
 * Copyright 2013 DI (FH) Martin Haimberger <martin.haimberger@thinstuff.at>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TaskSwitchTo.h"
#include <winpr/wlog.h>
#include <appcontext/ApplicationContext.h>
#include "CallOutSwitchTo.h"


namespace freerds
{
	namespace sessionmanager
	{
		namespace call
		{
			static wLog* logger_CallBacks = WLog_Get("freerds.SessionManager.module.taskshutdown");

			void TaskSwitchTo::run() {

				CallOutSwitchTo switchToCall;
				switchToCall.setServiceEndpoint(mServiceEndpoint);
				switchToCall.setConnectionId(mConnectionId);

				switchToCall.encodeRequest();
				APP_CONTEXT.getRpcOutgoingQueue()->addElement(&switchToCall);
				WaitForSingleObject(switchToCall.getAnswerHandle(),INFINITE);
				switchToCall.decodeResponse();

				if (switchToCall.getResult() != 0) {
					WLog_Print(logger_CallBacks, WLOG_ERROR, "TaskSwitchTo answer: RPC error %d!",switchToCall.getResult());
					return;
				}
				// first unpack the answer
				if (switchToCall.decodeResponse()) {
					//
					WLog_Print(logger_CallBacks, WLOG_ERROR, "TaskSwitchTo: decoding of switchto answer failed!");
					return;
				}
				if (!switchToCall.isSuccess()) {
					WLog_Print(logger_CallBacks, WLOG_ERROR, "TaskSwitchTo: switching in FreeRDS failed!");
					return;
				}
				sessionNS::SessionPtr currentSession;

				if (mOldSessionId != 0) {
					currentSession = APP_CONTEXT.getSessionStore()->getSession(mOldSessionId);
					if (!currentSession) {
						currentSession->stopModule();
						APP_CONTEXT.getSessionStore()->removeSession(currentSession->getSessionID());
						WLog_Print(logger_CallBacks, WLOG_INFO, "TaskSwitchTo: session with sessionId %d was stopped!",mOldSessionId);
					} else {
						WLog_Print(logger_CallBacks, WLOG_ERROR, "TaskSwitchTo: no session was found for sessionId %d!",mOldSessionId);
					}
				} else {
					WLog_Print(logger_CallBacks, WLOG_ERROR, "TaskSwitchTo: no oldSessionId was set!");
				}

				APP_CONTEXT.getConnectionStore()->getOrCreateConnection(mConnectionId)->setSessionId(mNewSessionId);
				return;



			}

			void TaskSwitchTo::setConnectionId(long connectionId) {
				mConnectionId = connectionId;
			}

			void TaskSwitchTo::setServiceEndpoint(std::string serviceEndpoint) {
				mServiceEndpoint = serviceEndpoint;
			}

			void TaskSwitchTo::setOldSessionId(long sessionId) {
				mOldSessionId = sessionId;
			}

			void TaskSwitchTo::setNewSessionId(long sessionId) {
				mNewSessionId = sessionId;
			}

}
	}
}
