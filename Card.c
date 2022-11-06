// Copyright 2022 Aldica Maria Miruna
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Card.h"

extern char *strdup(const char *);

// Functie pentru creare de noi pachete
Deck create_deck(Card head, Card tail, int data_size, Deck next, Deck prev)
{
    Deck d = malloc(sizeof(struct deck));
    d->head = head;
    d->tail = tail;
    d->data_size = data_size;
    d->next = next;
    d->prev = prev;
    return d;
}

// Functie pentru creare de noi carti
Card create_card(void *info, Card next, Card prev)
{
    Card c = malloc(sizeof(struct card));
    c->data = info;
    c->next = next;
    c->prev = prev;
    return c;
}

// Functie pentru adaugarea ultimei carti in lista
Deck add_last_card(Deck d, Card c)
{
    // daca pachetul e gol, adaugam o carte
    if (d->head == NULL)
    {
        d->head = c;
        d->tail = c;
        return d;
    }

    c->prev = d->tail;
    d->tail->next = c;
    c->next = NULL;
    d->tail = c;
    return d;
}

// Functie pentru adaugarea unei carti intr-un pachet specificat
Deck add_card_to_deck(int nr, char *card_name, Deck d)
{
    // informatie utila retine datele cartii adaugate
    Data informatie_utila = malloc(sizeof(struct data));
    informatie_utila->valoare = nr;
    informatie_utila->name = strdup(card_name);
    // am creat o carte noua pe care inca nu am atasat-o unui pachet
    Card c = create_card(informatie_utila, NULL, NULL);
    if (d->head == NULL)
    {
        d->head = c;
        d->tail = c;
    }
    else
    {
        d = add_last_card(d, c);
    }
    return d;
}

// Functie pentru adaugarea unui nou pachet
Deck_corners add_deck(Deck_corners corners, Deck c)
{
    if (corners->head == NULL)
    {
        corners->head = c;
        corners->tail = c;
        return corners;
    }

    c->prev = corners->tail;
    corners->tail->next = c;
    c->next = NULL;
    corners->tail = c;
    return corners;
}

// Functie pentru afisarea pachetelor cu cartile corespunzatoare
void show_deck(int idx, Deck_corners corners)
{
    printf("Deck %d:\n", idx);
    Deck iter = corners->head;
    // Cazul in care pachetul cerut e primul
    if (idx == 0)
    {
        Card i = iter->head;
        while (i != NULL)
        {
            printf("\t%d %s", ((Data)i->data)->valoare, ((Data)i->data)->name);
            i = i->next;
        }
        return;
    }

    while (idx > 0)
    {
        iter = iter->next;
        idx--;
    }
    Card j = iter->head;
    while (j != NULL)
    {
        printf("\t%d %s", ((Data)j->data)->valoare, ((Data)j->data)->name);
        j = j->next;
    }
}

// Functie de eliberare a memoriei pentru cartile de joc
void free_card(Card c)
{
    free(((Data)c->data)->name);
    free(((Data)c->data));
    free(c);
}

// Functie de eliberare a memoriei pentru pachetele de carti
void free_deck(Card c)
{
    if (c == NULL)
    {
        return;
    }
    Card x = c->next;
    free_card(c);
    free_deck(x);
}

// Functie de stergere a pachetului impreuna cu cartile din el
Deck_corners del_deck(int idx, Deck_corners corners)
{
    if (idx == 0)
    {
        Deck for_remove = corners->head;
        if (corners->head != NULL)
        {
            corners->head = corners->head->next;
            if (corners->head != NULL)
            {
                corners->head->prev = NULL;
            }
        }
        free_deck(for_remove->head);
        free(for_remove);
        return corners;
    }
    if (idx == deck_number(corners) - 1)
    {
        Deck for_remove = corners->tail;
        corners->tail = corners->tail->prev;
        corners->tail->next = NULL;
        free_deck(for_remove->head);
        free(for_remove);
        return corners;
    }
    Deck iter = corners->head;
    while (idx > 0)
    {
        iter = iter->next;
        idx--;
    }
    iter->prev->next = iter->next;
    if (iter->next != NULL)
    {
        iter->next->prev = iter->prev;
    }
    free_deck(iter->head);
    free(iter);
    return corners;
}

// Functie de eliberare a pachetului home
void free_ierarchy(Deck home)
{
    if (home == NULL)
        return;
    Deck d = home;
    home = home->next;
    free_deck(d->head);
    free(d);
    free_ierarchy(home);
}

// Functie de afisare a valorilor si numelui unui pachet luat la intamplare
static void show_deck_aux(Deck d) {
    Card i = d->head;
    while (i != NULL)
    {
        printf("\t%d %s", ((Data)i->data)->valoare, ((Data)i->data)->name);
        i = i->next;
    }
    return;
}

// Functie pentru afisarea intregii ierarhii de pachete si carti
void show_all(Deck_corners corners) {
    Deck i = corners->head;
    int idx = 0;
    while (i != NULL)
    {
        printf("Deck %d:\n", idx);
        show_deck_aux(i);
        i = i->next;
        idx++;
    }
}

// Functie ce returneaza numarul de pachete
int deck_number(Deck_corners corners)
{
    int i = 0;
    if (corners->head == NULL)
    {
        return 0;
    }
    else
    {
        i++;
        Deck d = corners->head;
        while (d->next != NULL)
        {
            d = d->next;
            i++;
        }
        return i;
    }
}

// Functie ce returneaza lungimea pachetului
int deck_len(int idx, Deck_corners corners)
{
    Deck pack = corners->head;
    for (int i = 0; i < idx; i++)
    {
        pack = pack->next;
    }
    int nr = 0;
    Card j = pack->head;
    while (j != NULL)
    {
        nr++;
        j = j->next;
    }
    return nr;
}
// Functie pentru stergerea unui anumit pachet
Deck_corners del_card(int idx_deck, int idx_card, Deck_corners corners)
{
    Deck current_deck = corners->head;
    // avansam pana la pachetul pe care il stergem
    for (int i = 0; i < idx_deck; i++)
    {
        current_deck = current_deck->next;
    }
    if (idx_card == 0 && deck_len(idx_deck, corners) == 1)
    {
        corners = del_deck(idx_deck, corners);
        return corners;
    }
    if (idx_card == 0)
    {
        Card for_remove = current_deck->head;
        current_deck->head = current_deck->head->next;
        if (current_deck->head != NULL)
        {
            current_deck->head->prev = NULL;
        }
        free_card(for_remove);
        return corners;
    }
    if (idx_card == (deck_len(idx_deck, corners) - 1))
    {
        Card for_remove = current_deck->tail;
        current_deck->tail = current_deck->tail->prev;
        current_deck->tail->next = NULL;
        free_card(for_remove);
        return corners;
    }

    Card iter = current_deck->head;
    for (int i = 0; i < idx_card; i++)
    {
        iter = iter->next;
    }

    iter->prev->next = iter->next;
    if (iter->next != NULL)
        iter->next->prev = iter->prev;
    free_card(iter);
    return corners;
}

// Functie care avanseaza pana ala pachetul in care facem modificari
Deck cauta_pachet_d(Deck_corners corners, int care_pachet)
{
    Deck current_deck = corners->head;
    for (int i = 0; i < care_pachet; i++)
    {
        current_deck = current_deck->next;
    }
    return current_deck;
}

