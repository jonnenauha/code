The cryptographic concepts are described in the design proposal sent in June.
<email>
The purpose of the licensing server is to provide a technological
framework with which the Sales team can use to ensure that our
technology innovations are providing acceptable financial returns to
3Di.

We assume that most of the "3Di OpenSim" product is no different from
upstream OpenSim, except for the addition of some proprietary 3Di
OpenSim Plugins, which provide the value that our customers are paying
for. As such, to protect our revenue, they should not be easily copied
and used without authorization.

We assume that our customers, in general, will not find it worth their
time to defeat a technological measure in order to illegally use a
plugin without authorization. We must therefore only provide a mechanism
that protects against the most obvious forms of unauthorized use.

We recognize that since our customers will be running 3Di software on
their own machines, with which they have every deepest access to, it is
literally **impossible** to completely protect our software from
unauthorized use without resorting to draconian hardware-based DRM
technologies.

We recognize that DRM technologies are not often used because they are
technologically faulty, awkward to use, and wildly unpopular among
consumers. We will not attempt to make ourselves disliked by our own
customers by imposing excessive controls on our software. Instead we
must in the end rely on providing a superior customer service to our
clients, just as every other business must.

We propose that when a 3Di proprietary plugin is loaded by OpenSim, it
will send a digital signature to central server, which will verify the
signature, check to see if the file has been altered, and return a
message whether the signature is licensed or not. If the plugin is not
licensed, the plugin unloads itself, and OpenSim proceeds as normal.

** Specific functional requirements from Sales:

1. Record which company holds which licenses
2. Record how many licenses and plugins are in use
3. Return authorization when a company attempts to use a plugin
4. ???
5. Profit

** Licensing System:

When a client purchases a license from 3Di to use a Plugin (possibly via
a web form), we ask for a contact email address. Because email addresses
are unique on the Internet, we can use this information to uniquely
identify binaries licensed to that company.

Our registration procedure uses that email, and a random number (called
a Salt), to generate a SHA-2 "License ID". An asymmetric cipher (RSA)
then uses the same input as the License ID to generate a PKI key pair.
The Private Key, contact email address, and Salt are kept secret in the
3Di database. A certain Number of Licenses are associated with a License
ID.

Next, the registration procedure embeds the License ID and Public Key
into the Plugin binary, and runs a .NET obfuscator on the result. Then a
SHA-2 message digest is run on the Plugin to create a "Plugin ID", which
is loaded with file length into the database. The user can download the
Plugin and use it in their server.

When the Plugin gets loaded it runs a SHA-2 on itself to determine its
Plugin ID. It then sends an "Authorize" XMLRPC message to the Licensing
Server that includes its file size, the MAC address of the sending
machine, its Plugin ID, and the embedded License ID.

The location of the Licensing Server is a URL controlled by a plain-text
entry in OpenSim.ini.

The License Server records the IP address and timestamp of each message
sent by a Plugin, for auditing purposes. 

If the Plugin ID matches one in the License Server for that License ID,
and the file size is correct, and the license has not expired, and the
license is not already in use, then the Plugin is Authorized, and the
server indicates that a license for that License ID is in use, and
decrements the Number of Licenses.

The License Server uses its Private Key to encrypt the License ID, and
send to the Plugin as a Digital Signature within the Authorization
message. When the Plugin received the Authorization, it uses the
embedded Public Key to verify the identity of the License Server.

When the Plugin gets unloaded it then sends a "Release" XMLRPC message
to the Licensing Server that includes its Plugin ID, and the embedded
License ID. The Plugin then quits without confirmation.

If the Plugin ID matches one in the License Server for that License ID,
and the license is currently in use, then the Plugin is Released, and
the server indicates that a license for that License ID is now
available, and increments the Number of Licenses.

** Network Issues

Many networks will now allow special connections to the outside
Internet, even if our XMLPRC runs over HTTP. We should be considerate of
their security needs.

To that end, we may have the License Server placed within the firewall,
on the customer's machinery. The security implications of that
possibility have not been addressed.

Perhaps we can have log files encrypted, and the right to inspect those
logs to ensure the Licensing Server hasn't been tampered with?

** Security in the System:

-- Attackers wishing to change the Number of Licenses must first
compromise the system-level security of the Operating System that the
Licensing Server runs on.

-- Attackers wishing to trick the License Server must guess both the
Plugin ID, and the License ID.

The License ID is unique to each Licensee. Even if the Licensee's
corporate email address is known, the attacker must guess the Salt in
order to get access to License. This security relies on whether
collisions can be produced given partial input in SHA-2.

The Plugin ID is unique to each Plugin. If the attacker attempts to
modify the binary, they will produce a different hash, and the Licensing
Server will fail to recognize the doctored binary. This security relies
on two similar inputs resulting in two different hashes in SHA-2.

-- Attackers wishing to trick the Plugin must guess the Private Key on
the Licensing Server for the Plugin's License ID. The Plugin will check
with Digital Signature with the Public Key that was embedded into it,
and discontinue execution if the signature appears forged.

** Some "Attacks" that might be used to defeat this System:

-- Delete the Plugin
Assume: Nothin
Requirements: Access to the server
Result: Attacker loses the features the Plugin provides

-- Install Plugin on upstream OpenSim
Assume: Nothing
Requirements: A 3Di Plugin
Result: No change since all checking logic is within the Plugin itself

-- Copy a valid Plugin
Assume: Plugin authenticates with 3Di License Server
Requirements: Copy of valid 3Di Plugin
Result: License server will validate the Plugin if it is not already in
use, and fail otherwise. If a customer complains that his Plugin is in
use by a unauthorized duplicate, we can use the IP address reported to
track the duplicate binary.

-- Modify the Plugin binary
Assume: Plugin authenticates with 3Di License Server
Requirements: A modified 3Di Plugin
Results: A modified binary would cause the SHA-2 Plugin ID to be
different from what is stored in our server at registration time.

-- Create a Fake License Server
Assumption: 3Di Plugin is not modified
Requirements: A Fake License Server
Results: Digital Signature fails, and the Fake Licensing Server is
unable to authorize Plugin.

-- Create a Fake License Server and Modified Plugin binary
Assumption: Skilled attacker
Requirements: A modified 3Di Plugin and Fake License Server
Results: Attacker would be successful at avoiding the protection scheme,
only if he replaces the Public and Private Key pair in both components.

-- Modify the Plugin binary to avoid the entire authorization procedure
Assumption: Skilled attacker
Requirements: A 3Di Plugin
Results: Attacker would be successful at avoiding the protection scheme,
but only after a considerable amount of time studying the obfuscated
CIL.

** Attacks not Considered Yet
-- Replay or Timing Attacks


</email>

The make script builds a TCP server that creates a cryptographically signed DLL 
that will later "phone home" its unique ID to the server.

The server will check the IDs against what is stored in a database, and return 
a code on "PASS" or "FAIL".

It is understood that the scheme is hardly very secure, but is deemed the best 
compromise under the circumstances.

How to use:

1. Write 3DiSpecialPlugin.cs so that the main class inherits from 
SecurePlugin.SecurePlugin and calls the base constructor of 
SecurePlugin.SecurePlugin, and so that each important call into
3DiSpecialPlugin.cs checks the SecurePlugin.Verified property, such that it only
runs if it is verified.

2. Create a web registration form that gathers the user's email address 
(integrated with an ecommerce site is ideal). 

The web site calls the server factory and builds SecurePlugin.dll for that 
customer, and adds their ID into the database.

The web site then offers the dll for download to the customer. The web site
also offers the proprietary plugin 3DiSpecialPlugin.dll, which is linked 
against SecurePlugin.dll.

(TODO: embed SecurePlugin.dll into 3DiSpecialPlugin.dll so it is one download)

3. The customer places the dlls into their 3Di OpenSim plugin directory. When
3DiSpecialPlugin.dll is run, the SecurePlugin constructor opens a TCP
connection to the home server, sends its IDs and waits for a reply.

The server checks the IDs against its database, and replies with a pass code
if the pair can be found.

If the server validates the plugin, the Verified property is set to "true", and all
methods of 3DiSpecialPlugin will run when called. Otherwise Verified is set to
"false", and they will return without performing their function.

To run the prototype:

0. Set up a PostgreSQL database.

1. Create a public/private key pair using sn.exe

2. Run make.sh. This will build the server, but the test client is missing
SecurePlugin.dll.

3. Run the server as "mono server.exe youremail@example.com". This will build
SecurePlugin.dll, embed the unique IDs, record them in the DB, and begin
waiting for TCP connections.

4. Run make.sh again, and the test client will now build.

5. Run "mono test.exe" and verify it works as expected.

6. Delete all entries from the DB table, run the client again, and verify that
the functionality no longer works.
