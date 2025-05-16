# 🎯 MatchaGame - Serveur de Matchmaking Modulaire

## Présentation

**MatchaGame** (jeu de mots sur *Match Your Game*) est un serveur de matchmaking modulaire développé en C++. Il permet de connecter automatiquement deux joueurs à un jeu de plateau au tour par tour, de manière générique. Le système est conçu pour être **indépendant de la logique de jeu**, facilitant ainsi l’intégration de jeux externes.

Un exemple d'intégration est fourni avec le jeu **TicTacNet**, un morpion multijoueur avec interface graphique (GUI).

---

## 🛠️ Technologies utilisées

- **Langage** : C++
- **Sockets** : BSD Sockets (ou Boost.Asio selon version finale)
- **Base de données** : SQLite (via `sqlite3`)
- **Interface graphique (GUI)** : SFML / Qt / Dear ImGui (à confirmer)
- **Build system** : CMake
- **IDE recommandé** : VSCode ou CLion

---

## 🧱 Architecture

Le projet est composé de deux modules indépendants :

### 🧩 Serveur `MatchaGame`
- Gère la file d’attente des joueurs
- Crée automatiquement des matchs
- Coordonne les échanges entre les clients
- Stocke les informations en base de données (file d’attente, matchs, tours)
- Permet de connecter tout jeu compatible via un protocole standardisé

### 🎮 Client `TicTacNet`
- Implémente la logique d’un jeu de morpion
- Dialogue avec `MatchaGame` via TCP
- **Interface graphique propre et minimaliste** pour l’utilisateur

---

## 📐 Fonctionnalités

### Serveur MatchaGame :
- File d’attente des joueurs
- Lancement automatique de matchs
- Communication réseau via sockets TCP
- Stockage des données (matchs / tours / file) dans une base SQLite
- Plugin de logique de jeu (morpion dans un premier temps)

### Jeu TicTacNet :
- Connexion au serveur MatchaGame
- **Affichage graphique du plateau de jeu**
- Interaction via clics souris / interface GUI
- Réception des coups adverses
- Fin de partie avec affichage du résultat

---

## 📊 Base de données

Le serveur utilise une base SQLite avec 3 tables principales :

- `queue` : liste d’attente des joueurs
- `matches` : données des parties en cours / terminées
- `turns` : historique des coups joués

📎 [Voir le schéma UML de la base de données](matchagame_schema.png)

---

## 📁 Arborescence prévue

matchagame/  
├── client/ # Client TicTacNet avec GUI  
│ ├── main.cpp  
│ └── ...   
├── server/ # Serveur MatchaGame    
│ ├── main.cpp  
│ └── ...   
├── database/ # Gestion base de données  
│ └── db.cpp    
├── docs/ # Documentation, schémas UML  
│ └── matchagame_schema.png  
├── CMakeLists.txt  
└── README.md

---

## 🚧 Avancement

- [x] Choix du nom
- [x] Rédaction du README
- [x] Schéma de base de données
- [ ] Setup de l’environnement CMake
- [ ] Implémentation du serveur matchmaking
- [ ] Implémentation du client TicTacNet (avec GUI)
- [ ] Tests réseau et sockets
- [ ] Documentation finale & livrables

---

## 🧠 Auteur

- [Kenny Vallée](https://github.com/kenmanga666)