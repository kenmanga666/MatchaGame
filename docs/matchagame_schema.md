# Schémas

## UML -> BDD

````mermaid
erDiagram
    QUEUE {
        INTEGER id PK
        TEXT pseudo
        TEXT ip
        INTEGER port
        DATETIME timestamp
    }

    MATCHES {
        INTEGER id PK
        TEXT player1_ip
        INTEGER player1_port
        TEXT player2_ip
        INTEGER player2_port
        TEXT board
        BOOLEAN is_finished
        TEXT result
    }

    TURNS {
        INTEGER id PK
        INTEGER match_id FK
        INTEGER player
        TEXT move
        DATETIME timestamp
    }

    TURNS ||--|| MATCHES : "match_id → id"
````

## Schéma architecture
````
┌────────────┐                    ┌────────────┐
│ Client 1   │ ◀── joue avec ──▶ │ Client 2   │
└────────────┘                    └────────────┘
     ▲                                 ▲
     │                                 │
     ▼                                 ▼
┌────────────┐                      ┌────────────┐
│ MatchaGame │ ─── relay only ──▶  │ Game Logic │
└────────────┘                      └────────────┘
````