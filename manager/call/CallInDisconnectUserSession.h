/**
 * Class for rpc call DisconnectUserSession (freerds to session manager)
 *
 * Copyright 2013 Thincast Technologies GmbH
 * Copyright 2013 DI (FH) Martin Haimberger <martin.haimberger@thincast.com>
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

#ifndef CALL_IN_DISCONNECT_USER_SESSION_H_
#define CALL_IN_DISCONNECT_USER_SESSION_H_

#include <string>

#include "CallFactory.h"
#include "CallIn.h"

namespace freerds
{
	class CallInDisconnectUserSession: public CallIn
	{
	public:
		CallInDisconnectUserSession();
		virtual ~CallInDisconnectUserSession();

		virtual unsigned long getCallType();
		virtual int decodeRequest();
		virtual int encodeResponse();
		virtual int doStuff();

	private:
		long mConnectionId;
		bool mDisconnected;

		UINT32 m_RequestId;
		FDSAPI_DISCONNECT_USER_REQUEST m_Request;

		UINT32 m_ResponseId;
		FDSAPI_DISCONNECT_USER_RESPONSE m_Response;
	};

	FACTORY_REGISTER_DWORD(CallFactory, CallInDisconnectUserSession, FDSAPI_DISCONNECT_USER_REQUEST_ID);
}

#endif
