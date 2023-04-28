# Bienvenue sur ft_irc 
L’objectif de ce projet est de reproduire le fonctionnement d’un serveur IRC.
Vous utiliserez un vrai client IRC afin de vous connecter à votre serveur et ainsi de le
tester.
Internet fonctionne grâce à de nombreux standards et protocoles pour permettre une
interopérabilité entre les machines connectées. Il est toujours intéressant de connaître ce
genre de chose.

<h1>
</h1>

**Pour lancer WeeChat lancer l'executable suivant :**

```
./srcs/client_srcs/weechat-docker/build-and-run.sh
```


<h1>
</h1>

**Pour connecter le serveur a weechat :**

* dans weechat ecrire la commande comme suit :

```
/server add
```
* suivi du nom que l'on donne au serveur :

```
/server add manger
```

* et ensuite de l'adresse du serveur et de son port :

```
server add manger 127.0.0.1/9999
```
(au lieu de l'IP j'ai pu tester avec une adresse serveur comme chat.manger.net/6697 ce qui m'add un serveur "test" qui me permet d'avoir les logs etc)
* on y rajoute -ssl qui indique que la connexion au serveur se fera en utilisant le protocole SSL/TLS pour le chiffrement de la communication

* on utilise ensuite la commande :

```
/connect manger
```
* **suivi du nom du serveur**, 
WeeChat va alors essayer de se connecter à notre serveur et on pourra rejoindre des canaux sur ce serveur.
