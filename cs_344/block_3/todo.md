: sleep 30 &
background pid is 4941
: kill -15 4941
background pid 4941 is done: terminated by signal 15

 73 :--------------------
 74 :pkill -signal SIGTERM sleep (10 points for pid of killed process, 10 points for signal)
 75 :(Ignore message about Operation Not Permitted)
 76 :pkill: 19199 - Operation not permitted
 77 pkill: 30735 - Operation not permitted
 78 terminated by signal 15


102 :mkdir testdir30750
103 ::
104 :
105 :--------------------
106 :cd testdir30750
107 ::
108 :
109 :--------------------
110 :pwd (5 points for being in the newly created dir)
111 :/nfs/stak/students/v/vollmerr



add : sigaction(SIGTERM, &ignore_action, NULL);
sigaction(SIGHUP, &ignore_action, NULL);
sigaction(SIGQUIT, &ignore_action, NULL);
