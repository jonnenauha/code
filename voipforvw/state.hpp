/* state.hpp -- state definition
 *
 *			Ryan McDougall -- 2008
 */

#ifndef _STATE_HPP_
#define _STATE_HPP_

#include <boost/mpl/list.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/state.hpp>

using namespace boost::statechart;

//=============================================================================
// State machine

class Server;
struct StateMachine;
struct StartState;
struct ConnectionState;
struct AccountState;
struct SessionState;
struct DialingState;
struct StopState;

//struct Event { RequestQueue messages; };
struct Event 
{
	Request *message;
	ResponseBase *result;
};

struct StartEvent : public Event, event <StartEvent> {};
struct AccountEvent : public Event, event <AccountEvent> {};
struct ConnectionEvent : public Event, event <ConnectionEvent> {};
struct SessionEvent : public Event, event <SessionEvent> {};
struct PositionEvent : public Event, event <PositionEvent> {};
struct AudioEvent : public Event, event <AudioEvent> {};
struct DialFailedEvent : public Event, event <DialFailedEvent> {};
struct DialSucceedEvent : public Event, event <DialSucceedEvent> {};
struct StopEvent : public Event, event <StopEvent> {};

struct StateMachine : state_machine <StateMachine, StartState> 
{
    Audio audio; // the current audio state
    Account account; // the current account state
    Connection connection; // the current connector state
    Session session; // the current session state
    Orientation speaker; // the position of the speaking voice
    Orientation listener; // the position of the listener to the speaker
};

struct StartState : state <StartState, StateMachine> 
{
    typedef custom_reaction <ConnectionEvent> reactions;

    StartState (my_context ctx);
    ~StartState ();

    result react (const ConnectionEvent& ev);

    StateMachine& machine;
};

struct ConnectionState : state <ConnectionState, StateMachine> 
{
    typedef boost::mpl::list 
        <custom_reaction <StopEvent>, 
        custom_reaction <AccountEvent> > reactions;

    ConnectionState (my_context ctx);
    ~ConnectionState ();

    result react (const StopEvent& ev);
    result react (const AccountEvent& ev);

    StateMachine& machine;
};

struct AccountState : state <AccountState, StateMachine> 
{
    typedef boost::mpl::list 
        <custom_reaction <StopEvent>, 
        custom_reaction <SessionEvent> > reactions;

    AccountState (my_context ctx);
    ~AccountState ();

    result react (const StopEvent& ev);
    result react (const SessionEvent& ev);

    StateMachine& machine;
};

struct SessionState : state <SessionState, StateMachine> 
{
    typedef boost::mpl::list 
        <custom_reaction <StopEvent>, 
        custom_reaction <AudioEvent>, 
        custom_reaction <PositionEvent> > reactions;

    SessionState (my_context ctx);
    ~SessionState (); 

    result react (const StopEvent& ev);
    result react (const AudioEvent& ev);
    result react (const PositionEvent& ev);

    StateMachine& machine;
};

struct DialingState : state <DialingState, StateMachine> 
{
    typedef boost::mpl::list 
        <custom_reaction <StopEvent>, 
        custom_reaction <DialSucceedEvent>, 
        custom_reaction <DialFailedEvent> > reactions;

    DialingState (my_context ctx);
    ~DialingState (); 

    result react (const StopEvent& ev);
    result react (const DialSucceedEvent& ev);
    result react (const DialFailedEvent& ev);

    StateMachine& machine;
};


struct StopState : state <StopState, StateMachine> 
{
    typedef boost::mpl::list 
        <custom_reaction <SessionEvent>, 
        custom_reaction <StopEvent> > reactions;

	StopState (my_context ctx);
    ~StopState ();

//	result react (const ConnectionEvent& ev);
	result react (const SessionEvent& ev);
	result react (const StopEvent& ev);

    StateMachine& machine;
};

#endif //_STATE_HPP_
