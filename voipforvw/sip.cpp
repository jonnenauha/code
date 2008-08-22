/* sip.cpp -- sip module
 *
 *			Ryan McDougall -- 2008
 */

#include <main.h>
#include <sip.hpp>

//=============================================================================
/* Custom log function */
static void my_pj_log_ (int level, const char *data, int len)
{
    static ofstream log_ ("my_pj_log.txt");

    if (!log_) throw runtime_error ("unable to write to log file");
    if (level < pj_log_get_level()) log_ << data << endl;
}

//=============================================================================
/* Callback called by the library upon receiving incoming call */
static void on_incoming_call (pjsua_acc_id acc_id, pjsua_call_id call_id,
        pjsip_rx_data *rdata)
{
    pjsua_call_info ci;
    pjsua_call_get_info (call_id, &ci);

    cout << "Incoming call from " << ci.remote_info.ptr << endl;

    /* Automatically answer incoming calls with 200/OK */
    pjsua_call_answer (call_id, 200, NULL, NULL);
}

//=============================================================================
/* Callback called by the library when call's state has changed */
static void on_call_state (pjsua_call_id call_id, pjsip_event *e)
{
    pjsua_call_info ci;
    pjsua_call_get_info (call_id, &ci);

    StateMachine& sm (glb_server->GetStateMachine());
    
//    cout << "Call " << call_id << " state= " << ci.state_text.ptr << endl;
	VFVW_LOG("Call %d state=%s", call_id, ci.state_text.ptr);

	/*PJSIP_INV_STATE_NULL 	Before INVITE is sent or received
      PJSIP_INV_STATE_CALLING 	After INVITE is sent
      PJSIP_INV_STATE_INCOMING 	After INVITE is received.
      PJSIP_INV_STATE_EARLY 	After response with To tag.
      PJSIP_INV_STATE_CONNECTING 	After 2xx is sent/received.
      PJSIP_INV_STATE_CONFIRMED 	After ACK is sent/received.
      PJSIP_INV_STATE_DISCONNECTED 	Session is terminated.*/

	sm.session.call_id = call_id;

    switch (ci.state)
    {
        case PJSIP_INV_STATE_CONFIRMED:
            {
				VFVW_LOG("PJSIP_INV_STATE_CONFIRMED");
                DialSucceedEvent ev;
                sm.process_event (ev);
            }
            break;

        case PJSIP_INV_STATE_DISCONNECTED:
            {
				VFVW_LOG("PJSIP_INV_STATE_DISCONNECTED");
                DialFailedEvent ev;
                sm.process_event (ev);
            }
            break;

        default:
            break;
    }
}

//=============================================================================
/* Callback called by the library when call's media state has changed */
static void on_call_media_state (pjsua_call_id call_id)
{
    pjsua_call_info ci;
    pjsua_call_get_info (call_id, &ci);

    if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) 
    {
        pjsua_conf_connect (ci.conf_slot, 0);
        pjsua_conf_connect (0, ci.conf_slot);
    }
}

//=============================================================================
/* Display error and exit application */
static void error_exit (const char *title, pj_status_t status)
{
	VFVW_LOG("pjsip error [%s, %d]", title, status);

	pjsua_perror ("voice app", title, status);
    pjsua_destroy ();
    exit (1);
}

//=============================================================================
SIPConference::SIPConference () 
{ 
	VFVW_LOG("entering SIPConference()");
    start_sip_stack_(); 
}

#if 0
//=============================================================================
SIPConference::SIPConference (const SIPServerInfo& s) : 
    server_ (s)
{ 
	VFVW_LOG("entering SIPConference(const SIPServerInfo&)");
	start_sip_stack_(); 
}
#endif

//=============================================================================
SIPConference::~SIPConference () 
{ 
	VFVW_LOG("entering ~SIPConference()");
    stop_sip_stack_(); 
}

//=============================================================================
void SIPConference::Register (const SIPUserInfo& user)
{
	VFVW_LOG("entering Register(const SIPUserInfo&)");

	user_ = user;

    string temp_useruri (user_.get_uri());
    string temp_username (user_.name);
    string temp_userpasswd (user_.password);
    string temp_serverreguri (server_.get_reg_uri());
    string temp_serverdomain (server_.domain);

    pjsua_acc_config cfg;
    pjsua_acc_config_default (&cfg);

    cfg.id = pj_str (const_cast <char*> (temp_useruri.c_str()));

	// don't send REGISTER
//    cfg.reg_uri = pj_str (const_cast <char*> (temp_serverreguri.c_str()));
    cfg.reg_uri = pj_str("");

    cfg.cred_count = 1;
    cfg.cred_info[0].scheme = pj_str ("digest");
    cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
//    cfg.cred_info[0].realm = pj_str (const_cast <char*> (temp_serverdomain.c_str()));
    cfg.cred_info[0].realm = pj_str (VFVW_REALM);
    cfg.cred_info[0].username = pj_str (const_cast <char*> (temp_username.c_str()));
    cfg.cred_info[0].data = pj_str (const_cast <char*> (temp_userpasswd.c_str()));

    status = pjsua_acc_add (&cfg, PJ_TRUE, &acc_id);

    if (status != PJ_SUCCESS) 
        error_exit ("Error adding account", status);
}

//=============================================================================
void SIPConference::Join (const SIPServerInfo& serverInfo) 
{ 
	VFVW_LOG("entering Join()");

	server_ = serverInfo;

	string temp_serveruri (server_.get_conf_uri());

    pj_str_t uri = pj_str (const_cast <char*> (temp_serveruri.c_str()));

    status = pjsua_call_make_call (acc_id, &uri, 0, NULL, NULL, NULL);
    if (status != PJ_SUCCESS) 
        error_exit ("Error making call", status);
}

//=============================================================================
void SIPConference::Leave () 
{ 
	VFVW_LOG("entering Leave()");
    pjsua_call_hangup_all(); 
}

//=============================================================================
void SIPConference::AdjustTranVolume(pjsua_call_id call_id, float level)
{
	VFVW_LOG("entering AdjustTranVolume()");

	pjsua_call_info ci;
	pjsua_call_get_info (call_id, &ci);

	VFVW_LOG("AdjustTranVolume call_id=%d,level=%f", call_id, level);

#ifdef DEBUG
	unsigned tx_level = 0;
	unsigned rx_level = 0;
	status = pjsua_conf_get_signal_level(ci.conf_slot, &tx_level, &rx_level);
    if (status != PJ_SUCCESS) 
        error_exit ("Error get signal level", status);

	VFVW_LOG("current tx_level=%d,rx_level=%d", tx_level, rx_level);
#endif

	status = pjsua_conf_adjust_tx_level(ci.conf_slot, level);
    if (status != PJ_SUCCESS) 
        error_exit ("Error adjust tx level", status);
}

//=============================================================================
void SIPConference::AdjustRecvVolume(pjsua_call_id call_id, float level)
{
	VFVW_LOG("entering AdjustRecvVolume()");

	pjsua_call_info ci;
	pjsua_call_get_info (call_id, &ci);

	VFVW_LOG("AdjustRecvVolume call_id=%d,level=%f", call_id, level);

#ifdef DEBUG
	unsigned tx_level = 0;
	unsigned rx_level = 0;
	status = pjsua_conf_get_signal_level(ci.conf_slot, &tx_level, &rx_level);
    if (status != PJ_SUCCESS) 
        error_exit ("Error get signal level", status);

	VFVW_LOG("current tx_level=%d,rx_level=%d", tx_level, rx_level);
#endif

	status = pjsua_conf_adjust_rx_level(ci.conf_slot, level);
    if (status != PJ_SUCCESS) 
        error_exit ("Error adjust rx level", status);
}

//=============================================================================
void SIPConference::start_sip_stack_ ()
{
	VFVW_LOG("entering start_sip_stack_()");

	status = pjsua_create ();
    if (status != PJ_SUCCESS) 
        error_exit ("Error in pjsua_create()", status);
    
    pj_log_set_log_func (my_pj_log_);
    
    pjsua_config cfg;
    pjsua_config_default (&cfg);

    cfg.cb.on_incoming_call = &on_incoming_call;
    cfg.cb.on_call_media_state = &on_call_media_state;
    cfg.cb.on_call_state = &on_call_state;

    status = pjsua_init (&cfg, NULL, NULL);
    if (status != PJ_SUCCESS) 
        error_exit ("Error in pjsua_init()", status);

    pjsua_transport_config tcfg;
    pjsua_transport_config_default (&tcfg);

    tcfg.port = 6060;
    status = pjsua_transport_create (PJSIP_TRANSPORT_UDP, &tcfg, NULL);
    if (status != PJ_SUCCESS) 
        error_exit ("Error creating transport", status);

    status = pjsua_start ();
    if (status != PJ_SUCCESS) 
        error_exit ("Error starting pjsua", status);
} 

//=============================================================================
void SIPConference::stop_sip_stack_ () 
{ 
	VFVW_LOG("entering stop_sip_stack_()");
    pjsua_destroy (); 
}

//=============================================================================
static string get_line_ (istream& in)
{
    string line;
    
    ios_base::fmtflags ff (in.flags());
    in.setf (ios_base::skipws);
    getline (in, line); 
    in.flags (ff);
    
    return line;
}

static string take_after_ (string pre, string s)
{
    size_t p (s.find (pre));
    return (p != string::npos)? s.substr (p + pre.size()) : string();
}

static string take_before_ (string pre, string s)
{
    size_t p (s.find (pre));
    return (p != string::npos)? s.substr (0, p) : s;
}

static pair <string,string> parse_sip_uri_ (string uri)
{
    static const string sip_ ("sip:");
    static const string at_ ("@");

    string addr (take_after_ (sip_, uri));
    string target (take_before_ (at_, addr));
    string domain (take_after_ (at_, addr));

    return make_pair (target, domain);
}

//=============================================================================
istream& operator>> (istream& in, SIPUserInfo& usr)
{
    string line (get_line_ (in));
    pair <string,string> result (parse_sip_uri_ (line));

    usr.name = result.first;
    usr.domain = result.second;

    return in;
}

//=============================================================================
istream& operator>> (istream& in, SIPServerInfo& srv)
{
    string line (get_line_ (in));
    pair <string,string> result (parse_sip_uri_ (line));

    srv.conference = result.first;
    srv.domain = result.second;

    return in;
}

//=============================================================================
stringstream& operator>> (stringstream& in, SIPUserInfo& usr)
{
	string line = in.str();
    pair <string,string> result (parse_sip_uri_ (line));

    usr.name = result.first;
    usr.domain = result.second;

    return in;
}

//=============================================================================
stringstream& operator>> (stringstream& in, SIPServerInfo& srv)
{
	string line = in.str();
    pair <string,string> result (parse_sip_uri_ (line));

    srv.conference = result.first;
    srv.domain = result.second;

    return in;
}
