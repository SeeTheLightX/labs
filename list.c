// ============================================================================
// list.c -- Implementation for Lab 1.
// ============================================================================

#include "list.h"

#include <stdio.h>
#include <stdlib.h>

TokenList list_create(void) {
    return (TokenList){.head = nullptr, .tail = nullptr, .length = 0};
}

void list_destroy(TokenList *list) {
    if (list == nullptr) {
        return;
    }

    TokenNode *current = list->head;
    while (current != nullptr) {
        TokenNode *next = current->next;
        token_free(&current->token);
        free(current);
        current = next;
    }

    *list = list_create();
}

bool list_push_front(TokenList *list, Token token) {
    if (list == nullptr) {
        token_free(&token);
        return false;
    }

    TokenNode *new_node = malloc(sizeof(TokenNode));
    if (new_node == nullptr) {
        token_free(&token);
        return false;
    }

    new_node->token = token;
    new_node->next  = list->head;
    list->head      = new_node;

    if (list->tail == nullptr) {
        list->tail = new_node;
    }

    list->length++;
    return true;
}

bool list_push_back(TokenList *list, Token token) {
    if (list == nullptr) {
        token_free(&token);
        return false;
    }

    TokenNode *new_node = malloc(sizeof(TokenNode));
    if (new_node == nullptr) {
        token_free(&token);
        return false;
    }

    new_node->token = token;
    new_node->next  = nullptr;

    if (list->tail == nullptr) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail       = new_node;
    }

    list->length++;
    return true;
}

bool list_insert_after(TokenList *list, TokenNode *node, Token token) {
    if (list == nullptr || node == nullptr) {
        token_free(&token);
        return false;
    }

    TokenNode *new_node = malloc(sizeof(TokenNode));
    if (new_node == nullptr) {
        token_free(&token);
        return false;
    }

    new_node->token = token;
    new_node->next  = node->next;
    node->next      = new_node;

    if (list->tail == node) {
        list->tail = new_node;
    }

    list->length++;
    return true;
}

TokenNode *list_find_first(const TokenList *list, TokenKind kind) {
    if (list == nullptr) {
        return nullptr;
    }

    TokenNode *current = list->head;
    while (current != nullptr) {
        if (current->token.kind == kind) {
            return current;
        }
        current = current->next;
    }

    return nullptr;
}

bool list_remove_first(TokenList *list, TokenKind kind) {
    if (list == nullptr || list->head == nullptr) {
        return false;
    }

    TokenNode *prev = nullptr;
    TokenNode *curr = list->head;

    while (curr != nullptr && curr->token.kind != kind) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == nullptr) {
        return false;
    }

    if (prev == nullptr) {
        list->head = curr->next;
    } else {
        prev->next = curr->next;
    }

    if (list->tail == curr) {
        list->tail = prev;
    }

    token_free(&curr->token);
    free(curr);

    list->length--;
    return true;
}

size_t list_length(const TokenList *list) {
    return list != nullptr ? list->length : 0;
}

void list_print(const TokenList *list, FILE *out) {
    if (list == nullptr || out == nullptr) {
        return;
    }

    const TokenNode *cur = list->head;
    while (cur != nullptr) {
        fprintf(out, "  %-9s '%s' (%d:%d)\n", token_kind_name(cur->token.kind), cur->token.lexeme,
                cur->token.line, cur->token.col);
        cur = cur->next;
    }
}

void list_to_dot(const TokenList *list, FILE *out) {
    if (list == nullptr || out == nullptr) {
        return;
    }

    fprintf(out, "digraph tokens {\n");
    fprintf(out, "  rankdir=LR;\n");
    fprintf(out, "  node [shape=record];\n");

    if (list->head == nullptr) {
        fprintf(out, "  empty [label=\"empty\"];\n");
    } else {
        const TokenNode *cur = list->head;
        size_t idx           = 0;

        while (cur != nullptr) {
            fprintf(out, "  n%zu [label=\"{%s|%s}\"];\n", idx, token_kind_name(cur->token.kind),
                    cur->token.lexeme);
            if (cur->next != nullptr) {
                fprintf(out, "  n%zu -> n%zu;\n", idx, idx + (size_t)1);
            } else {
                fprintf(out, "  n%zu -> nnull;\n", idx);
            }
            cur = cur->next;
            idx++;
        }
        fprintf(out, "  nnull [label=\"nullptr\", shape=plaintext];\n");
    }

    fprintf(out, "}\n");
}

bool list_check_invariant(const TokenList *list) {
    if (list == nullptr) {
        return false;
    }

    size_t count          = 0;
    const TokenNode *cur  = list->head;
    const TokenNode *last = nullptr;

    while (cur != nullptr) {
        count++;
        last = cur;
        cur  = cur->next;
    }

    if (count != list->length) {
        return false;
    }

    if (list->head == nullptr) {
        return list->tail == nullptr;
    }

    return list->tail == last;
}