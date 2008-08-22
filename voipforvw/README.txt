1. Building

Requirements
- CMake
- Boost (Statechart -- header-based library)
 
Win32
    Requirements
    - Platform SDK and DirectX SDK

    1. Build the PJSIP stack
        a. copy config_site_sample.h to config_site.h (in pjlib\include\pj)
        b. open the solution file and set to Release
        c. build
        d. note the "PJTARGET" which is a string that describes the platform PJSIP is built for (for example "i386-win32-vc8-release")
        
    2. Build the application
        a. open CMakeLists.txt and ensure that the variable for PJTARGET is correct for your system (for example "i386-win32-vc8-release")
        b. open the cmake GUI and configure the project
        c. open the solution file and build

Linux
    Requirements
    - libssl-devel
    - libspeex-devel
    - libasound-devel
    - libuuid-devel

    1. Build the PJSIP stack 
        a. change into pjproject/
        b. ./configure && make
        d. note the "PJTARGET" which is a string that describes the platform PJSIP is built for (for example "i386-pc-linux-gnu")

    2. Build the application
        a. open CMakeLists.txt and ensure that the variable for PJTARGET is correct for your system (for example "i386-pc-linux-gnu")
        a. cmake .
        b. make

2. Running
    - copy SLVoice(.exe) to the SecondLife directory, replacing the existing binary file

3. Connecting to a SIP conference
    - in the same directory as the executable file, create a file called "sip.conf" that has a conference SIP URI on the first line, and a registered user SIP URI on the second line.

    For example: echo -e "sip:conference@example.com\nsip:user@example.com" | cat > sip.conf

4. Setting up to work with OpenSim
    - ensure that your OpenSim.ini contains the following section:
        [Voice]
        enabled = true
        account_management_server = https://account-server.example.com
        sip_domain = sip.example.com
