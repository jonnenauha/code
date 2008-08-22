
/* sip.hpp -- sip definition
 *
 *			Ryan McDougall -- 2008
 */

#ifndef _SIP_HPP
#define _SIP_HPP

#include <pjsua-lib/pjsua.h>

#define VFVW_REALM	"asterisk"

//=============================================================================
// Voice classes

struct SIPUserInfo
{
    SIPUserInfo () {}
    SIPUserInfo (const string& n, const string& d) 
        : name (n), domain (d) {}

    string name;
    string domain;
    string password;

    string get_uri () const { return "sip:" + name + "@" + domain; }
};

struct SIPServerInfo
{
    SIPServerInfo () {}
	SIPServerInfo (const string& c, const string& d) 
        : conference (c), domain (d) {}

    string conference;
    string domain;
    
    string get_conf_uri () const { return "sip:" + conference + "@" + domain; }
    string get_reg_uri () const { return "sip:" + domain; }
};

class SIPConference
{
    public:
        typedef list <SIPUserInfo> SIPUserList;

        SIPConference (); 
//        SIPConference (const SIPServerInfo&);
        ~SIPConference (); 

        void Register (const SIPUserInfo&); 
        void Join (const SIPServerInfo&); 
        void Leave (); 

		void AdjustTranVolume(pjsua_call_id, float);
		void AdjustRecvVolume(pjsua_call_id, float);

    private:
        void start_sip_stack_(); 
        void stop_sip_stack_(); 

    private:
        SIPServerInfo server_;
        SIPUserInfo user_;
    
    private:
        pjsua_acc_id acc_id;
        pj_status_t status;

    private:
        SIPConference (const SIPConference&);
        void operator= (const SIPConference&);
};

istream& operator>> (istream&, SIPUserInfo&);
istream& operator>> (istream&, SIPServerInfo&);

stringstream& operator>> (stringstream&, SIPUserInfo&);
stringstream& operator>> (stringstream&, SIPServerInfo&);

#endif //_SIP_HPP
