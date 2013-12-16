netdisk-fuse
============

Mounting netdisks provided by cloud providers onto local filesystem using fuse 

Two parts of the project:

BCS-API:  the module interact between local filsystem and BCS
	files:
	bcs-api.h   -- definition of class netdisk_bcs
	bcs-api.cc	-- declaration of class netdisk_bcs


NetDisk:  the module implement filesystem sys-call and then register them into fuse
	files:
	netdisk.h	-- definition of filesystem sys-calls
	netdisk.cc	-- declaration
