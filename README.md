# SocketDate
Programme d'envoi et de réception de messages passant par des sockets

Programme compilé avec gcc version 14.2.0 sur une machine Debian avec C17.

Avant la compilation des 2 *.c vous pouvez modifier le port de connexion sur le daytime-client.c.
En modifiant la macro PORT, ensuite vous pouvez modifier l'adresse destination qui est par défaut 127.0.0.1.
/!\ Il faut garder une adresse IPv4.

Si vous avez modifié le port de destination sur le client, modifiez le aussi sur le daytime-server.c
En modifiant la macro PORT.

Vous pouvez tester le client ou le serveur à l'aide des commandes nc sur une machine Debian - exemple :
  - nc -l 9999 -> permet de tester le client
  - nc 127.0.0.1 9999 -> permet de tester le serveur

Après compilation, lancer les 2 programmes en parallèle et vous pouvez envoyer un message depuis le client au serveur.

/!\ Le message continuer ne fonctionne pas relancer le programme car le socket client est fermer après un msg.
/!\ Les messages heure et date peuvent être modifier
