#include "expression.h"
#include "stack.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ExprResult result(ExprStatus status, long value, size_t position, const char *message) {
    ExprResult r = {.status = status, .value = value, .position = position, .message = ""};
    if (message != NULL) {
        (void)snprintf(r.message, sizeof r.message, "%s", message);
    }
    return r;
}

static bool is_opener(char c) {
    return c == '(' || c == '[' || c == '{';
}
static bool is_closer(char c) {
    return c == ')' || c == ']' || c == '}';
}

static char expected_opener(char closer) {
    switch (closer) {
    case ')':
        return '(';
    case ']':
        return '[';
    case '}':
        return '{';
    default:
        return '\0';
    }
}

static bool is_operator_token(const char *token) {
    return token[0] != '\0' && token[1] == '\0' && strchr("+-*/%", token[0]) != NULL;
}

static bool parse_long_token(const char *token, long *out) {
    if (token == NULL || *token == '\0')
        return false;
    char *end = NULL;
    long value = strtol(token, &end, 10);
    if (*end != '\0')
        return false;
    *out = value;
    return true;
}

static bool apply_operator(char op, long left, long right, long *out) {
    switch (op) {
    case '+':
        *out = left + right;
        return true;
    case '-':
        *out = left - right;
        return true;
    case '*':
        *out = left * right;
        return true;
    case '/':
        if (right == 0)
            return false;
        *out = left / right;
        return true;
    case '%':
        if (right == 0)
            return false;
        *out = left % right;
        return true;
    default:
        return false;
    }
}

const char *expr_status_name(ExprStatus status) {
    switch (status) {
    case EXPR_OK:
        return "OK";
    case EXPR_UNMATCHED_CLOSER:
        return "UNMATCHED_CLOSER";
    case EXPR_UNCLOSED_OPENER:
        return "UNCLOSED_OPENER";
    case EXPR_MISMATCHED_DELIMITER:
        return "MISMATCHED_DELIMITER";
    case EXPR_INVALID_TOKEN:
        return "INVALID_TOKEN";
    case EXPR_TOO_FEW_OPERANDS:
        return "TOO_FEW_OPERANDS";
    case EXPR_TOO_MANY_OPERANDS:
        return "TOO_MANY_OPERANDS";
    case EXPR_DIVIDE_BY_ZERO:
        return "DIVIDE_BY_ZERO";
    case EXPR_OUT_OF_MEMORY:
        return "OUT_OF_MEMORY";
    }
    return "UNKNOWN";
}

ExprResult check_delimiters(const char *text) {
    Stack stack = stack_create();

    for (size_t i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        if (is_opener(c)) {
            if (!stack_push(&stack, (long)c)) {
                stack_destroy(&stack);
                return result(EXPR_OUT_OF_MEMORY, 0, i, "out of memory while pushing delimiter");
            }
        } else if (is_closer(c)) {
            long popped = 0;
            if (!stack_pop(&stack, &popped)) {
                stack_destroy(&stack);
                return result(EXPR_UNMATCHED_CLOSER, 0, i, "unmatched closing delimiter");
            }
            if ((char)popped != expected_opener(c)) {
                stack_destroy(&stack);
                return result(EXPR_MISMATCHED_DELIMITER, 0, i, "mismatched delimiter pair");
            }
        }
    }

    if (!stack_is_empty(&stack)) {
        stack_destroy(&stack);
        return result(EXPR_UNCLOSED_OPENER, 0, strlen(text), "unclosed opening delimiter");
    }

    stack_destroy(&stack);
    return result(EXPR_OK, 0, 0, "balanced");
}

ExprResult eval_postfix(const char *expression, bool trace) {
    Stack operands = stack_create();

    char *copy = malloc(strlen(expression) + 1);
    if (copy == NULL)
        return result(EXPR_OUT_OF_MEMORY, 0, 0, "could not copy expression");
    strcpy(copy, expression);

    size_t token_number = 0;
    for (char *token = strtok(copy, " \t\r\n"); token != NULL; token = strtok(NULL, " \t\r\n")) {
        token_number++;
        long number = 0;

        if (parse_long_token(token, &number)) {
            if (!stack_push(&operands, number)) {
                free(copy);
                stack_destroy(&operands);
                return result(EXPR_OUT_OF_MEMORY, 0, token_number, "out of memory");
            }
        } else if (is_operator_token(token)) {
            long right = 0;
            long left = 0;
            long computed = 0;

            if (!stack_pop(&operands, &right) || !stack_pop(&operands, &left)) {
                free(copy);
                stack_destroy(&operands);
                return result(EXPR_TOO_FEW_OPERANDS, 0, token_number,
                              "too few operands for operator");
            }

            if (!apply_operator(token[0], left, right, &computed)) {
                free(copy);
                stack_destroy(&operands);
                return result(EXPR_DIVIDE_BY_ZERO, 0, token_number, "division or modulo by zero");
            }

            if (!stack_push(&operands, computed)) {
                free(copy);
                stack_destroy(&operands);
                return result(EXPR_OUT_OF_MEMORY, 0, token_number, "out of memory");
            }
        } else {
            char msg[160];
            (void)snprintf(msg, sizeof msg, "'%s' is not an integer or supported operator", token);
            free(copy);
            stack_destroy(&operands);
            return result(EXPR_INVALID_TOKEN, 0, token_number, msg);
        }

        if (trace) {
            printf("%-10s ", token);
            stack_print(&operands, stdout);
            putchar('\n');
        }
    }

    free(copy);

    if (stack_size(&operands) == 0) {
        stack_destroy(&operands);
        return result(EXPR_TOO_FEW_OPERANDS, 0, token_number, "empty expression");
    }

    if (stack_size(&operands) > 1) {
        stack_destroy(&operands);
        return result(EXPR_TOO_MANY_OPERANDS, 0, token_number, "too many operands remaining");
    }

    long final_value = 0;
    bool popped_ok = stack_pop(&operands, &final_value);
    (void)popped_ok;

    stack_destroy(&operands);
    return result(EXPR_OK, final_value, 0, "eval successful");
}