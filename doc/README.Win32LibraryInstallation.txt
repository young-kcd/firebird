
One of the design goals of Firebird 1.5 is to prepare the way for multiple 
installs of the server. This will allow users to run different versions 
side by side. Firebird 1.5 does support this, although it is not well 
documented and very much requires intervention from a skilled user. Future
versions of Firebird will make this process far less complicated. In the 
meantime Firebird 1.5 needs to prepare the ground. This forces us to 
confront the issue of library installation. At the same time, Microsoft 
have taken their own steps to manage installation of different library 
versions. Taken together these two separate issues mean a new approach to 
library installation for Firebird 1.5 and beyond.

1/ Installation of Microsoft system libraries. 

The problems associated with installing different versions of Microsoft 
system libraries are so notorious that it has acquired the name
'DLL Hell'. 

From the release of Windows 2000 onwards Microsoft have made it almost 
impossible to upgrade system dll's. To resolve this Microsoft now 
recommend that each application installs local copies of any system 
libraries that are required. 

Firebird 1.5 follows this practice and places the required libraries
in the \bin directory along with the server. Microsoft have also 
established a convention for the operating system to load the local 
libraries by default. Empty files of the type:

  <mslibname>.local 

must be placed in the same directory. When the O/S receives the loadlibrary
call it uses the local library and not system library.


2/ Installation of fbclient.dll.

Firebird 1.5 and beyond no longer use gds32.dll as the client library. 
It is now called fbclient.dll. Given the problems that Microsoft have 
had with DLL hell it wouldn't make much sense if we continued to store the 
Firebird client library in the <system> directory. And as we want to allow 
installation of multiple engines simultaneously we would be creating our 
own DLL hell if we continued the practice of using the <system> directory 
for the client library. So, from Firebird 1.5 on, the client library resides 
in the \bin directory along with all the other binaries.

A new registry key has been added and all Firebird compliant applications
must now use this to locate the correct version of Firebird that they wish 
to use. The new key is:

  HKEY_LOCAL_MACHINE\SOFTWARE\Firebird Project\Firebird Server\Instances

Firebird will guarantee that one entry under this key always exists. It will 
be known as

  "DefaultInstance"
and will store the path to the root directory of (yes, you've guessed it) 
the default installation. Those that don't care about particular 
installations can alway use the default instance to locate the fbclient.dll.

Future versions of Firebird will see other entries under Instances. 
Applications will be able to enumerate the registry entries to determine
which library instance the wish to load. 


3/ Supporting legacy applications and drivers.

Traditionally, applications that use InterBase or Firebird have expected
to load the gds32.dll client library from the <system> directory. Firebird
1.5 ships with a stub gds32.dll that is Firebird compliant. It knows to 
look up the location of fbclient.dll via the registry and it routes all
application calls through to the fblcient.dll specified in the default 
instance.

This new gds32.dll really is a stub. If it is carelessly copied into the
<system> directory it will almost certainly break an existing install of 
InterBase or Firebird 1.0.  

During the installation process the installer checks to see if an 
installation of InterBase or Firebird exists. If nothing is installed 
it will write gds32.dll into the <system> directory. If it detects that 
any possible version of Firebird or InterBase may already be installed
it will not install the gds32.dll in the <system> directory. However
it will copy it into the \bin directory under the name of gds32.dll.stub.

It is intended that future versions of Firebird will not attempt to 
install gds32.dll into the <system> directory and ultimately it will
be completely removed from the distribution.


4/ Cleaning up release candidate installs.

It should be noted that the installer removes fbclient.dll from the 
<system> directory if the file is found there. The installer also 
removes the deprecated HKLM\Software\FirebirdSQL registry key.

