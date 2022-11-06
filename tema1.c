// Copyright 2022 Aldica Maria Miruna

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Card.h"
#include "Parsare_info.h"

extern char *strdup(const char *);

// Functie ce inverseaza prima si a doua jum a pachetului
Deck_corners shuffle_deck(Deck_corners corners, int idx)
{
    Deck start = cauta_pachet_d(corners, idx);
    Card iter = start->head;
    int cate_carti_are_pachetul = deck_len(idx, corners);
    // delimitam cele doua jumatati
    int mijloc = cate_carti_are_pachetul / 2;
    Card ultima_carte = start->tail;

    for (int i = 0; i < mijloc; i++)
    {
        start->head = iter->next;
        iter->next->prev = NULL;
        ultima_carte->next = iter;
        iter->prev = ultima_carte;
        iter->next = NULL;
        ultima_carte = iter;
        start->tail = iter;
        iter = start->head;
    }
    return corners;
}

// Refacem pachetul ce a suferit modificari
void remodelare(Deck_corners *corners, int idx1, int idx2)
{
    Deck p1 = cauta_pachet_d(*corners, idx1);
    Deck p2 = cauta_pachet_d(*corners, idx2);
    p2->prev->next = p2->next;
    if (p2->next != NULL)
    {
        p2->next->prev = p2->prev;
        free(p2);
    }

    if (p1 == (*corners)->head)
    {
        (*corners)->head = p1->next;
        p1->next->prev = NULL;
        free(p1);
    }
    else
    {
        p1->prev->next = p1->next;
        if (p1->next != NULL)
        {
            p1->next->prev = p1->prev;
            free(p1);
        }
    }
}

// Functie ce combina doua pachete luand cate o carte din fiecare
Deck_corners merge_decks(Deck_corners corners, int idx1, int idx2)
{   // noua lungime a noului pachet
    int lungime = deck_len(idx1, corners) + deck_len(idx2, corners);
    Deck pachet1 = cauta_pachet_d(corners, idx1);
    Deck pachet2 = cauta_pachet_d(corners, idx2);
    Deck d = create_deck(NULL, NULL, lungime, NULL, NULL);
    d->head = NULL;
    d->tail = NULL;
    Card c1 = pachet1->head;
    Card c2 = pachet2->head;

    Card next1 = c1;
    Card next2 = c2;

    while (next1 != NULL && next2 != NULL)
    {
        pachet1->head = next1->next;
        c1 = next1;
        if (c1->next != NULL)
        {
            c1->next->prev = NULL;
        }
        c1->next = NULL;
        d = add_last_card(d, c1);

        pachet2->head = next2->next;
        c2 = next2;
        if (c2->next != NULL)
        {
            c2->next->prev = NULL;
        }
        c2->next = NULL;
        d = add_last_card(d, c2);
        next1 = pachet1->head;
        next2 = pachet2->head;
    }

    if (next1 == NULL)
    {
        if (pachet2->head != NULL)
        {
            Card i = pachet2->head;
            while (i != NULL)
            {
                pachet2->head = i->next;
                if (i->next != NULL)
                {
                    i->next->prev = NULL;
                }
                i->next = NULL;
                d = add_last_card(d, i);
                i = pachet2->head;
            }
            pachet2->head = NULL;
            pachet2->tail = NULL;
        }
    }
    else
    {
        if (pachet1->head != NULL)
        {
            Card i = pachet1->head;
            while (i != NULL)
            {
                pachet1->head = i->next;
                if (i->next != NULL)
                {
                    i->next->prev = NULL;
                }
                i->next = NULL;
                d = add_last_card(d, i);
                i = pachet1->head;
            }
            pachet1->head = NULL;
            pachet1->tail = NULL;
        }
    }

    corners = add_deck(corners, d);
    if (idx1 > idx2)
    {
        remodelare(&corners, idx2, idx1);
    }
    else
    {
        remodelare(&corners, idx1, idx2);
    }
    return corners;
}

// Functie ce inverseaza ordinea cartilor din pachet
Deck_corners reverse_deck(Deck_corners corners, int idx)
{
    Deck start = cauta_pachet_d(corners, idx);
    Card head_initial = start->head;
    Card tail_initial = start->tail;

    Card temp = NULL;
    Card current = start->head;
    while (current != NULL)
    {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev;
    }

    if (temp != NULL)
    {
        start->head = temp->prev;
    }
    start->head = tail_initial;
    start->tail = head_initial;
    return corners;
}

// Functie ce reface pachetul dupa modificari
void remodelare2(Deck_corners *corners, int idx1, Deck *d)
{
    Deck p1 = cauta_pachet_d(*(corners), idx1);

    Deck next_lui_p1 = p1->next;
    if (next_lui_p1 != NULL)
    {
        next_lui_p1->prev = *d;
    }
    else
    {
        (*corners)->tail = *d;
    }
    (*d)->next = next_lui_p1;
    (*d)->prev = p1;
    p1->next = (*d);
}

// Functie care "taie" pachetul dupa indicele dat de idx_split
Deck_corners split(Deck_corners corners, int idx_pachet, int idx_split)
{
    Deck pachet_init = cauta_pachet_d(corners, idx_pachet);
    if (idx_split == 0)
    {
        return corners;
    }
    Card c = pachet_init->head;
    for (int i = 0; i < idx_split; i++)
    {
        c = c->next;
    }
    Card t = pachet_init->tail;  // tail.ul celui de al doilea pachet format
    Card anterior = c->prev;    // tail.ul primului pachet
    pachet_init->tail = anterior;
    c->prev = NULL;
    anterior->next = NULL;
    Card head_pachet2 = c;  // head.ul celui de al doilea pachet
    Deck d = create_deck(head_pachet2, t,
    (deck_len(idx_pachet, corners) - idx_split), NULL, NULL);
    remodelare2(&corners, idx_pachet, &d);
    return corners;
}

int main(void)
{
    char *input = malloc(MAX_LINE);
    Deck_corners corners = malloc(sizeof(struct deck_corners));
    corners->head = NULL;
    corners->tail = NULL;
    do
    {
        // citim de pe linie inputul pentru a vedea comenzile
        fgets(input, MAX_LINE, stdin);
        if (strstr(input, "ADD_DECK"))
        {   // in afara de ADD_DECK mai avem nevoie de un argument
            char *copy_input2 = strdup(input);
            if (verify_arg(copy_input2) != 2)
            {   // daca avem mai mult de 2 argumente
                printf("Invalid command. Please try again.\n");
                free(copy_input2);
            }
            else
            {
                int nr = aux_read1(input);
                // cazul in care se introduc numere negative
                if (nr < 0) {
printf("The provided index is out of bounds for the deck list.\n");
                } else {
                Deck p = create_deck(NULL, NULL, 0, NULL, NULL);
                int ok = 0;
                int i = 0;
                while (i < nr)
                {
                    fgets(input, MAX_LINE, stdin);
                    char *copy_input = strdup(input);

                    int nrr = aux_read2(input);
                    char *card_name = aux_read3(copy_input);

                    // conditiile pentru ca o carte sa fie  valida
                    if ((1 <= nrr) && (nrr < 15) &&
                        ((!(strcmp(card_name, "HEART\n")))
                         || (!(strcmp(card_name, "SPADE\n"))) ||
                         (!(strcmp(card_name, "CLUB\n")))
                         || (!(strcmp(card_name, "DIAMOND\n")))))
                    {
                        p = add_card_to_deck(nrr, card_name, p);
                        free(copy_input);
                        free(card_name);
                        i++;
                    }
                    else
                    {
                        printf("The provided card is not a valid one.\n");
                        free(copy_input);
                        free(card_name);
                    }
                }
                free(copy_input2);
                if (ok == 0)
                {
                    p->data_size = nr;
                    corners = add_deck(corners, p);
printf("The deck was successfully created with %d cards.\n", nr);
                }
            }
            }
        }
        else if (strstr(input, "DEL_DECK"))
        {
            char *copy_input2 = strdup(input);
            // verficam daca numarul de argumente e valid
            if (verify_arg(copy_input2) != 2)
            {
                printf("Invalid command. Please try again.\n");
                free(copy_input2);
            }
            else
            {
                int nr = aux_read1(input);
                // verificam daca numarul dat e valid pt pachte
                if (nr >= deck_number(corners) || nr < 0)
                {
printf("The provided index is out of bounds for the deck list.\n");
                }
                else
                {
                    corners = del_deck(nr, corners);
                    printf("The deck %d was successfully deleted.\n", nr);
                }
                free(copy_input2);
            }
        }
        else if (strstr(input, "DEL_CARD"))
        {
            char *copy_input2 = strdup(input);
            // verificam numarul de arg al comenzii
            if (verify_arg(copy_input2) != 3)
            {
                printf("Invalid command. Please try again.\n");
                free(copy_input2);
            }
            else
            {
                char *copy_input = strdup(input);
                int nr1 = aux_read0(input);
                if (nr1 >= deck_number(corners) || nr1 < 0)
                {
printf("The provided index is out of bounds for the deck list.\n");
                }
                else
                {
                    int nr2 = aux_read1(copy_input);
                    if (nr2 >= deck_len(nr1, corners) || nr2 < 0)
                    {
printf("The provided index is out of bounds for deck %d.\n", nr1);
                    }
                    else
                    {
                        corners = del_card(nr1, nr2, corners);
printf("The card was successfully deleted from deck %d.\n", nr1);
                    }
                }
                free(copy_input);
                free(copy_input2);
            }
        }
        else if (strstr(input, "ADD_CARDS"))
        {
            char *copy_input2 = strdup(input);
            if (verify_arg(copy_input2) != 3)
            {
                printf("Invalid command. Please try again.\n");
                free(copy_input2);
            }
            else
            {
                char *copy_input = strdup(input);
                int care_pachet = aux_read0(input);
                int cate_carti = aux_read1(copy_input);
                if (care_pachet >= deck_number(corners) || care_pachet < 0)
                {
printf("The provided index is out of bounds for the deck list.\n");
                }
                else
                {
                    Deck pachet = cauta_pachet_d(corners, care_pachet);
                    int ok = 0;
                    int i = 0;
                    while (i < cate_carti)
                    {
                        // avansam in poz. in care vrem sa introd cartea
                        fgets(input, MAX_LINE, stdin);
                        char *copy_input = strdup(input);
                        int nrr = aux_read2(input);
                        char *card_name = aux_read3(copy_input);
                        free(copy_input);
                        if ((1 <= nrr) && (nrr < 15) &&
                            ((!(strcmp(card_name, "HEART\n"))) ||
                             (!(strcmp(card_name, "SPADE\n"))) ||
                             (!(strcmp(card_name, "CLUB\n"))) ||
                              (!(strcmp(card_name, "DIAMOND\n")))))
                        {
                            pachet = add_card_to_deck(nrr, card_name, pachet);
                            i++;
                            free(card_name);
                        }
                        else
                        {
                            printf("The provided card is not a valid one.\n");
                            free(card_name);
                        }
                    }
                    if (ok == 0)
                    {
printf("The cards were successfully added to deck %d.\n", care_pachet);
                    }
                }
                // eliberam memoria
                free(copy_input);
                free(copy_input2);
            }
        }
        else if (strstr(input, "DECK_NUMBER"))
        {
            char *copy_input2 = strdup(input);
            if (verify_arg(copy_input2) != 1)
            {
                printf("Invalid command. Please try again.\n");
                free(copy_input2);
            }
            else
            {
                int nr = deck_number(corners);
                printf("The number of decks is %d.\n", nr);
                free(copy_input2);
            }
        }
        else if (strstr(input, "DECK_LEN"))
        {
            char *copy_input2 = strdup(input);
            if (verify_arg(copy_input2) != 2)
            {
                printf("Invalid command. Please try again.\n");
                free(copy_input2);
            }
            else
            {
                int idx = aux_read1(input);
                if (idx >= deck_number(corners) || idx < 0)
                {
printf("The provided index is out of bounds for the deck list.\n");
                }
                else
                {
                    int carti = deck_len(idx, corners);
                    printf("The length of deck %d is %d.\n", idx, carti);
                }
                free(copy_input2);
            }
        }
        else if (strstr(input, "SHUFFLE_DECK"))
        {
            char *copy_input2 = strdup(input);
            if (verify_arg(copy_input2) != 2)
            {
                printf("Invalid command. Please try again.\n");
                free(copy_input2);
            }
            else
            {
                int idx = aux_read1(input);
                if (idx >= deck_number(corners) || idx < 0)
                {
printf("The provided index is out of bounds for the deck list.\n");
                }
                else
                {
                    corners = shuffle_deck(corners, idx);
                    printf("The deck %d was successfully shuffled.\n", idx);
                }
                free(copy_input2);
            }
        }
        else if (strstr(input, "MERGE_DECKS"))
        {
            char *copy_input2 = strdup(input);
            if (verify_arg(copy_input2) != 3)
            {
                printf("Invalid command. Please try again.\n");
                free(copy_input2);
            }
            else
            {
                char *copy_input = strdup(input);
                int idx1 = aux_read0(input);
                if (idx1 >= deck_number(corners) || idx1 < 0)
                {
printf("The provided index is out of bounds for the deck list.\n");
                }
                else
                {
                    int idx2 = aux_read1(copy_input);
                    if (idx2 >= deck_number(corners) || idx2 < 0) {
printf("The provided index is out of bounds for the deck list.\n");
                    } else  {
                    corners = merge_decks(corners, idx1, idx2);
printf("The deck %d and the deck %d were successfully merged.\n", idx1, idx2);
                    }
                }
                // eliberam memoria
                free(copy_input);
                free(copy_input2);
            }
        }
        else if (strstr(input, "SPLIT_DECK"))
        {
            char *copy_input2 = strdup(input);
            if (verify_arg(copy_input2) != 3)
            {
                printf("Invalid command. Please try again.\n");
                free(copy_input2);
            }
            else
            {
                char *copy_input = strdup(input);
                int idx_pachet = aux_read0(input);
                if (idx_pachet >= deck_number(corners) || idx_pachet < 0)
                {
printf("The provided index is out of bounds for the deck list.\n");
                }
                else
                {
                    int idx_split = aux_read1(copy_input);
                    if (idx_split >= deck_len(idx_pachet, corners)
                    || idx_split < 0)
                    {
printf("The provided index is out of bounds for deck %d.\n", idx_pachet);
                    } else  {
                    corners = split(corners, idx_pachet, idx_split);
printf("The deck %d was successfully split by index %d.\n",
 idx_pachet, idx_split);
                    }
                }
                // eliberam memoria
                free(copy_input);
                free(copy_input2);
            }
        }
        else if (strstr(input, "REVERSE_DECK"))
        {
            char *copy_input2 = strdup(input);
            if (verify_arg(copy_input2) != 2)
            {
                printf("Invalid command. Please try again.\n");
                free(copy_input2);
            }
            else
            {
                int idx = aux_read1(input);
                if (idx >= deck_number(corners) || idx < 0)
                {
printf("The provided index is out of bounds for the deck list.\n");
                }
                else
                {
                    corners = reverse_deck(corners, idx);
                    printf("The deck %d was successfully reversed.\n", idx);
                }
                free(copy_input2);
            }
        }
        else if (strstr(input, "SHOW_DECK"))
        {
            char *copy_input2 = strdup(input);
            if (verify_arg(copy_input2) != 2)
            {
                printf("Invalid command. Please try again.\n");
                free(copy_input2);
            }
            else
            {
                int idx_pachet = aux_read1(input);
                if (idx_pachet >= deck_number(corners) || idx_pachet < 0)
                {
printf("The provided index is out of bounds for the deck list.\n");
                }
                else
                {
                    show_deck(idx_pachet, corners);
                }
                free(copy_input2);
            }
        }
        else if (strstr(input, "SHOW_ALL"))
        {
            char *copy_input2 = strdup(input);
            if (verify_arg(copy_input2) != 1)
            {
                printf("Invalid command. Please try again.\n");

                free(copy_input2);
            }
            else
            {
                show_all(corners);
                free(copy_input2);
            }
        }
        else if (strstr(input, "EXIT"))
        {
            break;
        }
        else
        {
            printf("Invalid command. Please try again.\n");
        }
    } while (!strstr(input, "EXIT"));
    // eliberam toata memoria
    free(input);
    free_ierarchy(corners->head);
    free(corners);
    return 0;
}
