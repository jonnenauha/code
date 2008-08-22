/* voice.hpp -- state definition
 *
 *			Ryan McDougall -- 2008
 */

#ifndef _VOICE_HPP_
#define _VOICE_HPP_

#include <pjsua-lib/pjsua.h>

struct Account;
struct Connection;
struct Session;
struct Audio;
struct Orientation;

//=============================================================================
// structs for tracking parameter state
struct Account
{
    string name;
    string password;
    string uri;

    string handle;
};

struct Connection
{
    string handle;
};

struct Session
{
    string name;
    string password;
    string uri;

    string handle;

	pjsua_call_id call_id;
};

struct Audio
{
    float mic_volume;
    bool mic_mute;

    float speaker_volume;
    bool speaker_mute;

    Audio () :
        mic_volume (0.0f),
        mic_mute (false),
        speaker_volume (0.0f),
        speaker_mute (false)
    {}
};

struct Orientation
{
    float position [3];
    float velocity [3];

    float at [3];
    float up [3];
    float left [3];

    Orientation ();
    Orientation (const Orientation& copy);
    Orientation& operator= (const Orientation& rhs);

    void swap (Orientation& rhs);

    void set_position (const float v [3]) { copy (v, v+3, position); }
    void set_velocity (const float v [3]) { copy (v, v+3, velocity); }
    void set_at (const float v [3]) { copy (v, v+3, at); }
    void set_up (const float v [3]) { copy (v, v+3, up); }
    void set_left (const float v [3]) { copy (v, v+3, left); }
};

#endif //_VOICE_HPP_
