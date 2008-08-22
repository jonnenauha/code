/* state.cpp -- state module
 *
 *			Ryan McDougall -- 2008
 */

#include <main.h>
#include <state.hpp>
#include <boost/lexical_cast.hpp>

//=============================================================================
StartState::StartState (my_context ctx) : 
    my_base (ctx), // required because we call context() from a constructor
    machine (context <StateMachine>())
{ 
	VFVW_LOG("start entered");
}

StartState::~StartState () 
{ 
	VFVW_LOG("start exited");
}

result StartState::react (const ConnectionEvent& ev) 
{ 
	VFVW_LOG("start state react");

	if (ev.message->Type == ConnectorCreate1) {
		((ConnectorCreateResponse *)ev.result)->ConnectorHandle = VFVW_HANDLE;
		//((ConnectorCreateResponse *)ev.result)->VersionID = "";	// TODO
	}

    return transit <ConnectionState> ();
}

//=============================================================================
ConnectionState::ConnectionState (my_context ctx) : 
    my_base (ctx), // required because we call context() from a constructor
    machine (context <StateMachine>())
{ 
	VFVW_LOG("connector entered");

    try
    {
        glb_server-> InitConf();
    }
    catch (runtime_error& e)
    {
        cerr << e.what() << endl;
        post_event (StopEvent ());
    }
}

ConnectionState::~ConnectionState () 
{ 
	VFVW_LOG("connector exited");
}

result ConnectionState::react (const AccountEvent& ev) 
{
	VFVW_LOG("connector state react (AccountEvent)");

	ev.message-> SetState (machine.account);

	if (ev.message->Type == AccountLogin1) {
		((AccountLoginResponse *)ev.result)->AccountHandle = VFVW_HANDLE;
	}

    return transit <AccountState> ();
}

result ConnectionState::react (const StopEvent& ev) 
{ 
	VFVW_LOG("connector state react (StopEvent)");
    return transit <StopState> (); 
}

//=============================================================================
AccountState::AccountState (my_context ctx) : 
    my_base (ctx), // required because we call context() from a constructor
    machine (context <StateMachine>())
{ 
	VFVW_LOG("account entered");

	LoginStateChangeEvent loginStateEvent;
	loginStateEvent.AccountHandle = VFVW_HANDLE;
	loginStateEvent.StatusCode = "200";
	loginStateEvent.State = "1";

    glb_server-> Send (loginStateEvent.ToString());
}

AccountState::~AccountState () 
{ 
	VFVW_LOG("account exited");
}

result AccountState::react (const SessionEvent& ev) 
{ 
	VFVW_LOG("account state react (SessionEvent)");

    ev.message-> SetState (machine.session); // this should have done the parsing

	if (ev.message->Type == SessionCreate1) {
		((SessionCreateResponse *)ev.result)->SessionHandle = VFVW_HANDLE;
	}

	return transit <DialingState> ();
}

result AccountState::react (const StopEvent& ev) 
{ 
    return transit <StopState> (); 
}

//=============================================================================
DialingState::DialingState (my_context ctx) : 
    my_base (ctx), // required because we call context() from a constructor
    machine (context <StateMachine>())
{ 
	VFVW_LOG("dialing entered");

    // connect to conference
    try
    {
        glb_server-> JoinConf(machine.account, machine.session);
    }
    catch (runtime_error& e)
    {
        cerr << e.what() << endl;
        post_event (StopEvent ());
    }
}

DialingState::~DialingState () 
{ 
	VFVW_LOG("dialing exited");
}

result DialingState::react (const StopEvent& ev)
{
	VFVW_LOG("dialing state react (StopEvent)");
    return transit <StopState> ();
}

result DialingState::react (const DialSucceedEvent& ev) 
{ 
	VFVW_LOG("dialing state react (DialSucceedEvent)");
    return transit <SessionState> (); 
}

result DialingState::react (const DialFailedEvent& ev) 
{ 
	VFVW_LOG("dialing state react (DialFailedEvent)");
    return transit <StopState> (); 
}

//=============================================================================
SessionState::SessionState (my_context ctx) : 
    my_base (ctx), // required because we call context() from a constructor
    machine (context <StateMachine>())
{ 
	VFVW_LOG("session entered");

	SessionStateChangeEvent sessionStateEvent;
	sessionStateEvent.SessionHandle = VFVW_HANDLE;
	//sessionStateEvent.StatusCode = "";
	//sessionStateEvent.StatusString = "";
	sessionStateEvent.State = "4";
	//sessionStateEvent.URI = "";
	//sessionStateEvent.IsChannel = "";
	//sessionStateEvent.ChannelName = "";

    glb_server-> Send (sessionStateEvent.ToString());

	ParticipantStateChangeEvent partStateEvent;
	//sessionStateEvent.StatusCode = "";
	//sessionStateEvent.StatusString = "";
	partStateEvent.State = "7";
	//sessionStateEvent.ParticipantURI = "";
	//sessionStateEvent.AccountName = "";
	//sessionStateEvent.DisplayName = "";
	//sessionStateEvent.ParticipantType = "";

    glb_server-> Send (partStateEvent.ToString());

	ParticipantPropertiesEvent partPropEvent;
	sessionStateEvent.SessionHandle = VFVW_HANDLE;
	//partPropEvent.ParticipantURI = "";
	//partPropEvent.IsLocallyMuted = "";
	//partPropEvent.IsModeratorMuted = "";
	//partPropEvent.Volume = "";
	//partPropEvent.Energy = "";

    glb_server-> Send (partPropEvent.ToString());
	
}

SessionState::~SessionState () 
{ 
	SessionStateChangeEvent sessionStateEvent;
	sessionStateEvent.SessionHandle = VFVW_HANDLE;
	//sessionStateEvent.StatusCode = "";
	//sessionStateEvent.StatusString = "";
	sessionStateEvent.State = "5";
	//sessionStateEvent.URI = "";
	//sessionStateEvent.IsChannel = "";
	//sessionStateEvent.ChannelName = "";

    glb_server-> Send (sessionStateEvent.ToString());

	VFVW_LOG("session exited");
}

result SessionState::react (const PositionEvent& ev) 
{ 
	VFVW_LOG("session state react (PositionEvent)");
    //ev.messages.back()-> SetState (machine.);
    return discard_event (); 
}

result SessionState::react (const AudioEvent& ev) 
{ 
	VFVW_LOG("session state react (AudioEvent)");
    ev.message-> SetState (machine.audio);

	glb_server->AudioControl(machine.session, machine.audio);

    return discard_event (); 
}

result SessionState::react (const StopEvent& ev) 
{ 
	VFVW_LOG("session state react (StopEvent)");

    return transit <StopState> (); 
}

//=============================================================================
StopState::StopState (my_context ctx) : 
    my_base (ctx), // required because we call context() from a constructor
    machine (context <StateMachine>())
{ 
	VFVW_LOG("stop entered");

	// disconnect
    glb_server->LeaveConf();

	// Send LoginStateChangeEvent
	LoginStateChangeEvent loginStateEvent;
	loginStateEvent.AccountHandle = VFVW_HANDLE;
	loginStateEvent.StatusCode = "";
	loginStateEvent.State = "0";

    glb_server-> Send (loginStateEvent.ToString());
}

StopState::~StopState () 
{ 
	VFVW_LOG("stop exited");
}

result StopState::react (const SessionEvent& ev) 
{ 
	VFVW_LOG("stop state react (SessionEvent)");

    ev.message-> SetState (machine.session); // this should have done the parsing

	if (ev.message->Type == SessionCreate1) {
		((SessionCreateResponse *)ev.result)->SessionHandle = VFVW_HANDLE;
	}

	return transit <DialingState> ();
}

result StopState::react (const StopEvent& ev) 
{ 
	VFVW_LOG("stop state react (StopEvent)");
    return discard_event (); 
}
