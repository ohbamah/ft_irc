une fois un channel cree : 
:clara!clara@localhost JOIN #c
:localhost 353 clara = #c :@clara 
:localhost 366 clara #c :End of /NAMES list

NICK 
:!@localhost NICK clara

juste Entree = 
:localhost 461 clara  :Not enough parameters

USER fonctionne meme si jai pas fait NICK ?

ne pas throw d'exception dans AddUser 

pb quand 1 des clients se deconnecte 