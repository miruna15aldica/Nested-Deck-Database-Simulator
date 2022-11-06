// Copyright 2022 Aldica Maria Miruna
#ifndef CARD_H_
#define CARD_H_

// structura datei unei carti
typedef struct data
{
    int valoare;
    char *name;
} * Data;

// cartile au tip de date generice (void *)
typedef struct card
{
    void *data;
    struct card *next, *prev;
} * Card;

// structura pentru pachete
typedef struct deck
{
    Card head;
    Card tail;
    unsigned int data_size;
    struct deck *next, *prev;
} * Deck;

// capetele pachetului
typedef struct deck_corners
{
    Deck head;
    Deck tail;
} * Deck_corners;

Deck create_deck(Card head, Card tail, int data_size, Deck next, Deck prev);
Card create_card(void *info, Card next, Card prev);
Deck add_last_card(Deck d, Card c);
Deck add_card_to_deck(int nr, char *card_name, Deck d);
Deck_corners add_deck(Deck_corners corners, Deck c);
void show_deck(int idx, Deck_corners corners);
void free_card(Card c);
void free_deck(Card c);
Deck_corners del_deck(int idx, Deck_corners corners);
void free_ierarchy(Deck home);
void show_all(Deck_corners corners);
int deck_number(Deck_corners corners);
int deck_len(int idx, Deck_corners corners);
Deck_corners del_card(int idx_deck, int idx_card, Deck_corners corners);
Deck cauta_pachet_d(Deck_corners corners, int care_pachet);

#endif  // CARD_H_
