# Echange client-serveur

Voici un exemple (résumé et illustré) des échanges réalisés entre le(s) client(s) et le serveur

## Résumé :

1. Client 1 et Client 2 envoient JOIN_QUEUE

2. Serveur répond à chacun avec MATCH_START dès qu’une paire est formée

3. Le jeu commence :

    - Client 1 joue MOVE: (1,1)

    - Serveur transmet à Client 2 : OPPONENT_MOVE: (1,1)

    - Client 2 joue à son tour : MOVE: (0,2)

    - Serveur transmet à Client 1 : OPPONENT_MOVE: (0,2)

4. Serveur informe chacun quand c’est leur tour avec YOUR_TURN

5. À la fin : ``GAME_OVER: player2_wins envoyé aux deux joueurs (ou player1_wins, c'est juste un exemple)``

## Schéma :
```mermaid
sequenceDiagram 
	participant C1 as Client 1
    participant S as Serveur MatchaGame
    participant C2 as Client 2

    C1->>S: JOIN_QUEUE
    C2->>S: JOIN_QUEUE
    S->>C1: MATCH_START
    S->>C2: MATCH_START
    C1->>S: MOVE: (1,1)
    S->>C2: OPPONENT_MOVE: (1,1)
    C2->>S: MOVE: (0,2)
    S->>C1: OPPONENT_MOVE: (0,2)
    S->>C1: YOUR_TURN
    S->>C2: YOUR_TURN
    S->>C1: GAME_OVER: player2_wins
    S->>C2: GAME_OVER: player2_wins
```