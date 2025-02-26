msg affiche au client apres quil se soit connecte au server PASS + NICK + USER : 
:localhost  001 clara :Welcome to the ft_irc clara!clara@localhost
:localhost  002 clara :Your host is ft_irc 1.0
:localhost  003 clara :Created at [26.2.2025]

une fois un channel cree : 
:clara!clara@localhost JOIN #c
:localhost 353 clara = #c :@clara 
:localhost 366 clara #c :End of /NAMES list

NICK 
:!@localhost NICK clara


juste Entree = 
:localhost 461 clara  :Not enough parameters

USER fonctionne meme si jai pas fait NICK ?